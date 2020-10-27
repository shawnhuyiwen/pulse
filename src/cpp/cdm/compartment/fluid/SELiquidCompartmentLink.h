/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/fluid/SEFluidCompartmentLink.h"
#include "compartment/fluid/SELiquidCompartment.h"
class SELiquidBlackBox;
class SELiquidCompartment;

class CDM_DECL SELiquidCompartmentLink : public SEFluidCompartmentLink<SELiquidTransportEdge, SELiquidTransportVertex, SELiquidCompartment>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SELiquidCompartmentLink(SELiquidCompartment& src, SELiquidCompartment & tgt, const std::string& name) : SEFluidCompartmentLink(src, tgt, name) {}
public:
  ~SELiquidCompartmentLink() {}

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual SELiquidBlackBox* GetBlackBox() { return m_BlackBox; }
  virtual const SELiquidBlackBox* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(SELiquidBlackBox* bb) { m_BlackBox = bb; }

protected:
  SELiquidBlackBox* m_BlackBox = nullptr;
};
