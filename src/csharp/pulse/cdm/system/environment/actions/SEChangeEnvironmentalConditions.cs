/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEChangeEnvironmentalConditions : SEEnvironmentAction
  {
    protected SEEnvironmentalConditions environmental_conditions;
    protected string                    environmental_conditions_file;

    public SEChangeEnvironmentalConditions()
    {
      environmental_conditions = null;
      environmental_conditions_file = null;
    }

    public SEChangeEnvironmentalConditions(SEChangeEnvironmentalConditions other)
    {
      Copy(other);
    }

    public override void Clear()
    {
      base.Clear();
      environmental_conditions = null;
      environmental_conditions_file = null;

    }

    public void Copy(SEChangeEnvironmentalConditions other)
    {
      if (this == other)
        return;
      base.Copy(other);
      environmental_conditions.Copy(other.environmental_conditions);
      environmental_conditions_file = other.environmental_conditions_file;
    }

    public bool IsActive()
    {
      return IsValid();
    }

    public override bool IsValid()
    {
      return HasEnvironmentalConditions() || HasEnvironmentalConditionsFile();
    }
    
    public bool HasEnvironmentalConditions()
    {
      return environmental_conditions != null;
    }
    public SEEnvironmentalConditions GetEnvironmentalConditions()
    {
      if (environmental_conditions == null)
        environmental_conditions = new SEEnvironmentalConditions();
      return environmental_conditions;
    }
    public void InvalidateEnvironmentalConditions()
    {
      environmental_conditions = null;
    }

    public bool HasEnvironmentalConditionsFile()
    {
      return environmental_conditions_file != null && environmental_conditions_file.Length > 0;
    }
    public string GetEnvironmentalConditionsFile()
    {
      return environmental_conditions_file;
    }
    public void SetEnvironmentalConditionsFile(string s)
    {
      environmental_conditions_file = s;
    }

    public new string ToString()
    {
      if (environmental_conditions != null)
        return "Environment Configuration : " + environmental_conditions.ToString();
      else if (HasEnvironmentalConditionsFile())
        return "Envrioment Configuration:"
            + "\n\tEnvironmentalConditionsFile: " + environmental_conditions_file;
      else
        return "Action not specified properly";
    }

  }
}
