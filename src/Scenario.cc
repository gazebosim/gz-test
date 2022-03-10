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
#include <yaml-cpp/yaml.h>

#include <sdf/Model.hh>
#include <sdf/Root.hh>
#include <sdf/World.hh>

#include <ignition/fuel_tools/Interface.hh>
#include <ignition/gazebo/Util.hh>
#include <ignition/gazebo/Server.hh>

#include <ignition/transport/Node.hh>

#include "websocket_server/WebsocketServer.hh"
#include "Test.hh"
#include "Util.hh"
#include "TimeTrigger.hh"
#include "Scenario.hh"

using namespace ignition;
using namespace test;

class Scenario::Implementation
{
  public: Implementation() = default;

  public: Implementation(const Scenario::Implementation &_impl)
          {
            *this = _impl;
          }

  public: Implementation &operator=(const Implementation &_impl)
          {
            this->name = _impl.name;
            this->description = _impl.description;
            this->worldFilename = _impl.worldFilename;
            this->models = _impl.models;
            this->tests = _impl.tests;
            this->serverConfig = _impl.serverConfig;

            return *this;
          }

  /// \brief Load the scenario configuration YAML section.
  /// \param[in] _config The YAML node holding the configuration data.
  public: void LoadConfiguration(const YAML::Node &_config);

  public: std::string name{""};
  public: std::string description{""};
  public: std::string worldFilename{""};
  public: std::vector<sdf::Model> models;
  public: std::vector<std::shared_ptr<Test>> tests;
  public: gazebo::ServerConfig serverConfig;

  public: std::string baseLogPath{""};
  public: bool recordSimState = true;

  /// \brief Ignition Transport node.
  public: transport::Node node;

  public: transport::Node::Publisher statusPub;

  public: std::unique_ptr<WebsocketServer> websocketServer;
};

/////////////////////////////////////////////////
void Scenario::Implementation::LoadConfiguration(const YAML::Node &_config)
{
  // Get the world file name
  if (_config["world"])
    this->worldFilename = _config["world"].as<std::string>();

  // Load all the models, if any
  if (_config["models"])
  {
    for (YAML::const_iterator it = _config["models"].begin();
        it!=_config["models"].end(); ++it)
    {
      sdf::Model model;

      // Get the model's URI
      if ((*it)["uri"])
      {
        std::string uri = (*it)["uri"].as<std::string>();
        std::string modelPath = fuel_tools::fetchResource(uri);
        std::string modelSdfFile = fuel_tools::sdfFromPath(modelPath);

        sdf::Root root;
        sdf::Errors errors = root.Load(modelSdfFile);

        for (sdf::Error &err : errors)
          std::cout << err.Message() << std::endl;

        model = *(root.Model());
        model.SetUri(uri);
      }
      else
      {
        ignerr << "model missing URI, skipping\n";
        continue;
      }

      // Get the model's name
      if ((*it)["name"])
        model.SetName((*it)["name"].as<std::string>());
      else
      {
        ignerr << "model missing name, skipping\n";
        continue;
      }

      // Get the model's pose
      if ((*it)["pose"])
      {
        /*math::Pose3d pose;
        if ((*it)["pose"]["x"])
          pose.SetX((*it)["pose"]["x"].as<double>());
        if ((*it)["pose"]["y"])
          pose.SetY((*it)["pose"]["y"].as<double>());
        if ((*it)["pose"]["z"])
          pose.SetZ((*it)["pose"]["z"].as<double>());

        math::Vector3d rot = pose.Rot().Euler();

        if ((*it)["pose"]["roll"])
          rot.X((*it)["pose"]["roll"].as<double>());
        if ((*it)["pose"]["pitch"])
          rot.Y((*it)["pose"]["pitch"].as<double>());
        if ((*it)["pose"]["yaw"])
          rot.Z((*it)["pose"]["yaw"].as<double>());

        pose.Rot().Euler(rot);
          */
        model.SetRawPose(yamlParsePose3d((*it)["pose"]));
      }

      this->models.push_back(model);
    }
  }

  // Read log record configuration, if present.
  if (_config["record"])
  {
    YAML::Node recordNode = _config["record"];
    if (recordNode["sim-state"])
      this->recordSimState = recordNode["sim-state"].as<bool>();
  }
}

/////////////////////////////////////////////////
Scenario::Scenario()
  : dataPtr(utils::MakeImpl<Implementation>())
{
}

/////////////////////////////////////////////////
bool Scenario::Load(const std::string &_filename,
    const std::string &_outputPath)
{
  this->dataPtr->baseLogPath = _outputPath;
  if (!common::isDirectory(this->dataPtr->baseLogPath))
    common::createDirectory(this->dataPtr->baseLogPath);

  // Try to parse the scenario file.
  YAML::Node config;
  try
  {
    config = YAML::LoadFile(_filename);
  }
  catch (YAML::ParserException &_e)
  {
    ignerr << "Invalid scenario format. Error at line "
      << _e.mark.line + 1 << ", column " << _e.mark.column + 1 << ": "
      << _e.msg << std::endl;
    return false;
  }

  // The scenario name
  if (config["name"])
    this->dataPtr->name = config["name"].as<std::string>();

  // The scenario description
  if (config["description"])
    this->dataPtr->description = config["description"].as<std::string>();

  // Load the configuration section of the scenario
  if (config["configuration"])
    this->dataPtr->LoadConfiguration(config["configuration"]);

  // Load all the tests
  for (YAML::const_iterator it = config["tests"].begin();
       it != config["tests"].end(); ++it)
  {
    std::shared_ptr<Test> test = std::make_shared<Test>();
    test->Load(*it);
    this->dataPtr->tests.push_back(std::move(test));
  }

  std::string worldFilePath = gazebo::resolveSdfWorldFile(
      this->dataPtr->worldFilename);
  if (worldFilePath.empty())
  {
    ignerr << "Unable to find world file ["
      << this->dataPtr->worldFilename << "]" << std::endl;
    return false;
  }

  sdf::Root root;
  sdf::Errors errors = root.Load(worldFilePath);
  if (!errors.empty())
  {
    ignerr << "Failed to load SDF world file[" << worldFilePath << "]\n";
    for (const sdf::Error err : errors)
      ignerr << err.Message() << std::endl;

    return false;
  }

  // Add the models to the world.
  for (sdf::Model model : this->dataPtr->models)
    root.WorldByIndex(0)->AddModel(model);

  this->dataPtr->serverConfig.SetSdfRoot(root);
  this->dataPtr->serverConfig.SetHeadlessRendering(true);

  this->dataPtr->statusPub =
    this->dataPtr->node.Advertise<ignition::msgs::StringMsg>("/test/status");

  this->dataPtr->websocketServer = std::make_unique<WebsocketServer>();
  this->dataPtr->websocketServer->Load();

  return true;
}

//////////////////////////////////////////////////
void Scenario::Run()
{
  std::pair<int64_t, int64_t> timePair;

  ignition::test::msgs::TestResults result;
  result.set_scenario_name(this->Name());
  result.set_description(this->Description());
  auto scenarioStartTime = std::chrono::steady_clock::now();

  this->dataPtr->websocketServer->RunNonBlocking();

  // Run each test
  for (std::shared_ptr<Test> test : this->dataPtr->tests)
  {
    auto startTime = std::chrono::steady_clock::now();

    igndbg << "Running Test[" << test->Name() << "]\n";

    if (!this->dataPtr->baseLogPath.empty())
    {
      this->dataPtr->serverConfig.SetUseLogRecord(
          this->dataPtr->recordSimState);
      this->dataPtr->serverConfig.SetLogRecordPath(
          common::joinPaths(this->dataPtr->baseLogPath, test->Name()));
    }
    else
    {
      this->dataPtr->serverConfig.SetUseLogRecord(false);
    }

    std::unique_ptr<gazebo::Server> server =
      std::make_unique<gazebo::Server>(this->dataPtr->serverConfig);

    server->AddSystem(test);

    server->Run(true, 20000, false);
    auto endTime = std::chrono::steady_clock::now();
    auto duration = endTime - startTime;

    test::msgs::Test *testResult = result.add_tests();
    timePair = math::timePointToSecNsec(startTime);
    testResult->mutable_start_time()->set_seconds(timePair.first);
    testResult->mutable_start_time()->set_nanos(timePair.second);

    timePair = math::durationToSecNsec(duration);
    testResult->mutable_duration()->set_seconds(timePair.first);
    testResult->mutable_duration()->set_nanos(timePair.second);

    test->FillResults(testResult);
  }

  auto scenarioEndTime = std::chrono::steady_clock::now();
  auto scenarioDuration = scenarioEndTime - scenarioStartTime;

  timePair = math::timePointToSecNsec(scenarioStartTime);
  result.mutable_start_time()->set_seconds(timePair.first);
  result.mutable_start_time()->set_nanos(timePair.second);

  timePair = math::durationToSecNsec(scenarioDuration);
  result.mutable_duration()->set_seconds(timePair.first);
  result.mutable_duration()->set_nanos(timePair.second);

  if (!this->dataPtr->baseLogPath.empty() &&
      common::isDirectory(this->dataPtr->baseLogPath))
  {
    std::string resultFilename =
      common::joinPaths(this->dataPtr->baseLogPath, "result.pbtxt");

    std::ofstream stream;
    stream.open(resultFilename, std::ofstream::out);
    stream << result.DebugString() << std::endl;
  }
  else
  {
    std::cout << result.DebugString() << std::endl;
  }

}

//////////////////////////////////////////////////
void Scenario::SendRecordingCompleteMessage()
{
  ignition::msgs::StringMsg completeMsg;
  completeMsg.set_data("recording_complete");
  this->dataPtr->statusPub.Publish(completeMsg);
}

//////////////////////////////////////////////////
void Scenario::SendFinishedMessage()
{
  ignition::msgs::StringMsg completeMsg;
  completeMsg.set_data("finished");
  this->dataPtr->statusPub.Publish(completeMsg);
}

//////////////////////////////////////////////////
std::string Scenario::Name() const
{
  return this->dataPtr->name;
}

//////////////////////////////////////////////////
void Scenario::SetName(const std::string &_name)
{
  this->dataPtr->name = _name;
}

//////////////////////////////////////////////////
std::string Scenario::Description() const
{
  return this->dataPtr->description;
}

//////////////////////////////////////////////////
void Scenario::SetDescription(const std::string &_description)
{
  this->dataPtr->description = _description;
}

//////////////////////////////////////////////////
std::string Scenario::WorldFilename() const
{
  return this->dataPtr->worldFilename;
}

//////////////////////////////////////////////////
void Scenario::SetWorldFilename(const std::string &_worldFilename)
{
  this->dataPtr->worldFilename = _worldFilename;
}

//////////////////////////////////////////////////
std::vector<sdf::Model> Scenario::Models() const
{
  return this->dataPtr->models;
}

//////////////////////////////////////////////////
void Scenario::SetModels(const std::vector<sdf::Model> &_models)
{
  this->dataPtr->models = _models;
}

//////////////////////////////////////////////////
std::vector<std::shared_ptr<Test>> Scenario::Tests() const
{
  return this->dataPtr->tests;
}

//////////////////////////////////////////////////
void Scenario::SetTests(const std::vector<std::shared_ptr<Test>> &_tests)
{
  this->dataPtr->tests = _tests;
}

//////////////////////////////////////////////////
gazebo::ServerConfig Scenario::ServerConfig() const
{
  return this->dataPtr->serverConfig;
}

//////////////////////////////////////////////////
void Scenario::SetServerConfig(const gazebo::ServerConfig &_config)
{
  this->dataPtr->serverConfig = _config;
}
