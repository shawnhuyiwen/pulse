/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEAirwayObstruction.h"
#include "properties/SEScalar0To1.h"
#include "io/protobuf/PBPatientActions.h"

SEAirwayObstruction::SEAirwayObstruction() : SEPatientAction()
{
  m_Severity=nullptr;
}

SEAirwayObstruction::~SEAirwayObstruction()
{
  Clear();
}

void SEAirwayObstruction::Clear()
{
  SEPatientAction::Clear();
  SAFE_DELETE(m_Severity);
}

void SEAirwayObstruction::Copy(const SEAirwayObstruction& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SEAirwayObstruction::IsValid() const
{
  return SEPatientAction::IsValid() && HasSeverity();
}

bool SEAirwayObstruction::IsActive() const
{
  return IsValid() ? !m_Severity->IsZero() : false;
}

bool SEAirwayObstruction::HasSeverity() const
{
  return m_Severity==nullptr?false:m_Severity->IsValid();
}
SEScalar0To1& SEAirwayObstruction::GetSeverity()
{
  if(m_Severity==nullptr)
    m_Severity=new SEScalar0To1();
  return *m_Severity;
}
double SEAirwayObstruction::GetSeverity() const
{
  if (m_Severity == nullptr)
    return SEScalar::dNaN();
  return m_Severity->GetValue();
}

void SEAirwayObstruction::ToString(std::ostream &str) const
{
  str << "Patient Action : Airway Obstruction"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tSeverity: "; HasSeverity() ? str << *m_Severity : str << "Not Set";
  str << std::flush;
}