/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/ventilation_mechanics/Engine.h"
#include "engine/human_adult/ventilation_mechanics/controller/Controller.h"

namespace pulse { namespace human_adult_ventilation_mechanics
{
  Engine::Engine(Logger* logger) : pulse::human_adult_whole_body::Engine(logger)
  {
    
  }
  void Engine::AllocateController() const
  {
    m_Controller = new Controller(m_Logger);
    ((Controller*)m_Controller)->Allocate();
  }
END_NAMESPACE_EX
