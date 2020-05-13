/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
PUSH_PROTO_WARNINGS()
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS()
#include "io/protobuf/PBUtils.h"

std::mutex log_mutex;
Logger* g_logger = nullptr;

void PBUtils::ProtobufLogHandler(google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
{
  std::stringstream ss;
  ss << filename << "::" << line;
  switch (level)
  {
  case google::protobuf::LOGLEVEL_INFO:
    g_logger->Info(message, ss.str());
    break;
  // Going to treat everything else as a warning, as Pulse will try to handle Errors/Fatal messages
  // Pulse will report an error/fatal when it feels it did everything it could but still is unable to serialize
  case google::protobuf::LOGLEVEL_WARNING:
    g_logger->Warning(message, ss.str());
    break;
  case google::protobuf::LOGLEVEL_ERROR:
    g_logger->Warning(message, ss.str());
    break;
  case google::protobuf::LOGLEVEL_FATAL:
    g_logger->Warning(message, ss.str());
    break;
  }
  g_logger = nullptr;
}

bool PBUtils::SerializeToString(const google::protobuf::Message& src, std::string& output, SerializationFormat m, Logger* logger)
{
  std::lock_guard<std::mutex> guard(log_mutex);
  if (m == JSON)
  {
    g_logger = logger;
    google::protobuf::SetLogHandler(static_cast<google::protobuf::LogHandler*>(PBUtils::ProtobufLogHandler));
    google::protobuf::util::JsonPrintOptions opts;
    opts.add_whitespace = true;
    opts.preserve_proto_field_names = true;
    bool ret =  google::protobuf::util::MessageToJsonString(src, &output, opts).ok();
    g_logger = nullptr;
    return ret;
    //return google::protobuf::TextFormat::PrintToString(data, &output);
  }
  else
    return src.SerializeToString(&output);
}

bool PBUtils::SerializeFromString(const std::string& src, google::protobuf::Message& dst, SerializationFormat m, Logger* logger)
{
  std::lock_guard<std::mutex> guard(log_mutex);
  if (m == JSON)
  {
    bool ret = true;
    g_logger = logger;
    google::protobuf::SetLogHandler(static_cast<google::protobuf::LogHandler*>(PBUtils::ProtobufLogHandler));
    google::protobuf::util::JsonParseOptions opts;
    google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, opts);
    if (!stat.ok())
    {
      if(logger != nullptr)
        logger->Warning("Protobuf " + stat.ToString());
      else
        std::cerr << stat.ToString() << std::endl;
      ret = false;
      // Old code to check if the string is in the old google text format
      // We generally do not support this, but if there is some weird reason
      // Here is how you would do that
      // logger->Info("Checking to see if string is in google text format...");
      // ret = google::protobuf::TextFormat::ParseFromString(src, &dst);
    }
    g_logger = nullptr;
    return ret;
  }
  else
  {
    return dst.ParseFromString(src);
  }
}
