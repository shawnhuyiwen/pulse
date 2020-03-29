/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEChestOcclusiveDressing.h"
#include "io/protobuf/PBPatientActions.h"

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

void SEChestOcclusiveDressing::Copy(const SEChestOcclusiveDressing& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEChestOcclusiveDressing::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SEChestOcclusiveDressing::IsActive() const
{
  return IsValid() && m_State == eSwitch::On;
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