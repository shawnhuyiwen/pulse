/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "compartment/SECompartmentLink.h"
#include "circuit/SECircuitManager.h"

SECompartmentLink::SECompartmentLink(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name)
{
  
}

SECompartmentLink::~SECompartmentLink()
{
  Clear();
}

void SECompartmentLink::Clear()
{
 
}

std::string SECompartmentLink::GetName() const
{
  return m_Name;
}



