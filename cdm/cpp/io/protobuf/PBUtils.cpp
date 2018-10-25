/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBUtils.h"
#include <google/protobuf/text_format.h>

void MyLogHandler(google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
{
  std::cout << message;
}

bool PBUtils::SerializeToString(const google::protobuf::Message& data, std::string& output, SerializationMode m)
{
  if (m == ASCII)
  {
    google::protobuf::SetLogHandler(MyLogHandler);
    return google::protobuf::TextFormat::PrintToString(data, &output);
  }
  else
    return data.SerializeToString(&output);
}

bool PBUtils::SerializeFromString(const std::string& src, google::protobuf::Message& dst, SerializationMode m)
{
  if (m == ASCII)
  {
    google::protobuf::SetLogHandler(MyLogHandler);
    if (!google::protobuf::TextFormat::ParseFromString(src, &dst))
      return false;
    return true;
  }
  else
  {
    return dst.ParseFromString(src);
  }
}