/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/ventilation_mechanics/Engine.h"
#include "engine/human_adult/ventilation_mechanics/controller/Controller.h"

namespace HUMAN_ADULT_VENT_MECH
{
  Engine::Engine(Logger* logger) : HUMAN_ADULT_WHOLE_BODY::Engine(logger)
  {
    
  }
  void Engine::AllocateController() const
  {
    m_Controller = new Controller(GetLogger());
    ((Controller*)m_Controller)->Allocate();
  }
}
