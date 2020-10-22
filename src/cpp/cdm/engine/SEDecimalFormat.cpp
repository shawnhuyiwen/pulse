/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include <iomanip>
#include <iostream>
#include "engine/SEDecimalFormat.h"

SEDecimalFormat::SEDecimalFormat(const SEDecimalFormat* dfault)
{
  Clear();
  if (dfault != nullptr) 
    Set(*dfault); 
}

SEDecimalFormat::~SEDecimalFormat()
{

}

void SEDecimalFormat::Clear()
{
  m_Precision = 6;
  m_Notation = eDecimalFormat_Type::SystemFormatting;
}

void SEDecimalFormat::Set(const SEDecimalFormat& f)
{
  m_Precision = f.m_Precision;
  m_Notation = f.m_Notation;
}

void SEDecimalFormat::SetPrecision(std::streamsize p)
{
  m_Precision = p;
}
std::streamsize SEDecimalFormat::GetPrecision()
{
  return m_Precision;
}

void SEDecimalFormat::SetNotation(eDecimalFormat_Type n)
{
  m_Notation = n;
}
eDecimalFormat_Type SEDecimalFormat::GetNotation()
{
  return m_Notation;
}

void SEDecimalFormat::SetStream(std::ofstream& s)
{
  switch (m_Notation)
  {
  case eDecimalFormat_Type::SystemFormatting:
    s << std::fixed << std::setprecision(m_Precision);
    break;
  case eDecimalFormat_Type::DefaultFloat:
    s << std::defaultfloat << std::setprecision(m_Precision);
    break;
  case eDecimalFormat_Type::FixedMantissa:
    s << std::fixed << std::setprecision(m_Precision);
    break;
  case eDecimalFormat_Type::SignificantDigits:
    s << std::scientific << std::setprecision(m_Precision);
  }
}
