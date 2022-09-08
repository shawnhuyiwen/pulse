/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eHemorrhage_Type { External = 0, Internal };
extern const std::string& eHemorrhage_Type_Name(eHemorrhage_Type m);

class CDM_DECL SEHemorrhage : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  // Convient 'enum' style class for often used compartments
  // Noted, External Hemorrhages are not limited to these compartments
  struct ExternalCompartment
  {
    ExternalCompartment(const std::string& v);
    const std::string value;

    static const ExternalCompartment RightLeg;
    static const ExternalCompartment LeftLeg;
    static const ExternalCompartment RightArm;
    static const ExternalCompartment LeftArm;
    static const ExternalCompartment Skin;
    static const ExternalCompartment Muscle;
    static const ExternalCompartment Brain;
    static const ExternalCompartment LeftKidney;
    static const ExternalCompartment RightKidney;
    static const ExternalCompartment Liver;
    static const ExternalCompartment Spleen;
    static const ExternalCompartment Splanchnic;
    static const ExternalCompartment SmallIntestine;
    static const ExternalCompartment LargeIntestine;
    static const ExternalCompartment Aorta;
    static const ExternalCompartment VenaCava;
  };

  struct InternalCompartment
  {
    InternalCompartment(const std::string&);
    const std::string value;

    static const InternalCompartment LeftKidney;
    static const InternalCompartment RightKidney;
    static const InternalCompartment Liver;
    static const InternalCompartment Spleen;
    static const InternalCompartment Splanchnic;
    static const InternalCompartment SmallIntestine;
    static const InternalCompartment LargeIntestine;
    static const InternalCompartment Aorta;
    static const InternalCompartment VenaCava;
  };

  SEHemorrhage(Logger* logger=nullptr);
  virtual ~SEHemorrhage();

  static constexpr char const* Name = "Hemorrhage";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SEHemorrhage& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eHemorrhage_Type GetType() const;
  virtual void SetType(eHemorrhage_Type t);

  virtual std::string GetCompartment() const;
  virtual void SetCompartment(const std::string& name);
  virtual bool HasCompartment() const;
  virtual void InvalidateCompartment();

  virtual void SetExternal(const ExternalCompartment& c);
  virtual void SetInternal(const InternalCompartment& c);

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
  std::string             m_Compartment;
  SEScalarVolumePerTime*  m_FlowRate;
  SEScalar0To1*           m_Severity;
  SEScalarVolume*         m_TotalBloodLost;
};