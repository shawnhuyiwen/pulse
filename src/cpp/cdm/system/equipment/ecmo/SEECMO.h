/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEScalar.h"
#include "cdm/system/equipment/SEEquipment.h"
#include "cdm/system/equipment/ecmo/SEECMOSettings.h"

class CDM_DECL SEECMO : public SEEquipment
{
  friend class PBECMO;//friend the serialization class
public:

  SEECMO(Logger* logger);
  virtual ~SEECMO();

  void Clear() override;
  virtual void TurnOff();

protected:
  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange() {};

public:

  const SEScalar* GetScalar(const std::string& name) override;

  virtual bool HasSettings() const;
  virtual SEECMOSettings& GetSettings();
  virtual const SEECMOSettings* GetSettings() const;
  virtual void RemoveSettings();

protected:

  SEECMOSettings*    m_Settings;
};
