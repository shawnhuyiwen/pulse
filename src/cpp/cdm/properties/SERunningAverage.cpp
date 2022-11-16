/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SERunningAverage.h"

SERunningAverage::SERunningAverage()
{
  Invalidate();
}

SERunningAverage::~SERunningAverage()
{

}

void SERunningAverage::Invalidate()
{
  m_Sum = 0.0;
  m_NumSamples = 0;
}

bool SERunningAverage::IsValid() const
{
  return m_NumSamples != 0;
}

double SERunningAverage::Sample(double d)
{
  m_Sum += d;
  m_NumSamples++;
  return Value();
}

double SERunningAverage::Value()
{
  if (m_Sum==0)
    return 0;
  if(!IsValid())
    throw CommonDataModelException("Running Average is Empty");
  return m_Sum / double(m_NumSamples);
}
