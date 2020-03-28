/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PULSE_BIND_DECL(Environment)

class PULSE_DECL PBPulseEnvironment
{
public:

  static void Load(const PULSE_BIND::EnvironmentData& src, Environment& dst);
  static PULSE_BIND::EnvironmentData* Unload(const Environment& src);
  static void Serialize(const PULSE_BIND::EnvironmentData& src, Environment& dst);
  static void Serialize(const Environment& src, PULSE_BIND::EnvironmentData& dst);

};