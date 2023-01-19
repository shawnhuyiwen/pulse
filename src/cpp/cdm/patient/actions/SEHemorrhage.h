/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eHemorrhage_Compartment
{
  None = 0,
  Aorta = 1,
  Brain = 2,
  Muscle = 3,
  LargeIntestine = 4,
  LeftArm = 5,
  LeftKidney = 6,
  LeftLeg = 7,
  Liver = 8,
  RightArm = 9,
  RightKidney = 10,
  RightLeg = 11,
  Skin = 12,
  SmallIntestine = 13,
  Splanchnic = 14,
  Spleen = 15,
  VenaCava = 16
};
extern const std::string& eHemorrhage_Compartment_Name(eHemorrhage_Compartment c);
extern eHemorrhage_Compartment eHemorrhage_Compartment_Parse(const std::string& s);

enum class eHemorrhage_Type { External = 0, Internal };
extern const std::string& eHemorrhage_Type_Name(eHemorrhage_Type t);

class CDM_DECL SEHemorrhage : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:
  SEHemorrhage(Logger* logger=nullptr);
  virtual ~SEHemorrhage();

  static constexpr char const* Name = "Hemorrhage";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEHemorrhage& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eHemorrhage_Type GetType() { return m_Type; }
  virtual void SetType(eHemorrhage_Type t) { m_Type = t; }

  virtual bool HasCompartment() const;
  virtual std::string GetCompartmentName() const;
  virtual eHemorrhage_Compartment GetCompartment() const;
  virtual void SetCompartment(eHemorrhage_Compartment cmpt);

  virtual bool HasFlowRate() const;
  virtual SEScalarVolumePerTime& GetFlowRate();
  virtual double GetFlowRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual bool HasTotalBloodLost() const;
  virtual SEScalarVolume& GetTotalBloodLost();
  virtual double GetTotalBloodLost(const VolumeUnit& unit) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eHemorrhage_Type        m_Type;
  std::string             m_CompartmentName;
  eHemorrhage_Compartment m_Compartment;
  SEScalarVolumePerTime*  m_FlowRate;
  SEScalar0To1*           m_Severity;
  SEScalarVolume*         m_TotalBloodLost;
};
