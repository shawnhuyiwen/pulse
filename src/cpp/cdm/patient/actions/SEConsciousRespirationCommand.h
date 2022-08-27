/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"

class CDM_DECL SEConsciousRespirationCommand : public Loggable
{
  friend class PBPatientAction;//friend the serialization class
public:

  SEConsciousRespirationCommand(Logger* logger);
  virtual ~SEConsciousRespirationCommand();


  static constexpr char const* ActionType = "Conscious Respiration Command";
  virtual std::string GetActionType() const { return ActionType; }

  virtual std::string GetName() const = 0;

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual std::string GetComment() const;
  virtual void SetComment(const std::string& comment);
  virtual bool HasComment() const;
  virtual void InvalidateComment();

protected:

  std::string  m_Comment;
};
