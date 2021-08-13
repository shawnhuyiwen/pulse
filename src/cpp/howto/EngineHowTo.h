/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

// Note that this project is set with the following Additional Include Paths: ../include;../include/cdm;../include/cdm/bind
// This will build an executable that is intended to execute a how-to method

#include "PhysiologyEngine.h"

// The following how-to functions are defined in their own file
void HowToEngineUse();
void HowToCreateAPatient();
void HowToSerialize();
void HowToPulseEnginePool();

void HowToAirwayObstruction();
void HowToAnesthesiaMachine();
void HowToAsthmaAttack();
void HowToBagValveMask();
void HowToBolusDrug();
void HowToBrainInjury();
void HowToCombatMultitrauma();
void HowToConsumeNutrients();
void HowToCOPD();
void HowToCOVID19Ventilated();
void HowToCPR();
void HowToEnvironmentChange();
void HowToExercise();
void HowToHemorrhage();
void HowToLobarPneumonia();
void HowToMechanicalVentilation();
void HowToMechanicalVentilator();
void HowToPulmonaryFibrosis();
void HowToPulmonaryFunctionTest();
void HowToRespiratoryMechanics();
void HowToSmoke();
void HowToTensionPneumothorax();

void HowToConcurrentEngines();
void HowToRunScenario();
void HowToDynamicHemorrhage();

void HowToSandbox();
void HowToTestSystemCapability();

void HowToVentilationMechanics();

class SEDataRequest;

/// This mehod will advance time and write each data request value to the specified engine csv
bool AdvanceAndTrackTime_s(double time_s, PhysiologyEngine& engine);
