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