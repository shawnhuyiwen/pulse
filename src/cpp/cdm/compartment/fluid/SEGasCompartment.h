/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/fluid/SEFluidCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/substances/SEGasSubstanceQuantity.h"
#include "substance/SESubstanceTransport.h"
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

  virtual void StateChange();

  virtual void Balance(BalanceGasBy e);

  virtual void AddChild(SEGasCompartment& child);
  virtual const std::vector<SEGasCompartment*>& GetChildren() { return m_Children; }
  virtual const std::vector<SEGasCompartment*>& GetLeaves() { return m_Leaves; }

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual SEGasBlackBox* GetBlackBox() { return m_BlackBox; }
  virtual const SEGasBlackBox* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(SEGasBlackBox* bb) { m_BlackBox = bb; }

protected:
  virtual SEGasSubstanceQuantity& CreateSubstanceQuantity(SESubstance& substance, bool zeroOut=true);
 
  std::vector<SEGasCompartment*> m_Children;
  std::vector<SEGasCompartment*> m_Leaves;

  SEGasBlackBox* m_BlackBox = nullptr;
};
