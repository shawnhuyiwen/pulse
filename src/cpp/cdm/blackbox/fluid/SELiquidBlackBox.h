/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "blackbox/fluid/SEFluidBlackBox.h"
//#include "blackbox/substances/SELiquidBlackBoxSubstanceQuantity.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"

class CDM_DECL SELiquidBlackBox : public SEFluidBlackBox
{
  friend class SEBlackBoxManager;
protected:
  SELiquidBlackBox(const std::string& name, Logger* logger);
public:
  virtual ~SELiquidBlackBox();

  virtual void Clear(); //clear memory

  // Graph Elements
  virtual bool HasCompartment() const { return m_Compartment != nullptr; }
  virtual SELiquidCompartment& GetCompartment() { return *m_Compartment; }
  virtual const SELiquidCompartment& GetCompartment() const { return *m_Compartment; }

  virtual bool HasSourceCompartment() const { return m_srcCompartment != nullptr; }
  virtual SELiquidCompartment& GetSourceCompartment() { return *m_srcCompartment; }
  virtual const SELiquidCompartment& GetSourceCompartment() const { return *m_srcCompartment; }

  virtual bool HasTargetCompartment() const { return m_tgtCompartment != nullptr; }
  virtual SELiquidCompartment& GetTargetCompartment() { return *m_tgtCompartment; }
  virtual const SELiquidCompartment& GetTargetCompartment() const { return *m_tgtCompartment; }

  virtual bool HasSourceLink() const { return m_srcLink != nullptr; }
  virtual SELiquidCompartmentLink& GetSourceLink() { return *m_srcLink; }
  virtual const SELiquidCompartmentLink& GetSourceLink() const { return *m_srcLink; }

  virtual bool HasTargetLink() const { return m_tgtLink != nullptr; }
  virtual SELiquidCompartmentLink& GetTargetLink() { return *m_tgtLink; }
  virtual const SELiquidCompartmentLink& GetTargetLink() const { return *m_tgtLink; }

protected:
  virtual void SetCompartment(SELiquidCompartment* c) { m_Compartment = c; }
  virtual void SetSourceCompartment(SELiquidCompartment* c) { m_srcCompartment = c; }
  virtual void SetTargetCompartment(SELiquidCompartment* c) { m_tgtCompartment = c; }
  virtual void SetSourceLink(SELiquidCompartmentLink* l) { m_srcLink = l; }
  virtual void SetTargetLink(SELiquidCompartmentLink* l) { m_tgtLink = l; }

  // Graph Elements
  SELiquidCompartment*     m_Compartment = nullptr;
  SELiquidCompartment*     m_srcCompartment = nullptr;
  SELiquidCompartment*     m_tgtCompartment = nullptr;
  SELiquidCompartmentLink* m_srcLink = nullptr;
  SELiquidCompartmentLink* m_tgtLink = nullptr;

};
