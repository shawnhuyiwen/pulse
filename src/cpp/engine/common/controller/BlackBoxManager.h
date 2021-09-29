/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/CommonDefs.h"
#include "cdm/blackbox/SEBlackBoxManager.h"

#define COMPATIBLE_BLACK_BOX_TEMPLATE typename CompartmentType

#define CREATE_BLACK_BOX_COMPONENTS_TEMPLATE typename BlackBoxType, \
                                             typename CircuitType, typename NodeType, typename PathType, \
                                             typename GraphType, typename CompartmentType, typename LinkType, \
                                             typename PotentialUnit, typename QuantityUnit, typename FluxUnit

namespace pulse
{
  class Controller;
  /**
  * @brief Manages and initializes all systems with substances needed by %Pulse
  */
  class PULSE_DECL BlackBoxManager : public SEBlackBoxManager
  {
    friend class PulseEngineTest;
  public:
    BlackBoxManager(Controller& data);
    virtual ~BlackBoxManager() = default;

  protected:
    template<COMPATIBLE_BLACK_BOX_TEMPLATE> bool IsValidBlackBoxRequest(CompartmentType* srcCmpt, CompartmentType* tgtCmpt);

    SEGasBlackBox* CreateGasBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) override;
    SELiquidBlackBox* CreateLiquidBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) override;
    SEThermalBlackBox* CreateThermalBlackBox(const std::string& srcCmptName, const std::string& tgtCmptName) override;

    template<CREATE_BLACK_BOX_COMPONENTS_TEMPLATE> bool CreateComponents(BlackBoxType& bb,
      NodeType& srcNode, NodeType& tgtNode,
      CompartmentType& srcCmpt, CompartmentType& tgtCmpt, LinkType& replaceLink,
      CircuitType& circuit, GraphType& graph,
      const PotentialUnit& pUnit, const QuantityUnit& qUnit, const FluxUnit& fUnit);

    Controller& m_data;
  };
END_NAMESPACE
