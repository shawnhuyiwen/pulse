/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceManager;
class SESubstanceConcentration;

class CDM_DECL SESubstanceCompound : public Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstanceCompound(Logger* logger);
  virtual ~SESubstanceCompound();

  virtual void Clear();

  virtual std::string GetName() const;
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  bool HasComponent() const;
  bool HasComponent(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetComponents();  
  const std::vector<const SESubstanceConcentration*>& GetComponents() const;
  const SESubstanceConcentration& GetComponent(SESubstance& substance);
  const SESubstanceConcentration* GetComponent(SESubstance& substance) const;
  void RemoveComponent(const SESubstance& substance);

protected: 

  std::string m_Name;

  std::vector<SESubstanceConcentration*>       m_Components;
  std::vector<const SESubstanceConcentration*> m_cComponents;
};