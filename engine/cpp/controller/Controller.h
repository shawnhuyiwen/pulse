/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "CommonDataModel.h"

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

class SaturationCalculator;

class PulseScenarioExec;
enum class EngineState { NotReady=0, Initialization, InitialStabilization, AtInitialStableState, SecondaryStabilization, AtSecondaryStableState, Active };


// Keep enums in sync with appropriate proto file !!
enum class eAirwayMode{ Free=0, AnesthesiaMachine, Inhaler, MechanicalVentilator,
                        NasalCannula, SimpleMask, NonRebreatherMask };
extern const std::string& eAirwayMode_Name(eAirwayMode m);

/**
* @brief Manages and controls execution of all data/systems in %Pulse
*/
class PULSE_DECL PulseController : public Loggable, protected LoggerForward
{
  friend class PulseEngineTest;
  friend class PulseScenarioExec;
protected:
  EngineState m_State;
public:
  
  PulseController(Logger* logger, const std::string& data_dir=".");
  PulseController(const std::string& logfileName, const std::string& data_dir=".");
  virtual ~PulseController();

  EngineState                             GetState();

  DataTrack&                              GetDataTrack();
  SaturationCalculator&                   GetSaturationCalculator();

  PulseSubstances&                        GetSubstances();

  SEPatient&                              GetPatient();
  bool                                    GetPatientAssessment(SEPatientAssessment& assessment) const;

  SEBloodChemistrySystem&                 GetBloodChemistry();
  SECardiovascularSystem&                 GetCardiovascular();
  SEDrugSystem&                           GetDrugs();
  SEEndocrineSystem&                      GetEndocrine();
  SEEnergySystem&                         GetEnergy();
  SEGastrointestinalSystem&               GetGastrointestinal();
  SEHepaticSystem&                        GetHepatic();
  SENervousSystem&                        GetNervous();
  SERenalSystem&                          GetRenal();
  SERespiratorySystem&                    GetRespiratory();
  SETissueSystem&                         GetTissue();

  SEEnvironment&                          GetEnvironment();

  SEAnesthesiaMachine&                    GetAnesthesiaMachine();

  SEElectroCardioGram&                    GetECG();

  SEInhaler&                              GetInhaler();

  SEActionManager&                        GetActions();

  SEConditionManager&                     GetConditions();

  PulseCircuits&                          GetCircuits();


  PulseCompartments&                      GetCompartments();

  const PulseConfiguration&               GetConfiguration();

  const SEScalarTime&                     GetEngineTime();
  const SEScalarTime&                     GetSimulationTime();
  const SEScalarTime&                     GetTimeStep();

  eAirwayMode                             GetAirwayMode();
  void                                    SetAirwayMode(eAirwayMode mode);

  eSwitch                                 GetIntubation();
  void                                    SetIntubation(eSwitch s);

  bool CreateCircuitsAndCompartments();
  virtual void AdvanceCallback(double time_s) {};
protected:
  void SetupCardiovascular();
  void SetupRenal();
  void SetupTissue();
  void SetupGastrointestinal();
  void SetupRespiratory();
  void SetupAnesthesiaMachine();
  void SetupInhaler();
  void SetupNasalCannula();
  void SetupSimpleMask();
  void SetupNonRebreatherMask();
  void SetupMechanicalVentilator();
  void SetupExternalTemperature();
  void SetupInternalTemperature();

  bool Initialize(const PulseConfiguration* config);
  bool SetupPatient();

  // Notify systems that steady state has been achieved
  virtual void AtSteadyState(EngineState state);
  void PreProcess();
  void Process();
  void PostProcess();

  void ForwardFatal(const std::string&  msg, const std::string&  origin);

  DataTrack*                                                    m_DataTrack;

  std::unique_ptr<SEScalarTime>                                 m_CurrentTime;
  std::unique_ptr<SEScalarTime>                                 m_SimulationTime;
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

  std::unique_ptr<SEPatient>                                    m_Patient;

  // Flag to destroy the logger or not                          
  bool                                                          myLogger;
  SEEventHandler*                                               m_EventHandler;
  SEAdvanceHandler*                                             m_AdvanceHandler;

  std::string                                                   m_DataDir;
};

