/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once 
#include <string>

class PhysiologyEngine;
class PulseLogger;
class PulseEventHandler;

class SESubstance;
class SEGasCompartment;
class SEGasSubstanceQuantity;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

class SEBloodChemistrySystem;
class SECardiovascularSystem;
class SEDrugSystem;
class SEEndocrineSystem;
class SEEnergySystem;
class SEGastrointestinalSystem;
class SEHepaticSystem;
class SENervousSystem;
class SERenalSystem;
class SERespiratorySystem;
class SETissueSystem;

class SEEnvironment;

class SEAnesthesiaMachine;
class SEElectroCardioGram;
class SEInhaler;

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

// Keep in sync with bind/Patient.pb.h
public enum class ePatientEvent
{
  Antidiuresis = 0,
  Asystole = 1,
  Bradycardia = 2,
  Bradypnea = 3,
  BrainOxygenDeficit = 4,
  CardiacArrest = 5,
  CardiogenicShock = 6,
  CriticalBrainOxygenDeficit = 7,
  Dehydration = 8,
  Diuresis = 9,
  Fasciculation = 10,
  Fatigue = 11,
  FunctionalIncontinence = 12,
  Hypercapnia = 13,
  Hyperglycemia = 14,
  Hyperthermia = 15,
  Hypoglycemia = 16,
  Hypothermia = 17,
  Hypoxia = 18,
  HypovolemicShock = 19,
  IntracranialHypertension = 20,
  IntracranialHypotension = 21,
  IrreversibleState = 22,
  Ketoacidosis = 23,
  LacticAcidosis = 24,
  MaximumPulmonaryVentilationRate = 25,
  MetabolicAcidosis = 26,
  MetabolicAlkalosis = 27,
  MildAcuteRespiratoryDistress = 28,
  ModerateAcuteRespiratoryDistress = 29,
  MyocardiumOxygenDeficit = 30,
  Natriuresis = 31,
  NutritionDepleted = 32,
  PulselessRhythm = 33,
  RenalHypoperfusion = 34,
  RespiratoryAcidosis = 35,
  RespiratoryAlkalosis = 36,
  StartOfCardiacCycle = 37,
  StartOfExhale = 38,
  StartOfInhale = 39,
  SevereAcuteRespiratoryDistress = 40,
  Tachycardia = 41,
  Tachypnea = 42
};

public ref class PulseEngineEventCallbacksRef abstract
{
public:
  virtual void HandlePatientEvent(ePatientEvent e, bool active) abstract;// CSharp will override this, and it will be called by the Pulse Event Handler
};

// Enum mappings from managed to unmanaged enums
public enum class eSide { Left = true, Right = false };
public enum class eState { On = true, Off = false };
public enum class eIntubationType {
  Off = 0,
  Esophageal = 1,
  LeftMainstem = 2,
  RightMainstem = 3,
  Tracheal = 4
};
public enum class eGate { Open = true, Closed = false };

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
  void Intubation(/*cdm::*/eIntubationType type);
  void ChestCompression(double compressionForce_Newtons);
  void NeedleDecompression(bool active, /*cdm::*/eSide side);
  void Pneumothorax(/*cdm::*/eGate state, /*cdm::*/eSide side, double severity);

  // Outputs and State
  VitalsData^ GetVitalsData() { return _vitalsData; }
  PatientData^ GetPatientData() { return _patientData; }
  void SetPulseEventCallback(PulseEngineEventCallbacksRef^ callback) { _callback = callback; }

protected:
  void Init();
  void Clear();
  delegate void PatientEventDelegate(ePatientEvent type, bool active);
  void HandlePatientEvent(ePatientEvent type, bool active);
  PulseEngineEventCallbacksRef^ _callback;
private:
  PatientEventDelegate ^ on_event;

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
