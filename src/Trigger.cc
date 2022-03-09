/*
 * Copyright (C) 2022 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef _WIN32
  #include <semaphore.h>
  #include <sys/stat.h>
  #include <sys/wait.h>
  #include <unistd.h>
#else
  #include <process.h>
  /* Needed for std::min */
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
  #include <Processthreadsapi.h>
#endif

#include "ignition/gazebo/Entity.hh"
#include "ignition/gazebo/Model.hh"
#include "ignition/gazebo/components/Pose.hh"
#include "Trigger.hh"

using namespace ignition;
using namespace test;

/////////////////////////////////////////////////
Trigger::Trigger()
{
}

//////////////////////////////////////////////////
bool Trigger::Load(const YAML::Node &_node)
{
  // Load the on commands
  if (_node["on"])
    this->LoadOnCommands(_node["on"]);

  return false;
}


//////////////////////////////////////////////////
bool Trigger::LoadOnCommands(const YAML::Node &_node)
{
  if (!_node.IsSequence())
    return false;

  // Iterate over the sequence of commands.
  for (YAML::const_iterator it = _node.begin(); it!=_node.end(); ++it)
  {
    if ((*it)["run"])
    {
      std::string cmd = (*it)["run"].as<std::string>();
      this->commands.push_back(cmd);
    }
    else if ((*it)["expect"])
    {
      std::string expectation = (*it)["expect"].as<std::string>();
      this->expectations.push_back(expectation);
    }
  }
  return true;
}

//////////////////////////////////////////////////
bool Trigger::CheckExpectations(const gazebo::World &_world,
    const gazebo::EntityComponentManager &_ecm)
{
  for (const std::string &exp : this->expectations)
  {
    std::cout << "Run Expectation[" << exp << "]\n";
    if (exp == "${{model.x1-a.pose.x == 0.0}}")
    {
      std::string modelName = "x1-a";
      if (_world.ModelByName(_ecm, modelName))
      {
        gazebo::Model model(_world.ModelByName(_ecm, modelName));
        auto pose = _ecm.ComponentData<gazebo::components::Pose>(model.Entity());
        if (pose && !math::equal(pose->Pos().X(), 0.0))
        {
          std::cout << "Model Name[" << model.Name(_ecm) << "] pose[" << *pose << "]\n";
          return false;
        }
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////
bool Trigger::RunOnCommands(const gazebo::World &_world,
    const gazebo::EntityComponentManager &_ecm)
{
  if (!this->CheckExpectations(_world, _ecm))
    return false;

  for (const std::string &cmd : this->commands)
  {
    std::vector<std::string> parts = common::split(cmd, " ");
    this->RunExecutable(parts);
  }

  return true;
}

//////////////////////////////////////////////////
std::string Trigger::Name() const
{
  return this->name;
}

//////////////////////////////////////////////////
void Trigger::SetName(const std::string &_name)
{
  this->name = _name;
}

//////////////////////////////////////////////////
Trigger::TriggerType Trigger::Type() const
{
  return this->type;
}

//////////////////////////////////////////////////
void Trigger::SetType(const TriggerType &_type)
{
  this->type = _type;
}

/////////////////////////////////////////////////
bool Trigger::RunExecutable(const std::vector<std::string> &_cmd)
{
  // Check for empty
  if (_cmd.empty())
  {
    ignerr << "Empty command.\n";
    return false;
  }

#ifdef _WIN32
  typedef struct MyData {
      std::vector<std::string> _cmd;
      HANDLE stoppedChildSem;
  } MYDATA, *PMYDATA;


  PMYDATA pDataArray = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(MYDATA));
  if (pDataArray == nullptr)
  {
    ignerr << "allocation fails " << GetLastErrorAsString() << '\n';
    return false;
  }

  for (auto & cmd : _cmd){
    pDataArray->_cmd.push_back(cmd);
  }

  pDataArray->stoppedChildSem = this->stoppedChildSem;

  auto dontThreadOnMe = [](LPVOID lpParam) -> DWORD {
    PMYDATA pDataArray;
    pDataArray = (PMYDATA)lpParam;

    // Create a vector of char* in the child process
    std::vector<char*> cstrings;
    cstrings.push_back("C:\\WINDOWS\\SYSTEM32\\CMD.EXE");
    cstrings.push_back("cmd.exe ");
    cstrings.push_back("/c");
    for (const std::string &part : pDataArray->_cmd)
    {
      cstrings.push_back(const_cast<char *>(part.c_str()));
    }

    // Add the nullptr termination.
    cstrings.push_back(nullptr);

    // Run the command, replacing the current process image
    if (_spawnv(_P_WAIT , cstrings[0], &cstrings[0]) < 0)
    {
      ignerr << "Unable to run command["
        << std::accumulate(
            pDataArray->_cmd.begin(),
            pDataArray->_cmd.end(),
            std::string(""))
        << "] " << GetLastErrorAsString() << "\n";
      return -1;
    }

    if (!ReleaseSemaphore(pDataArray->stoppedChildSem, 1, nullptr))
    {
      ignerr << "Error Releasing Semaphore "
             << GetLastErrorAsString() << std::endl;
    }

    if(pDataArray != NULL)
    {
      HeapFree(GetProcessHeap(), 0, pDataArray);
      pDataArray = NULL;    // Ensure address is not reused.
    }

    return 0;
  };

  auto thread = CreateThread(
    nullptr, 0, dontThreadOnMe, pDataArray, 0, nullptr);

  if (thread == nullptr) {
    ignerr << "Error creating thread on Windows "
           << GetLastErrorAsString() << '\n';
  }
  else
  {
    // std::lock_guard<std::mutex> mutex(this->executablesMutex);

    // Store the PID in the parent process.
    // this->executables.push_back(Executable(thread, _cmd));
  }
#else
  // Fork a process for the command
  pid_t pid = fork();

  // If parent process...
  if (pid)
  {
    igndbg << "Forked a process for command["
      << std::accumulate(_cmd.begin(), _cmd.end(), std::string("")) << "]\n"
      << std::flush;

    // std::lock_guard<std::mutex> mutex(this->executablesMutex);
    // Store the PID in the parent process.
    // this->executables.push_back(Executable(pid, _cmd));
  }
  // Else child process...
  else
  {
    // A child is not the master
    // this->master = false;

    // Create a vector of char* in the child process
    std::vector<char*> cstrings;
    for (const std::string &part : _cmd)
    {
      cstrings.push_back(const_cast<char *>(part.c_str()));
    }

    // Add the nullptr termination.
    cstrings.push_back(nullptr);

    // Remove from foreground process group.
    setpgid(0, 0);

    // Run the command, replacing the current process image
    if (execvp(cstrings[0], &cstrings[0]) < 0)
    {
      ignerr << "Unable to run command["
             << std::accumulate(
                _cmd.begin(),
                _cmd.end(),
                std::string("")) << "]\n";
      return false;
    }
  }
#endif
  return true;
}

//////////////////////////////////////////////////
void Trigger::SetResult(bool _passed)
{
  this->result = _passed;
}

//////////////////////////////////////////////////
std::optional<bool> Trigger::Result() const
{
  return this->result;
}
