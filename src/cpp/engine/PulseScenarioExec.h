/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/CommonDefs.h"
#include "engine/PulseScenario.h"
#include "cdm/scenario/SEScenarioExec.h"

/**
* @brief Create a Pulse engine and execute the scenario opts
*/
class PULSE_DECL PulseScenarioExec : public SEScenarioExec
{
public:
  PulseScenarioExec(Logger* logger) : SEScenarioExec(logger) { Clear(); }

  void Clear() override;
  void Copy(const PulseScenarioExec& src);

  eModelType GetModelType() const { return m_ModelType; }
  void SetModelType(eModelType mt) { m_ModelType = mt; }

  bool SerializeToString(std::string& output, eSerializationFormat m, Logger* logger = nullptr) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m, Logger* logger = nullptr);

  bool Execute();
  bool Execute(PulseScenario& sce);
  bool ConvertLog();

protected:
  eModelType m_ModelType = eModelType::HumanAdultWholeBody;
};
