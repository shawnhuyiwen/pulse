/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class PhysiologyEngine;

class CDM_DECL SEAdvanceHandler
{
public:
  SEAdvanceHandler() { m_OnStabilization = false; }
  SEAdvanceHandler(bool on_stabilization) { m_OnStabilization = on_stabilization; }
  virtual ~SEAdvanceHandler(){};

  bool OnForStabilization() { return m_OnStabilization; }
  virtual void SetStabilizationCallback(bool b){ m_OnStabilization = b; }
  virtual void OnAdvance(double time_s)=0;

protected:
  bool m_OnStabilization;// Tell the engine to call this handler during stabilization
};
