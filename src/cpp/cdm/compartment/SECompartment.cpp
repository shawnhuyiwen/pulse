/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "compartment/SECompartment.h"
#include "circuit/SECircuitManager.h"

SECompartment::SECompartment(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name)
{
  
}

SECompartment::~SECompartment()
{
  Clear();
}

void SECompartment::Clear()
{
  
}

std::string SECompartment::GetName() const
{
  return m_Name;
}
