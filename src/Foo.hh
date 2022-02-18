/*
 * Copyright (C) 2020 Open Source Robotics Foundation
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
#ifndef IGNITION_TEST_FUNCTIONS_HH_
#define IGNITION_TEST_FUNCTIONS_HH_

#include "ignition/test/config.hh"
#include "ignition/test/Export.hh"

namespace ignition
{
/// \brief Classes and function useful in robot applications.
namespace test
{
// Inline bracket to help doxygen filtering.
inline namespace IGNITION_TEST_VERSION_NAMESPACE {
  /// \brief foo
  /// \todo(anyone) Remove when/if ign-test has real symbols
  ///
  /// This exists purely to guarantee that the core component is generated
  /// via ign-cmake.  It may be removed once either ign-cmake has support
  /// for a header-only core component, or ign-test is no longer header-only.
  ///
  /// This is in now way part of the API and can be removed at anytime
  void IGNITION_TEST_VISIBLE foo();
}
}
}
#endif
