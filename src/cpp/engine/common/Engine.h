/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "engine/CommonDefs.h"

namespace pulse
{
  class Controller;
  //--------------------------------------------------------------------------------------------------
  /// @brief  
  /// This is the implementation of the PhysiologyEngine interface for the this engines.
  /// @details
  /// It contains the necessary execution calls, patient customization calls, insult and intervention 
  /// calls as well as assessment calls for obtaining the results. During engine execution a log files
  /// is generated containing information, warning and error data.
  //--------------------------------------------------------------------------------------------------
  class PULSE_DECL Engine : public PhysiologyEngine
  {
  public:
    Engine(Logger* logger=nullptr);
    virtual ~Engine();

    virtual Logger* GetLogger() const override;

    virtual std::string GetTypeName() const override;

    virtual bool SerializeFromFile(const std::string& file) override;
    virtual bool SerializeToFile(const std::string& file) const override;

    virtual bool SerializeFromString(const std::string& state, eSerializationFormat m) override;
    virtual bool SerializeToString(std::string& state, eSerializationFormat m) const override;

    virtual bool InitializeEngine(const std::string& patient_configuration, eSerializationFormat m) override;
    virtual bool InitializeEngine(const SEPatientConfiguration& patient_configuration) override;

    virtual void Clear() override;

    virtual bool SetConfigurationOverride(const SEEngineConfiguration* config) override;

    virtual const SEConditionManager& GetConditionManager() const override;

    virtual SEEngineTracker* GetEngineTracker() const override;

    virtual const SEEngineConfiguration* GetConfiguration() const override;

    virtual double GetTimeStep(const TimeUnit& unit) const override;
    virtual double GetSimulationTime(const TimeUnit& unit) const override;
    virtual void  SetSimulationTime(const SEScalarTime& time) override;

    virtual bool  AdvanceModelTime() override;
    virtual bool  AdvanceModelTime(double time, const TimeUnit& unit) override;
    virtual bool  ProcessAction(const SEAction& action) override;
    virtual const SEActionManager& GetActionManager() const override;

    virtual const SESubstanceManager& GetSubstanceManager() const override;
    virtual void  SetAdvanceHandler(SEAdvanceHandler* handler) override;
    virtual const SEPatient& GetPatient() const override;
    virtual const SEPatient& GetInitialPatient() const override;
    virtual bool  GetPatientAssessment(SEPatientAssessment& assessment) const override;

    virtual const SEEnvironment* GetEnvironment() const override;
    virtual const SEBloodChemistrySystem* GetBloodChemistrySystem() const override;
    virtual const SECardiovascularSystem* GetCardiovascularSystem() const override;
    virtual const SEDrugSystem* GetDrugSystem() const override;
    virtual const SEEndocrineSystem* GetEndocrineSystem() const override;
    virtual const SEEnergySystem* GetEnergySystem() const override;
    virtual const SEGastrointestinalSystem* GetGastrointestinalSystem() const override;
    virtual const SEHepaticSystem* GetHepaticSystem() const override;
    virtual const SENervousSystem* GetNervousSystem() const override;
    virtual const SERenalSystem* GetRenalSystem() const override;
    virtual const SERespiratorySystem* GetRespiratorySystem() const override;
    virtual const SETissueSystem* GetTissueSystem() const override;
    virtual const SEAnesthesiaMachine* GetAnesthesiaMachine() const override;
    virtual const SEBagValveMask* GetBagValveMask() const override;
    virtual const SEECMO* GetECMO() const override;
    virtual const SEElectroCardioGram* GetElectroCardioGram() const override;
    virtual const SEInhaler* GetInhaler() const override;
    virtual const SEMechanicalVentilator* GetMechanicalVentilator() const override;

    virtual const SECompartmentManager& GetCompartments() const override;
    virtual SEBlackBoxManager& GetBlackBoxes() const override;

    virtual const SEEventManager& GetEventManager() const override;

    Controller& GetController() const;
  protected:
    virtual void AllocateController() const = 0;
    mutable Controller* m_Controller=nullptr;
  };
END_NAMESPACE
