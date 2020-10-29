/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "blackbox/fluid/SELiquidBlackBox.h"

SELiquidBlackBox::SELiquidBlackBox(const std::string& name, Logger* logger) : SEFluidBlackBox(name, logger)
{

}
SELiquidBlackBox::~SELiquidBlackBox()
{

}
void SELiquidBlackBox::Clear()
{}
//  SEFluidBlackBox::Clear();
//  DELETE_VECTOR(m_SubstanceQuantities);
//}
//bool SELiquidBlackBox::HasSubstanceQuantities() const
//{
//  return !m_SubstanceQuantities.empty();
//}
//bool SELiquidBlackBox::HasSubstanceQuantity(const SESubstance& substance) const
//{
//  for (SELiquidBlackBoxSubstanceQuantity* subQ : m_SubstanceQuantities)
//  {
//    if (&subQ->GetSubstance() == &substance)
//      return true;
//  }
//  return false;
//}
//SELiquidBlackBoxSubstanceQuantity* SELiquidBlackBox::GetSubstanceQuantity(const SESubstance& substance) const
//{
//  for (SELiquidBlackBoxSubstanceQuantity* subQ : m_SubstanceQuantities)
//  {
//    if (&subQ->GetSubstance() == &substance)
//      return subQ;
//  }
//  return nullptr;
//}
//const std::vector<SELiquidBlackBoxSubstanceQuantity*>& SELiquidBlackBox::GetSubstanceQuantities() const
//{
//  return m_SubstanceQuantities;
//}
