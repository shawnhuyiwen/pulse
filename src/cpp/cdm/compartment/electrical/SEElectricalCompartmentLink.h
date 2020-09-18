/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/SECompartmentLink.h"
class SEElectricalBlackBox;
class SEElectricalCompartment;
class SEElectricalCircuitPath;

class CDM_DECL SEElectricalCompartmentLink : public SECompartmentLink
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SEElectricalCompartmentLink(SEElectricalCompartment& src, SEElectricalCompartment & tgt, const std::string& name);
public:
  virtual ~SEElectricalCompartmentLink();

  virtual void Clear();
  
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasCurrent() const;
  virtual SEScalarElectricCurrent& GetCurrent();
  virtual double GetCurrent(const ElectricCurrentUnit& unit) const;

  virtual SEElectricalCompartment& GetSourceCompartment() { return m_SourceCmpt; }
  virtual SEElectricalCompartment& GetTargetCompartment() { return m_TargetCmpt; }

  virtual bool HasPath() { return m_Path != nullptr; }
  virtual SEElectricalCircuitPath* GetPath() { return m_Path; }
  virtual void RemovePath() { m_Path = nullptr; }
  virtual void MapPath(SEElectricalCircuitPath& path) { Clear();  m_Path = &path; }

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual SEElectricalBlackBox* GetBlackBox() { return m_BlackBox; }
  virtual const SEElectricalBlackBox* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(SEElectricalBlackBox* bb) { m_BlackBox = bb; }

protected:
  SEScalarElectricCurrent*  m_Current;
  SEElectricalCompartment&  m_SourceCmpt;
  SEElectricalCompartment&  m_TargetCmpt;
  SEElectricalCircuitPath*  m_Path;

  SEElectricalBlackBox* m_BlackBox = nullptr;
};

#include "compartment/SECompartmentGraph.h"
class SEElectricalCompartmentGraph : public SECompartmentGraph<SEElectricalCompartment, SEElectricalCompartmentLink>
{
public:
  SEElectricalCompartmentGraph(const std::string& name, Logger* logger) : SECompartmentGraph(name, logger) {};
  virtual ~SEElectricalCompartmentGraph() {}
};
