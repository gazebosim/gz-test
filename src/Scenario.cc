#include <yaml-cpp/yaml.h>

#include <sdf/Model.hh>
#include <sdf/Root.hh>
#include <sdf/World.hh>

#include <ignition/fuel_tools/Interface.hh>
#include <ignition/gazebo/Util.hh>
#include <ignition/gazebo/Server.hh>

#include "TimeTrigger.hh"
#include "Scenario.hh"

using namespace ignition;
using namespace test;

class Scenario::Implementation
{
  public: gazebo::ServerConfig serverConfig;
};


/////////////////////////////////////////////////
Scenario::Scenario()
  : dataPtr(utils::MakeImpl<Implementation>())
{
}

/////////////////////////////////////////////////
bool Scenario::Load(const std::string &_filename)
{
  // Try to parse the scenario file.
  YAML::Node config;
  try
  {
    config = YAML::LoadFile(_filename);
  }
  catch (YAML::ParserException &_e)
  {
    std::cerr << "Invalid scenario format, with error: " << _e.msg << "\n";
    return false;
  }

  std::string worldFilename = "";

  // if (config["name"])
  //   std::cout << "Name: " << config["name"].as<std::string>() << "\n";

  // if (config["description"])
  //   std::cout << "Description: " << config["description"].as<std::string>() << "\n";

  std::vector<sdf::Model> models;
  // Get the world file
  if (config["configuration"])
  {
    YAML::Node configNode = config["configuration"];
    if (configNode["world"])
      worldFilename = configNode["world"].as<std::string>();
    if (configNode["models"])
    {
      for (YAML::const_iterator it = configNode["models"].begin();
          it!=configNode["models"].end(); ++it)
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
          math::Pose3d pose;
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
          model.SetRawPose(pose);
        }

        models.push_back(model);
      }
    }

    // Read log record configuration, if present.
    if (configNode["record"])
    {
      YAML::Node recordNode = configNode["record"];
      this->dataPtr->serverConfig.SetUseLogRecord(true);
      if (recordNode["path"])
      {
        this->dataPtr->serverConfig.SetLogRecordPath(
            recordNode["path"].as<std::string>());
      }
    }
  }

  std::string worldFilePath = gazebo::resolveSdfWorldFile(worldFilename);
  if (worldFilePath.empty())
  {
    ignerr << "Unable to find world file[" << worldFilename << "]\n";
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

  for (sdf::Model model : models)
    root.WorldByIndex(0)->AddModel(model);

  this->dataPtr->serverConfig.SetSdfRoot(root);

  return true;
}

//////////////////////////////////////////////////
gazebo::ServerConfig Scenario::ServerConfig() const
{
  return this->dataPtr->serverConfig;
}

//////////////////////////////////////////////////
void Scenario::Run()
{
  auto server = std::make_unique<gazebo::Server>(this->dataPtr->serverConfig);

  using namespace std::chrono_literals;
  std::shared_ptr<TimeTrigger> trigger = std::make_shared<TimeTrigger>(2s);
  server->AddSystem(trigger);
  server->Run(true, 20000, false);
}
