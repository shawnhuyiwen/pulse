/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "properties/SEScalarElectricCharge.h"
#include "properties/SEScalarElectricPotential.h"
#include "properties/SEScalarPressure.h"
#include "properties/SEScalarVolume.h"
#include "properties/SEScalarTemperature.h"
#include "properties/SEScalarEnergy.h"

#define CIRCUIT_NODE_TEMPLATE typename BlackBoxType, typename PotentialScalar, typename QuantityScalar
#define CIRCUIT_NODE_TYPES BlackBoxType,PotentialScalar,QuantityScalar
#define ELECTRICAL_CIRCUIT_NODE SEElectricalBlackBox, SEScalarElectricPotential, SEScalarElectricCharge
#define FLUID_CIRCUIT_NODE SEFluidBlackBox, SEScalarPressure, SEScalarVolume
#define THERMAL_CIRCUIT_NODE SEThermalBlackBox, SEScalarTemperature, SEScalarEnergy

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

  virtual bool HasPotential() const;
  virtual PotentialScalar& GetPotential();
  virtual bool HasNextPotential() const;
  virtual PotentialScalar& GetNextPotential();

  virtual bool HasQuantity() const;
  virtual QuantityScalar& GetQuantity();
  virtual bool HasNextQuantity() const;
  virtual QuantityScalar& GetNextQuantity();
  virtual bool HasQuantityBaseline() const;
  virtual QuantityScalar& GetQuantityBaseline();

  virtual bool HasBlackBox() const { return m_BlackBox != nullptr; }
  virtual BlackBoxType* GetBlackBox() const { return m_BlackBox; }
  virtual void SetBlackBox(BlackBoxType* bb) { m_BlackBox = bb; }

  void SetCalculatorIndex(const size_t index);
  size_t GetCalculatorIndex() const;

  bool IsReferenceNode() const;
  void SetAsReferenceNode();
  void RemoveAsReferenceNode();

protected:
  std::string            m_Name;

  PotentialScalar*       m_Potential;
  PotentialScalar*       m_NextPotential;

  ////////////////////
  // Quantity Types //
  ////////////////////
  QuantityScalar*         m_Quantity;
  QuantityScalar*         m_NextQuantity;
  QuantityScalar*         m_QuantityBaseline;

  BlackBoxType* m_BlackBox = nullptr;

private:
  size_t                  m_CalculatorIndex;
  bool                    m_IsReferenceNode = false;
};