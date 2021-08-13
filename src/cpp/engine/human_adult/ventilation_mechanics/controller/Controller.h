/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/human_adult/whole_body/controller/Controller.h"
#include "engine/human_adult/ventilation_mechanics/Engine.h"

namespace HUMAN_ADULT_VENT_MECH
{
  /**
  * @brief Manages and controls execution of all data/systems in an engine
  */
  class PULSE_DECL Controller : public HUMAN_ADULT_WHOLE_BODY::Controller
  {
    friend Engine;
  protected:// Create via PulseEngine
    Controller(Logger* logger = nullptr);
  public:
    virtual ~Controller() = default;

    virtual bool SerializeFromFile(const std::string& file) override;
    virtual bool SerializeToFile(const std::string& file) const override;

    virtual bool SerializeFromString(const std::string& state, SerializationFormat m) override;
    virtual bool SerializeToString(std::string& state, SerializationFormat m) const override;

    virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const override;

    virtual bool IsAirwayModeSupported(PULSE_ENGINE::eAirwayMode mode) override;

    virtual bool CreateCircuitsAndCompartments() override;

  protected:
    virtual void Allocate() override;
    virtual bool Stabilize(const SEPatientConfiguration& patient_configuration) override;

    virtual void InitializeModels() override;
    // Notify systems that steady state has been achieved
    virtual void AtSteadyState(PULSE_ENGINE::EngineState state) override;
    virtual void PreProcess() override;
    virtual void Process() override;
    virtual void PostProcess() override;
  };
}
