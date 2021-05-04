/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "PhysiologyEngine.h"
#include "engine/SEEventManager.h"

class CDM_DECL SEDataRequested : public LoggerForward, public SEEventHandler
{
  friend class PBEngine;//friend the serialization class
public:
  explicit SEDataRequested();
  virtual ~SEDataRequested();

  SEDataRequested(const SEDataRequested&) = delete;
  SEDataRequested operator=(const SEDataRequested&) = delete;

  virtual void SetEngine(const PhysiologyEngine& engine);

  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  static bool SerializeToString(const std::vector<SEDataRequested*>& src, std::string& dst, SerializationFormat m);

  virtual int GetID() const;
  virtual void SetID(int id);

  virtual bool IsActive() const;// Set when an Error or Fatal has been loggged
  virtual void SetIsActive(bool b);

  virtual void PullDataRequested();
  virtual void ClearDataRequested();

  virtual const std::vector<double>& GetValues() const;

  virtual bool KeepLogMessages() const { return m_KeepLogMessages; }
  virtual void KeepLogMessages(bool b) { m_KeepLogMessages = b; }
  virtual const std::vector<SEEventChange>& GetEventChanges() const;
  virtual const LogMessages& GetLogMessages() const;
  virtual void ForwardDebug(const std::string& msg, const std::string& origin);
  virtual void ForwardInfo(const std::string& msg, const std::string& origin);
  virtual void ForwardWarning(const std::string& msg, const std::string& origin);
  virtual void ForwardError(const std::string& msg, const std::string& origin);
  virtual void ForwardFatal(const std::string& msg, const std::string& origin);

  virtual bool KeepEventChanges() const { return m_KeepEventChanges; }
  virtual void KeepEventChanges(bool b) { m_KeepEventChanges = b; }
  void HandleEvent(eEvent type, bool active, const SEScalarTime* time = nullptr);

protected:
  int                             m_ID;
  bool                            m_IsActive;
  std::vector<double>             m_Values;
  bool                            m_KeepEventChanges;
  std::vector<SEEventChange>      m_EventChanges;
  bool                            m_KeepLogMessages;
  LogMessages                     m_LogMessages;
  const PhysiologyEngine*         m_Engine;
};
