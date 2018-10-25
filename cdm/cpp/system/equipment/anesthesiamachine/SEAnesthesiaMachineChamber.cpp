/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "system/equipment/anesthesiamachine/SEAnesthesiaMachineChamber.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "properties/SEScalar0To1.h"

SEAnesthesiaMachineChamber::SEAnesthesiaMachineChamber(SESubstanceManager& substances) : Loggable(substances.GetLogger()), m_Substances(substances)
{
  m_State = eSwitch::Off;
  m_SubstanceFraction = nullptr;
  m_Substance = nullptr;
}

SEAnesthesiaMachineChamber::~SEAnesthesiaMachineChamber()
{
  Clear();
}

void SEAnesthesiaMachineChamber::Clear()
{
  m_State = eSwitch::Off;
  SAFE_DELETE(m_SubstanceFraction);
  m_Substance=nullptr;
}

void SEAnesthesiaMachineChamber::Merge(const SEAnesthesiaMachineChamber& from)
{
  SetState(from.m_State);
  if (from.HasSubstanceFraction())
    GetSubstanceFraction().Set(*from.m_SubstanceFraction);
  if (from.m_Substance != nullptr)
  {
    if (&m_Substances != &from.m_Substances)
    {
      m_Substance = m_Substances.GetSubstance(from.m_Substance->GetName());
      if (m_Substance == nullptr)
      {
        std::stringstream ss;
        ss << "Do not have substance : " << from.m_Substance->GetName();
        Error(ss);
      }
    }
    else
      m_Substance = from.m_Substance;
  }
}

const SEScalar* SEAnesthesiaMachineChamber::GetScalar(const std::string& name)
{
  if (name == "SubstanceFraction")
    return &GetSubstanceFraction();
  return nullptr;
}

eSwitch SEAnesthesiaMachineChamber::GetState() const
{
  return m_State;
}
void SEAnesthesiaMachineChamber::SetState(eSwitch state)
{
  m_State = (state == eSwitch::NullSwitch) ? eSwitch::Off : state;
}

bool SEAnesthesiaMachineChamber::HasSubstanceFraction() const
{
  return m_SubstanceFraction==nullptr?false:m_SubstanceFraction->IsValid();
}
SEScalar0To1& SEAnesthesiaMachineChamber::GetSubstanceFraction()
{
  if(m_SubstanceFraction==nullptr)
    m_SubstanceFraction = new SEScalar0To1();
  return *m_SubstanceFraction;
}
double SEAnesthesiaMachineChamber::GetSubstanceFraction() const
{
  if (m_SubstanceFraction == nullptr)
    return SEScalar::dNaN();
  return m_SubstanceFraction->GetValue();
}

bool SEAnesthesiaMachineChamber::HasSubstance() const
{
  return m_Substance==nullptr?false:true;
}
SESubstance* SEAnesthesiaMachineChamber::GetSubstance() const
{
  return (SESubstance*)m_Substance;
}
void SEAnesthesiaMachineChamber::SetSubstance(const SESubstance& substance)
{
  m_Substance=&substance;
}
void SEAnesthesiaMachineChamber::RemoveSubstance()
{
  m_Substance=nullptr;;
}

void SEAnesthesiaMachineChamber::ToString(std::ostream &str) const
{
  if (m_SubstanceFraction != nullptr)
    str << " Anesthesia Machine Chamber, Substance Fraction" << m_SubstanceFraction
      <<", Substance :"<<GetSubstance()->GetName()<<std::flush;
  else
    str<<"Action not specified properly"<<std::flush;
}
