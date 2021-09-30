/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEArterialBloodGasTest : SEPatientAssessment
  {
    protected SEScalar blood_pH;
    protected SEScalarEquivalentWeightPerVolume bicarbonate;
    protected SEScalarPressure partial_pressure_of_oxygen;
    protected SEScalarPressure partial_pressure_of_carbon_dioxide;
    protected SEScalar oxygen_saturation;

    public override void Clear()
    {
      base.Clear();
      if (this.blood_pH != null)
        this.blood_pH.Invalidate();
      if (this.bicarbonate != null)
        this.bicarbonate.Invalidate();
      if (this.partial_pressure_of_oxygen != null)
        this.partial_pressure_of_oxygen.Invalidate();
      if (this.partial_pressure_of_carbon_dioxide != null)
        this.partial_pressure_of_carbon_dioxide.Invalidate();
      if (this.oxygen_saturation != null)
        this.oxygen_saturation.Invalidate();
    }

    public bool HasBloodPH()
    {
      return blood_pH == null ? false : blood_pH.IsValid();
    }
    public SEScalar GetBloodPH()
    {
      if (blood_pH == null)
        blood_pH = new SEScalar();
      return blood_pH;
    }

    public bool HasBicarbonate()
    {
      return bicarbonate == null ? false : bicarbonate.IsValid();
    }
    public SEScalarEquivalentWeightPerVolume GetBicarbonate()
    {
      if (bicarbonate == null)
        bicarbonate = new SEScalarEquivalentWeightPerVolume();
      return bicarbonate;
    }

    public bool HasPartialPressureOfOxygen()
    {
      return partial_pressure_of_oxygen == null ? false : partial_pressure_of_oxygen.IsValid();
    }
    public SEScalarPressure GetPartialPressureOfOxygen()
    {
      if (partial_pressure_of_oxygen == null)
        partial_pressure_of_oxygen = new SEScalarPressure();
      return partial_pressure_of_oxygen;
    }

    public bool HasPartialPressureOfCarbonDioxide()
    {
      return partial_pressure_of_carbon_dioxide == null ? false : partial_pressure_of_carbon_dioxide.IsValid();
    }
    public SEScalarPressure GetPartialPressureOfCarbonDioxide()
    {
      if (partial_pressure_of_carbon_dioxide == null)
        partial_pressure_of_carbon_dioxide = new SEScalarPressure();
      return partial_pressure_of_carbon_dioxide;
    }

    public bool HasOxygenSaturation()
    {
      return oxygen_saturation == null ? false : oxygen_saturation.IsValid();
    }
    public SEScalar GetOxygenSaturation()
    {
      if (oxygen_saturation == null)
        oxygen_saturation = new SEScalar();
      return oxygen_saturation;
    }

  }
}
