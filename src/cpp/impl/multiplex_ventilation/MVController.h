/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologyEngine.h"
#include "controller/Controller.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "properties/SEScalarTime.h"

class MVController : public Loggable
{
public:
  MVController(const std::string& logfileName, const std::string& data_dir = ".");
  virtual ~MVController();

  bool  AdvanceModelTime(double time, const TimeUnit& unit);

protected:
  void SetupMultiplexVentilation();

  double                                  m_TimeStep_s;
  double                                  m_CurrentTime_s;
  double                                  m_SpareAdvanceTime_s;

  SESubstanceManager*                     m_SubMgr;
  SEDyspnea*                              m_Dyspnea;
  SEIntubation*                           m_Intubation;
  SEImpairedAlveolarExchangeExacerbation* m_ImpairedAlveolarExchangeExacerbation;
  SEMechanicalVentilatorConfiguration*    m_MechacicalVentilatorConfiguration;
  SESubstanceFraction*                    m_FiO2;

  std::vector<PulseController*>           m_Patients;
};
