/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "blackbox/SEBlackBox.h"
#include "circuit/fluid/SEFluidCircuitPath.h"

class CDM_DECL SEFluidBlackBox : public SEBlackBox<FLUID_BLACK_BOX>
{
  friend class SEBlackBoxManager;
  friend class CommonDataModelTest;
protected:
  SEFluidBlackBox(const std::string& name, Logger* logger);
public:
  virtual ~SEFluidBlackBox();

protected:

  virtual bool MapBlackBox(SEFluidCircuitPath& srcPath, SEFluidCircuitPath& tgtPath)
  {
    return SEBlackBox::MapBlackBox(srcPath, tgtPath);
  }
  virtual void MapBlackBox(SEFluidCircuitPath& srcPath, SEFluidCircuitPath tgtPath,
                           SEFluidCircuitNode& bbNode, SEFluidCircuitNode& srcNode, SEFluidCircuitNode tgtNode) override
  {
    bbNode.SetBlackBox(this);
    srcNode.SetBlackBox(this);
    tgtNode.SetBlackBox(this);
    srcPath.SetBlackBox(this);
    tgtPath.SetBlackBox(this);
  }

protected:

};
