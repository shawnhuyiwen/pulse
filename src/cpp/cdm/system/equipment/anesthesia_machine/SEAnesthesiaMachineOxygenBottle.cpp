/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/system/equipment/anesthesia_machine/SEAnesthesiaMachineOxygenBottle.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalarVolume.h"

SEAnesthesiaMachineOxygenBottle::SEAnesthesiaMachineOxygenBottle(Logger* logger) : Loggable(logger)
{
  m_Volume = nullptr;
}

SEAnesthesiaMachineOxygenBottle::~SEAnesthesiaMachineOxygenBottle()
{
  SAFE_DELETE(m_Volume);
}

void SEAnesthesiaMachineOxygenBottle::Clear()
{
  INVALIDATE_PROPERTY(m_Volume);
}

void SEAnesthesiaMachineOxygenBottle::Merge(const SEAnesthesiaMachineOxygenBottle& from)
{
  COPY_PROPERTY(Volume);
}

const SEScalar* SEAnesthesiaMachineOxygenBottle::GetScalar(const std::string& name)
{
  if (name == "Volume")
    return &GetVolume();
  return nullptr;
}

bool SEAnesthesiaMachineOxygenBottle::HasVolume() const
{
  return m_Volume==nullptr?false:m_Volume->IsValid();
}

SEScalarVolume& SEAnesthesiaMachineOxygenBottle::GetVolume()
{
  if(m_Volume==nullptr)
    m_Volume = new SEScalarVolume();
  return *m_Volume;
}
double SEAnesthesiaMachineOxygenBottle::GetVolume(const VolumeUnit& unit) const
{
  if (m_Volume == nullptr)
    return SEScalar::dNaN();
  return m_Volume->GetValue(unit);
}

void SEAnesthesiaMachineOxygenBottle::ToString(std::ostream &str)
{
  str<<" Anesthesia Machine Oxygen Bottle, ";
  if(HasVolume())
    str<<"\rVolume : "<<GetVolume();
  else
    str<<"\rVolume : NaN"<<GetVolume();
  str<<std::flush;
}
