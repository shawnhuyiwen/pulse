/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

#include "SEInhalerAction.h"
class SEInhaler;

class CDM_DECL SEInhalerConfiguration : public SEInhalerAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEInhalerConfiguration(Logger* logger=nullptr);
  virtual ~SEInhalerConfiguration();

  virtual void Clear();
  virtual void Copy(const SEInhalerConfiguration& src, const SESubstanceManager& subMgr);
  
  virtual bool IsValid() const;

  bool HasConfiguration() const;
  SEInhaler& GetConfiguration();
  const SEInhaler* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual void ToString(std::ostream &str) const;

protected:

  std::string m_ConfigurationFile;
  SEInhaler*  m_Configuration;
};
