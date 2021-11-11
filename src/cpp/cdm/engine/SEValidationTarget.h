/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEDataRequest.h"
#include "cdm/engine/SEEventManager.h"

enum class eValidationTargetType { Mean, Min, Max };

class SEValidationTarget : public SEDataRequest
{
  friend class PBEngine;//friend the serialization class
  friend class SEDataRequestManager;
protected:
  SEValidationTarget(const SEValidationTarget& vt);
  SEValidationTarget(eValidationTargetType t, eDataRequest_Category category, const SEDecimalFormat* dfault = nullptr);
public:
  virtual ~SEValidationTarget() = default;

  virtual void Clear() override;

  eValidationTargetType GetType() const;

  double GetRangeMin() const;
  void   SetRangeMin(double min);

  double GetRangeMax() const;
  void   SetRangeMax(double max);

  double GetValue() const { return m_TypeValue; }

  std::vector<double>& GetData();

  double GetError() const;
  bool ComputeError();

protected:
  eValidationTargetType m_Type;
  double m_RangeMin=SEScalar::dNaN();
  double m_RangeMax=SEScalar::dNaN();
  double m_TypeValue=SEScalar::dNaN();
  double m_Error=SEScalar::dNaN();

  // Not serializing
  std::vector<double> m_Data;
};
