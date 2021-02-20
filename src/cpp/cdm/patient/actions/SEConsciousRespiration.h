/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"
class SEConsciousRespirationCommand;
class SEForcedExhale;
class SEForcedInhale;
class SEForcedPause;
class SEUseInhaler;

class CDM_DECL SEConsciousRespiration : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEConsciousRespiration(Logger* logger=nullptr);
  virtual ~SEConsciousRespiration();

  virtual void Clear(); //clear memory
  virtual void Copy(const SEConsciousRespiration& src, bool preserveState=false);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void Deactivate();

  virtual bool StartImmediately() const;
  virtual void SetStartImmediately(bool b);

  // Get the active command
  virtual bool HasCommands() const;
  virtual SEConsciousRespirationCommand* GetActiveCommand();
  /// When the active command has been processed, remove it
  virtual void RemoveActiveCommand();

  virtual SEForcedExhale& AddForcedExhale();
  virtual SEForcedInhale& AddForcedInhale();
  virtual SEForcedPause&  AddForcedPause();
  virtual SEUseInhaler&   AddUseInhaler();

  virtual void ToString(std::ostream &str) const;

  virtual const SEScalar* GetScalar(const std::string& name);

protected:

  bool m_StartImmediately;
  std::vector<SEConsciousRespirationCommand*> m_Commands;
};  