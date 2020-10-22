/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/SESystem.h"

class CDM_DECL SEPupillaryResponse : public Loggable
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEPupillaryResponse(Logger* logger);
  virtual ~SEPupillaryResponse();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasReactivityModifier() const;
  virtual SEScalarNegative1To1& GetReactivityModifier();
  virtual double GetReactivityModifier() const;

  virtual bool HasShapeModifier() const;
  virtual SEScalarNegative1To1& GetShapeModifier();
  virtual double GetShapeModifier() const;

  virtual bool HasSizeModifier() const;
  virtual SEScalarNegative1To1& GetSizeModifier();
  virtual double GetSizeModifier() const;

protected:

  SEScalarNegative1To1* m_ReactivityModifier;
  SEScalarNegative1To1* m_ShapeModifier;
  SEScalarNegative1To1* m_SizeModifier;

};
