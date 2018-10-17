/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include <iomanip> 
#include "scenario/SEDecimalFormat.h"
#include "bind/cdm/Scenario.pb.h"
#include "bind/cdm/ScenarioEnums.pb.h"

const std::string& eDecimalFormat_Type_Name(eDecimalFormat_Type m)
{
  return cdm::eDecimalFormat_Type_Name((cdm::eDecimalFormat_Type)m);
}

SEDecimalFormat::SEDecimalFormat(const SEDecimalFormat* dfault)
{
  Clear();
  if (dfault != nullptr) 
    Set(*dfault); 
}

SEDecimalFormat::~SEDecimalFormat()
{
  Clear();
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

void SEDecimalFormat::Load(const cdm::DecimalFormatData& src, SEDecimalFormat& dst)
{
  SEDecimalFormat::Serialize(src, dst);
}
void SEDecimalFormat::Serialize(const cdm::DecimalFormatData& src, SEDecimalFormat& dst)
{
  dst.Clear();
  dst.SetNotation((eDecimalFormat_Type)src.type());
  dst.SetPrecision(src.precision());
}

cdm::DecimalFormatData* SEDecimalFormat::Unload(const SEDecimalFormat& src)
{
  cdm::DecimalFormatData* dst = new cdm::DecimalFormatData();
  Serialize(src, *dst);
  return dst;
}
void SEDecimalFormat::Serialize(const SEDecimalFormat& src, cdm::DecimalFormatData& dst)
{
  dst.set_type((cdm::eDecimalFormat_Type)src.m_Notation);
  dst.set_precision((google::protobuf::uint32)src.m_Precision);
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
