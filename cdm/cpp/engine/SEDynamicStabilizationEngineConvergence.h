/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEDynamicStabilizationPropertyConvergence;
class SEDataRequest;
class SEDataRequestManager;

class CDM_DECL SEDynamicStabilizationEngineConvergence : public Loggable
{
  friend class PBEngine;//friend the serialization class
  friend class SEDynamicStabilization;
protected:
  SEDynamicStabilizationEngineConvergence(Logger* logger);

public:
  virtual ~SEDynamicStabilizationEngineConvergence();

  virtual void Clear();

public:

  virtual SEScalarTime& GetConvergenceTime();
  virtual double GetConvergenceTime(const TimeUnit& unit) const;

  virtual SEScalarTime& GetMinimumReactionTime();
  virtual double GetMinimumReactionTime(const TimeUnit& unit) const;

  virtual SEScalarTime& GetMaximumAllowedStabilizationTime();
  virtual double GetMaximumAllowedStabilizationTime(const TimeUnit& unit) const;

  virtual const std::vector<SEDynamicStabilizationPropertyConvergence*>& GetPropertyConvergence() const;
  virtual SEDynamicStabilizationPropertyConvergence& CreatePropertyConvergence(SEDataRequest& dr, double percentError);

protected:
  SEScalarTime*                                           m_ConvergenceTime;
  SEScalarTime*                                           m_MinimumReactionTime;
  SEScalarTime*                                           m_MaximumAllowedStabilizationTime;

  SEDataRequestManager*                                   m_DataRequestMgr;
  std::vector<SEDynamicStabilizationPropertyConvergence*> m_PropertyConvergence;
};

