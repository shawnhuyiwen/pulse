/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceManager;
class SESubstanceConcentration;

class CDM_DECL SESubstanceCompound : public Loggable
{
  friend class PBSubstance;//friend the serialization class
  friend class SESubstanceManager;
protected:
  SESubstanceCompound(const std::string& name, Logger* logger);
public:
  virtual ~SESubstanceCompound();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, const SESubstanceManager& subMgr, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

  virtual std::string GetName() const;

  bool HasComponent() const;
  bool HasComponent(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetComponents();
  const std::vector<const SESubstanceConcentration*>& GetComponents() const;
  SESubstanceConcentration& GetComponent(const SESubstance& substance);
  const SESubstanceConcentration* GetComponent(const SESubstance& substance) const;
  void RemoveComponent(const SESubstance& substance);

protected: 

  std::string m_Name;

  std::vector<SESubstanceConcentration*>       m_Components;
  std::vector<const SESubstanceConcentration*> m_cComponents;
};