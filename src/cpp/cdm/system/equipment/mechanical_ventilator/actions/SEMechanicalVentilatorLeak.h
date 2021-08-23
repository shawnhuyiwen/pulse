/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/mechanical_ventilator/actions/SEMechanicalVentilatorAction.h"
class SEMechanicalVentilator;
class SESubstanceManager;

class CDM_DECL SEMechanicalVentilatorLeak : public SEMechanicalVentilatorAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEMechanicalVentilatorLeak(Logger* logger = nullptr);
  virtual ~SEMechanicalVentilatorLeak();

  virtual void Clear();
  virtual void Copy(const SEMechanicalVentilatorLeak& src, bool /*preserveState*/ = false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  virtual void ToString(std::ostream& str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  SEScalar0To1* m_Severity;
};
