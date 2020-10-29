/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

class CDM_DECL SEActiveConditioning : public Loggable
{
  friend class PBEnvironment;//friend the serialization class
public:

  SEActiveConditioning(Logger* logger);
  virtual ~SEActiveConditioning();

  virtual void Clear();

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasPower() const;
  virtual SEScalarPower& GetPower();
  virtual double GetPower(const PowerUnit& unit) const;

  virtual bool HasSurfaceArea() const;
  virtual SEScalarArea& GetSurfaceArea();
  virtual double GetSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasSurfaceAreaFraction() const;
  virtual SEScalar0To1& GetSurfaceAreaFraction();
  virtual double GetSurfaceAreaFraction() const;

  virtual void ToString(std::ostream &str) const;

protected:
  SEScalarPower*     m_Power;
  SEScalarArea*      m_SurfaceArea;
  SEScalar0To1*      m_SurfaceAreaFraction;
};