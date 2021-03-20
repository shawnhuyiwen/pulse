/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEPulmonaryFunctionTest : SEPatientAssessment
  {
    protected SEScalarVolume expiratoryReserveVolume;
    protected SEScalarVolume forcedVitalCapacity;
    protected SEScalarVolume forcedExpiratoryVolume;
    protected SEScalarVolumePerTime forcedExpiratoryFlow;
    protected SEScalarVolume functionalResidualCapacity;
    protected SEScalarVolume inspiratoryCapacity;
    protected SEScalarVolume inspiratoryReserveVolume;
    protected SEScalarVolume maximumVoluntaryVentilation;
    protected SEScalarVolumePerTime peakExpiratoryFlow;
    protected SEScalarVolume residualVolume;
    protected SEScalarVolume slowVitalCapacity;
    protected SEScalarVolume totalLungCapacity;
    protected SEScalarVolume vitalCapacity;

    public override void Clear()
    {
      base.Clear();
      if (this.expiratoryReserveVolume != null)
        this.expiratoryReserveVolume.Invalidate();
      if (this.forcedVitalCapacity != null)
        this.forcedVitalCapacity.Invalidate();
      if (this.forcedExpiratoryVolume != null)
        this.forcedExpiratoryVolume.Invalidate();
      if (this.forcedExpiratoryFlow != null)
        this.forcedExpiratoryFlow.Invalidate();
      if (this.functionalResidualCapacity != null)
        this.functionalResidualCapacity.Invalidate();
      if (this.inspiratoryCapacity != null)
        this.inspiratoryCapacity.Invalidate();
      if (this.inspiratoryReserveVolume != null)
        this.inspiratoryReserveVolume.Invalidate();
      if (this.maximumVoluntaryVentilation != null)
        this.maximumVoluntaryVentilation.Invalidate();
      if (this.peakExpiratoryFlow != null)
        this.peakExpiratoryFlow.Invalidate();
      if (this.residualVolume != null)
        this.residualVolume.Invalidate();
      if (this.slowVitalCapacity != null)
        this.slowVitalCapacity.Invalidate();
      if (this.totalLungCapacity != null)
        this.totalLungCapacity.Invalidate();
      if (this.vitalCapacity != null)
        this.vitalCapacity.Invalidate();
    }

    public bool HasExpiratoryReserveVolume()
    {
      return expiratoryReserveVolume == null ? false : expiratoryReserveVolume.IsValid();
    }
    public SEScalarVolume GetExpiratoryReserveVolume()
    {
      if (expiratoryReserveVolume == null)
        expiratoryReserveVolume = new SEScalarVolume();
      return expiratoryReserveVolume;
    }

    public bool HasForcedVitalCapacity()
    {
      return forcedVitalCapacity == null ? false : forcedVitalCapacity.IsValid();
    }
    public SEScalarVolume GetForcedVitalCapacity()
    {
      if (forcedVitalCapacity == null)
        forcedVitalCapacity = new SEScalarVolume();
      return forcedVitalCapacity;
    }

    public bool HasForcedExpiratoryVolume()
    {
      return forcedExpiratoryVolume == null ? false : forcedExpiratoryVolume.IsValid();
    }
    public SEScalarVolume GetForcedExpiratoryVolume()
    {
      if (forcedExpiratoryVolume == null)
        forcedExpiratoryVolume = new SEScalarVolume();
      return forcedExpiratoryVolume;
    }

    public bool HasForcedExpiratoryFlow()
    {
      return forcedExpiratoryFlow == null ? false : forcedExpiratoryFlow.IsValid();
    }
    public SEScalarVolumePerTime GetForcedExpiratoryFlow()
    {
      if (forcedExpiratoryFlow == null)
        forcedExpiratoryFlow = new SEScalarVolumePerTime();
      return forcedExpiratoryFlow;
    }

    public bool HasFunctionalResidualCapacity()
    {
      return functionalResidualCapacity == null ? false : functionalResidualCapacity.IsValid();
    }
    public SEScalarVolume GetFunctionalResidualCapacity()
    {
      if (functionalResidualCapacity == null)
        functionalResidualCapacity = new SEScalarVolume();
      return functionalResidualCapacity;
    }

    public bool HasInspiratoryCapacity()
    {
      return inspiratoryCapacity == null ? false : inspiratoryCapacity.IsValid();
    }
    public SEScalarVolume GetInspiratoryCapacity()
    {
      if (inspiratoryCapacity == null)
        inspiratoryCapacity = new SEScalarVolume();
      return inspiratoryCapacity;
    }

    public bool HasInspiratoryReserveVolume()
    {
      return inspiratoryReserveVolume == null ? false : inspiratoryReserveVolume.IsValid();
    }
    public SEScalarVolume GetInspiratoryReserveVolume()
    {
      if (inspiratoryReserveVolume == null)
        inspiratoryReserveVolume = new SEScalarVolume();
      return inspiratoryReserveVolume;
    }

    public bool HasMaximumVoluntaryVentilation()
    {
      return maximumVoluntaryVentilation == null ? false : maximumVoluntaryVentilation.IsValid();
    }
    public SEScalarVolume GetMaximumVoluntaryVentilation()
    {
      if (maximumVoluntaryVentilation == null)
        maximumVoluntaryVentilation = new SEScalarVolume();
      return maximumVoluntaryVentilation;
    }

    public bool HasPeakExpiratoryFlow()
    {
      return peakExpiratoryFlow == null ? false : peakExpiratoryFlow.IsValid();
    }
    public SEScalarVolumePerTime GetPeakExpiratoryFlow()
    {
      if (peakExpiratoryFlow == null)
        peakExpiratoryFlow = new SEScalarVolumePerTime();
      return peakExpiratoryFlow;
    }

    public bool HasResidualVolume()
    {
      return residualVolume == null ? false : residualVolume.IsValid();
    }
    public SEScalarVolume GetResidualVolume()
    {
      if (residualVolume == null)
        residualVolume = new SEScalarVolume();
      return residualVolume;
    }

    public bool HasSlowVitalCapacity()
    {
      return slowVitalCapacity == null ? false : slowVitalCapacity.IsValid();
    }
    public SEScalarVolume GetSlowVitalCapacity()
    {
      if (slowVitalCapacity == null)
        slowVitalCapacity = new SEScalarVolume();
      return slowVitalCapacity;
    }

    public bool HasTotalLungCapacity()
    {
      return totalLungCapacity == null ? false : totalLungCapacity.IsValid();
    }
    public SEScalarVolume GetTotalLungCapacity()
    {
      if (totalLungCapacity == null)
        totalLungCapacity = new SEScalarVolume();
      return totalLungCapacity;
    }

    public bool HasVitalCapacity()
    {
      return vitalCapacity == null ? false : vitalCapacity.IsValid();
    }
    public SEScalarVolume GetVitalCapacity()
    {
      if (vitalCapacity == null)
        vitalCapacity = new SEScalarVolume();
      return vitalCapacity;
    }

  }
}
