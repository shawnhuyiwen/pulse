/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologyEngine.h"
#include "controller/Controller.h"

class MVPulseController : public PulseController
{
  friend class MVController;
public:
  MVPulseController(const std::string& logfileName, const std::string& data_dir = ".");
  virtual ~MVPulseController();

protected:
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;
};
