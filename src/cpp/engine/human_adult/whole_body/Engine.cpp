/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace HUMAN_ADULT_WHOLE_BODY
{
  Engine::Engine(Logger* logger) : PULSE_ENGINE::Engine(logger)
  {
    
  }
  void Engine::AllocateController() const
  {
    m_Controller = new Controller(GetLogger());
    ((Controller*)m_Controller)->Allocate();
  }
}
