/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/PulseConfiguration.h"
#include "engine/common/controller/CircuitManager.h"
#include "engine/common/controller/CompartmentManager.h"
#include "engine/common/controller/SubstanceManager.h"
#include "engine/common/system/equipment/ECMOModel.h"

#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"
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
#include "cdm/properties/SERunningAverage.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse
{
  /*
  ========================
    Constructors
  ========================
  */
  ECMOModel::ECMOModel(Data& data) : ECMOSystem(data.GetLogger()), Model(data)
  {
    Clear();
  }

  ECMOModel::~ECMOModel()
  {
    Clear();
  }

  void ECMOModel::Clear()
  {
    ECMOSystem::Clear();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes system properties to valid homeostatic values.
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::Initialize()
  {
    Model::Initialize();

    StateChange();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes parameters for the ECMO class
  ///
  /// \details
  /// Initializes member variables and system level values on the common data model.
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::SetUp()
  {
  }

  void ECMOModel::StateChange()
  {
  }


  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// ECMO preprocess function
  ///
  /// \details
  /// Applies all the settings and calculates the instantaneous driver value.
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::PreProcess()
  {
    if (m_data.GetActions().GetEquipmentActions().HasECMOConfiguration())
    {
      GetSettings().ProcessConfiguration(m_data.GetActions().GetEquipmentActions().GetECMOConfiguration(), m_data.GetSubstances());
      m_data.GetActions().GetEquipmentActions().RemoveECMOConfiguration();
      StateChange();
    }

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// ECMO process function
  ///
  /// \details
  /// TBD
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::Process(bool /*solve_and_transport*/)
  {
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// ECMO postprocess function
  ///
  /// \details
  /// TBD
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::PostProcess(bool /*solve_and_transport*/)
  {
  }

  void ECMOModel::ComputeExposedModelParameters()
  {
  }

END_NAMESPACE
