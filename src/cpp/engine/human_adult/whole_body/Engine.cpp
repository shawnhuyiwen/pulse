/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/human_adult/whole_body/Engine.h"
#include "engine/human_adult/whole_body/controller/Controller.h"

namespace pulse { namespace human_adult_whole_body
{
  Engine::Engine(Logger* logger) : pulse::Engine(logger)
  {
    
  }
  void Engine::AllocateController() const
  {
    m_Controller = new Controller(GetLogger());
    ((Controller*)m_Controller)->Allocate();
  }
END_NAMESPACE_EX
