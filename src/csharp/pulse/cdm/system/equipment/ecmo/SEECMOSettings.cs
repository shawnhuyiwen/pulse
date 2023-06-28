/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  // Keep enums in sync with appropriate schema/cdm/ECMO.proto file !!
  public enum eECMO_CannulationLocation : int
  {
    NullCannulationLocation = 0,
    InternalJugular,
    RightFemoralVein,
    LeftFemoralVein,
    RightSubclavianVein,
    LeftSubclavianVein
  }

  public class SEECMOSettings : SEEquipment
  {
    protected eECMO_CannulationLocation      inflow_location;
    protected eECMO_CannulationLocation      outflow_location;
    protected SEScalarVolume                 oxygenator_volume;
    protected SEScalarVolumePerTime          transfusion_flow;
    protected string                         substance_compound;
    protected List<SESubstanceConcentration> substance_concentrations;

    public SEECMOSettings()
    {
      inflow_location = eECMO_CannulationLocation.NullCannulationLocation;
      outflow_location = eECMO_CannulationLocation.NullCannulationLocation;
      oxygenator_volume = null;
      transfusion_flow = null;
      substance_compound = null;
      this.substance_concentrations = new List<SESubstanceConcentration>();
    }

    public override void Clear()
    {
      base.Clear();
      inflow_location = eECMO_CannulationLocation.NullCannulationLocation;
      outflow_location = eECMO_CannulationLocation.NullCannulationLocation;
      if (oxygenator_volume != null)
        oxygenator_volume.Invalidate();
      if (transfusion_flow != null)
        transfusion_flow.Invalidate();
      substance_compound = null;
      substance_concentrations.Clear();
    }

    public void Copy(SEECMOSettings from)
    {
      base.Copy(from);
      if (from.inflow_location != eECMO_CannulationLocation.NullCannulationLocation)
        this.inflow_location = from.inflow_location;
      if (from.outflow_location != eECMO_CannulationLocation.NullCannulationLocation)
        this.outflow_location = from.inflow_location;
      if (from.HasOxygenatorVolume())
        this.GetOxygenatorVolume().Set(from.GetOxygenatorVolume());
      if (from.HasTransfusionFlow())
        this.GetTransfusionFlow().Set(from.GetTransfusionFlow());
      substance_compound = from.substance_compound;

      if (from.substance_concentrations != null)
      {
        SESubstanceConcentration mine;
        foreach (SESubstanceConcentration sc in from.substance_concentrations)
        {
          mine = this.CreateSubstanceConcentration(sc.GetSubstance());
          if (sc.HasConcentration())
            mine.GetConcentration().Set(sc.GetConcentration());
        }
      }
    }

    public eECMO_CannulationLocation GetInflowLocation()
    {
      return inflow_location;
    }
    public void SetInflowLocation(eECMO_CannulationLocation c)
    {
      inflow_location = c;
    }
    public bool HasInflowLocation()
    {
      return inflow_location != eECMO_CannulationLocation.NullCannulationLocation;
    }

    public eECMO_CannulationLocation GetOutflowLocation()
    {
      return outflow_location;
    }
    public void SetOutflowLocation(eECMO_CannulationLocation c)
    {
      outflow_location = c;
    }
    public bool HasOutflowLocation()
    {
      return outflow_location != eECMO_CannulationLocation.NullCannulationLocation;
    }

    public SEScalarVolume GetOxygenatorVolume()
    {
      if (oxygenator_volume == null)
        oxygenator_volume = new SEScalarVolume();
      return oxygenator_volume;
    }
    public bool HasOxygenatorVolume()
    {
      return oxygenator_volume == null ? false : oxygenator_volume.IsValid();
    }

    public SEScalarVolumePerTime GetTransfusionFlow()
    {
      if (transfusion_flow == null)
        transfusion_flow = new SEScalarVolumePerTime();
      return transfusion_flow;
    }
    public bool HasTransfusionFlow()
    {
      return transfusion_flow == null ? false : transfusion_flow.IsValid();
    }

    public string GetSubstanceCompound()
    {
      return substance_compound;
    }
    public void SetSubstanceCompound(string c)
    {
      substance_compound = c;
    }
    public bool HasSubstanceCompound()
    {
      return !string.IsNullOrEmpty(substance_compound);
    }

    public SESubstanceConcentration CreateSubstanceConcentration(string substance)
    {
      return GetSubstanceConcentration(substance);
    }
    public SESubstanceConcentration GetSubstanceConcentration(string substance)
    {
      foreach (SESubstanceConcentration sc in substance_concentrations)
      {
        if (sc.GetSubstance() == substance)
        {
          return sc;
        }
      }
      SESubstanceConcentration nsc = new SESubstanceConcentration(substance);
      substance_concentrations.Add(nsc);
      return nsc;
    }
    public bool HasSubstanceConcentration()
    {
      return substance_concentrations.Count != 0;
    }
    public bool HasSubstanceConcentration(string substance)
    {
      foreach (SESubstanceConcentration sc in substance_concentrations)
      {
        if (sc.GetSubstance() == substance)
        {
          return true;
        }
      }
      return false;
    }
    public List<SESubstanceConcentration> GetSubstanceConcentrations()
    {
      return substance_concentrations;
    }
    public void RemoveSubstanceConcentration(string substance)
    {
      foreach (SESubstanceConcentration sc in substance_concentrations)
      {
        if (sc.GetSubstance() == substance)
        {
          substance_concentrations.Remove(sc);
          return;
        }
      }
    }
    public void RemoveSubstanceConcentrations()
    {
      substance_concentrations.Clear();
    }

    public override string ToString()
    {
      string str = "ECMO Settings"
      + "\n\tInflowLocation: " + (HasInflowLocation() ? eECMO.CannulationLocation_Name(GetInflowLocation()) : "NotProvided")
      + "\n\tOutflowLocation: " + (HasOutflowLocation() ? eECMO.CannulationLocation_Name(GetOutflowLocation()) : "NotProvided")
      + "\n\tOxygenatorVolume: " + (HasOxygenatorVolume() ? GetOxygenatorVolume().ToString() : "Not Provided")
      + "\n\tTransfusionFlow: " + (HasTransfusionFlow() ? GetTransfusionFlow().ToString() : "Not Provided")
      + "\n\tSubstanceCompound: " + (HasSubstanceCompound() ? GetSubstanceCompound().ToString() : "Not Provided");

      foreach (SESubstanceConcentration sc in this.substance_concentrations)
        str += "\n\t" + sc.GetSubstance();

      return str;
    }
  }
}