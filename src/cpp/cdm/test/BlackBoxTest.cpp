/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "CommonDataModelTest.h"
#include "blackbox/SEBlackBoxManager.h"
#include "blackbox/fluid/SEFluidBlackBox.h"

#include "utils/testing/SETestReport.h"
#include "utils/testing/SETestCase.h"
#include "utils/testing/SETestSuite.h"
#include "circuit/SECircuitManager.h"
#include "circuit/fluid/SEFluidCircuitCalculator.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarPressureTimePerVolume.h"
#include "properties/SEScalarTime.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarVolumePerTime.h"
#include "utils/DataTrack.h"
#include "utils/TimingProfile.h"

void CommonDataModelTest::BasicBlackBoxComparisonTest(const std::string& sOutputDirectory)
{

  //jbw - Add transport

  std::cout << "BlackBoxComparisonCircuitTest\n";

  m_Logger->SetLogFile(sOutputDirectory + "/BasicBlackBoxComparisonTest.log");
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  double currentTime_s = 0.0;
  DataTrack trk1;
  DataTrack trk2;
  SEFluidCircuitCalculator fluidCalculator(m_Logger);

  //Test Circuit
  SEFluidCircuit* fluidCircuit = &m_Circuits->CreateFluidCircuit("Fluid");
  //-----------------------------------------------------------
  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit->CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit->CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit->CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit->CreateNode("Node4");
  //Reference node
  //This works like ground - I added code to allow it to have a value other than 0
  fluidCircuit->AddNode(Node4); Node4.SetAsReferenceNode();
  Node4.GetNextPressure().SetValue(0, PressureUnit::Pa);
  //-----------------------------------------------------------
  //Paths
  SEFluidCircuitPath& Path2 = fluidCircuit->CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit->CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit->CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path5 = fluidCircuit->CreatePath(Node2, Node4, "Path5");
  Path5.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit->CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);
  fluidCircuit->StateChange();

  bool serialized = false;
  double sample = 0;
  while (currentTime_s < 100)
  {
    //PreProcess - to be done by the systems
    FluidPreProcess(*fluidCircuit, currentTime_s);
    //Process
    fluidCalculator.Process(*fluidCircuit, timeStep_s);
    //PostProcess
    fluidCalculator.PostProcess(*fluidCircuit);
    currentTime_s += timeStep_s;
    sample += timeStep_s;
    if (sample > 0.1)// every 0.1 seconds, track state of circuit
    {
      sample = 0;
      trk1.Track(currentTime_s, *fluidCircuit);
    }
    if (!serialized && currentTime_s > 80)
    {
      serialized = true;
      std::string jsonDir = sOutputDirectory + "/BasicBlackBoxComparisonSerializationTest.json";
      TestCircuitSerialization(jsonDir);
      fluidCircuit = m_Circuits->GetFluidCircuit("Fluid");
    }
  }
  std::string sOutputFile = sOutputDirectory + "/BasicBlackBoxComparisonTest.csv";
  trk1.WriteTrackToFile(sOutputFile.c_str());
  m_Circuits->Clear();
}

void CommonDataModelTest::BasicBlackBoxTest(const std::string& sOutputDirectory)
{
  //jbw - Add transport

  std::cout << "BlackBoxTest\n";
  m_Logger->SetLogFile(sOutputDirectory + "/BasicBlackBoxTest.log");
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  double currentTime_s = 0.0;
  DataTrack trk1;
  DataTrack trk2;

  SEFluidCircuitCalculator fluidCalculator(m_Logger);

  //Circuit 1
  SEFluidCircuit* fluidCircuit1 = &m_Circuits->CreateFluidCircuit("fluidCircuit1");

  //Nodes
  SEFluidCircuitNode& Node1 = fluidCircuit1->CreateNode("Node1");
  SEFluidCircuitNode& Node2 = fluidCircuit1->CreateNode("Node2");
  SEFluidCircuitNode& Node3 = fluidCircuit1->CreateNode("Node3");
  SEFluidCircuitNode& Node4 = fluidCircuit1->CreateNode("Node4");

  Node4.SetAsReferenceNode();
  Node4.GetNextPressure().SetValue(0, PressureUnit::Pa);

  //Paths
  SEFluidCircuitPath& Path2 = fluidCircuit1->CreatePath(Node1, Node2, "Path2");
  Path2.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path3 = fluidCircuit1->CreatePath(Node2, Node3, "Path3");
  Path3.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  SEFluidCircuitPath& Path4 = fluidCircuit1->CreatePath(Node3, Node4, "Path4");
  Path4.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
  //Pressure source
  SEFluidCircuitPath& Path1 = fluidCircuit1->CreatePath(Node4, Node1, "Path1");
  Path1.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

  //Circuit 2
  SEFluidCircuit* fluidCircuit2 = &m_Circuits->CreateFluidCircuit("fluidCircuit2");

  SEFluidCircuitNode& Node5 = fluidCircuit2->CreateNode("Node5");
  SEFluidCircuitNode& Node6 = fluidCircuit2->CreateNode("Node6");

  Node6.SetAsReferenceNode();
  Node6.GetNextPressure().SetValue(0, PressureUnit::Pa);

  SEFluidCircuitPath& Path5 = fluidCircuit2->CreatePath(Node5, Node6, "Path5");
  Path5.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);

  //Black Boxes
  SEBlackBoxManager BBmgr(m_Logger);

  //Black Box Elements
  SEFluidCircuitNode& BB1Node = fluidCircuit1->CreateNode("BB1Node");
  SEFluidCircuitPath& BB1Path1 = fluidCircuit1->CreatePath(Node2, BB1Node, "BB1Path1");
  SEFluidCircuitPath& BB1Path2 = fluidCircuit1->CreatePath(BB1Node, Node4, "BB1Path2");
  SEFluidBlackBox& BlackBox1 = *BBmgr.GetLiquidBlackBox("BB1","");// jbw correct names
  BBmgr.MapBlackBox<MAP_FLUID_BLACK_BOX>(BlackBox1, BB1Path1, BB1Path2);

  SEFluidCircuitNode& BB2Node = fluidCircuit2->CreateNode("BB2Node");
  SEFluidCircuitPath& BB2Path1 = fluidCircuit2->CreatePath(Node6, BB2Node, "BB2Path1");
  SEFluidCircuitPath& BB2Path2 = fluidCircuit2->CreatePath(BB2Node, Node5, "BB2Path2");
  SEFluidBlackBox& BlackBox2 = *BBmgr.GetLiquidBlackBox("BB2", "");// jbw correct names
  BBmgr.MapBlackBox<MAP_FLUID_BLACK_BOX>(BlackBox2, BB2Path1, BB2Path2);

  fluidCircuit1->StateChange();
  fluidCircuit2->StateChange();

  //Imposed = black box forces it on circuit
  //Provided = circuit calculates it
  BlackBox2.ProvidePotential();
  //BlackBox2.GetSourcePressure().SetType(eBlackBox_Property_Type::Imposed);
  //BlackBox2.GetTargetPressure().SetType(eBlackBox_Property_Type::Imposed);
  BlackBox2.ProvideSourceFlux();
  BlackBox2.ProvideTargetFlux();

  BlackBox1.ProvidePotential();
  BlackBox1.ProvideSourcePotential();
  BlackBox1.ProvideTargetPotential();
  //BlackBox1.GetSourceFlow().SetType(eBlackBox_Property_Type::Imposed);
  //BlackBox1.GetTargetFlow().SetType(eBlackBox_Property_Type::Imposed);

  Node2.GetPressure().SetValue(0.0, PressureUnit::Pa);
  Node4.GetPressure().SetValue(0.0, PressureUnit::Pa);
  BB2Path2.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);

  //BlackBox1.GetPressure().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox1.GetSourcePressure().SetType(eBlackBox_Property_Type::Imposed);
  //BlackBox1.GetTargetPressure().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox1.GetSourceFlow().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox1.GetTargetFlow().SetType(eBlackBox_Property_Type::Provided);

  //BlackBox2.GetPressure().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox2.GetSourcePressure().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox2.GetTargetPressure().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox2.GetSourceFlow().SetType(eBlackBox_Property_Type::Provided);
  //BlackBox2.GetTargetFlow().SetType(eBlackBox_Property_Type::Imposed);

  //Node5.GetPressure().SetValue(8.0, PressureUnit::Pa);
  //BB1Path2.GetFlow().SetValue(0.32, VolumePerTimeUnit::m3_Per_s);

  //Run -----------------------------------------------------------------------------
  double sample = 0;
  while (currentTime_s < 100)
  {
    //Black Box settings ------------------------------------------------------------
    BlackBox2.ImposeSourcePotential(Node4.GetPressure());
    BlackBox2.ImposeTargetPotential(Node2.GetPressure());
    BlackBox1.ImposeSourceFlux(BB2Path2.GetFlow());
    BlackBox1.ImposeTargetFlux(BB2Path2.GetFlow());

    //BlackBox1.GetSourcePressure().Set(Node5.GetPressure());
    //BlackBox2.GetTargetFlow().Set(BB1Path2.GetFlow());

    //BlackBox1.GetSourcePressure().SetValue(Node5.GetPressure(PressureUnit::Pa), PressureUnit::Pa);
    //BlackBox2.GetTargetFlow().SetValue(BB1Path2.GetFlow(VolumePerTimeUnit::m3_Per_s), VolumePerTimeUnit::m3_Per_s);
        
    FluidPreProcess(*fluidCircuit1, currentTime_s);

    fluidCalculator.Process(*fluidCircuit1, timeStep_s);
    fluidCalculator.Process(*fluidCircuit2, timeStep_s);

    fluidCalculator.PostProcess(*fluidCircuit1);
    fluidCalculator.PostProcess(*fluidCircuit2);
    
    currentTime_s += timeStep_s;
    sample += timeStep_s;
    if (sample > 0.1) //every 0.1 seconds, track state of circuit
    {
      sample = 0;
      trk1.Track(currentTime_s, *fluidCircuit1);
      trk2.Track(currentTime_s, *fluidCircuit2);
    }
  }

  std::string sOutputFile1 = sOutputDirectory + "/BasicBlackBoxTestCircuit1.csv";
  std::string sOutputFile2 = sOutputDirectory + "/BasicBlackBoxTestCircuit2.csv";
  trk1.WriteTrackToFile(sOutputFile1.c_str());
  trk2.WriteTrackToFile(sOutputFile2.c_str());
  m_Circuits->Clear();
}

void CommonDataModelTest::SimpleBlackBoxTest(const std::string& sOutputDirectory)
{
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  DataTrack trkDriver;
  DataTrack trkPassive;
  DataTrack trkCombined;
  std::cout << "SimpleBlackBoxTest\n";
  m_Logger->SetLogFile(sOutputDirectory + "/SimpleBlackBoxTest.log");

  for (unsigned int iter = 0; iter < (1 << 8); ++iter)
  {
    bool isCombined = (iter == 0) ? true : false;

    bool imposeDriverSourcePressure = iter & 0x001;
    bool imposeDriverTargetPressure = iter & 0x002;
    bool imposeDriverSourceFlow = iter & 0x004;
    bool imposeDriverTargetFlow = iter & 0x008;

    bool imposePassiveSourcePressure = iter & 0x010;
    bool imposePassiveTargetPressure = iter & 0x020;
    bool imposePassiveSourceFlow = iter & 0x040;
    bool imposePassiveTargetFlow = iter & 0x080;

    bool isValid = int(imposeDriverSourcePressure) + int(imposeDriverSourceFlow) == 1 &&
      int(imposeDriverTargetPressure) + int(imposeDriverTargetFlow) == 1 &&
      int(imposePassiveSourcePressure) + int(imposePassiveSourceFlow) == 1 &&
      int(imposePassiveTargetPressure) + int(imposePassiveTargetFlow) == 1;

    //only use valid combinations
    if (!isCombined && !isValid)
    {
      continue;
    }

    SEBlackBoxManager managerBlackBox(m_Logger);
    SEFluidCircuitCalculator fluidCalculator(m_Logger);

    SEFluidCircuit* circuitCombined = &m_Circuits->CreateFluidCircuit("circuitCombined");
    SEFluidCircuit* circuitDriver = &m_Circuits->CreateFluidCircuit("circuitDriver");
    SEFluidCircuit* circuitPassive = &m_Circuits->CreateFluidCircuit("circuitPassive");

    //Driver
    //Nodes
    SEFluidCircuitNode& nodeDriverSource = circuitDriver->CreateNode("nodeDriverSource");
    SEFluidCircuitNode& nodeDriverConnection = circuitDriver->CreateNode("nodeDriverConnection");
    SEFluidCircuitNode& nodeDriverResistors = circuitDriver->CreateNode("nodeDriverResistors");
    SEFluidCircuitNode& nodeDriverGround = circuitDriver->CreateNode("nodeDriverGround");
    SEFluidCircuitNode& nodeDriverBBSource = circuitDriver->CreateNode("nodeDriverBBSource");
    SEFluidCircuitNode& nodeDriverBBTarget = circuitDriver->CreateNode("nodeDriverBBTarget");

    nodeDriverGround.SetAsReferenceNode();
    nodeDriverGround.GetNextPressure().SetValue(0, PressureUnit::Pa);

    //Paths
    SEFluidCircuitPath& pathDriverSourceToConnection = circuitDriver->CreatePath(nodeDriverSource, nodeDriverConnection, "pathDriverSourceToConnection");
    pathDriverSourceToConnection.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverConnectionToResistors = circuitDriver->CreatePath(nodeDriverConnection, nodeDriverResistors, "pathDriverConnectionToResistors");
    pathDriverConnectionToResistors.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverResistorsToGround = circuitDriver->CreatePath(nodeDriverResistors, nodeDriverGround, "pathDriverResistorsToGround");
    pathDriverResistorsToGround.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverConnectionToBBSource = circuitDriver->CreatePath(nodeDriverConnection, nodeDriverBBSource, "pathDriverConnectionToBBSource");
    //pathDriverConnectionToBBSource.GetNextResistance().SetValue(5, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverBBTargetToGround = circuitDriver->CreatePath(nodeDriverBBTarget, nodeDriverGround, "pathDriverConnectionToBBSource");
    //pathDriverBBTargetToGround.GetNextResistance().SetValue(5, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    //Pressure source
    SEFluidCircuitPath& pathDriverGroundToSource = circuitDriver->CreatePath(nodeDriverGround, nodeDriverSource, "pathDriverGroundToSource");
    pathDriverGroundToSource.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

    //Passive
    //Nodes
    SEFluidCircuitNode& nodePassiveConnection = circuitPassive->CreateNode("nodePassiveConnection");
    SEFluidCircuitNode& nodePassiveGround = circuitPassive->CreateNode("nodePassiveGround");
    SEFluidCircuitNode& nodePassiveBBSource = circuitPassive->CreateNode("nodePassiveBBSource");
    SEFluidCircuitNode& nodePassiveBBTarget = circuitPassive->CreateNode("nodePassiveBBTarget");

    nodePassiveGround.SetAsReferenceNode();
    nodePassiveGround.GetNextPressure().SetValue(0, PressureUnit::Pa);

    //Paths
    SEFluidCircuitPath& pathPassiveConnectionToGround = circuitPassive->CreatePath(nodePassiveConnection, nodePassiveGround, "pathPassiveConnectionToGround");
    pathPassiveConnectionToGround.GetNextResistance().SetValue(25, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathPassiveGroundToBBSource = circuitPassive->CreatePath(nodePassiveGround, nodePassiveBBSource, "pathPassiveGroundToBBSource");
    //pathPassiveGroundToBBSource.GetNextResistance().SetValue(5, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathPassiveBBTargetToConnection = circuitPassive->CreatePath(nodePassiveBBTarget, nodePassiveConnection, "pathPassiveBBTargetToConnection");
    //pathPassiveBBTargetToConnection.GetNextResistance().SetValue(5, PressureTimePerVolumeUnit::Pa_s_Per_m3);

    SEFluidBlackBox* blackBoxDriver = nullptr;
    SEFluidBlackBox* blackBoxPassive = nullptr;
    if (!isCombined)
    {
      //Replaces Passive
      SEFluidCircuitNode& nodeBBPassive = circuitDriver->CreateNode("nodeBBPassive");
      SEFluidCircuitPath& pathBBPassiveSource = circuitDriver->CreatePath(nodeDriverBBSource, nodeBBPassive, "pathBBPassiveSource");
      SEFluidCircuitPath& pathBBPassiveTarget = circuitDriver->CreatePath(nodeBBPassive, nodeDriverBBTarget, "pathBBPassiveTarget");
      blackBoxPassive = managerBlackBox.GetLiquidBlackBox("blackBoxPassive", "");// jbw correct names
      managerBlackBox.MapBlackBox<MAP_FLUID_BLACK_BOX>(*blackBoxPassive, pathBBPassiveSource, pathBBPassiveTarget);

      //Replaces Driver
      SEFluidCircuitNode& nodeBBDriver = circuitPassive->CreateNode("nodeBBDriver");
      SEFluidCircuitPath& pathBBDriverSource = circuitPassive->CreatePath(nodePassiveBBSource, nodeBBDriver, "pathBBDriverSource");
      SEFluidCircuitPath& pathBBDriverTarget = circuitPassive->CreatePath(nodeBBDriver, nodePassiveBBTarget, "pathBBDriverTarget");
      blackBoxDriver = managerBlackBox.GetLiquidBlackBox("blackBoxDriver", "");// jbw correct names
      managerBlackBox.MapBlackBox<MAP_FLUID_BLACK_BOX>(*blackBoxDriver, pathBBDriverSource, pathBBDriverTarget);

      // Imposed = black box forces it on circuit
      // Provided = circuit calculates it
      // Reset everything to provided
      blackBoxDriver->ProvidePotential();
      blackBoxDriver->ProvideSourcePotential();
      blackBoxDriver->ProvideTargetPotential();
      blackBoxDriver->ProvideSourceFlux();
      blackBoxDriver->ProvideTargetFlux();

      blackBoxPassive->ProvidePotential();
      blackBoxPassive->ProvideSourcePotential();
      blackBoxPassive->ProvideTargetPotential();
      blackBoxPassive->ProvideSourceFlux();
      blackBoxPassive->ProvideTargetFlux();

      //Initialize all potential black box inputs
      nodeDriverBBSource.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeDriverBBTarget.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodePassiveBBSource.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodePassiveBBTarget.GetPressure().SetValue(0.0, PressureUnit::Pa);
      pathDriverConnectionToBBSource.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathDriverBBTargetToGround.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathPassiveGroundToBBSource.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathPassiveBBTargetToConnection.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
    }

    circuitDriver->StateChange();
    circuitPassive->StateChange();

    if (isCombined)
    {
      //Combined circuit for comparison
      circuitCombined->AddCircuit(*circuitDriver);
      circuitCombined->AddCircuit(*circuitPassive);
      circuitCombined->CreatePath(nodeDriverBBSource, nodePassiveBBTarget, "pathCombinedDriverToPassive");
      circuitCombined->CreatePath(nodePassiveBBSource, nodeDriverBBTarget, "pathCombinedPassiveToDriver");
      circuitCombined->StateChange();
    }

    //Run -----------------------------------------------------------------------------
    double currentTime_s = 0.0;
    double sample = 0;
    bool hasError = false;
    while (currentTime_s < 100)
    {
      double dPotential = 20 + 20 * sin(currentTime_s);
      pathDriverGroundToSource.GetNextPressureSource().SetValue(dPotential, PressureUnit::Pa);

      if (!isCombined)
      {
        if (imposePassiveSourcePressure)
        {
          blackBoxPassive->ImposeSourcePotential(nodePassiveBBTarget.GetPressure());
        }
        if (imposePassiveTargetPressure)
        {
          blackBoxPassive->ImposeTargetPotential(nodePassiveBBSource.GetPressure());
        }
        if (imposePassiveSourceFlow)
        {
          blackBoxPassive->ImposeSourceFlux(pathPassiveBBTargetToConnection.GetFlow());
        }
        if (imposePassiveTargetFlow)
        {
          blackBoxPassive->ImposeTargetFlux(pathPassiveGroundToBBSource.GetFlow());
        }

        if (imposeDriverSourcePressure)
        {
          blackBoxDriver->ImposeSourcePotential(nodeDriverBBTarget.GetPressure());
        }
        if (imposeDriverTargetPressure)
        {
          blackBoxDriver->ImposeTargetPotential(nodeDriverBBSource.GetPressure());
        }
        if (imposeDriverSourceFlow)
        {
          blackBoxDriver->ImposeSourceFlux(pathDriverBBTargetToGround.GetFlow());
        }
        if (imposeDriverTargetFlow)
        {
          blackBoxDriver->ImposeTargetFlux(pathDriverConnectionToBBSource.GetFlow());
        }

        try
        {
          fluidCalculator.Process(*circuitDriver, timeStep_s);
          fluidCalculator.Process(*circuitPassive, timeStep_s);
        }
        catch (std::exception ex)
        {
          hasError = true;
          break;
        }

        fluidCalculator.PostProcess(*circuitDriver);
        fluidCalculator.PostProcess(*circuitPassive);
      }
      else
      {
        fluidCalculator.Process(*circuitCombined, timeStep_s);
        fluidCalculator.PostProcess(*circuitCombined);
      }

      if (sample > 0.1) //every 0.1 seconds, track state of circuit
      {
        sample = 0;

        if (!isCombined)
        {
          trkPassive.Track(currentTime_s, *circuitPassive);
          trkDriver.Track(currentTime_s, *circuitDriver);
        }
        else
        {
          trkCombined.Track(currentTime_s, *circuitCombined);
        }
      }

      sample += timeStep_s;
      currentTime_s += timeStep_s;
    }

    std::string outputFile;

    if (imposePassiveSourcePressure)
    {
      outputFile += "_imposePassiveSourcePressure";
    }
    if (imposePassiveTargetPressure)
    {
      outputFile += "_imposePassiveTargetPressure";
    }
    if (imposePassiveSourceFlow)
    {
      outputFile += "_imposePassiveSourceFlow";
    }
    if (imposePassiveTargetFlow)
    {
      outputFile += "_imposePassiveTargetFlow";
    }

    if (imposeDriverSourcePressure)
    {
      outputFile += "_imposeDriverSourcePressure";
    }
    if (imposeDriverTargetPressure)
    {
      outputFile += "_imposeDriverTargetPressure";
    }
    if (imposeDriverSourceFlow)
    {
      outputFile += "_imposeDriverSourceFlow";
    }
    if (imposeDriverTargetFlow)
    {
      outputFile += "_imposeDriverTargetFlow";
    }

    if (hasError)
    {
      m_Logger->Info("Failure: " + outputFile);
      std::stringstream ss;
      ss << "Time(s): " << currentTime_s;
      m_Logger->Info(ss);
      m_Circuits->Clear();
      continue;
    }


    std::string sOutputFileDriver = sOutputDirectory + "/SimpleBlackBoxDriver" + outputFile + ".csv";
    std::string sOutputFilePassive = sOutputDirectory + "/SimpleBlackBoxPassive" + outputFile + ".csv";
    std::string sOutputFileCombined = sOutputDirectory + "/SimpleBlackBoxCombined.csv";

    if (!isCombined)
    {
      trkDriver.WriteTrackToFile(sOutputFileDriver.c_str());
      trkPassive.WriteTrackToFile(sOutputFilePassive.c_str());
    }
    else
    {
      trkCombined.WriteTrackToFile(sOutputFileCombined.c_str());
    }

    m_Circuits->Clear();
  }
}


void CommonDataModelTest::WindkesselBlackBoxTest(const std::string& sOutputDirectory)
{
  TimingProfile p;
  double timeStep_s = 1.0 / 50.0;
  DataTrack trkWindkessel;
  DataTrack trkDriver;
  DataTrack trkCombined;
  std::cout << "WindkesselBlackBoxTest\n";
  m_Logger->SetLogFile(sOutputDirectory + "/WindkesselBlackBoxTest.log");
  m_Logger->LogToConsole(true);

  for (unsigned int iter = 0; iter < (1 << 8); ++iter)
  {
    bool isCombined = (iter == 0) ? true : false;

    bool imposeWindkesselSourcePressure = iter & 0x001;
    bool imposeWindkesselTargetPressure = iter & 0x002;
    bool imposeWindkesselSourceFlow = iter & 0x004;
    bool imposeWindkesselTargetFlow = iter & 0x008;

    bool imposeDriverSourcePressure = iter & 0x010;
    bool imposeDriverTargetPressure = iter & 0x020;
    bool imposeDriverSourceFlow = iter & 0x040;
    bool imposeDriverTargetFlow = iter & 0x080;

    bool isValid = int(imposeWindkesselSourcePressure) + int(imposeWindkesselSourceFlow) == 1 &&
      int(imposeWindkesselTargetPressure) + int(imposeWindkesselTargetFlow) == 1 &&
      int(imposeDriverSourcePressure) + int(imposeDriverSourceFlow) == 1 &&
      int(imposeDriverTargetPressure) + int(imposeDriverTargetFlow) == 1;

    //only use valid combinations
    if (!isCombined && !isValid)
    {
      continue;
    }

    SEBlackBoxManager managerBlackBox(m_Logger);
    SEFluidCircuitCalculator fluidCalculator(m_Logger);

    SEFluidCircuit* circuitCombined = &m_Circuits->CreateFluidCircuit("circuitCombined");
    SEFluidCircuit* circuitWindkessel = &m_Circuits->CreateFluidCircuit("circuitWindkessel");
    SEFluidCircuit* circuitDriver = &m_Circuits->CreateFluidCircuit("circuitDriver");

    //Windkessel Circuit
    //Nodes
    SEFluidCircuitNode& nodeWindkesselResistance1 = circuitWindkessel->CreateNode("nodeWindkesselResistance1");
    SEFluidCircuitNode& nodeWindkesselResistance2 = circuitWindkessel->CreateNode("nodeWindkesselResistance2");
    SEFluidCircuitNode& nodeWindkesselCompliance = circuitWindkessel->CreateNode("nodeWindkesselCompliance");
    nodeWindkesselCompliance.GetPressure().SetValue(0, PressureUnit::Pa);
    nodeWindkesselCompliance.GetVolume().SetValue(0.0, VolumeUnit::m3);
    SEFluidCircuitNode& nodeWindkesselGround = circuitWindkessel->CreateNode("nodeWindkesselGround");
    nodeWindkesselGround.SetAsReferenceNode();
    nodeWindkesselGround.GetPressure().SetValue(0, PressureUnit::Pa);

    //Paths
    SEFluidCircuitPath& pathWindkesselResistance1 = circuitWindkessel->CreatePath(nodeWindkesselResistance1, nodeWindkesselCompliance, "pathWindkesselResistance1");
    pathWindkesselResistance1.GetNextResistance().SetValue(5, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathWindkesselResistance2 = circuitWindkessel->CreatePath(nodeWindkesselCompliance, nodeWindkesselResistance2, "pathWindkesselResistance2");
    pathWindkesselResistance2.GetNextResistance().SetValue(10, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathWindkesselCompliance = circuitWindkessel->CreatePath(nodeWindkesselCompliance, nodeWindkesselGround, "pathWindkesselCompliance");
    pathWindkesselCompliance.GetNextCompliance().SetValue(1.0, VolumePerPressureUnit::m3_Per_Pa);

    //Driver Circuit
    //Nodes
    SEFluidCircuitNode& nodeDriverResistance1 = circuitDriver->CreateNode("nodeDriverResistance1");
    SEFluidCircuitNode& nodeDriverResistance2 = circuitDriver->CreateNode("nodeDriverResistance2");
    SEFluidCircuitNode& nodeDriverSource = circuitDriver->CreateNode("nodeDriverSource");
    SEFluidCircuitNode& nodeDriverGround = circuitDriver->CreateNode("nodeDriverGround");
    nodeDriverGround.SetAsReferenceNode();
    nodeDriverGround.GetPressure().SetValue(0, PressureUnit::Pa);

    //Paths
    SEFluidCircuitPath& pathDriverResistance1 = circuitDriver->CreatePath(nodeDriverResistance1, nodeDriverSource, "pathDriverResistance1");
    pathDriverResistance1.GetNextResistance().SetValue(12, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverResistance2 = circuitDriver->CreatePath(nodeDriverSource, nodeDriverResistance2, "pathDriverResistance2");
    pathDriverResistance2.GetNextResistance().SetValue(8, PressureTimePerVolumeUnit::Pa_s_Per_m3);
    SEFluidCircuitPath& pathDriverSource = circuitDriver->CreatePath(nodeDriverGround, nodeDriverSource, "pathDriverSource");
    pathDriverSource.GetNextPressureSource().SetValue(20, PressureUnit::Pa);

    SEFluidBlackBox* blackBoxWindkessel = nullptr;
    SEFluidBlackBox* blackBoxDriver = nullptr;
    if (!isCombined)
    {
      //Replaces the driver
      SEFluidCircuitNode& nodeBlackBoxDriver = circuitWindkessel->CreateNode("nodeBlackBoxDriver");
      SEFluidCircuitPath& pathBlackBoxDriverSource = circuitWindkessel->CreatePath(nodeWindkesselResistance2, nodeBlackBoxDriver, "pathBlackBoxDriverSource");
      SEFluidCircuitPath& pathBlackBoxDriverTarget = circuitWindkessel->CreatePath(nodeBlackBoxDriver, nodeWindkesselResistance1, "pathBlackBoxDriverTarget");
      blackBoxDriver = managerBlackBox.GetLiquidBlackBox("blackBoxDriver", "");// jbw correct names
      managerBlackBox.MapBlackBox<MAP_FLUID_BLACK_BOX>(*blackBoxDriver, pathBlackBoxDriverSource, pathBlackBoxDriverTarget);

      //Replaces the Windkessel
      SEFluidCircuitNode& nodeBlackBoxWindkessel = circuitDriver->CreateNode("nodeBlackBoxWindkessel");
      SEFluidCircuitPath& pathBlackBoxWindkesselSource = circuitDriver->CreatePath(nodeDriverResistance2, nodeBlackBoxWindkessel, "pathBlackBoxWindkesselSource");
      SEFluidCircuitPath& pathBlackBoxWindkesselTarget = circuitDriver->CreatePath(nodeBlackBoxWindkessel, nodeDriverResistance1, "pathBlackBoxWindkesselTarget");
      blackBoxWindkessel = managerBlackBox.GetLiquidBlackBox("blackBoxWindkessel", "");// jbw correct names
      managerBlackBox.MapBlackBox<MAP_FLUID_BLACK_BOX>(*blackBoxWindkessel, pathBlackBoxWindkesselSource, pathBlackBoxWindkesselTarget);

      // Imposed = black box forces it on circuit
      // Provided = circuit calculates it
      // Reset everything to provided
      blackBoxWindkessel->ProvidePotential();
      blackBoxWindkessel->ProvideSourcePotential();
      blackBoxWindkessel->ProvideTargetPotential();
      blackBoxWindkessel->ProvideSourceFlux();
      blackBoxWindkessel->ProvideTargetFlux();

      blackBoxDriver->ProvidePotential();
      blackBoxDriver->ProvideSourcePotential();
      blackBoxDriver->ProvideTargetPotential();
      blackBoxDriver->ProvideSourceFlux();
      blackBoxDriver->ProvideTargetFlux();

      //Initialize all potential black box inputs
      nodeWindkesselResistance1.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeWindkesselResistance2.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeDriverResistance1.GetPressure().SetValue(0.0, PressureUnit::Pa);
      nodeDriverResistance2.GetPressure().SetValue(0.0, PressureUnit::Pa);
      pathWindkesselResistance1.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathWindkesselResistance2.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathDriverResistance1.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
      pathDriverResistance2.GetFlow().SetValue(0.0, VolumePerTimeUnit::m3_Per_s);
    }

    circuitWindkessel->StateChange();
    circuitDriver->StateChange();

    if (isCombined)
    {
      //Combined circuit for comparison
      circuitCombined->AddCircuit(*circuitWindkessel);
      circuitCombined->AddCircuit(*circuitDriver);
      circuitCombined->CreatePath(nodeDriverResistance2, nodeWindkesselResistance1, "pathDriverResistance2ToWindkesselResistance1");
      circuitCombined->CreatePath(nodeWindkesselResistance2, nodeDriverResistance1, "pathWindkesselResistance2ToDriverResistance1");
      circuitCombined->StateChange();
    }    

    //Run -----------------------------------------------------------------------------
    double currentTime_s = 0.0;
    double sample = 0;
    bool hasError = false;
    while (currentTime_s < 100)
    {
      double dPotential = 20 + 20 * sin(currentTime_s);
      pathDriverSource.GetNextPressureSource().SetValue(dPotential, PressureUnit::Pa);

      if (!isCombined)
      {
        if (imposeDriverSourcePressure)
        {
          blackBoxDriver->ImposeSourcePotential(nodeDriverResistance1.GetPressure());
        }
        if (imposeDriverTargetPressure)
        {
          blackBoxDriver->ImposeTargetPotential(nodeDriverResistance2.GetPressure());
        }
        if (imposeDriverSourceFlow)
        {
          blackBoxDriver->ImposeSourceFlux(pathDriverResistance1.GetFlow());
        }
        if (imposeDriverTargetFlow)
        {
          blackBoxDriver->ImposeTargetFlux(pathDriverResistance2.GetFlow());
        }
        
        if (imposeWindkesselSourcePressure)
        {
          blackBoxWindkessel->ImposeSourcePotential(nodeWindkesselResistance1.GetPressure());
        }
        if (imposeWindkesselTargetPressure)
        {
          blackBoxWindkessel->ImposeTargetPotential(nodeWindkesselResistance2.GetPressure());
        }
        if (imposeWindkesselSourceFlow)
        {
          blackBoxWindkessel->ImposeSourceFlux(pathWindkesselResistance1.GetFlow());
        }
        if (imposeWindkesselTargetFlow)
        {
          blackBoxWindkessel->ImposeTargetFlux(pathWindkesselResistance2.GetFlow());
        }

        try 
        {
          fluidCalculator.Process(*circuitWindkessel, timeStep_s);
          fluidCalculator.Process(*circuitDriver, timeStep_s);
        }
        catch (std::exception ex)
        {
          hasError = true;
          break;
        }

        fluidCalculator.PostProcess(*circuitWindkessel);
        fluidCalculator.PostProcess(*circuitDriver);
      }
      else
      {
        fluidCalculator.Process(*circuitCombined, timeStep_s);
        fluidCalculator.PostProcess(*circuitCombined);
      }

      currentTime_s += timeStep_s;
      sample += timeStep_s;
      if (sample > 0.1) //every 0.1 seconds, track state of circuit
      {
        sample = 0;

        if (currentTime_s > timeStep_s)
        {
          if (!isCombined)
          {
            trkDriver.Track(currentTime_s, *circuitDriver);
            trkWindkessel.Track(currentTime_s, *circuitWindkessel);
          }
          else
          {
            trkCombined.Track(currentTime_s, *circuitCombined);
          }
        }
      }
    }

    std::string outputFile;

    if (imposeDriverSourcePressure)
    {
      outputFile += "_imposeDriverSourcePressure";
    }
    if (imposeDriverTargetPressure)
    {
      outputFile += "_imposeDriverTargetPressure";
    }
    if (imposeDriverSourceFlow)
    {
      outputFile += "_imposeDriverSourceFlow";
    }
    if (imposeDriverTargetFlow)
    {
      outputFile += "_imposeDriverTargetFlow";
    }

    if (imposeWindkesselSourcePressure)
    {
      outputFile += "_imposeWindkesselSourcePressure";
    }
    if (imposeWindkesselTargetPressure)
    {
      outputFile += "_imposeWindkesselTargetPressure";
    }
    if (imposeWindkesselSourceFlow)
    {
      outputFile += "_imposeWindkesselSourceFlow";
    }
    if (imposeWindkesselTargetFlow)
    {
      outputFile += "_imposeWindkesselTargetFlow";
    }

    if (hasError)
    {
      m_Logger->Info("Failure: " + outputFile);
      std::stringstream ss;
      ss << "Time(s): " << currentTime_s;
      m_Logger->Info(ss);
      m_Circuits->Clear();
      continue;
    }

    Info("Writing Windkessel for " + outputFile);
    std::string sOutputFileWindkessel = sOutputDirectory + "/BlackBoxWindkessel" + outputFile + ".csv";
    std::string sOutputFileDriver = sOutputDirectory + "/BlackBoxDriver" + outputFile + ".csv";
    std::string sOutputFileCombined = sOutputDirectory + "/BlackBoxCombined.csv";

    if (!isCombined)
    {
      trkWindkessel.WriteTrackToFile(sOutputFileWindkessel.c_str());
      trkDriver.WriteTrackToFile(sOutputFileDriver.c_str());
    }
    else
    {
      trkCombined.WriteTrackToFile(sOutputFileCombined.c_str());
    }   

    m_Circuits->Clear();
  }
}
