/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PROTO_PUSH
#include "bind/cdm/PatientEnums.pb.h"
#include "bind/cdm/AnesthesiaMachineEnums.pb.h"
PROTO_POP

class CDM_DECL SEEventHandler
{
public:
  SEEventHandler() {};
  virtual ~SEEventHandler(){};

  virtual void HandlePatientEvent(cdm::ePatient_Event type, bool active, const SEScalarTime* time = nullptr)=0;
  virtual void HandleAnesthesiaMachineEvent(cdm::eAnesthesiaMachine_Event type, bool active, const SEScalarTime* time = nullptr) = 0;
};