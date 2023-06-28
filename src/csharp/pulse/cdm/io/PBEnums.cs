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
    public static string Name(this eBreathState w)
    {
      return ((pulse.cdm.bind.eBreathState)(int)w).ToString();
    }

    public static string Name(this eDefaultType t)
    {
      return ((pulse.cdm.bind.eDefaultType)(int)t).ToString();
    }

    public static string Name(this eDriverWaveform w)
    {
      return ((pulse.cdm.bind.eDriverWaveform)(int)w).ToString();
    }

  }
}