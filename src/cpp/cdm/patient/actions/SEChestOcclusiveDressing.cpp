/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestOcclusiveDressing.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestOcclusiveDressing::SEChestOcclusiveDressing(Logger* logger) : SEPatientAction(logger)
{
  m_State = eSwitch::Off;
  m_Side= eSide::NullSide;
}

SEChestOcclusiveDressing::~SEChestOcclusiveDressing()
{
  m_State = eSwitch::Off;
  m_Side = eSide::NullSide;
}

void SEChestOcclusiveDressing::Clear()
{
  SEPatientAction::Clear();
  m_State= eSwitch::Off;
  m_Side= eSide::NullSide;
}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
void SEChestOcclusiveDressing::Copy(const SEChestOcclusiveDressing& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}
DISABLE_WARNING_POP

bool SEChestOcclusiveDressing::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SEChestOcclusiveDressing::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return m_State == eSwitch::On;
}
void SEChestOcclusiveDressing::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

DISABLE_UNREFERENCED_FORMAL_PARAMETER
const SEScalar* SEChestOcclusiveDressing::GetScalar(const std::string& name)
{
  return nullptr;
}
DISABLE_WARNING_POP

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