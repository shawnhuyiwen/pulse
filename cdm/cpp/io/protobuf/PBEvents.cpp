/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "bind/cpp/cdm/Events.pb.h"
#include "engine/SEEventManager.h"
#include "utils/FileUtils.h"

const std::string& eEvent_Name(eEvent m)
{
  return cdm::eEvent_Name((cdm::eEvent)m);
}