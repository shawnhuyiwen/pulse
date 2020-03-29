/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBEngine.h"

bool LogMessages::SerializeToString(const LogMessages& msgs, std::string& output, SerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeToString(msgs, output, m, logger);
}
bool LogMessages::SerializeFromString(const std::string& src, LogMessages& msgs, SerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeFromString(src, msgs, m, logger);
}