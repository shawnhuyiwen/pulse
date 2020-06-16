/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEChestCompression.h"

class CDM_DECL SEChestCompressionForce : public SEChestCompression
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEChestCompressionForce(Logger* logger=nullptr);
  virtual ~SEChestCompressionForce();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEChestCompressionForce& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();
  virtual double GetForce(const ForceUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalarForce*           m_Force;
};