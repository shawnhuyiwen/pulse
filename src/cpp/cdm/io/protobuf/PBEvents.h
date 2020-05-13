/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEActiveEvent;
class SEEventChange;

class CDM_DECL PBEvents
{
public:

  static bool SerializeToString(std::vector<const SEEventChange*>& changes, std::string& output, SerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, std::vector<const SEEventChange*>& changes, SerializationFormat m, Logger* logger);

  static bool SerializeToString(std::vector<const SEActiveEvent*>& active_events, std::string& output, SerializationFormat m, Logger* logger);
  static bool SerializeFromString(const std::string& src, std::vector<const SEActiveEvent*>& active_events, SerializationFormat m, Logger* logger);

};