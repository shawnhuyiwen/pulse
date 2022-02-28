/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once
#include "SEArray.h"

class CDM_DECL SEArrayElectricPotential : public SEArray
{
  friend class PBProperty;//friend the serialization class
public:
  SEArrayElectricPotential() = default;
  ~SEArrayElectricPotential() = default;

  void Copy(const SEArray& src) = delete;
  void Copy(const SEArrayElectricPotential& src);

  const ElectricPotentialUnit* GetUnit() const { return m_ElectricPotentialUnit; }

protected:
  const ElectricPotentialUnit* m_ElectricPotentialUnit = nullptr;
};
