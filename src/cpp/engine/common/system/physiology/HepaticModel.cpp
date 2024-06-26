﻿/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/common/system/physiology/HepaticModel.h"

namespace pulse
{
  HepaticModel::HepaticModel(Data& data) : HepaticSystem(data.GetLogger()), Model(data)
  {
    Clear();
  }

  HepaticModel::~HepaticModel()
  {
    Clear();
  }

  void HepaticModel::Clear()
  {
    HepaticSystem::Clear();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes system properties to valid homeostatic values.
  ///
  /// \details
  /// For stabilization only!
  /// Called AFTER Setup when stabilizing a new patient
  //--------------------------------------------------------------------------------------------------
  void HepaticModel::Initialize()
  {
    Model::Initialize();

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes parameters for the Hepatic Class
  ///
  /// \details
  /// Called during both State loading and Patient Stabilization
  /// Pull and setup up our data (can be from other systems)
  /// Initialize will be called after this and can overwrite any of this data (only if stabilizing)
  //--------------------------------------------------------------------------------------------------
  void HepaticModel::SetUp()
  {

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Determines if the engine is stable.
  ///
  /// \details
  /// When the engine is stable, the CDM makes this call to update the member variable.
  //--------------------------------------------------------------------------------------------------
  void HepaticModel::AtSteadyState()
  {

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Preprocess function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void HepaticModel::PreProcess()
  {

  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Process function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void HepaticModel::Process(bool /*solve_and_transport*/)
  {
    ComputeExposedModelParameters();
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// PostProcess function
  ///
  /// \details
  //--------------------------------------------------------------------------------------------------
  void HepaticModel::PostProcess(bool /*solve_and_transport*/)
  {

  }

  void HepaticModel::ComputeExposedModelParameters()
  {

  }
END_NAMESPACE
