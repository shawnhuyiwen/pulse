/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PulsePhysiologyEngine.h"
#include "properties/SEScalarTime.h"
#include "MVPulseController.h"

class MVController : public Loggable
{
public:
  MVController(const std::string& logfileName, const std::string& data_dir = ".");
  virtual ~MVController();

  void  AdvanceModelTime(double time, const TimeUnit& unit);

protected:
  void SetupMultiplexVentilation();

  void PreProcess();
  void Process();
  void PostProcess();

  double         m_SpareAdvanceTime_s;

  std::vector<MVPulseController*> m_Patients;
};
