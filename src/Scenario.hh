#include <ignition/utils/ImplPtr.hh>

#include <ignition/gazebo/ServerConfig.hh>

namespace ignition
{
  namespace test
  {
  class Scenario
  {
    /// \brief Default constructor.
    public: Scenario();

    /// \brief Load a scenario file.
    /// \param[in] _filename The scenario filename
    /// \return True if the file was loaded successfully.
    public: bool Load(const std::string &_filename);

    /// \brief Get a copy of the SDF Root object that was generated during
    /// Load.
    /// \return A copy of the SDF Root object.
    public: gazebo::ServerConfig ServerConfig() const;

    /// \brief Execute the loaded scenario.
    public: void Run();

    /// \brief Private data pointer.
    IGN_UTILS_IMPL_PTR(dataPtr)
  };
  }
}
