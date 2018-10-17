/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "bind/cdm/PatientActions.pb.h"

SEChestOcclusiveDressing::SEChestOcclusiveDressing() : SEPatientAction()
{
  m_State = eSwitch::Off;
  m_Side= eSide::NullSide;
}

SEChestOcclusiveDressing::~SEChestOcclusiveDressing()
{
  Clear();
}

void SEChestOcclusiveDressing::Clear()
{
  SEPatientAction::Clear();
  m_State= eSwitch::Off;
  m_Side= eSide::NullSide;
}

bool SEChestOcclusiveDressing::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SEChestOcclusiveDressing::IsActive() const
{
  return IsValid() && m_State == eSwitch::On;
}

void SEChestOcclusiveDressing::Load(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst)
{
  SEChestOcclusiveDressing::Serialize(src, dst);
}
void SEChestOcclusiveDressing::Serialize(const cdm::ChestOcclusiveDressingData& src, SEChestOcclusiveDressing& dst)
{
  SEPatientAction::Serialize(src.patientaction(), dst);
  dst.SetSide((eSide)src.side());
  if (src.state() != cdm::eSwitch::NullSwitch)
    dst.SetState((eSwitch)src.state());
}

cdm::ChestOcclusiveDressingData* SEChestOcclusiveDressing::Unload(const SEChestOcclusiveDressing& src)
{
  cdm::ChestOcclusiveDressingData* dst = new cdm::ChestOcclusiveDressingData();
  SEChestOcclusiveDressing::Serialize(src, *dst);
  return dst;
}
void SEChestOcclusiveDressing::Serialize(const SEChestOcclusiveDressing& src, cdm::ChestOcclusiveDressingData& dst)
{
  SEPatientAction::Serialize(src, *dst.mutable_patientaction());
  dst.set_state((cdm::eSwitch)src.m_State);
  if (src.HasSide())
    dst.set_side((cdm::eSide)src.m_Side);
}

eSide SEChestOcclusiveDressing::GetSide() const
{
  return m_Side;
}
void SEChestOcclusiveDressing::SetSide(eSide Side)
{
  m_Side = Side;
}
bool SEChestOcclusiveDressing::HasSide() const
{
  return m_Side== eSide::NullSide ?false:true;
}
void SEChestOcclusiveDressing::InvalidateSide()
{
  m_Side = eSide::NullSide;
}

void SEChestOcclusiveDressing::ToString(std::ostream &str) const
{
  str << "Patient Action : Chest Occlusive Dressing"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);
  str << "\n\tSide: "; HasSide()? str << eSide_Name(GetSide()) : str << "Not Set"; 
  str<<std::flush;
}