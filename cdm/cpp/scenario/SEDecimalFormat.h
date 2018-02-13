/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(DecimalFormatData)
PROTO_PUSH
#include "bind/cdm/ScenarioEnums.pb.h"
PROTO_PUSH

class CDM_DECL SEDecimalFormat
{
public:
  SEDecimalFormat(const SEDecimalFormat* dfault = nullptr);
  virtual ~SEDecimalFormat();

  virtual void Clear();
  virtual void Set(const SEDecimalFormat& f);

  static void Load(const cdm::DecimalFormatData& src, SEDecimalFormat& dst);
  static cdm::DecimalFormatData* Unload(const SEDecimalFormat& src);
protected:
  static void Serialize(const cdm::DecimalFormatData& src, SEDecimalFormat& dst);
  static void Serialize(const SEDecimalFormat& src, cdm::DecimalFormatData& dst);
public:

  void SetPrecision(std::streamsize p);
  std::streamsize GetPrecision();

  void SetNotation(cdm::eDecimalFormat_Type n);
  cdm::eDecimalFormat_Type GetNotation();

  void SetStream(std::ofstream& s);

protected:
  std::streamsize               m_Precision;
  cdm::eDecimalFormat_Type m_Notation;

};