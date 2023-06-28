/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SETubeThoracostomy : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SETubeThoracostomy(Logger* logger=nullptr);
  virtual ~SETubeThoracostomy();

  static constexpr char const* Name = "Needle Decompression";
  virtual std::string GetName() const { return Name; }

  virtual void Clear(); //clear memory
  virtual void Copy(const SETubeThoracostomy& src, bool /*preserveState*/=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual eSide GetSide() const;
  virtual void SetSide(eSide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual bool HasFlowRate() const;
  virtual SEScalarVolumePerTime& GetFlowRate();
  virtual double GetFlowRate(const VolumePerTimeUnit& unit) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  eSide                   m_Side;
  SEScalarVolumePerTime*  m_FlowRate;
};
