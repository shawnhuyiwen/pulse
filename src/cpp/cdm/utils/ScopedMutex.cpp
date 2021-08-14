/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
#include "cdm/utils/ScopedMutex.h"

#include <mutex>

static std::recursive_mutex g_rMutex;

ScopedMutex::ScopedMutex()
{
  g_rMutex.lock();
}

ScopedMutex::~ScopedMutex()
{
  g_rMutex.unlock();
}


