/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEConsumeNutrients : SEPatientAction
  {
    protected SENutrition nutrition = null;
    protected string      nutrition_file = "";

    public SEConsumeNutrients()
    {

    }

    public SEConsumeNutrients(SEConsumeNutrients other)
    {
      Copy(other);
    }

    public override void Clear()
    {
      base.Clear();
      if(nutrition !=  null)
        nutrition.Clear();
      nutrition_file = null;
    }

    public void Copy(SEConsumeNutrients other)
    {
      if (this == other)
        return;
      base.Copy(other);
      if(other.nutrition != null)
        GetNutrition().Copy(other.nutrition);
      nutrition_file = other.nutrition_file;
    }

    public bool IsActive()
    {
      return IsValid();
    }

    public override bool IsValid()
    {
      return HasNutrition() || HasNutritionFile();
    }
    
    public bool HasNutrition()
    {
      return nutrition != null;
    }
    public SENutrition GetNutrition()
    {
      if (nutrition == null)
        nutrition = new SENutrition();
      return nutrition;
    }
    public void InvalidateNutrition()
    {
      nutrition = null;
    }

    public bool HasNutritionFile()
    {
      return nutrition_file != null && nutrition_file.Length > 0;
    }
    public string GetNutritionFile()
    {
      return nutrition_file;
    }
    public void SetNutritionFile(string s)
    {
      nutrition_file = s;
    }

    public new string ToString()
    {
      if (HasNutritionFile())
        return "Consume Nutrients:"
            + "\n\tNutritionFile: " + nutrition_file;
      else if (nutrition != null)
        return "Consume Nutrients : \n" + nutrition.ToString();
      else
        return "Action not specified properly";
    }

  }
}
