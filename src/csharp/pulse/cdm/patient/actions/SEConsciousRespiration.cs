/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  public class SEConsciousRespiration : SEPatientAction
  {
    protected bool start_immediately;
    protected List<SEConsciousRespirationCommand> commands;

    public SEConsciousRespiration()
    {
      start_immediately = false;
      commands = new List<SEConsciousRespirationCommand>();
    }

    public override void Clear()
    {
      base.Clear();
      start_immediately = false;
      commands.Clear();
    }

    public override bool IsValid()
    {
      return HasCommad();
    }

    public bool StartImmediately()
    {
      return start_immediately;
    }
    public void SetStartImmediately(bool b)
    {
      start_immediately = b;
    }

    public bool HasCommad()
    {
      return commands.Count > 0;
    }
    public List<SEConsciousRespirationCommand> GetCommands()
    {
      return commands;
    }

    public SEForcedExhale AddForcedExhale()
    {
      SEForcedExhale c = new SEForcedExhale();
      commands.Add(c);
      return c;
    }
    public SEForcedInhale AddForcedInhale()
    {
      SEForcedInhale c = new SEForcedInhale();
      commands.Add(c);
      return c;
    }
    public SEForcedPause AddForcedPause()
    {
      SEForcedPause c = new SEForcedPause();
      commands.Add(c);
      return c;
    }
    public SEUseInhaler AddUseInhaler()
    {
      SEUseInhaler c = new SEUseInhaler();
      commands.Add(c);
      return c;
    }
  }

  public abstract class SEConsciousRespirationCommand
  {
    protected string comment;

    public SEConsciousRespirationCommand()
    {
      comment = null;
    }

    public void Copy(SEConsciousRespirationCommand other)
    {
      Clear();
      comment = other.comment;
    }

    public virtual void Clear()
    {
      comment = null;
    }

    public abstract bool IsValid();

    public bool HasComment()
    {
      return !string.IsNullOrEmpty(comment);
    }
    public string GetComment()
    {
      return this.comment;
    }
    public void SetComment(string comment)
    {
      this.comment = comment;
    }
    public void InvalidateComment()
    {
      this.comment = null;
    }
  }

  public class SEForcedExhale : SEConsciousRespirationCommand
  {
    protected SEScalar0To1 expiratory_reserve_volume_fraction;
    protected SEScalarTime exhale_period;
    protected SEScalarTime hold_period;
    protected SEScalarTime release_period;

    public SEForcedExhale()
    {
      expiratory_reserve_volume_fraction = null;
      exhale_period = null;
      hold_period = null;
      release_period = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (expiratory_reserve_volume_fraction != null)
        expiratory_reserve_volume_fraction.Invalidate();
      if (exhale_period != null)
        exhale_period.Invalidate();
      if (hold_period != null)
        hold_period.Invalidate();
      if (release_period != null)
        release_period.Invalidate();
    }

    public override bool IsValid()
    {
      return HasExpiratoryReserveVolumeFraction() && HasExhalePeriod();
    }

    public bool HasExpiratoryReserveVolumeFraction()
    {
      return expiratory_reserve_volume_fraction == null ? false : expiratory_reserve_volume_fraction.IsValid();
    }
    public SEScalar0To1 GetExpiratoryReserveVolumeFraction()
    {
      if (expiratory_reserve_volume_fraction == null)
        expiratory_reserve_volume_fraction = new SEScalar0To1();
      return expiratory_reserve_volume_fraction;
    }

    public bool HasExhalePeriod()
    {
      return exhale_period == null ? false : exhale_period.IsValid();
    }
    public SEScalarTime GetExhalePeriod()
    {
      if (exhale_period == null)
        exhale_period = new SEScalarTime();
      return exhale_period;
    }

    public bool HasHoldPeriod()
    {
      return hold_period == null ? false : hold_period.IsValid();
    }
    public SEScalarTime GetHoldPeriod()
    {
      if (hold_period == null)
        hold_period = new SEScalarTime();
      return hold_period;
    }

    public bool HasReleasePeriod()
    {
      return release_period == null ? false : release_period.IsValid();
    }
    public SEScalarTime GetReleasePeriod()
    {
      if (release_period == null)
        release_period = new SEScalarTime();
      return release_period;
    }
  }

  public class SEForcedInhale : SEConsciousRespirationCommand
  {
    protected SEScalar0To1 inspiratory_capacity_fraction;
    protected SEScalarTime inhale_period;
    protected SEScalarTime hold_period;
    protected SEScalarTime release_period;

    public SEForcedInhale()
    {
      inspiratory_capacity_fraction = null;
      inhale_period = null;
      hold_period = null;
      release_period = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (inspiratory_capacity_fraction != null)
        inspiratory_capacity_fraction.Invalidate();
      if (inhale_period != null)
        inhale_period.Invalidate();
      if (hold_period != null)
        hold_period.Invalidate();
      if (release_period != null)
        release_period.Invalidate();
    }

    public override bool IsValid()
    {
      return HasInspiratoryCapacityFraction() && HasInhalePeriod();
    }

    public bool HasInspiratoryCapacityFraction()
    {
      return inspiratory_capacity_fraction == null ? false : inspiratory_capacity_fraction.IsValid();
    }
    public SEScalar0To1 GetInspiratoryCapacityFraction()
    {
      if (inspiratory_capacity_fraction == null)
        inspiratory_capacity_fraction = new SEScalar0To1();
      return inspiratory_capacity_fraction;
    }

    public bool HasInhalePeriod()
    {
      return inhale_period == null ? false : inhale_period.IsValid();
    }
    public SEScalarTime GetInhalePeriod()
    {
      if (inhale_period == null)
        inhale_period = new SEScalarTime();
      return inhale_period;
    }

    public bool HasHoldPeriod()
    {
      return hold_period == null ? false : hold_period.IsValid();
    }
    public SEScalarTime GetHoldPeriod()
    {
      if (hold_period == null)
        hold_period = new SEScalarTime();
      return hold_period;
    }

    public bool HasReleasePeriod()
    {
      return release_period == null ? false : release_period.IsValid();
    }
    public SEScalarTime GetReleasePeriod()
    {
      if (release_period == null)
        release_period = new SEScalarTime();
      return release_period;
    }
  }

  public class SEForcedPause : SEConsciousRespirationCommand
  {
    protected SEScalarTime period;

    public SEForcedPause()
    {
      period = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (period != null)
        period.Invalidate();
    }

    public override bool IsValid()
    {
      return HasPeriod();
    }

    public bool HasPeriod()
    {
      return period == null ? false : period.IsValid();
    }
    public SEScalarTime GetPeriod()
    {
      if (period == null)
        period = new SEScalarTime();
      return period;
    }
  }

  public class SEUseInhaler : SEConsciousRespirationCommand
  {

    public SEUseInhaler()
    {

    }

    public override void Clear()
    {
      base.Clear();
    }

    public override bool IsValid()
    {
      return true;
    }
  }
}
