/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SECardiacArrest.h"
#include "bind/cdm/PatientActions.pb.h"

SECardiacArrest::SECardiacArrest() : SEPatientAction()
{
  m_State = eSwitch::Off;
}

SECardiacArrest::~SECardiacArrest()
{
  Clear();
}

void SECardiacArrest::Clear()
{
  SEPatientAction::Clear();
  m_State = eSwitch::Off;
}

bool SECardiacArrest::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SECardiacArrest::IsActive() const
{
  return IsValid() && m_State == eSwitch::On;
}

void SECardiacArrest::Load(const cdm::CardiacArrestData& src, SECardiacArrest& dst)
{
  SECardiacArrest::Serialize(src, dst);
}
void SECardiacArrest::Serialize(const cdm::CardiacArrestData& src, SECardiacArrest& dst)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
}

cdm::CardiacArrestData* SECardiacArrest::Unload(const SECardiacArrest& src)
{
  cdm::CardiacArrestData* dst = new cdm::CardiacArrestData();
  SECardiacArrest::Serialize(src, *dst);
  return dst;
}
void SECardiacArrest::Serialize(const SECardiacArrest& src, cdm::CardiacArrestData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_state((cdm::eSwitch)src.m_State);
}

void SECardiacArrest::ToString(std::ostream &str) const
{
  str << "Patient Action : Cardiac Arrest";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);;
  str << std::flush;
}