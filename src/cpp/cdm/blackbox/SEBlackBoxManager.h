/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "blackbox/electrical/SEElectricalBlackBox.h"
#include "blackbox/fluid/SEGasBlackBox.h"
#include "blackbox/fluid/SELiquidBlackBox.h"
#include "blackbox/thermal/SEThermalBlackBox.h"

class CDM_DECL SEBlackBoxManager : public Loggable
{
  friend class CommonDataModelTest;
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
  template<typename tBlackBox> tBlackBox* CreateBlackBox(const std::string& name, std::map<std::string, tBlackBox*>& ledger);
  
  template<typename tBlackBox, typename tCompartment, typename tLink, typename tNode, typename tPath>
  bool MapBlackBox(tBlackBox& bb, tLink& src2bbLink, tLink& bb2tgtLink)
  {
    tCompartment& bbCmpt = src2bbLink.GetTargetCompartment();
    tCompartment& srcCmpt = src2bbLink.GetSourceCompartment();
    tCompartment& tgtCmpt = bb2tgtLink.GetTargetCompartment();
    bb.SetCompartment(&bbCmpt);
    bb.SetSourceCompartment(&srcCmpt);
    bb.SetSourceLink(&src2bbLink);
    bb.SetTargetCompartment(&tgtCmpt);
    bb.SetTargetLink(&bb2tgtLink);

    bbCmpt.SetBlackBox(&bb);
    src2bbLink.SetBlackBox(&bb);
    bb2tgtLink.SetBlackBox(&bb);

    tPath& srcPath = *src2bbLink.GetPath();
    tPath& tgtPath = *bb2tgtLink.GetPath();

    return MapBlackBox<tBlackBox, tNode, tPath>(bb, srcPath, tgtPath);
  }

  template<typename tBlackBox, typename tNode, typename tPath>
  bool MapBlackBox(tBlackBox& bb, tPath& srcPath, tPath& tgtPath)
  {
    tNode& bbNode = srcPath.GetTargetNode();
    tNode& srcNode = srcPath.GetSourceNode();
    tNode& tgtNode = tgtPath.GetTargetNode();

    //Check our assumptions
    if (&srcPath.GetSourceNode() == &tgtPath.GetSourceNode() ||
      &srcPath.GetSourceNode() == &tgtPath.GetTargetNode())
    {
      bbNode = srcPath.GetSourceNode();
      srcNode = srcPath.GetTargetNode();
    }

    if (&tgtPath.GetTargetNode() == &srcPath.GetSourceNode() ||
      &tgtPath.GetTargetNode() == &srcPath.GetTargetNode())
    {
      bbNode = tgtPath.GetTargetNode();
      tgtNode = tgtPath.GetSourceNode();
    }

    bb.SetNode(&bbNode);
    bb.SetSourceNode(&srcNode);
    bb.SetSourcePath(&srcPath);
    bb.SetTargetNode(&tgtNode);
    bb.SetTargetPath(&tgtPath);

    bbNode.SetBlackBox(&bb);
    srcNode.SetBlackBox(&bb);
    tgtNode.SetBlackBox(&bb);
    srcPath.SetBlackBox(&bb);
    tgtPath.SetBlackBox(&bb);

    return true;
  }

  std::map<std::string, SEElectricalBlackBox*> m_ElectricalBoxes;
  std::map<std::string, SEGasBlackBox*> m_GasBoxes;
  std::map<std::string, SELiquidBlackBox*> m_LiquidBoxes;
  std::map<std::string, SEThermalBlackBox*> m_ThermalBoxes;
};
