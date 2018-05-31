/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "PhysiologyEngine.h"
#include "Controller/Controller.h"

//--------------------------------------------------------------------------------------------------
/// @brief  
/// This is the implementation of the PhysiologyEngine interface for the this engines.
/// @details
/// It contains the necessary execution calls, patient customization calls, insult and intervention 
/// calls as well as assessment calls for obtaining the results. During engine execution a log files
/// is generated containing information, warning and error data.
//--------------------------------------------------------------------------------------------------
class PULSE_DECL PulseEngine : public PhysiologyEngine, public PulseController
{
public:

  PulseEngine(Logger* logger);
  PulseEngine(const std::string&);
  virtual ~PulseEngine();

  virtual bool LoadStateFile(const std::string& file, const SEScalarTime* simTime = nullptr, const SEEngineConfiguration* config = nullptr);
  virtual bool LoadState(const google::protobuf::Message& state, const SEScalarTime* simTime = nullptr, const SEEngineConfiguration* config = nullptr);
  virtual std::unique_ptr<google::protobuf::Message> SaveState(const std::string& file = "");

  virtual const SEConditionManager& GetConditionManager() const;
  
  virtual Logger* GetLogger() const;
  virtual SEEngineTracker* GetEngineTracker() const;

  virtual bool InitializeEngine(const std::string& patientFile, const std::vector<const SECondition*>* conditions = nullptr, const SEEngineConfiguration* config = nullptr);
  virtual bool InitializeEngine(const SEPatient& patient, const std::vector<const SECondition*>* conditions = nullptr, const SEEngineConfiguration* config = nullptr);

  virtual const SEEngineConfiguration* GetConfiguration() const;

  virtual double GetTimeStep(const TimeUnit& unit) const;
  virtual double GetSimulationTime(const TimeUnit& unit) const;

  virtual void  AdvanceModelTime();
  virtual void  AdvanceModelTime(double time, const TimeUnit& unit);
  virtual bool  ProcessAction(const SEAction& action); 
  virtual const SEActionManager& GetActionManager() const;

  virtual SESubstanceManager& GetSubstanceManager();
  virtual const SESubstanceManager& GetSubstanceManager() const;
  virtual void SetAdvanceHandler(SEAdvanceHandler* handler);
  virtual void SetEventHandler(SEEventHandler* handler);
  virtual const SEPatient& GetPatient() const;
  virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const;

  virtual const SEEnvironment*                         GetEnvironment() const;
  virtual const SEBloodChemistrySystem*                GetBloodChemistrySystem() const;
  virtual const SECardiovascularSystem*                GetCardiovascularSystem() const;
  virtual const SEDrugSystem*                          GetDrugSystem() const;
  virtual const SEEndocrineSystem*                     GetEndocrineSystem() const;
  virtual const SEEnergySystem*                        GetEnergySystem() const;
  virtual const SEGastrointestinalSystem*              GetGastrointestinalSystem() const;
  virtual const SEHepaticSystem*                       GetHepaticSystem() const;
  virtual const SENervousSystem*                       GetNervousSystem() const;
  virtual const SERenalSystem*                         GetRenalSystem() const;
  virtual const SERespiratorySystem*                   GetRespiratorySystem() const;
  virtual const SETissueSystem*                        GetTissueSystem() const;
  virtual const SEAnesthesiaMachine*                   GetAnesthesiaMachine() const;
  virtual const SEElectroCardioGram*                   GetElectroCardioGram() const;
  virtual const SEInhaler*                             GetInhaler() const;

  virtual const SECompartmentManager&                  GetCompartments() const;

protected:

  virtual bool IsReady() const;
  virtual bool InitializeEngine(const std::vector<const SECondition*>* conditions = nullptr, const SEEngineConfiguration* config = nullptr);

  SEEventHandler*                                 m_EventHandler;
  SEAdvanceHandler*                               m_AdvanceHandler;
  SEEngineTracker*                                m_EngineTrack;
  std::stringstream                               m_ss;
};