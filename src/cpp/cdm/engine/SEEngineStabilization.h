/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SECondition;
class SEConditionManager;
class SEEngineTrack;
class PhysiologyEngine;
class SEEngineConfiguration;

class CDM_DECL SEEngineStabilization : public Loggable
{
public:
  SEEngineStabilization(Logger* logger);
  virtual ~SEEngineStabilization();

  virtual void Clear();

  virtual bool SerializeToString(std::string& output, SerializationFormat) const = 0;
  virtual bool SerializeToFile(const std::string& filename, SerializationFormat) const = 0;
  virtual bool SerializeFromString(const std::string& src, SerializationFormat) = 0;
  virtual bool SerializeFromFile(const std::string& filename, SerializationFormat) = 0;

  virtual bool StabilizeRestingState(PhysiologyEngine& engine)=0;
  virtual bool StabilizeFeedbackState(PhysiologyEngine& engine) = 0;
  virtual bool StabilizeConditions(PhysiologyEngine& engine, const SEConditionManager& conditions)=0;
  
  virtual void LogProgress(bool b);

  virtual void CancelStabilization();

  virtual void TrackStabilization(eSwitch state);
  virtual bool IsTrackingStabilization();

  virtual bool HasStabilizationDuration();
  virtual SEScalarTime& GetStabilizationDuration();

protected:

  bool m_Cancelled;
  bool m_LogProgress;
  eSwitch m_TrackingStabilization;
  double m_currentTime_s;
  std::stringstream m_ss;

  SEScalarTime* m_StabilizationDuration;
  std::vector<const SECondition*> m_Conditions;
};
