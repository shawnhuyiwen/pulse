/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/SENutrition.h"

class CDM_DECL SEMeal : public SENutrition
{
  friend class PBPatientNutrition;//friend the serialization class
public:

  SEMeal(Logger* logger);
  virtual ~SEMeal();

  void Clear() override;

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

public:

  virtual bool HasElapsedTime() const;
  virtual SEScalarTime& GetElapsedTime();
  virtual double GetElapsedTime(const TimeUnit& unit) const;
  
protected:

  SEScalarTime*    m_ElapsedTime;
};