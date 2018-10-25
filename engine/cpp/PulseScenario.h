/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "scenario/SEScenario.h"
class PulseConfiguration;
PULSE_BIND_DECL(ScenarioData)

/**
* @brief A Pulse specific scenario (i.e. holds a %Pulse configuration object)
*/
class PULSE_DECL PulseScenario : public SEScenario
{
  friend class PBPulse;//friend the serialization class
public:

  PulseScenario(SESubstanceManager& subMgr);
  virtual ~PulseScenario();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationMode m) const;
  bool SerializeToFile(const std::string& filename, SerializationMode m) const;
  bool SerializeFromString(const std::string& src, SerializationMode m);
  bool SerializeFromFile(const std::string& filename, SerializationMode m);

  virtual PulseConfiguration& GetConfiguration();
  virtual const PulseConfiguration* GetConfiguration() const;
  virtual bool HasConfiguration() const;
  virtual void InvalidateConfiguration();

protected:
  PulseConfiguration*         m_Configuration;
};