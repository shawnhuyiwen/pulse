/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/fluid/SEFluidCompartmentLink.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
class SEGasBlackBox;
class SEGasCompartment;

class CDM_DECL SEGasCompartmentLink : public SEFluidCompartmentLink<SEGasTransportEdge, SEGasTransportVertex, SEGasCompartment>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SEGasCompartmentLink(SEGasCompartment& src, SEGasCompartment & tgt, const std::string& name) : SEFluidCompartmentLink(src,tgt,name) {}
public:
  ~SEGasCompartmentLink() {}

protected:
};
