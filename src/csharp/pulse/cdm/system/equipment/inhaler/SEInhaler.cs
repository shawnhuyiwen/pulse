/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  // TODO Substance Fractions and Concentrations!!!
  public class SEInhaler : SEEquipment
  {
    protected eSwitch state;
    protected SEScalarMass meteredDose;
    protected SEScalar0To1 nozzleLoss;
    protected SEScalarVolume spacerVolume;
    protected string substance;

    public SEInhaler()
    {
      Clear();
    }

    public override void Clear()
    {
      state = eSwitch.Off;
      if (meteredDose != null)
        meteredDose.Invalidate();
      if (nozzleLoss != null)
        nozzleLoss.Invalidate();
      if (spacerVolume != null)
        spacerVolume.Invalidate();
      substance = "";
    }

    public void Copy(SEInhaler from)
    {
      Clear();
      setState(from.GetState());
      meteredDose = from.GetMeteredDose();
      nozzleLoss = from.GetNozzleLoss();
      spacerVolume = from.GetSpacerVolume();
      substance = from.substance;
    }
    
    public eSwitch GetState()
    {
      return state;
    }
    public void setState(eSwitch s)
    {
      this.state = (s == eSwitch.NullSwitch) ? eSwitch.Off : s;
    }

    public bool HasMeteredDose()
    {
      return meteredDose == null ? false : meteredDose.IsValid();
    }
    public SEScalarMass GetMeteredDose()
    {
      if (meteredDose == null)
        meteredDose = new SEScalarMass();
      return meteredDose;
    }

    public bool HasNozzleLoss()
    {
      return nozzleLoss == null ? false : nozzleLoss.IsValid();
    }
    public SEScalar0To1 GetNozzleLoss()
    {
      if (nozzleLoss == null)
        nozzleLoss = new SEScalar0To1();
      return nozzleLoss;
    }

    public bool HasSpacerVolume()
    {
      return spacerVolume == null ? false : spacerVolume.IsValid();
    }
    public SEScalarVolume GetSpacerVolume()
    {
      if (spacerVolume == null)
        spacerVolume = new SEScalarVolume();
      return spacerVolume;
    }

    public string GetSubstance()
    {
      return substance;
    }
    public void SetSubstance(string s)
    {
      this.substance = s == null ? "" : s;
    }
    public bool HasSubstance()
    {
      return substance == null ? false : true;
    }

  public override string ToString()
    {
      string str = "Inhaler:";
      str += "\n\tState: " + GetState();
      str += "\n\tMetered Dose: "; str += (this.HasMeteredDose() ? this.GetMeteredDose().ToString() : "Not Supplied");
      str += "\n\tNozzle Loss: "; str += (this.HasNozzleLoss() ? this.GetNozzleLoss().ToString() : "Not Supplied");
      str += "\n\tSpacer Volume: "; str += (this.HasSpacerVolume() ? this.GetSpacerVolume().ToString() : "Not Supplied");
      str += "\n\tSubstance: "; str += (this.HasSubstance() ? this.GetSubstance().ToString() : "Not Supplied");
      return str;
    }
  }
}
