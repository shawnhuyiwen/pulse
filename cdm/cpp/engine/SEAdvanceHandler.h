/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class CDM_DECL SEAdvanceHandler
{
public:
  SEAdvanceHandler() { m_OnStabilization = false; }
  SEAdvanceHandler(bool on_stabilization) { m_OnStabilization = on_stabilization; }
  virtual ~SEAdvanceHandler(){};

  virtual void SetStabilizationCallback(bool b){ m_OnStabilization = b; }
  virtual void OnAdvance(double time_s, const PhysiologyEngine& engine)=0;

protected:
  bool m_OnStabilization;// Tell the engine to call this handler during stabilization
};
