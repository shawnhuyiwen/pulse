/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "properties/SEScalar.h"
#include "utils/GeneralMath.h"

// Declare template classes all Scalar types at the bottom

unsigned long long int SEScalar::NaN = 
  ((unsigned long long int)255 << (8*7)) + 
  ((unsigned long long int)(255-8) << (8*6)) + 
  ((unsigned long long int)255 << (8*5)) + 
  ((unsigned long long int)255 << (8*4)) +
  ((unsigned long long int)255 << (8*3)) + 
  (255 << (8*2)) + 
  (255 << (8*1)) + 
   255;

double SEScalar::dNaN()
{
  double d;
  *(reinterpret_cast<unsigned long long int *>(&d)) = NaN;
  return d;
}

// Opposite of isnan which can be slow
bool SEScalar::IsNumber(double d)
{
  if (d >= -std::numeric_limits<double>::max()&&
      d <=  std::numeric_limits<double>::max())
    return true;
  return false;
}

const NoUnit NoUnit::unitless;

SEScalar::SEScalar() : SEProperty()
{  
  Clear();
}

SEScalar::~SEScalar()
{
  Clear();
}

void SEScalar::Clear()
{
  SEProperty::Clear();
  m_readOnly = false;
  Invalidate();
}

bool SEScalar::Set(const SEScalar& s)
{
  if (dynamic_cast<const SEUnitScalar*>(&s) != nullptr)
    throw CommonDataModelException("Scalar is not of the proper type");
  if(!s.IsValid())
    return false;
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = s.m_value;
  m_isnan = false;
  m_isinf = false;
  if (!IsNumber(m_value))
  {
    m_isnan = (std::isnan(m_value)) ? true : false;
    m_isinf = (std::isinf(m_value)) ? true : false;
  }
  return true;
}

void SEScalar::Copy(const SEScalar& s)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = s.m_value;
  m_isnan = s.m_isnan;
  m_isinf = s.m_isinf;
}

void SEScalar::Invalidate()
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_isnan = true;
  m_isinf = false;
  *(reinterpret_cast<unsigned long long int *>(&m_value)) = NaN;
}

bool SEScalar::IsValid() const 
{
  if(m_isnan)
    return false;
  return true;
}

bool SEScalar::IsZero(double limit) const
{
  if (!IsValid())
    return false;
  return SEScalar::IsZero(m_value,limit);
}

bool SEScalar::IsPositive() const
{
  if (!IsValid())
    return false;
  return m_value>0;
}

bool SEScalar::IsNegative() const
{
  if (!IsValid())
    return false;
  return m_value<0;
}

void SEScalar::SetReadOnly(bool b)
{
  m_readOnly = b;
}
bool SEScalar::IsReadOnly() const
{
  return m_readOnly;
}

double SEScalar::GetValue() const
{
  if (!m_isnan)
    return m_value;
  throw CommonDataModelException("Value is NaN");
}

void SEScalar::SetValue(double d)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = d;
  m_isnan = false;
  m_isinf = false;
  if (!IsNumber(m_value))
  {
    m_isnan = (std::isnan(m_value)) ? true : false;
    m_isinf = (std::isinf(m_value)) ? true : false;
  }
}
void SEScalar::ForceValue(double d)
{
  m_value = d;
  m_isnan = false;
  m_isinf = false;
  if (!IsNumber(m_value))
  {
    m_isnan = (std::isnan(m_value)) ? true : false;
    m_isinf = (std::isinf(m_value)) ? true : false;
  }
}

double SEScalar::Increment(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    IncrementValue(s.GetValue());
  return m_value;
}

double SEScalar::IncrementValue(double d)
{
  if (!IsValid())
  {
    SetValue(d);
    return d;
  }
  SetValue(m_value+d);
  return m_value;
}


double SEScalar::Multiply(const SEScalar& s)
{
  if (!s.IsValid())
    Invalidate();
  else
    MultiplyValue(s.GetValue());
  return m_value;
}

double SEScalar::MultiplyValue(double d)
{
  if (!IsValid())
  {
    SetValue(d);
    return d;
  }
  SetValue(m_value * d);
  return m_value;
}

void SEScalar::Average(int cnt)
{
  if (!IsValid() || cnt == 0)
    return;
  m_value /= cnt;
}

bool SEScalar::Equals(const SEScalar& to) const
{
  if (m_isnan && to.m_isnan)
    return true;
  if (m_isnan || to.m_isnan)
    return false;
  if (m_isinf&&to.m_isinf)
    return true;
  if (m_isinf || to.m_isinf)
    return false;
  return std::abs(GeneralMath::PercentDifference(m_value, to.m_value)) <= 1e-15;
}

std::string SEScalar::ToString() const
{
  return cdm::to_string(m_value);
}                                      
void SEScalar::ToString(std::ostream &str) const
{
  str<<m_value<<std::flush;
}

bool SEScalar::IsValue(double target, double value)
{
  if (value<(target + 1e-10) && value>(target - 1e-10))
    return true;
  return false;
}

bool SEScalar::IsZero(double d, double limit)
{
  if (d<limit&&d>-limit)
    return true;
  return false;
}

//////////////////////
// SEScalarQuantity //
//////////////////////

template<typename Unit>
SEScalarQuantity<Unit>::SEScalarQuantity() : SEUnitScalar()
{
  Clear();
}

template<typename Unit>
SEScalarQuantity<Unit>::~SEScalarQuantity()
{
  this->Clear();
}

template<typename Unit>
void SEScalarQuantity<Unit>::Clear()
{
  SEScalar::Clear();
  m_unit = nullptr;
}

template<typename Unit>
void SEScalarQuantity<Unit>::Invalidate()
{
  SEScalar::Invalidate();
  m_unit = nullptr;
}

template<typename Unit>
bool SEScalarQuantity<Unit>::IsValid() const
{
  if (!SEScalar::IsValid())
    return false;
  if (m_unit == nullptr)
    return false;
  return true;
}

template<typename Unit>
bool SEScalarQuantity<Unit>::Set(const SEScalar& s)
{
  const SEScalarQuantity<Unit>* q = dynamic_cast<const SEScalarQuantity<Unit>*>(&s);
  if (q == nullptr)
    throw CommonDataModelException("Set method called with differnt scalar quantity type");
  return this->Set(*q);
}

template<typename Unit>
bool SEScalarQuantity<Unit>::Set(const SEScalarQuantity<Unit>& s)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  if (!s.IsValid())
    return false;
  m_value = s.m_value;
  m_isnan = false;
  m_isinf = false;
  m_unit = s.m_unit;
  if (!IsNumber(m_value))
  {
    m_isnan = (std::isnan(m_value)) ? true : false;
    m_isinf = (std::isinf(m_value)) ? true : false;
  }
  return true;
}

template<typename Unit>
void SEScalarQuantity<Unit>::Copy(const SEScalar& s)
{
  const SEScalarQuantity<Unit>* q = dynamic_cast<const SEScalarQuantity<Unit>*>(&s);
  if (q == nullptr)
    throw CommonDataModelException("Set method called with differnt scalar quantity type");
  this->Copy(*q);
}

template<typename Unit>
void SEScalarQuantity<Unit>::Copy(const SEScalarQuantity<Unit>& s)
{
  SEScalar::Copy(s);
  m_unit = s.m_unit;
}

template<typename Unit>
double SEScalarQuantity<Unit>::GetValue(const Unit& unit) const
{
  if (m_isnan)
    throw CommonDataModelException("Value is NaN");
  if (m_isinf)
    return m_value;
  if (m_value == 0)
    return m_value;
  if (m_unit == &unit)
    return m_value;
  return Convert(m_value, *m_unit, unit);
}

template<typename Unit>
double SEScalarQuantity<Unit>::GetValue(const CCompoundUnit& unit) const
{
  const Unit* u = dynamic_cast<const Unit*>(&unit);
  if (u == nullptr)
    throw CommonDataModelException("Provided unit is not of proper quantity type");
  return this->GetValue(*u);
}

template<typename Unit>
bool SEScalarQuantity<Unit>::HasUnit() const
{
  return m_unit != nullptr;
}
template<typename Unit>
const Unit* SEScalarQuantity<Unit>::GetUnit() const
{
  return m_unit;
}

template<typename Unit>
void SEScalarQuantity<Unit>::SetValue(double d, const Unit& unit)
{
  if (m_readOnly)
    throw CommonDataModelException("Scalar is marked read-only");
  m_value = d;
  m_isnan = false;
  m_isinf = false;
  m_unit = &unit;
  if (!IsNumber(m_value))
  {
    m_isnan = (std::isnan(m_value)) ? true : false;
    m_isinf = (std::isinf(m_value)) ? true : false;
  }
}
template<typename Unit>
void SEScalarQuantity<Unit>::ForceValue(double d, const Unit& unit)
{
  m_value = d;
  m_isnan = false;
  m_isinf = false;
  m_unit = &unit;
  if (!IsNumber(m_value))
  {
    m_isnan = (std::isnan(m_value)) ? true : false;
    m_isinf = (std::isinf(m_value)) ? true : false;
  }
  m_unit = &unit;
}

template<typename Unit>
void SEScalarQuantity<Unit>::SetValue(double d, const CCompoundUnit& unit)
{
  const Unit* u = dynamic_cast<const Unit*>(&unit);
  if (u == nullptr)
    throw CommonDataModelException("Provided unit is not of proper quantity type");
  this->SetValue(d, *u);
}
template<typename Unit>
void SEScalarQuantity<Unit>::ForceValue(double d, const CCompoundUnit& unit)
{
  const Unit* u = dynamic_cast<const Unit*>(&unit);
  if (u == nullptr)
    throw CommonDataModelException("Provided unit is not of proper quantity type");
  this->ForceValue(d, *u);
}

template<typename Unit>
double SEScalarQuantity<Unit>::Increment(const SEScalarQuantity<Unit>& s)
{
  if (!s.IsValid())
    this->Invalidate();
  else
    return this->IncrementValue(s.m_value, *s.m_unit);
  return m_value;
}

template<typename Unit>
double SEScalarQuantity<Unit>::IncrementValue(double d, const Unit& unit)
{
  if (!IsValid())
  {
    this->SetValue(d, unit);
    return d;
  }
  this->SetValue(m_value + Convert(d, unit, *m_unit), *m_unit);
  return Convert(m_value, *m_unit, unit);
}

template<typename Unit>
double SEScalarQuantity<Unit>::IncrementValue(double d, const CCompoundUnit& unit)
{
  const Unit* u = dynamic_cast<const Unit*>(&unit);
  if (u == nullptr)
    throw CommonDataModelException("Provided unit is not of proper quantity type");
  return this->IncrementValue(d, *u);
}

template<typename Unit>
double SEScalarQuantity<Unit>::Multiply(const SEScalarQuantity<Unit>& s)
{
  if (!s.IsValid())
    this->Invalidate();
  else
    return this->MultiplyValue(s.m_value, *s.m_unit);
  return m_value;
}

template<typename Unit>
double SEScalarQuantity<Unit>::MultiplyValue(double d, const Unit& unit)
{
  if (!IsValid())
  {
    this->SetValue(d, unit);
    return d;
  }
  this->SetValue(m_value * Convert(d, unit, *m_unit), *m_unit);
  return Convert(m_value, *m_unit, unit);
}

template<typename Unit>
double SEScalarQuantity<Unit>::MultiplyValue(double d, const CCompoundUnit& unit)
{
  const Unit* u = dynamic_cast<const Unit*>(&unit);
  if (u == nullptr)
    throw CommonDataModelException("Provided unit is not of proper quantity type");
  return this->MultiplyValue(d, *u);
}

template<typename Unit>
bool SEScalarQuantity<Unit>::Equals(const SEScalarQuantity<Unit>& to) const
{
  if (m_unit == nullptr)
    return false;
  if (m_isnan && to.m_isnan)
    return true;
  if (m_isnan || to.m_isnan)
    return false;
  if (m_isinf && to.m_isinf)
    return true;
  if (m_isinf || to.m_isinf)
    return false;
  double t = to.GetValue(*m_unit);
  return std::abs(GeneralMath::PercentDifference(m_value, t)) <= 1e-15;
}

template<typename Unit>
const CCompoundUnit* SEScalarQuantity<Unit>::GetCompoundUnit(const std::string& unit) const
{
  return &Unit::GetCompoundUnit(unit);
}

template<typename Unit>
std::string SEScalarQuantity<Unit>::ToString() const
{
  if (m_isnan || m_isinf)
    return cdm::to_string(m_value);
  else
    return cdm::to_string(m_value) + "(" + m_unit->GetString() + ")";
}
template<typename Unit>
void SEScalarQuantity<Unit>::ToString(std::ostream& str) const
{
  if (m_isnan || m_isinf)
    str << m_value << std::flush;
  else
    str << m_value << "(" << *m_unit << ")" << std::flush;
}

SEGenericScalar::SEGenericScalar(Logger* logger) : Loggable(logger)
{
  m_Scalar = nullptr;
  m_UnitScalar = nullptr;
}

bool SEGenericScalar::HasScalar() const
{
  return m_Scalar != nullptr;
}
void SEGenericScalar::SetScalar(const SEScalar& s)
{
  m_Scalar = &s;
  m_UnitScalar = dynamic_cast<const SEUnitScalar*>(m_Scalar);
}

bool SEGenericScalar::IsValid() const
{
  return m_UnitScalar == nullptr ? m_Scalar->IsValid() : m_UnitScalar->IsValid();
}

bool SEGenericScalar::IsInfinity() const
{
  return m_Scalar->IsInfinity();
}

bool SEGenericScalar::HasUnit() const
{
  return m_UnitScalar != nullptr && m_UnitScalar->HasUnit();
}
const CCompoundUnit* SEGenericScalar::GetUnit() const
{
  if (m_UnitScalar == nullptr)
    return nullptr;
  return m_UnitScalar->GetUnit();
}
const CCompoundUnit* SEGenericScalar::GetCompoundUnit(const std::string& unit) const
{
  if (m_UnitScalar == nullptr)
    return nullptr;
  return m_UnitScalar->GetCompoundUnit(unit);
}
bool SEGenericScalar::IsValidUnit(const CCompoundUnit& unit) const
{
  if (m_UnitScalar == nullptr)
    return false;
  return false;
}

double SEGenericScalar::GetValue() const
{
  return m_Scalar->GetValue();
}
double SEGenericScalar::GetValue(const CCompoundUnit& unit) const
{
  if (m_UnitScalar == nullptr)
    return SEScalar::dNaN();
  return m_UnitScalar->GetValue(unit);
}
#include "properties/SEScalarAmount.h"
template class SEScalarQuantity<AmountUnit>;
#include "properties/SEScalarAmountPerMass.h"
template class SEScalarQuantity<AmountPerMassUnit>;
#include "properties/SEScalarAmountPerTime.h"
template class SEScalarQuantity<AmountPerTimeUnit>;
#include "properties/SEScalarAmountPerVolume.h"
template class SEScalarQuantity<AmountPerVolumeUnit>;
#include "properties/SEScalarArea.h"
template class SEScalarQuantity<AreaUnit>;
#include "properties/SEScalarAreaPerTimePressure.h"
template class SEScalarQuantity<AreaPerTimePressureUnit>;
#include "properties/SEScalarElectricCapacitance.h"
template class SEScalarQuantity<ElectricCapacitanceUnit>;
#include "properties/SEScalarElectricCharge.h"
template class SEScalarQuantity<ElectricChargeUnit>;
#include "properties/SEScalarElectricCurrent.h"
template class SEScalarQuantity<ElectricCurrentUnit>;
#include "properties/SEScalarElectricInductance.h"
template class SEScalarQuantity<ElectricInductanceUnit>;
#include "properties/SEScalarElectricPotential.h"
template class SEScalarQuantity<ElectricPotentialUnit>;
#include "properties/SEScalarElectricResistance.h"
template class SEScalarQuantity<ElectricResistanceUnit>;
#include "properties/SEScalarEnergy.h"
template class SEScalarQuantity<EnergyUnit>;
#include "properties/SEScalarEnergyPerAmount.h"
template class SEScalarQuantity<EnergyPerAmountUnit>;
#include "properties/SEScalarEnergyPerMass.h"
template class SEScalarQuantity<EnergyPerMassUnit>;
#include "properties/SEScalarForce.h"
template class SEScalarQuantity<ForceUnit>;
#include "properties/SEScalarFrequency.h"
template class SEScalarQuantity<FrequencyUnit>;
#include "properties/SEScalarHeatCapacitance.h"
template class SEScalarQuantity<HeatCapacitanceUnit>;
#include "properties/SEScalarHeatCapacitancePerAmount.h"
template class SEScalarQuantity<HeatCapacitancePerAmountUnit>;
#include "properties/SEScalarHeatCapacitancePerMass.h"
template class SEScalarQuantity<HeatCapacitancePerMassUnit>;
#include "properties/SEScalarHeatConductance.h"
template class SEScalarQuantity<HeatConductanceUnit>;
#include "properties/SEScalarHeatConductancePerArea.h"
template class SEScalarQuantity<HeatConductancePerAreaUnit>;
#include "properties/SEScalarHeatResistance.h"
template class SEScalarQuantity<HeatResistanceUnit>;
#include "properties/SEScalarHeatResistanceArea.h"
template class SEScalarQuantity<HeatResistanceAreaUnit>;
#include "properties/SEScalarHeatInductance.h"
template class SEScalarQuantity<HeatInductanceUnit>;
#include "properties/SEScalarInversePressure.h"
template class SEScalarQuantity<InversePressureUnit>;
#include "properties/SEScalarInverseVolume.h"
template class SEScalarQuantity<InverseVolumeUnit>;
#include "properties/SEScalarLength.h"
template class SEScalarQuantity<LengthUnit>;
#include "properties/SEScalarLengthPerTime.h"
template class SEScalarQuantity<LengthPerTimeUnit>;
#include "properties/SEScalarLengthPerTimePressure.h"
template class SEScalarQuantity<LengthPerTimePressureUnit>;
#include "properties/SEScalarMass.h"
template class SEScalarQuantity<MassUnit>;
#include "properties/SEScalarMassPerAmount.h"
template class SEScalarQuantity<MassPerAmountUnit>;
#include "properties/SEScalarMassPerAreaTime.h"
template class SEScalarQuantity<MassPerAreaTimeUnit>;
#include "properties/SEScalarMassPerMass.h"
template class SEScalarQuantity<MassPerMassUnit>;
#include "properties/SEScalarMassPerTime.h"
template class SEScalarQuantity<MassPerTimeUnit>;
#include "properties/SEScalarMassPerVolume.h"
template class SEScalarQuantity<MassPerVolumeUnit>;
#include "properties/SEScalarOsmolality.h"
template class SEScalarQuantity<OsmolalityUnit>;
#include "properties/SEScalarOsmolarity.h"
template class SEScalarQuantity<OsmolarityUnit>;
#include "properties/SEScalarPower.h"
template class SEScalarQuantity<PowerUnit>;
#include "properties/SEScalarPowerPerAreaTemperatureToTheFourth.h"
template class SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>;
#include "properties/SEScalarPressure.h"
template class SEScalarQuantity<PressureUnit>;
#include "properties/SEScalarPressurePerVolume.h"
template class SEScalarQuantity<PressurePerVolumeUnit>;
#include "properties/SEScalarPressureTimePerVolume.h"
template class SEScalarQuantity<PressureTimePerVolumeUnit>;
#include "properties/SEScalarPressureTimePerVolumeArea.h"
template class SEScalarQuantity<PressureTimePerVolumeAreaUnit>;
#include "properties/SEScalarPressureTimePerArea.h"
template class SEScalarQuantity<PressureTimePerAreaUnit>;
#include "properties/SEScalarPressureTimeSquaredPerVolume.h"
template class SEScalarQuantity<PressureTimeSquaredPerVolumeUnit>;
#include "properties/SEScalarTime.h"
template class SEScalarQuantity<TimeUnit>;
#include "properties/SEScalarTemperature.h"
template class SEScalarQuantity<TemperatureUnit>;
#include "properties/SEScalarVolume.h"
template class SEScalarQuantity<VolumeUnit>;
#include "properties/SEScalarVolumePerPressure.h"
template class SEScalarQuantity<VolumePerPressureUnit>;
#include "properties/SEScalarVolumePerTime.h"
template class SEScalarQuantity<VolumePerTimeUnit>;
#include "properties/SEScalarVolumePerTimeArea.h"
template class SEScalarQuantity<VolumePerTimeAreaUnit>;
#include "properties/SEScalarVolumePerTimePressureArea.h"
template class SEScalarQuantity<VolumePerTimePressureAreaUnit>;
#include "properties/SEScalarVolumePerTimeMass.h"
template class SEScalarQuantity<VolumePerTimeMassUnit>;
#include "properties/SEScalarVolumePerTimePressure.h"
template class SEScalarQuantity<VolumePerTimePressureUnit>;
