/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/substances/SESubstanceQuantity.h"
#include "cdm/substance/SESubstanceTransport.h"
class SEGasCompartment;

class CDM_DECL SEGasSubstanceQuantity : public SESubstanceQuantity, public SEGasTransportSubstance
{
  friend class PBSubstanceQuantity;//friend the serialization class
  friend class SEGasCompartment;
protected:
  SEGasSubstanceQuantity(SESubstance& sub, SEGasCompartment& compartment);
public:
  virtual ~SEGasSubstanceQuantity();

  void Clear() override;

  virtual std::string GetCompartmentName() override;

  void SetToZero() override;
  const SEScalar* GetScalar(const std::string& name) override;

  virtual bool HasPartialPressure() const;
  virtual SEScalarPressure& GetPartialPressure();
  virtual double GetPartialPressure(const PressureUnit& unit) const;
  
  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual bool HasVolumeFraction() const;
  virtual SEScalar0To1& GetVolumeFraction();
  virtual double GetVolumeFraction() const;

protected:
  virtual void AddChild(SEGasSubstanceQuantity& subQ);

  bool HasExtensive() const override { return HasVolume(); }
  SEScalarVolume& GetExtensive() override { return GetVolume(); }

  bool HasIntensive() const override { return HasVolumeFraction(); }
  SEScalar0To1& GetIntensive() override { return GetVolumeFraction(); }

  SEScalarPressure* m_PartialPressure;
  SEScalarVolume*   m_Volume;
  SEScalar0To1* m_VolumeFraction;

  SEGasCompartment&        m_Compartment;
  std::vector<SEGasSubstanceQuantity*> m_Children;
};