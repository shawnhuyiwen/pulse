/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#define _USE_MATH_DEFINES


#include "EngineTest.h"
#include "controller/Engine.h"
#include "controller/Controller.h"
#include "controller/Circuits.h"
#include "controller/Compartments.h"
#include "controller/Substances.h"
#include "physiology/Saturation.h"
#include "PulseConfiguration.h"
#include "patient/SEPatient.h"
#include "substance/SESubstanceFraction.h"
#include "substance/SESubstanceTransport.h"
#include "circuit/fluid/SEFluidCircuit.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "compartment/fluid/SEGasCompartmentGraph.h"
#include "compartment/fluid/SEFluidCompartmentLink.h"
#include "system/environment/SEEnvironmentalConditions.h"
#include "properties/SEScalarVolumePerPressure.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "properties/SEScalar0To1.h"
#include "utils/DataTrack.h"
#include "utils/TimingProfile.h"
#include <math.h>


//--------------------------------------------------------------------------------------------------
/// \brief
/// Runs the test for the circuit math in the machanical ventilator
///
/// \param  sTestDirectory  Contains the path to the directory where the output file is stored
///
/// \details
/// Tests the mechanical ventilator circuit mechanics by reading in a text file that contains the driver
/// and variable values for the circuit elements.The outputs are the resultant flows and pressures
/// on the circuit nodes and paths. These are then stored in a file in sTestDirectory
//--------------------------------------------------------------------------------------------------
void PulseEngineTest::MechanicalVentilatorCircuitAndTransportTest(RespiratoryConfiguration config, const std::string&  sTestDirectory)
{
  TimingProfile tmr;
  tmr.Start("Test");
  //Output files
  DataTrack outTrkCircuit;
  DataTrack outTrkGraph;
  std::ofstream fileCircuit;
  std::ofstream fileGraph;
  
  PulseEngine pe;
  PulseController& pc = pe.GetController();
  pc.GetLogger()->SetLogFile(sTestDirectory + "/MechanicalVentilatorCircuitAndTransportTest.log");
  SEPatient patient(pc.GetLogger());
  patient.SerializeFromFile("./patients/StandardMale.json");
  pc.SetupPatient(patient);
  pc.GetSubstances().LoadSubstanceDirectory("./");
  pc.GetSaturationCalculator().Setup();
  pc.m_Config->Initialize("./", &pc.GetSubstances());
  pc.m_Config->EnableRenal(eSwitch::Off);
  pc.m_Config->EnableTissue(eSwitch::Off);
  pc.CreateCircuitsAndCompartments();
  SEEnvironmentalConditions env(pc.GetLogger());
  env.SerializeFromFile("./environments/Standard.json", pc.GetSubstances());
  SEGasCompartment* cEnv = pc.GetCompartments().GetGasCompartment(pulse::EnvironmentCompartment::Ambient);
  for (SESubstanceFraction* subFrac : env.GetAmbientGases())
  {
    pc.GetSubstances().AddActiveSubstance(subFrac->GetSubstance());
    cEnv->GetSubstanceQuantity(subFrac->GetSubstance())->GetVolumeFraction().Set(subFrac->GetFractionAmount());
  }
  pc.GetSubstances().InitializeGasCompartments();

  //set environment pressure
  SEFluidCircuitNode* nEnvironment = pc.GetCircuits().GetFluidNode(pulse::EnvironmentNode::Ambient);
  nEnvironment->GetPressure().Set(env.GetAtmosphericPressure());
  nEnvironment->GetNextPressure().Set(env.GetAtmosphericPressure());
  
  //Grab the mechanical ventilator circuits and graphs
  SEFluidCircuit* mvCircuit = nullptr;
  SEGasCompartmentGraph* mvGraph = nullptr;
  std::string sCircuitFileName;
  std::string sTransportFileName;
  if (config == MechanicalVentilatorSolo)
  {
    mvCircuit = &pc.GetCircuits().GetMechanicalVentilatorCircuit();
    mvGraph = &pc.GetCompartments().GetMechanicalVentilatorGraph();
    sCircuitFileName = "/MechanicalVentilatorCircuitOutput.csv";
    sTransportFileName = "/MechanicalVentilatorTransportOutput.csv";

    //Allow things to flow to ground, since the respiratory circuit isn't here
    //This approximates the total respiratory system resistance
    SEFluidCircuitPath* MechanicalVentilatorConnectionToEnvironment = mvCircuit->GetPath(pulse::MechanicalVentilatorPath::ConnectionToEnvironment);
    //Use a pressure source here instead of a resistance because the valves cause unsolvable matrices
    MechanicalVentilatorConnectionToEnvironment->GetPressureSourceBaseline().SetValue(0.1, PressureUnit::cmH2O);
    MechanicalVentilatorConnectionToEnvironment->GetNextPressureSource().SetValue(0.1, PressureUnit::cmH2O);
  }
  else if (config == RespiratoryWithMechanicalVentilator)
  {
    pc.GetSubstances().InitializeGasCompartments();

    mvCircuit = &pc.GetCircuits().GetRespiratoryAndMechanicalVentilatorCircuit();
    mvGraph = &pc.GetCompartments().GetRespiratoryAndMechanicalVentilatorGraph();
    sCircuitFileName = "/RespiratoryAndMechanicalVentilatorCircuitOutput.csv";
    sTransportFileName = "/RespiratoryAndMechanicalVentilatorTransportOutput.csv";

    //Precharge the stomach to prevent negative volume
    mvCircuit->GetNode(pulse::RespiratoryNode::Stomach)->GetNextPressure().Set(env.GetAtmosphericPressure());
    mvCircuit->GetNode(pulse::RespiratoryNode::Stomach)->GetPressure().Set(env.GetAtmosphericPressure());

    SEFluidCircuitPath *driverPath = mvCircuit->GetPath(pulse::RespiratoryPath::EnvironmentToRespiratoryMuscle);
    driverPath->GetPressureSourceBaseline().SetValue(-12.0, PressureUnit::cmH2O);
    driverPath->GetNextPressureSource().SetValue(-12.0, PressureUnit::cmH2O);
  }
  else
  {
    return;
  }
  
  // Pull Pressure Source Paths
  SEFluidCircuitPath* EnvironmentToVentilatorPath = mvCircuit->GetPath(pulse::MechanicalVentilatorPath::EnvironmentToVentilator);

  SEGasTransporter txpt(VolumePerTimeUnit::L_Per_s, VolumeUnit::L, VolumeUnit::L, pc.GetLogger());
  SEFluidCircuitCalculator calc(VolumePerPressureUnit::L_Per_cmH2O, VolumePerTimeUnit::L_Per_s, PressureTimeSquaredPerVolumeUnit::cmH2O_s2_Per_L, PressureUnit::cmH2O, VolumeUnit::L, PressureTimePerVolumeUnit::cmH2O_s_Per_L, pc.GetLogger());

  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 90.0;
  double runTime_min = 10.0;
  //Drive waveform parameters
  double period = 5.0;
  double alpha = (2 * M_PI) / (period);
  double driverPressure_cmH2O = 0.0;
  double amplitude_cmH2O = 6.0;
  double yOffset = 6.0;

  for (unsigned int i = 0; i < runTime_min * 60.0 / deltaT_s; i++)
  {
    //PreProcess
    //Set the driver pressure
    driverPressure_cmH2O = yOffset + amplitude_cmH2O * sin(alpha * time);   //compute new pressure
    EnvironmentToVentilatorPath->GetNextPressureSource().SetValue(driverPressure_cmH2O, PressureUnit::cmH2O);

    //Process - Execute the circuit
    calc.Process(*mvCircuit, deltaT_s);
    //Execute the substance transport function
    txpt.Transport(*mvGraph, deltaT_s);
    //convert 'Next' values to current
    calc.PostProcess(*mvCircuit);

    outTrkCircuit.Track(time, *mvCircuit);
    outTrkGraph.Track(time, *mvGraph);
    time += deltaT_s;

    if (i == 0)
    {
      outTrkCircuit.CreateFile(std::string(sTestDirectory + sCircuitFileName).c_str(), fileCircuit);
      outTrkGraph.CreateFile(std::string(sTestDirectory + sTransportFileName).c_str(), fileGraph);
    }

    outTrkCircuit.StreamTrackToFile(fileCircuit);
    outTrkGraph.StreamTrackToFile(fileGraph);
  }
  fileCircuit.close();
  fileGraph.close();
  std::stringstream ss;
  ss << "It took " << tmr.GetElapsedTime_s("Test") << "s to run";
  pc.GetLogger()->Info(ss.str(), "MechanicalVentilatorCircuitAndTransportTest");
}

void PulseEngineTest::MechanicalVentilatorCircuitAndTransportTest(const std::string& sTestDirectory)
{
  MechanicalVentilatorCircuitAndTransportTest(MechanicalVentilatorSolo, sTestDirectory);
}

void PulseEngineTest::RespiratoryWithMechanicalVentilatorCircuitAndTransportTest(const std::string& sTestDirectory)
{
  MechanicalVentilatorCircuitAndTransportTest(RespiratoryWithMechanicalVentilator, sTestDirectory);
}
