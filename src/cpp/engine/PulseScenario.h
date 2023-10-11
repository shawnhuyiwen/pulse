/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/CommonDefs.h"
#include "engine/PulseConfiguration.h"
#include "cdm/scenario/SEScenario.h"

class PBScenario;
/**
* @brief A Pulse specific scenario (i.e. holds a %Pulse configuration object)
*/
class PULSE_DECL PulseScenario : public SEScenario
{
  friend PBScenario;//friend the serialization class
public:

  PulseScenario(std::string const& dataDir = "./");
  PulseScenario(Logger* logger, std::string const& dataDir = "./");
  virtual ~PulseScenario();

  void Clear() override;
  using SEScenario::Copy;
  void Copy(const PulseScenario& src);

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

  virtual PulseConfiguration& GetConfiguration();
  virtual const PulseConfiguration* GetConfiguration() const;
  virtual bool HasConfiguration() const;
  virtual void InvalidateConfiguration();

protected:
  PulseConfiguration* m_Configuration;
};