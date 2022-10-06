/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/utils/FileUtils.h"

std::mutex log_mutex;
Logger* g_logger = nullptr;

void PBUtils::ProtobufLogHandler(google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
{
  if (g_logger != nullptr)
  {
    switch (level)
    {
    case google::protobuf::LOGLEVEL_INFO:
      g_logger->Info(message);
      break;
      // Going to treat everything else as a warning, as Pulse will try to handle Errors/Fatal messages
      // Pulse will report an error/fatal when it feels it did everything it could but still is unable to serialize
    case google::protobuf::LOGLEVEL_WARNING:
      g_logger->Warning(message);
      break;
    case google::protobuf::LOGLEVEL_ERROR:
      g_logger->Warning(message);
      break;
    case google::protobuf::LOGLEVEL_FATAL:
      g_logger->Warning(message);
      break;
    }
  }
  else
    std::cerr << message << std::endl;

  g_logger = nullptr;
}

bool PBUtils::SerializeFromFile(const std::string& filename, google::protobuf::Message& dst, Logger* logger)
{
  if (!FileExists(filename))
    return false;
  if (IsJSONFile(filename))
  {
    std::string content;
    if (!ReadFile(filename, content))
      return false;
    if (!SerializeFromString(content, dst, eSerializationFormat::JSON, logger))
      return false;
    return true;
  }
  std::ifstream input(filename, std::ios::binary);
  if (!input.is_open())
    return false;
  std::scoped_lock<std::mutex> guard(log_mutex);
  g_logger = logger;
  google::protobuf::SetLogHandler(static_cast<google::protobuf::LogHandler*>(PBUtils::ProtobufLogHandler));
  bool b = dst.ParseFromIstream(&input);
  g_logger = nullptr;
  input.close();
  return b;
}

bool PBUtils::SerializeToFile(const google::protobuf::Message& src, const std::string& filename, Logger* logger)
{
  if (IsJSONFile(filename))
  {
    std::string content;
    if (!SerializeToString(src, content, eSerializationFormat::JSON, logger))
      return false;
    return WriteFile(content, filename);
  }
  if (!CreateFilePath(filename))
    return false;
  std::scoped_lock<std::mutex> guard(log_mutex);
  g_logger = logger;
  google::protobuf::SetLogHandler(static_cast<google::protobuf::LogHandler*>(PBUtils::ProtobufLogHandler));
  std::ofstream output(filename, std::ios::binary);
  bool b = src.SerializeToOstream(&output);
  g_logger = nullptr;
  output.close();
  return b;
}


bool PBUtils::SerializeFromString(const std::string& src, google::protobuf::Message& dst, eSerializationFormat m, Logger* logger)
{
  std::scoped_lock<std::mutex> guard(log_mutex);

  g_logger = logger;
  google::protobuf::SetLogHandler(static_cast<google::protobuf::LogHandler*>(PBUtils::ProtobufLogHandler));

  bool ret = true;
  if (m == eSerializationFormat::JSON)
  {
    google::protobuf::util::JsonParseOptions opts;
    google::protobuf::util::Status stat = google::protobuf::util::JsonStringToMessage(src, &dst, opts);
    if (!stat.ok())
    {
      if (logger != nullptr)
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
  }
  else if (m == eSerializationFormat::TEXT)
  {
    google::protobuf::TextFormat::ParseFromString(src, &dst);
  }
  else
  {
    ret = dst.ParseFromString(src);
  }
  g_logger = nullptr;
  return ret;
}

bool PBUtils::SerializeToString(const google::protobuf::Message& src, std::string& output, eSerializationFormat m, Logger* logger)
{
  std::scoped_lock<std::mutex> guard(log_mutex);

  g_logger = logger;
  google::protobuf::SetLogHandler(static_cast<google::protobuf::LogHandler*>(PBUtils::ProtobufLogHandler));

  bool ret = true;
  if (m == eSerializationFormat::JSON)
  {
    google::protobuf::util::JsonPrintOptions opts;
    opts.add_whitespace = true;
    opts.preserve_proto_field_names = true;
    //opts.always_print_primitive_fields = true;
    ret = google::protobuf::util::MessageToJsonString(src, &output, opts).ok();
  }
  else if (m == eSerializationFormat::TEXT)
  {
    ret = google::protobuf::TextFormat::PrintToString(src, &output);
  }
  else
  {
    ret = src.SerializeToString(&output);
  }
  g_logger = nullptr;
  return ret;
}
