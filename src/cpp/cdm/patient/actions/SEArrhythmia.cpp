/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEArrhythmia.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEArrhythmia::SEArrhythmia(Logger* logger) : SEPatientAction(logger)
{
  m_Rhythm = eHeartRhythm::NormalSinus;
}

SEArrhythmia::~SEArrhythmia()
{
  m_Rhythm = eHeartRhythm::NormalSinus;
}

void SEArrhythmia::Clear()
{
  SEPatientAction::Clear();
  m_Rhythm = eHeartRhythm::NormalSinus;
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
  return true;
}
void SEArrhythmia::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEArrhythmia::GetScalar(const std::string&)
{
  return nullptr;
}

eHeartRhythm SEArrhythmia::GetRhythm() const
{
  return m_Rhythm;
}
void SEArrhythmia::SetRhythm(eHeartRhythm r)
{
  m_Rhythm = r;
}

void SEArrhythmia::ToString(std::ostream &str) const
{
  str << "Patient Action : Arrhythmia";
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tRhythm: "<< eHeartRhythm_Name(GetRhythm());
  str << std::flush;
}