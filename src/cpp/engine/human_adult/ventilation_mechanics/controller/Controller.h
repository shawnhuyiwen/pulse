/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/human_adult/whole_body/controller/Controller.h"
#include "engine/human_adult/ventilation_mechanics/Engine.h"

namespace pulse { namespace human_adult_ventilation_mechanics
{
  /**
  * @brief Manages and controls execution of all data/systems in an engine
  */
  class PULSE_DECL Controller : public pulse::human_adult_whole_body::Controller
  {
    friend Engine;
  protected:// Create via PulseEngine
    Controller(Logger* logger = nullptr);
  public:
    virtual ~Controller() = default;

    virtual bool SerializeFromFile(const std::string& file) override;
    virtual bool SerializeToFile(const std::string& file) const override;

    virtual bool SerializeFromString(const std::string& state, eSerializationFormat m) override;
    virtual bool SerializeToString(std::string& state, eSerializationFormat m) const override;

    virtual bool GetPatientAssessment(SEPatientAssessment& assessment) const override;

    virtual bool IsAirwayModeSupported(pulse::eAirwayMode mode) override;

    virtual bool CreateCircuitsAndCompartments() override;

  protected:
    virtual std::string GetTypeName() const override { return "Human Adult Ventilation Mechanics"; }
    virtual void Allocate() override;
    virtual bool Stabilize(const SEPatientConfiguration& patient_configuration) override;
  };
END_NAMESPACE_EX
