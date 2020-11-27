/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "blackbox/electrical/SEElectricalBlackBox.h"

SEElectricalBlackBox::SEElectricalBlackBox(const std::string& name, Logger* logger) : SEBlackBox<ELECTRICAL_BLACK_BOX>(name,logger)
{

}
SEElectricalBlackBox::~SEElectricalBlackBox()
{
  Clear();
}
