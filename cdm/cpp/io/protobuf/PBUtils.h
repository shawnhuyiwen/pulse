/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
namespace google { namespace protobuf { class Message; } }

class CDM_DECL PBUtils
{
public:

  static bool SerializeFromString(const std::string& src, google::protobuf::Message& dst, SerializationFormat m);
  static bool SerializeToString(const google::protobuf::Message& src, std::string& output, SerializationFormat m);
};