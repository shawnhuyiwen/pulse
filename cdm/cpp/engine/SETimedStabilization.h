/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "engine/SEEngineStabilization.h"

class CDM_DECL SETimedStabilization : public SEEngineStabilization
{
  friend class PBEngine;//friend the serialization class
public:
  SETimedStabilization(Logger* logger);
  virtual ~SETimedStabilization();

  virtual void Clear();

public:

  bool SerializeToString(std::string& output, SerializationMode m) const;
  bool SerializeToFile(const std::string& filename, SerializationMode m) const;
  bool SerializeFromString(const std::string& src, SerializationMode m);
  bool SerializeFromFile(const std::string& filename, SerializationMode m);

  virtual bool StabilizeRestingState(PhysiologyEngine& engine);
  virtual bool StabilizeFeedbackState(PhysiologyEngine& engine);
  virtual bool StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions);

  virtual SEScalarTime& GetRestingStabilizationTime();
  virtual double GetRestingStabilizationTime(const TimeUnit& unit) const;

  virtual bool HasFeedbackStabilizationTime() const;
  virtual SEScalarTime& GetFeedbackStabilizationTime();
  virtual double GetFeedbackStabilizationTime(const TimeUnit& unit) const;

  virtual bool HasConditionTime(const std::string& name) const;
  virtual void RemoveConditionTime(const std::string& name);
  virtual const std::map<std::string,SEScalarTime*>& GetConditionTimes() const;
  virtual SEScalarTime& GetConditionTime(const std::string& name);
  virtual const SEScalarTime* GetConditionTime(const std::string& name) const;

protected:

  virtual bool Stabilize(PhysiologyEngine& engine, const SEScalarTime& time);

  SEScalarTime* m_RestingStabilizationTime;
  SEScalarTime* m_FeedbackStabilizationTime;
  std::map<std::string,SEScalarTime*> m_ConditionTimes;
};
