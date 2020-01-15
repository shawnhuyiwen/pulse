Version 2.3.0 {#version}
=============

Our versioning follows the <a href="http://semver.org">Semantic Versioning 2.0.0</a> format.

Our version number sematic is Major.Minor.Patch-ReleaseStage, where :
- MAJOR changes when we make incompatible API changes,
- MINOR changes when we add functionality in a backwards-compatible manner, and
- PATCH changes when we make backwards-compatible bug fixes.
- Release Stage - We have extended this versioning with a <a href="http://en.wikipedia.org/wiki/Software_release_life_cycle">release stage</a>

- - -

## Pulse v2.3.0 (January 2020)

- Software Architecture
  - C# API Updates
    - Added Data Request support for requesting any data from the engine
    - Added support for patient creation, with or without chronic conditions
    - Added support for more actions
  - C++ API Updates
    - Created separate <i>Initial</i> and <i>Current</i> patient definitions
    - Updated general math exponential functions to be more intuitive
    - Added a <i>Multiply</i> value setting, similar to <i>Increment</i>
- Physiology Models
  - Patient lung volumes are now determined using ideal body weight rather than actual body weight
  - Significant respiratory model updates
    - Changed standard respiration rate from 16 bpm to 12 bpm to better match standards in literature and validation
    - Tweaked respiratory circuit for improved modeling
    - Refactored the respiratory muscle driver with a new waveform
    - Added a sigmoidal chest wall compliance model
    - Refactored respiratory conditions and actions for improved restrictive and obstructive disease results and combined effects
    - Added an ARDS condition
    - Added a pulmonary fibrosis condition
    - Added exacerbation action to degrade/improve respiratory conditions during simulations
    - Renamed apnea action to dyspnea    
    - Refactored conscious respiration, leading to improved spirometry curves
    - More/better validation
  - Updated anesthesia machine circuit to use pressure sources for supplying gas, rather than flow sources to avoid issues with flow source pressure calculations
  
### Congratulations

Congratulations and thank you to everyone who contributed to this release. This release, we would like to specially recognize our 3 new contributors:

  - Hongpeng Liu
  - Mattias Lantz Cronqvist
  - Anusha Muralidharan

## Planned Improvements

- Python bindings
- Black box circuit/compartment components
- Sepsis
- Hemorrhagic Shock
- Hemorrhage methodology update for better performance and usability
- Work of breathing and respiratory fatigue models
- A second order baroreceptor model
- Modularity improvements for system/model/circuit swapping
- Pediatric physiology prototype
- Official contribution plan for merge requests for methodology/model changes

@anchor known-issues
## Known Physiology Model Issues and Limitations

The following are known issues with the current version of the software:
- Lack of a full sympathetic/parasympathetic nervous system
- Extravascular fluid exchange model is incomplete
- Peripheral resistance currently does not scale with core temperature
- Only tested a simulation up to 12 hours in length (No sleep model)
- Limited Consumption model
  - Limited number of macronutrients available
  - Limited conversion and use within the engine
- Oxygen saturation drops too sharply

- - -

## Pulse v2.2.0 (August 2019)

- Software Architecture
  - CMake integration improvements
  - Improved Data Request API
  - Scenario Execution Improvements
  - Unified API for Engine Events
  - MSVC Build Sped up
- Physiology Models
  - Added the ability to provide supplemental oxygen through a nasal cannula, simple mask, and nonrebreather mask.
  - Updated vascular effects caused by respiratory pleural cavity imbalances, mainly to increased venous return resistance when the patient has a pneumothorax / collapsed lungs.

## Pulse v2.1.0 (February 2019)

- Software updates necessary for integration with Unity

## Pulse v2.0.0 (January 2019)

- Software Architecture
  - Converted ASCII file I/O to JSON (Compliant to Protobuf IDLs)
  - Updated Engine Interface to support Both ASCII and Binary serialization
  - Started a formal C# CDM Library for use in .NET applications
  - Created a C interface to Pulse for external application integration
  - Updated to the latest versions of Eigen/Protobufs/log4cplus
  - Unity integration support and examples
- Physiology Models
  - Internal Hemorrhage Support
  - External hemorrhage from specific compartments (only Vena Cava before)
  - Added support for Packed Red Blood Cells infusion
  - Added dissolved oxygen and carbon dioxide to IV fluids to resolve issues with fluid resuscitation


### Pulse v1.1.0 (June 2018)

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

### Pulse v1.0.0 (September 2017)

- Fixed multi-platform compiling bugs
- Moved from an in-source to out-of-source build
  - src tree is treated as read only
  - See a description <a href="https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#out-of-source-build-trees">here</a>
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