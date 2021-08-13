/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/controller/Controller.h"
#include "engine/human_adult/whole_body/Engine.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  class EngineTest;
  /**
  * @brief Manages and controls execution of all data/systems in an engine
  */
  class PULSE_DECL Controller : public PULSE_ENGINE::Controller
  {
    friend Engine;
    friend EngineTest;
  protected:// Create via Engine
    Controller(Logger* logger = nullptr);
  public:
    virtual ~Controller() = default;

    virtual bool SerializeFromFile(const std::string& file) override;
    virtual bool SerializeToFile(const std::string& file) const override;

    virtual bool SerializeFromString(const std::string& state, SerializationFormat m) override;
    virtual bool SerializeToString(std::string& state, SerializationFormat m) const override;

    virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const override;

  protected:
    virtual void Allocate() override;
    virtual bool SetupPatient(const SEPatient& patient) override;

    virtual void InitializeModels() override;
    virtual void AtSteadyState(PULSE_ENGINE::EngineState state) override;
    virtual void PreProcess() override;
    virtual void Process() override;
    virtual void PostProcess() override;
  };
}
