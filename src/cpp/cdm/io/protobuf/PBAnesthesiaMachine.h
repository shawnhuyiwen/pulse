/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL2(AnesthesiaMachine)
CDM_BIND_DECL2(AnesthesiaMachineChamber)
CDM_BIND_DECL2(AnesthesiaMachineOxygenBottle)

class CDM_DECL PBAnesthesiaMachine
{
public:

  static void Load(const CDM_BIND::AnesthesiaMachineData& src, SEAnesthesiaMachine& dst);
  static CDM_BIND::AnesthesiaMachineData* Unload(const SEAnesthesiaMachine& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineData& src, SEAnesthesiaMachine& dst);
  static void Serialize(const SEAnesthesiaMachine& src, CDM_BIND::AnesthesiaMachineData& dst);

  static bool SerializeToString(const SEAnesthesiaMachine& src, std::string& output, SerializationFormat m);
  static bool SerializeToFile(const SEAnesthesiaMachine& src, const std::string& filename, SerializationFormat m);
  static bool SerializeFromString(const std::string& src, SEAnesthesiaMachine& dst, SerializationFormat m);
  static bool SerializeFromFile(const std::string& filename, SEAnesthesiaMachine& dst, SerializationFormat m);

  static void Load(const CDM_BIND::AnesthesiaMachineChamberData& src, SEAnesthesiaMachineChamber& dst);
  static CDM_BIND::AnesthesiaMachineChamberData* Unload(const SEAnesthesiaMachineChamber& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineChamberData& src, SEAnesthesiaMachineChamber& dst);
  static void Serialize(const SEAnesthesiaMachineChamber& src, CDM_BIND::AnesthesiaMachineChamberData& dst);

  static void Load(const CDM_BIND::AnesthesiaMachineOxygenBottleData& src, SEAnesthesiaMachineOxygenBottle& dst);
  static CDM_BIND::AnesthesiaMachineOxygenBottleData* Unload(const SEAnesthesiaMachineOxygenBottle& src);
  static void Serialize(const CDM_BIND::AnesthesiaMachineOxygenBottleData& src, SEAnesthesiaMachineOxygenBottle& dst);
  static void Serialize(const SEAnesthesiaMachineOxygenBottle& src, CDM_BIND::AnesthesiaMachineOxygenBottleData& dst);

};