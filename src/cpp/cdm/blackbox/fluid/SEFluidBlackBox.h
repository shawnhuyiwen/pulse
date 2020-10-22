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

};
