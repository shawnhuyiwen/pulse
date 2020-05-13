/* Distributed under the Apache License, Version 2.0.
See accompanying NOTICE file for details.*/

#pragma once

#include "controller/System.h"
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "system/physiology/SEEndocrineSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SEGastrointestinalSystem.h"
#include "system/physiology/SEHepaticSystem.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SERenalSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/physiology/SETissueSystem.h"
#include "system/environment/SEEnvironment.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"

#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"

// These classes are used to expose any methodology specific data to end users
// This data would not belong in the cdm, and it's not a clinical assessment, nor on a compartment
// It is data specific to the system implementation
// Users only get this data if they are using the pulse engine
// They should only contain const pure vitrual methods

class PULSE_DECL PulseBloodChemistrySystem : public SEBloodChemistrySystem, public PulseSystem
{
public:
  PulseBloodChemistrySystem(Logger* logger) : SEBloodChemistrySystem(logger) {}
  virtual ~PulseBloodChemistrySystem() = default;

  virtual const SEScalar* GetScalar(const std::string& name) override
  {
    const SEScalar* s = SEBloodChemistrySystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters()=0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseCardiovascularSystem : public SECardiovascularSystem, public PulseSystem
{
public:
  PulseCardiovascularSystem(Logger* logger) : SECardiovascularSystem(logger) {}
  virtual ~PulseCardiovascularSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SECardiovascularSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseDrugSystem : public SEDrugSystem, public PulseSystem
{
public:
  PulseDrugSystem(Logger* logger) : SEDrugSystem(logger) {}
  virtual ~PulseDrugSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEDrugSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseEndocrineSystem : public SEEndocrineSystem, public PulseSystem
{
public:
  PulseEndocrineSystem(Logger* logger) : SEEndocrineSystem(logger) {}
  virtual ~PulseEndocrineSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEEndocrineSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseEnergySystem : public SEEnergySystem, public PulseSystem
{
public:
  PulseEnergySystem(Logger* logger) : SEEnergySystem(logger) {}
  virtual ~PulseEnergySystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEEnergySystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseGastrointestinalSystem : public SEGastrointestinalSystem, public PulseSystem
{
public:
  PulseGastrointestinalSystem(Logger* logger) : SEGastrointestinalSystem(logger) {}
  virtual ~PulseGastrointestinalSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEGastrointestinalSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseHepaticSystem : public SEHepaticSystem, public PulseSystem
{
public:
  PulseHepaticSystem(Logger* logger) : SEHepaticSystem(logger) {}
  virtual ~PulseHepaticSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEHepaticSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseNervousSystem : public SENervousSystem, public PulseSystem
{
public:
  PulseNervousSystem(Logger* logger) : SENervousSystem(logger) {}
  virtual ~PulseNervousSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SENervousSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseRenalSystem : public SERenalSystem, public PulseSystem
{
public:
  PulseRenalSystem(Logger* logger) : SERenalSystem(logger) {}
  virtual ~PulseRenalSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SERenalSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseRespiratorySystem : public SERespiratorySystem, public PulseSystem
{
public:
  PulseRespiratorySystem(Logger* logger) : SERespiratorySystem(logger) {}
  virtual ~PulseRespiratorySystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SERespiratorySystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    if (name.compare("TotalRespiratoryModelCompliance") == 0)
      return &m_TotalRespiratoryModelCompliance;
    if (name.compare("TotalRespiratoryModelResistance") == 0)
      return &m_TotalRespiratoryModelResistance;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;

  DEFINE_UNIT_SCALAR(TotalRespiratoryModelCompliance, VolumePerPressure);
  DEFINE_UNIT_SCALAR(TotalRespiratoryModelResistance, PressureTimePerVolume);
};

class PULSE_DECL PulseTissueSystem : public SETissueSystem, public PulseSystem
{
public:
  PulseTissueSystem(Logger* logger) : SETissueSystem(logger) {}
  virtual ~PulseTissueSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SETissueSystem::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseEnvironmentSystem : public SEEnvironment, public PulseSystem
{
public:
  PulseEnvironmentSystem(SESubstanceManager& substances) : SEEnvironment(substances) {}
  virtual ~PulseEnvironmentSystem() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEEnvironment::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseAnesthesiaMachine : public SEAnesthesiaMachine, public PulseSystem
{
public:
  PulseAnesthesiaMachine(SESubstanceManager& substances) : SEAnesthesiaMachine(substances) {}
  virtual ~PulseAnesthesiaMachine() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEAnesthesiaMachine::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseElectroCardioGram : public SEElectroCardioGram, public PulseSystem
{
public:
  PulseElectroCardioGram(Logger* logger) : SEElectroCardioGram(logger) {}
  virtual ~PulseElectroCardioGram() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEElectroCardioGram::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseInhaler : public SEInhaler, public PulseSystem
{
public:
  PulseInhaler(SESubstanceManager& substances) : SEInhaler(substances) {}
  virtual ~PulseInhaler() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEInhaler::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};

class PULSE_DECL PulseMechanicalVentilator : public SEMechanicalVentilator, public PulseSystem
{
public:
  PulseMechanicalVentilator(SESubstanceManager& substances) : SEMechanicalVentilator(substances) {}
  virtual ~PulseMechanicalVentilator() = default;

  virtual const SEScalar* GetScalar(const std::string & name) override
  {
    const SEScalar* s = SEMechanicalVentilator::GetScalar(name);
    if (s != nullptr)
      return s;
    // Check to see if this a model specific request
    //if (name.compare("ModelParameter") == 0)
    //  return m_ModelParameter;
    return nullptr;
  }
  virtual void ComputeExposedModelParameters() = 0;
protected:
  //SEScalar m_ModelParameter;
};