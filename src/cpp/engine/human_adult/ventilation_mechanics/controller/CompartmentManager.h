/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/human_adult/ventilation_mechanics/Engine.h"
#include "engine/human_adult/whole_body/controller/CompartmentManager.h"

namespace pulse { namespace human_adult_ventilation_mechanics
{
  /**
  * @brief Manages all compartments and graphs associated with all %Pulse systems/equipment
  */
  class PULSE_DECL CompartmentManager : public pulse::CompartmentManager
  {
  public:
    CompartmentManager(pulse::Data& data);
    virtual ~CompartmentManager();

    virtual void StateChange() override;
  };
END_NAMESPACE_EX
