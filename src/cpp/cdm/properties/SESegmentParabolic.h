/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "properties/SESegment.h"

class CDM_DECL SESegmentParabolic : public SESegment
{
protected:
  friend class PBProperty;//friend the serialization class
  friend class SECurve;
  SESegmentParabolic();
public:
  virtual ~SESegmentParabolic();

  virtual void Clear() override;
  virtual bool IsValid() const override;

  virtual bool HasCoefficient1() const;
  virtual SEScalar& GetCoefficient1();
  virtual double GetCoefficient1() const;

  virtual bool HasCoefficient2() const;
  virtual SEScalar& GetCoefficient2();
  virtual double GetCoefficient2() const;

  virtual bool HasCoefficient3() const;
  virtual SEScalar& GetCoefficient3();
  virtual double GetCoefficient3() const;

  virtual bool HasCoefficient4() const;
  virtual SEScalar& GetCoefficient4();
  virtual double GetCoefficient4() const;

  virtual std::string ToString() const override;
  virtual void ToString(std::ostream& str) const override;

protected:
  virtual void Abstract() override { };

  SEScalar* m_Coefficient1;
  SEScalar* m_Coefficient2;
  SEScalar* m_Coefficient3;
  SEScalar* m_Coefficient4;
};
