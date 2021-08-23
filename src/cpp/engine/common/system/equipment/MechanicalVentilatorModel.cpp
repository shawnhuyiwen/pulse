/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/PulseConfiguration.h"
#include "engine/common/controller/CircuitManager.h"
#include "engine/common/controller/CompartmentManager.h"
#include "engine/common/controller/SubstanceManager.h"
#include "engine/common/system/equipment/MechanicalVentilatorModel.h"

#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorConfiguration.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEEquipmentActionCollection.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/circuit/fluid/SEFluidCircuit.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/substances/SEGasSubstanceQuantity.h"
#include "cdm/compartment/substances/SELiquidSubstanceQuantity.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/GeneralMath.h"

namespace PULSE_ENGINE
{
  /*
  ========================
    Constructors
  ========================
  */
  MechanicalVentilatorModel::MechanicalVentilatorModel(Data& data) : MechanicalVentilatorSystem(data.GetLogger()), Model(data)
  {
    Clear();
  }

  MechanicalVentilatorModel::~MechanicalVentilatorModel()
  {
    Clear();
  }

  void MechanicalVentilatorModel::Clear()
  {
    MechanicalVentilatorSystem::Clear();
    m_Environment = nullptr;
    m_Ventilator = nullptr;
    m_VentilatorAerosol = nullptr;
    m_VentilatorNode = nullptr;
    m_ExpiratoryLimbNode = nullptr;
    m_ExpiratoryValveNode = nullptr;
    m_InspiratoryLimbNode = nullptr;
    m_InspiratoryValveNode = nullptr;
    m_YPieceNode = nullptr;
    m_ConnectionNode = nullptr;
    m_AmbientNode = nullptr;
    m_EnvironmentToVentilator = nullptr;
    m_YPieceToConnection = nullptr;
    m_VentilatorToExpiratoryValve = nullptr;
    m_VentilatorToInspiratoryValve = nullptr;
    m_ExpiratoryLimbToYPiece = nullptr;
    m_InspiratoryLimbToYPiece = nullptr;
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes system properties to valid homeostatic values.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::Initialize()
  {
    Model::Initialize();
    SetConnection(eMechanicalVentilator_Connection::Off);
    m_CurrentBreathState = eBreathState::Exhale;
    m_CurrentPeriodTime_s = 0.0;
    m_DriverPressure_cmH2O = SEScalar::dNaN();
    m_DriverFlow_L_Per_s = SEScalar::dNaN();
    m_CurrentInspiratoryVolume_L = 0.0;
    StateChange();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes parameters for the mechanical ventilator class
  ///
  /// \details
  /// Initializes member variables and system level values on the common data model.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::SetUp()
  {
    // Compartments
    m_Environment = m_data.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
    m_Ventilator = m_data.GetCompartments().GetGasCompartment(pulse::MechanicalVentilatorCompartment::MechanicalVentilator);
    m_VentilatorAerosol = m_data.GetCompartments().GetLiquidCompartment(pulse::MechanicalVentilatorCompartment::MechanicalVentilator);

    // Nodes
    m_VentilatorNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::Ventilator);
    m_ExpiratoryLimbNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::ExpiratoryLimb);
    m_ExpiratoryValveNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::ExpiratoryValve);
    m_InspiratoryLimbNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::InspiratoryLimb);
    m_InspiratoryValveNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::InspiratoryValve);
    m_YPieceNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::YPiece);
    m_ConnectionNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::MechanicalVentilatorNode::Connection);
    m_AmbientNode = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetNode(pulse::EnvironmentNode::Ambient);

    // Paths
    m_EnvironmentToVentilator = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::EnvironmentToVentilator);
    m_YPieceToConnection = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::YPieceToConnection);
    m_VentilatorToExpiratoryValve = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::VentilatorToExpiratoryValve);
    m_VentilatorToInspiratoryValve = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::VentilatorToInspiratoryValve);
    m_ExpiratoryLimbToYPiece = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::ExpiratoryLimbToYPiece);
    m_InspiratoryLimbToYPiece = m_data.GetCircuits().GetMechanicalVentilatorCircuit().GetPath(pulse::MechanicalVentilatorPath::InspiratoryLimbToYPiece);
  }

  void MechanicalVentilatorModel::StateChange()
  {
    if (m_data.GetAirwayMode() != eAirwayMode::MechanicalVentilator)
      return;
    SetConnection();
    m_CurrentBreathState = eBreathState::Exhale;
    m_CurrentPeriodTime_s = 0.0;
    m_CurrentInspiratoryVolume_L = 0.0;

    // If you have one substance, make sure its Oxygen and add the standard CO2 and N2 to fill the difference

    //Set the substance volume fractions ********************************************
    std::vector<SESubstanceFraction*> gasFractions = GetSettings().GetFractionInspiredGases();

    //Reset the substance quantities at the connection
    for (SEGasSubstanceQuantity* subQ : m_Ventilator->GetSubstanceQuantities())
      subQ->SetToZero();

    //Start by setting everything to ambient
    for (auto s : m_Environment->GetSubstanceQuantities())
    {
      m_Ventilator->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().Set(s->GetVolumeFraction());
    }

    //Has fractions defined
    for (auto f : gasFractions)
    {
      const SESubstance& sub = f->GetSubstance();
      double fraction = f->GetFractionAmount().GetValue();

      //Do this, just in case it's something new
      m_data.GetSubstances().AddActiveSubstance(sub);

      //Now set it on the ventilator compartment
      //It has a infinate volume, so this will keep the same volume fraction no matter what's going on around it
      m_Ventilator->GetSubstanceQuantity(sub)->GetVolumeFraction().SetValue(fraction);
    }

    double totalFractionDefined = 0.0;
    for (auto s : m_Ventilator->GetSubstanceQuantities())
    {
      totalFractionDefined += m_Ventilator->GetSubstanceQuantity(s->GetSubstance())->GetVolumeFraction().GetValue();
    }

    //Add or remove Nitrogen to balance
    double gasFractionDiff = 1.0 - totalFractionDefined;
    double currentN2Fraction = m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().GetValue();
    if (currentN2Fraction + gasFractionDiff < 0.0)
    {
      double FiO2 = m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetVolumeFraction().GetValue();

      /// \error Error: FiO2 setting + ambient fractions other than N2 is greater than 1.0. Setting FiO2 to max value
      m_ss << "FiO2 setting + ambient fractions other than N2 is greater than 1.0. Setting FiO2 to max value of " << FiO2 + currentN2Fraction + gasFractionDiff << ".";
      Error(m_ss);
      m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().SetValue(0.0);
      m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetVolumeFraction().SetValue(FiO2 + currentN2Fraction + gasFractionDiff);
    }
    else
    {
      Info("Adding "+cdm::to_string(currentN2Fraction + gasFractionDiff) + "% of N2 to the system");
      m_Ventilator->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().SetValue(currentN2Fraction + gasFractionDiff);
    }

    //Set the aerosol concentrations ********************************************
    std::vector<SESubstanceConcentration*> liquidConcentrations = GetSettings().GetConcentrationInspiredAerosols();

    //Reset the substance quantities at the ventilator
    for (SELiquidSubstanceQuantity* subQ : m_VentilatorAerosol->GetSubstanceQuantities())
      subQ->SetToZero();

    if (!liquidConcentrations.empty())
    {
      //Has fractions defined
      for (auto f : liquidConcentrations)
      {
        const SESubstance& sub = f->GetSubstance();
        SEScalarMassPerVolume concentration = f->GetConcentration();

        //Do this, just in case it's something new
        m_data.GetSubstances().AddActiveSubstance(sub);
        //Now set it on the connection compartment
        //It has infinite volume, so this will keep the same volume fraction no matter what's going on around it
        m_VentilatorAerosol->GetSubstanceQuantity(sub)->GetConcentration().Set(concentration);
      }
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Connect to the patient via the ventilator mask, an endotracheal tube, or no connection
  ///
  /// \param  enumMechanicalVentilatorConnection
  /// Connectoin type : Mask, tube, or off
  /// 
  /// \details
  /// If the enum is set to mask, then the mask is applied to the face
  /// If the enum is set to tube, then the ventilator is connected to the tube
  /// If the enum is set to off, the airway mode is set to free.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::SetConnection(eMechanicalVentilator_Connection c)
  {
    if (GetSettings().GetConnection() == c)
      return; // No Change
    // Update the Pulse airway mode when this changes
    GetSettings().SetConnection(c);
    if (c == eMechanicalVentilator_Connection::Mask && m_data.GetIntubation() == eSwitch::Off)
    {
      m_data.SetAirwayMode(eAirwayMode::MechanicalVentilator);
      return;
    }
    else if (c == eMechanicalVentilator_Connection::Tube && m_data.GetIntubation() == eSwitch::On)
    {
      m_data.SetAirwayMode(eAirwayMode::MechanicalVentilator);
      return;
    }
    else if (c == eMechanicalVentilator_Connection::Mask && m_data.GetIntubation() == eSwitch::On)
      Error("Connection failed : Cannot apply mechanical ventilator mask if patient is intubated.");
    else if (c == eMechanicalVentilator_Connection::Tube && m_data.GetIntubation() == eSwitch::Off)
      Error("Connection failed : Cannot apply mechanical ventilator to tube if patient is not intubated.");
    // Make sure we are active to make sure we go back to free
    m_data.SetAirwayMode(eAirwayMode::Free);
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Removes the connection to the patient
  ///
  /// \details
  /// If the mask is on or the tube is connected, it is removed and the airway mode is set to free. The action is then removed from 
  /// the action manager.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::InvalidateConnection()
  {
    // Set airway mode to free
    m_data.SetAirwayMode(eAirwayMode::Free);
    // THEN invalidate
    GetSettings().SetConnection(eMechanicalVentilator_Connection::Off);
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes gas volumes and volume fractions supplied by the mechanical ventilator depending on the airway mode
  ///
  /// \details
  /// The gas volumes and volume fractions are initialized and updated based on the airway mode (mask, free, or tube)
  /// and the volume associated with each airway mode.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::SetConnection()
  {
    switch (m_data.GetAirwayMode())
    {
    case eAirwayMode::Free:
      GetSettings().SetConnection(eMechanicalVentilator_Connection::Off);
      break;
    case eAirwayMode::MechanicalVentilator:
      if (GetSettings().GetConnection() == eMechanicalVentilator_Connection::Mask)
      {
        if (m_data.GetIntubation() == eSwitch::On)// Somebody intubated while we had the mask on
        {
          Info("Mechanical Ventilator has been disconnected due to an intubation.");
          m_data.SetAirwayMode(eAirwayMode::Free);
          return;
        }

        //Keep the baseline resistance to ground = an open switch
        //Leaks handled later:L);
      }
      else if (GetSettings().GetConnection() == eMechanicalVentilator_Connection::Tube)
      {
        if (m_data.GetIntubation() == eSwitch::Off)// Somebody removed intubated while we were connected to it
        {
          Info("Mechanical Ventilator has been disconnected removal of intubation.");
          m_data.SetAirwayMode(eAirwayMode::Free);
          return;
        }

        //Keep the baseline resistance to ground = an open switch
        //Leaks handled later:L);
      }
      break;
    default:
      Fatal("Unhandled Airway Mode.");
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Mechanical ventilator preprocess function
  ///
  /// \details
  /// Applies all the settings and calculates the instantaneous driver value.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::PreProcess()
  {
    if (m_data.GetActions().GetEquipmentActions().HasMechanicalVentilatorConfiguration())
    {
      GetSettings().ProcessConfiguration(m_data.GetActions().GetEquipmentActions().GetMechanicalVentilatorConfiguration(), m_data.GetSubstances());
      m_data.GetActions().GetEquipmentActions().RemoveMechanicalVentilatorConfiguration();
      StateChange();
    }
    //Do nothing if the ventilator is off and not initialized
    if (GetSettings().GetConnection() == eMechanicalVentilator_Connection::Off)
    {
      m_CurrentBreathState = eBreathState::Exhale;
      m_CurrentPeriodTime_s = 0.0;
      m_CurrentInspiratoryVolume_L = 0.0;
      return;
    }

    SetConnection();
    CalculateInspiration();
    CalculatePause();
    CalculateExpiration();
    SetVentilatorDriver();
    SetResistances();

    m_CurrentPeriodTime_s += m_data.GetTimeStep_s();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Mechanical ventilator process function
  ///
  /// \details
  /// The current Pulse implementation has no specific process functionality for the mechanical ventilator.
  /// Mechanical ventilator processing is currently done in the Respiratory System with the combined circuit methodology.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::Process(bool /*solve_and_transport*/)
  {
    ComputeExposedModelParameters();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Mechanical ventilator postprocess function
  ///
  /// \details
  /// The substance volumes and the volume fractions are updated for all of the nodes in the mechanical ventilator
  /// circuit during post process. 
  /// The ventilator volumes are updated based on the previously calculated nodal analysis.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::PostProcess(bool /*solve_and_transport*/)
  {

  }

  void MechanicalVentilatorModel::ComputeExposedModelParameters()
  {

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Set the instantaneous driver pressure or flow on the circuit source.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::SetVentilatorDriver()
  {
    if (!std::isnan(m_DriverPressure_cmH2O) && !std::isnan(m_DriverFlow_L_Per_s))
      {
        /// \error Error: Ventilator driver pressure and flow both set, only one allowed. Using the pressure value.
        Error("Ventilator driver pressure and flow both set, only one allowed. Using the pressure value.");
      }
    else if (std::isnan(m_DriverPressure_cmH2O) && std::isnan(m_DriverFlow_L_Per_s))
    {
      /// \error Error: Ventilator driver pressure or flow must be set. Using a pressure of 0.
      Error("Ventilator driver pressure or flow must be set. Using a pressure of 0.");

      m_DriverPressure_cmH2O = 0.0;
    }

    bool stateChange = false;
    if (!std::isnan(m_DriverPressure_cmH2O))
    {
      if (m_EnvironmentToVentilator->HasNextFlowSource())
      {
        stateChange = true;
        m_EnvironmentToVentilator->RemoveFlowSource();
      }
      if (!m_EnvironmentToVentilator->HasNextPressureSource())
      {
        stateChange = true;
        m_EnvironmentToVentilator->GetPressureSourceBaseline().SetValue(0, PressureUnit::cmH2O);
      }
      m_EnvironmentToVentilator->GetNextPressureSource().SetValue(m_DriverPressure_cmH2O, PressureUnit::cmH2O);
    }
    else if (!std::isnan(m_DriverFlow_L_Per_s))
    {
      if (m_EnvironmentToVentilator->HasNextPressureSource())
      {
        stateChange = true;
        m_EnvironmentToVentilator->RemovePressureSource();
      }
      if (!m_EnvironmentToVentilator->HasNextFlowSource())
      {
        stateChange = true;
        m_EnvironmentToVentilator->GetFlowSourceBaseline().SetValue(0, VolumePerTimeUnit::L_Per_s);
      }
      m_EnvironmentToVentilator->GetNextFlowSource().SetValue(m_DriverFlow_L_Per_s, VolumePerTimeUnit::L_Per_s);
    }
    if (stateChange)
      m_data.GetCircuits().GetRespiratoryAndMechanicalVentilationCircuit().StateChange();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Determine the instantaneous driver pressure during inspiration.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::CalculateInspiration()
  {
    if (m_CurrentBreathState != eBreathState::Inhale)
    {
      m_CurrentInspiratoryVolume_L = 0.0;
      return;
    }

    m_CurrentInspiratoryVolume_L += m_YPieceToConnection->GetNextFlow(VolumePerTimeUnit::L_Per_s) * m_data.GetTimeStep_s();

    // Check trigger
    // Any combination of triggers can be used, but there must be at least one
    bool triggerDefined = false;
    if (GetSettings().HasExpirationCycleTime())
    {
      triggerDefined = true;
      if (m_CurrentPeriodTime_s >= GetSettings().GetExpirationCycleTime(TimeUnit::s))
      {
        CycleMode();
        return;
      }
    }
    
    if (GetSettings().HasExpirationCyclePressure())
    {
      triggerDefined = true;
      /// \error Fatal: Expiration pressure cycle is not yet supported.
      Fatal("Expiration pressure cycle is not yet supported.");
    }
    
    if (GetSettings().HasExpirationCycleVolume())
    {
      triggerDefined = true;
      if (m_CurrentInspiratoryVolume_L >= GetSettings().GetExpirationCycleVolume(VolumeUnit::L))
      {
        m_CurrentInspiratoryVolume_L = 0.0;
        CycleMode();
        return;
      }
    }
    
    if (GetSettings().HasExpirationCycleFlow())
    {
      triggerDefined = true;
      if (m_YPieceToConnection->GetNextFlow(VolumePerTimeUnit::L_Per_s) <= GetSettings().GetExpirationCycleFlow(VolumePerTimeUnit::L_Per_s) &&
        m_CurrentPeriodTime_s > 0.0) //Check if we just cycled the mode
      {
        CycleMode();
        return;
      }
    }
    
    if(!triggerDefined)
    {
      /// \error Fatal: No expiration cycle defined.
      Fatal("No expiration cycle defined.");
    }

    // Check limit
    if (GetSettings().HasInspirationLimitPressure() ||
        GetSettings().HasInspirationLimitFlow() ||
        GetSettings().HasInspirationLimitVolume())
    {
      /// \error Fatal: Limits are not yet supported.
      Fatal("Limits are not yet supported.");
    }

    // Check waveform
    if (GetSettings().GetInspirationWaveform() != eMechanicalVentilator_DriverWaveform::Square &&
      !GetSettings().HasInspirationWaveformPeriod())
    {
      /// \error Fatal: Non-square waveforms require a period.
      Fatal("Non-square waveforms require a period.");
    }

    // Apply waveform
    if (GetSettings().GetInspirationWaveform() == eMechanicalVentilator_DriverWaveform::Square ||
      (GetSettings().HasInspirationWaveformPeriod() && m_CurrentPeriodTime_s > GetSettings().GetInspirationWaveformPeriod(TimeUnit::s)))
    {
      if (GetSettings().HasPeakInspiratoryPressure())
      {
        m_DriverPressure_cmH2O = GetSettings().GetPeakInspiratoryPressure(PressureUnit::cmH2O);
        m_DriverFlow_L_Per_s = SEScalar::dNaN();
      }
      else if (GetSettings().HasInspirationTargetFlow())
      {
        m_DriverFlow_L_Per_s = GetSettings().GetInspirationTargetFlow(VolumePerTimeUnit::L_Per_s);
        m_DriverPressure_cmH2O = SEScalar::dNaN();
      }
      else
      {
        /// \error Fatal: Inspiration mode not yet supported.
        Fatal("Inspiration mode not yet supported.");
      }
    }
    else if (GetSettings().GetInspirationWaveform() == eMechanicalVentilator_DriverWaveform::Ramp)
    {
      if (GetSettings().HasPeakInspiratoryPressure())
      {
        double initialPressure_cmH2O = 0.0;
        if (GetSettings().HasPositiveEndExpiredPressure())
        {
          initialPressure_cmH2O = GetSettings().GetPositiveEndExpiredPressure(PressureUnit::cmH2O);
        }

        double finalPressure_cmH2O = GetSettings().GetPeakInspiratoryPressure(PressureUnit::cmH2O);
        m_DriverPressure_cmH2O = initialPressure_cmH2O + (finalPressure_cmH2O - initialPressure_cmH2O) * m_CurrentPeriodTime_s / GetSettings().GetInspirationWaveformPeriod(TimeUnit::s);
        m_DriverFlow_L_Per_s = SEScalar::dNaN();
      }
      else if (GetSettings().HasInspirationTargetFlow())
      {
        double initialFlow_L_Per_s = 0.0;
        double finalFlow_L_Per_s = GetSettings().GetInspirationTargetFlow(VolumePerTimeUnit::L_Per_s);
        m_DriverFlow_L_Per_s = initialFlow_L_Per_s + (finalFlow_L_Per_s - initialFlow_L_Per_s) * m_CurrentPeriodTime_s / GetSettings().GetInspirationWaveformPeriod(TimeUnit::s);
        m_DriverPressure_cmH2O = SEScalar::dNaN();
      }
      else
      {
        /// \error Fatal: Inspiration mode not yet supported.
        Fatal("Inspiration mode not yet supported.");
      }
    }
    else
    {
      /// \error Fatal: Waveform type not yet supported.
      Fatal("Waveform type not yet supported.");
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Determine the instantaneous driver pressure during pause.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::CalculatePause()
  {
    if (m_CurrentBreathState != eBreathState::Pause)
    {
      return;
    }

    if (GetSettings().HasInspirationPauseTime())
    {
      if (m_CurrentPeriodTime_s < GetSettings().GetInspirationPauseTime(TimeUnit::s))
      {
        // Hold this pressure
        m_DriverPressure_cmH2O = m_VentilatorNode->GetNextPressure(PressureUnit::cmH2O);
        m_DriverFlow_L_Per_s = SEScalar::dNaN();
      }
      else
      {
        CycleMode();
      }
    }
    else
    {
      CycleMode();
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Determine the instantaneous driver pressure during expiration.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::CalculateExpiration()
  {
    if (m_CurrentBreathState != eBreathState::Exhale)
    {
      return;
    }

    // Check trigger
    // Any combination of triggers can be used, but there must be at least one
    bool triggerDefined = false;
    if (GetSettings().HasInspirationMachineTriggerTime())
    {
      triggerDefined = true;
      if (m_CurrentPeriodTime_s >= GetSettings().GetInspirationMachineTriggerTime(TimeUnit::s))
      {
        CycleMode();
        return;
      }
    }
    
    if (GetSettings().HasInspirationPatientTriggerPressure())
    {
      triggerDefined = true;
      double relativePressure_cmH2O = m_ConnectionNode->GetNextPressure(PressureUnit::cmH2O) - m_AmbientNode->GetNextPressure(PressureUnit::cmH2O);
      if (GetSettings().HasPositiveEndExpiredPressure())
      {
        relativePressure_cmH2O -= GetSettings().GetPositiveEndExpiredPressure(PressureUnit::cmH2O);
      }
      if (relativePressure_cmH2O <= GetSettings().GetInspirationPatientTriggerPressure(PressureUnit::cmH2O) &&
        m_CurrentPeriodTime_s > 0.0) //Check if we just cycled the mode
      {
        CycleMode();
        return;
      }
    }
    
    if (GetSettings().HasInspirationPatientTriggerFlow())
    {
      triggerDefined = true;
      if (m_YPieceToConnection->GetNextFlow(VolumePerTimeUnit::L_Per_s) >= GetSettings().GetInspirationPatientTriggerFlow(VolumePerTimeUnit::L_Per_s) &&
        m_CurrentPeriodTime_s > 0.0) //Check if we just cycled the mode
      {
        CycleMode();
        return;
      }
    }
    
    if(!triggerDefined)
    {
      /// \error Fatal: No inspiration trigger defined.
      Fatal("No inspiration trigger defined.");
    }

    // Apply waveform
    if (GetSettings().GetExpirationWaveform() == eMechanicalVentilator_DriverWaveform::Square)
    {
      if (GetSettings().HasPositiveEndExpiredPressure())
      {
        m_DriverPressure_cmH2O = GetSettings().GetPositiveEndExpiredPressure(PressureUnit::cmH2O);
        m_DriverFlow_L_Per_s = SEScalar::dNaN();
      }
      else if (GetSettings().HasFunctionalResidualCapacity())
      {
        /// \error Fatal: Functional residual capacity expiratory baseline not yet supported.
        Fatal("Functional residual capacity expiratory baseline not yet supported.");
      }
      else
      {
        m_DriverPressure_cmH2O = 0.0;
        m_DriverFlow_L_Per_s = SEScalar::dNaN();
      }
    }
    else
    {
      /// \error Fatal: Non-square waveforms are not yet supported.
      Fatal("Non-square waveforms are not yet supported.");
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Go to next breath mode based on current mode.
  ///
  /// \details
  /// Inhale to pause, pause to exhale, or exhale to inhale.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::CycleMode()
  {
    m_CurrentPeriodTime_s = 0.0;

    if (m_CurrentBreathState == eBreathState::Inhale)
    {
      m_CurrentBreathState = eBreathState::Pause;
    }
    else if (m_CurrentBreathState == eBreathState::Pause)
    {
      m_CurrentBreathState = eBreathState::Exhale;
    }
    else if (m_CurrentBreathState == eBreathState::Exhale)
    {
      m_CurrentBreathState = eBreathState::Inhale;
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Set ventilator circuit resistances.
  ///
  /// \details
  /// If no values are specified, defaults will be used.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::SetResistances()
  {
    if (GetSettings().HasExpirationTubeResistance())
    {
      m_VentilatorToExpiratoryValve->GetNextResistance().Set(GetSettings().GetExpirationTubeResistance());
    }

    if (GetSettings().HasInspirationTubeResistance())
    {
      m_VentilatorToInspiratoryValve->GetNextResistance().Set(GetSettings().GetInspirationTubeResistance());
    }

    if (GetSettings().HasExpirationValveResistance())
    {
      m_ExpiratoryLimbToYPiece->GetNextResistance().Set(GetSettings().GetExpirationValveResistance());
    }

    if (GetSettings().HasInspirationValveResistance())
    {
      m_InspiratoryLimbToYPiece->GetNextResistance().Set(GetSettings().GetInspirationValveResistance());
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Set ventialtor node/compartment volumes.
  ///
  /// \details
  /// If no values are specified, defaults will be used.
  //--------------------------------------------------------------------------------------------------
  void MechanicalVentilatorModel::SetVolumes()
  {
    if (GetSettings().HasExpirationLimbVolume())
    {
      m_ExpiratoryLimbNode->GetNextVolume().Set(GetSettings().GetExpirationLimbVolume());
    }
    else
    {
      //Volume doesn't reset to baseline like path elements
      m_ExpiratoryLimbNode->GetNextVolume().Set(m_ExpiratoryLimbNode->GetVolumeBaseline());
    }

    if (GetSettings().HasExpirationValveVolume())
    {
      m_ExpiratoryValveNode->GetNextVolume().Set(GetSettings().GetExpirationValveVolume());
    }
    else
    {
      //Volume doesn't reset to baseline like path elements
      m_ExpiratoryValveNode->GetNextVolume().Set(m_ExpiratoryValveNode->GetVolumeBaseline());
    }

    if (GetSettings().HasInspirationLimbVolume())
    {
      m_InspiratoryLimbNode->GetNextVolume().Set(GetSettings().GetInspirationLimbVolume());
    }
    else
    {
      //Volume doesn't reset to baseline like path elements
      m_InspiratoryLimbNode->GetNextVolume().Set(m_InspiratoryLimbNode->GetVolumeBaseline());
    }

    if (GetSettings().HasInspirationValveVolume())
    {
      m_InspiratoryValveNode->GetNextVolume().Set(GetSettings().GetInspirationValveVolume());
    }
    else
    {
      //Volume doesn't reset to baseline like path elements
      m_InspiratoryValveNode->GetNextVolume().Set(m_InspiratoryValveNode->GetVolumeBaseline());
    }

    if (GetSettings().HasYPieceVolume())
    {
      m_YPieceNode->GetNextVolume().Set(GetSettings().GetYPieceVolume());
    }
    else
    {
      //Volume doesn't reset to baseline like path elements
      m_YPieceNode->GetNextVolume().Set(m_YPieceNode->GetVolumeBaseline());
    }

    if (GetSettings().HasConnectionVolume())
    {
      m_ConnectionNode->GetNextVolume().Set(GetSettings().GetConnectionVolume());
    }
    else
    {
      //Volume doesn't reset to baseline like path elements
      m_ConnectionNode->GetNextVolume().Set(m_ConnectionNode->GetVolumeBaseline());
    }
  }
}
