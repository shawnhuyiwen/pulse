/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  static class eEnum
  {
    public static string Name(this eSwitch s)
    {
      return ((pulse.cdm.bind.eSwitch)(int)s).ToString();
    }
  }
}