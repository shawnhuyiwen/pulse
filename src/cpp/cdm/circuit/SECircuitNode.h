/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalarElectricCharge.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarEnergy.h"

enum class eBlackBox_Property_Type
{
  Calculate = 0,
  Imposed
};

enum class eBlackBox_Node_Type
{
  None = 0,
  Source,
  Target,
  Middle
};

#define CIRCUIT_NODE_TEMPLATE typename PotentialScalar, typename QuantityScalar, \
                              typename PotentialUnit, typename QuantityUnit
#define CIRCUIT_NODE_TYPES PotentialScalar,QuantityScalar, \
                           PotentialUnit,QuantityUnit
#define ELECTRICAL_CIRCUIT_NODE SEScalarElectricPotential, SEScalarElectricCharge, \
                                ElectricPotentialUnit, ElectricChargeUnit
#define FLUID_CIRCUIT_NODE SEScalarPressure, SEScalarVolume, \
                           PressureUnit, VolumeUnit
#define THERMAL_CIRCUIT_NODE SEScalarTemperature, SEScalarEnergy, \
                             TemperatureUnit, EnergyUnit

template<CIRCUIT_NODE_TEMPLATE>
class SECircuitNode : public Loggable
{
  friend class PBCircuit;//friend the serialization class
  template< typename NodeType, typename PathType> friend class SECircuit;
protected:
  SECircuitNode(const std::string& name, Logger* logger);
public:
  virtual ~SECircuitNode();

  virtual void Clear(); //clear memory

  virtual std::string GetName() const;

  virtual bool IsPartOfBlackBox() const;
  virtual eBlackBox_Node_Type GetBlackBoxType() const;
  virtual void SetBlackBoxType(eBlackBox_Node_Type e);

  virtual bool HasPotential() const;
  virtual PotentialScalar& GetPotential();
  virtual bool HasNextPotential() const;
  virtual PotentialScalar& GetNextPotential();

  virtual void RemoveImposedPotential();
  virtual bool IsPotentialImposed() const;
  virtual void ImposePotential(const PotentialScalar& s);
  virtual void ImposePotential(double v, const PotentialUnit& unit);

  virtual bool HasQuantity() const;
  virtual QuantityScalar& GetQuantity();
  virtual bool HasNextQuantity() const;
  virtual QuantityScalar& GetNextQuantity();
  virtual bool HasQuantityBaseline() const;
  virtual QuantityScalar& GetQuantityBaseline();

  virtual void RemoveImposedQuantity();
  virtual bool IsQuantityImposed() const;
  virtual void ImposeQuantity(const QuantityScalar& s);
  virtual void ImposeQuantity(double v, const QuantityUnit& unit);

  void SetCalculatorIndex(const size_t index);
  size_t GetCalculatorIndex() const;

  bool IsReferenceNode() const;
  void SetAsReferenceNode();
  void RemoveAsReferenceNode();

protected:
  std::string             m_Name;
  eBlackBox_Node_Type   m_BlackBoxType = eBlackBox_Node_Type::None;

  PotentialScalar*        m_Potential;
  PotentialScalar*        m_NextPotential;
  eBlackBox_Property_Type m_PotentialType = eBlackBox_Property_Type::Calculate;

  ////////////////////
  // Quantity Types //
  ////////////////////
  QuantityScalar*         m_Quantity;
  QuantityScalar*         m_NextQuantity;
  QuantityScalar*         m_QuantityBaseline;
  eBlackBox_Property_Type m_QuantityType = eBlackBox_Property_Type::Calculate;

private:
  size_t                  m_CalculatorIndex;
  bool                    m_IsReferenceNode = false;
};
