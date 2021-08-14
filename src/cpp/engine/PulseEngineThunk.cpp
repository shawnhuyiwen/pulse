/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "PulseEngineThunk.h"
#include "PulseScenarioExec.h"
#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/ventilation_mechanics/Engine.h"


namespace pulse::engine
{
  PulseEngineThunk::PulseEngineThunk(eModelType t, const std::string& dataDir) : PhysiologyEngineThunk(dataDir)
  {
    m_type = t;
    AllocateEngine();
    m_engine->GetLogger()->LogToConsole(false);
    m_engine->GetLogger()->AddForward(this);
  }
  PulseEngineThunk::~PulseEngineThunk()
  {

  }

  bool PulseEngineThunk::ExecuteScenario(std::string const& sceExecOpts, eSerializationFormat format)
  {
    SEScenarioExec opts;
    if (!opts.SerializeFromString(sceExecOpts, format))
      return false;
    return PULSE_ENGINE::PulseScenarioExec::Execute(*m_engine, opts);
  }

  void PulseEngineThunk::AllocateEngine()
  {
    m_engine = CreatePulseEngine(m_type);
  }

  std::unique_ptr<PhysiologyEngine> CreatePulseEngine(eModelType type, Logger* logger)
  {
    switch (type)
    {
    case eModelType::HumanAdultWholeBody:
      return std::move(std::make_unique<pulse::engine::human_adult::whole_body::Engine>(logger));
    case eModelType::HumanAdultVentilationMechanics:
      return std::move(std::make_unique<pulse::engine::human_adult::ventilation_mechanics::Engine>(logger));
    default:
      throw CommonDataModelException("Unsupported Physiology Engine Type");
    }
  }

  bool valueOf(const std::string s, eModelType& t)
  {
    if (s == "HumanAdultWholeBody")
      t = eModelType::HumanAdultWholeBody;
    else if (s == "HumanAdultVentilationMechanics")
      t = eModelType::HumanAdultVentilationMechanics;
    else
      return false;
    return true;
  }
}
