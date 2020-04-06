/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "PulsePhysiologyEngine.h"
#include "controller/Controller.h"
#include "substance/SESubstanceManager.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceTransport.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
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

  bool Run(std::vector<std::string>& patients);

protected:
  std::string m_BaseFileName;
};
