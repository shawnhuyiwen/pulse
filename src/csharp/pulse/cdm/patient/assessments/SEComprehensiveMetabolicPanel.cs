/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEComprehensiveMetabolicPanel : SEPatientAssessment
  {
    protected SEScalarMassPerVolume albumin;
    protected SEScalarMassPerVolume ALP;
    protected SEScalarMassPerVolume ALT;
    protected SEScalarMassPerVolume AST;
    protected SEScalarMassPerVolume BUN;
    protected SEScalarMassPerVolume calcium;
    protected SEScalarAmountPerVolume chloride;
    protected SEScalarAmountPerVolume CO2;
    protected SEScalarMassPerVolume creatinine;
    protected SEScalarMassPerVolume glucose;
    protected SEScalarAmountPerVolume potassium;
    protected SEScalarAmountPerVolume sodium;
    protected SEScalarMassPerVolume total_bilirubin;
    protected SEScalarMassPerVolume total_protein;

    public override void Clear()
    {
      base.Clear();
      if (this.albumin != null)
        this.albumin.Invalidate();
      if (this.ALP != null)
        this.ALP.Invalidate();
      if (this.ALT != null)
        this.ALT.Invalidate();
      if (this.AST != null)
        this.AST.Invalidate();
      if (this.BUN != null)
        this.BUN.Invalidate();
      if (this.calcium != null)
        this.calcium.Invalidate();
      if (this.chloride != null)
        this.chloride.Invalidate();
      if (this.CO2 != null)
        this.CO2.Invalidate();
      if (this.creatinine != null)
        this.creatinine.Invalidate();
      if (this.glucose != null)
        this.glucose.Invalidate();
      if (this.potassium != null)
        this.potassium.Invalidate();
      if (this.sodium != null)
        this.sodium.Invalidate();
      if (this.total_bilirubin != null)
        this.total_bilirubin.Invalidate();
      if (this.total_protein != null)
        this.total_protein.Invalidate();
    }

    public bool HasAlbumin()
    {
      return albumin == null ? false : albumin.IsValid();
    }
    public SEScalarMassPerVolume GetAlbumin()
    {
      if (albumin == null)
        albumin = new SEScalarMassPerVolume();
      return albumin;
    }

    public bool HasALP()
    {
      return ALP == null ? false : ALP.IsValid();
    }
    public SEScalarMassPerVolume GetALP()
    {
      if (ALP == null)
        ALP = new SEScalarMassPerVolume();
      return ALP;
    }

    public bool HasALT()
    {
      return ALT == null ? false : ALT.IsValid();
    }
    public SEScalarMassPerVolume GetALT()
    {
      if (ALT == null)
        ALT = new SEScalarMassPerVolume();
      return ALT;
    }

    public bool HasAST()
    {
      return AST == null ? false : AST.IsValid();
    }
    public SEScalarMassPerVolume GetAST()
    {
      if (AST == null)
        AST = new SEScalarMassPerVolume();
      return AST;
    }

    public bool HasBUN()
    {
      return BUN == null ? false : BUN.IsValid();
    }
    public SEScalarMassPerVolume GetBUN()
    {
      if (BUN == null)
        BUN = new SEScalarMassPerVolume();
      return BUN;
    }

    public bool HasCalcium()
    {
      return calcium == null ? false : calcium.IsValid();
    }
    public SEScalarMassPerVolume GetCalcium()
    {
      if (calcium == null)
        calcium = new SEScalarMassPerVolume();
      return calcium;
    }

    public bool HasChloride()
    {
      return chloride == null ? false : chloride.IsValid();
    }
    public SEScalarAmountPerVolume GetChloride()
    {
      if (chloride == null)
        chloride = new SEScalarAmountPerVolume();
      return chloride;
    }

    public bool HasCO2()
    {
      return CO2 == null ? false : CO2.IsValid();
    }
    public SEScalarAmountPerVolume GetCO2()
    {
      if (CO2 == null)
        CO2 = new SEScalarAmountPerVolume();
      return CO2;
    }

    public bool HasCreatinine()
    {
      return creatinine == null ? false : creatinine.IsValid();
    }
    public SEScalarMassPerVolume GetCreatinine()
    {
      if (creatinine == null)
        creatinine = new SEScalarMassPerVolume();
      return creatinine;
    }

    public bool HasGlucose()
    {
      return glucose == null ? false : glucose.IsValid();
    }
    public SEScalarMassPerVolume GetGlucose()
    {
      if (glucose == null)
        glucose = new SEScalarMassPerVolume();
      return glucose;
    }

    public bool HasPotassium()
    {
      return potassium == null ? false : potassium.IsValid();
    }
    public SEScalarAmountPerVolume GetPotassium()
    {
      if (potassium == null)
        potassium = new SEScalarAmountPerVolume();
      return potassium;
    }

    public bool HasSodium()
    {
      return sodium == null ? false : sodium.IsValid();
    }
    public SEScalarAmountPerVolume GetSodium()
    {
      if (sodium == null)
        sodium = new SEScalarAmountPerVolume();
      return sodium;
    }

    public bool HasTotalBilirubin()
    {
      return total_bilirubin == null ? false : total_bilirubin.IsValid();
    }
    public SEScalarMassPerVolume GetTotalBilirubin()
    {
      if (total_bilirubin == null)
        total_bilirubin = new SEScalarMassPerVolume();
      return total_bilirubin;
    }

    public bool HasTotalProtein()
    {
      return total_protein == null ? false : total_protein.IsValid();
    }
    public SEScalarMassPerVolume GetTotalProtein()
    {
      if (total_protein == null)
        total_protein = new SEScalarMassPerVolume();
      return total_protein;
    }

  }
}
