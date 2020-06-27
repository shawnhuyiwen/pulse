/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;

class CDM_DECL SESubstanceQuantity : public Loggable
{
  friend class PBSubstanceQuantity;//friend the serialization class
public:
  SESubstanceQuantity(SESubstance& sub);
  virtual ~SESubstanceQuantity();

  virtual void Clear() = 0; //clear memory
  virtual void Invalidate() = 0;

  virtual std::string GetCompartmentName() = 0;
  virtual SESubstance& GetSubstance() { return m_Substance; }
  virtual const SESubstance& GetSubstance() const { return m_Substance; }

  virtual void SetToZero() = 0;
  virtual const SEScalar* GetScalar(const std::string& name) = 0;

protected:
  SESubstance& m_Substance;
};
