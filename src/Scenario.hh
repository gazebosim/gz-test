#ifndef IGNITION_TEST_SCENARIO_HH_
#define IGNITION_TEST_SCENARIO_HH_

#include <ignition/utils/ImplPtr.hh>
#include <ignition/gazebo/ServerConfig.hh>

#include "Test.hh"

namespace ignition
{
  namespace test
  {
  class Scenario
  {
    /// \brief Default constructor.
    public: Scenario();

    /// \brief Copy constructor.
    /// \param[in] _scenario Scenario to copy;
    // public: Scenario(const Scenario &_scenario);

    /// \brief Move constructor.
    /// \param[in] _scenario Scenario to move;
    // public: Scenario(Scenario &&_scenario) noexcept;

    /// \brief Load a scenario file.
    /// \param[in] _filename The scenario filename
    /// \return True if the file was loaded successfully.
    public: bool Load(const std::string &_filename);


    /// \brief Execute the loaded scenario.
    public: void Run();

    public: std::string Name() const;
    public: void SetName(const std::string &_name);

    public: std::string Description() const;
    public: void SetDescription(const std::string &_description);

    public: std::string WorldFilename() const;
    public: void SetWorldFilename(const std::string &_worldFilename);

    public: std::vector<sdf::Model> Models() const;
    public: void SetModels(const std::vector<sdf::Model> &_model);

    public: std::vector<Test> Tests() const;
    public: void SetTests(const std::vector<Test> &_tests);

    /// \brief Get a copy of the SDF Root object that was generated during
    /// Load.
    /// \return A copy of the SDF Root object.
    public: gazebo::ServerConfig ServerConfig() const;
    public: void SetServerConfig(const gazebo::ServerConfig &_config);

    /*public: Scenario &operator=(const Scenario &_scenario);

    public: Scenario &operator=(Scenario &&_scenario) noexcept;
    */

    /// \brief Private data pointer.
    IGN_UTILS_IMPL_PTR(dataPtr)
  };
  }
}
#endif
