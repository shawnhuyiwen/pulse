Mechanical Ventilator Methodology {#MechanicalVentilatorMethodology}
==========================

@anchor ventilator-overview
# Overview
@anchor ventilator-abstract
## Abstract

The Mechanical Ventilator Model is a generic representation of a positive-pressure ventilation device and 
inhaled gas/agent administration. It models a semi-closed circuit breathing system. The results of this system were evaluated for pressure control - continuous mandatory ventilation (PC-CMV), volume control - continuous mandatory ventilation (VC-CMV), pressure control - assist control (PC-AC), volume control - assist control (VC-AC) ventilation, and continuous positive airway pressure (CPAP) modes. The results show excellent correlation with the expected values. Future work will evaluate more ventilation modes.

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
<i>Figure 1. Circuit diagram of the Mechanical Ventilator. The circuit employs a driver source (either pressure or flow, depending on the mode and settings), resistances, valves, and a compliance.</i>
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
  - Waveform Period: Time to reach maximum driver value
	- Target (PIP or flow): Driver value to set
	- Limit: Cutoff/maximum
		- Pressure: Ventilator sensor pressure cutoff/maximum
		- Flow: Ventilator sensor flow cutoff/maximum
		- Volume: Total lung volume cutoff/maximum
  - Pause: Time of plateau (i.e., constant driver pressure) between inspiration and expiration
- Expiration Phase
	- Cycle: Transition to expiration
		- Time: Total length of inspiration phase to trigger expiration phase
		- Pressure: Ventilator sensor pressure value to trigger expiration phase
    - Volume: Ventilator sensor volume change (i.e., TV) to trigger expiration phase
		- Flow: Ventilator sensor flow value to trigger expiration phase
	- Waveform (square, exponential, ramp, sinusoidal, sigmoidal): Pattern of driver function
	- Baseline (PEEP or FRC): Value to set/achieve
- Driver Damping: Fractional change parameter that prevents driver discontinuities (i.e., smoother driver curve)
- Substances
	- Fraction of inspired gas (FiO2 and other gases fractions)
	- Concentration of inspired aerosol (albuterol, etc.)
- Circuit Resistances
	- Inspiration tube resistance: Total resistance of inspiratory limb tubing
	- Inspiration valve resistance: Total resistance of inspiratory valves
	- Expiration tube resistance: Total resistance of expiratory limb tubing
	- Expiration valve resistance: Total resistance of expiratory valves
- Circuit Volumes
  - Inspiratory limb volume: Total volume of inspiratory limb tubing
	- Inspiratory valve volume: Total volume of inspiratory valves
	- Expiratory tube volume: Total volume of expiratory limb tubing
	- Expiratory valve volume: Total volume of expiratory valves
	- Y-piece volume: Total volume of the y-piece
  - Connection volume: Total volume of the mask/endotracheal tube
- Circuit Compliance: Total compliance of the ventilator circuit

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

Standard ventilator mode settings have been added to allow for more intuitive application of ventilator modes. The following mode settings are automatically translated to the lower level configuration settings that are listed previously in this section.  Some setting are not exposed (e.g., trigger flow and cycle flow) at this level and default values are used.

Pressure Control (PC-CMV and PC-AC):
- Fraction of Inspired Oxygen
- Inspiratory Period
- Inspiratory Pressure
- Positive End Expired Pressure
- Respiration Rate
- Slope

Volume Control (VC-CMV and VC-AC):
- Flow
- Fraction of Inspired Oxygen
- Inspiratory Period
- Positive End Expired Pressure
- Respiration Rate
- Tidal Volume

Continuous Positive Airway Pressure (CPAP):
- Delta Pressure Support
- Fraction of Inspired Oxygen
- Positive End Expired Pressure
- Slope

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

Currently, the Mechanical Ventilator uses ideal pressure sources and one-way valves. Only setting appropriate for a PC-CMV, VC-CMV, and VC-AC modes are allowed and tested.  However, the system is defined and implemented to allow for future mode expansion without data model changes. 

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
## Validation - Healthy vs. ARDS vs. COPD

The %Respiratory ARDS and COPD models with mild, moderate, and severe severities is extensively tested in the scenarios shown in table 1.  Each row is a separate invasive mechanical ventilation scenario that is run for several minutes to reach a new homeostatic point based on the patient's disease state and ventilator settings. The patient is administered a neuromuscular blockade to prevent spontaneous breathing for all but the VC-AC scenario.  Typical/ideal ventilator setting are used based on literature @cite arnal2013feasibility @cite el2020comparison @cite acute2000ventilation and subject matter @cite chatburnSME input. Results successfully match expected empirical data and trends.

<center><br>
Table 1. Cumulative validation results for Anesthesia Machine specific conditions and actions scenarios.
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Patient State	|	Ventilator Settings	|	Respiration Rate (bpm)	|	Inspiratory-Expiratory Ratio	|	Pulmonary Resistance (cmH2O-s/L)	|	Pulmonary Compliance (L/cmH2O)	|	pH	|	Arterial Carbon Dioxide Pressure (mmHg)	|	Arterial Oxygen Pressure (mmHg)	|	PaO2/FiO2 (mmHg)	|	Oxygen Saturation (%)	|	Pulmonary Shunt Fraction (%)	|	Alveolar Dead Space (L)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Healthy	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 7<br>VT Target (mL): 527<br>PEEP (cm H2O): 5<br>FiO2 (%): 30<br>Mandatory Rate (bpm): 15 <br>I:E: 0.60<br>Minute Ventilation (L/min): 7.9<br>	|<span class="success">	15 (ventilator setting)	</span>|<span class="success">	0.6 (ventilator setting)	</span>|<span class="success">	12-17 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	20-54 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.27-7.40 @cite arnal2013feasibility	</span>|<span class="success">	35-42 @cite arnal2013feasibility	</span>|<span class="success">	85-114 @cite arnal2013feasibility	</span>|<span class="success">	264-366 @cite arnal2013feasibility, @cite ferguson2012berlin	</span>|<span class="success">	96.4-98.5 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	2-5 @cite Levitzky2013pulmonary, @cite radermacher2017fifty, @cite petersson2014gas	</span>|<span class="success">	~0 @cite Levitzky2013pulmonary	</span>|
|	Healthy	|	Mode: VC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 7<br>VT Target (mL): 527<br>PEEP (cm H2O): 5<br>FiO2 (%): 30<br>Mandatory Rate (bpm): 15 <br>I:E: 0.60<br>Minute Ventilation (L/min): 7.9<br>	|<span class="success">	15 (ventilator setting)	</span>|<span class="success">	0.6 (ventilator setting)	</span>|<span class="success">	12-17 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	20-54 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.27-7.40 @cite arnal2013feasibility	</span>|<span class="success">	35-42 @cite arnal2013feasibility	</span>|<span class="success">	85-114 @cite arnal2013feasibility	</span>|<span class="success">	264-366 @cite arnal2013feasibility, @cite ferguson2012berlin	</span>|<span class="success">	96.4-98.5 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	2-5 @cite Levitzky2013pulmonary, @cite radermacher2017fifty, @cite petersson2014gas	</span>|<span class="success">	~0 @cite Levitzky2013pulmonary	</span>|
|	Healthy	|	Mode: VC-AC<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 7<br>VT Target (mL): 527<br>PEEP (cm H2O): 5<br>FiO2 (%): 30<br>Mandatory Rate (bpm): 15 <br>I:E: 0.60<br>Minute Ventilation (L/min): 7.9<br>	|<span class="success">	15-21 @cite arnal2013feasibility	</span>|<span class="success">	0.4-0.8 @cite arnal2013feasibility	</span>|<span class="success">	12-17 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	20-54 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.27-7.40 @cite arnal2013feasibility	</span>|<span class="success">	35-42 @cite arnal2013feasibility	</span>|<span class="success">	85-114 @cite arnal2013feasibility	</span>|<span class="success">	264-366 @cite arnal2013feasibility, @cite ferguson2012berlin	</span>|<span class="success">	96.4-98.5 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	2-5 @cite Levitzky2013pulmonary, @cite radermacher2017fifty, @cite petersson2014gas	</span>|<span class="success">	~0 @cite Levitzky2013pulmonary	</span>|
|	Mild ARDS (severity = 0.3)	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 6<br>VT Target (mL): 452<br>PEEP (cm H2O): 11<br>FiO2 (%): 40<br>Mandatory Rate (bpm): 17<br>I:E: 0.90<br>Minute Ventilation (L/min): 7.6<br>	|<span class="success">	17 (ventilator setting)	</span>|<span class="success">	0.9 (ventilator setting)	</span>|<span class="success">	No change, 10-16 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	Decrease, 20-54 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.15-7.34 @cite arnal2013feasibility	</span>|<span class="success">	45-57 @cite arnal2013feasibility	</span>|<span class="success">	103-133 @cite arnal2013feasibility	</span>|<span class="success">	Decrease, 200-300 @cite ferguson2012berlin	</span>|<span class="success">	98.0-99.0 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	Increase, >10% @cite radermacher2017fifty, @cite petersson2014gas	</span>|<span class="success">	Increase @cite nuckton2002pulmonary	</span>|
|	Moderate ARDS (severity = 0.6)	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 6<br>VT Target (mL): 452<br>PEEP (cm H2O): 11<br>FiO2 (%): 70<br>Mandatory Rate (bpm): 20<br>I:E: 0.90<br>Minute Ventilation (L/min): 9.0<br>	|<span class="success">	20 (ventilator setting)	</span>|<span class="success">	0.9 (ventilator setting)	</span>|<span class="success">	No change, 10-16 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	Decrease, 20-54 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.15-7.34 @cite arnal2013feasibility	</span>|<span class="success">	45-57 @cite arnal2013feasibility	</span>|<span class="success">	81-117 @cite arnal2013feasibility	</span>|<span class="success">	Decrease, 100-200 @cite ferguson2012berlin	</span>|<span class="success">	95.9-98.6 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	Increase, >20% @cite radermacher2017fifty, @cite petersson2014gas	</span>|<span class="success">	Increase @cite nuckton2002pulmonary	</span>|
|	Severe ARDS (severity = 0.9)	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 6<br>VT Target (mL): 452<br>PEEP (cm H2O): 11<br>FiO2 (%): 100<br>Mandatory Rate (bpm): 25<br>I:E: 0.90<br>Minute Ventilation (L/min): 11.5<br>	|<span class="success">	25 (ventilator setting)	</span>|<span class="success">	0.9 (ventilator setting)	</span>|<span class="success">	No change, 10-16 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	Decrease, 20-54 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.15-7.34 @cite arnal2013feasibility	</span>|<span class="success">	45-57 @cite arnal2013feasibility	</span>|<span class="success">	56-83 @cite arnal2013feasibility	</span>|<span class="success">	Decrease, <100 @cite ferguson2012berlin	</span>|<span class="success">	88.8-96.1 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	Increase, >40% @cite radermacher2017fifty, @cite petersson2014gas	</span>|<span class="success">	Increase @cite nuckton2002pulmonary	</span>|
|	Mild COPD (severity = 0.3)	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 9.0<br>VT Target (mL): 678<br>PEEP (cm H2O): 8<br>FiO2 (%): 31<br>Mandatory Rate (bpm): 12<br>I:E: 0.43<br>Minute Ventilation (L/min): 8.0<br>	|<span class="success">	12 (ventilator setting)	</span>|<span class="success">	0.43 (ventilator setting)	</span>|<span class="success">	Increase, 17-26 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	Increase, 46-61 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.2-7.31 @cite arnal2013feasibility	</span>|<span class="success">	44-67 @cite arnal2013feasibility	</span>|<span class="success">	77-94 @cite arnal2013feasibility	</span>|<span class="success">	Decrease, 206-295 @cite ferguson2012berlin	</span>|<span class="success">	95.2-97.3 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	Increase @cite boerrigter2011cardiac	</span>|<span class="success">	Increase @cite Levitzky2013pulmonary	</span>|
|	Moderate COPD (severity = 0.6)	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 8.5<br>VT Target (mL): 640<br>PEEP (cm H2O): 8<br>FiO2 (%): 31<br>Mandatory Rate (bpm): 14<br>I:E: 0.43<br>Minute Ventilation (L/min): 9.0<br>	|<span class="success">	14 (ventilator setting)	</span>|<span class="success">	0.43 (ventilator setting)	</span>|<span class="success">	Increase, 17-26 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	Increase, 46-61 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.2-7.31 @cite arnal2013feasibility	</span>|<span class="success">	44-67 @cite arnal2013feasibility	</span>|<span class="success">	77-94 @cite arnal2013feasibility	</span>|<span class="success">	Decrease, 206-295 @cite ferguson2012berlin	</span>|<span class="success">	95.2-97.3 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	Increase @cite boerrigter2011cardiac	</span>|<span class="success">	Increase @cite Levitzky2013pulmonary	</span>|
|	Severe COPD (severity = 0.9)	|	Mode: PC-CMV<br>Inspiratory Pressure Target: set empirically to achieve VT Target<br>VT Target (mL/kg): 9.0<br>VT Target (mL): 678<br>PEEP (cm H2O): 8<br>FiO2 (%): 40<br>Mandatory Rate (bpm): 15<br>I:E: 0.43<br>Minute Ventilation (L/min): 10.2<br>	|<span class="success">	15 (ventilator setting)	</span>|<span class="success">	0.43 (ventilator setting)	</span>|<span class="success">	Increase, 17-26 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	Increase, 46-61 @cite arnal2013feasibility, @cite arnal2018parameters	</span>|<span class="success">	7.2-7.31 @cite arnal2013feasibility	</span>|<span class="success">	44-67 @cite arnal2013feasibility	</span>|<span class="success">	77-94 @cite arnal2013feasibility	</span>|<span class="success">	Decrease, 206-295 @cite ferguson2012berlin	</span>|<span class="success">	95.2-97.3 @cite arnal2013feasibility, @cite kacmarek2016egan	</span>|<span class="success">	Increase @cite boerrigter2011cardiac	</span>|<span class="success">	Increase @cite Levitzky2013pulmonary	</span>|

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

The engine modularity could be taken advantage of to add parameters and elements for specific equipment models and manufacturers. 

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
