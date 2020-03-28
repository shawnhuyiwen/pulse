/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEAnesthesiaMachineYPieceDisconnect::SEAnesthesiaMachineYPieceDisconnect() : SEAnesthesiaMachineAction()
{
  m_Severity=nullptr;
}

SEAnesthesiaMachineYPieceDisconnect::~SEAnesthesiaMachineYPieceDisconnect()
{
  Clear();
}

void SEAnesthesiaMachineYPieceDisconnect::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineYPieceDisconnect::Copy(const SEAnesthesiaMachineYPieceDisconnect& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEAnesthesiaMachineYPieceDisconnect::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineYPieceDisconnect::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEAnesthesiaMachineYPieceDisconnect::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEAnesthesiaMachineYPieceDisconnect::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEAnesthesiaMachineYPieceDisconnect::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAnesthesiaMachineYPieceDisconnect::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : YPiece Disconnect"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}