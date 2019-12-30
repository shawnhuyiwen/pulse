/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/


public class SEPatient
{
  public enum eSex : int
  {
    Male = 0,
    Female
  }

  protected string name;
  protected eSex sex;
  protected SEScalarTime age;
  protected SEScalarMass weight;
  protected SEScalarLength height;
  protected SEScalarMassPerVolume body_density;
  protected SEScalar0To1 body_fat_fraction;
  protected SEScalarMass ideal_body_weight;
  protected SEScalarMass lean_body_mass;

  protected SEScalarArea alveoli_surface_area;
  protected SEScalar0To1 right_lung_ratio;
  protected SEScalarArea skin_surface_area;

  protected SEScalarPower basal_metabolic_rate;
  protected SEScalarVolume blood_volume_baseline;
  protected SEScalarPressure diastolic_arterial_pressure_baseline;
  protected SEScalarFrequency heart_rate_baseline;
  protected SEScalarPressure mean_arterial_pressure_baseline;
  protected SEScalarFrequency respiration_rate_baseline;
  protected SEScalarPressure systolic_arterial_pressure_baseline;
  protected SEScalarVolume tidal_volume_baseline;

  protected SEScalarFrequency heart_rate_maximum;
  protected SEScalarFrequency heart_rate_minimum;
  protected SEScalarVolume expiratory_reserve_volume;
  protected SEScalarVolume functional_residual_capacity;
  protected SEScalarVolume inspiratory_capacity;
  protected SEScalarVolume inspiratory_reserve_volume;
  protected SEScalarVolume residual_volume;
  protected SEScalarVolume total_lung_capacity;
  protected SEScalarVolume vital_capacity;

  public SEPatient()
  {
    name = null;
    sex = eSex.Male;
    age = null;
    weight = null;
    height = null;
    body_density = null;
    body_fat_fraction = null;
    lean_body_mass = null;
    ideal_body_weight = null;

    alveoli_surface_area = null;
    right_lung_ratio = null;
    skin_surface_area = null;

    basal_metabolic_rate = null;
    blood_volume_baseline = null;
    diastolic_arterial_pressure_baseline = null;
    heart_rate_baseline = null;
    mean_arterial_pressure_baseline = null;
    respiration_rate_baseline = null;
    systolic_arterial_pressure_baseline = null;
    tidal_volume_baseline = null;

    heart_rate_maximum = null;
    heart_rate_minimum = null;
    expiratory_reserve_volume = null;
    functional_residual_capacity = null;
    inspiratory_capacity = null;
    inspiratory_reserve_volume = null;
    residual_volume = null;
    total_lung_capacity = null;
    vital_capacity = null;
  }

  public void Clear()
  {
    name = null;
    sex = eSex.Male;
    if (age != null) { age.Invalidate(); }
    if (weight != null) { weight.Invalidate(); }
    if (height != null) { height.Invalidate(); }
    if (body_density != null) { body_density.Invalidate(); }
    if (body_fat_fraction != null) { body_fat_fraction.Invalidate(); }
    if (lean_body_mass != null) { lean_body_mass.Invalidate(); }
    if (ideal_body_weight != null) { ideal_body_weight.Invalidate(); }

    if (alveoli_surface_area != null) { alveoli_surface_area.Invalidate(); }
    if (right_lung_ratio != null) { right_lung_ratio.Invalidate(); }
    if (skin_surface_area != null) { skin_surface_area.Invalidate(); }

    if (basal_metabolic_rate != null) { basal_metabolic_rate.Invalidate(); }
    if (blood_volume_baseline != null) { blood_volume_baseline.Invalidate(); }
    if (diastolic_arterial_pressure_baseline != null) { diastolic_arterial_pressure_baseline.Invalidate(); }
    if (heart_rate_baseline != null) { heart_rate_baseline.Invalidate(); }
    if (mean_arterial_pressure_baseline != null) { mean_arterial_pressure_baseline.Invalidate(); }
    if (respiration_rate_baseline != null) { respiration_rate_baseline.Invalidate(); }
    if (systolic_arterial_pressure_baseline != null) { systolic_arterial_pressure_baseline.Invalidate(); }
    if (tidal_volume_baseline != null) { tidal_volume_baseline.Invalidate(); }

    if (heart_rate_maximum != null) { heart_rate_maximum.Invalidate(); }
    if (heart_rate_minimum != null) { heart_rate_minimum.Invalidate(); }
    if (expiratory_reserve_volume != null) { expiratory_reserve_volume.Invalidate(); }
    if (functional_residual_capacity != null) { functional_residual_capacity.Invalidate(); }
    if (inspiratory_capacity != null) { inspiratory_capacity.Invalidate(); }
    if (inspiratory_reserve_volume != null) { inspiratory_reserve_volume.Invalidate(); }
    if (residual_volume != null) { residual_volume.Invalidate(); }
    if (total_lung_capacity != null) { total_lung_capacity.Invalidate(); }
    if (vital_capacity != null) { vital_capacity.Invalidate(); }
  }

  public void Copy(SEPatient src)
  {
    Clear();
    if (src.HasName()) { name = src.name; }
    sex = src.sex;
    if (src.HasAge()) { GetAge().Set(src.age); }
    if (src.HasWeight()) { GetWeight().Set(src.weight); }
    if (src.HasHeight()) { GetHeight().Set(src.height); }
    if (src.HasBodyDensity()) { GetBodyDensity().Set(src.body_density); }
    if (src.HasBodyFatFraction()) { GetBodyFatFraction().Set(src.body_fat_fraction); }
    if (src.HasLeanBodyMass()) { GetLeanBodyMass().Set(src.lean_body_mass); }
    if (src.HasIdealBodyWeight()) { GetIdealBodyWeight().Set(src.ideal_body_weight); }

    if (src.HasAlveoliSurfaceArea()) { GetAlveoliSurfaceArea().Set(src.alveoli_surface_area); }
    if (src.HasRightLungRatio()) { GetRightLungRatio().Set(src.right_lung_ratio); }
    if (src.HasSkinSurfaceArea()) { GetSkinSurfaceArea().Set(src.skin_surface_area); }

    if (src.HasBasalMetabolicRate()) { GetBasalMetabolicRate().Set(src.basal_metabolic_rate); }
    if (src.HasBloodVolumeBaseline()) { GetBloodVolumeBaseline().Set(src.blood_volume_baseline); }
    if (src.HasDiastolicArterialPressureBaseline()) { GetDiastolicArterialPressureBaseline().Set(src.diastolic_arterial_pressure_baseline); }
    if (src.HasHeartRateBaseline()) { GetHeartRateBaseline().Set(src.heart_rate_baseline); }
    if (src.HasMeanArterialPressureBaseline()) { GetMeanArterialPressureBaseline().Set(src.mean_arterial_pressure_baseline); }
    if (src.HasRespirationRateBaseline()) { GetRespirationRateBaseline().Set(src.respiration_rate_baseline); }
    if (src.HasSystolicArterialPressureBaseline()) { GetSystolicArterialPressureBaseline().Set(src.systolic_arterial_pressure_baseline); }
    if (src.HasTidalVolumeBaseline()) { GetTidalVolumeBaseline().Set(src.tidal_volume_baseline); }

    if (src.HasHeartRateMaximum()) { GetHeartRateMaximum().Set(src.heart_rate_maximum); }
    if (src.HasHeartRateMinimum()) { GetHeartRateMinimum().Set(src.heart_rate_minimum); }
    if (src.HasExpiratoryReserveVolume()) { GetExpiratoryReserveVolume().Set(src.expiratory_reserve_volume); }
    if (src.HasFunctionalResidualCapacity()) { GetFunctionalResidualCapacity().Set(src.functional_residual_capacity); }
    if (src.HasInspiratoryCapacity()) { GetInspiratoryCapacity().Set(src.inspiratory_capacity); }
    if (src.HasInspiratoryReserveVolume()) { GetInspiratoryReserveVolume().Set(src.inspiratory_reserve_volume); }
    if (src.HasResidualVolume()) { GetResidualVolume().Set(src.residual_volume); }
    if (src.HasTotalLungCapacity()) { GetTotalLungCapacity().Set(src.total_lung_capacity); }
    if (src.HasVitalCapacity()) { GetVitalCapacity().Set(src.vital_capacity); }
  }

  public bool SerializeFromFile(string filename, SerializationFormat fmt)
  {
    try
    {
      string json = System.IO.File.ReadAllText(filename);
      return SerializeFromString(json, SerializationFormat.JSON);
    }
    catch(System.Exception ex)
    {
      System.Console.WriteLine("Error Reading Patient File! "+ex.ToString());
      return false;
    }
  }
  public bool SerializeFromString(string src, SerializationFormat fmt)
  {
    return PBPatient.SerializeFromString(src, this);
  }

  public bool SerializeToFile(string filename, SerializationFormat fmt)
  {
    string json = SerializeToString(SerializationFormat.JSON);
    if (json == null || json.Length == 0)
      return false;
    try
    {
      System.IO.File.WriteAllText(filename, json);
    }
    catch (System.Exception ex)
    {
      System.Console.WriteLine("Error Writing Patient File! " + ex.ToString());
      return false;
    }
    return true;
  }
  public string SerializeToString(SerializationFormat format)
  {
    return PBPatient.SerializeToString(this);
  }

  public string GetName() { return name; }
  public void SetName(string n) { name = n; }
  public bool HasName() { return name == null; }
  public void InvalidateName() { name = null; }

  public eSex GetSex() { return sex; }
  public void SetSex(eSex s) { sex = s; }

  public bool HasAge()
  {
    return age == null ? false : age.IsValid();
  }
  public SEScalarTime GetAge()
  {
    if (age == null)
      age = new SEScalarTime();
    return age;
  }

  public bool HasWeight()
  {
    return weight == null ? false : weight.IsValid();
  }
  public SEScalarMass GetWeight()
  {
    if (weight == null)
      weight = new SEScalarMass();
    return weight;
  }

  public bool HasHeight()
  {
    return height == null ? false : height.IsValid();
  }
  public SEScalarLength GetHeight()
  {
    if (height == null)
      height = new SEScalarLength();
    return height;
  }

  public bool HasAlveoliSurfaceArea()
  {
    return alveoli_surface_area == null ? false : alveoli_surface_area.IsValid();
  }
  public SEScalarArea GetAlveoliSurfaceArea()
  {
    if (alveoli_surface_area == null)
      alveoli_surface_area = new SEScalarArea();
    return alveoli_surface_area;
  }

  public bool HasBasalMetabolicRate()
  {
    return basal_metabolic_rate == null ? false : basal_metabolic_rate.IsValid();
  }
  public SEScalarPower GetBasalMetabolicRate()
  {
    if (basal_metabolic_rate == null)
      basal_metabolic_rate = new SEScalarPower();
    return basal_metabolic_rate;
  }

  public bool HasBloodVolumeBaseline()
  {
    return blood_volume_baseline == null ? false : blood_volume_baseline.IsValid();
  }
  public SEScalarVolume GetBloodVolumeBaseline()
  {
    if (blood_volume_baseline == null)
      blood_volume_baseline = new SEScalarVolume();
    return blood_volume_baseline;
  }

  public bool HasBodyDensity()
  {
    return body_density == null ? false : body_density.IsValid();
  }
  public SEScalarMassPerVolume GetBodyDensity()
  {
    if (body_density == null)
      body_density = new SEScalarMassPerVolume();
    return body_density;
  }

  public bool HasBodyFatFraction()
  {
    return body_fat_fraction == null ? false : body_fat_fraction.IsValid();
  }
  public SEScalar0To1 GetBodyFatFraction()
  {
    if (body_fat_fraction == null)
      body_fat_fraction = new SEScalar0To1();
    return body_fat_fraction;
  }

  public bool HasDiastolicArterialPressureBaseline()
  {
    return diastolic_arterial_pressure_baseline == null ? false : diastolic_arterial_pressure_baseline.IsValid();
  }
  public SEScalarPressure GetDiastolicArterialPressureBaseline()
  {
    if (diastolic_arterial_pressure_baseline == null)
      diastolic_arterial_pressure_baseline = new SEScalarPressure();
    return diastolic_arterial_pressure_baseline;
  }

  public bool HasExpiratoryReserveVolume()
  {
    return expiratory_reserve_volume == null ? false : expiratory_reserve_volume.IsValid();
  }
  public SEScalarVolume GetExpiratoryReserveVolume()
  {
    if (expiratory_reserve_volume == null)
      expiratory_reserve_volume = new SEScalarVolume();
    return expiratory_reserve_volume;
  }

  public bool HasFunctionalResidualCapacity()
  {
    return functional_residual_capacity == null ? false : functional_residual_capacity.IsValid();
  }
  public SEScalarVolume GetFunctionalResidualCapacity()
  {
    if (functional_residual_capacity == null)
      functional_residual_capacity = new SEScalarVolume();
    return functional_residual_capacity;
  }

  public bool HasHeartRateBaseline()
  {
    return heart_rate_baseline == null ? false : heart_rate_baseline.IsValid();
  }
  public SEScalarFrequency GetHeartRateBaseline()
  {
    if (heart_rate_baseline == null)
      heart_rate_baseline = new SEScalarFrequency();
    return heart_rate_baseline;
  }

  public bool HasHeartRateMaximum()
  {
    return heart_rate_maximum == null ? false : heart_rate_maximum.IsValid();
  }
  public SEScalarFrequency GetHeartRateMaximum()
  {
    if (heart_rate_maximum == null)
      heart_rate_maximum = new SEScalarFrequency();
    return heart_rate_maximum;
  }

  public bool HasHeartRateMinimum()
  {
    return heart_rate_minimum == null ? false : heart_rate_minimum.IsValid();
  }
  public SEScalarFrequency GetHeartRateMinimum()
  {
    if (heart_rate_minimum == null)
      heart_rate_minimum = new SEScalarFrequency();
    return heart_rate_minimum;
  }

  public bool HasIdealBodyWeight()
  {
    return ideal_body_weight == null ? false : ideal_body_weight.IsValid();
  }
  public SEScalarMass GetIdealBodyWeight()
  {
    if (ideal_body_weight == null)
      ideal_body_weight = new SEScalarMass();
    return ideal_body_weight;
  }

  public bool HasInspiratoryCapacity()
  {
    return inspiratory_capacity == null ? false : inspiratory_capacity.IsValid();
  }
  public SEScalarVolume GetInspiratoryCapacity()
  {
    if (inspiratory_capacity == null)
      inspiratory_capacity = new SEScalarVolume();
    return inspiratory_capacity;
  }

  public bool HasInspiratoryReserveVolume()
  {
    return inspiratory_reserve_volume == null ? false : inspiratory_reserve_volume.IsValid();
  }
  public SEScalarVolume GetInspiratoryReserveVolume()
  {
    if (inspiratory_reserve_volume == null)
      inspiratory_reserve_volume = new SEScalarVolume();
    return inspiratory_reserve_volume;
  }

  public bool HasLeanBodyMass()
  {
    return lean_body_mass == null ? false : lean_body_mass.IsValid();
  }
  public SEScalarMass GetLeanBodyMass()
  {
    if (lean_body_mass == null)
      lean_body_mass = new SEScalarMass();
    return lean_body_mass;
  }

  public bool HasMeanArterialPressureBaseline()
  {
    return mean_arterial_pressure_baseline == null ? false : mean_arterial_pressure_baseline.IsValid();
  }
  public SEScalarPressure GetMeanArterialPressureBaseline()
  {
    if (mean_arterial_pressure_baseline == null)
      mean_arterial_pressure_baseline = new SEScalarPressure();
    return mean_arterial_pressure_baseline;
  }

  public bool HasResidualVolume()
  {
    return residual_volume == null ? false : residual_volume.IsValid();
  }
  public SEScalarVolume GetResidualVolume()
  {
    if (residual_volume == null)
      residual_volume = new SEScalarVolume();
    return residual_volume;
  }

  public bool HasRespirationRateBaseline()
  {
    return respiration_rate_baseline == null ? false : respiration_rate_baseline.IsValid();
  }
  public SEScalarFrequency GetRespirationRateBaseline()
  {
    if (respiration_rate_baseline == null)
      respiration_rate_baseline = new SEScalarFrequency();
    return respiration_rate_baseline;
  }

  public bool HasRightLungRatio()
  {
    return right_lung_ratio == null ? false : right_lung_ratio.IsValid();
  }
  public SEScalar0To1 GetRightLungRatio()
  {
    if (right_lung_ratio == null)
      right_lung_ratio = new SEScalar0To1();
    return right_lung_ratio;
  }

  public bool HasSkinSurfaceArea()
  {
    return skin_surface_area == null ? false : skin_surface_area.IsValid();
  }
  public SEScalarArea GetSkinSurfaceArea()
  {
    if (skin_surface_area == null)
      skin_surface_area = new SEScalarArea();
    return skin_surface_area;
  }

  public bool HasSystolicArterialPressureBaseline()
  {
    return systolic_arterial_pressure_baseline == null ? false : systolic_arterial_pressure_baseline.IsValid();
  }
  public SEScalarPressure GetSystolicArterialPressureBaseline()
  {
    if (systolic_arterial_pressure_baseline == null)
      systolic_arterial_pressure_baseline = new SEScalarPressure();
    return systolic_arterial_pressure_baseline;
  }

  public bool HasTidalVolumeBaseline()
  {
    return tidal_volume_baseline == null ? false : tidal_volume_baseline.IsValid();
  }
  public SEScalarVolume GetTidalVolumeBaseline()
  {
    if (tidal_volume_baseline == null)
      tidal_volume_baseline = new SEScalarVolume();
    return tidal_volume_baseline;
  }

  public bool HasTotalLungCapacity()
  {
    return total_lung_capacity == null ? false : total_lung_capacity.IsValid();
  }
  public SEScalarVolume GetTotalLungCapacity()
  {
    if (total_lung_capacity == null)
      total_lung_capacity = new SEScalarVolume();
    return total_lung_capacity;
  }

  public bool HasVitalCapacity()
  {
    return vital_capacity == null ? false : vital_capacity.IsValid();
  }
  public SEScalarVolume GetVitalCapacity()
  {
    if (vital_capacity == null)
      vital_capacity = new SEScalarVolume();
    return vital_capacity;
  }

}