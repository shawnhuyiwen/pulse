/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEDataRequest.h"
#include "cdm/engine/SEEventManager.h"

enum class eValidationTargetType { Min, Max, Mean };

class SEValidationTarget : public Loggable
{
public:
  SEValidationTarget(Logger* logger = nullptr);
  ~SEValidationTarget() = default;

  eValidationTargetType GetType() const;
  void SetType(eValidationTargetType t);

  SEDataRequest* GetDataRequest() const;
  void SetDataRequest(SEDataRequest* dr);

  double GetRangeMin() const;
  void   SetRangeMin(double min);

  double GetRangeMax() const;
  void   SetRangeMax(double max);

  std::vector<double>& GetData();

  double GetError() const;
  bool ComputeError();

protected:
  eValidationTargetType m_Type;
  SEDataRequest* m_DataRequest=nullptr;
  double m_RangeMin=SEScalar::dNaN();
  double m_RangeMax=SEScalar::dNaN();
    
  double m_TypeValue=SEScalar::dNaN();
  double m_Error=SEScalar::dNaN();
  std::vector<double> m_Data;
};
