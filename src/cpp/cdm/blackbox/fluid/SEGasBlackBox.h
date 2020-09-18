/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "blackbox/fluid/SEFluidBlackBox.h"
//#include "blackbox/substances/SEGasBlackBoxSubstanceQuantity.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"

class CDM_DECL SEGasBlackBox : public SEFluidBlackBox
{
  friend class SEBlackBoxManager;
protected:
  SEGasBlackBox(const std::string& name, Logger* logger);
public:
  virtual ~SEGasBlackBox();

  virtual void Clear(); //clear memory

  // Graph Elements
  virtual bool HasCompartment() const { return m_Compartment != nullptr; }
  virtual SEGasCompartment& GetCompartment() { return *m_Compartment; }
  virtual const SEGasCompartment& GetCompartment() const { return *m_Compartment; }

  virtual bool HasSourceCompartment() const { return m_srcCompartment != nullptr; }
  virtual SEGasCompartment& GetSourceCompartment() { return *m_srcCompartment; }
  virtual const SEGasCompartment& GetSourceCompartment() const { return *m_srcCompartment; }

  virtual bool HasTargetCompartment() const { return m_tgtCompartment != nullptr; }
  virtual SEGasCompartment& GetTargetCompartment() { return *m_tgtCompartment; }
  virtual const SEGasCompartment& GetTargetCompartment() const { return *m_tgtCompartment; }

  virtual bool HasSourceLink() const { return m_srcLink != nullptr; }
  virtual SEGasCompartmentLink& GetSourceLink() { return *m_srcLink; }
  virtual const SEGasCompartmentLink& GetSourceLink() const { return *m_srcLink; }

  virtual bool HasTargetLink() const { return m_tgtLink != nullptr; }
  virtual SEGasCompartmentLink& GetTargetLink() { return *m_tgtLink; }
  virtual const SEGasCompartmentLink& GetTargetLink() const { return *m_tgtLink; }

protected:
  virtual void SetCompartment(SEGasCompartment* c) { m_Compartment = c; }
  virtual void SetSourceCompartment(SEGasCompartment* c) { m_srcCompartment = c; }
  virtual void SetTargetCompartment(SEGasCompartment* c) { m_tgtCompartment = c; }
  virtual void SetSourceLink(SEGasCompartmentLink* l) { m_srcLink = l; }
  virtual void SetTargetLink(SEGasCompartmentLink* l) { m_tgtLink = l; }

  // Graph Elements
  SEGasCompartment*     m_Compartment = nullptr;
  SEGasCompartment*     m_srcCompartment = nullptr;
  SEGasCompartment*     m_tgtCompartment = nullptr;
  SEGasCompartmentLink* m_srcLink = nullptr;
  SEGasCompartmentLink* m_tgtLink = nullptr;
};
