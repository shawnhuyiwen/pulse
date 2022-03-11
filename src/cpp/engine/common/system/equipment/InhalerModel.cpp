/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/PulseConfiguration.h"
#include "engine/common/controller/CircuitManager.h"
#include "engine/common/controller/CompartmentManager.h"
#include "engine/common/controller/SubstanceManager.h"
#include "engine/common/system/equipment/InhalerModel.h"

#include "cdm/circuit/SECircuit.h"
#include "cdm/circuit/SECircuitPath.h"
#include "cdm/circuit/SECircuitNode.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEEquipmentActionCollection.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/patient/actions/SEConsciousRespiration.h"
#include "cdm/patient/actions/SEUseInhaler.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SEScalarLength.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalar0To1.h"

namespace pulse
{
  /*
  ========================
  Constructors
  ========================
  */

  InhalerModel::InhalerModel(Data& data) : InhalerSystem(data.GetLogger()), Model(data)
  {
    Clear();
  }

  InhalerModel::~InhalerModel()
  {
    Clear();
  }

  void InhalerModel::Clear()
  {
    InhalerSystem::Clear();
    m_Mouthpiece = nullptr;
    m_AerosolMouthpiece = nullptr;
    m_AmbientEnv = nullptr;
    m_InhalerDrug = nullptr;
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes system properties to valid homeostatic values.
  //--------------------------------------------------------------------------------------------------
  void InhalerModel::Initialize()
  {
    Model::Initialize();
    m_InhalerDrug = nullptr;
  }

  void InhalerModel::SetUp()
  {
    m_AmbientEnv = m_data.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
    m_Mouthpiece = m_data.GetCompartments().GetGasCompartment(pulse::InhalerCompartment::Mouthpiece);
    m_AerosolMouthpiece = m_data.GetCompartments().GetLiquidCompartment(pulse::InhalerCompartment::Mouthpiece);
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Inhaler Action Preprocessor
  ///
  /// \param  None
  ///
  /// \return void
  ///
  /// \details
  /// This method:
  ///  <UL>
  ///   <LI>Determines when to activate the inhaler given an inhaler command.</LI>
  ///   <LI>Starts and stops the inhaler.</LI>
  ///   <LI>Models deposition of inhaled substance in the orthopharynx</LI>
  ///  </UL>
  ///
  //--------------------------------------------------------------------------------------------------
  void InhalerModel::PreProcess()
  {
    if (m_data.GetActions().GetEquipmentActions().HasInhalerConfiguration())
    {
      eSwitch state = GetState();
      SEInhalerConfiguration& config = m_data.GetActions().GetEquipmentActions().GetInhalerConfiguration();
      ProcessConfiguration(config, m_data.GetSubstances());
      m_data.GetActions().GetEquipmentActions().RemoveInhalerConfiguration();
      if (state != m_State)
      {
        m_State = state;
        Warning("Pulse does not allow you to change inhaler state via the configuration, please use a Conscious Respiration Action. Ignoring the configuration state.");
      }
    }
    if (m_data.GetActions().GetPatientActions().HasConsciousRespiration())
    {
      SEConsciousRespiration& cr = m_data.GetActions().GetPatientActions().GetConsciousRespiration();
      SEConsciousRespirationCommand* cmd = cr.GetActiveCommand();
      SEUseInhaler* ui = dynamic_cast<SEUseInhaler*>(cmd);
      if (ui != nullptr)
      {
        Administer();
        cr.RemoveActiveCommand();
      }
    }

    // ### HANDLE INHALER-BASED UPDATES
    if (m_State == eSwitch::On)
    {
      //  Check to see if there is a substantial mass of substance on the inhaler node.
      //  If not, we'll disconnect the inhaler.    
      double dCInhalerSubstanceMass_ug = m_InhalerDrug->GetMass(MassUnit::ug);
      if (SEScalar::IsZero(dCInhalerSubstanceMass_ug, 1e-7))
      {
        Info("Inhaler removed!");
        m_InhalerDrug = nullptr;
        m_State = eSwitch::Off;
        m_data.SetAirwayMode(eAirwayMode::Free);
      }
    }
  }

  void InhalerModel::StateChange()
  {
    SEInhaler::StateChange();
    if (m_State == eSwitch::On)
    {
      if (m_Substance == nullptr)
      {
        Fatal("State is on, but without a substance");
      }
      if (m_InhalerDrug == nullptr)
      {
        m_InhalerDrug = m_AerosolMouthpiece->GetSubstanceQuantity(*m_Substance);
      }
    }
  }


  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Administer Substance Using Inhaler 
  ///
  /// \param  None
  ///
  /// \return void
  ///
  /// \details
  /// This method initializes substance values in the inhaler when actuated.  It is called once
  /// per actuation. 
  //--------------------------------------------------------------------------------------------------
  void InhalerModel::Administer()
  {
    // Check to see if the inhaler is already on. We should not run this method unless the
    //  inhaler is currently off and about to be activated.
    if (m_State == eSwitch::On)
    {
      /// \error: Already processing a Substance Inhalation, ignoring this command.
      Error("Already processing a Substance Inhalation, ignoring this command");
      return;
    }

    // Alert the user that the inhaler is actuated
    Info("Inhaler actuated!");
    m_State = eSwitch::On;
    m_data.SetAirwayMode(eAirwayMode::Inhaler);

    // Initialize pressure in the inhaler node to ambient  
    double dAmbientPressure = m_AmbientEnv->GetPressure(PressureUnit::cmH2O);
    m_Mouthpiece->GetPressure().SetValue(dAmbientPressure, PressureUnit::cmH2O);

    // Copy the environment substance data to the mouth piece
    m_Mouthpiece->ZeroSubstanceQuantities();
    for (size_t i = 0; i < m_AmbientEnv->GetSubstanceQuantities().size(); i++)
    {
      m_Mouthpiece->GetSubstanceQuantities()[i]->GetVolumeFraction().Set(m_AmbientEnv->GetSubstanceQuantities()[i]->GetVolumeFraction());
    }
    m_Mouthpiece->Balance(BalanceGasBy::VolumeFraction);

    // Add the dose substance to the list of active substances to be tracked in Pulse
    m_data.GetSubstances().AddActiveSubstance(*m_Substance);

    // Get the inhaler node volume
    double dVolume_L = m_Mouthpiece->GetVolume(VolumeUnit::L);

    // Check to see if there is a spacer.  If so, change the inhaler volume. 
    if (m_SpacerVolume != nullptr)
    {
      double dSpacerVolume_L = m_SpacerVolume->GetValue(VolumeUnit::L);
      dVolume_L += dSpacerVolume_L;
      m_Mouthpiece->GetVolume().SetValue(dVolume_L, VolumeUnit::L);
    }

    // Get dose properties we need
    double dDoseDensity_ug_Per_L;
    if (m_Substance->HasDensity())
    {
      dDoseDensity_ug_Per_L = m_Substance->GetDensity(MassPerVolumeUnit::ug_Per_L);  // Density of dose in micrograms per liter
    }
    else
    {
      /// \error Fatal: Inhaled substance does not have a density value.
      Fatal("Inhaled substance does not have a density value.");
    }

    // Dosage amount in micrograms
    double dDoseMass_ug = m_MeteredDose->GetValue(MassUnit::ug);

    // Reduce the total dose to account for nozzle losses
    double dNozzleLossFraction = m_NozzleLoss->GetValue();

    dDoseMass_ug *= (1 - dNozzleLossFraction);

    // Loop all the substances on the inhaler node
    for (SELiquidSubstanceQuantity* subQ : m_AerosolMouthpiece->GetSubstanceQuantities())
    {
      if (!subQ->HasMass())
        continue;

      if (&subQ->GetSubstance() == m_Substance)
      {
        // This is the metered dose of the substance we're adding to the inhaler      
        m_InhalerDrug = subQ;
        m_InhalerDrug->GetMass().SetValue(dDoseMass_ug, MassUnit::ug);
      }
    }
    m_InhalerDrug->Balance(BalanceLiquidBy::Mass);

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Inhaler system process function
  ///
  /// \details
  /// Currently not used.
  //--------------------------------------------------------------------------------------------------
  void InhalerModel::Process(bool /*solve_and_transport*/)
  {
    ComputeExposedModelParameters();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Inhaler system postprocess function
  ///
  /// \details
  /// Currently not used.
  //--------------------------------------------------------------------------------------------------
  void InhalerModel::PostProcess(bool /*solve_and_transport*/)
  {

  }

  void InhalerModel::ComputeExposedModelParameters()
  {

  }
END_NAMESPACE
