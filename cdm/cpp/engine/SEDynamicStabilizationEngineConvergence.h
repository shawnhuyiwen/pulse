/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
CDM_BIND_DECL(DynamicStabilizationData_EngineConvergenceData)
class SEDynamicStabilizationPropertyConvergence;
class SEDataRequest;
class SEDataRequestManager;

class CDM_DECL SEDynamicStabilizationEngineConvergence : public Loggable
{
  friend class SEDynamicStabilization;
protected:
  SEDynamicStabilizationEngineConvergence(Logger* logger);

public:
  virtual ~SEDynamicStabilizationEngineConvergence();

  virtual void Clear();

  static void Load(const cdm::DynamicStabilizationData_EngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst);
  static cdm::DynamicStabilizationData_EngineConvergenceData* Unload(const SEDynamicStabilizationEngineConvergence& src);
protected:
  static void Serialize(const cdm::DynamicStabilizationData_EngineConvergenceData& src, SEDynamicStabilizationEngineConvergence& dst);
  static void Serialize(const SEDynamicStabilizationEngineConvergence& src, cdm::DynamicStabilizationData_EngineConvergenceData& dst);

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

