/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SETensionPneumothorax.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SETensionPneumothorax::SETensionPneumothorax(Logger* logger) : SEPatientAction(logger)
{
  m_Type=eGate::NullGate;
  m_Side=eSide::NullSide;
  m_Severity=nullptr;
}

SETensionPneumothorax::~SETensionPneumothorax()
{
  m_Type = eGate::NullGate;
  m_Side = eSide::NullSide;
  SAFE_DELETE(m_Severity);
}

void SETensionPneumothorax::Clear()
{
  SEPatientAction::Clear();
  m_Type= eGate::NullGate;
  m_Side= eSide::NullSide;
  INVALIDATE_PROPERTY(m_Severity);
}

void SETensionPneumothorax::Copy(const SETensionPneumothorax& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SETensionPneumothorax::IsValid() const
{
  return SEPatientAction::IsValid() && HasType() && HasSide() && HasSeverity();
}

bool SETensionPneumothorax::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return !m_Severity->IsZero();
}
void SETensionPneumothorax::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SETensionPneumothorax::GetScalar(const std::string& name)
{
  if (name.compare("Severity") == 0)
    return &GetSeverity();
  return nullptr;
}

eGate SETensionPneumothorax::GetType() const
{
  return m_Type;
}
void SETensionPneumothorax::SetType(eGate Type)
{
  m_Type = Type;
}
bool SETensionPneumothorax::HasType() const
{
  return m_Type== eGate::NullGate ?false:true;
}
void SETensionPneumothorax::InvalidateType()
{
  m_Type = eGate::NullGate;
}

eSide SETensionPneumothorax::GetSide() const
{
  return m_Side;
}
void SETensionPneumothorax::SetSide(eSide Side)
{
  m_Side = Side;
}
bool SETensionPneumothorax::HasSide() const
{
  return m_Side== eSide::NullSide ?false:true;
}
void SETensionPneumothorax::InvalidateSide()
{
  m_Side = eSide::NullSide;
}

bool SETensionPneumothorax::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SETensionPneumothorax::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SETensionPneumothorax::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SETensionPneumothorax::ToString(std::ostream &str) const
{
  str << "Patient Action : Tension Pneumothorax"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "NaN";
  str  << "\n\tType: "; HasType()? str << eGate_Name(GetType()) : str << "Not Set";
  str  << "\n\tSide: "; HasSide()? str << eSide_Name(GetSide()) : str << "Not Set";
  str  << std::flush;
}