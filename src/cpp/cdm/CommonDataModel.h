/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
 
#pragma once

#ifdef SHARED_CDM
  #if defined (__clang__)
      #define CDM_DECL
  #elif defined(__gnu_linux__)
      #define CDM_DECL __attribute__ ((visibility ("default")))
  #else
      #ifdef SHARED_CDM
        #define CDM_DECL __declspec(dllexport)
      #else
        #define CDM_DECL __declspec(dllimport)
      #endif
  #endif
#else
  #define CDM_DECL
#endif

#if (0)
#define DEBUGOUT(x) x
#else
#define DEBUGOUT(x) 
#endif

#define CDM_BIND pulse::cdm::bind
#define CDM_BIND_DECL(type) \
  namespace pulse { namespace cdm { namespace bind { class type; }}}
#define CDM_BIND_DECL2(type) \
  class SE##type;\
  namespace pulse { namespace cdm { namespace bind { class type##Data; }}}

#if defined(_MSC_VER)
  #define PUSH_PROTO_WARNINGS() \
    __pragma(pack(push)) \
    __pragma(warning(disable:4127)) \
    __pragma(warning(disable:4244)) \
    __pragma(warning(disable:4267))
  #define POP_PROTO_WARNINGS() __pragma(pack(pop))
#else
  #define PUSH_PROTO_WARNINGS() \
    _Pragma("pack(push)") \
    _Pragma("warning(disable:4127)") \
    _Pragma("warning(disable:4267)")
  #define POP_PROTO_WARNINGS() _Pragma("pack(pop)")
#endif


#include <memory>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <string>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <dirent.h>
#endif

#include <math.h>
#include <vector>
#include <stack>
#include <map>
#include <set>

#if defined(_MSC_VER)
  #include <unordered_map>
  using namespace stdext;
#else//if (__GCC__) || (__GNUC__)
  #include <unordered_map>
#endif

 /*
// Take from : https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers/blob/master/README.md
namespace xsd
{
  template<typename T>
  struct Serializable
  {
    using base_type = T;

    virtual ~Serializable() = default;

    virtual bool Load(const T& in) = 0;
  protected:
    virtual T* Unload() const
    {
      using base_type = typename T::base_type;
      static_assert(std::is_base_of<base_type, T>::value, "T object has to derived from T::base_type");
      auto ptr = static_cast<const base_type&>(object).NewBindType();
      ptr->Unload(static_cast<T*>(ptr));
      return std::unique_ptr<T>(static_cast<T*>(ptr));
    }
    virtual T* NewBindType() const = 0;
    virtual void Unload(T& object) const = 0;
  };
}
*/

/*
template class __declspec(dllexport) std::basic_string<char, 
                                                      std::char_traits<char>, 
                                                      std::allocator<char> >;

template class __declspec(dllexport) std::basic_stringstream<char, 
                                                            std::char_traits<char>, 
                                                            std::allocator<char> >;

template class __declspec(dllexport) std::vector<std::string>;
*/

#if defined(_MSC_VER)
  // Disabling the waring about STL classes used have 
  // to have a dll interface to be used by clients
  // From what I have read STL is compiler dependent
  // But since we are releasing source, you can build
  // the project necessary to ensure proper linkage
  // If anyone else has opinions on this, let us know
  // kitware@kitware.com
  #pragma warning(disable : 4251)
  #pragma warning(disable:4100) // unreferenced formal parameter (intentional in base classes)
  #pragma warning(disable:4996) // Deprecation
  #pragma warning(disable:4505) // unreferenced local function has been removed (dirent)
  #pragma warning(disable:4503)
#endif

//Utilities
enum SerializationFormat { JSON = 0, BINARY };

struct CommonDataModelException : public std::runtime_error
{
  CommonDataModelException()
    : std::runtime_error( "Common Data Model Error" ) {}

  CommonDataModelException(const std::string& _Message)
    : std::runtime_error(_Message) {}
};
// Basics
#include "Macros.h"
#include "utils/Logger.h"


// General Enums
// Keep enums in sync with appropriate schema/cdm/Enums.proto file !!
//
enum class eSide { NullSide = 0, Left, Right };
extern const std::string& eSide_Name(eSide m);

enum class eGate { NullGate = 0, Open, Closed };
extern const std::string& eGate_Name(eGate m);

enum class eSwitch { NullSwitch = 0, Off, On };
extern const std::string& eSwitch_Name(eSwitch m);

enum class eCharge { NullCharge = 0, Negative, Neutral, Positive };
extern const std::string& eCharge_Name(eCharge m);

//
// End General Enum

// IF YOU ADD A NEW SCALAR TYPE
// PLEASE UPDATE THE METHOD IN SEProperty.cpp
// ALSO DECLARE A TEMPLATE CLASS AT THE BOTTOM OF SESCALAR.CPP

class CCompoundUnit;

class SERunningAverage;
class SEScalar; class SEUnitScalar; class SEGenericScalar; class NoUnit;
class SEScalar0To1; 
class SEScalarNegative1To1;
class SEScalarAmount; class AmountUnit;
class SEScalarAmountPerMass; class AmountPerMassUnit;
class SEScalarAmountPerTime; class AmountPerTimeUnit;
class SEScalarAmountPerVolume; class AmountPerVolumeUnit;
class SEScalarArea; class AreaUnit;
class SEScalarAreaPerTimePressure; class AreaPerTimePressureUnit;
class SEScalarElectricCapacitance; class ElectricCapacitanceUnit;
class SEScalarElectricCharge; class ElectricChargeUnit;
class SEScalarElectricCurrent; class ElectricCurrentUnit;
class SEScalarElectricInductance; class ElectricInductanceUnit;
class SEScalarElectricPotential; class ElectricPotentialUnit;
class SEScalarElectricResistance; class ElectricResistanceUnit;
class SEScalarEnergy; class EnergyUnit;
class SEScalarEnergyPerAmount; class EnergyPerAmountUnit;
class SEScalarEnergyPerMass; class EnergyPerMassUnit;
class SEScalarForce; class ForceUnit;
class SEScalar0To1;
class SEScalarFrequency; class FrequencyUnit;
class SEScalarHeatCapacitance; class HeatCapacitanceUnit;
class SEScalarHeatCapacitancePerAmount; class HeatCapacitancePerAmountUnit;
class SEScalarHeatCapacitancePerMass; class HeatCapacitancePerMassUnit;
class SEScalarHeatConductance; class HeatConductanceUnit;
class SEScalarHeatConductancePerArea; class HeatConductancePerAreaUnit;
class SEScalarHeatResistance; class HeatResistanceUnit;
class SEScalarHeatResistanceArea; class HeatResistanceAreaUnit;
class SEScalarHeatInductance; class HeatInductanceUnit;
class SEScalarInversePressure; class InversePressureUnit; 
class SEScalarInverseVolume; class InverseVolumeUnit;
class SEScalarLength; class LengthUnit;
class SEScalarLengthPerTime; class LengthPerTimeUnit;
class SEScalarLengthPerTimePressure; class LengthPerTimePressureUnit;
class SEScalarMass; class MassUnit;
class SEScalarMassPerAmount; class MassPerAmountUnit;
class SEScalarMassPerAreaTime; class MassPerAreaTimeUnit;
class SEScalarMassPerMass; class MassPerMassUnit;
class SEScalarMassPerTime; class MassPerTimeUnit;
class SEScalarMassPerVolume; class MassPerVolumeUnit;
class SEScalarOsmolality; class OsmolalityUnit;
class SEScalarOsmolarity; class OsmolarityUnit;
class SEScalarPower; class PowerUnit;
class SEScalarPowerPerAreaTemperatureToTheFourth; class PowerPerAreaTemperatureToTheFourthUnit;
class SEScalarPressure; class PressureUnit;
class SEScalarPressurePerVolume; class PressurePerVolumeUnit;
class SEScalarPressureTimePerVolume; class PressureTimePerVolumeUnit;
class SEScalarPressureTimePerVolumeArea; class PressureTimePerVolumeAreaUnit;
class SEScalarPressureTimePerArea; class PressureTimePerAreaUnit;
class SEScalarPressureTimeSquaredPerVolume; class PressureTimeSquaredPerVolumeUnit;
class SEScalarTime; class TimeUnit;
class SEScalarTemperature; class TemperatureUnit;
class SEScalarVolume; class VolumeUnit;
class SEScalarVolumePerPressure; class VolumePerPressureUnit;
class SEScalarVolumePerTime; class VolumePerTimeUnit;
class SEScalarVolumePerTimeArea; class VolumePerTimeAreaUnit;
class SEScalarVolumePerTimePressureArea; class VolumePerTimePressureAreaUnit;
class SEScalarVolumePerTimeMass; class VolumePerTimeMassUnit;
class SEScalarVolumePerTimePressure; class VolumePerTimePressureUnit;

class SEFunction;
class SEFunctionVolumeVsTime;
class SEFunctionElectricPotentialVsTime;

class SEHistogram;
class SEHistogramFractionVsLength;
