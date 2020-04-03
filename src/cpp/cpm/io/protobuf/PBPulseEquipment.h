/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PULSE_BIND_DECL(Inhaler)
PULSE_BIND_DECL(ElectroCardioGram)
PULSE_BIND_DECL(AnesthesiaMachine)
PULSE_BIND_DECL(MechanicalVentilator)

class PULSE_DECL PBPulseEquipment
{
public:

  static void Load(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachine& dst);
  static PULSE_BIND::AnesthesiaMachineData* Unload(const AnesthesiaMachine& src);
  static void Serialize(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachine& dst);
  static void Serialize(const AnesthesiaMachine& src, PULSE_BIND::AnesthesiaMachineData& dst);

  static void Load(const PULSE_BIND::ElectroCardioGramData& src, ECG& dst);
  static PULSE_BIND::ElectroCardioGramData* Unload(const ECG& src);
  static void Serialize(const PULSE_BIND::ElectroCardioGramData& src, ECG& dst);
  static void Serialize(const ECG& src, PULSE_BIND::ElectroCardioGramData& dst);

  static void Load(const PULSE_BIND::InhalerData& src, Inhaler& dst);
  static PULSE_BIND::InhalerData* Unload(const Inhaler& src);
  static void Serialize(const PULSE_BIND::InhalerData& src, Inhaler& dst);
  static void Serialize(const Inhaler& src, PULSE_BIND::InhalerData& dst);

  static void Load(const PULSE_BIND::MechanicalVentilatorData& src, MechanicalVentilator& dst);
  static PULSE_BIND::MechanicalVentilatorData* Unload(const MechanicalVentilator& src);
  static void Serialize(const PULSE_BIND::MechanicalVentilatorData& src, MechanicalVentilator& dst);
  static void Serialize(const MechanicalVentilator& src, PULSE_BIND::MechanicalVentilatorData& dst);

};