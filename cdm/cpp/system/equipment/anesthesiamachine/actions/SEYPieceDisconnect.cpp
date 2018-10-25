/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesiamachine/actions/SEYPieceDisconnect.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBAnesthesiaMachineActions.h"

SEYPieceDisconnect::SEYPieceDisconnect() : SEAnesthesiaMachineAction()
{
  m_Severity=nullptr;
}

SEYPieceDisconnect::~SEYPieceDisconnect()
{
  Clear();
}

void SEYPieceDisconnect::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEYPieceDisconnect::Copy(const SEYPieceDisconnect& src)
{// Using Bindings to make a copy
  PBAnesthesiaMachineAction::Copy(src, *this);
}

bool SEYPieceDisconnect::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEYPieceDisconnect::IsActive() const
{
  return HasSeverity() ? !m_Severity->IsZero() : false;
}

bool SEYPieceDisconnect::HasSeverity() const
{
  return m_Severity != nullptr;
}
SEScalar0To1& SEYPieceDisconnect::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
double SEYPieceDisconnect::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEYPieceDisconnect::ToString(std::ostream &str) const
{
  str << "Anesthesia Machine Action : YPiece Disconnect"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity :"; HasSeverity() ? str << *m_Severity : str << "NaN";
  str << std::flush;
}