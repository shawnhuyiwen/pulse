/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
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

  static constexpr char const* Name = "Conscious Respiration";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SEConsciousRespiration& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

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

protected:

  bool m_StartImmediately;
  std::vector<SEConsciousRespirationCommand*> m_Commands;
};  