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

  SEHemorrhage(Logger* logger=nullptr);
  virtual ~SEHemorrhage();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEHemorrhage& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eHemorrhage_Type GetType() const;
  virtual void SetType(eHemorrhage_Type t);

  virtual std::string GetCompartment() const;
  virtual void SetCompartment(const std::string& name);
  virtual bool HasCompartment() const;
  virtual void InvalidateCompartment();

  virtual bool HasFlowRate() const;
  virtual SEScalarVolumePerTime& GetFlowRate();
  virtual double GetFlowRate(const VolumePerTimeUnit& unit) const;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual bool HasTotalBloodLost() const;
  virtual SEScalarVolume& GetTotalBloodLost();
  virtual double GetTotalBloodLost(const VolumeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eHemorrhage_Type        m_Type;
  std::string             m_Compartment;
  SEScalarVolumePerTime*  m_FlowRate;
  SEScalar0To1*           m_Severity;
  SEScalarVolume*         m_TotalBloodLost;
};