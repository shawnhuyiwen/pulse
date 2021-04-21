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
  PulseBloodChemistrySystem(PulseData& data) : SEBloodChemistrySystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseBloodChemistrySystem() = default;

  void Clear() override
  {
    SEBloodChemistrySystem::Clear();
  }

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
  PulseCardiovascularSystem(PulseData& data) : SECardiovascularSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseCardiovascularSystem() = default;

  void Clear() override
  {
    SECardiovascularSystem::Clear();
  }

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
  PulseDrugSystem(PulseData& data) : SEDrugSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseDrugSystem() = default;

  void Clear() override
  {
    SEDrugSystem::Clear();
  }

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
  PulseEndocrineSystem(PulseData& data) : SEEndocrineSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseEndocrineSystem() = default;

  void Clear() override
  {
    SEEndocrineSystem::Clear();
  }

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
  PulseEnergySystem(PulseData& data) : SEEnergySystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseEnergySystem() = default;

  void Clear() override
  {
    SEEnergySystem::Clear();
  }

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
  PulseGastrointestinalSystem(PulseData& data) : SEGastrointestinalSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseGastrointestinalSystem() = default;

  void Clear() override
  {
    SEGastrointestinalSystem::Clear();
  }

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
  PulseHepaticSystem(PulseData& data) : SEHepaticSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseHepaticSystem() = default;

  void Clear() override
  {
    SEHepaticSystem::Clear();
  }

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
  PulseNervousSystem(PulseData& data) : SENervousSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseNervousSystem() = default;

  void Clear() override
  {
    SENervousSystem::Clear();
  }

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
  PulseRenalSystem(PulseData& data) : SERenalSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseRenalSystem() = default;

  void Clear() override
  {
    SERenalSystem::Clear();
  }

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
  PulseRespiratorySystem(PulseData& data) : SERespiratorySystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseRespiratorySystem() = default;

  void Clear() override
  {
    SERespiratorySystem::Clear();
    m_TotalRespiratoryModelCompliance.Invalidate();
    m_TotalRespiratoryModelResistance.Invalidate();
  }

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
  PulseTissueSystem(PulseData& data) : SETissueSystem(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseTissueSystem() = default;

  void Clear() override
  {
    SETissueSystem::Clear();
  }

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
  PulseEnvironmentSystem(PulseData& data) : SEEnvironment(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseEnvironmentSystem() = default;

  void Clear() override
  {
    SEEnvironment::Clear();
  }

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
  PulseAnesthesiaMachine(PulseData& data) : SEAnesthesiaMachine(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseAnesthesiaMachine() = default;

  void Clear() override
  {
    SEAnesthesiaMachine::Clear();
  }

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
  PulseElectroCardioGram(PulseData& data) : SEElectroCardioGram(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseElectroCardioGram() = default;

  void Clear() override
  {
    SEElectroCardioGram::Clear();
  }

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
  PulseInhaler(PulseData& data) : SEInhaler(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseInhaler() = default;

  void Clear() override
  {
    SEInhaler::Clear();
  }

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
  PulseMechanicalVentilator(PulseData& data) : SEMechanicalVentilator(data.GetLogger()), PulseSystem(data) {}
  virtual ~PulseMechanicalVentilator() = default;

  void Clear() override
  {
    SEMechanicalVentilator::Clear();
  }

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