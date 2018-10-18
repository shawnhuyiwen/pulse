/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/cdm/PBPhysiology.h"
#include "system/physiology/SEPupillaryResponse.h"
#include "properties/SEScalarNegative1To1.h"
#include "bind/cdm/Physiology.pb.h"

void PBPhysiology::Load(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst)
{
  PBPhysiology::Serialize(src, dst);
}
void PBPhysiology::Serialize(const cdm::PupillaryResponseData& src, SEPupillaryResponse& dst)
{
  dst.Clear();
  if (src.has_reactivitymodifier())
    SEScalarNegative1To1::Load(src.reactivitymodifier(), dst.GetReactivityModifier());
  if (src.has_shapemodifier())
    SEScalarNegative1To1::Load(src.shapemodifier(), dst.GetShapeModifier());
  if (src.has_sizemodifier())
    SEScalarNegative1To1::Load(src.sizemodifier(), dst.GetSizeModifier());
}

cdm::PupillaryResponseData* PBPhysiology::Unload(const SEPupillaryResponse& src)
{
  cdm::PupillaryResponseData* dst = new cdm::PupillaryResponseData();
  PBPhysiology::Serialize(src, *dst);
  return dst;
}
void PBPhysiology::Serialize(const SEPupillaryResponse& src, cdm::PupillaryResponseData& dst)
{
  if (src.HasReactivityModifier())
    dst.set_allocated_reactivitymodifier(SEScalarNegative1To1::Unload(*src.m_ReactivityModifier));
  if (src.HasShapeModifier())
    dst.set_allocated_shapemodifier(SEScalarNegative1To1::Unload(*src.m_ShapeModifier));
  if (src.HasSizeModifier())
    dst.set_allocated_sizemodifier(SEScalarNegative1To1::Unload(*src.m_SizeModifier));
}