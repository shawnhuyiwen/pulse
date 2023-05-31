/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/PhysiologyEngine.h"
#include "cdm/engine/SEEventManager.h"
class DataTrack;

class CDM_DECL SEDataRequested : public LoggerForward, public SEEventHandler
{
  friend class PBEngine;//friend the serialization class
public:
  explicit SEDataRequested();
  virtual ~SEDataRequested();

  SEDataRequested(const SEDataRequested&) = delete;
  SEDataRequested operator=(const SEDataRequested&) = delete;

  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  static bool SerializeToString(const std::vector<SEDataRequested*>& src, std::string& dst, eSerializationFormat m);

  virtual int GetID() const;
  virtual void SetID(int id);

  virtual bool IsActive() const;// Set when an Error or Fatal has been loggged
  virtual void SetIsActive(bool b);

  virtual void ClearDataRequested();
  virtual void PullDataRequested(double currentTime_s, DataTrack& tracker);

  virtual const std::vector<std::string>& GetHeaders() const;
  virtual const std::map<double, std::vector<double>>& GetAllValues() const;

  virtual bool KeepLogMessages() const { return m_KeepLogMessages; }
  virtual void KeepLogMessages(bool b) { m_KeepLogMessages = b; }
  virtual const std::vector<SEEventChange>& GetEventChanges() const;
  virtual const LogMessages& GetLogMessages() const;
  
  void ForwardDebug(const std::string& msg) override;
  void ForwardInfo(const std::string& msg) override;
  void ForwardWarning(const std::string& msg) override;
  void ForwardError(const std::string& msg) override;
  void ForwardFatal(const std::string& msg) override;

  virtual bool KeepEventChanges() const { return m_KeepEventChanges; }
  virtual void KeepEventChanges(bool b) { m_KeepEventChanges = b; }

  void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr) override;

protected:
  int                                   m_ID;
  bool                                  m_IsActive;
  bool                                  m_KeepEventChanges;
  std::vector<SEEventChange>            m_EventChanges;
  bool                                  m_KeepLogMessages;
  LogMessages                           m_LogMessages;
  std::vector<std::string>              m_Headers;
  std::map<double, std::vector<double>> m_SegmentsPerSimTime_s;
  const PhysiologyEngine*               m_Engine;
};
