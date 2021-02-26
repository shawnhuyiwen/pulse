/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SECompleteBloodCount : SEPatientAssessment
  {
    protected SEScalar0To1 hematocrit;
    protected SEScalarMassPerVolume hemoglobin;
    protected SEScalarAmountPerVolume plateletCount;
    protected SEScalarMassPerAmount mean_corpuscular_hemoglobin;
    protected SEScalarMassPerVolume mean_corpuscular_hemoglobin_concentration;
    protected SEScalarVolume mean_corpuscular_volume;
    protected SEScalarAmountPerVolume red_blood_cell_count;
    protected SEScalarAmountPerVolume white_blood_cell_count;

    public override void Clear()
    {
      base.Clear();
      if (this.hematocrit != null)
        this.hematocrit.Invalidate();
      if (this.hemoglobin != null)
        this.hemoglobin.Invalidate();
      if (this.plateletCount != null)
        this.plateletCount.Invalidate();
      if (this.mean_corpuscular_hemoglobin != null)
        this.mean_corpuscular_hemoglobin.Invalidate();
      if (this.mean_corpuscular_hemoglobin_concentration != null)
        this.mean_corpuscular_hemoglobin_concentration.Invalidate();
      if (this.mean_corpuscular_volume != null)
        this.mean_corpuscular_volume.Invalidate();
      if (this.red_blood_cell_count != null)
        this.red_blood_cell_count.Invalidate();
      if (this.white_blood_cell_count != null)
        this.white_blood_cell_count.Invalidate();
    }

    public bool HasHematocrit()
    {
      return hematocrit == null ? false : hematocrit.IsValid();
    }
    public SEScalar0To1 GetHematocrit()
    {
      if (hematocrit == null)
        hematocrit = new SEScalar0To1();
      return hematocrit;
    }

    public bool HasHemoglobin()
    {
      return hemoglobin == null ? false : hemoglobin.IsValid();
    }
    public SEScalarMassPerVolume GetHemoglobin()
    {
      if (hemoglobin == null)
        hemoglobin = new SEScalarMassPerVolume();
      return hemoglobin;
    }

    public bool HasPlateletCount()
    {
      return plateletCount == null ? false : plateletCount.IsValid();
    }
    public SEScalarAmountPerVolume GetPlateletCount()
    {
      if (plateletCount == null)
        plateletCount = new SEScalarAmountPerVolume();
      return plateletCount;
    }

    public bool HasMeanCorpuscularHemoglobin()
    {
      return mean_corpuscular_hemoglobin == null ? false : mean_corpuscular_hemoglobin.IsValid();
    }
    public SEScalarMassPerAmount GetMeanCorpuscularHemoglobin()
    {
      if (mean_corpuscular_hemoglobin == null)
        mean_corpuscular_hemoglobin = new SEScalarMassPerAmount();
      return mean_corpuscular_hemoglobin;
    }

    public bool HasMeanCorpuscularHemoglobinConcentration()
    {
      return mean_corpuscular_hemoglobin_concentration == null ? false : mean_corpuscular_hemoglobin_concentration.IsValid();
    }
    public SEScalarMassPerVolume GetMeanCorpuscularHemoglobinConcentration()
    {
      if (mean_corpuscular_hemoglobin_concentration == null)
        mean_corpuscular_hemoglobin_concentration = new SEScalarMassPerVolume();
      return mean_corpuscular_hemoglobin_concentration;
    }

    public bool HasMeanCorpuscularVolume()
    {
      return mean_corpuscular_volume == null ? false : mean_corpuscular_volume.IsValid();
    }
    public SEScalarVolume GetMeanCorpuscularVolume()
    {
      if (mean_corpuscular_volume == null)
        mean_corpuscular_volume = new SEScalarVolume();
      return mean_corpuscular_volume;
    }

    public bool HasRedBloodCellCount()
    {
      return red_blood_cell_count == null ? false : red_blood_cell_count.IsValid();
    }
    public SEScalarAmountPerVolume GetRedBloodCellCount()
    {
      if (red_blood_cell_count == null)
        red_blood_cell_count = new SEScalarAmountPerVolume();
      return red_blood_cell_count;
    }

    public bool HasWhiteBloodCellCount()
    {
      return white_blood_cell_count == null ? false : white_blood_cell_count.IsValid();
    }
    public SEScalarAmountPerVolume GetWhiteBloodCellCount()
    {
      if (white_blood_cell_count == null)
        white_blood_cell_count = new SEScalarAmountPerVolume();
      return white_blood_cell_count;
    }

  }
}