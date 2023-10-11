/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
#include "cdm/system/SESystem.h"

class CDM_DECL SEEquipment : public SESystem
{
public:

  SEEquipment(Logger* logger);
  virtual ~SEEquipment();

  void Clear() override;// Deletes all members
};
