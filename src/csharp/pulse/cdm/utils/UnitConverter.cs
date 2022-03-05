using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pulse.CDM
{
  class UnitConverter
  {

    public static double Convert(double d, Unit fromUnit, Unit toUnit)
    {
      if (fromUnit == toUnit)
        return d;
      if (fromUnit.ToString() == toUnit.ToString())
        return d;
      if (!PulseEngine.AreCompatibleUnits(fromUnit.ToString(), toUnit.ToString()))
        throw new System.InvalidOperationException("Cannot convert between incompatible units");
      return PulseEngine.ConvertValue(d, fromUnit.ToString(), toUnit.ToString());
    }
  }
}
