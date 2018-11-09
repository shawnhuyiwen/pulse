/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eHemorrhage_Type { External = 0, Internal };
extern const std::string& eHemorrhage_Type_Name(eHemorrhage_Type m);

class CDM_DECL SEHemorrhage : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEHemorrhage();
  virtual ~SEHemorrhage();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEHemorrhage& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetCompartment() const;
  virtual void SetCompartment(const std::string& name);
  virtual bool HasCompartment() const;
  virtual void InvalidateCompartment();

  virtual bool HasRate() const;
  virtual SEScalarVolumePerTime& GetRate();
  virtual double GetRate(const VolumePerTimeUnit& unit) const;

  virtual eHemorrhage_Type GetType() const;
  virtual void SetType(eHemorrhage_Type t);

  virtual void ToString(std::ostream &str) const;

protected:
  std::string             m_Compartment;
  SEScalarVolumePerTime*  m_Rate;
  eHemorrhage_Type        m_Type;
};