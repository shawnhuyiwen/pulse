/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "PhysiologyEngine.h"
#include "controller/Controller.h"
PULSE_BIND_DECL(StateData)
//--------------------------------------------------------------------------------------------------
/// @brief  
/// This is the implementation of the PhysiologyEngine interface for the this engines.
/// @details
/// It contains the necessary execution calls, patient customization calls, insult and intervention 
/// calls as well as assessment calls for obtaining the results. During engine execution a log files
/// is generated containing information, warning and error data.
//--------------------------------------------------------------------------------------------------
class PULSE_DECL PulseEngine : public PhysiologyEngine
{
public:

  PulseEngine(Logger* logger, const std::string& data_dir=".");
  PulseEngine(const std::string&, const std::string& data_dir=".");
  virtual ~PulseEngine();

  virtual bool SerializeFromFile(const std::string& file, SerializationFormat m);
  virtual bool SerializeFromFile(const std::string& file, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  virtual bool SerializeToFile(const std::string& file, SerializationFormat m) const;

  virtual bool SerializeFromString(const std::string& state, SerializationFormat m);
  virtual bool SerializeFromString(const std::string& state, SerializationFormat m, const SEScalarTime* simTime, const SEEngineConfiguration* config);
  virtual bool SerializeToString(std::string& state, SerializationFormat m) const;

  virtual const SEConditionManager& GetConditionManager() const;
  
  virtual Logger* GetLogger() const;
  virtual SEEngineTracker* GetEngineTracker() const;

  virtual bool InitializeEngine(const std::string& patient_configuration, SerializationFormat m, const SEEngineConfiguration* config = nullptr);

  virtual bool InitializeEngine(const SEPatientConfiguration& patient_configuration, const SEEngineConfiguration* config = nullptr);
  
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
  virtual const SEMechanicalVentilator*                GetMechanicalVentilator() const;

  virtual const SECompartmentManager&                  GetCompartments() const;

  virtual const SEEventManager&                        GetEventManager() const;

  PulseController* GetController() { return m_PulseController; }
protected:
  PulseController* m_PulseController;
};