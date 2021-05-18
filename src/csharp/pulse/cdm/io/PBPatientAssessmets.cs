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

      if (dst is SEPulmonaryFunctionTest)
      {
        SEPulmonaryFunctionTest pft = (SEPulmonaryFunctionTest)dst;
        try
        {
          pulse.cdm.bind.PulmonaryFunctionTestData data = JsonParser.Default.Parse<pulse.cdm.bind.PulmonaryFunctionTestData>(src);
          PBPatientAssessmets.Load(data, pft);
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

    #region SEPulmonaryFunctionTest
    public static void Load(pulse.cdm.bind.PulmonaryFunctionTestData src, SEPulmonaryFunctionTest dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.PulmonaryFunctionTestData src, SEPulmonaryFunctionTest dst)
    {
      Serialize(src.PatientAssessment, dst);
      if (src.ExpiratoryReserveVolume != null)
        PBProperty.Load(src.ExpiratoryReserveVolume, dst.GetExpiratoryReserveVolume());
      if (src.ForcedVitalCapacity != null)
        PBProperty.Load(src.ForcedVitalCapacity, dst.GetForcedVitalCapacity());
      if (src.ForcedExpiratoryVolume != null)
        PBProperty.Load(src.ForcedExpiratoryVolume, dst.GetForcedExpiratoryVolume());
      if (src.ForcedExpiratoryFlow != null)
        PBProperty.Load(src.ForcedExpiratoryFlow, dst.GetForcedExpiratoryFlow());
      if (src.FunctionalResidualCapacity != null)
        PBProperty.Load(src.FunctionalResidualCapacity, dst.GetFunctionalResidualCapacity());
      if (src.InspiratoryCapacity != null)
        PBProperty.Load(src.InspiratoryCapacity, dst.GetInspiratoryCapacity());
      if (src.InspiratoryReserveVolume != null)
        PBProperty.Load(src.InspiratoryReserveVolume, dst.GetInspiratoryReserveVolume());
      if (src.MaximumVoluntaryVentilation != null)
        PBProperty.Load(src.MaximumVoluntaryVentilation, dst.GetMaximumVoluntaryVentilation());
      if (src.PeakExpiratoryFlow != null)
        PBProperty.Load(src.PeakExpiratoryFlow, dst.GetPeakExpiratoryFlow());
      if (src.ResidualVolume != null)
        PBProperty.Load(src.ResidualVolume, dst.GetResidualVolume());
      if (src.SlowVitalCapacity != null)
        PBProperty.Load(src.SlowVitalCapacity, dst.GetSlowVitalCapacity());
      if (src.TotalLungCapacity != null)
        PBProperty.Load(src.TotalLungCapacity, dst.GetTotalLungCapacity());
      if (src.VitalCapacity != null)
        PBProperty.Load(src.VitalCapacity, dst.GetVitalCapacity());
      //if (src.LungVolumePlot != null)
      //  PBProperty.Load(src.LungVolumePlot, dst.GetLungVolumePlot());
    }
    public static bool SerializeFromString(string src, SEPulmonaryFunctionTest dst)
    {
      try
      {
        pulse.cdm.bind.PulmonaryFunctionTestData data = JsonParser.Default.Parse<pulse.cdm.bind.PulmonaryFunctionTestData>(src);
        PBPatientAssessmets.Load(data, dst);
      }
      catch (Google.Protobuf.InvalidJsonException)
      {
        return false;
      }
      return true;
    }

    public static pulse.cdm.bind.PulmonaryFunctionTestData Unload(SEPulmonaryFunctionTest src)
    {
      pulse.cdm.bind.PulmonaryFunctionTestData dst = new pulse.cdm.bind.PulmonaryFunctionTestData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEPulmonaryFunctionTest src, pulse.cdm.bind.PulmonaryFunctionTestData dst)
    {
      dst.PatientAssessment = new pulse.cdm.bind.PatientAssessmentData();
      Serialize(src, dst.PatientAssessment);
      if (src.HasExpiratoryReserveVolume())
        dst.ExpiratoryReserveVolume = (PBProperty.Unload(src.GetExpiratoryReserveVolume()));
      if (src.HasForcedVitalCapacity())
        dst.ForcedVitalCapacity = (PBProperty.Unload(src.GetForcedVitalCapacity()));
      if (src.HasForcedExpiratoryVolume())
        dst.ForcedExpiratoryVolume = (PBProperty.Unload(src.GetForcedExpiratoryVolume()));
      if (src.HasForcedExpiratoryFlow())
        dst.ForcedExpiratoryFlow = (PBProperty.Unload(src.GetForcedExpiratoryFlow()));
      if (src.HasFunctionalResidualCapacity())
        dst.FunctionalResidualCapacity = (PBProperty.Unload(src.GetFunctionalResidualCapacity()));
      if (src.HasInspiratoryCapacity())
        dst.InspiratoryCapacity = (PBProperty.Unload(src.GetInspiratoryCapacity()));
      if (src.HasInspiratoryReserveVolume())
        dst.InspiratoryReserveVolume = (PBProperty.Unload(src.GetInspiratoryReserveVolume()));
      if (src.HasMaximumVoluntaryVentilation())
        dst.MaximumVoluntaryVentilation = (PBProperty.Unload(src.GetMaximumVoluntaryVentilation()));
      if (src.HasPeakExpiratoryFlow())
        dst.PeakExpiratoryFlow = (PBProperty.Unload(src.GetPeakExpiratoryFlow()));
      if (src.HasResidualVolume())
        dst.ResidualVolume = (PBProperty.Unload(src.GetResidualVolume()));
      if (src.HasSlowVitalCapacity())
        dst.SlowVitalCapacity = (PBProperty.Unload(src.GetSlowVitalCapacity()));
      if (src.HasTotalLungCapacity())
        dst.TotalLungCapacity = (PBProperty.Unload(src.GetTotalLungCapacity()));
      if (src.HasVitalCapacity())
        dst.VitalCapacity = (PBProperty.Unload(src.GetVitalCapacity()));
      //if (src.HasLungVolumePlot())
      //  dst.LungVolumePlot = (PBProperty.Unload(src.GetLungVolumePlot()));
    }
    public static string SerializeToString(SEPulmonaryFunctionTest src)
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
      dst.SetColorResult((SEUrinalysis.eUrineColor)(int)src.Color);
      dst.SetAppearanceResult((SEUrinalysis.eClarityIndicator)(int)src.Appearance);
      dst.SetGlucoseResult((SEUrinalysis.ePresenceIndicator)(int)src.Glucose);
      dst.SetKetoneResult((SEUrinalysis.ePresenceIndicator)(int)src.Ketone);
      if (src.Bilirubin != null)
        PBProperty.Load(src.Bilirubin, dst.GetBilirubinResult());
      if (src.SpecificGravity != null)
        PBProperty.Load(src.SpecificGravity, dst.GetSpecificGravityResult());
      dst.SetBloodResult((SEUrinalysis.ePresenceIndicator)(int)src.Blood);
      if (src.PH != null)
        PBProperty.Load(src.PH, dst.GetPHResult());
      dst.SetProteinResult((SEUrinalysis.ePresenceIndicator)(int)src.Protein);
      if (src.Urobilinogen != null)
        PBProperty.Load(src.Urobilinogen, dst.GetUrobilinogenResult());
      dst.SetNitriteResult((SEUrinalysis.ePresenceIndicator)(int)src.Nitrite);
      dst.SetLeukocyteEsteraseResult((SEUrinalysis.ePresenceIndicator)(int)src.LeukocyteEsterase);

      if (src.Microscopic != null)
        PBPatientAssessmets.Load(src.Microscopic, dst.GetMicroscopicResult());
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
      if (src.HasColorResult())
        dst.Color = (pulse.cdm.bind.UrinalysisData.Types.eUrineColor)(int)src.GetColorResult();
      if (src.HasAppearanceResult())
        dst.Appearance = (pulse.cdm.bind.UrinalysisData.Types.eClarityIndicator)(int)src.GetAppearanceResult();
      if (src.HasGlucoseResult())
        dst.Glucose = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetGlucoseResult();
      if (src.HasKetoneResult())
        dst.Ketone = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetKetoneResult();
      if (src.HasBilirubinResult())
        dst.Bilirubin = PBProperty.Unload(src.GetBilirubinResult());
      if (src.HasSpecificGravityResult())
        dst.SpecificGravity = PBProperty.Unload(src.GetSpecificGravityResult());
      if (src.HasBloodResult())
        dst.Blood = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetBloodResult();
      if (src.HasPHResult())
        dst.PH = PBProperty.Unload(src.GetPHResult());
      if (src.HasProteinResult())
        dst.Protein = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetProteinResult();
      if (src.HasUrobilinogenResult())
        dst.Urobilinogen = PBProperty.Unload(src.GetUrobilinogenResult());
      if (src.HasNitriteResult())
        dst.Nitrite = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetNitriteResult();
      if (src.HasLeukocyteEsteraseResult())
        dst.LeukocyteEsterase = (pulse.cdm.bind.UrinalysisData.Types.ePresenceIndicator)(int)src.GetLeukocyteEsteraseResult();

      if (src.HasMicroscopicResult())
        dst.Microscopic = PBPatientAssessmets.Unload(src.GetMicroscopicResult());
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
      dst.SetObservationType((SEUrinalysisMicroscopic.eObservationType)(int)src.ObservationType);
      if (src.RedBloodCells != null)
        PBProperty.Load(src.RedBloodCells, dst.GetRedBloodCellsResult());
      if (src.WhiteBloodCells != null)
        PBProperty.Load(src.WhiteBloodCells, dst.GetWhiteBloodCellsResult());
      dst.SetEpithelialCellsResult((SEUrinalysisMicroscopic.eObservationAmount)(int)src.EpithelialCells);
      if (src.Casts != null)
        PBProperty.Load(src.Casts, dst.GetCastsResult());
      dst.SetCrystalsResult((SEUrinalysisMicroscopic.eObservationAmount)(int)src.Crystals);
      dst.SetBacteriaResult((SEUrinalysisMicroscopic.eObservationAmount)(int)src.Bacteria);
      dst.SetTrichomonadsResult((SEUrinalysisMicroscopic.eObservationAmount)(int)src.Trichomonads);
      dst.SetYeastResult((SEUrinalysisMicroscopic.eObservationAmount)(int)src.Yeast);
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
      if (src.HasRedBloodCellsResult())
        dst.RedBloodCells = PBProperty.Unload(src.GetRedBloodCellsResult());
      if (src.HasWhiteBloodCellsResult())
        dst.WhiteBloodCells = PBProperty.Unload(src.GetWhiteBloodCellsResult());
      if (src.HasEpithelialCellsResult())
        dst.EpithelialCells = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetEpithelialCellsResult();
      if (src.HasCastsResult())
        dst.Casts = PBProperty.Unload(src.GetCastsResult());
      if (src.HasCrystalsResult())
        dst.Crystals = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetCrystalsResult();
      if (src.HasBacteriaResult())
        dst.Bacteria = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetBacteriaResult();
      if (src.HasTrichomonadsResult())
        dst.Trichomonads = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetTrichomonadsResult();
      if (src.HasYeastResult())
        dst.Yeast = (pulse.cdm.bind.UrinalysisMicroscopicData.Types.eObservationAmount)(int)src.GetYeastResult();
    }
    #endregion
  }
}
