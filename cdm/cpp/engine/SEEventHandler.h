/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/SEPatient.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachine.h"

class CDM_DECL SEEventHandler
{
public:
  SEEventHandler() {};
  virtual ~SEEventHandler(){};

  virtual void HandlePatientEvent(ePatient_Event type, bool active, const SEScalarTime* time = nullptr)=0;
  virtual void HandleAnesthesiaMachineEvent(eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr) = 0;
};