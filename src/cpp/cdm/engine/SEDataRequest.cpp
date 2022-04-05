/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEDataRequest.h"
#include "cdm/utils/unitconversion/UCCommon.h"
#include "cdm/io/protobuf/PBEngine.h"

std::string Space2Underscore(const std::string& str)
{
  std::string s = str; 
  std::transform(s.begin(), s.end(), s.begin(), [](char ch) {
    return ch == ' ' ? '_' : ch;
  });
  return s;
}

SEDataRequest::SEDataRequest(const SEDataRequest& dr)
{
  Set(dr);
  m_Category = dr.m_Category;
  m_ActionName = dr.m_ActionName;
  m_CompartmentName = dr.m_CompartmentName;
  m_SubstanceName = dr.m_SubstanceName;
  m_PropertyName = dr.m_PropertyName;
  m_RequestedUnit = dr.m_RequestedUnit;
  m_Unit = dr.m_Unit;
}

SEDataRequest::SEDataRequest(eDataRequest_Category category, const SEDecimalFormat* dfault) : SEDecimalFormat(dfault)
{
  m_Category = category;
  Clear();
}

SEDataRequest::~SEDataRequest()
{
  Clear();
}

void SEDataRequest::Clear()
{
  m_ActionName = "";
  m_CompartmentName = "";
  m_SubstanceName = "";
  m_PropertyName="";
  m_RequestedUnit="";
  m_Unit = nullptr;
}

void SEDataRequest::Copy(const SEDataRequest& src)
{
  PBEngine::Copy(src, *this);
}

bool SEDataRequest::IsValid()
{// TODO this could be a little stronger, and maybe logged, maybe pass in a logger?
 // Or this can go in the SEEngineTracker that is processing data requests
  // We also need to check that the substance name (and possibly Compartment Name) are valid
  if (m_PropertyName.empty())
    return false;
  switch (m_Category)
  {
    case eDataRequest_Category::Patient:
    {
      if (HasCompartmentName() || HasSubstanceName())
        std::cout << "Ignoring compartment and substance name on patient data request" << std::endl;
      return true;
    }
    case eDataRequest_Category::Physiology:
    {
      if (HasCompartmentName() || HasSubstanceName())
        std::cout << "Ignoring compartment and substance name on physiology data request" << std::endl;
      return true;
    }
    case eDataRequest_Category::Environment:
    {
      if (HasCompartmentName() || HasSubstanceName())
        std::cout << "Ignoring compartment and substance name on environment data request" << std::endl;
      return true;
    }
    case eDataRequest_Category::Action:
    {
      if (!HasActionName())
        return false;
      return true;
    }
    case eDataRequest_Category::GasCompartment:
    case eDataRequest_Category::LiquidCompartment:
    case eDataRequest_Category::ThermalCompartment:
    case eDataRequest_Category::TissueCompartment:
    {
      if (!HasCompartmentName())
        return false;
      return true;
    }
    case eDataRequest_Category::Substance:
    {
      if (!HasSubstanceName())
        return false;
      return true;
    }
    default:
      return true;//Equipment
  }
}

size_t SEDataRequest::HashCode() const
{
  size_t h = std::hash<std::string>()(m_PropertyName);
  return h;
}

eDataRequest_Category SEDataRequest::GetCategory() const 
{ 
  return m_Category;
}

std::string SEDataRequest::GetActionName() const
{
  return m_ActionName;
}
void SEDataRequest::SetActionName(const std::string& name)
{
  m_ActionName = name;
}
bool SEDataRequest::HasActionName() const
{
  return m_ActionName.empty() ? false : true;
}
void SEDataRequest::InvalidateActionName()
{
  m_ActionName = "";
}

std::string SEDataRequest::GetCompartmentName() const
{
  return m_CompartmentName;
}
void SEDataRequest::SetCompartmentName(const std::string& name)
{
  m_CompartmentName = name;
}
bool SEDataRequest::HasCompartmentName() const
{
  return m_CompartmentName.empty() ? false : true;
}
void SEDataRequest::InvalidateCompartmentName()
{
  m_CompartmentName = "";
}

std::string SEDataRequest::GetSubstanceName() const
{
  return m_SubstanceName;
}
void SEDataRequest::SetSubstanceName(const std::string& name)
{
  m_SubstanceName = name;
}
bool SEDataRequest::HasSubstanceName() const
{
  return m_SubstanceName.empty() ? false : true;
}
void SEDataRequest::InvalidateSubstanceName()
{
  m_SubstanceName = "";
}

std::string SEDataRequest::GetPropertyName() const
{
  return m_PropertyName;
}
void SEDataRequest::SetPropertyName(const std::string& name)
{
  m_PropertyName = name;
}
bool SEDataRequest::HasPropertyName() const
{
  return m_PropertyName.empty()?false:true;
}
void SEDataRequest::InvalidatePropertyName()
{
  m_PropertyName = "";
}

std::string SEDataRequest::GetRequestedUnit() const
{
  return m_RequestedUnit;
}
void SEDataRequest::SetRequestedUnit(const std::string& unit)
{
  m_RequestedUnit = unit;
  m_Unit = nullptr;
}
bool SEDataRequest::HasRequestedUnit() const
{
  return m_RequestedUnit.empty()?false:true;
}
void SEDataRequest::InvalidateRequestedUnit()
{
  m_RequestedUnit = "";
}

const CCompoundUnit* SEDataRequest::GetUnit() const
{
  return m_Unit;
}
void SEDataRequest::SetUnit(const CCompoundUnit& unit)
{
  m_Unit = &unit;
  m_RequestedUnit = "";
}
bool SEDataRequest::HasUnit() const
{
  return m_Unit==nullptr?false:true;
}
void SEDataRequest::InvalidateUnit()
{
  m_Unit = nullptr;
}

std::string SEDataRequest::ToString() const
{
  std::stringstream str;
  str << eDataRequest_Category_Name(m_Category) << " Data Request";
  if (m_Category == eDataRequest_Category::Action)
    str << "\n\tAction : " << m_ActionName;
  if(HasCompartmentName())
    str << "\n\tCompartment : " << m_CompartmentName;
  if (HasSubstanceName())
    str << "\n\tSubstance : " << m_SubstanceName;
  str << "\n\tProperty: " << m_PropertyName;
  if (HasRequestedUnit())
    str << "\n\tRequested Unit : " << m_RequestedUnit;
  return str.str();
}

std::string SEDataRequest::GetHeaderName() const
{
  std::stringstream ss;
  if(GetCategory() == eDataRequest_Category::Patient)
    ss << "Patient-";
  else if(GetCategory() == eDataRequest_Category::MechanicalVentilator)
    ss << "MechanicalVentilator-";
  // TODO We probably should prefix all equipment amb

  switch (GetCategory())
  {
    case eDataRequest_Category::Patient:
    case eDataRequest_Category::Physiology:
    case eDataRequest_Category::Environment:
    case eDataRequest_Category::AnesthesiaMachine:
    case eDataRequest_Category::ECG:
    case eDataRequest_Category::Inhaler:
    case eDataRequest_Category::MechanicalVentilator:
    {
      if (!HasUnit() && !HasRequestedUnit())
        ss << GetPropertyName();
      else if(HasUnit())
        ss << GetPropertyName() << "(" << *GetUnit() << ")";
      else //Requested unit
        ss << GetPropertyName() << "(" << GetRequestedUnit() << ")";

      return Space2Underscore(ss.str());
    }
    case eDataRequest_Category::Action:
    {
      if (HasCompartmentName() && HasSubstanceName())
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetActionName() << "-" << GetCompartmentName() << "-" << GetSubstanceName() << "-" << GetPropertyName();
        else if(HasUnit())
           ss << GetActionName() << "-" << GetCompartmentName() << "-" << GetSubstanceName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetActionName() << "-" << GetCompartmentName() << "-" << GetSubstanceName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
      }
      else if (HasCompartmentName())
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetActionName() << "-" << GetCompartmentName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetActionName() << "-" << GetCompartmentName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetActionName() << "-" << GetCompartmentName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
      }
      else if (HasSubstanceName())
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetActionName() << "-" << GetSubstanceName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetActionName() << "-" << GetSubstanceName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetActionName() << "-" << GetSubstanceName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
      }
      else
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetActionName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetActionName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetActionName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
      }

      return Space2Underscore(ss.str());
    }
    case eDataRequest_Category::GasCompartment:
    case eDataRequest_Category::LiquidCompartment:
    case eDataRequest_Category::ThermalCompartment:
    case eDataRequest_Category::TissueCompartment:
    {
      if (HasSubstanceName())
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetCompartmentName() << "-" << GetSubstanceName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetCompartmentName() << "-" << GetSubstanceName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetCompartmentName() << "-" << GetSubstanceName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
      }
      else
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetCompartmentName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetCompartmentName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetCompartmentName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
      }

      return Space2Underscore(ss.str());
    }
    case eDataRequest_Category::Substance:
    {
      if (HasCompartmentName())
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetSubstanceName() << "-" << GetCompartmentName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetSubstanceName() << "-" << GetCompartmentName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetSubstanceName() << "-" << GetCompartmentName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";

        return ss.str();
      }
      else
      {
        if (!HasUnit() && !HasRequestedUnit())
          ss << GetSubstanceName() << "-" << GetPropertyName();
        else if(HasUnit())
          ss << GetSubstanceName() << "-" << GetPropertyName() << "(" << *GetUnit() << ")";
        else //Requested unit
          ss << GetSubstanceName() << "-" << GetPropertyName() << "(" << GetRequestedUnit() << ")";
        
        return Space2Underscore(ss.str());
      }
    }
    default:
      return "";
  }

}
