/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

#include "engine/SEOverrides.h"
#include "patient/actions/SEDyspnea.h"
#include "patient/actions/SEIntubation.h"
#include "patient/actions/SEImpairedAlveolarExchangeExacerbation.h"
#include "patient/actions/SEPulmonaryShuntExacerbation.h"
#include "equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "equipment/mechanical_ventilator/SEMechanicalVentilator.h"

#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerPressure.h"

//--------------------------------------------------------------------------------------------------
/// \brief
/// Generate a set of patients stabilized on a ventilator
/// 1 ventilator for each patient
//--------------------------------------------------------------------------------------------------
void GenerateStabilizedPatients()
{
  double minCompliance = 0;
  double maxCompliance = 0;
  double stepCompliance = 0;
  double minImpairment = 0.0;
  double maxImpairment = 1.0;
  double stepImpairment = 0.1;
  double minPEEP = 12;
  double maxPEEP = 20;
  double stepPEEP = 2;
  double minPIP = 0;
  double maxPIP = 0;
  double stepPIP = 0;
  double minFiO2 = 0;
  double maxFiO2 = 0;
  double stepFiO2 = 0;

  Logger logger("MultiplexVentilationGenData.log");
  SESubstanceManager subMgr(&logger);
  subMgr.LoadSubstanceDirectory();
  SESubstance* Oxygen = subMgr.GetSubstance("Oxygen");

  SEDyspnea dyspnea;
  dyspnea.GetSeverity().SetValue(1.0);

  SEIntubation intubation;
  intubation.SetType(eIntubation_Type::Tracheal);

  SEOverrides overrides;

  SEImpairedAlveolarExchangeExacerbation impairedAlveolarExchange;
  SEPulmonaryShuntExacerbation pulmonaryShunt;

  SEMechanicalVentilatorConfiguration mvc(subMgr);
  auto& mv = mvc.GetConfiguration();
  mv.SetConnection(eMechanicalVentilator_Connection::Tube);
  mv.SetControl(eMechanicalVentilator_Control::P_CMV);
  mv.SetDriverWaveform(eMechanicalVentilator_DriverWaveform::Square);
  mv.GetRespiratoryRate().SetValue(20, FrequencyUnit::Per_min);
  mv.GetInspiratoryExpiratoryRatio().SetValue(0.5);
  mv.GetPeakInspiratoryPressure().SetValue(28, PressureUnit::cmH2O);
  mv.GetPositiveEndExpiredPressure().SetValue(10, PressureUnit::cmH2O);
  SESubstanceFraction* FiO2 = &mv.GetFractionInspiredGas(*Oxygen);
  FiO2->GetFractionAmount().SetValue(0.3);

  for (double c=minCompliance; c<maxCompliance; c+=stepCompliance)
  {
    overrides.AddPair("LungCompliance", c);
    for (double i=minImpairment; i<maxImpairment; i+=stepImpairment)
    {
      // jbw Convert i to the appropriate action severity
      impairedAlveolarExchange.GetSeverity().SetValue(i);
      pulmonaryShunt.GetSeverity().SetValue(i);

      for (double peep=minPEEP; peep<maxPEEP; peep+=stepPEEP)
      {
        mv.GetInspiratoryExpiratoryRatio().SetValue(peep);

        // Step the PIP until we get a TidalVolume between 6-8 mL
        for (double pip=minPIP; pip<maxPIP; pip+=stepPIP)
        {
          mv.GetPeakInspiratoryPressure().SetValue(pip, PressureUnit::cmH2O);
        }

        // Step the FiO2 until we get an SpO2 of > 90
        for (double o2=minFiO2; o2<maxFiO2; o2+=stepFiO2)
        {
          FiO2->GetFractionAmount().SetValue(o2);
        }

        // Save our state

        // Append to our "list" of generated states
      }
    }
  }
  // Write out our list file
}