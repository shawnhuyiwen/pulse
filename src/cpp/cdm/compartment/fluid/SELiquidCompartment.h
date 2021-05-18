/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/fluid/SEFluidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"
#include "compartment/substances/SELiquidSubstanceQuantity.h"
#include "substance/SESubstanceTransport.h"
class SELiquidBlackBox;

class CDM_DECL SELiquidCompartment : public SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity>
{
  friend class PBCompartment;//friend the serialization class
  friend class SETissueCompartment;
  friend class SECompartmentManager;
  friend class SELiquidSubstanceQuantity;
protected:
  SELiquidCompartment(const std::string& name, Logger* logger);
public:
  virtual ~SELiquidCompartment();

  virtual void Clear();

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual void StateChange();

  virtual void Balance(BalanceLiquidBy e);// Balance all substances based on a specific property

  virtual bool HasPH() const;
  virtual SEScalar& GetPH();
  virtual double GetPH() const;

  virtual bool HasWaterVolumeFraction() const;
  virtual SEScalar0To1& GetWaterVolumeFraction();
  virtual double GetWaterVolumeFraction() const;

  virtual void AddChild(SELiquidCompartment& child);
  virtual const std::vector<SELiquidCompartment*>& GetChildren() { return m_Children; }
  virtual const std::vector<SELiquidCompartment*>& GetLeaves() { return m_Leaves; }

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual SELiquidBlackBox* GetBlackBox() { return m_BlackBox; }
  virtual const SELiquidBlackBox* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(SELiquidBlackBox* bb) { m_BlackBox = bb; }

protected:
  virtual SELiquidSubstanceQuantity& CreateSubstanceQuantity(SESubstance& substance, bool zeroOut=true);

  SEScalar* m_pH;
  SEScalar0To1* m_WaterVolumeFraction;

  std::vector<SELiquidCompartment*> m_Children;
  std::vector<SELiquidCompartment*> m_Leaves;

  SELiquidBlackBox* m_BlackBox = nullptr;
};