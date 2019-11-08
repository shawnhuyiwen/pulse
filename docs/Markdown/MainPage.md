
<center>
<img src="./Images/PulseLogo.png" alt="Pulse Physiology Engine">
</center>

Distributed under the <a href="https://www.apache.org/licenses/LICENSE-2.0">Apache License, Version 2.0.</a>
See accompanying <a href="https://gitlab.kitware.com/physiology/engine/blob/master/NOTICE">NOTICE</a> file for details.

Pulse @ref version is available from our <a href="https://gitlab.kitware.com/physiology/engine">GitLab repository</a>.

<a href="http://eepurl.com/gj8Mrf">Subscribe</a> to the newsletter for the most up to date release and feature announcements.

For the latest news visit our <a href="https://blog.kitware.com/tag/pulse/">blog</a>

Use this <a href="https://discourse.kitware.com/c/pulse-physiology-engine">discourse channel</a> 
to discuss anything and everything about building, using, or understanding the Pulse engine.

Visit and post an <a href="https://gitlab.kitware.com/physiology/engine/issues">issue</a> at the repository if you have any problems with the codebase.

If you have any other questions or concerns, email: kitware@kitware.com

## What is the Pulse Physiology Engine?

The Pulse Physiology Engine is a C++ based, open source, multi-platform (Windows, Mac, and Linux), comprehensive human physiology
simulator that will drive medical education, research, and training technologies. 
The engine enables accurate and consistent physiology simulation across the medical community. 
The engine can be used as a standalone application or integrated with simulators, sensor interfaces, and models of all fidelities.
We are advancing the engine and exploring new avenues of research, such as pediatrics, patient-specific modeling and virtual surgery planning/rehearsal.

There are @ref published on several systems and clinical scenarios.  

<center>
<img src="./Images/PulseRunner.png" width="550" align="middle">
</center>

## What kind of data can I get from the physiology engine?

An instance of an engine models a single patient's physiology.
- The patient is defined by parameters, such as height, weight, systolic and diastolic pressure.
- You can initialize the patient with specific chronic and/or disease states via conditions.
- You can modify the patients external environmental conditions (weather, submerge in water, etc.)
- You can apply various actions (acute insults/injuries, interventions, conscious breathing, exercise, etc.) to be applied to the patient.
- The patient can also interact with equipment models, such as an Anesthesia and/or an %ECG Machine as well as an %Inhaler via actions.
- You can integrate the engine into your applications.

Available data is defined within the engine in three major ways:
-	System data, such as %Cardiovascular, %Respiratory, etc.
	-	Analogous to system vitals
	  -	Examples: heart rate, oxygen consumption, mean arterial pressure, etc.
-	Compartment data
	-	Flow, pressure, and volume related to specific region of the body or component of equipment
	  - Examples: Cerebral Blood Flow, Right Lung Volume, Right Heart Pressure
	- Substance specific data related to a specific part of the body or component of equipment
	  -	Examples: The Extracellular concentration of succinylcholine in the brain tissue, anesthesia machine gas inlet oxygen volume fraction
-	Assessments
	-	Formed at the level of a clinician's report, Intended to mimic test results
	  -	Example: Pulmonary Function Test

Want to learn more about @ref techdeets?

Check out the @ref FAQ for more about Pulse.
- - -

@insert ./docs/markdown/MainPageFeatures.md

