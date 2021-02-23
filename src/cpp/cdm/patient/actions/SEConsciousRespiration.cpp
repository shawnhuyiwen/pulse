/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SEConsciousRespiration.h"
#include "patient/actions/SEConsciousRespirationCommand.h"
#include "patient/actions/SEForcedPause.h"
#include "patient/actions/SEForcedExhale.h"
#include "patient/actions/SEForcedInhale.h"
#include "patient/actions/SEUseInhaler.h"
#include "io/protobuf/PBPatientActions.h"

SEConsciousRespiration::SEConsciousRespiration(Logger* logger) : SEPatientAction(logger)
{
  m_StartImmediately = false;
}

SEConsciousRespiration::~SEConsciousRespiration()
{
  DELETE_VECTOR(m_Commands);
  m_StartImmediately = false;
}

void SEConsciousRespiration::Clear()
{
  SEPatientAction::Clear();
  DELETE_VECTOR(m_Commands);
  m_StartImmediately = false;
}

void SEConsciousRespiration::Copy(const SEConsciousRespiration& src, bool preserveState)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEConsciousRespiration::IsValid() const
{
  return SEPatientAction::IsValid() && !m_Commands.empty();
}

bool SEConsciousRespiration::IsActive() const
{
  return SEPatientAction::IsActive();
}
void SEConsciousRespiration::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEConsciousRespiration::GetScalar(const std::string& name)
{
  return nullptr;
}

bool SEConsciousRespiration::StartImmediately() const
{
  return m_StartImmediately;
}
void SEConsciousRespiration::SetStartImmediately(bool b)
{
  m_StartImmediately = b;
}

bool SEConsciousRespiration::HasCommands() const
{
  return !m_Commands.empty();
}
SEConsciousRespirationCommand* SEConsciousRespiration::GetActiveCommand()
{
  if (!m_Commands.empty())
    return m_Commands[0];
  return nullptr;
}
void SEConsciousRespiration::RemoveActiveCommand()
{
  SEConsciousRespirationCommand* c = GetActiveCommand();
  if (c != nullptr)
  {
    m_Commands.erase(m_Commands.begin());
    SAFE_DELETE(c);
  }
}

SEForcedExhale&  SEConsciousRespiration::AddForcedExhale()
{
    SEForcedExhale* myEx = new SEForcedExhale(GetLogger());
    m_Commands.push_back(myEx);
    return *myEx;
}

SEForcedInhale& SEConsciousRespiration::AddForcedInhale()
{
  SEForcedInhale* myIn = new SEForcedInhale(GetLogger());
  m_Commands.push_back(myIn);
  return *myIn;
}

SEForcedPause& SEConsciousRespiration::AddForcedPause()
{
  SEForcedPause* myHold = new SEForcedPause(GetLogger());
  m_Commands.push_back(myHold);
  return *myHold;
}

SEUseInhaler& SEConsciousRespiration::AddUseInhaler()
{
    SEUseInhaler* myUse = new SEUseInhaler(GetLogger());
    m_Commands.push_back(myUse);
    return *myUse;
}

void SEConsciousRespiration::ToString(std::ostream &str) const
{
  str << "Patient Action : Conscious Respiration";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  for (SEConsciousRespirationCommand* c : m_Commands)
  {
    str << "\n\t";
    c->ToString(str);    
  }
  str << std::flush;
}