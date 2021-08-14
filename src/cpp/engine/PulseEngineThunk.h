/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/PulseEngine.h"
#include "cdm/PhysiologyEngineThunk.h"

namespace pulse::engine
{
  class PulseEngineThunk : public PhysiologyEngineThunk
  {
  public:
    PulseEngineThunk(eModelType t, const std::string& dataDir = "./");
    virtual ~PulseEngineThunk();

    virtual bool ExecuteScenario(std::string const& sceExecOpts, eSerializationFormat format) override;

  protected:
    virtual void AllocateEngine() override;

    eModelType m_type;
  };
}
