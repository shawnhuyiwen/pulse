/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "blackbox/electrical/SEElectricalBlackBox.h"
#include "blackbox/fluid/SEGasBlackBox.h"
#include "blackbox/fluid/SELiquidBlackBox.h"
#include "blackbox/thermal/SEThermalBlackBox.h"

class CDM_DECL SEBlackBoxManager : public Loggable
{
public:
  SEBlackBoxManager(Logger* logger);
  virtual ~SEBlackBoxManager();

  virtual bool HasElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  virtual bool HasGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  virtual bool HasLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  virtual bool HasThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;

  virtual SEElectricalBlackBox* GetElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);
  virtual SEGasBlackBox* GetGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);
  virtual SELiquidBlackBox* GetLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);
  virtual SEThermalBlackBox* GetThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);

protected:
  virtual SEElectricalBlackBox* CreateElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);
  virtual SEGasBlackBox* CreateGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);
  virtual SELiquidBlackBox* CreateLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);
  virtual SEThermalBlackBox* CreateThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName);

  virtual std::string GetBlackBoxName(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  template<typename BlackBoxType> BlackBoxType* CreateBlackBox(const std::string& name, std::map<std::string, BlackBoxType*>& ledger);
  
  template<typename BlackBoxType, typename CompartmentType, typename LinkType, typename NodeType, typename PathType>
  bool MapBlackBox(BlackBoxType& bb, LinkType& src2bbLink, LinkType& bb2tgtLink)
  {
    CompartmentType& bbCmpt  = src2bbLink.GetTargetCompartment();
    CompartmentType& srcCmpt = src2bbLink.GetSourceCompartment();
    CompartmentType& tgtCmpt = bb2tgtLink.GetTargetCompartment();
    bb.SetCompartment(&bbCmpt);
    bb.SetSourceCompartment(&srcCmpt);
    bb.SetSourceLink(&src2bbLink);
    bb.SetTargetCompartment(&tgtCmpt);
    bb.SetTargetLink(&bb2tgtLink);

    bbCmpt.SetBlackBox(&bb);
    src2bbLink.SetBlackBox(&bb);
    bb2tgtLink.SetBlackBox(&bb);

    return bb.MapBlackBox<BlackBoxType, NodeType,PathType>(*src2bbLink.GetPath(), *bb2tgtLink.GetPath());
  }

  std::map<std::string, SEElectricalBlackBox*> m_ElectricalBoxes;
  std::map<std::string, SEGasBlackBox*> m_GasBoxes;
  std::map<std::string, SELiquidBlackBox*> m_LiquidBoxes;
  std::map<std::string, SEThermalBlackBox*> m_ThermalBoxes;
};
