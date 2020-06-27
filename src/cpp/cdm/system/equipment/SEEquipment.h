/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
#include "system/SESystem.h"

class CDM_DECL SEEquipment : public SESystem
{
public:

  SEEquipment(Logger* logger);
  virtual ~SEEquipment();

  virtual void Clear();// Deletes all members
};
