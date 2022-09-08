/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eIntubation_Type { Off = 0, Esophageal, LeftMainstem, RightMainstem, Tracheal,
  Oropharyngeal, Nasopharyngeal };
extern const std::string& eIntubation_Type_Name(eIntubation_Type m);

class CDM_DECL SEIntubation : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEIntubation(Logger* logger=nullptr);
  virtual ~SEIntubation();

  static constexpr char const* Name = "Intubation";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEIntubation& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eIntubation_Type GetType() const;
  virtual void SetType(eIntubation_Type t);

  virtual bool HasAirwayResistance() const;
  virtual SEScalarPressureTimePerVolume& GetAirwayResistance();
  virtual double GetAirwayResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eIntubation_Type               m_Type;
  SEScalarPressureTimePerVolume* m_AirwayResistance;
  SEScalar0To1*                  m_Severity;
};
