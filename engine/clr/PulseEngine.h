/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once 
#include <string>

#include "PulsePhysiologyEngine.h"
#include "engine/SEEngineConfiguration.h"

#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "system/physiology/SEEndocrineSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SEGastrointestinalSystem.h"
#include "system/physiology/SEHepaticSystem.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SERenalSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/physiology/SETissueSystem.h"

#include "system/environment/SEEnvironment.h"
#include "system/environment/SEEnvironmentalConditions.h"

#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineChamber.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineOxygenBottle.h"

#include "system/equipment/ElectroCardioGram/SEElectroCardioGram.h"

#include "system/equipment/Inhaler/SEInhaler.h"

#include "patient/SEPatient.h"
#include "patient/actions/SEAcuteStress.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "patient/actions/SEApnea.h"
#include "patient/actions/SEAsthmaAttack.h"
#include "patient/actions/SEBrainInjury.h"
#include "patient/actions/SEBronchoconstriction.h"
#include "patient/actions/SECardiacArrest.h"
#include "patient/actions/SEChestCompressionForce.h"
#include "patient/actions/SEChestCompressionForceScale.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEConsumeNutrients.h"
#include "patient/actions/SEExercise.h"
#include "patient/actions/SEHemorrhage.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEMechanicalVentilation.h"
#include "patient/actions/SENeedleDecompression.h"
#include "patient/actions/SEPericardialEffusion.h"
#include "patient/actions/SESubstanceBolus.h"
#include "patient/actions/SESubstanceCompoundInfusion.h"
#include "patient/actions/SESubstanceInfusion.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "patient/actions/SEUrinate.h"

#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"

#include "engine/SEEventHandler.h"

// This class is used to expose the actual patient data parameters (i.e. data in the patient file)
// Expand this class with data you want from the patient specification
// It gets pulled once (at engine startup) as this data never changes
public ref class PatientData
{
public:
  double TotalLungCapacity_mL;
};

// Extend this class with data you want from the engine
public ref class VitalsData
{
public:
  // Cardiovascular
  double HeartRate_per_min;
  double CardiacOutput_mL_Per_s;
  double ArterialBloodPressure_mmHg;
  double CentralVenousPressure_mmHg;
  double DiastolicArterialPressure_mmHg; //(noninvasive and invasive)
  double SystolicArterialPressure_mmHg;//(noninvasive and invasive)
  double MeanArterialPressure_mmHg;//(noninvasive and invasive)
  // Respiratory
  double EndTidalCarbonDioxidePressure_cmH2O;
  double RespiratoryRate_per_min;
  double TidalVolume_mL;
  double TotalLungVolume_mL;
  double LeftLungVolume_mL;
  double RightLungVolume_mL;
  // Energy
  double SkinTemperature_F;
  double CoreTemperature_F;
  // Blood Chemistry
  double SpO2;
  // ECG
  double ECG_Lead3_mV;

  // Compartment data
};

typedef void(__stdcall *fpLog)(const std::string&);
class PulseLogger : public LoggerForward
{
public:
  // Make Set methods to set up a function pointer to call for each event we are interested in
  virtual void SetFatalCallback(fpLog callback) { _fatal = callback; }
  virtual void SetErrorCallback(fpLog callback) { _error = callback; }
  virtual void SetWarnCallback(fpLog callback) { _warn = callback; }
  virtual void SetInfoCallback(fpLog callback) { _info = callback; }
  virtual void SetDebugCallback(fpLog callback) { _debug = callback; }
protected:
  // Make a member variables for the function pointers we are to call
  fpLog _fatal;
  fpLog _error;
  fpLog _warn;
  fpLog _info;
  fpLog _debug;

public:// Note for my example, I am ignoring the origin (what class created this log msg)
  virtual void ForwardFatal(const std::string& msg, const std::string& origin) { _fatal(msg); }
  virtual void ForwardError(const std::string& msg, const std::string& origin) { _error(msg); }
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) { _warn(msg); }
  virtual void ForwardInfo(const std::string& msg, const std::string& origin) { _info(msg); }
  virtual void ForwardDebug(const std::string& msg, const std::string& origin) { _debug(msg); }
};

// Create function pointers to call when we get events from pulse
// For this example, I can check activity in my handler and I don't need to propagate time
// It's up to you what data you provide your system, just know you can!
typedef void(__stdcall *fpPatientEvent)(ePatient_Event type, bool active);
// This is the class to provide Pulse to call when any events are triggered in the engine
class PulseEventHandler : public SEEventHandler, public Loggable
{
public:
  PulseEventHandler(Logger *logger) : Loggable(logger) {}

public:
  virtual void SetPatientEventCallback(fpPatientEvent callback) { _on_patient_event = callback; }// Make Set methods to set up a function pointer to call for each event we are interested in
protected:
  fpPatientEvent _on_patient_event;// Make a member variables for the function pointers we are to call

                                   //These methods need definitions and should call your callback with what ever data you made it require
public:
  virtual void HandlePatientEvent(ePatient_Event type, bool active, const SEScalarTime* time = nullptr);
  virtual void HandleAnesthesiaMachineEvent(eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr) { /*Not Expecting these */ }
};

public ref class PulseEngineEventCallbacksRef abstract
{
public:
  virtual void HandlePatientEvent(ePatient_Event e, bool active) abstract;// CSharp will override this, and it will be called by the Pulse Event Handler
};

public ref class PulseEngineRef
{
public:

  PulseEngineRef();
  PulseEngineRef(System::String^ logfile);
  //PulseEngineRef(Logger*);
  virtual ~PulseEngineRef();

  bool LoadStateFile(System::String^ filename, double sim_start_time_s);
  void SaveStateFile(System::String^ filename);

  double GetSimTime_s();
  void AdvanceModelTime_s(double time_s);

  // Action Methods
  void AcuteStress(double severity);
  void AirwayObstruction(double severity);
  void Apnea(double severity);
  void Asthma(double severity);
  void CardiacArrest();
  void Hemorrhage(double flowRate_mL_per_min);
  void Intubation(eIntubation_Type type);
  void ChestCompression(double compressionForce_Newtons);
  void NeedleDecompression(bool active, eSide side);
  void Pneumothorax(eGate state, eSide side, double severity);

  // Outputs and State
  VitalsData^ GetVitalsData() { return _vitalsData; }
  PatientData^ GetPatientData() { return _patientData; }
  void SetPulseEventCallback(PulseEngineEventCallbacksRef^ callback) { _callback = callback; }

protected:
  void Init();
  void Clear();
  delegate void PatientEventDelegate(ePatient_Event type, bool active);
  void HandlePatientEvent(ePatient_Event type, bool active);
  PulseEngineEventCallbacksRef^ _callback;
private:
  PatientEventDelegate^ on_event;

internal: // Should be hidden from C#, which is what we want for these.
  PhysiologyEngine *               _pulse = nullptr;
  PulseLogger*                     _logger = nullptr;
  PulseEventHandler*               _events = nullptr;
  Object^                          _lock = gcnew System::Object();

  VitalsData^                      _vitalsData = gcnew VitalsData();
  PatientData^                     _patientData = gcnew PatientData();

  // Utility to convert C# strings to C++ Strings
  std::string MarshalString(System::String^ s);

  // Pulse Systems
  const SEBloodChemistrySystem*    _bldChem;
  const SECardiovascularSystem*    _cv;
  const SEDrugSystem*              _drgs;
  const SEEndocrineSystem*         _endo;
  const SEEnergySystem*            _enrgy;
  const SEGastrointestinalSystem*  _gi;
  const SEHepaticSystem*           _hep;
  const SENervousSystem*           _nrv;
  const SERenalSystem*             _renl;
  const SERespiratorySystem*       _resp;
  const SETissueSystem*            _tiss;

  const SEEnvironment*             _env;

  const SEAnesthesiaMachine*       _am;
  const SEElectroCardioGram*       _ecg;
  const SEInhaler*                 _inh;

  // Get substances and compartments we are interested in
  // You can add you specific compartments, substances, and substance quantites here:
  SESubstance*                     _O2 = nullptr;
  SESubstance*                     _CO2 = nullptr;

  const SELiquidCompartment*       _aorta;
  const SELiquidSubstanceQuantity* _aorta_O2;
  const SELiquidSubstanceQuantity* _aorta_CO2;

  const SEGasCompartment*          _carina;
  const SEGasSubstanceQuantity*    _carina_O2;
  const SEGasSubstanceQuantity*    _carina_CO2;

  const SEGasCompartment*          _leftLung;
  const SEGasCompartment*          _rightLung;
};
