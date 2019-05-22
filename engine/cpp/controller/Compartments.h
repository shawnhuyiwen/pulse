/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PulseController;
#include "compartment/SECompartmentManager.h"

/**
* @brief Manages all compartments and graphs associated with all %Pulse systems/equipement
*/
class PULSE_DECL PulseCompartments : public SECompartmentManager
{
  friend class PulseSubstances;
public:

  PulseCompartments(PulseController& data);
  virtual ~PulseCompartments();

  void Clear();

  void StateChange();
  void UpdateAirwayGraph() { m_UpdateActiveAirwayGraph = true; m_UpdateActiveAerosolGraph = true; }

  SELiquidCompartmentGraph& GetActiveCardiovascularGraph();
  SELiquidCompartmentGraph& GetCardiovascularGraph();
  SELiquidCompartmentGraph& GetRenalGraph();

  SEGasCompartmentGraph&    GetActiveRespiratoryGraph();
  SEGasCompartmentGraph&    GetRespiratoryGraph();
  SEGasCompartmentGraph&    GetRespiratoryAndAnesthesiaMachineGraph();
  SEGasCompartmentGraph&    GetRespiratoryAndInhalerGraph();
  SEGasCompartmentGraph&    GetRespiratoryAndNasalCannulaGraph();
  SEGasCompartmentGraph&    GetRespiratoryAndSimpleMaskGraph();
  SEGasCompartmentGraph&    GetRespiratoryAndNonRebreatherMaskGraph();
  SEGasCompartmentGraph&    GetRespiratoryAndMechanicalVentilatorGraph();

  SEGasCompartmentGraph&    GetAnesthesiaMachineGraph();

  SELiquidCompartmentGraph& GetActiveAerosolGraph();
  SELiquidCompartmentGraph& GetAerosolGraph();
  SELiquidCompartmentGraph& GetAerosolAndInhalerGraph();
  SELiquidCompartmentGraph& GetAerosolAndMechanicalVentilatorGraph();

  // Compartment Helpers  
  const std::vector<SELiquidCompartment*>& GetAerosolCompartments() { return m_AerosolCompartments; }
  const std::vector<SELiquidCompartment*>& GetAerosolLeafCompartments() { return m_AerosolLeafCompartments; }

  const std::vector<SELiquidCompartment*>& GetChymeCompartments() { return m_ChymeCompartments; }
  const std::vector<SELiquidCompartment*>& GetChymeLeafCompartments() { return m_ChymeLeafCompartments; }

  const std::vector<SEGasCompartment*>& GetPulmonaryCompartments() { return m_PulmonaryCompartments; }
  const std::vector<SEGasCompartment*>& GetPulmonaryLeafCompartments() { return m_PulmonaryLeafCompartments; }

  const std::vector<SEThermalCompartment*>& GetTemperatureCompartments() { return m_TemperatureCompartments; }
  const std::vector<SEThermalCompartment*>& GetTemperatureLeafCompartments() { return m_TemperatureLeafCompartments; }

  const std::vector<SETissueCompartment*>& GetTissueCompartments() { return m_TissueCompartments; }
  const std::vector<SETissueCompartment*>& GetTissueLeafCompartments() { return m_TissueLeafCompartments; }

  const std::vector<SELiquidCompartment*>& GetUrineCompartments() { return m_UrineCompartments; }
  const std::vector<SELiquidCompartment*>& GetUrineLeafCompartments() { return m_UrineLeafCompartments; }

  const std::vector<SELiquidCompartment*>& GetVascularCompartments() { return m_VascularCompartments; }
  const std::vector<SELiquidCompartment*>& GetVascularLeafCompartments() { return m_VascularLeafCompartments; }

  const std::vector<SEGasCompartment*>& GetAnesthesiaMachineCompartments() { return m_AnesthesiaMachineCompartments; }
  const std::vector<SEGasCompartment*>& GetAnesthesiaMachineLeafCompartments() { return m_AnesthesiaMachineLeafCompartments; }

  const std::vector<SEGasCompartment*>& GetInhalerCompartments() { return m_InhalerCompartments; }
  const std::vector<SEGasCompartment*>& GetInhalerLeafCompartments() { return m_InhalerLeafCompartments; }

  const std::vector<SELiquidCompartment*>& GetInhalerAerosolCompartments() { return m_InhalerAerosolCompartments; }
  const std::vector<SELiquidCompartment*>& GetInhalerAerosolLeafCompartments() { return m_InhalerAerosolLeafCompartments; }

  const std::vector<SEGasCompartment*>& GetMechanicalVentilatorCompartments() { return m_MechanicalVentilatorCompartments; }
  const std::vector<SEGasCompartment*>& GetMechanicalVentilatorLeafCompartments() { return m_MechanicalVentilatorCompartments; }

  const std::vector<SELiquidCompartment*>& GetMechanicalVentilatorAerosolCompartments() { return m_MechanicalVentilatorAerosolCompartments; }
  const std::vector<SELiquidCompartment*>& GetMechanicalVentilatorAerosolLeafCompartments() { return m_MechanicalVentilatorAerosolCompartments; }

  SELiquidCompartment& GetExtracellularFluid(SETissueCompartment& t) { return *m_ExtracellularFluid[&t]; }
  SELiquidCompartment& GetIntracellularFluid(SETissueCompartment& t) { return *m_IntracellularFluid[&t]; }
  const std::map<SETissueCompartment*, SELiquidCompartment*>& GetExtracellularFluid() { return m_ExtracellularFluid; }
  const std::map<SETissueCompartment*, SELiquidCompartment*>& GetIntracellularFluid() { return m_IntracellularFluid; }

protected:
  // I don't want these exposed in Pulse, you should be calling the Substance manager
  virtual void AddGasCompartmentSubstance(SESubstance& sub);
  virtual void AddLiquidCompartmentSubstance(SESubstance& sub);
  
  virtual bool AllowGasSubstance(SESubstance& s, SEGasCompartment& cmpt) const;
  virtual bool AllowLiquidSubstance(SESubstance& s, SELiquidCompartment& cmpt) const;

  PulseController& m_data;

  bool m_UpdateActiveAirwayGraph;
  bool m_UpdateActiveAerosolGraph;

  SELiquidCompartmentGraph*   m_CombinedCardiovascularGraph;
  SELiquidCompartmentGraph*   m_CardiovascularGraph;
  SELiquidCompartmentGraph*   m_RenalGraph;

  SEGasCompartmentGraph*     m_RespiratoryGraph;
  SEGasCompartmentGraph*     m_CombinedRespiratoryAnesthesiaGraph;
  SEGasCompartmentGraph*     m_CombinedRespiratoryInhalerGraph;
  SEGasCompartmentGraph*     m_CombinedRespiratoryNasalCannulaGraph;
  SEGasCompartmentGraph*     m_CombinedRespiratorySimpleMaskGraph;
  SEGasCompartmentGraph*     m_CombinedRespiratoryNonRebreatherMaskGraph;
  SEGasCompartmentGraph*     m_CombinedRespiratoryMechanicalVentilatorGraph;
  SEGasCompartmentGraph*     m_AnesthesiaMachineGraph;

  SELiquidCompartmentGraph*  m_AerosolGraph;
  SELiquidCompartmentGraph*  m_CombinedAerosolInhalerGraph;
  SELiquidCompartmentGraph*  m_CombinedAerosolMechanicalVentilatorGraph;


  std::vector<SELiquidCompartment*>  m_AerosolCompartments;
  std::vector<SELiquidCompartment*>  m_AerosolLeafCompartments;
  std::vector<SELiquidCompartment*>  m_ChymeCompartments;
  std::vector<SELiquidCompartment*>  m_ChymeLeafCompartments;
  std::vector<SEGasCompartment*>     m_PulmonaryCompartments;
  std::vector<SEGasCompartment*>     m_PulmonaryLeafCompartments;
  std::vector<SEThermalCompartment*> m_TemperatureCompartments;
  std::vector<SEThermalCompartment*> m_TemperatureLeafCompartments;
  std::vector<SETissueCompartment*>  m_TissueCompartments;
  std::vector<SETissueCompartment*>  m_TissueLeafCompartments;
  std::vector<SELiquidCompartment*>  m_UrineCompartments;
  std::vector<SELiquidCompartment*>  m_UrineLeafCompartments;
  std::vector<SELiquidCompartment*>  m_VascularCompartments;
  std::vector<SELiquidCompartment*>  m_VascularLeafCompartments;
  std::vector<SEGasCompartment*>     m_AnesthesiaMachineCompartments;
  std::vector<SEGasCompartment*>     m_AnesthesiaMachineLeafCompartments;
  std::vector<SEGasCompartment*>     m_InhalerCompartments;
  std::vector<SEGasCompartment*>     m_InhalerLeafCompartments;
  std::vector<SELiquidCompartment*>  m_InhalerAerosolCompartments;
  std::vector<SELiquidCompartment*>  m_InhalerAerosolLeafCompartments;
  std::vector<SEGasCompartment*>     m_MechanicalVentilatorCompartments;
  std::vector<SEGasCompartment*>     m_MechanicalVentilatorLeafCompartments;
  std::vector<SELiquidCompartment*>  m_MechanicalVentilatorAerosolCompartments;
  std::vector<SELiquidCompartment*>  m_MechanicalVentilatorAerosolLeafCompartments;

  std::map<SETissueCompartment*, SELiquidCompartment*> m_ExtracellularFluid;
  std::map<SETissueCompartment*, SELiquidCompartment*> m_IntracellularFluid;
};