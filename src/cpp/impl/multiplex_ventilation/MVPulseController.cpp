/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVPulseController.h"

#include "physiology/BloodChemistry.h"
#include "physiology/Cardiovascular.h"
#include "physiology/Drugs.h"
#include "physiology/Endocrine.h"
#include "physiology/Energy.h"
#include "physiology/Gastrointestinal.h"
#include "physiology/Hepatic.h"
#include "physiology/Nervous.h"
#include "physiology/Renal.h"
#include "physiology/Respiratory.h"
#include "physiology/Saturation.h"
#include "physiology/Tissue.h"
#include "environment/Environment.h"
#include "equipment/AnesthesiaMachine.h"
#include "equipment/ECG.h"
#include "equipment/Inhaler.h"
#include "equipment/MechanicalVentilator.h"

MVPulseController::MVPulseController(const std::string& logFileName, const std::string& data_dir) : PulseController(new Logger(logFileName), data_dir)
{

}

MVPulseController::~MVPulseController()
{

}

void MVPulseController::PreProcess()
{
  m_Environment->PreProcess();
  m_CardiovascularSystem->PreProcess();
  m_Inhaler->PreProcess();
  m_RespiratorySystem->PreProcess();
  m_AnesthesiaMachine->PreProcess();
  m_MechanicalVentilator->PreProcess();
  m_GastrointestinalSystem->PreProcess();
  m_HepaticSystem->PreProcess();
  m_RenalSystem->PreProcess();
  m_NervousSystem->PreProcess();
  m_EnergySystem->PreProcess();
  m_EndocrineSystem->PreProcess();
  m_DrugSystem->PreProcess();
  m_TissueSystem->PreProcess();
  m_BloodChemistrySystem->PreProcess();
  m_ECG->PreProcess();
}

void MVPulseController::Process()
{
  m_Environment->Process();
  m_CardiovascularSystem->Process();
  m_Inhaler->Process();
  m_RespiratorySystem->Process();
  m_AnesthesiaMachine->Process();
  m_MechanicalVentilator->Process();
  m_GastrointestinalSystem->Process();
  m_HepaticSystem->Process();
  m_RenalSystem->Process();
  m_NervousSystem->Process();
  m_EnergySystem->Process();
  m_EndocrineSystem->Process();
  m_DrugSystem->Process();
  m_TissueSystem->Process();
  m_BloodChemistrySystem->Process();
  m_ECG->Process();
}

void MVPulseController::PostProcess()
{
  m_Environment->PostProcess();
  m_CardiovascularSystem->PostProcess();
  m_Inhaler->PostProcess();
  m_RespiratorySystem->PostProcess();
  m_AnesthesiaMachine->PostProcess();
  m_MechanicalVentilator->PostProcess();
  m_GastrointestinalSystem->PostProcess();
  m_HepaticSystem->PostProcess();
  m_RenalSystem->PostProcess();
  m_NervousSystem->PostProcess();
  m_EnergySystem->PostProcess();
  m_EndocrineSystem->PostProcess();
  m_DrugSystem->PostProcess();
  m_TissueSystem->PostProcess();
  m_BloodChemistrySystem->PostProcess();
  m_ECG->PostProcess();
}
