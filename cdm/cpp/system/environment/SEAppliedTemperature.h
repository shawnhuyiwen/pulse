/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once

class CDM_DECL SEAppliedTemperature : public Loggable
{
  friend class PBEnvironment;//friend the serialization class
public:

  SEAppliedTemperature(Logger* logger);
  virtual ~SEAppliedTemperature();
  
  virtual void Clear();

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasTemperature() const;
  virtual SEScalarTemperature& GetTemperature();
  virtual double GetTemperature(const TemperatureUnit& unit) const;

  virtual bool HasSurfaceArea() const;
  virtual SEScalarArea& GetSurfaceArea();
  virtual double GetSurfaceArea(const AreaUnit& unit) const;

  virtual bool HasSurfaceAreaFraction() const;
  virtual SEScalar0To1& GetSurfaceAreaFraction();
  virtual double GetSurfaceAreaFraction() const;

  virtual eSwitch GetState() const;
  virtual void SetState(eSwitch onOff);

  virtual void ToString(std::ostream &str) const;

protected:
  
  SEScalarTemperature*   m_Temperature;
  SEScalarArea*          m_SurfaceArea;
  SEScalar0To1*          m_SurfaceAreaFraction;
  eSwitch  m_State;
};