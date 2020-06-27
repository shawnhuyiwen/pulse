/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/physiology/SEPupillaryResponse.h"
#include "properties/SEScalarNegative1To1.h"

SEPupillaryResponse::SEPupillaryResponse(Logger* logger)
{
  m_ReactivityModifier = nullptr;
  m_ShapeModifier = nullptr;
  m_SizeModifier = nullptr;
}

SEPupillaryResponse::~SEPupillaryResponse()
{
  SAFE_DELETE(m_ReactivityModifier);
  SAFE_DELETE(m_ShapeModifier);
  SAFE_DELETE(m_SizeModifier);
}

void SEPupillaryResponse::Clear()
{
  INVALIDATE_PROPERTY(m_ReactivityModifier);
  INVALIDATE_PROPERTY(m_ShapeModifier);
  INVALIDATE_PROPERTY(m_SizeModifier);
}

const SEScalar* SEPupillaryResponse::GetScalar(const std::string& name)
{
  if (name.compare("ReactivityModifier") == 0)
    return &GetReactivityModifier();
  if (name.compare("ShapeModifier") == 0)
    return &GetShapeModifier();
  if (name.compare("SizeModifier") == 0)
    return &GetSizeModifier();
  return nullptr;
}

bool SEPupillaryResponse::HasReactivityModifier() const
{
  return m_ReactivityModifier == nullptr ? false : m_ReactivityModifier->IsValid();
}
SEScalarNegative1To1& SEPupillaryResponse::GetReactivityModifier()
{
  if (m_ReactivityModifier == nullptr)
    m_ReactivityModifier = new SEScalarNegative1To1();
  return *m_ReactivityModifier;
}
double SEPupillaryResponse::GetReactivityModifier() const
{
  if (m_ReactivityModifier == nullptr)
    return SEScalar::dNaN();
  return m_ReactivityModifier->GetValue();
}

bool SEPupillaryResponse::HasShapeModifier() const
{
  return m_ShapeModifier == nullptr ? false : m_ShapeModifier->IsValid();
}
SEScalarNegative1To1& SEPupillaryResponse::GetShapeModifier()
{
  if (m_ShapeModifier == nullptr)
    m_ShapeModifier = new SEScalarNegative1To1();
  return *m_ShapeModifier;
}
double SEPupillaryResponse::GetShapeModifier() const
{
  if (m_ShapeModifier == nullptr)
    return SEScalar::dNaN();
  return m_ShapeModifier->GetValue();
}

bool SEPupillaryResponse::HasSizeModifier() const
{
  return m_SizeModifier == nullptr ? false : m_SizeModifier->IsValid();
}
SEScalarNegative1To1& SEPupillaryResponse::GetSizeModifier()
{
  if (m_SizeModifier == nullptr)
    m_SizeModifier = new SEScalarNegative1To1();
  return *m_SizeModifier;
}
double SEPupillaryResponse::GetSizeModifier() const
{
  if (m_SizeModifier == nullptr)
    return SEScalar::dNaN();
  return m_SizeModifier->GetValue();
}


