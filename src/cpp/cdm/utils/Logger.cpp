/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEngine.h"
#include <cstring>

bool LogMessages::SerializeToString(const LogMessages& msgs, std::string& output, SerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeToString(msgs, output, m, logger);
}
bool LogMessages::SerializeFromString(const std::string& src, LogMessages& msgs, SerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeFromString(src, msgs, m, logger);
}

std::string cdm::to_string(double d)
{
  return to_string(float(d));
}
std::string cdm::to_string(float f)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%g", f);
  return std::string(buffer);
}