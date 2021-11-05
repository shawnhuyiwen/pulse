/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEArrhythmia.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEArrhythmia::SEArrhythmia(Logger* logger) : SEPatientAction(logger)
{
  m_Type = eHeartRhythm::NormalSinus;
}

SEArrhythmia::~SEArrhythmia()
{
  m_Type = eHeartRhythm::NormalSinus;
}

void SEArrhythmia::Clear()
{
  SEPatientAction::Clear();
  m_Type = eHeartRhythm::NormalSinus;
}

void SEArrhythmia::Copy(const SEArrhythmia& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEArrhythmia::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEArrhythmia::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
}
void SEArrhythmia::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEArrhythmia::GetScalar(const std::string& name)
{
  return nullptr;
}

eHeartRhythm SEArrhythmia::GetType() const
{
  return m_Type;
}
void SEArrhythmia::SetType(eHeartRhythm Type)
{
  m_Type = Type;
}

void SEArrhythmia::ToString(std::ostream &str) const
{
  str << "Patient Action : Arrhythmia";
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tType: "<< eHeartRhythm_Name(GetType());
  str << std::flush;
}