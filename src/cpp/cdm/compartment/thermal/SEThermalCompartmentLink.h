/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "compartment/SECompartmentLink.h"
class SEThermalBlackBox;
class SEThermalCompartment;
class SEThermalCircuitPath;

class CDM_DECL SEThermalCompartmentLink : public SECompartmentLink
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SEThermalCompartmentLink(SEThermalCompartment& src, SEThermalCompartment & tgt, const std::string& name);
public:
  virtual ~SEThermalCompartmentLink();

  virtual void Clear();
  
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasHeatTransferRate() const;
  virtual SEScalarPower& GetHeatTransferRate();
  virtual double GetHeatTransferRate(const PowerUnit& unit) const;

  virtual SEThermalCompartment& GetSourceCompartment() { return m_SourceCmpt; }
  virtual SEThermalCompartment& GetTargetCompartment() { return m_TargetCmpt; }

  virtual bool HasPath() { return m_Path != nullptr; }
  virtual SEThermalCircuitPath* GetPath() { return m_Path; }
  virtual void RemovePath() { m_Path = nullptr; }
  virtual void MapPath(SEThermalCircuitPath& path) { Clear();  m_Path = &path; }

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual SEThermalBlackBox* GetBlackBox() { return m_BlackBox; }
  virtual const SEThermalBlackBox* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(SEThermalBlackBox* bb) { m_BlackBox = bb; }

protected:
  SEScalarPower*         m_HeatTransferRate;
  SEThermalCompartment&  m_SourceCmpt;
  SEThermalCompartment&  m_TargetCmpt;
  SEThermalCircuitPath*  m_Path;

  SEThermalBlackBox* m_BlackBox = nullptr;
};

#include "compartment/SECompartmentGraph.h"
class SEThermalCompartmentGraph : public SECompartmentGraph<SEThermalCompartment, SEThermalCompartmentLink>
{
public:
  SEThermalCompartmentGraph(const std::string& name, Logger* logger) : SECompartmentGraph(name, logger) {};
  virtual ~SEThermalCompartmentGraph() {}
};
