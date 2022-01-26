/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/io/protobuf/PBEngine.h"
#include <cstring>

bool LogMessages::SerializeToString(const LogMessages& msgs, std::string& output, eSerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeToString(msgs, output, m, logger);
}
bool LogMessages::SerializeFromString(const std::string& src, LogMessages& msgs, eSerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeFromString(src, msgs, m, logger);
}

std::string pulse::cdm::to_string(double d)
{
  return to_string(float(d));
}
std::string pulse::cdm::to_string(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}