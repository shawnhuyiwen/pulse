/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  // TODO Substance Fractions and Concentrations!!!
  public class SEBagValveMask : SEEquipment
  {
    // Keep enums in sync with appropriate schema/cdm/BagValveMask.proto file !!
    public enum Connection : int
    {
      NullConnection = 0,/** Signals not provided, or no change */
      Off,
      Mask,
      Tube
    }

    protected Connection connection;
    protected SEScalarPressureTimePerVolume bag_resistance;
    protected SEScalarPressureTimePerVolume filter_resistance;
    protected SEScalarVolume filter_volume;
    protected SEScalarPressure valve_positive_end_expired_pressure;
    protected SEScalarPressureTimePerVolume valve_resistance;

    protected List<SESubstanceFraction> fraction_inspired_gases;
    protected List<SESubstanceConcentration> concentration_inspired_aerosols;

    public SEBagValveMask()
    {
      connection = Connection.NullConnection;
      filter_resistance = null;
      filter_volume = null;
      valve_positive_end_expired_pressure = null;
      valve_resistance = null;

      this.fraction_inspired_gases = new List<SESubstanceFraction>();
      this.concentration_inspired_aerosols = new List<SESubstanceConcentration>();
    }

    public override void Clear()
    {
      base.Clear();
      connection = Connection.NullConnection;
      if (bag_resistance != null)
        bag_resistance.Invalidate();
      if (filter_resistance != null)
        filter_resistance.Invalidate();
      if (filter_volume != null)
        filter_volume.Invalidate();
      if (valve_positive_end_expired_pressure != null)
        valve_positive_end_expired_pressure.Invalidate();
      if (valve_resistance != null)
        valve_resistance.Invalidate();

      fraction_inspired_gases.Clear();
      concentration_inspired_aerosols.Clear();
    }

    public void Copy(SEBagValveMask from)
    {
      base.Copy(from);
      if (from.connection != Connection.NullConnection)
        this.connection = from.connection;
      if (from.HasBagResistance())
        this.GetBagResistance().Set(from.GetBagResistance());
      if (from.HasFilterResistance())
        this.GetFilterResistance().Set(from.GetFilterResistance());
      if (from.HasFilterVolume())
        this.GetFilterVolume().Set(from.GetFilterVolume());
      if (from.HasValvePositiveEndExpiredPressure())
        this.GetValvePositiveEndExpiredPressure().Set(from.GetValvePositiveEndExpiredPressure());
      if (from.HasValveResistance())
        this.GetValveResistance().Set(from.GetValveResistance());

      if (from.fraction_inspired_gases != null)
      {
        SESubstanceFraction mine;
        foreach (SESubstanceFraction sf in from.fraction_inspired_gases)
        {
          mine = this.CreateFractionInspiredGas(sf.GetSubstance());
          if (sf.HasFractionAmount())
            mine.GetFractionAmount().Set(sf.GetFractionAmount());
        }
      }

      if (from.concentration_inspired_aerosols != null)
      {
        SESubstanceConcentration mine;
        foreach (SESubstanceConcentration sc in from.concentration_inspired_aerosols)
        {
          mine = this.CreateConcentrationInspiredAerosol(sc.GetSubstance());
          if (sc.HasConcentration())
            mine.GetConcentration().Set(sc.GetConcentration());
        }
      }
    }

    public Connection GetConnection()
    {
      return connection;
    }
    public void SetConnection(Connection c)
    {
      connection = c;
    }
    public bool HasConnection()
    {
      return connection != Connection.NullConnection;
    }

    public SEScalarPressureTimePerVolume GetBagResistance()
    {
      if (bag_resistance == null)
        bag_resistance = new SEScalarPressureTimePerVolume();
      return bag_resistance;
    }
    public bool HasBagResistance()
    {
      return bag_resistance == null ? false : bag_resistance.IsValid();
    }

    public SEScalarPressureTimePerVolume GetFilterResistance()
    {
      if (filter_resistance == null)
        filter_resistance = new SEScalarPressureTimePerVolume();
      return filter_resistance;
    }
    public bool HasFilterResistance()
    {
      return filter_resistance == null ? false : filter_resistance.IsValid();
    }

    public SEScalarVolume GetFilterVolume()
    {
      if (filter_volume == null)
        filter_volume = new SEScalarVolume();
      return filter_volume;
    }
    public bool HasFilterVolume()
    {
      return filter_volume == null ? false : filter_volume.IsValid();
    }

    public SEScalarPressure GetValvePositiveEndExpiredPressure()
    {
      if (valve_positive_end_expired_pressure == null)
        valve_positive_end_expired_pressure = new SEScalarPressure();
      return valve_positive_end_expired_pressure;
    }
    public bool HasValvePositiveEndExpiredPressure()
    {
      return valve_positive_end_expired_pressure == null ? false : valve_positive_end_expired_pressure.IsValid();
    }

    public SEScalarPressureTimePerVolume GetValveResistance()
    {
      if (valve_resistance == null)
        valve_resistance = new SEScalarPressureTimePerVolume();
      return valve_resistance;
    }
    public bool HasValveResistance()
    {
      return valve_resistance == null ? false : valve_resistance.IsValid();
    }
    public SESubstanceFraction CreateFractionInspiredGas(string substance)
    {
      return GetFractionInspiredGas(substance);
    }
    public SESubstanceFraction GetFractionInspiredGas(string substance)
    {
      foreach (SESubstanceFraction sf in fraction_inspired_gases)
      {
        if (sf.GetSubstance() == substance)
        {
          return sf;
        }
      }
      SESubstanceFraction nsf = new SESubstanceFraction(substance);
      fraction_inspired_gases.Add(nsf);
      return nsf;
    }
    public bool HasFractionInspiredGas()
    {
      return fraction_inspired_gases.Count != 0;
    }
    public bool HasFractionInspiredGas(string substance)
    {
      foreach (SESubstanceFraction sf in fraction_inspired_gases)
      {
        if (sf.GetSubstance() == substance)
        {
          return true;
        }
      }
      return false;
    }
    public List<SESubstanceFraction> GetFractionInspiredGases()
    {
      return fraction_inspired_gases;
    }
    public void RemoveFractionInspiredGas(string substance)
    {
      foreach (SESubstanceFraction sf in fraction_inspired_gases)
      {
        if (sf.GetSubstance() == substance)
        {
          fraction_inspired_gases.Remove(sf);
          return;
        }
      }
    }
    public void RemoveFractionInspiredGases()
    {
      fraction_inspired_gases.Clear();
    }

    public SESubstanceConcentration CreateConcentrationInspiredAerosol(string substance)
    {
      return GetConcentrationInspiredAerosol(substance);
    }
    public SESubstanceConcentration GetConcentrationInspiredAerosol(string substance)
    {
      foreach (SESubstanceConcentration sc in concentration_inspired_aerosols)
      {
        if (sc.GetSubstance() == substance)
        {
          return sc;
        }
      }
      SESubstanceConcentration nsc = new SESubstanceConcentration(substance);
      concentration_inspired_aerosols.Add(nsc);
      return nsc;
    }
    public bool HasConcentrationInspiredAerosol()
    {
      return concentration_inspired_aerosols.Count != 0;
    }
    public bool HasConcentrationInspiredAerosol(string substance)
    {
      foreach (SESubstanceConcentration sc in concentration_inspired_aerosols)
      {
        if (sc.GetSubstance() == substance)
        {
          return true;
        }
      }
      return false;
    }
    public List<SESubstanceConcentration> GetConcentrationInspiredAerosols()
    {
      return concentration_inspired_aerosols;
    }
    public void RemoveConcentrationInspiredAerosol(string substance)
    {
      foreach (SESubstanceConcentration sc in concentration_inspired_aerosols)
      {
        if (sc.GetSubstance() == substance)
        {
          concentration_inspired_aerosols.Remove(sc);
          return;
        }
      }
    }
    public void RemoveConcentrationInspiredAerosols()
    {
      concentration_inspired_aerosols.Clear();
    }

    public override string ToString()
    {
      string str = "Bag Valve Mask"
      + "\n\tConnection: " + (HasConnection() ? PBBagValveMask.Connection_Name(GetConnection()) : "NotProvided")
      + "\n\tBagResistance: " + (HasBagResistance() ? GetBagResistance().ToString() : "Not Provided")
      + "\n\tFilterResistance: " + (HasFilterResistance() ? GetFilterResistance().ToString() : "Not Provided")
      + "\n\tFilterVolume: " + (HasFilterVolume() ? GetFilterVolume().ToString() : "Not Provided")
      + "\n\tValvePositiveEndExpiredPressure: " + (HasValvePositiveEndExpiredPressure() ? GetValvePositiveEndExpiredPressure().ToString() : "Not Provided")
      + "\n\tValveResistance: " + (HasValveResistance() ? GetValveResistance().ToString() : "Not Provided");
      foreach (SESubstanceFraction sf in this.fraction_inspired_gases)
        str += "\n\t" + sf.GetSubstance();
      foreach (SESubstanceConcentration sc in this.concentration_inspired_aerosols)
        str += "\n\t" + sc.GetSubstance();

      return str;
    }
  }
}