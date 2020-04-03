/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "MVController.h"

int main(int argc, char* argv[])
{
  MVController mvc("MultiplexVentilation.log");
  mvc.AdvanceModelTime(60, TimeUnit::s);
  return 0;
}
