/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "blackbox/fluid/SEGasBlackBox.h"

SEGasBlackBox::SEGasBlackBox(const std::string& name, Logger* logger) : SEFluidBlackBox(name, logger)
{

}
SEGasBlackBox::~SEGasBlackBox()
{

}
void SEGasBlackBox::Clear()
{}
//  SEFluidBlackBox::Clear();
//  DELETE_VECTOR(m_SubstanceQuantities);
//}
//bool SEGasBlackBox::HasSubstanceQuantities() const
//{
//  return !m_SubstanceQuantities.empty();
//}
//bool SEGasBlackBox::HasSubstanceQuantity(const SESubstance& substance) const
//{
//  for (SEGasBlackBoxSubstanceQuantity* subQ : m_SubstanceQuantities)
//  {
//    if (&subQ->GetSubstance() == &substance)
//      return true;
//  }
//  return false;
//}
//SEGasBlackBoxSubstanceQuantity* SEGasBlackBox::GetSubstanceQuantity(const SESubstance& substance) const
//{
//  for (SEGasBlackBoxSubstanceQuantity* subQ : m_SubstanceQuantities)
//  {
//    if (&subQ->GetSubstance() == &substance)
//      return subQ;
//  }
//  return nullptr;
//}
//const std::vector<SEGasBlackBoxSubstanceQuantity*>& SEGasBlackBox::GetSubstanceQuantities() const
//{
//  return m_SubstanceQuantities;
//}
