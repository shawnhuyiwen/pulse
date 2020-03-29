/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class CDM_DECL SESubstanceAerosolization : public Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstanceAerosolization(Logger* logger);
  virtual ~SESubstanceAerosolization();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasBronchioleModifier() const;
  virtual SEScalarNegative1To1& GetBronchioleModifier();
  virtual double GetBronchioleModifier() const;

  virtual bool HasInflammationCoefficient() const;
  virtual SEScalar0To1& GetInflammationCoefficient();
  virtual double GetInflammationCoefficient() const;

  virtual bool HasParticulateSizeDistribution() const;
  virtual SEHistogramFractionVsLength& GetParticulateSizeDistribution();
  virtual const SEHistogramFractionVsLength* GetParticulateSizeDistribution() const;
  
protected: 

  SEScalarNegative1To1*          m_BronchioleModifier;
  SEScalar0To1*                  m_InflammationCoefficient;
  SEHistogramFractionVsLength*   m_ParticulateSizeDistribution;
};