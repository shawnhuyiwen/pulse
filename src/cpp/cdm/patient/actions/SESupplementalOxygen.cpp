/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SESupplementalOxygen.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "io/protobuf/PBPatientActions.h"

SESupplementalOxygen::SESupplementalOxygen(Logger* logger) : SEPatientAction(logger)
{
  m_Device = eSupplementalOxygen_Device::None;
  m_Flow=nullptr;
  m_Volume=nullptr;
}

SESupplementalOxygen::~SESupplementalOxygen()
{
  Clear();
}

void SESupplementalOxygen::Clear()
{
  SEPatientAction::Clear();
  m_Device = eSupplementalOxygen_Device::None;
  SAFE_DELETE(m_Flow);
  SAFE_DELETE(m_Volume);
}

void SESupplementalOxygen::Copy(const SESupplementalOxygen& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SESupplementalOxygen::IsValid() const
{
  return SEPatientAction::IsValid();
}

eSupplementalOxygen_Device SESupplementalOxygen::GetDevice() const
{
  return m_Device;
}
void SESupplementalOxygen::SetDevice(eSupplementalOxygen_Device Device)
{
  m_Device = Device;
}

bool SESupplementalOxygen::HasFlow() const
{
  return m_Flow==nullptr?false:m_Flow->IsValid();
}
SEScalarVolumePerTime& SESupplementalOxygen::GetFlow()
{
  if(m_Flow==nullptr)
    m_Flow=new SEScalarVolumePerTime();
  return *m_Flow;
}
double SESupplementalOxygen::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}

bool SESupplementalOxygen::HasVolume() const
{
  return m_Volume==nullptr?false:m_Volume->IsValid();
}
SEScalarVolume& SESupplementalOxygen::GetVolume()
{
  if(m_Volume==nullptr)
    m_Volume=new SEScalarVolume();
  return *m_Volume;
}
double SESupplementalOxygen::GetVolume(const VolumeUnit& unit) const
{
  if (m_Volume == nullptr)
    return SEScalar::dNaN();
  return m_Volume->GetValue(unit);
}

void SESupplementalOxygen::ToString(std::ostream &str) const
{
  str << "Patient Action : Supplemental Oxygen"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tDevice: " << eSupplementalOxygen_Device_Name(GetDevice());
  str << "\n\tFlow: "; HasFlow() ? str << *m_Flow : str << "NaN";
  str << "\n\tVolume: "; HasVolume() ? str << *m_Volume : str << "NaN";
  str  <<std::flush;
}