/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(SubstanceQuantityData)
class SESubstance;

class CDM_DECL SESubstanceQuantity : public Loggable
{
public:
  SESubstanceQuantity(SESubstance& sub);
  virtual ~SESubstanceQuantity();

  virtual void Clear() = 0; //clear memory
  virtual void Invalidate() = 0;

protected:
  static void Serialize(const cdm::SubstanceQuantityData& src, SESubstanceQuantity& dst);
  static void Serialize(const SESubstanceQuantity& src, cdm::SubstanceQuantityData& dst);

public:
  
  virtual SESubstance& GetSubstance() const { return m_Substance; }

  virtual void SetToZero() = 0;
  virtual const SEScalar* GetScalar(const std::string& name) = 0;

protected:
  SESubstance& m_Substance;
};
