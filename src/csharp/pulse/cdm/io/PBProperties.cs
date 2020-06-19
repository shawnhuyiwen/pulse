/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class PBProperty
  {
    #region SEScalar
    public static void Load(pulse.cdm.bind.ScalarData src, SEScalar dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarData src, SEScalar dst)
    {
      dst.Invalidate();
      dst.SetValue(src.Value);
    }
    public static pulse.cdm.bind.ScalarData Unload(SEScalar src)
    {
      pulse.cdm.bind.ScalarData dst = new pulse.cdm.bind.ScalarData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalar src, pulse.cdm.bind.ScalarData dst)
    {
      dst.Value = src.GetValue();
      dst.Unit = "";
    }
    #endregion

    #region SEScalar0To1
    public static void Load(pulse.cdm.bind.Scalar0To1Data src, SEScalar0To1 dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.Scalar0To1Data src, SEScalar0To1 dst)
    {
      dst.Invalidate();
      dst.SetValue(src.Scalar0To1.Value);
    }
    public static pulse.cdm.bind.Scalar0To1Data Unload(SEScalar0To1 src)
    {
      pulse.cdm.bind.Scalar0To1Data dst = new pulse.cdm.bind.Scalar0To1Data();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalar0To1 src, pulse.cdm.bind.Scalar0To1Data dst)
    {
      dst.Scalar0To1 = new pulse.cdm.bind.ScalarData();
      dst.Scalar0To1.Value = src.GetValue();
      dst.Scalar0To1.Unit = "";
    }
    #endregion

    #region SEScalarArea
    public static void Load(pulse.cdm.bind.ScalarAreaData src, SEScalarArea dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarAreaData src, SEScalarArea dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarArea.Value, AreaUnit.FromString(src.ScalarArea.Unit));
    }
    public static pulse.cdm.bind.ScalarAreaData Unload(SEScalarArea src)
    {
      pulse.cdm.bind.ScalarAreaData dst = new pulse.cdm.bind.ScalarAreaData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarArea src, pulse.cdm.bind.ScalarAreaData dst)
    {
      dst.ScalarArea = new pulse.cdm.bind.ScalarData();
      dst.ScalarArea.Value = src.GetValue();
      dst.ScalarArea.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarFrequency
    public static void Load(pulse.cdm.bind.ScalarFrequencyData src, SEScalarFrequency dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarFrequencyData src, SEScalarFrequency dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarFrequency.Value, FrequencyUnit.FromString(src.ScalarFrequency.Unit));
    }
    public static pulse.cdm.bind.ScalarFrequencyData Unload(SEScalarFrequency src)
    {
      pulse.cdm.bind.ScalarFrequencyData dst = new pulse.cdm.bind.ScalarFrequencyData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarFrequency src, pulse.cdm.bind.ScalarFrequencyData dst)
    {
      dst.ScalarFrequency = new pulse.cdm.bind.ScalarData();
      dst.ScalarFrequency.Value = src.GetValue();
      dst.ScalarFrequency.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarHeatConductancePerArea
    public static void Load(pulse.cdm.bind.ScalarHeatConductancePerAreaData src, SEScalarHeatConductancePerArea dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarHeatConductancePerAreaData src, SEScalarHeatConductancePerArea dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarHeatConductancePerArea.Value, HeatConductancePerAreaUnit.FromString(src.ScalarHeatConductancePerArea.Unit));
    }

    public static pulse.cdm.bind.ScalarHeatConductancePerAreaData Unload(SEScalarHeatConductancePerArea src)
    {
      pulse.cdm.bind.ScalarHeatConductancePerAreaData dst = new pulse.cdm.bind.ScalarHeatConductancePerAreaData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarHeatConductancePerArea src, pulse.cdm.bind.ScalarHeatConductancePerAreaData dst)
    {
      dst.ScalarHeatConductancePerArea = new pulse.cdm.bind.ScalarData();
      dst.ScalarHeatConductancePerArea.Value = src.GetValue();
      dst.ScalarHeatConductancePerArea.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarHeatResistanceArea
    public static void Load(pulse.cdm.bind.ScalarHeatResistanceAreaData src, SEScalarHeatResistanceArea dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarHeatResistanceAreaData src, SEScalarHeatResistanceArea dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarHeatResistanceArea.Value, HeatResistanceAreaUnit.FromString(src.ScalarHeatResistanceArea.Unit));
    }
    public static pulse.cdm.bind.ScalarHeatResistanceAreaData Unload(SEScalarHeatResistanceArea src)
    {
      pulse.cdm.bind.ScalarHeatResistanceAreaData dst = new pulse.cdm.bind.ScalarHeatResistanceAreaData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarHeatResistanceArea src, pulse.cdm.bind.ScalarHeatResistanceAreaData dst)
    {
      dst.ScalarHeatResistanceArea = new pulse.cdm.bind.ScalarData();
      dst.ScalarHeatResistanceArea.Value = src.GetValue();
      dst.ScalarHeatResistanceArea.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarInversePressure
    public static void Load(pulse.cdm.bind.ScalarInversePressureData src, SEScalarInversePressure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarInversePressureData src, SEScalarInversePressure dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarInversePressure.Value, InversePressureUnit.FromString(src.ScalarInversePressure.Unit));
    }
    public static pulse.cdm.bind.ScalarInversePressureData Unload(SEScalarInversePressure src)
    {
      pulse.cdm.bind.ScalarInversePressureData dst = new pulse.cdm.bind.ScalarInversePressureData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarInversePressure src, pulse.cdm.bind.ScalarInversePressureData dst)
    {
      dst.ScalarInversePressure = new pulse.cdm.bind.ScalarData();
      dst.ScalarInversePressure.Value = src.GetValue();
      dst.ScalarInversePressure.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarLength
    public static void Load(pulse.cdm.bind.ScalarLengthData src, SEScalarLength dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarLengthData src, SEScalarLength dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarLength.Value, LengthUnit.FromString(src.ScalarLength.Unit));
    }
    public static pulse.cdm.bind.ScalarLengthData Unload(SEScalarLength src)
    {
      pulse.cdm.bind.ScalarLengthData dst = new pulse.cdm.bind.ScalarLengthData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarLength src, pulse.cdm.bind.ScalarLengthData dst)
    {
      dst.ScalarLength = new pulse.cdm.bind.ScalarData();
      dst.ScalarLength.Value = src.GetValue();
      dst.ScalarLength.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarLengthPerTime
    public static void Load(pulse.cdm.bind.ScalarLengthPerTimeData src, SEScalarLengthPerTime dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarLengthPerTimeData src, SEScalarLengthPerTime dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarLengthPerTime.Value, LengthPerTimeUnit.FromString(src.ScalarLengthPerTime.Unit));
    }
    public static pulse.cdm.bind.ScalarLengthPerTimeData Unload(SEScalarLengthPerTime src)
    {
      pulse.cdm.bind.ScalarLengthPerTimeData dst = new pulse.cdm.bind.ScalarLengthPerTimeData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarLengthPerTime src, pulse.cdm.bind.ScalarLengthPerTimeData dst)
    {
      dst.ScalarLengthPerTime = new pulse.cdm.bind.ScalarData();
      dst.ScalarLengthPerTime.Value = src.GetValue();
      dst.ScalarLengthPerTime.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarMass
    public static void Load(pulse.cdm.bind.ScalarMassData src, SEScalarMass dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarMassData src, SEScalarMass dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarMass.Value, MassUnit.FromString(src.ScalarMass.Unit));
    }
    public static pulse.cdm.bind.ScalarMassData Unload(SEScalarMass src)
    {
      pulse.cdm.bind.ScalarMassData dst = new pulse.cdm.bind.ScalarMassData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarMass src, pulse.cdm.bind.ScalarMassData dst)
    {
      dst.ScalarMass = new pulse.cdm.bind.ScalarData();
      dst.ScalarMass.Value = src.GetValue();
      dst.ScalarMass.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarMassPerAmount
    public static void Load(pulse.cdm.bind.ScalarMassPerAmountData src, SEScalarMassPerAmount dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarMassPerAmountData src, SEScalarMassPerAmount dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarMassPerAmount.Value, MassPerAmountUnit.FromString(src.ScalarMassPerAmount.Unit));
    }
    public static pulse.cdm.bind.ScalarMassPerAmountData Unload(SEScalarMassPerAmount src)
    {
      pulse.cdm.bind.ScalarMassPerAmountData dst = new pulse.cdm.bind.ScalarMassPerAmountData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarMassPerAmount src, pulse.cdm.bind.ScalarMassPerAmountData dst)
    {
      dst.ScalarMassPerAmount = new pulse.cdm.bind.ScalarData();
      dst.ScalarMassPerAmount.Value = src.GetValue();
      dst.ScalarMassPerAmount.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarMassPerAreaTime
    public static void Load(pulse.cdm.bind.ScalarMassPerAreaTimeData src, SEScalarMassPerAreaTime dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarMassPerAreaTimeData src, SEScalarMassPerAreaTime dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarMassPerAreaTime.Value, MassPerAreaTimeUnit.FromString(src.ScalarMassPerAreaTime.Unit));
    }
    public static pulse.cdm.bind.ScalarMassPerAreaTimeData Unload(SEScalarMassPerAreaTime src)
    {
      pulse.cdm.bind.ScalarMassPerAreaTimeData dst = new pulse.cdm.bind.ScalarMassPerAreaTimeData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarMassPerAreaTime src, pulse.cdm.bind.ScalarMassPerAreaTimeData dst)
    {
      dst.ScalarMassPerAreaTime = new pulse.cdm.bind.ScalarData();
      dst.ScalarMassPerAreaTime.Value = src.GetValue();
      dst.ScalarMassPerAreaTime.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarMassPerTime
    public static void Load(pulse.cdm.bind.ScalarMassPerTimeData src, SEScalarMassPerTime dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarMassPerTimeData src, SEScalarMassPerTime dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarMassPerTime.Value, MassPerTimeUnit.FromString(src.ScalarMassPerTime.Unit));
    }
    public static pulse.cdm.bind.ScalarMassPerTimeData Unload(SEScalarMassPerTime src)
    {
      pulse.cdm.bind.ScalarMassPerTimeData dst = new pulse.cdm.bind.ScalarMassPerTimeData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarMassPerTime src, pulse.cdm.bind.ScalarMassPerTimeData dst)
    {
      dst.ScalarMassPerTime = new pulse.cdm.bind.ScalarData();
      dst.ScalarMassPerTime.Value = src.GetValue();
      dst.ScalarMassPerTime.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarMassPerVolume
    public static void Load(pulse.cdm.bind.ScalarMassPerVolumeData src, SEScalarMassPerVolume dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarMassPerVolumeData src, SEScalarMassPerVolume dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarMassPerVolume.Value, MassPerVolumeUnit.FromString(src.ScalarMassPerVolume.Unit));
    }
    public static pulse.cdm.bind.ScalarMassPerVolumeData Unload(SEScalarMassPerVolume src)
    {
      pulse.cdm.bind.ScalarMassPerVolumeData dst = new pulse.cdm.bind.ScalarMassPerVolumeData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarMassPerVolume src, pulse.cdm.bind.ScalarMassPerVolumeData dst)
    {
      dst.ScalarMassPerVolume = new pulse.cdm.bind.ScalarData();
      dst.ScalarMassPerVolume.Value = src.GetValue();
      dst.ScalarMassPerVolume.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarNegative1To1
    public static void Load(pulse.cdm.bind.ScalarNegative1To1Data src, SEScalarNegative1To1 dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarNegative1To1Data src, SEScalarNegative1To1 dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarNegative1To1.Value);
    }
    public static pulse.cdm.bind.ScalarNegative1To1Data Unload(SEScalarNegative1To1 src)
    {
      pulse.cdm.bind.ScalarNegative1To1Data dst = new pulse.cdm.bind.ScalarNegative1To1Data();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarNegative1To1 src, pulse.cdm.bind.ScalarNegative1To1Data dst)
    {
      dst.ScalarNegative1To1 = new pulse.cdm.bind.ScalarData();
      dst.ScalarNegative1To1.Value = src.GetValue();
      dst.ScalarNegative1To1.Unit = "";
    }
    #endregion

    #region SEScalarPressure
    public static void Load(pulse.cdm.bind.ScalarPressureData src, SEScalarPressure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarPressureData src, SEScalarPressure dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarPressure.Value, PressureUnit.FromString(src.ScalarPressure.Unit));
    }
    public static pulse.cdm.bind.ScalarPressureData Unload(SEScalarPressure src)
    {
      pulse.cdm.bind.ScalarPressureData dst = new pulse.cdm.bind.ScalarPressureData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarPressure src, pulse.cdm.bind.ScalarPressureData dst)
    {
      dst.ScalarPressure = new pulse.cdm.bind.ScalarData();
      dst.ScalarPressure.Value = src.GetValue();
      dst.ScalarPressure.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarPressureTimePerVolume
    public static void Load(pulse.cdm.bind.ScalarPressureTimePerVolumeData src, SEScalarPressureTimePerVolume dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarPressureTimePerVolumeData src, SEScalarPressureTimePerVolume dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarPressureTimePerVolume.Value, PressureTimePerVolumeUnit.FromString(src.ScalarPressureTimePerVolume.Unit));
    }
    public static pulse.cdm.bind.ScalarPressureTimePerVolumeData Unload(SEScalarPressureTimePerVolume src)
    {
      pulse.cdm.bind.ScalarPressureTimePerVolumeData dst = new pulse.cdm.bind.ScalarPressureTimePerVolumeData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarPressureTimePerVolume src, pulse.cdm.bind.ScalarPressureTimePerVolumeData dst)
    {
      dst.ScalarPressureTimePerVolume = new pulse.cdm.bind.ScalarData();
      dst.ScalarPressureTimePerVolume.Value = src.GetValue();
      dst.ScalarPressureTimePerVolume.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarTemperature
    public static void Load(pulse.cdm.bind.ScalarTemperatureData src, SEScalarTemperature dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarTemperatureData src, SEScalarTemperature dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarTemperature.Value, TemperatureUnit.FromString(src.ScalarTemperature.Unit));
    }
    public static pulse.cdm.bind.ScalarTemperatureData Unload(SEScalarTemperature src)
    {
      pulse.cdm.bind.ScalarTemperatureData dst = new pulse.cdm.bind.ScalarTemperatureData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarTemperature src, pulse.cdm.bind.ScalarTemperatureData dst)
    {
      dst.ScalarTemperature = new pulse.cdm.bind.ScalarData();
      dst.ScalarTemperature.Value = src.GetValue();
      dst.ScalarTemperature.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarPower
    public static void Load(pulse.cdm.bind.ScalarPowerData src, SEScalarPower dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarPowerData src, SEScalarPower dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarPower.Value, PowerUnit.FromString(src.ScalarPower.Unit));
    }
    public static pulse.cdm.bind.ScalarPowerData Unload(SEScalarPower src)
    {
      pulse.cdm.bind.ScalarPowerData dst = new pulse.cdm.bind.ScalarPowerData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarPower src, pulse.cdm.bind.ScalarPowerData dst)
    {
      dst.ScalarPower = new pulse.cdm.bind.ScalarData();
      dst.ScalarPower.Value = src.GetValue();
      dst.ScalarPower.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarTime
    public static void Load(pulse.cdm.bind.ScalarTimeData src, SEScalarTime dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarTimeData src, SEScalarTime dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarTime.Value, TimeUnit.FromString(src.ScalarTime.Unit));
    }
    public static pulse.cdm.bind.ScalarTimeData Unload(SEScalarTime src)
    {
      pulse.cdm.bind.ScalarTimeData dst = new pulse.cdm.bind.ScalarTimeData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarTime src, pulse.cdm.bind.ScalarTimeData dst)
    {
      dst.ScalarTime = new pulse.cdm.bind.ScalarData();
      dst.ScalarTime.Value = src.GetValue();
      dst.ScalarTime.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarVolume
    public static void Load(pulse.cdm.bind.ScalarVolumeData src, SEScalarVolume dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarVolumeData src, SEScalarVolume dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarVolume.Value, VolumeUnit.FromString(src.ScalarVolume.Unit));
    }
    public static pulse.cdm.bind.ScalarVolumeData Unload(SEScalarVolume src)
    {
      pulse.cdm.bind.ScalarVolumeData dst = new pulse.cdm.bind.ScalarVolumeData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEScalarVolume src, pulse.cdm.bind.ScalarVolumeData dst)
    {
      dst.ScalarVolume = new pulse.cdm.bind.ScalarData();
      dst.ScalarVolume.Value = src.GetValue();
      dst.ScalarVolume.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarVolumePerTime
    public static void Load(pulse.cdm.bind.ScalarVolumePerTimeData src, SEScalarVolumePerTime dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarVolumePerTimeData src, SEScalarVolumePerTime dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarVolumePerTime.Value, VolumePerTimeUnit.FromString(src.ScalarVolumePerTime.Unit));
    }
    public static pulse.cdm.bind.ScalarVolumePerTimeData Unload(SEScalarVolumePerTime src)
    {
      pulse.cdm.bind.ScalarVolumePerTimeData dst = new pulse.cdm.bind.ScalarVolumePerTimeData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarVolumePerTime src, pulse.cdm.bind.ScalarVolumePerTimeData dst)
    {
      dst.ScalarVolumePerTime = new pulse.cdm.bind.ScalarData();
      dst.ScalarVolumePerTime.Value = src.GetValue();
      dst.ScalarVolumePerTime.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarVolumePerTimeMass
    public static void Load(pulse.cdm.bind.ScalarVolumePerTimeMassData src, SEScalarVolumePerTimeMass dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarVolumePerTimeMassData src, SEScalarVolumePerTimeMass dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarVolumePerTimeMass.Value, VolumePerTimeMassUnit.FromString(src.ScalarVolumePerTimeMass.Unit));
    }
    public static pulse.cdm.bind.ScalarVolumePerTimeMassData Unload(SEScalarVolumePerTimeMass src)
    {
      pulse.cdm.bind.ScalarVolumePerTimeMassData dst = new pulse.cdm.bind.ScalarVolumePerTimeMassData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarVolumePerTimeMass src, pulse.cdm.bind.ScalarVolumePerTimeMassData dst)
    {
      dst.ScalarVolumePerTimeMass = new pulse.cdm.bind.ScalarData();
      dst.ScalarVolumePerTimeMass.Value = src.GetValue();
      dst.ScalarVolumePerTimeMass.Unit = src.GetUnit().ToString();
    }
    #endregion

    #region SEScalarVolumePerTimePressure
    public static void Load(pulse.cdm.bind.ScalarVolumePerTimePressureData src, SEScalarVolumePerTimePressure dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ScalarVolumePerTimePressureData src, SEScalarVolumePerTimePressure dst)
    {
      dst.Invalidate();
      dst.SetValue(src.ScalarVolumePerTimePressure.Value, VolumePerTimePressureUnit.FromString(src.ScalarVolumePerTimePressure.Unit));
    }
    public static pulse.cdm.bind.ScalarVolumePerTimePressureData Unload(SEScalarVolumePerTimePressure src)
    {
      pulse.cdm.bind.ScalarVolumePerTimePressureData dst = new pulse.cdm.bind.ScalarVolumePerTimePressureData();
      Serialize(src, dst);
      return dst;
    }

    public static void Serialize(SEScalarVolumePerTimePressure src, pulse.cdm.bind.ScalarVolumePerTimePressureData dst)
    {
      dst.ScalarVolumePerTimePressure = new pulse.cdm.bind.ScalarData();
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
}
