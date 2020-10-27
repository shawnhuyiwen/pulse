/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEElectricCompartment;
class SEGasCompartment;
class SELiquidCompartment;
class SEThermalCompartment;
class SETissueCompartment;
class SESubstance;
class SECircuitManager;

class CDM_DECL SECompartment : public Loggable
{
  friend class PBCompartment;//friend the serialization class
protected:
  SECompartment(const std::string& name, Logger* logger);
public: 
  virtual ~SECompartment();

  virtual void Clear();

  virtual std::string GetName() const;

  virtual const SEScalar* GetScalar(const std::string& name) = 0;

  virtual bool HasChildren() const = 0; // Compartments with children contain 'read only' scalars

  virtual void StateChange() = 0;

protected:
  std::string  m_Name;

public:

  template<typename CompartmentType>
  static void FindLeaves(CompartmentType& cmpt, std::vector<CompartmentType*>& leaves)
  {
    for (CompartmentType* child : cmpt.GetChildren())
    {
      if (!child->HasChildren())
        leaves.push_back(child);
      else
        FindLeaves(*child, leaves);
    }
  }
};