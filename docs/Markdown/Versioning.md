Version {#version}
=======

We have taken the following approach to versioning our products : Major.Minor.Patch-ReleaseStage

Our version number sematic is MAJOR.MINOR.PATCH, where :
- MAJOR changes when we make incompatible API changes,
- MINOR changes when we add functionality in a backwards-compatible manner, and
- PATCH changes when we make backwards-compatible bug fixes.
- Release Stage - We have extended this versioning with a <a href="http://en.wikipedia.org/wiki/Software_release_life_cycle">release stage</a>

This versioning follows the <a href="http://semver.org">Semantic Versioning 2.0.0</a> format.

## Pulse Version History

### Pulse v1.1 (June 25th, 2018)

The latest code base includes the following notable updates:
- Updated Wiki for using Pulse in your application
- Added C# example interface
- Refactored respiratory system for improvements determining the transition between inhale and exhale
  - Important for determining correct vital signs (e.g., respiration rate, tidal volume, end tidal CO2)
  - Most noticeable improvement is with external sources, such as a ventilator
- Added aerosols (e.g., albuterol) to the mechanical ventilator
- Added the ability to remove the cardiac arrest action and transition from asystole to normal sinus
- Added norepinephrine to the drug library
- Switched the included logger from log4cpp to log4cplus
- Updated to the latest versions of Eigen and Protobufs
- Transitioned results file extension from .txt to .csv
- Exposed action and condition information via the API
- Improved exception handling
- Discretized verification data sets for easier management
- Cleaned up headers
- CMake build improvements
  - Pulse builds and runs on single board pc's and all major operating systems

### Pulse v1.0 (September 1st, 2017) included the following notable updates:
- Fixed multi-platform compiling bugs
- Moved from an in-source to out-of-source build
  - src tree is treated as read only
  - See a description <a href="https://cmake.org/Wiki/CMake_FAQ#What_is_an_.22out-of-source.22_build.3F">here</a>
- Full CMake Builds
  - Replaced all ant and scripts (.sh and .bat) with CMake
    - Improves build support across all target platforms
      - Currently supporting Windows, Mac, UNIX (including AArch64) devices
    - More multi-platform/compiler compliance
      - Currently supporting MSVC (2015+), GCC (4.8.1+), and Clang (3.3+)
      - Supports the Ninja build system 
  - Created a superbuild
    - Build scripts will download and build all dependent 3rd party libraries - removes the libs from source pool
    - Turnkey build process
- Converted reporting from emailing to write html reports to the test directory
  - Removes SMTP server requirement
- Moved the verification data sets (very large) from source repository to a data server integrated with CMake
- Updates to ensure no 3rd party software license compliance issues for certain commercial applications
- Conversion of the data model from XML to Google Protocol Buffers