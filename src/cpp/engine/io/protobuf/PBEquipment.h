/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

PULSE_BIND_DECL(AnesthesiaMachine)
PULSE_BIND_DECL(BagValveMask)
PULSE_BIND_DECL(ElectroCardioGram)
PULSE_BIND_DECL(Inhaler)
PULSE_BIND_DECL(MechanicalVentilator)
#include "engine/common/system/equipment/AnesthesiaMachineModel.h"
#include "engine/common/system/equipment/BagValveMaskModel.h"
#include "engine/common/system/equipment/ElectroCardioGramModel.h"
#include "engine/common/system/equipment/InhalerModel.h"
#include "engine/common/system/equipment/MechanicalVentilatorModel.h"

namespace pulse
{
  class PULSE_DECL PBEquipment
  {
  public:

    static void Load(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachineModel& dst);
    static PULSE_BIND::AnesthesiaMachineData* Unload(const AnesthesiaMachineModel& src);
    static void Serialize(const PULSE_BIND::AnesthesiaMachineData& src, AnesthesiaMachineModel& dst);
    static void Serialize(const AnesthesiaMachineModel& src, PULSE_BIND::AnesthesiaMachineData& dst);

    static void Load(const PULSE_BIND::BagValveMaskData& src, BagValveMaskModel& dst);
    static PULSE_BIND::BagValveMaskData* Unload(const BagValveMaskModel& src);
    static void Serialize(const PULSE_BIND::BagValveMaskData& src, BagValveMaskModel& dst);
    static void Serialize(const BagValveMaskModel& src, PULSE_BIND::BagValveMaskData& dst);

    static void Load(const PULSE_BIND::ElectroCardioGramData& src, ElectroCardioGramModel& dst);
    static PULSE_BIND::ElectroCardioGramData* Unload(const ElectroCardioGramModel& src);
    static void Serialize(const PULSE_BIND::ElectroCardioGramData& src, ElectroCardioGramModel& dst);
    static void Serialize(const ElectroCardioGramModel& src, PULSE_BIND::ElectroCardioGramData& dst);

    static void Load(const PULSE_BIND::InhalerData& src, InhalerModel& dst);
    static PULSE_BIND::InhalerData* Unload(const InhalerModel& src);
    static void Serialize(const PULSE_BIND::InhalerData& src, InhalerModel& dst);
    static void Serialize(const InhalerModel& src, PULSE_BIND::InhalerData& dst);

    static void Load(const PULSE_BIND::MechanicalVentilatorData& src, MechanicalVentilatorModel& dst);
    static PULSE_BIND::MechanicalVentilatorData* Unload(const MechanicalVentilatorModel& src);
    static void Serialize(const PULSE_BIND::MechanicalVentilatorData& src, MechanicalVentilatorModel& dst);
    static void Serialize(const MechanicalVentilatorModel& src, PULSE_BIND::MechanicalVentilatorData& dst);
  };
}