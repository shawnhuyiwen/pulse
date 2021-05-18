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
      // Unit converter not implemented yet...
      throw new System.NotImplementedException();
    }
  }
}
