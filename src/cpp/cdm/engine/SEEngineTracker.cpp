/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEEngineTracker.h"
#include "engine/SEActionManager.h"
#include "engine/SEDataRequest.h"
#include "engine/SEDataRequestManager.h"
#include "PhysiologyEngine.h"
#include "patient/SEPatient.h"
// Compartments
#include "compartment/SECompartmentManager.h"
#include "compartment/fluid/SEGasCompartment.h"
#include "compartment/fluid/SEGasCompartmentLink.h"
#include "compartment/fluid/SELiquidCompartment.h"
#include "compartment/fluid/SELiquidCompartmentLink.h"
#include "compartment/thermal/SEThermalCompartment.h"
#include "compartment/tissue/SETissueCompartment.h"
// Circuit
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitNode.h"
#include "circuit/fluid/SEFluidCircuitPath.h"
#include "circuit/thermal/SEThermalCircuit.h"
#include "circuit/thermal/SEThermalCircuitNode.h"
#include "circuit/thermal/SEThermalCircuitPath.h"
#include "circuit/electrical/SEElectricalCircuit.h"
#include "circuit/electrical/SEElectricalCircuitNode.h"
#include "circuit/electrical/SEElectricalCircuitPath.h"
// Substances
#include "substance/SESubstance.h"
#include "substance/SESubstancePharmacokinetics.h"
#include "substance/SESubstanceTissuePharmacokinetics.h"
#include "substance/SESubstanceManager.h"
// Patient
#include "patient/SEPatient.h"
// Systems
#include "system/physiology/SEBloodChemistrySystem.h"
#include "system/physiology/SECardiovascularSystem.h"
#include "system/physiology/SEDrugSystem.h"
#include "system/physiology/SEEndocrineSystem.h"
#include "system/physiology/SEEnergySystem.h"
#include "system/physiology/SEGastrointestinalSystem.h"
#include "system/physiology/SENervousSystem.h"
#include "system/physiology/SERenalSystem.h"
#include "system/physiology/SERespiratorySystem.h"
#include "system/physiology/SETissueSystem.h"
#include "system/environment/SEEnvironment.h"
#include "system/equipment/anesthesia_machine/SEAnesthesiaMachine.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "system/equipment/inhaler/SEInhaler.h"
#include "system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
// Scalars
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalarMassPerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarAmountPerVolume.h"
#include "properties/SEScalarMass.h"
#include "properties/SEScalar0To1.h"
#include "properties/SEScalarFrequency.h"
#include "properties/SEScalarElectricPotential.h"
#include "utils/DataTrack.h"

std::string Space2Underscore(const std::string& str)
{
  std::string s = str; 
  std::transform(s.begin(), s.end(), s.begin(), [](char ch) {
    return ch == ' ' ? '_' : ch;
  });
  return s;
}

SEEngineTracker::SEEngineTracker(SEPatient& p, SEActionManager& a, SESubstanceManager& s, SECompartmentManager& c, Logger* logger) : Loggable(logger),
  m_Patient(p), m_ActionMgr(a), m_SubMgr(s), m_CmptMgr(c)
{
  m_DataTrack = new DataTrack(logger);
  m_DataRequestMgr = new SEDataRequestManager(logger);
  m_ForceConnection = false;
}

SEEngineTracker::~SEEngineTracker()
{
  Clear();
  delete m_DataTrack;
  delete m_DataRequestMgr;
}

void SEEngineTracker::Clear()
{
  ResetFile();
  m_ForceConnection = false;
  DELETE_MAP_SECOND(m_Request2Scalar);
  m_DataRequestMgr->Clear();
}

void SEEngineTracker::AddSystem(SESystem& sys)
{
  if (dynamic_cast<SEEnvironment*>(&sys) != nullptr)
  {
    m_Environment = dynamic_cast<SEEnvironment*>(&sys);
    return;
  }
  if (dynamic_cast<SEAnesthesiaMachine*>(&sys) != nullptr)
  {
    m_AnesthesiaMachine = dynamic_cast<SEAnesthesiaMachine*>(&sys);
    return;
  }
  if (dynamic_cast<SEElectroCardioGram*>(&sys) != nullptr)
  {
    m_ECG = dynamic_cast<SEElectroCardioGram*>(&sys);
    return;
  }
  if (dynamic_cast<SEInhaler*>(&sys) != nullptr)
  {
    m_Inhaler = dynamic_cast<SEInhaler*>(&sys);
    return;
  }
  if (dynamic_cast<SEMechanicalVentilator*>(&sys) != nullptr)
  {
    m_MechanicalVentilator = dynamic_cast<SEMechanicalVentilator*>(&sys);
    return;
  }
  // Not equipment or environment, so it must be a physiology system
  m_PhysiologySystems.push_back(&sys);
}

void SEEngineTracker::ResetFile()
{
  if (m_ResultsStream.is_open())
    m_ResultsStream.close();
}

DataTrack& SEEngineTracker::GetDataTrack()
{
  return *m_DataTrack;
}

double SEEngineTracker::GetValue(const SEDataRequest& dr) const
{
  auto drs = GetScalar(dr);
  if (drs == nullptr)
    return SEScalar::dNaN();
  if (!drs->IsValid())
    return SEScalar::dNaN();
  if (dr.HasUnit())
    return drs->GetValue(*dr.GetUnit());
  return drs->GetValue();
}

const SEDataRequestScalar* SEEngineTracker::GetScalar(const SEDataRequest& dr) const
{
  auto found = m_Request2Scalar.find(&dr);
  if (found == m_Request2Scalar.end())
    return nullptr;
  return found->second;
}

void SEEngineTracker::SetupRequests()
{
  if (m_Mode == TrackMode::CSV)
  {
    bool isOpen = m_ResultsStream.is_open();
    if (!isOpen || m_ForceConnection)
    {// Process/Hook up all requests with their associated scalers
      DELETE_MAP_SECOND(m_Request2Scalar);// Get our scalars again
      for (SEDataRequest* dr : m_DataRequestMgr->GetDataRequests())
      {
        if (!TrackRequest(*dr))
        {// Could not hook this up, get rid of it
          m_ss << "Unable to find data for " << m_Request2Scalar[dr]->Heading;
          Error(m_ss);
        }
      }
      m_ForceConnection = false;
    }
    // Create the file now that all probes and requests have been added to the track
    // So we get columns for all of our data
    if (!isOpen)
      m_DataTrack->CreateFile(m_DataRequestMgr->GetResultFilename().c_str(), m_ResultsStream);
  }
  else
  {
    for (SEDataRequest* dr : m_DataRequestMgr->GetDataRequests())
    {
      if (!TrackRequest(*dr))
      {// Could not hook this up, get rid of it
        m_ss << "Unable to find data for " << m_Request2Scalar[dr]->Heading;
        Error(m_ss);
      }
    }
  }
}

void SEEngineTracker::TrackData(double time_s)
{
  if (!m_DataRequestMgr->HasDataRequests())
    return;// Nothing to do here...

  SetupRequests();
  PullData();

  if(m_Mode == TrackMode::CSV)
    m_DataTrack->StreamProbesToFile(time_s, m_ResultsStream);
}
void SEEngineTracker::PullData()
{
  SEDataRequestScalar* ds;
  for (SEDataRequest* dr : m_DataRequestMgr->GetDataRequests())
  {
    ds = m_Request2Scalar[dr];
    if (ds == nullptr)
    {
      Error("You cannot modify CSV Results file data requests in the middle of a run.");
      Error("Ignorning data request " + dr->GetPropertyName());
      continue;
    }
    if (!ds->HasScalar())
    {
      m_DataTrack->Probe(ds->idx, SEScalar::dNaN());
      continue;
    }
    ds->UpdateScalar();// Update compartment if needed
    if (ds->IsValid())
    {
      if (ds->HasUnit())
      {
        if (dr->GetUnit() == nullptr)
          dr->SetUnit(*ds->GetUnit());
        m_DataTrack->Probe(ds->idx, ds->GetValue(*dr->GetUnit()));
      }
      else
        m_DataTrack->Probe(ds->idx, ds->GetValue());
    }
    else if (ds->IsInfinity())
      m_DataTrack->Probe(ds->idx, std::numeric_limits<double>::infinity());
    else
      m_DataTrack->Probe(ds->idx, SEScalar::dNaN());
  }
}


bool SEEngineTracker::TrackRequest(SEDataRequest& dr)
{
  if (m_Request2Scalar.find(&dr) != m_Request2Scalar.end())
    return true; // We have this connected already

  SEDataRequestScalar* ds=new SEDataRequestScalar(GetLogger());
  m_Request2Scalar[&dr]=ds;

  bool success = ConnectRequest(dr, *ds);

  switch (dr.GetCategory())
  {
    case eDataRequest_Category::Patient:
      m_ss << "Patient";
    case eDataRequest_Category::Physiology:
    case eDataRequest_Category::Environment:
    case eDataRequest_Category::AnesthesiaMachine:
    case eDataRequest_Category::ECG:
    case eDataRequest_Category::Inhaler:
    case eDataRequest_Category::MechanicalVentilator:
    {
      if (!dr.GetUnit())
        m_ss << dr.GetPropertyName();
      else
        m_ss << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";

      ds->Heading = Space2Underscore(m_ss.str());
      m_ss.str("");//Reset Buffer
      ds->idx = m_DataTrack->Probe(ds->Heading, 0);
      m_DataTrack->SetFormatting(ds->Heading, dr);
      return success;
    }
    case eDataRequest_Category::Action:
    {
      if (dr.HasCompartmentName() && dr.HasSubstanceName())
      {
        if (!dr.GetUnit())
          m_ss << dr.GetActionName() << "-" << dr.GetCompartmentName() << "-" << dr.GetSubstanceName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetActionName() << "-" << dr.GetCompartmentName() << "-" << dr.GetSubstanceName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
      }
      else if (dr.HasCompartmentName())
      {
        if (!dr.GetUnit())
          m_ss << dr.GetActionName() << "-" << dr.GetCompartmentName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetActionName() << "-" << dr.GetCompartmentName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
      }
      else if (dr.HasSubstanceName())
      {
        if (!dr.GetUnit())
          m_ss << dr.GetActionName() << "-" << dr.GetSubstanceName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetActionName() << "-" << dr.GetSubstanceName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
      }
      else
      {
        if (!dr.GetUnit())
          m_ss << dr.GetActionName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetActionName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
      }
      ds->Heading = Space2Underscore(m_ss.str());
      m_ss.str("");//Reset Buffer
      ds->idx = m_DataTrack->Probe(ds->Heading, 0);
      m_DataTrack->SetFormatting(ds->Heading, dr);
      return success;
    }
    case eDataRequest_Category::GasCompartment:
    case eDataRequest_Category::LiquidCompartment:
    case eDataRequest_Category::ThermalCompartment:
    case eDataRequest_Category::TissueCompartment:
    {
      if (dr.HasSubstanceName())
      {
        if (!dr.GetUnit())
          m_ss << dr.GetCompartmentName() << "-" << dr.GetSubstanceName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetCompartmentName() << "-" << dr.GetSubstanceName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
      }
      else
      {
        if (!dr.GetUnit())
          m_ss << dr.GetCompartmentName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetCompartmentName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
      }
      ds->Heading = Space2Underscore(m_ss.str());
      m_ss.str("");//Reset Buffer
      ds->idx = m_DataTrack->Probe(ds->Heading, 0);
      m_DataTrack->SetFormatting(ds->Heading, dr);
      return success;
    }
    case eDataRequest_Category::Substance:
    {
      if (dr.HasCompartmentName())
      {
        if (!dr.GetUnit())
          m_ss << dr.GetSubstanceName() << "-" << dr.GetCompartmentName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetSubstanceName() << "-" << dr.GetCompartmentName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
        ds->Heading = Space2Underscore(m_ss.str());
        m_ss.str("");//Reset Buffer
        ds->idx = m_DataTrack->Probe(ds->Heading, 0);
        m_DataTrack->SetFormatting(ds->Heading, dr);
        return success;
      }
      else
      {
        if (!dr.GetUnit())
          m_ss << dr.GetSubstanceName() << "-" << dr.GetPropertyName();
        else
          m_ss << dr.GetSubstanceName() << "-" << dr.GetPropertyName() << "(" << *dr.GetUnit() << ")";
        ds->Heading = Space2Underscore(m_ss.str());
        m_ss.str("");//Reset Buffer
        ds->idx = m_DataTrack->Probe(ds->Heading, 0);
        m_DataTrack->SetFormatting(ds->Heading, dr);
        return success;
      }
    }
    default:
      m_ss << "Unhandled data request category: " << eDataRequest_Category_Name(dr.GetCategory()) << std::endl;
      Error(m_ss);
  }

  m_ss << "Unhandled data request : " << dr.GetPropertyName() << std::endl;
  Error(m_ss);
  return false;
}

bool SEEngineTracker::ConnectRequest(SEDataRequest& dr, SEDataRequestScalar& ds)
{
  const SEScalar* s = nullptr;
  std::string propertyName = dr.GetPropertyName();
  switch (dr.GetCategory())
  {
    case eDataRequest_Category::Patient:
    {
      s = m_Patient.GetScalar(propertyName);
      break;
    }
    case eDataRequest_Category::Physiology:
    {
      s = SESystem::GetScalar(propertyName, &m_PhysiologySystems);
      break;
    }    
    case eDataRequest_Category::Environment:
    {
      if (m_Environment != nullptr)
        s = m_Environment->GetScalar(propertyName);
      else
        Error("Cannot track environment data as no environment was provide");
      break;
    }
    case eDataRequest_Category::Action:
    {
      s = m_ActionMgr.GetScalar(dr.GetActionName(), dr.GetCompartmentName(), dr.GetSubstanceName(), propertyName);
      break;
    }
    case eDataRequest_Category::AnesthesiaMachine:
    {
      if (m_AnesthesiaMachine != nullptr)
        s = m_AnesthesiaMachine->GetScalar(propertyName);
      else
        Error("Cannot track anesthesia machine data as no anesthesia machine was provide");
      break;
    }
    case eDataRequest_Category::ECG:
    {
      if (m_ECG != nullptr)
        s = m_ECG->GetScalar(propertyName);
      else
        Error("Cannot track ECG data as no ECG was provide");
      break;
    }
    case eDataRequest_Category::Inhaler:
    {
      if (m_Inhaler != nullptr)
        s = m_Inhaler->GetScalar(propertyName);
      else
        Error("Cannot track inhaler data as no inhaler was provide");
      break;
    }
    case eDataRequest_Category::MechanicalVentilator:
    {
      if (m_MechanicalVentilator != nullptr)
        s = m_MechanicalVentilator->GetScalar(propertyName);
      else
        Error("Cannot track mechanical ventilator data as no mechanical ventilator was provide");
      break;
    }
    case eDataRequest_Category::GasCompartment:
    {
      if (!m_CmptMgr.HasGasCompartment(dr.GetCompartmentName()))
      {
        Error("Unknown gas compartment : " + dr.GetCompartmentName());
        return false;
      }
      // Removing const because I need to create objects in order to track those objects
      SEGasCompartment* gasCmpt = (SEGasCompartment*)m_CmptMgr.GetGasCompartment(dr.GetCompartmentName());
      if (dr.HasSubstanceName())
      {
        SESubstance* sub = m_SubMgr.GetSubstance(dr.GetSubstanceName());
        // Activate this substance so compartments have it
        m_SubMgr.AddActiveSubstance(*sub);
        if (gasCmpt->HasChildren())
        {
          if (propertyName == "Volume")
            ds.UpdateProperty = CompartmentUpdate::Volume;
          else if (propertyName == "VolumeFraction")
            ds.UpdateProperty = CompartmentUpdate::VolumeFraction;
          else if (propertyName == "PartialPressure")
            ds.UpdateProperty = CompartmentUpdate::PartialPressure;
          ds.GasSubstance = gasCmpt->GetSubstanceQuantity(*sub);
        }
        s = gasCmpt->GetSubstanceQuantity(*sub)->GetScalar(propertyName);
      }
      else
      {
        if (gasCmpt->HasChildren() || gasCmpt->HasNodeMapping())
        {
          if (propertyName == "Volume")
            ds.UpdateProperty = CompartmentUpdate::Volume;
          if (propertyName == "Pressure")
            ds.UpdateProperty = CompartmentUpdate::Pressure;
        }

        {// Always Update these
          if (propertyName == "InFlow")
            ds.UpdateProperty = CompartmentUpdate::InFlow;
          else if (propertyName == "OutFlow")
            ds.UpdateProperty = CompartmentUpdate::OutFlow;
        }
        ds.GasCmpt = gasCmpt;
        s = gasCmpt->GetScalar(propertyName);
      }
      break;
    }
    case eDataRequest_Category::LiquidCompartment:
    {
      if (!m_CmptMgr.HasLiquidCompartment(dr.GetCompartmentName()))
      {
        Error("Unknown liquid compartment : " + dr.GetCompartmentName());
        return false;
      }
      // Removing const because I need to create objects in order to track those objects
      SELiquidCompartment* liquidCmpt = (SELiquidCompartment*)m_CmptMgr.GetLiquidCompartment(dr.GetCompartmentName());

      if (dr.HasSubstanceName())
      {
        SESubstance* sub = m_SubMgr.GetSubstance(dr.GetSubstanceName());
        // Activate this substance so compartments have it
        m_SubMgr.AddActiveSubstance(*sub);
        if (liquidCmpt->HasChildren())
        {
          if (propertyName == "Mass")
            ds.UpdateProperty = CompartmentUpdate::Mass;
          else if (propertyName == "Concentration")
            ds.UpdateProperty = CompartmentUpdate::Concentration;
          else if (propertyName == "Molarity")
            ds.UpdateProperty = CompartmentUpdate::Molarity;
          else if (propertyName == "PartialPressure")
            ds.UpdateProperty = CompartmentUpdate::PartialPressure;
          else if (propertyName == "Saturation")
            ds.UpdateProperty = CompartmentUpdate::Saturation;
          ds.LiquidSubstance = liquidCmpt->GetSubstanceQuantity(*sub);
        }
        s = liquidCmpt->GetSubstanceQuantity(*sub)->GetScalar(propertyName);
      }
      else
      {
        if (liquidCmpt->HasChildren() || liquidCmpt->HasNodeMapping())
        {
          if (propertyName == "Volume")
            ds.UpdateProperty = CompartmentUpdate::Volume;
          if (propertyName == "Pressure")
            ds.UpdateProperty = CompartmentUpdate::Pressure;
        }

        {// Always Update these
          if (propertyName == "InFlow")
            ds.UpdateProperty = CompartmentUpdate::InFlow;
          else if (propertyName == "OutFlow")
            ds.UpdateProperty = CompartmentUpdate::OutFlow;
        }
        ds.LiquidCmpt = liquidCmpt;
        s = liquidCmpt->GetScalar(propertyName);
      }
      break;
    }
    case eDataRequest_Category::ThermalCompartment:
    {
      if (!m_CmptMgr.HasThermalCompartment(dr.GetCompartmentName()))
      {
        Error("Unknown thermal compartment : " + dr.GetCompartmentName());
        return false;
      }
      // Removing const because I need to create objects in order to track those objects
      SEThermalCompartment* thermalCmpt = (SEThermalCompartment*)m_CmptMgr.GetThermalCompartment(dr.GetCompartmentName());

      if (thermalCmpt->HasChildren() || thermalCmpt->HasNodeMapping())
      {
        if (propertyName == "Heat")
          ds.UpdateProperty = CompartmentUpdate::Heat;
        if (propertyName == "Temperature")
          ds.UpdateProperty = CompartmentUpdate::Temperature;
      }

      {// Always Update these       
        if (propertyName == "HeatTransferRateIn")
          ds.UpdateProperty = CompartmentUpdate::HeatTransferRateIn;
        else if (propertyName == "HeatTransferRateOut")
          ds.UpdateProperty = CompartmentUpdate::HeatTransferRateOut;
      }
      ds.ThermalCmpt = thermalCmpt;
      s = thermalCmpt->GetScalar(propertyName);
      break;
    }
    case eDataRequest_Category::TissueCompartment:
    {
      if (!m_CmptMgr.HasTissueCompartment(dr.GetCompartmentName()))
      {
        Error("Unknown tissue compartment : " + dr.GetCompartmentName());
        return false;
      }
      // Removing const because I need to create objects in order to track those objects
      SETissueCompartment* tissueCmpt = (SETissueCompartment*)m_CmptMgr.GetTissueCompartment(dr.GetCompartmentName());
      s = tissueCmpt->GetScalar(propertyName);
      break;
    }
    case eDataRequest_Category::Substance:
    {
      // Removing const because I want to allocate and grab scalars to track for later
      SESubstance* sub = m_SubMgr.GetSubstance(dr.GetSubstanceName());
      m_SubMgr.AddActiveSubstance(*sub);
      if (dr.HasCompartmentName())
      {// I don't really have a generic/reflexive way of doing this...yet
        if (dr.GetPropertyName() == "PartitionCoefficient")
        {
          SESubstanceTissuePharmacokinetics& tk = sub->GetPK().GetTissueKinetics(dr.GetCompartmentName());
          s = &tk.GetPartitionCoefficient();
          break;
        }
      }
      else
      {
        s = sub->GetScalar(propertyName);
        break;
      }
    }
    default:
      m_ss << "Unhandled data request category: " << eDataRequest_Category_Name(dr.GetCategory()) << std::endl;
      Error(m_ss);
  }

  if (s != nullptr)
  {
    ds.SetScalar(s, dr);
    return true;
  }
  m_ss << "Unhandled data request : " << propertyName << std::endl;
  Error(m_ss);
  return false;
}

void SEDataRequestScalar::SetScalar(const SEScalar* s, SEDataRequest& dr)
{
  if (s==nullptr)
  {
    Error("Unknown Data Request : " + dr.GetPropertyName());
    return;
  }
  SEGenericScalar::SetScalar(*s);
  if (m_UnitScalar != nullptr)
  {
    if (!dr.HasRequestedUnit())// Use set unit if none provide
    {
      if (!dr.HasUnit())//use the unit it has if there is no requested unit
      {
        if(HasUnit())
          dr.SetUnit(*GetUnit());
        else
        {
          Error("I have no idea what unit you want this data request in : " + dr.GetPropertyName());
        }
      }
    }
    else
    {
      const CCompoundUnit* unit = GetCompoundUnit(dr.GetRequestedUnit());
      if (unit==nullptr)
      {
        std::stringstream ss;
        ss << dr.GetRequestedUnit() << " is not compatible with " << dr.GetPropertyName();
        Fatal(ss);
      }
      dr.SetUnit(*unit);
    }
  }
}

void SEDataRequestScalar::UpdateScalar()
{
  if (UpdateProperty == CompartmentUpdate::None)
    return;

  if (GasCmpt != nullptr)
  {
    switch (UpdateProperty)
    {
    case CompartmentUpdate::InFlow:
      GasCmpt->GetInFlow();
      return;
    case CompartmentUpdate::OutFlow:
      GasCmpt->GetOutFlow();
      return;
    case CompartmentUpdate::Volume:
      GasCmpt->GetVolume();
      return;
    case CompartmentUpdate::Pressure:
      GasCmpt->GetPressure();
      return;
    default:    
      Error("Property is not supported on Gas Compartment");    
    }
  }
  else if (GasSubstance != nullptr)
  {
    switch (UpdateProperty)
    {
    case CompartmentUpdate::Volume:
      GasSubstance->GetVolume();
      return;
    case CompartmentUpdate::VolumeFraction:
      GasSubstance->GetVolumeFraction();
      return;
    case CompartmentUpdate::PartialPressure:
      GasSubstance->GetPartialPressure();
      return;
    default:
      Error("Property is not supported on Gas Substance");
    }
  }
  else if (LiquidCmpt != nullptr)
  {
    switch (UpdateProperty)
    {
    case CompartmentUpdate::InFlow:
      LiquidCmpt->GetInFlow();
      return;
    case CompartmentUpdate::OutFlow:
      LiquidCmpt->GetOutFlow();
      return;
    case CompartmentUpdate::Volume:
      LiquidCmpt->GetVolume();
      return;
    case CompartmentUpdate::pH:
      LiquidCmpt->GetPH();
      return;
    case CompartmentUpdate::Pressure:
      LiquidCmpt->GetPressure();
      return;
    default:
      Error("Property is not supported on liquid Compartment");
    }
  }
  else if (LiquidSubstance != nullptr)
  {
    switch (UpdateProperty)
    {
    case CompartmentUpdate::Mass:
      LiquidSubstance->GetMass();
      return;
    case CompartmentUpdate::Molarity:
      LiquidSubstance->GetMolarity();
      return;
    case CompartmentUpdate::Concentration:
      LiquidSubstance->GetConcentration();
      return;
    case CompartmentUpdate::PartialPressure:
      LiquidSubstance->GetPartialPressure();
      return;
    case CompartmentUpdate::Saturation:
      LiquidSubstance->GetSaturation();
      return;
    default:
      Error("Property is not supported on Liquid Substance");
    }
  }
  else if (ThermalCmpt != nullptr)
  {
    switch (UpdateProperty)
    {
    case CompartmentUpdate::HeatTransferRateIn:
      ThermalCmpt->GetHeatTransferRateIn();
      return;
    case CompartmentUpdate::HeatTransferRateOut:
      ThermalCmpt->GetHeatTransferRateOut();
      return;   
    case CompartmentUpdate::Heat:
      ThermalCmpt->GetHeat();
      return;
    case CompartmentUpdate::Temperature:
      ThermalCmpt->GetTemperature();
      return;
    default:
      Error("Property is not supported on Thermal Substance");
    }
  }
  Error("Could not update " + Heading);
}
