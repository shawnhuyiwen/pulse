/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SECircuitManager;

class CDM_DECL SECompartmentLink : public Loggable
{
  friend class PBCompartment;//friend the serialization class
protected:
  SECompartmentLink(const std::string& nam, Logger* logger);
public: 
  virtual ~SECompartmentLink();

  virtual void Clear();

  virtual std::string GetName() const;

  virtual const SEScalar* GetScalar(const std::string& name) = 0;

protected:
  std::string  m_Name;
};