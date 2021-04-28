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

  virtual bool HasElectricalBlackBox(const std::string& name) const;
  virtual bool HasElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  virtual bool HasGasBlackBox(const std::string& name) const;
  virtual bool HasGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  virtual bool HasLiquidBlackBox(const std::string& name) const;
  virtual bool HasLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  virtual bool HasThermalBlackBox(const std::string& name) const;
  virtual bool HasThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) const;

  virtual SEElectricalBlackBox* GetElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, std::string name="");
  virtual SEGasBlackBox* GetGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, std::string name = "");
  virtual SELiquidBlackBox* GetLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, std::string name = "");
  virtual SEThermalBlackBox* GetThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, std::string name = "");

protected:
  virtual SEElectricalBlackBox* CreateElectricalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name);
  virtual SEGasBlackBox* CreateGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name);
  virtual SELiquidBlackBox* CreateLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name);
  virtual SEThermalBlackBox* CreateThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName, const std::string& name);

  virtual std::string GetBlackBoxName(const std::string& srcCmptName, const std::string& tgtCmptName) const;
  template<typename tBlackBox> tBlackBox* CreateBlackBox(const std::string& name, std::map<std::string, tBlackBox*>& ledger);
  
  // Map circuit compartments in a separate function, to support unit tests
  template<typename tBlackBox, typename tCompartment, typename tLink, typename tNode, typename tPath>
  bool MapBlackBox(tBlackBox& bb, tLink& src2bbLink, tLink& bb2tgtLink)
  {
    tCompartment& midCmpt = src2bbLink.GetTargetCompartment();
    tCompartment& srcCmpt = src2bbLink.GetSourceCompartment();
    tCompartment& tgtCmpt = bb2tgtLink.GetTargetCompartment();
    bb.SetCompartment(&midCmpt);
    bb.SetSourceCompartment(&srcCmpt);
    bb.SetSourceLink(&src2bbLink);
    bb.SetTargetCompartment(&tgtCmpt);
    bb.SetTargetLink(&bb2tgtLink);

    tPath& srcPath = *src2bbLink.GetPath();
    tPath& tgtPath = *bb2tgtLink.GetPath();

    return MapBlackBox<tBlackBox, tNode, tPath>(bb, srcPath, tgtPath);
  }

  // Map circuit components in a separate function, to support unit tests
  template<typename tBlackBox, typename tNode, typename tPath>
  bool MapBlackBox(tBlackBox& bb, tPath& srcPath, tPath& tgtPath)
  {
    tNode& midNode = srcPath.GetTargetNode();
    tNode& srcNode = srcPath.GetSourceNode();
    tNode& tgtNode = tgtPath.GetTargetNode();

    // Check if things are setup really werid
    if (&srcPath.GetSourceNode() == &tgtPath.GetSourceNode() ||
      &srcPath.GetSourceNode() == &tgtPath.GetTargetNode())
    {
      midNode = srcPath.GetSourceNode();
      srcNode = srcPath.GetTargetNode();
    }
    if (&tgtPath.GetTargetNode() == &srcPath.GetSourceNode() ||
      &tgtPath.GetTargetNode() == &srcPath.GetTargetNode())
    {
      midNode = tgtPath.GetTargetNode();
      tgtNode = tgtPath.GetSourceNode();
    }

    if (srcNode.GetBlackBoxType() != eBlackBox_Node_Type::None &&
      srcNode.GetBlackBoxType() != eBlackBox_Node_Type::Source)
    {
      Warning("Changing a preexisting black box node type to Source");
    }
    if (tgtNode.GetBlackBoxType() != eBlackBox_Node_Type::None &&
      tgtNode.GetBlackBoxType() != eBlackBox_Node_Type::Target)
    {
      Warning("Changing a preexisting black box node type to Target");
    }
    midNode.SetBlackBoxType(eBlackBox_Node_Type::Middle);
    midNode.SetBlackBoxSourceNode(&srcNode);
    midNode.SetBlackBoxTargetNode(&tgtNode);
    srcNode.SetBlackBoxType(eBlackBox_Node_Type::Source);
    tgtNode.SetBlackBoxType(eBlackBox_Node_Type::Target);
    srcPath.SetBlackBoxType(eBlackBox_Path_Type::Source);
    tgtPath.SetBlackBoxType(eBlackBox_Path_Type::Target);


    bb.SetMiddleNode(&midNode);
    bb.SetSourceNode(&srcNode);
    bb.SetSourcePath(&srcPath);
    bb.SetTargetNode(&tgtNode);
    bb.SetTargetPath(&tgtPath);
  }
  std::map<std::string, SEElectricalBlackBox*> m_ElectricalBoxes;
  std::map<std::string, SEGasBlackBox*> m_GasBoxes;
  std::map<std::string, SELiquidBlackBox*> m_LiquidBoxes;
  std::map<std::string, SEThermalBlackBox*> m_ThermalBoxes;
};
