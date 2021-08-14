/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"

#include "cdm/compartment/substances/SESubstanceQuantity.h"
#include "cdm/substance/SESubstance.h"

SESubstanceQuantity::SESubstanceQuantity(SESubstance& sub) : Loggable(sub.GetLogger()), m_Substance(sub)
{

};
SESubstanceQuantity::~SESubstanceQuantity()
{

};

