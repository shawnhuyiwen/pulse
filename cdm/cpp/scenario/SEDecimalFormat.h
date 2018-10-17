/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(DecimalFormatData)

enum class eDecimalFormat_Type { SystemFormatting = 0, DefaultFloat, FixedMantissa, SignificantDigits };
extern const std::string& eDecimalFormat_Type_Name(eDecimalFormat_Type m);

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

  void SetNotation(eDecimalFormat_Type n);
  eDecimalFormat_Type GetNotation();

  void SetStream(std::ofstream& s);

protected:
  std::streamsize     m_Precision;
  eDecimalFormat_Type m_Notation;

};