/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#include "stdafx.h"
#include "system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineYPieceDisconnect.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBEquipmentActions.h"

SEAnesthesiaMachineYPieceDisconnect::SEAnesthesiaMachineYPieceDisconnect(Logger* logger) : SEAnesthesiaMachineAction(logger)
{
  m_Severity=nullptr;
}

SEAnesthesiaMachineYPieceDisconnect::~SEAnesthesiaMachineYPieceDisconnect()
{
  SAFE_DELETE(m_Severity);
}

void SEAnesthesiaMachineYPieceDisconnect::Clear()
{
  SEAnesthesiaMachineAction::Clear();
  INVALIDATE_PROPERTY(m_Severity);
}

void SEAnesthesiaMachineYPieceDisconnect::Copy(const SEAnesthesiaMachineYPieceDisconnect& src, bool preserveState)
{// Using Bindings to make a copy
  PBEquipmentAction::Copy(src, *this);
}

bool SEAnesthesiaMachineYPieceDisconnect::IsValid() const
{
  return SEAnesthesiaMachineAction::IsValid() && HasSeverity();
}

bool SEAnesthesiaMachineYPieceDisconnect::IsActive() const
{
  if (!SEAnesthesiaMachineAction::IsActive())
    return false;
  return HasSeverity() ? !m_Severity->IsZero() : false;
}
void SEAnesthesiaMachineYPieceDisconnect::Deactivate()
{
  SEAnesthesiaMachineAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEAnesthesiaMachineYPieceDisconnect::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
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