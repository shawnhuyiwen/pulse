/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "CommonDataModel.h"
#include "engine/SEEngineStabilization.h"

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
                         Active };


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
public:
  PulseData(Logger* logger=nullptr);
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

  virtual SEBloodChemistrySystem&       GetBloodChemistry() const;
  virtual SECardiovascularSystem&       GetCardiovascular() const;
  virtual SEDrugSystem&                 GetDrugs() const;
  virtual SEEndocrineSystem&            GetEndocrine() const;
  virtual SEEnergySystem&               GetEnergy() const;
  virtual SEGastrointestinalSystem&     GetGastrointestinal() const;
  virtual SEHepaticSystem&              GetHepatic() const;
  virtual SENervousSystem&              GetNervous() const;
  virtual SERenalSystem&                GetRenal() const;
  virtual SERespiratorySystem&          GetRespiratory() const;
  virtual SETissueSystem&               GetTissue() const;

  virtual SEEnvironment&                GetEnvironment() const;
  virtual SEAnesthesiaMachine&          GetAnesthesiaMachine() const;
  virtual SEElectroCardioGram&          GetECG() const;
  virtual SEInhaler&                    GetInhaler() const;
  virtual SEMechanicalVentilator&       GetMechanicalVentilator() const;

  virtual SEActionManager&              GetActions() const;

  virtual SEConditionManager&           GetConditions() const;

  virtual SEEventManager&               GetEvents() const;

  virtual PulseCircuits&                GetCircuits() const;

  virtual PulseCompartments&            GetCompartments() const;

  virtual const PulseConfiguration&     GetConfiguration() const;

  virtual const SEScalarTime&           GetTimeStep() const;
  virtual const SEScalarTime&           GetEngineTime() const;
  virtual const SEScalarTime&           GetSimulationTime() const;

  virtual eAirwayMode                   GetAirwayMode() const { return m_AirwayMode; }
  virtual void                          SetAirwayMode(eAirwayMode mode);

  virtual eSwitch                       GetIntubation() const { return m_Intubation; }
  virtual void                          SetIntubation(eSwitch s);

  virtual void                          SetAdvanceHandler(SEAdvanceHandler* handler) { m_AdvanceHandler = handler; }

  virtual bool                          HasOverride() const;
  virtual const std::vector<SEScalarProperty>& GetOverrides() const;

  std::stringstream                                             m_ss;
protected:
  EngineState                                                   m_State;
  SEEngineTracker*                                              m_EngineTrack;
  DataTrack*                                                    m_DataTrack;

  SEScalarTime                                                  m_CurrentTime;
  SEScalarTime                                                  m_SimulationTime;
  double                                                        m_SpareAdvanceTime_s;
  eAirwayMode                                                   m_AirwayMode;
  eSwitch                                                       m_Intubation;

  std::unique_ptr<PulseConfiguration>                           m_Config;
  std::unique_ptr<SaturationCalculator>                         m_SaturationCalculator;

  std::unique_ptr<PulseSubstances>                              m_Substances;

  std::unique_ptr<SEActionManager>                              m_Actions;
  std::unique_ptr<SEConditionManager>                           m_Conditions;
  std::unique_ptr<PulseCircuits>                                m_Circuits;
  std::unique_ptr<PulseCompartments>                            m_Compartments;

  std::unique_ptr<Environment>                                  m_Environment;

  std::unique_ptr<BloodChemistry>                               m_BloodChemistrySystem;
  std::unique_ptr<Cardiovascular>                               m_CardiovascularSystem;
  std::unique_ptr<Endocrine>                                    m_EndocrineSystem;
  std::unique_ptr<Energy>                                       m_EnergySystem;
  std::unique_ptr<Gastrointestinal>                             m_GastrointestinalSystem;
  std::unique_ptr<Hepatic>                                      m_HepaticSystem;
  std::unique_ptr<Nervous>                                      m_NervousSystem;
  std::unique_ptr<Renal>                                        m_RenalSystem;
  std::unique_ptr<Respiratory>                                  m_RespiratorySystem;
  std::unique_ptr<Drugs>                                        m_DrugSystem;
  std::unique_ptr<Tissue>                                       m_TissueSystem;

  std::unique_ptr<ECG>                                          m_ECG;

  std::unique_ptr<AnesthesiaMachine>                            m_AnesthesiaMachine;

  std::unique_ptr<Inhaler>                                      m_Inhaler;

  std::unique_ptr<MechanicalVentilator>                         m_MechanicalVentilator;

  std::unique_ptr<SEPatient>                                    m_InitialPatient;
  std::unique_ptr<SEPatient>                                    m_CurrentPatient;

  std::unique_ptr<SEEventManager>                               m_EventManager;

  SEAdvanceHandler*                                             m_AdvanceHandler;

  std::string                                                   m_DataDir;

  std::vector<SEScalarProperty>                                 m_ScalarOverrides;

};

/**
* @brief Manages and controls execution of all data/systems in %Pulse
*/
class PULSE_DECL PulseController : public PulseData
{
  friend class PulseEngine;
  friend class PulseEngineTest;
  friend class PulseScenarioExec;
  friend class PBPulseState;//friend the serialization class
public:
  
  PulseController(Logger* logger=nullptr);
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

  virtual void  AdvanceModelTime();
  virtual void  AdvanceModelTime(double time, const TimeUnit& unit);
  virtual bool  ProcessAction(const SEAction& action);

  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const;

  virtual bool CreateCircuitsAndCompartments();
  virtual bool OverrideCircuits();
protected:

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

  // Notify systems that steady state has been achieved
  virtual void AtSteadyState(EngineState state);
  virtual void PreProcess();
  virtual void Process();
  virtual void PostProcess();

  virtual void ForwardFatal(const std::string&  msg, const std::string&  origin);

  PulseConfiguration const* m_ConfigOverride;
  PulseStabilizationController* m_Stabilizer;
};

class PULSE_DECL PulseStabilizationController : public SEEngineStabilization::Controller
{
public:
  PulseStabilizationController(PulseController& pc) : _pc(pc) {}
  ~PulseStabilizationController() = default;

  virtual void AdvanceTime() override { _pc.AdvanceModelTime(); }
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
