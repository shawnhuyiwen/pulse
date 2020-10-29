/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "engine/SEDynamicStabilizationPropertyConvergence.h"
#include "engine/SEDataRequest.h"
#include "engine/SEEngineTracker.h"
#include "properties/SEScalar.h"
#include "utils/GeneralMath.h"

SEDynamicStabilizationPropertyConvergence::SEDynamicStabilizationPropertyConvergence(SEDataRequest& dr, Logger* logger) : Loggable(logger), m_DataRequest(dr)
{
  m_Error = 0;
  m_Target = 0;
  m_LastError = 0;
  m_LastErrorTime_s = 0;
  m_Optional = false;
  *(reinterpret_cast<unsigned long long int *>(&m_LastError)) = SEScalar::NaN;
  *(reinterpret_cast<unsigned long long int *>(&m_Target)) = SEScalar::NaN;
  m_DataRequestScalar = new SEDataRequestScalar(logger);
}

SEDynamicStabilizationPropertyConvergence::~SEDynamicStabilizationPropertyConvergence()
{
  SAFE_DELETE(m_DataRequestScalar)
}

// This basically tests the current property with the target proptry and if they are in a window of acceptance
bool SEDynamicStabilizationPropertyConvergence::Test(double time_s)
{
  double v = !m_DataRequestScalar->HasUnit() ?
    m_DataRequestScalar->GetValue() : m_DataRequestScalar->GetValue(*m_DataRequestScalar->GetUnit());
  if (std::isnan(m_Target))
  {
    m_Target = v;
    return false;// Initially target will be NaN until it is pulled for the first time
  }
  m_LastError = GeneralMath::PercentTolerance(m_Target, v);
  if (m_LastError<m_Error)
  {
    return true;
  }
  m_ss << time_s << "s - Resetting the target for " << GetDataRequest().GetPropertyName() << " to " << v << ", the last target value was " << m_Target;
  Info(m_ss);
  // Not converging, so this is now our target property
  m_Target = v;
  m_LastErrorTime_s = time_s;
  return false;
}


void SEDynamicStabilizationPropertyConvergence::TrackScalar(const SEScalar& s) 
{ 
  m_DataRequestScalar->SetScalar(&s, m_DataRequest); 
}
SEDataRequestScalar& SEDynamicStabilizationPropertyConvergence::GetDataRequestScalar() 
{ 
  return *m_DataRequestScalar; 
}

