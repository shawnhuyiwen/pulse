Implementation and Integration {#techdeets}
==============================

<b>What are your models and how did you validate them?</b><br>
The engine is a closed-loop whole body physiology model that combines physics-based lumped-parameter models 
and control system feedback mechanisms to model real-time system level 
physiologic behaviors. Lumped-parameter models use electrical circuit analogs 
to represent the major physiologic systems. 

Models/results are validated using a combination of peer-reviewed publications 
and subject matter expertise. The validation process includes:
- Defining key parameters for system validation
- Performing literature reviews to gather published data in the form of waveforms, and max, min, and mean values
- Using custom developed tools to compare data, perform analysis, and generate plots and tables of results

The primary purpose of model validation is to ensure that the model has an appropriate domain of validity 
given the goals of the modeling process. In the future, we plan to investigate sensitivity and uncertainty analyses to further quantify the validity of our models.
We would welcome and support, in as much as we are able, any validation or uncertainty quantification effort by the community.

We provide reports for each @ref SystemMethodology included in the physiology engine.
<br>  
These documents cover the design, implementation, assumptions, limitations, and validation of each model. 

<b>Physiology Systems</b>
@secreflist
 @refitem BloodChemistryMethodology "Blood Chemistry"
 @refitem CardiovascularMethodology "Cardiovascular"
 @refitem DrugsMethodology "Drugs"
 @refitem EndocrineMethodology "Endocrine"
 @refitem EnergyMethodology "Energy"
 @refitem EnvironmentMethodology "Environment"
 @refitem GastrointestinalMethodology "Gastrointestinal"
 @refitem NervousMethodology "Nervous"
 @refitem RenalMethodology "Renal"
 @refitem RespiratoryMethodology "Respiratory"
 @refitem TissueMethodology "Tissue"
@endsecreflist 
<br><b>Equipment</b>
@secreflist
 @refitem AnesthesiaMachineMethodology "Anesthesia Machine"
 @refitem InhalerMethodology "Inhaler"
@endsecreflist  
<br><b>Modeling Support</b><br>
@secreflist
 @refitem PatientMethodology "Patient Variability"
 @refitem CircuitMethodology "Circuits"
 @refitem SubstanceTransportMethodology "Substance Transport"
@endsecreflist
<br>

<b>Are there any publications related to the models that you have developed and choose to implement in the engine?</b><br>
A list of publications and presentations about the engine can be found on the @ref published "Publications" page. 
Many of the physiology models in the engine are adapted or implemented directly from models described in literature. 
The implementation methodology is described in detail in the @ref SystemMethodology and sub-system documentation, and 
all of the source publications are cited in the methodology reports and listed in the Bibliography. You can also find blog posts about work related to the physiology engine at https://blog.kitware.com.


<b>How do I integrate the physiology engine into my application?</b><br>
The engine has a modular architecture to reduce costs for applications that need a physiology engine and want to develop or extend a physiology model.

This architecture contains :
- A @ref CDM containing a library of classes and generic algorithms for lumped parameter physiology modeling 
- A standard @ref physeng for controlling a physiology engine via these CDM objects

We created a @ref SDK (SDK) to help developers integrate
the engine into software applications. This SDK provides
pre-built libraries and headers, as well as examples of how to programmatically
using the provided interfaces. The provided application programming
interfaces (APIs) provide full control over the engine to execute
various actions and retrieve a range of calculated physiological
outputs.

<b>How fast does the physiology engine run? How can I make it faster?</b><br>
The engine currently runs at about 5 to 10 times real time, depending on your machine's specifications. The included models and systems are included with the goal of creating the most complete engine possible. If your application does not require all of the existing models/systems, 
then you can strip features by modifying the source code in the same way that you would integrate a new model. Reducing the scope of the models will increase the speed of calculation.

<b>Why does it take so long to initialize the physiology engine?</b><br>
The engine represents a single, @ref PatientMethodology "variable patient".
Patient variability requires that the engine analyze the provided patient baseline values and stabilize the physiology to those values. 
This initialization can take several minutes, but once complete, the engine state can be saved to a file as protobuf binary or json.
You can then load this state and instantaneously start execution of the simulation without any initialization time.
Please consult the example in the SDK for how to take advantage of this feature and eliminate any initialization time in your application.


<b>Do you plan to provide support for interpreter-level model input, for example with the Python language?</b><br>
We do not have any immediate plans to provide support for those languages. 
We do have support for Java. We are working towards creating a C# interface on top of our C++ interface. But feel free to report this and other feature requests as issues at <a href="https://gitlab.kitware.com/physiology/engine/issues">gitlab</a>.


<b>Can I contact the physiology team to work on my current or upcoming project?</b><br>
Absolutely. We always welcome new and challenging opportunities to 
work with research partners and sponsors. Please email us at kitware@kitware.com.


<b>How can I modify or extend the models in the physiology engine?</b><br>
The engine uses an extensible architecture to promote integration with external
models with varying levels of fidelity. System-level model fidelity can be 
increased or decreased by adding or removing nodes and sub-circuits.

All integration/extension will require a custom build of our @ref SourceCode.
The @ref CDM provides a standard for data interchange between models. 
The deliberate identification of data requirements must precede any model modification or addition to determine 
if an extension of the Common Data Model is required. If the existing data model is sufficient to meet your modeling needs, 
you may be able to implement changes satisfactorily just by modifying the source code for the physiologic system 
of interest. If a Common Data Model extension is necessary, modification of the source code becomes more complicated. The 
quickest and easiest way to modify the engine to meet your needs is to work with Kitware - email: kitware@kitware.com.

We can help with requirements definition, provide development support, and/or make modifications for you.

<b>How do I ensure my changes/model are good?</b><br>
We include scenarios and their results for verification and validation. 
These results provide a baseline we can use to measure deviations to results when the code is modified.
As changes are implemented in the code base, we rerun all scenarios and compare the new results with baseline results to see how the implemented changes manifest in system data. 
Any new result that is over 2% error is marked as a failure. 
This data is used extensively to validate each system individually, as well as the combined effects of insults and interventions. See the Methodology Reports for more details.  
The scenarios output requests match the columns in the results file; we recommend that these scenarios remain unmodified.

<b>How can I contribute to the physiology engine?</b><br>
Take a look at our <a href="https://gitlab.kitware.com/physiology/engine/blob/master/CONTRIBUTING.md">Contribution Guide</a>.