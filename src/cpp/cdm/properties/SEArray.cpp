/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SEArray.h"

SEArray::SEArray() : SEProperty()
{
  Clear();
}

SEArray::~SEArray()
{
}

void SEArray::Clear()
{
  m_Data.clear();
}

void SEArray::Copy(const SEArray& src)
{
  Clear();

  std::copy(
    src.m_Data.begin(),
    src.m_Data.end(),
    std::back_inserter(m_Data)
  );
}

bool SEArray::IsValid() const
{
  if (m_Data.size() == 0)
    return false;
  return true;
}

void SEArray::Invalidate()
{
  Clear();
}

std::vector<double>& SEArray::GetData()
{
  return m_Data;
}
