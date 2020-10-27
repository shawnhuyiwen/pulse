/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/fluid/SEFluidCompartmentLink.h"
#include "compartment/fluid/SEGasCompartment.h"
class SEGasBlackBox;
class SEGasCompartment;

class CDM_DECL SEGasCompartmentLink : public SEFluidCompartmentLink<SEGasTransportEdge, SEGasTransportVertex, SEGasCompartment>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SEGasCompartmentLink(SEGasCompartment& src, SEGasCompartment & tgt, const std::string& name) : SEFluidCompartmentLink(src,tgt,name) {}
public:
  ~SEGasCompartmentLink() {}

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual SEGasBlackBox* GetBlackBox() { return m_BlackBox; }
  virtual const SEGasBlackBox* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(SEGasBlackBox* bb) { m_BlackBox = bb; }

protected:
  SEGasBlackBox* m_BlackBox = nullptr;
};
