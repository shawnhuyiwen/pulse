/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PULSE_BIND_DECL(Inhaler)
PULSE_BIND_DECL(ElectroCardioGram)
PULSE_BIND_DECL(AnesthesiaMachine)


class PULSE_DECL PBPulseEquipment
{
public:

  static void Load(const pulse::proto::AnesthesiaMachineData& src, AnesthesiaMachine& dst);
  static pulse::proto::AnesthesiaMachineData* Unload(const AnesthesiaMachine& src);
  static void Serialize(const pulse::proto::AnesthesiaMachineData& src, AnesthesiaMachine& dst);
  static void Serialize(const AnesthesiaMachine& src, pulse::proto::AnesthesiaMachineData& dst);

  static void Load(const pulse::proto::ElectroCardioGramData& src, ECG& dst);
  static pulse::proto::ElectroCardioGramData* Unload(const ECG& src);
  static void Serialize(const pulse::proto::ElectroCardioGramData& src, ECG& dst);
  static void Serialize(const ECG& src, pulse::proto::ElectroCardioGramData& dst);

  static void Load(const pulse::proto::InhalerData& src, Inhaler& dst);
  static pulse::proto::InhalerData* Unload(const Inhaler& src);
  static void Serialize(const pulse::proto::InhalerData& src, Inhaler& dst);
  static void Serialize(const Inhaler& src, pulse::proto::InhalerData& dst);

};