/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "scenario/SEScenario.h"
class PulseConfiguration;

/**
* @brief A Pulse specific scenario (i.e. holds a %Pulse configuration object)
*/
class PULSE_DECL PulseScenario : public SEScenario
{
  friend class PBPulse;//friend the serialization class
public:

  PulseScenario(Logger* logger, std::string const& dataDir = "./");
  virtual ~PulseScenario();

  virtual void Clear();
  virtual void Copy(const PulseScenario& src);

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

  virtual PulseConfiguration& GetConfiguration();
  virtual const PulseConfiguration* GetConfiguration() const;
  virtual bool HasConfiguration() const;
  virtual void InvalidateConfiguration();

protected:
  PulseConfiguration*         m_Configuration;
};