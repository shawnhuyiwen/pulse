/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/human_adult/ventilation_mechanics/Engine.h"
#include "engine/human_adult/whole_body/controller/CompartmentManager.h"

namespace HUMAN_ADULT_VENT_MECH
{
  /**
  * @brief Manages all compartments and graphs associated with all %Pulse systems/equipment
  */
  class PULSE_DECL CompartmentManager : public PULSE_ENGINE::CompartmentManager
  {
  public:
    CompartmentManager(PULSE_ENGINE::Data& data);
    virtual ~CompartmentManager();

    virtual void StateChange() override;
  };
}