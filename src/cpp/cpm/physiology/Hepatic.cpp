/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "physiology/Hepatic.h"

Hepatic::Hepatic(PulseData& data) : PulseHepaticSystem(data)
{
  Clear();
}

Hepatic::~Hepatic()
{
  Clear();
}

void Hepatic::Clear()
{
  SEHepaticSystem::Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
///
/// \details
/// For stabilization only!
/// Called AFTER Setup when stabilizing a new patient
//--------------------------------------------------------------------------------------------------
void Hepatic::Initialize()
{
  PulseSystem::Initialize();

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
void Hepatic::SetUp()
{
 
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines if the engine is stable.
///
/// \details
/// When the engine is stable, the CDM makes this call to update the member variable.
//--------------------------------------------------------------------------------------------------
void Hepatic::AtSteadyState()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Preprocess function
///
/// \details
//--------------------------------------------------------------------------------------------------
void Hepatic::PreProcess()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Process function
///
/// \details
//--------------------------------------------------------------------------------------------------
void Hepatic::Process(bool solve_and_transport)
{
  ComputeExposedModelParameters();
}
void Hepatic::ComputeExposedModelParameters()
{

}

//--------------------------------------------------------------------------------------------------
/// \brief
/// PostProcess function
///
/// \details
//--------------------------------------------------------------------------------------------------
void Hepatic::PostProcess(bool solve_and_transport)
{

}