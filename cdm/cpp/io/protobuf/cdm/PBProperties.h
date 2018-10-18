/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
   // Forward Declaire SE and Protobuf Physiology Types
CDM_DECL_BIND(PupillaryResponse)


class CDM_DECL PBPhysiology
{
  // Pupillary Response
public:
  static void Load(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst);
  static cdm::PupillaryResponseData* Unload(const SEPupillaryResponse& src);
protected:
  static void Serialize(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst);
  static void Serialize(const SEPupillaryResponse& src, cdm::PupillaryResponseData& dst);

};