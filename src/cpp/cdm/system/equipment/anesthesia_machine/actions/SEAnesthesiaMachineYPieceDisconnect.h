/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/anesthesia_machine/actions/SEAnesthesiaMachineAction.h"

class CDM_DECL SEAnesthesiaMachineYPieceDisconnect : public SEAnesthesiaMachineAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEAnesthesiaMachineYPieceDisconnect(Logger* logger=nullptr);
  virtual ~SEAnesthesiaMachineYPieceDisconnect();

  static constexpr char const* Name = "Y Piece Disconnect";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEAnesthesiaMachineYPieceDisconnect& src, bool /*preserveState*/=false);

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  virtual double GetSeverity() const;

  const SEScalar* GetScalar(const std::string& name) override;

protected:

  SEScalar0To1*     m_Severity;
};  