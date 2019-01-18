/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include <google/protobuf/message.h>

class CDM_DECL PBUtils
{
public:

  static bool SerializeFromString(const std::string& src, google::protobuf::Message& dst, SerializationFormat m, Logger* logger);
  static bool SerializeToString(const google::protobuf::Message& src, std::string& output, SerializationFormat m, Logger* logger);

  static void ProtobufLogHandler(google::protobuf::LogLevel level, const char* filename, int line, const std::string& message);

};