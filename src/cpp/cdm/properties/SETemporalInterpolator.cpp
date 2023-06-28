/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SETemporalInterpolator.h"

void SETemporalInterpolator::Invalidate()
{
  m_Complete = false;
  m_PositiveTransition = true;
  m_Period_s = 0;
  m_Current = 1;
  m_Target = 1;
  m_Initial = 1;
}

bool SETemporalInterpolator::IsValid() const
{
  return m_Period_s > 0;
}

void SETemporalInterpolator::Set(double d)
{
  m_Current = d;
  m_Target = d;
  m_Initial = d;
}

void SETemporalInterpolator::SetTarget(double target, double period_s)
{
  SetTransition(m_Current, target, period_s);
}

void SETemporalInterpolator::SetTransition(double initial, double target, double period_s)
{
  m_Target = target;
  m_Initial = initial;
  m_Period_s = period_s;
  if (period_s == 0)
  {
    m_Complete = true;
    m_Current = m_Target;
  }
  else if (target == initial)
  {
    m_Complete = true;
  }
  else
  {
    m_Complete = false;
    m_PositiveTransition = (m_Target > m_Initial);
  }
}

bool SETemporalInterpolator::Transition(double dt_s)
{
  if (m_Complete)
    return m_Complete;

  if (!IsValid())
  {
    m_Current = m_Target;
    m_Complete = true;
    return m_Complete;
  }

  // Do a linear increment by timestep to eventually reach the target at the end of the period
  if (m_PositiveTransition)
  {
    if (m_Current < m_Target)
    {
      m_Current += (dt_s / m_Period_s) * (m_Target - m_Initial);
      if (m_Current > m_Target)
      {
        m_Current = m_Target;
        m_Complete = true;
      }
    }
  }
  else
  {
    if (m_Target < m_Current)
    {
      m_Current -= (dt_s / m_Period_s) * (m_Initial - m_Target);
      if (m_Current < m_Target)
      {
        m_Current = m_Target;
        m_Complete = true;
      }
    }
  }
  return m_Complete;
}
