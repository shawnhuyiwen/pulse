/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/fluid/SEFluidCompartment.h"
#include "cdm/compartment/fluid/SEGasCompartmentLink.h"
#include "cdm/compartment/substances/SEGasSubstanceQuantity.h"
#include "cdm/substance/SESubstanceTransport.h"
class SEGasBlackBox;

enum class BalanceGasBy { Volume, VolumeFraction };

class CDM_DECL SEGasCompartment : public SEFluidCompartment<SEGasCompartmentLink, SEGasTransportVertex, SEGasTransportSubstance, SEGasSubstanceQuantity>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SEGasCompartment(const std::string& name, Logger* logger);
public:
  virtual ~SEGasCompartment();

  virtual void Clear();

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual void StateChange();

  virtual void Balance(BalanceGasBy e);

  virtual bool HasDiffusionSurfaceArea() const;
  virtual SEScalarArea& GetDiffusionSurfaceArea();
  virtual double GetDiffusionSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasVentilation() const;
  virtual SEScalarVolumePerTime& GetVentilation();
  virtual double GetVentilation(const VolumePerTimeUnit& unit) const;

  virtual bool HasVentilationPerfusionRatio() const;
  virtual SEScalar& GetVentilationPerfusionRatio();
  virtual double GetVentilationPerfusionRatio() const;

  virtual void AddChild(SEGasCompartment& child);
  virtual const std::vector<SEGasCompartment*>& GetChildren() { return m_Children; }
  virtual const std::vector<SEGasCompartment*>& GetLeaves() { return m_Leaves; }

protected:
  virtual SEGasSubstanceQuantity& CreateSubstanceQuantity(SESubstance& substance, bool zeroOut=true);

  SEScalarArea*          m_DiffusionSurfaceArea;
  SEScalarVolumePerTime* m_Ventilation;
  SEScalar*              m_VentilationPerfusionRatio;

  std::vector<SEGasCompartment*> m_Children;
  std::vector<SEGasCompartment*> m_Leaves;
};
