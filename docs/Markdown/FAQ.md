FAQ {#FAQ}
============

<b>Is the physiology engine a game?</b><br>
No, it is an series of mathematical models (engine) that can power immersive 
learning and serious games for medical training. The 
engine can provide a realistic training experience by producing real-time 
results to trauma and treatment. A physiology engine can enhance the user experience of applications 
by providing a comprehensive physiological response to insults and interventions.

<b>What is the long-term plan for the physiology engine?</b><br>
Our team's goal is to first and foremost develop the most advanced, 
open source, whole-body engine created to date. We plan to continue advancing physiologic models and incorporating state of the art models into the physiology engine to ensure we remain at the forefront of physiologic modeling research. We also plan to continue working with the user community to ensure 
the engine becomes the standard in physiology modeling by integrating it into a variety of applications.

Our system architecture was developed in a way that will make 
the system easy to extend for new models and external interfaces. The Apache 2.0 license 
allows for both open-source and proprietary applications to promote 
widespread use across government, military, academic, and commercial markets.


<b>What is the fidelity of the physiology engine?</b><br>
One definition of fidelity is "The degree to which a model or simulation represents the state and 
behavior of a real world object or the perception of a real world object, feature, condition, or chosen 
standard in a measurable or perceivable manner; a measure of the realism of a model or simulation @cite msco . " 
The validation documentation (in the @ref SystemMethodology reports) describes how well the engine 
reproduces physiology at the system level. Like the human body, the engine is a self-compensating system of 
physiological systems with outcomes based on interventions @cite pettitt2009task , and therefore can be considered high-fidelity.

Sometimes the word fidelity is used to refer to the spatial (anatomical) level of resolution of a model. 
The engine is a closed loop 
total body physiology model that combines physics-based lumped-parameter models 
and control system feedback mechanisms to model real-time system-level 
physiologic behaviors. Spatial resolution is limited by the lumped-parameter approach 
to sections of organs (what may arguably be referred to as the tissue level). However, the engine
uses an extensible architecture to promote integration with external models with varying levels of 
fidelity (resolution or granularity). For more details, please see the recorded [Committee on Credible Practice of Modeling & Simulation 
in Healthcare](https://simtk.org/projects/cpms/ "CPMS") webinar.


<b>What kind of uncertainty quantification do you do perform in your physiology model?</b><br>
We have not performed a systematic forward propagation or inverse quantification of model uncertainty, 
nor have we conducted a formal sensitivity analysis. We plan to begin this type of analysis in the near future. However, we can quantify the numerical 
uncertainty introduced in solving the lumped-parameter fluid dynamics of the two foundation sub-models 
(@ref CardiovascularMethodology "Cardiovascular" and @ref RespiratoryMethodology "Respiratory"). The engine currently uses a bi-conjugate 
gradient method specific for sparse square systems (using the Eigen third party packages). This is an 
iterative method and we use the default tolerance for their solver, which is as close to zero as reasonable (around 1e-16). 


<b>Where did Pulse come from?</b><br>
The Pulse Physiology Engine started as a forked of the BioGears 6.1.1 Physiology Engine by members of its original team.

This fork was driven by several community needs, including:

1. Active, Publicly Available Repository<br>
The <a href="https://gitlab.kitware.com/physiology/engine">Pulse git repository</a> was created with the BioGears 6.1.1 code archive. 
The Pulse team actively develops on this repository with contributions from other community members. 
The public repository  provides a central location for the community to view and test active development, 
report issues and contribute to the Pulse Physiology Engine. This is a truly open and community-driven project.


2. Cross-Platform Build Process Improvements<br>
Pulse has refactored the build process to require only <a href="https://cmake.org/">CMake</a>, a cross-platform family of tools designed to build, test and package software. 
This removes the need for any platform and compiler customized build scripts, which has greatly simplified the process for 3rd party users
 to build and integrate Pulse into their applications. 
With this change, Pulse can easily be built with the GCC, Clang, MSVC, and Ninja compilers on any of our supported platforms (Windows, Mac, Linux, and AArch64).


3. License Updates<br>
The licensing goal of the engine is to maintain a permissive Apache 2.0 license (free to use with no restrictions)
to encourage use of the open-source software in academic, government and commercial products.
Several open-source libraries are used by the engine and have permissive licenses that do not affect the use of the engine in future products and applications.
However, the Code Synthesis XSD <a href="http://www.codesynthesis.com/licenses/gpl-2.txt">GNU General Public License</a> with <a href="http://www.codesynthesis.com/projects/xsd/FLOSSE">FLOSS exception</a>
created a significant open-source licensing limitation, which restricts any proprietary extensions of the physiology engine.
<br>
Any use of the engine, without modification, is exempt from the GPL license and does not need to be made public (open-source).
Any extensions to the engine must be made publicly available without cost, or a Proprietary License for Code Synthesis
 must be <a href="http://www.codesynthesis.com/contact/">purchased</a> for the product/application using the modified engine to comply with the license terms for Code Synthesis.
<br>
To resolve this license issue, Pulse has replaced the Xerces and Code Synthesis libraries with the <a href="https://developers.google.com/protocol-buffers/">Google Protocol Buffer</a> library. 
This library is licensed under <a href="https://opensource.org/licenses/BSD-3-Clause">BSD 3-clause</a> and allows anyone to modify the Pulse Physiology Engine in anyway without any requirement
 to make their project code base publicly available.

4. Improved Data Management and Test Suite Reporting<br>
The provided test suite executes scenarios and compares the generated results file to the "gold standard" data file for the same scenario. 
Pulse removed the SMTP requirement for viewing the test report, and instead generates a summary on disk that can be viewed by a user. 
This change reduces the required infrastructure needed to perform unit, verification and validation testing. 
The set of "gold standard" data files  for the physiology engine is very large (~500MB). 
Keeping these files in the git repository can easily bloat the repository. 
Pulse has addressed this issue by hosting all verification sets in <a href="https://data.kitware.com/#collection/59849c788d777f7d33e9c084/folder/598c95d88d777f7d33e9c1fa">a publicly available data server</a>, 
associating different versions to the git repository history though a collection of hash files in the git repository. 
CMake is then used to provide management between the data server and the specific code version from the git repository. 
This separation provides a repository with a small footprint to allow for quick pulling and branching, 
while maintaining a historical associations with the large data set needed for testing.



