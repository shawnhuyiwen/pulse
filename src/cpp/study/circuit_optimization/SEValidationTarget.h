/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEDataRequest.h"
#include "cdm/engine/SEEventManager.h"

class SEValidationTarget : public Loggable
{
public:
  SEValidationTarget(Logger* logger = nullptr);
  ~SEValidationTarget() = default;

  enum class Type {Min, Max, Mean};

  Type GetType() const;
  void SetType(Type t);

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
  Type           m_Type;
  SEDataRequest* m_DataRequest=nullptr;
    
  double m_Value=SEScalar::dNaN();
  double m_Error=SEScalar::dNaN();
  double m_RangeMin=SEScalar::dNaN();
  double m_RangeMax=SEScalar::dNaN();
  std::vector<double> m_Data;
};
