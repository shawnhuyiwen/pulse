
<center>
<img src="./Images/PulseLogo.png" alt="Pulse Physiology Engine">
</center>


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

## What's new in Pulse v2.0 (Jan 20th, 2018)

The latest code base includes the following notable updates:

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



(Interested in a previous @ref version?)

- - -

## Planned Software Improvements

- Pulse asset in the Unity Asset Store
- Pediatric physiology prototype
- Respiratory fatigue and chronic condition exacerbation
- Surface area and efficiency parameters for the gas diffusion model
- A more accurate pneumothorax model
- A second order baroreceptor model
- Modularity improvements for system/model/circuit swapping
- Official contribution plan for merge requests for methodology/model changes


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

