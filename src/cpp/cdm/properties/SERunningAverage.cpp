/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SERunningAverage.h"

SERunningAverage::SERunningAverage()
{
  Clear();
}

SERunningAverage::~SERunningAverage()
{

}

void SERunningAverage::Clear()
{
  m_Sum = 0.0;
  m_NumSamples = 0;
}

void SERunningAverage::Invalidate()
{
  Clear();
}

bool SERunningAverage::IsValid() const
{
  return m_NumSamples == 0 ? false : true;
}

double SERunningAverage::Sample(double d)
{
  m_Sum += d;
  m_NumSamples++;
  return Value();
}

double SERunningAverage::Value()
{
  return m_Sum / double(m_NumSamples);
}
