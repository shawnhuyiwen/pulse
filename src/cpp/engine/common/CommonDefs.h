/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PulseEngine.h"
#define PULSE_DECL CDM_DECL

#define PULSE_BIND pulse::engine::bind
#define PULSE_ENGINE pulse::engine

#define PULSE_BIND_DECL(type) \
  namespace PULSE_BIND { class type##Data; }
  
#define PULSE_ENGINE_DECL(type) \
  namespace PULSE_ENGINE { class type; }
