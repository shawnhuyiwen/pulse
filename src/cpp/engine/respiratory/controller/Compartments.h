/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseData;
#include "cpm/controller/Compartments.h"

/**
* @brief Manages all compartments and graphs associated with all %Pulse systems/equipement
*/
class PULSE_DECL Compartments : public PulseCompartments
{
  friend class PulseSubstances;
public:

  Compartments(PulseData& data);
  virtual ~Compartments();

  virtual void StateChange() override;
};