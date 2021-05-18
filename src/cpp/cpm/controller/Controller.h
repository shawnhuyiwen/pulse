/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "CommonDataModel.h"
#include "engine/SEEngineStabilization.h"

class PulseBlackBoxes;
class PulseConfiguration;
class PulseCircuits;
class PulseCompartments;
class PulseSubstances;

// CDM 
class SEActionManager;
class SEConditionManager;

class DataTrack;
class SEEngineTracker;

// Pulse
class BloodChemistry;
class Cardiovascular;
class Endocrine;
class Energy;
class Gastrointestinal;
class Hepatic;
class Nervous;
class Renal;
class Respiratory;
class Drugs;
class Tissue;
class Environment;

class ECG;
class AnesthesiaMachine;
class Inhaler;
class MechanicalVentilator;

class SaturationCalculator;

class PulseScenarioExec;
class PulseStabilizationController;

enum class EngineState { NotReady=0,
                         Initialization,
                         InitialStabilization,
                         AtInitialStableState,
                         SecondaryStabilization,
                         AtSecondaryStableState,
                         Active,
                         Fatal};


// Keep enums in sync with appropriate proto file !!
enum class eAirwayMode{ Free=0, 
                        AnesthesiaMachine,
                        Inhaler,
                        MechanicalVentilation,// Action
                        MechanicalVentilator, // Equipment
                        NasalCannula,
                        NonRebreatherMask,
                        SimpleMask};
extern const std::string& eAirwayMode_Name(eAirwayMode m);

class PULSE_DECL PulseData : public Loggable
{
  friend class PulseEngine;
protected:// Create via PulseEngine
  PulseData(Logger* logger = nullptr);
public:
  virtual ~PulseData();

  virtual std::string GetDataRoot() const { return m_DataDir; }
  virtual void SetDataRoot(const std::string& dir) { m_DataDir = dir; }

  virtual void                          AdvanceCallback(double time_s);

  virtual EngineState                   GetState() const { return m_State; }

  virtual SEEngineTracker&              GetEngineTracker() const;
  virtual DataTrack&                    GetDataTrack() const;
  virtual SaturationCalculator&         GetSaturationCalculator() const;

  virtual PulseSubstances&              GetSubstances() const;

  virtual const SEPatient&              GetInitialPatient() const;
  virtual SEPatient&                    GetCurrentPatient() const;

  virtual bool                          HasBloodChemistry() const;
  virtual SEBloodChemistrySystem&       GetBloodChemistry() const;
  virtual bool                          HasCardiovascular() const;
  virtual SECardiovascularSystem&       GetCardiovascular() const;
  virtual bool                          HasDrugs() const;
  virtual SEDrugSystem&                 GetDrugs() const;
  virtual bool                          HasEndocrine() const;
  virtual SEEndocrineSystem&            GetEndocrine() const;
  virtual bool                          HasEnergy() const;
  virtual SEEnergySystem&               GetEnergy() const;
  virtual bool                          HasGastrointestinal() const;
  virtual SEGastrointestinalSystem&     GetGastrointestinal() const;
  virtual bool                          HasHepatic() const;
  virtual SEHepaticSystem&              GetHepatic() const;
  virtual bool                          HasNervous() const;
  virtual SENervousSystem&              GetNervous() const;
  virtual bool                          HasRenal() const;
  virtual SERenalSystem&                GetRenal() const;
  virtual bool                          HasRespiratory() const;
  virtual SERespiratorySystem&          GetRespiratory() const;
  virtual bool                          HasTissue() const;
  virtual SETissueSystem&               GetTissue() const;

  virtual bool                          HasEnvironment() const;
  virtual SEEnvironment&                GetEnvironment() const;
  virtual bool                          HasAnesthesiaMachine() const;
  virtual SEAnesthesiaMachine&          GetAnesthesiaMachine() const;
  virtual bool                          HasECG() const;
  virtual SEElectroCardioGram&          GetECG() const;
  virtual bool                          HasInhaler() const;
  virtual SEInhaler&                    GetInhaler() const;
  virtual bool                          HasMechanicalVentilator() const;
  virtual SEMechanicalVentilator&       GetMechanicalVentilator() const;

  virtual SEActionManager&              GetActions() const;

  virtual SEConditionManager&           GetConditions() const;

  virtual SEEventManager&               GetEvents() const;

  virtual PulseCircuits&                GetCircuits() const;

  virtual PulseCompartments&            GetCompartments() const;

  virtual PulseBlackBoxes&              GetBlackBoxes() const;

  virtual const PulseConfiguration&     GetConfiguration() const;

  virtual double                        GetTimeStep_s() const;
  virtual const SEScalarTime&           GetTimeStep() const;
  virtual const SEScalarTime&           GetEngineTime() const;
  virtual const SEScalarTime&           GetSimulationTime() const;

  virtual bool                          IsAirwayModeSupported(eAirwayMode mode) { return true; }
  virtual eAirwayMode                   GetAirwayMode() const { return m_AirwayMode; }
  virtual void                          SetAirwayMode(eAirwayMode mode);

  virtual eSwitch                       GetIntubation() const { return m_Intubation; }
  virtual void                          SetIntubation(eSwitch s);

  virtual void                          SetAdvanceHandler(SEAdvanceHandler* handler) { m_AdvanceHandler = handler; }

  virtual bool                          HasOverride() const;
  virtual const std::vector<SEScalarProperty>& GetOverrides() const;

  std::stringstream                     m_ss;
protected:
  virtual void                          Allocate() {};
  virtual void                          SetupTracker();

  EngineState                           m_State;
  SEEngineTracker*                      m_EngineTrack;

  SEScalarTime                          m_CurrentTime;
  SEScalarTime                          m_SimulationTime;
  double                                m_SpareAdvanceTime_s;
  eAirwayMode                           m_AirwayMode;
  eSwitch                               m_Intubation;

  PulseConfiguration*                   m_Config=nullptr;
  SaturationCalculator*                 m_SaturationCalculator=nullptr;

  PulseSubstances*                      m_Substances=nullptr;

  SEActionManager*                      m_Actions=nullptr;
  SEConditionManager*                   m_Conditions=nullptr;
  PulseCircuits*                        m_Circuits=nullptr;
  PulseCompartments*                    m_Compartments=nullptr;
  PulseBlackBoxes*                      m_BlackBoxes=nullptr;

  Environment*                          m_Environment=nullptr;

  BloodChemistry*                       m_BloodChemistrySystem=nullptr;
  Cardiovascular*                       m_CardiovascularSystem=nullptr;
  Endocrine*                            m_EndocrineSystem=nullptr;
  Energy*                               m_EnergySystem=nullptr;
  Gastrointestinal*                     m_GastrointestinalSystem=nullptr;
  Hepatic*                              m_HepaticSystem=nullptr;
  Nervous*                              m_NervousSystem=nullptr;
  Renal*                                m_RenalSystem=nullptr;
  Respiratory*                          m_RespiratorySystem=nullptr;
  Drugs*                                m_DrugSystem=nullptr;
  Tissue*                               m_TissueSystem=nullptr;

  ECG*                                  m_ECG=nullptr;

  AnesthesiaMachine*                    m_AnesthesiaMachine=nullptr;

  Inhaler*                              m_Inhaler=nullptr;

  MechanicalVentilator*                 m_MechanicalVentilator=nullptr;

  SEPatient*                            m_InitialPatient=nullptr;
  SEPatient*                            m_CurrentPatient=nullptr;

  SEEventManager*                       m_EventManager=nullptr;
  LoggerForward*                        m_LogForward=nullptr;

  SEAdvanceHandler*                     m_AdvanceHandler=nullptr;

  std::string                           m_DataDir;

  std::vector<SEScalarProperty>         m_ScalarOverrides;

};

/**
* @brief Manages and controls execution of all data/systems in %Pulse
*/
class PULSE_DECL PulseController : public PulseData
{
  friend class PulseEngine;
  friend class PulseEngineTest;
  friend class PBPulseState;//friend the serialization class
protected:// Create via PulseEngine
  PulseController(Logger* logger = nullptr);
public:
  virtual ~PulseController();

  virtual PulseData& GetData() { return (*this); }
  virtual const PulseData& GetData() const { return (*this); }

  virtual bool SerializeFromFile(const std::string& file);
  virtual bool SerializeToFile(const std::string& file) const;

  virtual bool SerializeFromString(const std::string& state, SerializationFormat m);
  virtual bool SerializeToString(std::string& state, SerializationFormat m) const;

  virtual bool InitializeEngine(const std::string& patient_configuration, SerializationFormat m);
  virtual bool InitializeEngine(const SEPatientConfiguration& patient_configuration);
  virtual bool IsReady() const;

  virtual bool SetConfigurationOverride(const SEEngineConfiguration* config);

  virtual void SetSimulationTime(const SEScalarTime& time);

  virtual bool  AdvanceModelTime();
  virtual bool  AdvanceModelTime(double time, const TimeUnit& unit);
  virtual bool  ProcessAction(const SEAction& action);

  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const;

  virtual bool CreateCircuitsAndCompartments();
  virtual bool OverrideCircuits();
protected:
  virtual void Allocate() override;

  virtual void SetupCardiovascular();
  virtual void SetupRenal();
  virtual void SetupTissue();
  virtual void SetupCerebrospinalFluid();
  virtual void SetupGastrointestinal();
  virtual void SetupRespiratory();
  virtual void SetupAnesthesiaMachine();
  virtual void SetupInhaler();
  virtual void SetupMechanicalVentilation();
  virtual void SetupMechanicalVentilator();
  virtual void SetupNasalCannula();
  virtual void SetupSimpleMask();
  virtual void SetupNonRebreatherMask();
  virtual void SetupExternalTemperature();
  virtual void SetupInternalTemperature();

  virtual bool Initialize(const SEPatient& patient);
  virtual bool SetupPatient(const SEPatient& patient);
  virtual void InitializeSystems();

  // Notify systems that steady state has been achieved
  virtual void AtSteadyState(EngineState state);
  virtual void PreProcess();
  virtual void Process();
  virtual void PostProcess();

  PulseConfiguration const* m_ConfigOverride=nullptr;
  PulseStabilizationController* m_Stabilizer=nullptr;
};

class PULSE_DECL PulseStabilizationController : public SEEngineStabilization::Controller
{
public:
  PulseStabilizationController(PulseController& pc) : _pc(pc) {}
  ~PulseStabilizationController() = default;

  virtual bool AdvanceTime() override { return _pc.AdvanceModelTime(); }
  virtual SEEngineTracker* GetEngineTracker() override
  {
    return &_pc.GetData().GetEngineTracker();
  }
  virtual double GetTimeStep(const TimeUnit& unit) override
  {
    return _pc.GetData().GetTimeStep().GetValue(unit);
  }

protected:
  PulseController& _pc;
};
