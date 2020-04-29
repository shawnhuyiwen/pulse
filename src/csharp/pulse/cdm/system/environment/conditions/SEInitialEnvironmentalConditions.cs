/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEInitialEnvironmentalConditions : SEEnvironmentCondition
  {
    protected SEEnvironmentalConditions environmental_conditions;
    protected string                    environmental_conditions_file;

    public SEInitialEnvironmentalConditions()
    {
      environmental_conditions = new SEEnvironmentalConditions();
    }

    public SEInitialEnvironmentalConditions(SEInitialEnvironmentalConditions other)
    {
      Copy(other);
    }

    public void Copy(SEInitialEnvironmentalConditions other)
    {
      if (this == other)
        return;

      environmental_conditions.Copy(other.environmental_conditions);
      environmental_conditions_file = other.environmental_conditions_file;
    }

    public override void Clear()
    {
      base.Clear();
      environmental_conditions.Clear();
      environmental_conditions_file = "";
    }

    public string GetName()
    {
      return "InitialEnvironmentalConditions";
    }


    public override bool IsValid()
    {
      return HasEnvironmentalConditions() || HasEnvironmentalConditionsFile();
    }

    public bool IsActive()
    {
      return IsValid();
    }

    public bool HasEnvironmentalConditions()
    {
      return environmental_conditions != null;
    }
    public SEEnvironmentalConditions GetEnvironmentalConditions()
    {
      return environmental_conditions;
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
    public void InvalidateEnvironmentalConditionsFile()
    {
      environmental_conditions_file = null;
    }


    public new string ToString()
    {
      if (environmental_conditions != null)
        return "Initial Environmental Conditions : " + environmental_conditions.ToString();
      else if (HasEnvironmentalConditionsFile())
        return "Initial Envrionmental Conditions File : " + environmental_conditions_file;
      else
        return "Condition not specified properly";
    }

  }
}
