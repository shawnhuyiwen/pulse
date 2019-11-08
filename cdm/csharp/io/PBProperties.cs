/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

public class PBProperty
{
  #region SEScalar
  public static void Load(Cdm.ScalarData src, SEScalar dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarData src, SEScalar dst)
  {
    dst.Invalidate();
    dst.SetValue(src.Value);
  }
  public static Cdm.ScalarData Unload(SEScalar src)
  {
    Cdm.ScalarData dst = new Cdm.ScalarData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEScalar src, Cdm.ScalarData dst)
  {
    dst.Value = src.GetValue();
    dst.Unit = "";
  }
  #endregion

  #region SEScalar0To1
  public static void Load(Cdm.Scalar0To1Data src, SEScalar0To1 dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.Scalar0To1Data src, SEScalar0To1 dst)
  {
    dst.Invalidate();
    dst.SetValue(src.Scalar0To1.Value);
  }
  public static Cdm.Scalar0To1Data Unload(SEScalar0To1 src)
  {
    Cdm.Scalar0To1Data dst = new Cdm.Scalar0To1Data();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEScalar0To1 src, Cdm.Scalar0To1Data dst)
  {
    dst.Scalar0To1 = new Cdm.ScalarData();
    dst.Scalar0To1.Value = src.GetValue();
    dst.Scalar0To1.Unit = "";
  }
  #endregion

  #region SEScalarFrequency
  public static void Load(Cdm.ScalarFrequencyData src, SEScalarFrequency dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarFrequencyData src, SEScalarFrequency dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarFrequency.Value, FrequencyUnit.FromString(src.ScalarFrequency.Unit));
  }
  public static Cdm.ScalarFrequencyData Unload(SEScalarFrequency src)
  {
    Cdm.ScalarFrequencyData dst = new Cdm.ScalarFrequencyData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarFrequency src, Cdm.ScalarFrequencyData dst)
  {
    dst.ScalarFrequency = new Cdm.ScalarData();
    dst.ScalarFrequency.Value = src.GetValue();
    dst.ScalarFrequency.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarInversePressure
  public static void Load(Cdm.ScalarInversePressureData src, SEScalarInversePressure dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarInversePressureData src, SEScalarInversePressure dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarInversePressure.Value, InversePressureUnit.FromString(src.ScalarInversePressure.Unit));
  }
  public static Cdm.ScalarInversePressureData Unload(SEScalarInversePressure src)
  {
    Cdm.ScalarInversePressureData dst = new Cdm.ScalarInversePressureData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarInversePressure src, Cdm.ScalarInversePressureData dst)
  {
    dst.ScalarInversePressure = new Cdm.ScalarData();
    dst.ScalarInversePressure.Value = src.GetValue();
    dst.ScalarInversePressure.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarMass
  public static void Load(Cdm.ScalarMassData src, SEScalarMass dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarMassData src, SEScalarMass dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarMass.Value, MassUnit.FromString(src.ScalarMass.Unit));
  }
  public static Cdm.ScalarMassData Unload(SEScalarMass src)
  {
    Cdm.ScalarMassData dst = new Cdm.ScalarMassData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarMass src, Cdm.ScalarMassData dst)
  {
    dst.ScalarMass = new Cdm.ScalarData();
    dst.ScalarMass.Value = src.GetValue();
    dst.ScalarMass.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarMassPerAmount
  public static void Load(Cdm.ScalarMassPerAmountData src, SEScalarMassPerAmount dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarMassPerAmountData src, SEScalarMassPerAmount dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarMassPerAmount.Value, MassPerAmountUnit.FromString(src.ScalarMassPerAmount.Unit));
  }
  public static Cdm.ScalarMassPerAmountData Unload(SEScalarMassPerAmount src)
  {
    Cdm.ScalarMassPerAmountData dst = new Cdm.ScalarMassPerAmountData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarMassPerAmount src, Cdm.ScalarMassPerAmountData dst)
  {
    dst.ScalarMassPerAmount = new Cdm.ScalarData();
    dst.ScalarMassPerAmount.Value = src.GetValue();
    dst.ScalarMassPerAmount.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarMassPerAreaTime
  public static void Load(Cdm.ScalarMassPerAreaTimeData src, SEScalarMassPerAreaTime dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarMassPerAreaTimeData src, SEScalarMassPerAreaTime dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarMassPerAreaTime.Value, MassPerAreaTimeUnit.FromString(src.ScalarMassPerAreaTime.Unit));
  }
  public static Cdm.ScalarMassPerAreaTimeData Unload(SEScalarMassPerAreaTime src)
  {
    Cdm.ScalarMassPerAreaTimeData dst = new Cdm.ScalarMassPerAreaTimeData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarMassPerAreaTime src, Cdm.ScalarMassPerAreaTimeData dst)
  {
    dst.ScalarMassPerAreaTime = new Cdm.ScalarData();
    dst.ScalarMassPerAreaTime.Value = src.GetValue();
    dst.ScalarMassPerAreaTime.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarMassPerTime
  public static void Load(Cdm.ScalarMassPerTimeData src, SEScalarMassPerTime dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarMassPerTimeData src, SEScalarMassPerTime dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarMassPerTime.Value, MassPerTimeUnit.FromString(src.ScalarMassPerTime.Unit));
  }
  public static Cdm.ScalarMassPerTimeData Unload(SEScalarMassPerTime src)
  {
    Cdm.ScalarMassPerTimeData dst = new Cdm.ScalarMassPerTimeData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarMassPerTime src, Cdm.ScalarMassPerTimeData dst)
  {
    dst.ScalarMassPerTime = new Cdm.ScalarData();
    dst.ScalarMassPerTime.Value = src.GetValue();
    dst.ScalarMassPerTime.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarMassPerVolume
  public static void Load(Cdm.ScalarMassPerVolumeData src, SEScalarMassPerVolume dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarMassPerVolumeData src, SEScalarMassPerVolume dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarMassPerVolume.Value, MassPerVolumeUnit.FromString(src.ScalarMassPerVolume.Unit));
  }
  public static Cdm.ScalarMassPerVolumeData Unload(SEScalarMassPerVolume src)
  {
    Cdm.ScalarMassPerVolumeData dst = new Cdm.ScalarMassPerVolumeData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarMassPerVolume src, Cdm.ScalarMassPerVolumeData dst)
  {
    dst.ScalarMassPerVolume = new Cdm.ScalarData();
    dst.ScalarMassPerVolume.Value = src.GetValue();
    dst.ScalarMassPerVolume.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarNegative1To1
  public static void Load(Cdm.ScalarNegative1To1Data src, SEScalarNegative1To1 dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarNegative1To1Data src, SEScalarNegative1To1 dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarNegative1To1.Value);
  }
  public static Cdm.ScalarNegative1To1Data Unload(SEScalarNegative1To1 src)
  {
    Cdm.ScalarNegative1To1Data dst = new Cdm.ScalarNegative1To1Data();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEScalarNegative1To1 src, Cdm.ScalarNegative1To1Data dst)
  {
    dst.ScalarNegative1To1 = new Cdm.ScalarData();
    dst.ScalarNegative1To1.Value = src.GetValue();
    dst.ScalarNegative1To1.Unit = "";
  }
  #endregion

  #region SEScalarPressure
  public static void Load(Cdm.ScalarPressureData src, SEScalarPressure dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarPressureData src, SEScalarPressure dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarPressure.Value, PressureUnit.FromString(src.ScalarPressure.Unit));
  }
  public static Cdm.ScalarPressureData Unload(SEScalarPressure src)
  {
    Cdm.ScalarPressureData dst = new Cdm.ScalarPressureData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarPressure src, Cdm.ScalarPressureData dst)
  {
    dst.ScalarPressure = new Cdm.ScalarData();
    dst.ScalarPressure.Value = src.GetValue();
    dst.ScalarPressure.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarTime
  public static void Load(Cdm.ScalarTimeData src, SEScalarTime dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarTimeData src, SEScalarTime dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarTime.Value, TimeUnit.FromString(src.ScalarTime.Unit));
  }
  public static Cdm.ScalarTimeData Unload(SEScalarTime src)
  {
    Cdm.ScalarTimeData dst = new Cdm.ScalarTimeData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEScalarTime src, Cdm.ScalarTimeData dst)
  {
    dst.ScalarTime = new Cdm.ScalarData();
    dst.ScalarTime.Value = src.GetValue();
    dst.ScalarTime.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarVolume
  public static void Load(Cdm.ScalarVolumeData src, SEScalarVolume dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarVolumeData src, SEScalarVolume dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarVolume.Value, VolumeUnit.FromString(src.ScalarVolume.Unit));
  }
  public static Cdm.ScalarVolumeData Unload(SEScalarVolume src)
  {
    Cdm.ScalarVolumeData dst = new Cdm.ScalarVolumeData();
    Serialize(src, dst);
    return dst;
  }
  public static void Serialize(SEScalarVolume src, Cdm.ScalarVolumeData dst)
  {
    dst.ScalarVolume = new Cdm.ScalarData();
    dst.ScalarVolume.Value = src.GetValue();
    dst.ScalarVolume.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarVolumePerTime
  public static void Load(Cdm.ScalarVolumePerTimeData src, SEScalarVolumePerTime dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarVolumePerTimeData src, SEScalarVolumePerTime dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarVolumePerTime.Value, VolumePerTimeUnit.FromString(src.ScalarVolumePerTime.Unit));
  }
  public static Cdm.ScalarVolumePerTimeData Unload(SEScalarVolumePerTime src)
  {
    Cdm.ScalarVolumePerTimeData dst = new Cdm.ScalarVolumePerTimeData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarVolumePerTime src, Cdm.ScalarVolumePerTimeData dst)
  {
    dst.ScalarVolumePerTime = new Cdm.ScalarData();
    dst.ScalarVolumePerTime.Value = src.GetValue();
    dst.ScalarVolumePerTime.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarVolumePerTimeMass
  public static void Load(Cdm.ScalarVolumePerTimeMassData src, SEScalarVolumePerTimeMass dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarVolumePerTimeMassData src, SEScalarVolumePerTimeMass dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarVolumePerTimeMass.Value, VolumePerTimeMassUnit.FromString(src.ScalarVolumePerTimeMass.Unit));
  }
  public static Cdm.ScalarVolumePerTimeMassData Unload(SEScalarVolumePerTimeMass src)
  {
    Cdm.ScalarVolumePerTimeMassData dst = new Cdm.ScalarVolumePerTimeMassData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarVolumePerTimeMass src, Cdm.ScalarVolumePerTimeMassData dst)
  {
    dst.ScalarVolumePerTimeMass = new Cdm.ScalarData();
    dst.ScalarVolumePerTimeMass.Value = src.GetValue();
    dst.ScalarVolumePerTimeMass.Unit = src.GetUnit().ToString();
  }
  #endregion

  #region SEScalarVolumePerTimePressure
  public static void Load(Cdm.ScalarVolumePerTimePressureData src, SEScalarVolumePerTimePressure dst)
  {
    Serialize(src, dst);
  }
  public static void Serialize(Cdm.ScalarVolumePerTimePressureData src, SEScalarVolumePerTimePressure dst)
  {
    dst.Invalidate();
    dst.SetValue(src.ScalarVolumePerTimePressure.Value, VolumePerTimePressureUnit.FromString(src.ScalarVolumePerTimePressure.Unit));
  }
  public static Cdm.ScalarVolumePerTimePressureData Unload(SEScalarVolumePerTimePressure src)
  {
    Cdm.ScalarVolumePerTimePressureData dst = new Cdm.ScalarVolumePerTimePressureData();
    Serialize(src, dst);
    return dst;
  }

  public static void Serialize(SEScalarVolumePerTimePressure src, Cdm.ScalarVolumePerTimePressureData dst)
  {
    dst.ScalarVolumePerTimePressure = new Cdm.ScalarData();
    dst.ScalarVolumePerTimePressure.Value = src.GetValue();
    dst.ScalarVolumePerTimePressure.Unit = src.GetUnit().ToString();
  }
  #endregion

  //public static void load(FunctionData src, SEFunction dest)
  //{
  //  if (src == null)
  //    return;
  //  dest.setDependent(SEArray.toArray(src.getDependent().getValueList()), src.getDependentUnit());
  //  dest.setIndependent(SEArray.toArray(src.getIndependent().getValueList()), src.getIndependentUnit());
  //  if (!dest.isValid())
  //    Log.error("Invalid function has been loaded");
  //}
  //public static FunctionData unload(SEFunction src)
  //{
  //  if (!src.isValid())
  //    return null;
  //  FunctionData.Builder dst = FunctionData.newBuilder();
  //  unload(src, dst);
  //  return dst.build();
  //}
  //protected static void unload(SEFunction src, FunctionData.Builder dst)
  //{
  //  dst.setDependentUnit(src.dependentUnit);
  //  SEArray.toData(dst.getDependentBuilder(), src.dependent);

  //  dst.setIndependentUnit(src.independentUnit);
  //  SEArray.toData(dst.getIndependentBuilder(), src.independent);
  //}

}