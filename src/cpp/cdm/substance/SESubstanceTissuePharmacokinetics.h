/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class CDM_DECL SESubstanceTissuePharmacokinetics : public Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstanceTissuePharmacokinetics(const std::string& name, Logger* logger);
  virtual ~SESubstanceTissuePharmacokinetics();

  virtual void Clear();

  const SEScalar* GetScalar(const std::string& name);

  virtual std::string GetName() const;

  virtual bool HasPartitionCoefficient() const;
  virtual SEScalar& GetPartitionCoefficient();
  virtual double GetPartitionCoefficient() const;


protected:
  std::string   m_Name;
  SEScalar*     m_PartitionCoefficient;
};