/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstanceManager;
class CCompoundUnit;
class SEDataRequestManager;
#include "engine/SEDecimalFormat.h"

enum class eDataRequest_Category {
  Patient = 0, Physiology, Environment, Action,
  GasCompartment, LiquidCompartment, ThermalCompartment, TissueCompartment,
  Substance, AnesthesiaMachine, ECG, Inhaler, MechanicalVentilator
};
extern const std::string& eDataRequest_Category_Name(eDataRequest_Category m);

class CDM_DECL SEDataRequest : public SEDecimalFormat
{
  friend class PBEngine;//friend the serialization class
  friend class SEDataRequestManager;
protected:
  SEDataRequest(const SEDataRequest& dr);
  SEDataRequest(eDataRequest_Category category, const SEDecimalFormat* dfault = nullptr);
public:

  virtual ~SEDataRequest();

  virtual void Clear(); //clear memory
  virtual bool IsValid();
  virtual void Copy(const SEDataRequest& src);

  virtual size_t HashCode() const;

  // The Request Category
  virtual eDataRequest_Category GetCategory() const;

  // OPTIONAL The Action Name holding the property
  virtual std::string GetActionName() const;
  virtual void SetActionName(const std::string& name);
  virtual bool HasActionName() const;
  virtual void InvalidateActionName();

  // OPTIONAL The Compartment Name holding the property
  virtual std::string GetCompartmentName() const;
  virtual void SetCompartmentName(const std::string& name);
  virtual bool HasCompartmentName() const;
  virtual void InvalidateCompartmentName();

  // OPTIONAL The Substance Name holding the property
  virtual std::string GetSubstanceName() const;
  virtual void SetSubstanceName(const std::string& name);
  virtual bool HasSubstanceName() const;
  virtual void InvalidateSubstanceName();

  // The System Property Name
  virtual std::string GetPropertyName() const;
  virtual void SetPropertyName(const std::string& name);
  virtual bool HasPropertyName() const;
  virtual void InvalidatePropertyName();

  // The Requested Unit String
  virtual std::string GetRequestedUnit() const;
  virtual void SetRequestedUnit(const std::string& unit);
  virtual bool HasRequestedUnit() const;
  virtual void InvalidateRequestedUnit();

  // The Unit that data will be retrieved in
  // If the Requested unit is present, this should be set to that unit
  // I cannot do this automatically, since I don't know what the quantity of the property is, so some other logic needs to complete this logic
  // If the Requested unit is NOT present, this should be set to whatever the unit of the System Property is set as
  virtual const CCompoundUnit* GetUnit() const;
  virtual void SetUnit(const CCompoundUnit& unit);
  virtual bool HasUnit() const;
  virtual void InvalidateUnit();

protected:

  eDataRequest_Category          m_Category;
  std::string                    m_ActionName;
  std::string                    m_CompartmentName;
  std::string                    m_SubstanceName;
  std::string                    m_PropertyName;
  std::string                    m_RequestedUnit;
  const CCompoundUnit*           m_Unit;
};