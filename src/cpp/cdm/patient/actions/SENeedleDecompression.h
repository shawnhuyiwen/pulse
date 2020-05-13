/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "patient/actions/SEPatientAction.h"

class CDM_DECL SENeedleDecompression : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SENeedleDecompression();
  virtual ~SENeedleDecompression();

  virtual void Clear(); //clear memory
  virtual void Copy(const SENeedleDecompression& src);

  virtual bool IsValid() const;
  virtual bool IsActive() const;
  virtual void SetActive(bool b);

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch name);

  virtual eSide GetSide() const;
  virtual void SetSide(eSide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();
  
  virtual void ToString(std::ostream &str) const;

protected:
  eSide m_Side;
  eSwitch m_State;
};    