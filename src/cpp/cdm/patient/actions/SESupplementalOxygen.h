/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
class SESubstance;

// Keep enums in sync with appropriate schema/cdm/PatientActionEnums.proto file !!
enum class eSupplementalOxygen_Device { None = 0, NasalCannula, SimpleMask, NonRebreatherMask };
extern const std::string& eSupplementalOxygen_Device_Name(eSupplementalOxygen_Device e);

class CDM_DECL SESupplementalOxygen : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SESupplementalOxygen(Logger* logger=nullptr);
  virtual ~SESupplementalOxygen();

  virtual void Clear(); //clear memory
  virtual void Copy(const SESupplementalOxygen& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eSupplementalOxygen_Device GetDevice() const;
  virtual void SetDevice(eSupplementalOxygen_Device name);

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eSupplementalOxygen_Device              m_Device;
  SEScalarVolumePerTime*                  m_Flow;
  SEScalarVolume*                         m_Volume;
};