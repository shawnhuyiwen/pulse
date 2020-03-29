/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstancePhysicochemicals;
class SESubstanceTissuePharmacokinetics;

class CDM_DECL SESubstancePharmacokinetics : public Loggable
{
  friend class PBSubstance;//friend the serialization class
public:

  SESubstancePharmacokinetics(Logger* logger);
  virtual ~SESubstancePharmacokinetics();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasPhysicochemicals() const;
  virtual SESubstancePhysicochemicals& GetPhysicochemicals();
  virtual const SESubstancePhysicochemicals* GetPhysicochemicals() const;

  virtual bool HasTissueKinetics() const;
  virtual bool HasTissueKinetics(const std::string& name) const;
  virtual SESubstanceTissuePharmacokinetics& GetTissueKinetics(const std::string& name);
  virtual const SESubstanceTissuePharmacokinetics* GetTissueKinetics(const std::string& name) const;
  virtual void RemoveTissueKinetics(const std::string& name);

protected: 

  SESubstancePhysicochemicals*                              m_Physicochemicals;
  std::map<std::string, SESubstanceTissuePharmacokinetics*> m_TissueKinetics;

};