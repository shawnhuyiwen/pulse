/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using Google.Protobuf;

namespace Pulse.CDM
{
  class PBPatientAssessmets
  {
    #region SEPatientAssessment
    public static void Load(pulse.cdm.bind.PatientAssessmentData src, SEPatientAssessment dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PatientAssessmentData src, SEPatientAssessment dst)
    {
      dst.Clear();
    }
    public static bool SerializeFromString(string src, SEPatientAssessment dst)
    {
      if (dst is SEArterialBloodGasTest)
      {
        SEArterialBloodGasTest abg = (SEArterialBloodGasTest)dst;
        try
        {
          pulse.cdm.bind.ArterialBloodGasTestData data = JsonParser.Default.Parse<pulse.cdm.bind.ArterialBloodGasTestData>(src);
          PBPatientAssessmets.Load(data, abg);
        }
        catch (Google.Protobuf.InvalidJsonException)
        {
          return false;
        }
        return true;
      }

      if (dst is SECompleteBloodCount)
      {
        SECompleteBloodCount cbc = (SECompleteBloodCount)dst;
        try
        {
          pulse.cdm.bind.CompleteBloodCountData data = JsonParser.Default.Parse<pulse.cdm.bind.CompleteBloodCountData>(src);
          PBPatientAssessmets.Load(data, cbc);
        }
        catch (Google.Protobuf.InvalidJsonException)
        {
          return false;
        }
        return true;
      }

      if (dst is SEComprehensiveMetabolicPanel)
      {
        SEComprehensiveMetabolicPanel cmp = (SEComprehensiveMetabolicPanel)dst;
        try
        {
          pulse.cdm.bind.ComprehensiveMetabolicPanelData data = JsonParser.Default.Parse<pulse.cdm.bind.ComprehensiveMetabolicPanelData>(src);
          PBPatientAssessmets.Load(data, cmp);
        }
        catch (Google.Protobuf.InvalidJsonException)
        {
          return false;
        }
        return true;
      }

      if (dst is SEUrinalysis)
      {
        SEUrinalysis u = (SEUrinalysis)dst;
        try
        {
          pulse.cdm.bind.UrinalysisData data = JsonParser.Default.Parse<pulse.cdm.bind.UrinalysisData>(src);
          PBPatientAssessmets.Load(data, u);
        }
        catch (Google.Protobuf.InvalidJsonException)
        {
          return false;
        }
        return true;
      }

      return false;
    }

    public static pulse.cdm.bind.PatientAssessmentData Unload(SEPatientAssessment src)
    {
      pulse.cdm.bind.PatientAssessmentData dst = new pulse.cdm.bind.PatientAssessmentData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPatientAssessment src, pulse.cdm.bind.PatientAssessmentData dst)
    {

    }
    #endregion


    #region SEArterialBloodGasTest
    public static void Load(pulse.cdm.bind.ArterialBloodGasTestData src, SEArterialBloodGasTest dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ArterialBloodGasTestData src, SEArterialBloodGasTest dst)
    {
      Serialize(src.PatientAssessment, dst);
      if (src.BloodPH != null)
        PBProperty.Load(src.BloodPH, dst.GetBloodPH());
      if (src.Bicarbonate != null)
        PBProperty.Load(src.Bicarbonate, dst.GetBicarbonate());
      if (src.PartialPressureOfOxygen != null)
        PBProperty.Load(src.PartialPressureOfOxygen, dst.GetPartialPressureOfOxygen());
      if (src.PartialPressureOfCarbonDioxide != null)
        PBProperty.Load(src.PartialPressureOfCarbonDioxide, dst.GetPartialPressureOfCarbonDioxide());
      if (src.OxygenSaturation != null)
        PBProperty.Load(src.OxygenSaturation, dst.GetOxygenSaturation());
    }
    public static bool SerializeFromString(string src, SEArterialBloodGasTest dst)
    {
      try
      {
        pulse.cdm.bind.ArterialBloodGasTestData data = JsonParser.Default.Parse<pulse.cdm.bind.ArterialBloodGasTestData>(src);
        PBPatientAssessmets.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ArterialBloodGasTestData Unload(SEArterialBloodGasTest src)
    {
      pulse.cdm.bind.ArterialBloodGasTestData dst = new pulse.cdm.bind.ArterialBloodGasTestData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEArterialBloodGasTest src, pulse.cdm.bind.ArterialBloodGasTestData dst)
    {
      dst.PatientAssessment = new pulse.cdm.bind.PatientAssessmentData();
      Serialize(src, dst.PatientAssessment);
      if (src.HasBloodPH())
        dst.BloodPH = (PBProperty.Unload(src.GetBloodPH()));
      if (src.HasBicarbonate())
        dst.Bicarbonate = (PBProperty.Unload(src.GetBicarbonate()));
      if (src.HasPartialPressureOfOxygen())
        dst.PartialPressureOfOxygen = (PBProperty.Unload(src.GetPartialPressureOfOxygen()));
      if (src.HasPartialPressureOfCarbonDioxide())
        dst.PartialPressureOfCarbonDioxide = (PBProperty.Unload(src.GetPartialPressureOfCarbonDioxide()));
      if (src.HasOxygenSaturation())
        dst.OxygenSaturation = (PBProperty.Unload(src.GetOxygenSaturation()));
    }
    public static string SerializeToString(SEArterialBloodGasTest src)
    {
      var pb = PBPatientAssessmets.Unload(src);
      return pb.ToString();
    }
    #endregion

    #region SECompleteBloodCount
    public static void Load(pulse.cdm.bind.CompleteBloodCountData src, SECompleteBloodCount dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.CompleteBloodCountData src, SECompleteBloodCount dst)
    {
      Serialize(src.PatientAssessment, dst);
      if (src.Hematocrit != null)
        PBProperty.Load(src.Hematocrit, dst.GetHematocrit());
      if (src.Hemoglobin != null)
        PBProperty.Load(src.Hemoglobin, dst.GetHemoglobin());
      if (src.PlateletCount != null)
        PBProperty.Load(src.PlateletCount, dst.GetPlateletCount());
      if (src.MeanCorpuscularHemoglobin != null)
        PBProperty.Load(src.MeanCorpuscularHemoglobin, dst.GetMeanCorpuscularHemoglobin());
      if (src.MeanCorpuscularHemoglobinConcentration != null)
        PBProperty.Load(src.MeanCorpuscularHemoglobinConcentration, dst.GetMeanCorpuscularHemoglobinConcentration());
      if (src.MeanCorpuscularVolume != null)
        PBProperty.Load(src.MeanCorpuscularVolume, dst.GetMeanCorpuscularVolume());
      if (src.RedBloodCellCount != null)
        PBProperty.Load(src.RedBloodCellCount, dst.GetRedBloodCellCount());
      if (src.WhiteBloodCellCount != null)
        PBProperty.Load(src.WhiteBloodCellCount, dst.GetWhiteBloodCellCount());
    }
    public static bool SerializeFromString(string src, SECompleteBloodCount dst)
    {
      try
      {
        pulse.cdm.bind.CompleteBloodCountData data = JsonParser.Default.Parse<pulse.cdm.bind.CompleteBloodCountData>(src);
        PBPatientAssessmets.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.CompleteBloodCountData Unload(SECompleteBloodCount src)
    {
      pulse.cdm.bind.CompleteBloodCountData dst = new pulse.cdm.bind.CompleteBloodCountData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SECompleteBloodCount src, pulse.cdm.bind.CompleteBloodCountData dst)
    {
      dst.PatientAssessment = new pulse.cdm.bind.PatientAssessmentData();
      Serialize(src, dst.PatientAssessment);
      if (src.HasHematocrit())
        dst.Hematocrit = (PBProperty.Unload(src.GetHematocrit()));
      if (src.HasHemoglobin())
        dst.Hemoglobin = (PBProperty.Unload(src.GetHemoglobin()));
      if (src.HasPlateletCount())
        dst.PlateletCount = (PBProperty.Unload(src.GetPlateletCount()));
      if (src.HasMeanCorpuscularHemoglobin())
        dst.MeanCorpuscularHemoglobin = (PBProperty.Unload(src.GetMeanCorpuscularHemoglobin()));
      if (src.HasMeanCorpuscularHemoglobinConcentration())
        dst.MeanCorpuscularHemoglobinConcentration = (PBProperty.Unload(src.GetMeanCorpuscularHemoglobinConcentration()));
      if (src.HasMeanCorpuscularVolume())
        dst.MeanCorpuscularVolume = (PBProperty.Unload(src.GetMeanCorpuscularVolume()));
      if (src.HasRedBloodCellCount())
        dst.RedBloodCellCount = (PBProperty.Unload(src.GetRedBloodCellCount()));
      if (src.HasWhiteBloodCellCount())
        dst.WhiteBloodCellCount = (PBProperty.Unload(src.GetWhiteBloodCellCount()));
    }
    public static string SerializeToString(SECompleteBloodCount src)
    {
      var pb = PBPatientAssessmets.Unload(src);
      return pb.ToString();
    }
    #endregion

    #region SEComprehensiveMetabolicPanel
    public static void Load(pulse.cdm.bind.ComprehensiveMetabolicPanelData src, SEComprehensiveMetabolicPanel dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ComprehensiveMetabolicPanelData src, SEComprehensiveMetabolicPanel dst)
    {
      Serialize(src.PatientAssessment, dst);
      if (src.Albumin != null)
        PBProperty.Load(src.Albumin, dst.GetAlbumin());
      if (src.ALP != null)
        PBProperty.Load(src.ALP, dst.GetALP());
      if (src.ALT != null)
        PBProperty.Load(src.ALT, dst.GetALT());
      if (src.AST != null)
        PBProperty.Load(src.AST, dst.GetAST());
      if (src.BUN != null)
        PBProperty.Load(src.BUN, dst.GetBUN());
      if (src.Calcium != null)
        PBProperty.Load(src.Calcium, dst.GetCalcium());
      if (src.Chloride != null)
        PBProperty.Load(src.Chloride, dst.GetChloride());
      if (src.CO2 != null)
        PBProperty.Load(src.CO2, dst.GetCO2());
      if (src.Creatinine != null)
        PBProperty.Load(src.Creatinine, dst.GetCreatinine());
      if (src.Glucose != null)
        PBProperty.Load(src.Glucose, dst.GetGlucose());
      if (src.Potassium != null)
        PBProperty.Load(src.Potassium, dst.GetPotassium());
      if (src.Sodium != null)
        PBProperty.Load(src.Sodium, dst.GetSodium());
      if (src.TotalBilirubin != null)
        PBProperty.Load(src.TotalBilirubin, dst.GetTotalBilirubin());
      if (src.TotalProtein != null)
        PBProperty.Load(src.TotalProtein, dst.GetTotalProtein());
    }
    public static bool SerializeFromString(string src, SEComprehensiveMetabolicPanel dst)
    {
      try
      {
        pulse.cdm.bind.ComprehensiveMetabolicPanelData data = JsonParser.Default.Parse<pulse.cdm.bind.ComprehensiveMetabolicPanelData>(src);
        PBPatientAssessmets.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.ComprehensiveMetabolicPanelData Unload(SEComprehensiveMetabolicPanel src)
    {
      pulse.cdm.bind.ComprehensiveMetabolicPanelData dst = new pulse.cdm.bind.ComprehensiveMetabolicPanelData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEComprehensiveMetabolicPanel src, pulse.cdm.bind.ComprehensiveMetabolicPanelData dst)
    {
      dst.PatientAssessment = new pulse.cdm.bind.PatientAssessmentData();
      Serialize(src, dst.PatientAssessment);
      if (src.HasAlbumin())
        dst.Albumin = (PBProperty.Unload(src.GetAlbumin()));
      if (src.HasALP())
        dst.ALP = (PBProperty.Unload(src.GetALP()));
      if (src.HasALT())
        dst.ALT = (PBProperty.Unload(src.GetALT()));
      if (src.HasAST())
        dst.AST = (PBProperty.Unload(src.GetAST()));
      if (src.HasBUN())
        dst.BUN = (PBProperty.Unload(src.GetBUN()));
      if (src.HasCalcium())
        dst.Calcium = (PBProperty.Unload(src.GetCalcium()));
      if (src.HasChloride())
        dst.Chloride = (PBProperty.Unload(src.GetChloride()));
      if (src.HasCO2())
        dst.CO2 = (PBProperty.Unload(src.GetCO2()));
      if (src.HasCreatinine())
        dst.Creatinine = (PBProperty.Unload(src.GetCreatinine()));
      if (src.HasGlucose())
        dst.Glucose = (PBProperty.Unload(src.GetGlucose()));
      if (src.HasPotassium())
        dst.Potassium = (PBProperty.Unload(src.GetPotassium()));
      if (src.HasSodium())
        dst.Sodium = (PBProperty.Unload(src.GetSodium()));
      if (src.HasTotalBilirubin())
        dst.TotalBilirubin = (PBProperty.Unload(src.GetTotalBilirubin()));
      if (src.HasTotalProtein())
        dst.TotalProtein = (PBProperty.Unload(src.GetTotalProtein()));
    }
    public static string SerializeToString(SEComprehensiveMetabolicPanel src)
    {
      var pb = PBPatientAssessmets.Unload(src);
      return pb.ToString();
    }
    #endregion

    #region SEUrinalysis
    public static void Load(pulse.cdm.bind.UrinalysisData src, SEUrinalysis dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.UrinalysisData src, SEUrinalysis dst)
    {
      Serialize(src.PatientAssessment, dst);
      dst.SetColor((eUrinalysis_UrineColor)(int)src.Color);
      dst.SetAppearance((eUrinalysis_ClarityIndicator)(int)src.Appearance);
      dst.SetGlucose((eUrinalysis_PresenceIndicator)(int)src.Glucose);
      dst.SetKetone((eUrinalysis_PresenceIndicator)(int)src.Ketone);
      if (src.Bilirubin != null)
        PBProperty.Load(src.Bilirubin, dst.GetBilirubin());
      if (src.SpecificGravity != null)
        PBProperty.Load(src.SpecificGravity, dst.GetSpecificGravity());
      dst.SetBlood((eUrinalysis_PresenceIndicator)(int)src.Blood);
      if (src.PH != null)
        PBProperty.Load(src.PH, dst.GetPH());
      dst.SetProtein((eUrinalysis_PresenceIndicator)(int)src.Protein);
      if (src.Urobilinogen != null)
        PBProperty.Load(src.Urobilinogen, dst.GetUrobilinogen());
      dst.SetNitrite((eUrinalysis_PresenceIndicator)(int)src.Nitrite);
      dst.SetLeukocyteEsterase((eUrinalysis_PresenceIndicator)(int)src.LeukocyteEsterase);

      if (src.Microscopic != null)
        PBPatientAssessmets.Load(src.Microscopic, dst.GetMicroscopic());
    }
    public static bool SerializeFromString(string src, SEUrinalysis dst)
    {
      try
      {
        pulse.cdm.bind.UrinalysisData data = JsonParser.Default.Parse<pulse.cdm.bind.UrinalysisData>(src);
        PBPatientAssessmets.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.UrinalysisData Unload(SEUrinalysis src)
    {
      pulse.cdm.bind.UrinalysisData dst = new pulse.cdm.bind.UrinalysisData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEUrinalysis src, pulse.cdm.bind.UrinalysisData dst)
    {
      dst.PatientAssessment = new pulse.cdm.bind.PatientAssessmentData();
      Serialize(src, dst.PatientAssessment);
      if (src.HasColor())
        dst.Color = (pulse.cdm.bind.UrinalysisData.Types.eUrineColor)(int)src.GetColor();
      if (src.HasAppearance())
        dst.Appearance = (pulse.cdm.bind.UrinalysisData.Types.eClarityIndicator)(int)src.GetAppearance();
      if (src.HasGlucose())
        dst.Glucose = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetGlucose();
      if (src.HasKetone())
        dst.Ketone = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetKetone();
      if (src.HasBilirubin())
        dst.Bilirubin = PBProperty.Unload(src.GetBilirubin());
      if (src.HasSpecificGravity())
        dst.SpecificGravity = PBProperty.Unload(src.GetSpecificGravity());
      if (src.HasBlood())
        dst.Blood = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetBlood();
      if (src.HasPH())
        dst.PH = PBProperty.Unload(src.GetPH());
      if (src.HasProtein())
        dst.Protein = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetProtein();
      if (src.HasUrobilinogen())
        dst.Urobilinogen = PBProperty.Unload(src.GetUrobilinogen());
      if (src.HasNitrite())
        dst.Nitrite = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetNitrite();
      if (src.HasLeukocyteEsterase())
        dst.LeukocyteEsterase = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetLeukocyteEsterase();

      if (src.HasMicroscopic())
        dst.Microscopic = PBPatientAssessmets.Unload(src.GetMicroscopic());
    }
    public static string SerializeToString(SEUrinalysis src)
    {
      var pb = PBPatientAssessmets.Unload(src);
      return pb.ToString();
    }
    #endregion

    #region SEUrinalysisMicroscopic
    public static void Load(pulse.cdm.bind.UrinalysisMicroscopicData src, SEUrinalysisMicroscopic dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.UrinalysisMicroscopicData src, SEUrinalysisMicroscopic dst)
    {
      dst.SetObservationType((eUrinalysis_MicroscopicObservationType)(int)src.ObservationType);
      if (src.RedBloodCells != null)
        PBProperty.Load(src.RedBloodCells, dst.GetRedBloodCells());
      if (src.WhiteBloodCells != null)
        PBProperty.Load(src.WhiteBloodCells, dst.GetWhiteBloodCells());
      dst.SetEpithelialCells((eUrinalysis_MicroscopicObservationAmount)(int)src.EpithelialCells);
      if (src.Casts != null)
        PBProperty.Load(src.Casts, dst.GetCasts());
      dst.SetCrystals((eUrinalysis_MicroscopicObservationAmount)(int)src.Crystals);
      dst.SetBacteria((eUrinalysis_MicroscopicObservationAmount)(int)src.Bacteria);
      dst.SetTrichomonads((eUrinalysis_MicroscopicObservationAmount)(int)src.Trichomonads);
      dst.SetYeast((eUrinalysis_MicroscopicObservationAmount)(int)src.Yeast);
    }

    public static pulse.cdm.bind.UrinalysisMicroscopicData Unload(SEUrinalysisMicroscopic src)
    {
      pulse.cdm.bind.UrinalysisMicroscopicData dst = new pulse.cdm.bind.UrinalysisMicroscopicData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEUrinalysisMicroscopic src, pulse.cdm.bind.UrinalysisMicroscopicData dst)
    {
      if (src.HasObservationType())
        dst.ObservationType = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationType)(int)src.GetObservationType();
      if (src.HasRedBloodCells())
        dst.RedBloodCells = PBProperty.Unload(src.GetRedBloodCells());
      if (src.HasWhiteBloodCells())
        dst.WhiteBloodCells = PBProperty.Unload(src.GetWhiteBloodCells());
      if (src.HasEpithelialCells())
        dst.EpithelialCells = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetEpithelialCells();
      if (src.HasCasts())
        dst.Casts = PBProperty.Unload(src.GetCasts());
      if (src.HasCrystals())
        dst.Crystals = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetCrystals();
      if (src.HasBacteria())
        dst.Bacteria = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetBacteria();
      if (src.HasTrichomonads())
        dst.Trichomonads = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetTrichomonads();
      if (src.HasYeast())
        dst.Yeast = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetYeast();
    }
    #endregion
  }
}
