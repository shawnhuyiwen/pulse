/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalar.h"

class CDM_DECL OsmolalityUnit : public CCompoundUnit
{
public:
  OsmolalityUnit(const std::string& u) : CCompoundUnit(u) {}
  virtual ~OsmolalityUnit() {}

  static bool IsValidUnit(const std::string& unit);
  static const OsmolalityUnit& GetCompoundUnit(const std::string& unit);

  static const OsmolalityUnit Osm_Per_kg;
  static const OsmolalityUnit mOsm_Per_kg;
};

class CDM_DECL SEScalarOsmolality : public SEScalarQuantity<OsmolalityUnit>
{
public:
  SEScalarOsmolality() {}
  virtual ~SEScalarOsmolality() {}
};
