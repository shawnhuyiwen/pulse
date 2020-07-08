Mechanical Ventilator Methodology {#MechanicalVentilatorMethodology}
==========================

@anchor ventilator-overview
# Overview
@anchor ventilator-abstract
## Abstract

The Mechanical Ventilator Model is a generic representation of a positive-pressure ventilation device and 
inhaled gas/agent administration. It models a semi-closed circuit breathing system. The current implementation is limited, but the data model is designed for future expansion.
The results of this system were evaluated for pressure control - continuous mandatory ventilation (PC-CMV) and volume control - assist control (VC-AC) ventilation modes. The results show excellent correlation with the expected values. 
Future work will add more ventilation modes.

@anchor ventilator-intro
## Introduction

### Mechanical Ventilators and Positive-Pressure Ventilation

Mechanical ventilators use mechanical means for artificial ventilation to assist or replace spontaneous breathing.  Positive pressure ventilation pushes air into the lungs through the airways.  Ventilators provide the following benefits @cite cleveland2020mechanical :
- The patient does not have to work as hard to breathe - their respiratory muscles rest.
- The patient's as allowed time to recover in hopes that breathing becomes normal again.
- Helps the patient get adequate oxygen and clears carbon dioxide.
- Preserves a stable airway and preventing injury from aspiration.

@anchor ventilator-system
# System Design

## Data Flow

### Preprocess

The Mechanical Ventilator equipment object modifies circuit and substance values during the preprocess phase.  It sets the connection to the respiratory system based on the airway mode, applies settings, calculates the instantaneous driving pressure value, and sets substance and aerosol values at the source.

### Process

The current implementation has no specific circuit or transport process
functionality for the mechanical ventilator. Mechanical Ventilator processing
is currently done in the %Respiratory System with the combined circuit
methodology.

### Postprocess

The Postprocess step moves values calculated in the Process step from the next
time step calculation to the current time step calculation. The current implementation has no 
specific post process functionality for the mechanical ventilator. All postprocessing is done in the
%Respiratory System with the combined circuit methodology.

@anchor ventilator-features
## Features and Capabilities

### The Mechanical Ventilator Circuit

The Mechanical Ventilator model consists of a pressure source with tubes and valves for inspiration and expiration.  The unidirectional valves are ideal and do not allow any backflow.  Figure 1 shows the Mechanical Ventilator circuit.  The compartments and transport graph mirrors the circuit.  Substance values are set on the Ventilator node/compartment, assuming infinite volume.

<img src="./Images/MechanicalVentilator/MechanicalVentilatorCircuit.png" width="400">
<center>
<i>Figure 1. Circuit diagram of the Mechanical Ventilator. The circuit employs a driver source (either pressure or flow, depending on the mode and settings), resistances, and valves.</i>
</center><br>

### Connecting to the %Respiratory Circuit

When an mechanical ventilator is used on a patient, there is a direct
connection that allows air to flow freely between both. In the same
fashion, the Mechanical Ventilator and %Respiratory circuits in the engine are
directly connected and allowed to share the same fluid. When the machine
is turned on, both individually defined circuits are combined into a
single circuit that is then used for calculations.

### Mechanical Ventilator Settings

The Mechanical Ventilator parameters were defined to allow for setting all types of ventilation modes - all control variable types and all breath sequences. To achieve this, these hierarchical definitions are implemented:
- Connection (Off, Mask, Tube): Connection type to the %Respiratory System
- Inspiration Phase
	- Trigger: Transition to inspiration
    - Machine Trigger: Control triggers
      - Time: Total length of expiration phase to trigger inspiration phase      
    - Patient Trigger: Assisted triggers
      - Volume: Ventilator sensor volume change value to trigger inspiration phase
      - Pressure: Ventilator sensor pressure value to trigger inspiration phase      
	- Waveform (square, exponential, ramp, sinusoidal, sigmoidal): Pattern of driver function
	- Pause: Time of plateau (i.e., constant driver pressure) between inspiration and expiration
	- Target (PIP, TV, etc.): Value to set/achieve
	- Limit: Cutoff/maximum
		- Pressure: Ventilator sensor pressure cutoff/maximum
		- Flow: Ventilator sensor flow cutoff/maximum
		- Volume: Total lung volume cutoff/maximum
- Expiration Phase
	- Cycle: Transition to expiration
		- Time: Total length of inspiration phase to trigger expiration phase
		- Pressure: Ventilator sensor pressure value to trigger expiration phase
    - Volume: Ventilator sensor volume change (i.e., TV) to trigger expiration phase
		- Flow: Ventilator sensor flow value to trigger expiration phase
	- Waveform (square, exponential, ramp, sinusoidal, sigmoidal): Pattern of driver function
	- Baseline (PEEP or FRC): Value to set/achieve
- Substances
	- Fraction of inspired gas (FiO2 and other gases fractions)
	- Concentration of inspired aerosol (albuterol, etc.)
- Circuit Resistances
	- Inspiration tube resistance: Total resistance of inspiratory limb tubing
	- Inspiration valve resistance: Total resistance of inspiratory valves
	- Expiration tube resistance: Total resistance of expiratory limb tubing
	- Expiration valve resistance: Total resistance of expiratory valves
	- Endotracheal tube resistance: Total connection resistance

While the parameter list is meant to be all encompassing for all ventilator modes, some typical settings are required to be translated by the user or application.  For example, the respiration rate (<i>RR</i>) and I:E Ratio (<i>IE</i>)settings can be translated to an inspiratory period (<i>P<sub>ins</sub></i>) and expiratory period (<i>P<sub>exp</sub></i>) by:


\f[{P_{tot}}[s] = \frac{{60}}{{RR[bpm]}}\f]
<center>
<i>Equation 1.</i>
</center><br> 

\f[{P_{ins}}[s] = \frac{{IE \cdot {P_{tot}}[s]}}{{1 + IE}}\f]
<center>
<i>Equation 2.</i>
</center><br> 

\f[{P_{exp}}[s] = {P_{tot}}[s] - {P_{ins}}[s]\f]
<center>
<i>Equation 3.</i>
</center><br> 

Where the I:E Ratio (<i>IE</i>) is defined by a fraction, for example 1:2 is 0.5 and 1:1 is 1.0.

@anchor ventilator-dependencies
### Dependencies

The Mechanical Ventilator interacts with 
the %Respiratory System
through a connection that delivers gases and aerosols into the
%Respiratory System (see @ref RespiratoryMethodology). The two 
systems are connected to each other through a path that connects the airway node of the
 %Respiratory System to the mask/endotracheal node (referred as Connection 
in the circuit diagram) of the Mechanical Ventilator System. During spontaneous ventilation, the airway node of the
%Respiratory System is connected to the atmosphere via the %Environment System. 
This serves as a ground node for the %Respiratory System. 

When the Mechanical Ventilator is connected, a network of combined circuits that
include the elements from both the %Respiratory and Ventilator Systems is
created. When the combined circuit is generated at runtime, the
ground environment node connected to the mouth node of the %Respiratory System is
replaced by the Connection node that represents the mask/endotracheal node, 
becoming one combined circuit.
Apart from such interaction, the Ventilator System is
also responsive to the flow resistances of the %Respiratory System. In
this regard, the ventilator driver pressure serves as a positive-pressure source for 
the combined circuit. The Ventilator is linked to the %Environment 
System that regulates the atmospheric/reference pressure.

@anchor ventilator-assumptions
## Assumptions and Limitations

Currently, the Mechanical Ventilator uses ideal pressure sources and one-way valves. Only setting appropriate for a PC-CMV or VC-AC modes are allowed and tested.  However, the system is defined and implemented to allow for future mode expansion without data model changes. 

@anchor ventilator-results
# Results and Conclusions

@anchor ventilator-settingsvalidation
## Validation - Settings

The Mechanical ventilator settings are fully dynamic and do not have any bounds enforced.  A scenario that varies the PC-CMV settings in several different combinations is included with the code base and produces the outputs shown in Figure 2.

<center>
<table border="0">
<tr>    
    <td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledVaried_TotalLungVolume.jpg" width="550"></td>
	<td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledVaried_RR.jpg" width="550"></td>
</tr>
<tr>    
    <td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledVaried_TidalVolume.jpg" width="550"></td>
	<td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledVaried_InspiratoryExpiratoryRatio.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledVariedLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 2. These plots show the successful implementation of varying Mechanical Ventilator settings without patient spontaneous breathing.</i></center><br>

A scenario that varies the VC-AC settings for assisted breathing for a patient with moderate ARDS produces the outputs shown in Figure 3.

<center>
<table border="0">
<tr>    
    <td><img src="./plots/MechanicalVentilator/MechanicalVentilatorVolumeControlledARDS_TotalLungVolume.jpg" width="550"></td>
	<td><img src="./plots/MechanicalVentilator/MechanicalVentilatorVolumeControlledARDS_RR.jpg" width="550"></td>
</tr>
<tr>    
    <td><img src="./plots/MechanicalVentilator/MechanicalVentilatorVolumeControlledARDS_TidalVolume.jpg" width="550"></td>
	<td><img src="./plots/MechanicalVentilator/MechanicalVentilatorVolumeControlledARDS_InspiratoryExpiratoryRatio.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/MechanicalVentilator/MechanicalVentilatorVolumeControlledARDSLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 3. These plots show the successful implementation of varying Mechanical Ventilator settings with patient spontaneous breathing.</i></center><br>

@anchor ventilator-ardsvalidation
## Validation - ARDS

The %Respiratory ARDS model with mild, moderate, and severe severities is extensively tested in this scenario.  The patient is administered a neuromuscular blockade to prevent spontaneous breathing and ideal PIP, PEEP, and FiO2 values are set in the ventilator to maintain adequate SpO2 values. Results successfully match expected empirical data and trends, as shown in table 1. Example outputs are shown in Figure 4.

<center>
<table border="0">
<tr>    
    <td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledARDS_TidalVolume.jpg" width="550"></td>
	<td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledARDS_OxygenSaturation.jpg" width="550"></td>
</tr>
<tr>    
    <td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledARDS_CarricoIndex.jpg" width="550"></td>
	<td><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledARDS_ShuntFraction.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/MechanicalVentilator/MechanicalVentilatorPressureControlledARDSLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 4. These plots show the successful implementation of a ventilated patient with varying ARDS severities.</i></center><br>

<center><br>
Table 1. Cumulative validation results for Anesthesia Machine specific conditions and actions scenarios.
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Carrico Index [PaO2/FiO2] (mmHg)	|	Shunt Fraction	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Pulmonary Compliance (L/cmH2O)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Mild ARDS (severity = 0.3)	|	Chronic condition	|		|	0	|<span class="success">	Increased @cite mortelliti2002acute	</span>|<span class="success">	>200 @cite villar2013universal	</span>|<span class="success">	2%-5% @cite Levitzky2013pulmonary	</span>|<span class="success">	Reduced	</span>|<span class="success">	Reduced (fatigue)	</span>|<span class="success">	Reduced @cite mortelliti2002acute	</span>|
|	Tracheal Intubation	|		|	30	|	60	|<span class="success">	Increased @cite mortelliti2002acute	</span>|<span class="success">	>200 @cite villar2013universal	</span>|<span class="success">	2%-5% @cite Levitzky2013pulmonary	</span>|<span class="success">	Reduced	</span>|<span class="success">	Reduced  further (artificial airway) @cite arnal2018parameters	</span>|<span class="success">	Reduced  further  (artificial airway) @cite mortelliti2002acute @cite arnal2018parameters	</span>|
|	Apnea (Dyspnea severity = 1.0); Turn on P-CMV mechanical ventilator and provide supplemental oxygen	|	Use apnea to mimic neuromuscular blockade; Ventilator settings chosen for target tidal volume and oxygen saturation	|	60	|	360	|<span class="success">	20 (ventilator setting)	</span>|<span class="success">	>200 @cite villar2013universal	</span>|<span class="success">	2%-5% @cite Levitzky2013pulmonary	</span>|<span class="success">	88%-95% @cite mortelliti2002acute (ventilator target)	</span>|<span class="success">	6 mL/kg (ideal body weight) = 450 mL @cite mortelliti2002acute (ventilator target)	</span>|<span class="success">	Reduced @cite mortelliti2002acute	</span>|
|	Moderate ARDS (severity = 0.6); Adjust ventilator settings	|	Ventilator settings chosen for target tidal volume and oxygen saturation	|	360	|	660	|<span class="success">	20 (ventilator setting)	</span>|<span class="success">	100-200 @cite villar2013universal	</span>|<span class="success">	>20% @cite radermacher2017fifty	</span>|<span class="success">	88%-95% @cite mortelliti2002acute (ventilator target)	</span>|<span class="success">	6 mL/kg (ideal body weight) = 450 mL @cite mortelliti2002acute (ventilator target)	</span>|<span class="success">	Reduced further @cite mortelliti2002acute	</span>|
|	Severe ARDS (severity = 0.9); Adjust ventilator settings	|	Ventilator settings chosen for target tidal volume and supplemental oxygen at max (1.0 fraction O2)	|	660	|	960	|<span class="success">	20 (ventilator setting)	</span>|<span class="success">	<100 @cite villar2013universal	</span>|<span class="success">	>40% @cite radermacher2017fifty	</span>|<span class="success">	Reduced (100% FiO2)	</span>|<span class="success">	6 mL/kg (ideal body weight) = 450 mL @cite mortelliti2002acute (ventilator target)	</span>|<span class="success">	Reduced further @cite mortelliti2002acute	</span>|

@anchor ventilator-conclusion
## Conclusion

While the model is a generic representation of mechanical ventilation, inhaled gases, and inhaled agent 
administration, this model represents the behavior of a complex piece of equipment that is 
associated with a difficult
medical specialty. The engine provides a whole-body approach to modeling that 
allows for simulation of this complex field. This system is a strong addition to the engine with the potential for 
future development.

@anchor ventilator-improvements
# Future Work

## Coming Soon

Logic and results for handling more ventilation modes.

## Recommended Improvements

The engine modularity could be taken advantage of to add parameters and elements 
for specific equipment models and manufacturers. 

@anchor ventilator-appendices
# Appendices

## Data Model Implementation

@ref MechanicalVentilatorTable "Mechanical Ventilator"

## Acronyms

ARDS - Acute %Respiratory Distress Syndrome
FiO2 - Fraction of Inpspired Oxygen
FRC - Functional Residual Capacity
I:E Ratio - Inspiratory-Expiratory Ratio
PaO2 - Arterial Oxygen Partial Pressure
PEEP - Positive End Expired Pressure
PIP - Peak Inspiratory Pressure
SpO2 - Oxygen Saturation

## Compartments

* Connection
* YPiece
* ExpiratoryLimb
* InspiratoryLimb
* ExpiratoryValve
* InspiratoryValve
* Ventilator
