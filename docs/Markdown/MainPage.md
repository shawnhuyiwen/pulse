
<img src="./Images/PulseLogo.png" alt="Pulse Physiology Engine">

Distributed under the <a href="https://www.apache.org/licenses/LICENSE-2.0">Apache License, Version 2.0.</a>
See accompanying <a href="https://gitlab.kitware.com/physiology/engine/blob/master/NOTICE">NOTICE</a> file for details.

For the latest new visit our <a href="https://blog.kitware.com/tag/pulse/">blog</a>

There are @ref published on several systems and clinical scenarios. 

You can get Pulse from the <a href="https://gitlab.kitware.com/physiology/engine">GitLab repository</a>.

Use this <a href="https://discourse.kitware.com/c/pulse-physiology-engine">discourse channel</a> 
to ask or share anything and everything about building, using, or understanding the Pulse engine.

Visit and post an <a href="https://gitlab.kitware.com/physiology/engine/issues">issue</a> at the repository if you have any problems with the codebase.

If you have any other questions or concerns, email: kitware@kitware.com

And welcome to our community!!


@insert ./docs/markdown/MainPageFAQ.md

## Have more questions?

Check out the @ref ExtraFAQ for more insight.


- - -

## What's new in Pulse v1.1 (May 17th, 2017)?

The latest code base includes the following notable updates:
- Refactored respiratory system for improvements determining the transition between inhale and exhale
  - Important for determining correct vital signs (e.g., respiration rate, tidal volume, end tidal CO2)
  - Most noticeable improvement is with external sources, such as a ventilator
- Added aerosols (e.g., albuterol) to the mechanical ventilator
- Added the ability to remove the cardiac arrest action and transition from asystole to normal sinus
- Added norepinephrine to the drug library
- Switched the included logger from log4cpp to log4cplus
- Transitioned results file extension from .txt to .csv
- Exposed action and condition information via the API
- Improved exception handling
- Discretized verification data sets for easier management
- Cleaned up headers
- CMake build improvements
- Added a rudimentary C# interface

(Interested in a previous @ref version?)

- - -

## Planned Software Improvements

- Integration with CTest
- Public Continuous Integration Server and CDash Server for improved verification and validation
- Offical contribution plan for pulling/merging methodology changes
- Modularity improvements for model swapping

- - -
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


@page errors Errors

@page events Events

