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

    #region SEScalarMassPerVolume
    public static void Load(Cdm.ScalarMassPerVolumeData src, SEScalarMassPerVolume dst)
    {
        Serialize(src, dst);
    }
    public static void Serialize(Cdm.ScalarMassPerVolumeData src, SEScalarMassPerVolume dst)
    {
        dst.Invalidate();
        dst.SetValue(src.ScalarMassPerVolume.Value);
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

    #region SEScalarTime
    public static void Load(Cdm.ScalarTimeData src, SEScalarTime dst)
    {
        Serialize(src, dst);
    }
    public static void Serialize(Cdm.ScalarTimeData src, SEScalarTime dst)
    {
        dst.Invalidate();
        dst.SetValue(src.ScalarTime.Value);
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
        dst.SetValue(src.ScalarVolume.Value);
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
        dst.SetValue(src.ScalarVolumePerTime.Value);
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
}