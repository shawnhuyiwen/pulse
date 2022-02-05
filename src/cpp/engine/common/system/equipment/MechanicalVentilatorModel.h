/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/common/system/Model.h"
#include "engine/common/system/Systems.h"

class SEEquipmentActionCollection;
class SEGasCompartment;
class SELiquidCompartment;
class SEGasSubstanceQuantity;
class SEFluidCircuitNode;
class SEFluidCircuitPath;

namespace pulse
{
  class PBEquipment;
  class EngineTest;
  /**
   * @brief
   * Generic mechanical ventilator for positive pressure ventilation.
   */
  class PULSE_DECL MechanicalVentilatorModel : public MechanicalVentilatorSystem, public Model
  {
    friend PBEquipment;//friend the serialization class
    friend EngineTest;
  public:
    MechanicalVentilatorModel(Data& pc);
    virtual ~MechanicalVentilatorModel();

    void Clear();

    // Set members to a stable homeostatic state
    void Initialize();
    // Set pointers and other member varialbes common to both homeostatic initialization and loading a state
    void SetUp();

    void StateChange();

    void AtSteadyState() {}
    void PreProcess();
    void Process(bool solve_and_transport = true);
    void PostProcess(bool solve_and_transport = true);

  protected:

    void ComputeExposedModelParameters() override;

    void UpdateAirwayMode();

    void CalculateInspiration();
    void CalculatePause();
    void CalculateExpiration();
    void SetVentilatorDriver();
    void CycleMode(bool patientTriggered);
    void SetLeak();
    void SetHold();
    void CalculateRespiratoryParameters();
    void CalculateInspiratoryRespiratoryParameters();
    void CalculatePauseRespiratoryParameters();
    void CalculateExpiratoryRespiratoryParameters();
    void SetResistances();
    void SetCompliance();
    void SetVolumes();

    // Serializable member variables (Set in Initialize and in schema)
    double                m_CurrentPeriodTime_s;
    double                m_DriverPressure_cmH2O;
    double                m_PreviousDriverFlow_L_Per_s;
    double                m_PreviousDriverPressure_cmH2O;
    double                m_DriverFlow_L_Per_s;
    double                m_CurrentVentilatorVolume_L;
    double                m_CurrentRespiratoryVolume_L;
    double                m_InspirationTime_s;
    double                m_InspiratoryFlow_L_Per_s;
    bool                  m_LimitReached;
    double                m_PreviousYPieceToConnectionFlow_L_Per_s;
    double                m_PreviousConnectionPressure_cmH2O;
    eBreathState          m_CurrentBreathState;

    SERunningAverage* m_MeanAirwayPressure_cmH2O;

    // Stateless member variable (Set in SetUp())
    SEGasCompartment*    m_Environment;
    SEGasCompartment*    m_Ventilator;
    SEGasCompartment*    m_Connection;
    SELiquidCompartment* m_VentilatorAerosol;
    SEFluidCircuitNode*  m_VentilatorNode;
    SEFluidCircuitNode*  m_ExpiratoryLimbNode;
    SEFluidCircuitNode*  m_ExpiratoryValveNode;
    SEFluidCircuitNode*  m_InspiratoryLimbNode;
    SEFluidCircuitNode*  m_InspiratoryValveNode;
    SEFluidCircuitNode*  m_YPieceNode;
    SEFluidCircuitNode*  m_ConnectionNode;
    SEFluidCircuitNode*  m_AmbientNode;
    SEFluidCircuitPath*  m_EnvironmentToVentilator;
    SEFluidCircuitPath*  m_VentilatorToEnvironment;
    SEFluidCircuitPath*  m_YPieceToConnection;
    SEFluidCircuitPath*  m_VentilatorToExpiratoryValve;
    SEFluidCircuitPath*  m_VentilatorToInspiratoryValve;
    SEFluidCircuitPath*  m_ExpiratoryLimbToYPiece;
    SEFluidCircuitPath*  m_InspiratoryLimbToYPiece;
    SEFluidCircuitPath*  m_LeakConnectionToEnvironment;
    SEFluidCircuitPath*  m_ConnectionToAirway;
    double               m_DefaultClosedFlowResistance_cmH2O_s_Per_L;
  };
END_NAMESPACE
