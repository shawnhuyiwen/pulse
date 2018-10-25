/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PULSE_BIND_DECL(Environment)

class PULSE_DECL PBPulseEnvironment
{
public:

  static void Load(const pulse::proto::EnvironmentData& src, Environment& dst);
  static pulse::proto::EnvironmentData* Unload(const Environment& src);
  static void Serialize(const pulse::proto::EnvironmentData& src, Environment& dst);
  static void Serialize(const Environment& src, pulse::proto::EnvironmentData& dst);

};