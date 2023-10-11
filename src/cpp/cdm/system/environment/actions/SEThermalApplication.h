/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/environment/actions/SEEnvironmentAction.h"
class SEActiveConditioning;
class SEAppliedTemperature;

class CDM_DECL SEThermalApplication : public SEEnvironmentAction
{
  friend class PBEnvironmentAction;//friend the serialization class
public:

  SEThermalApplication(Logger* logger=nullptr);
  virtual ~SEThermalApplication();

  static constexpr char const* Name = "Thermal Application";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEThermalApplication& src, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasActiveHeating() const;
  virtual SEActiveConditioning& GetActiveHeating();
  virtual const SEActiveConditioning* GetActiveHeating() const;
  virtual void RemoveActiveHeating();

  virtual bool HasActiveCooling() const;
  virtual SEActiveConditioning& GetActiveCooling();
  virtual const SEActiveConditioning* GetActiveCooling() const;
  virtual void RemoveActiveCooling();

  virtual bool HasAppliedTemperature() const;
  virtual SEAppliedTemperature& GetAppliedTemperature();
  virtual const SEAppliedTemperature* GetAppliedTemperature() const;
  virtual void RemoveAppliedTemperature();

  const SEScalar* GetScalar(const std::string& name) override;

protected:
  bool                   m_ClearContents;
  SEActiveConditioning*  m_ActiveHeating;
  SEActiveConditioning*  m_ActiveCooling;
  SEAppliedTemperature*  m_AppliedTemperature;
};