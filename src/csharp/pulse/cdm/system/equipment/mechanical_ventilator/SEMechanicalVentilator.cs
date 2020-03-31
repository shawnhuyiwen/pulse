/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SEMechanicalVentilator : SEEquipment
  {
    // Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
    public enum Connection : int
    {
      NullConnection = 0,/** Signals not provided, or no change */
      Off,
      Mask,
      Tube
    }
    // Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
    public enum Control : int
    {
      NullControl = 0,/** Signals not provided, or no change */
      P_CMV
    }
    // Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
    public enum DriverWaveform : int
    {
      NullDriverWaveform = 0,/** Signals not provided, or no change */
      Square
    }

    protected SEScalarTime breathPeriod;
    protected Connection connection;
    protected Control control;
    protected DriverWaveform driverWaveform;
    protected SEScalarTime expiratoryPeriod;
    protected SEScalar inspiratoryExpiratoryRatio;
    protected SEScalarTime inspiratoryPeriod;
    protected SEScalarPressure peakInspiratoryPressure;
    protected SEScalarPressure positiveEndExpiredPressure;
    protected SEScalarFrequency respiratoryRate;

    public SEMechanicalVentilator()
    {
      breathPeriod = null;
      connection = Connection.NullConnection;
      control = Control.NullControl;
      driverWaveform = DriverWaveform.NullDriverWaveform;
      expiratoryPeriod = null;
      inspiratoryExpiratoryRatio = null;
      inspiratoryPeriod = null;
      peakInspiratoryPressure = null;
      positiveEndExpiredPressure = null;
      respiratoryRate = null;
    }

    public override void Clear()
    {
      base.Clear();
      if (breathPeriod != null)
        breathPeriod.Invalidate();
      connection = Connection.NullConnection;
      control = Control.NullControl;
      driverWaveform = DriverWaveform.NullDriverWaveform;
      if (expiratoryPeriod != null)
        expiratoryPeriod.Invalidate();
      if (inspiratoryExpiratoryRatio != null)
        inspiratoryExpiratoryRatio.Invalidate();
      if (inspiratoryPeriod != null)
        inspiratoryPeriod.Invalidate();
      if (peakInspiratoryPressure != null)
        peakInspiratoryPressure.Invalidate();
      if (positiveEndExpiredPressure != null)
        positiveEndExpiredPressure.Invalidate();
      if (respiratoryRate != null)
        respiratoryRate.Invalidate();
    }

    public void Copy(SEMechanicalVentilator from)
    {
      base.Copy(from);
      if (from.HasBreathPeriod())
        this.GetBreathPeriod().Set(from.GetBreathPeriod());
      if (from.connection != Connection.NullConnection)
        this.connection = from.connection;
      if (from.control != Control.NullControl)
        this.control = from.control;
      if (from.driverWaveform != DriverWaveform.NullDriverWaveform)
        this.driverWaveform = from.driverWaveform;
      if (from.HasExpiratoryPeriod())
        this.GetExpiratoryPeriod().Set(from.GetExpiratoryPeriod());
      if (from.HasInspiratoryExpiratoryRatio())
        this.GetInspiratoryExpiratoryRatio().Set(from.GetInspiratoryExpiratoryRatio());
      if (from.HasInspiratoryPeriod())
        this.GetInspiratoryPeriod().Set(from.GetInspiratoryPeriod());
      if (from.HasPeakInspiratoryPressure())
        this.GetPeakInspiratoryPressure().Set(from.GetPeakInspiratoryPressure());
      if (from.HasPositiveEndExpiredPressure())
        this.GetPositiveEndExpiredPressure().Set(from.GetPositiveEndExpiredPressure());
      if (from.HasRespiratoryRate())
        this.GetRespiratoryRate().Set(from.GetRespiratoryRate());
    }

    public SEScalarTime GetBreathPeriod()
    {
      if (breathPeriod == null)
        breathPeriod = new SEScalarTime();
      return breathPeriod;
    }
    public bool HasBreathPeriod()
    {
      return breathPeriod == null ? false : breathPeriod.IsValid();
    }

    public Connection GetConnection()
    {
      return connection;
    }
    public void SetConnection(Connection c)
    {
      connection = c;
    }
    public bool HasConnection()
    {
      return connection != Connection.NullConnection;
    }

    public Control GetControl()
    {
      return control;
    }
    public void SetControl(Control c)
    {
      control = c;
    }
    public bool HasControl()
    {
      return control != Control.NullControl;
    }

    public DriverWaveform GetDriverWaveform()
    {
      return driverWaveform;
    }
    public void SetDriverWaveform(DriverWaveform w)
    {
      driverWaveform = w;
    }
    public bool HasDriverWaveform()
    {
      return driverWaveform != DriverWaveform.NullDriverWaveform;
    }

    public SEScalarTime GetExpiratoryPeriod()
    {
      if (expiratoryPeriod == null)
        expiratoryPeriod = new SEScalarTime();
      return expiratoryPeriod;
    }
    public bool HasExpiratoryPeriod()
    {
      return expiratoryPeriod == null ? false : expiratoryPeriod.IsValid();
    }

    public SEScalar GetInspiratoryExpiratoryRatio()
    {
      if (inspiratoryExpiratoryRatio == null)
        inspiratoryExpiratoryRatio = new SEScalar();
      return inspiratoryExpiratoryRatio;
    }
    public bool HasInspiratoryExpiratoryRatio()
    {
      return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.IsValid();
    }

    public SEScalarTime GetInspiratoryPeriod()
    {
      if (inspiratoryPeriod == null)
        inspiratoryPeriod = new SEScalarTime();
      return inspiratoryPeriod;
    }
    public bool HasInspiratoryPeriod()
    {
      return inspiratoryPeriod == null ? false : inspiratoryPeriod.IsValid();
    }

    public SEScalarPressure GetPeakInspiratoryPressure()
    {
      if (peakInspiratoryPressure == null)
        peakInspiratoryPressure = new SEScalarPressure();
      return peakInspiratoryPressure;
    }
    public bool HasPeakInspiratoryPressure()
    {
      return peakInspiratoryPressure == null ? false : peakInspiratoryPressure.IsValid();
    }

    public SEScalarPressure GetPositiveEndExpiredPressure()
    {
      if (positiveEndExpiredPressure == null)
        positiveEndExpiredPressure = new SEScalarPressure();
      return positiveEndExpiredPressure;
    }
    public bool HasPositiveEndExpiredPressure()
    {
      return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.IsValid();
    }

    public SEScalarFrequency GetRespiratoryRate()
    {
      if (respiratoryRate == null)
        respiratoryRate = new SEScalarFrequency();
      return respiratoryRate;
    }
    public bool HasRespiratoryRate()
    {
      return respiratoryRate == null ? false : respiratoryRate.IsValid();
    }

    public override string ToString()
    {
      return "Mechanical Ventilator"
      + "\n\tBreathPeriod: " + GetBreathPeriod()
      + "\n\tConnection: " + (HasConnection() ? PBMechanicalVentilator.Connection_Name(GetConnection()) : "NotProvided")
      + "\n\tConnection: " + (HasConnection() ? PBMechanicalVentilator.Connection_Name(GetConnection()) : "NotProvided")
      + "\n\tConnection: " + (HasConnection() ? PBMechanicalVentilator.Connection_Name(GetConnection()) : "NotProvided")
      + "\n\tExpiratoryPeriod: " + GetExpiratoryPeriod()
      + "\n\tInspiratoryExpiratoryRatio: " + GetInspiratoryExpiratoryRatio()
      + "\n\tInspiratoryPeriod: " + GetInspiratoryPeriod()
      + "\n\tPeakInspiratoryPressure: " + GetPeakInspiratoryPressure()
      + "\n\tPositiveEndExpiredPressure: " + GetPositiveEndExpiredPressure()
      + "\n\tRespiratoryRate: " + GetRespiratoryRate();
    }
  }
}