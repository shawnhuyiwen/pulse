/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEProperty.h"

class CDM_DECL SETemporalInterpolator : public SEProperty
{
  friend class PBProperty;//friend the serialization class
public:

  void Invalidate() override;
  bool IsValid() const override;

  double GetCurrent() const { return m_Current; }

  void Set(double d);

  void SetTarget(double target, double period_s);// initial will be set to current

  void SetTransition(double initial, double target, double period_s);

  bool Transition(double dt_s);

protected:
  bool   m_Complete = false;
  bool   m_PositiveTransition = true;
  double m_Period_s;
  double m_Current = 1;  // The Current Value
  double m_Target = 1;   // The Target Value
  double m_Initial = 1;  // The Initial Value
};
