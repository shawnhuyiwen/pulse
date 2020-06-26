/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

using System.Collections.Generic;

namespace Pulse.CDM
{
  // TODO Substance Fractions and Concentrations!!!
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
    public enum DriverWaveform : int
    {
      NullDriverWaveform = 0,/** Signals not provided, or no change */
      Square
    }

    protected Connection connection;
    protected SEScalarPressureTimePerVolume endotracheal_tube_resistance;

    // Expiratory Baseline (Only set one)
    protected SEScalarPressure positive_end_expired_pressure;
    protected SEScalarPressure functional_residual_capacity;

    // Expiration Cyclye (Only set one)
    protected SEScalarVolumePerTime expiration_cycle_flow;
    protected SEScalarPressure expiration_cycle_pressure;
    protected SEScalarTime expiration_cycle_time;
    protected SEScalarVolume expiration_cycle_volume;

    protected SEScalarPressureTimePerVolume expiration_tube_resistance;
    protected SEScalarPressureTimePerVolume expiration_valve_resistance;
    protected DriverWaveform expiration_waveform;

    // Inspiration Limit (Only set one)
    protected SEScalarVolumePerTime inspiration_limit_flow;
    protected SEScalarPressure inspiration_limit_pressure;
    protected SEScalarVolume inspiration_limit_volume;

    protected SEScalarTime inspiration_pause_time;

    // Inspiratory Target (Only set one)
    protected SEScalarPressure peak_inpiratory_pressure;
    protected SEScalarVolumePerTime inspiration_target_flow;

    // Inspiration Machine Trigger (Only set one)
    protected SEScalarTime inspiration_machine_trigger_time;

    // Inspiration Patient Trigger (Only set one)
    protected SEScalarVolumePerTime inspiration_patient_trigger_flow;
    protected SEScalarPressure inspiration_patient_trigger_pressure;

    protected SEScalarPressureTimePerVolume inspiration_tube_resistance;
    protected SEScalarPressureTimePerVolume inspiration_valve_resistance;
    protected DriverWaveform inspiration_waveform;

    protected List<SESubstanceFraction> fraction_inspired_gases;
    protected List<SESubstanceConcentration> concentration_inspired_aerosols;

    public SEMechanicalVentilator()
    {
      connection = Connection.NullConnection;
      endotracheal_tube_resistance = null;
      positive_end_expired_pressure = null;
      functional_residual_capacity = null;
      expiration_cycle_flow = null;
      expiration_cycle_pressure = null;
      expiration_cycle_time = null;
      expiration_cycle_volume = null;
      expiration_tube_resistance = null;
      expiration_valve_resistance = null;
      expiration_waveform = DriverWaveform.NullDriverWaveform;


      inspiration_limit_flow = null;
      inspiration_limit_pressure = null;
      inspiration_limit_volume = null;
      inspiration_pause_time = null;
      peak_inpiratory_pressure = null;
      inspiration_target_flow = null;
      inspiration_machine_trigger_time = null;
      inspiration_patient_trigger_flow = null;
      inspiration_patient_trigger_pressure = null;
      inspiration_tube_resistance = null;
      inspiration_valve_resistance = null;
      inspiration_waveform = DriverWaveform.NullDriverWaveform;

      this.fraction_inspired_gases = new List<SESubstanceFraction>();
      this.concentration_inspired_aerosols = new List<SESubstanceConcentration>();
    }

    public override void Clear()
    {
      base.Clear();
      connection = Connection.NullConnection;
      if (endotracheal_tube_resistance != null)
        endotracheal_tube_resistance.Invalidate();
      if (positive_end_expired_pressure != null)
        positive_end_expired_pressure.Invalidate();
      if (functional_residual_capacity != null)
        functional_residual_capacity.Invalidate();
      if (expiration_cycle_flow != null)
        expiration_cycle_flow.Invalidate();
      if (expiration_cycle_pressure != null)
        expiration_cycle_pressure.Invalidate();
      if (expiration_cycle_time != null)
        expiration_cycle_time.Invalidate();
      if (expiration_cycle_volume != null)
        expiration_cycle_volume.Invalidate();
      if (expiration_tube_resistance != null)
        expiration_tube_resistance.Invalidate();
      if (expiration_valve_resistance != null)
        expiration_valve_resistance.Invalidate();
      expiration_waveform = DriverWaveform.NullDriverWaveform;


      if (inspiration_limit_flow != null)
        inspiration_limit_flow.Invalidate();
      if (inspiration_limit_pressure != null)
        inspiration_limit_pressure.Invalidate();
      if (inspiration_limit_volume != null)
        inspiration_limit_volume.Invalidate();
      if (inspiration_pause_time != null)
        inspiration_pause_time.Invalidate();
      if (peak_inpiratory_pressure != null)
        peak_inpiratory_pressure.Invalidate();
      if (inspiration_target_flow != null)
        inspiration_target_flow.Invalidate();
      if (inspiration_machine_trigger_time != null)
        inspiration_machine_trigger_time.Invalidate();
      if (inspiration_patient_trigger_flow != null)
        inspiration_patient_trigger_flow.Invalidate();
      if (inspiration_patient_trigger_pressure != null)
        inspiration_patient_trigger_pressure.Invalidate();
      if (inspiration_tube_resistance != null)
        inspiration_tube_resistance.Invalidate();
      if (inspiration_valve_resistance != null)
        inspiration_valve_resistance.Invalidate();
      inspiration_waveform = DriverWaveform.NullDriverWaveform;

      fraction_inspired_gases.Clear();
      concentration_inspired_aerosols.Clear();
    }

    public void Copy(SEMechanicalVentilator from)
    {
      base.Copy(from);
      if (from.connection != Connection.NullConnection)
        this.connection = from.connection;
      if (from.HasEndotrachealTubeResistance())
        this.GetEndotrachealTubeResistance().Set(from.GetEndotrachealTubeResistance());
      if (from.HasPositiveEndExpiredPressure())
        this.GetPositiveEndExpiredPressure().Set(from.GetPositiveEndExpiredPressure());
      if (from.HasFunctionalResidualCapacity())
        this.GetFunctionalResidualCapacity().Set(from.GetFunctionalResidualCapacity());
      if (from.HasExpirationCycleFlow())
        this.GetExpirationCycleFlow().Set(from.GetExpirationCycleFlow());
      if (from.HasExpirationCyclePressure())
        this.GetExpirationCyclePressure().Set(from.GetExpirationCyclePressure());
      if (from.HasExpirationCycleTime())
        this.GetExpirationCycleTime().Set(from.GetExpirationCycleTime());
      if (from.HasExpirationCycleVolume())
        this.GetExpirationCycleVolume().Set(from.GetExpirationCycleVolume());
      if (from.HasExpirationTubeResistance())
        this.GetExpirationTubeResistance().Set(from.GetExpirationTubeResistance());
      if (from.HasExpirationValveResistance())
        this.GetExpirationValveResistance().Set(from.GetExpirationValveResistance());
      if (from.expiration_waveform != DriverWaveform.NullDriverWaveform)
        this.expiration_waveform = from.expiration_waveform;

      if (from.HasInspirationLimitFlow())
        this.GetInspirationLimitFlow().Set(from.GetInspirationLimitFlow());
      if (from.HasInspirationLimitPressure())
        this.GetInspirationLimitPressure().Set(from.GetInspirationLimitPressure());
      if (from.HasInspirationLimitVolume())
        this.GetInspirationLimitVolume().Set(from.GetInspirationLimitVolume());
      if (from.HasInspirationPauseTime())
        this.GetInspirationPauseTime().Set(from.GetInspirationPauseTime());
      if (from.HasPeakInspiratoryPressure())
        this.GetPeakInspiratoryPressure().Set(from.GetPeakInspiratoryPressure());
      if (from.HasInspirationTargetFlow())
        this.GetInspirationTargetFlow().Set(from.GetInspirationTargetFlow());
      if (from.HasInspirationMachineTriggerTime())
        this.GetInspirationMachineTriggerTime().Set(from.GetInspirationMachineTriggerTime());
      if (from.HasInspirationPatientTriggerFlow())
        this.GetInspirationPatientTriggerFlow().Set(from.GetInspirationPatientTriggerFlow());
      if (from.HasInspirationPatientTriggerPressure())
        this.GetInspirationPatientTriggerPressure().Set(from.GetInspirationPatientTriggerPressure());
      if (from.HasInspirationTubeResistance())
        this.GetInspirationTubeResistance().Set(from.GetInspirationTubeResistance());
      if (from.HasInspirationValveResistance())
        this.GetInspirationValveResistance().Set(from.GetInspirationValveResistance());
      if (from.inspiration_waveform != DriverWaveform.NullDriverWaveform)
        this.inspiration_waveform = from.inspiration_waveform;

      if (from.fraction_inspired_gases != null)
      {
        SESubstanceFraction mine;
        foreach (SESubstanceFraction sf in from.fraction_inspired_gases)
        {
          mine = this.CreateFractionInspiredGas(sf.GetSubstance());
          if (sf.HasFractionAmount())
            mine.GetFractionAmount().Set(sf.GetFractionAmount());
        }
      }

      if (from.concentration_inspired_aerosols != null)
      {
        SESubstanceConcentration mine;
        foreach (SESubstanceConcentration sc in from.concentration_inspired_aerosols)
        {
          mine = this.CreateConcentrationInspiredAerosol(sc.GetSubstance());
          if (sc.HasConcentration())
            mine.GetConcentration().Set(sc.GetConcentration());
        }
      }
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

    public SEScalarPressureTimePerVolume GetEndotrachealTubeResistance()
    {
      if (endotracheal_tube_resistance == null)
        endotracheal_tube_resistance = new SEScalarPressureTimePerVolume();
      return endotracheal_tube_resistance;
    }
    public bool HasEndotrachealTubeResistance()
    {
      return endotracheal_tube_resistance == null ? false : endotracheal_tube_resistance.IsValid();
    }

    public SEScalarPressure GetPositiveEndExpiredPressure()
    {
      if (positive_end_expired_pressure == null)
        positive_end_expired_pressure = new SEScalarPressure();
      return positive_end_expired_pressure;
    }
    public bool HasPositiveEndExpiredPressure()
    {
      return positive_end_expired_pressure == null ? false : positive_end_expired_pressure.IsValid();
    }

    public SEScalarPressure GetFunctionalResidualCapacity()
    {
      if (functional_residual_capacity == null)
        functional_residual_capacity = new SEScalarPressure();
      return functional_residual_capacity;
    }
    public bool HasFunctionalResidualCapacity()
    {
      return functional_residual_capacity == null ? false : functional_residual_capacity.IsValid();
    }

    public SEScalarVolumePerTime GetExpirationCycleFlow()
    {
      if (expiration_cycle_flow == null)
        expiration_cycle_flow = new SEScalarVolumePerTime();
      return expiration_cycle_flow;
    }
    public bool HasExpirationCycleFlow()
    {
      return expiration_cycle_flow == null ? false : expiration_cycle_flow.IsValid();
    }

    public SEScalarPressure GetExpirationCyclePressure()
    {
      if (expiration_cycle_pressure == null)
        expiration_cycle_pressure = new SEScalarPressure();
      return expiration_cycle_pressure;
    }
    public bool HasExpirationCyclePressure()
    {
      return expiration_cycle_pressure == null ? false : expiration_cycle_pressure.IsValid();
    }

    public SEScalarTime GetExpirationCycleTime()
    {
      if (expiration_cycle_time == null)
        expiration_cycle_time = new SEScalarTime();
      return expiration_cycle_time;
    }
    public bool HasExpirationCycleTime()
    {
      return expiration_cycle_time == null ? false : expiration_cycle_time.IsValid();
    }

    public SEScalarVolume GetExpirationCycleVolume()
    {
      if (expiration_cycle_volume == null)
        expiration_cycle_volume = new SEScalarVolume();
      return expiration_cycle_volume;
    }
    public bool HasExpirationCycleVolume()
    {
      return expiration_cycle_volume == null ? false : expiration_cycle_volume.IsValid();
    }

    public SEScalarPressureTimePerVolume GetExpirationTubeResistance()
    {
      if (expiration_tube_resistance == null)
        expiration_tube_resistance = new SEScalarPressureTimePerVolume();
      return expiration_tube_resistance;
    }
    public bool HasExpirationTubeResistance()
    {
      return expiration_tube_resistance == null ? false : expiration_tube_resistance.IsValid();
    }

    public SEScalarPressureTimePerVolume GetExpirationValveResistance()
    {
      if (expiration_valve_resistance == null)
        expiration_valve_resistance = new SEScalarPressureTimePerVolume();
      return expiration_valve_resistance;
    }
    public bool HasExpirationValveResistance()
    {
      return expiration_valve_resistance == null ? false : expiration_valve_resistance.IsValid();
    }

    public DriverWaveform GetExpirationWaveform()
    {
      return expiration_waveform;
    }
    public void SetExpirationWaveform(DriverWaveform w)
    {
      expiration_waveform = w;
    }
    public bool HasExpirationWaveform()
    {
      return expiration_waveform != DriverWaveform.NullDriverWaveform;
    }

    public SEScalarVolumePerTime GetInspirationLimitFlow()
    {
      if (inspiration_limit_flow == null)
        inspiration_limit_flow = new SEScalarVolumePerTime();
      return inspiration_limit_flow;
    }
    public bool HasInspirationLimitFlow()
    {
      return inspiration_limit_flow == null ? false : inspiration_limit_flow.IsValid();
    }

    public SEScalarPressure GetInspirationLimitPressure()
    {
      if (inspiration_limit_pressure == null)
        inspiration_limit_pressure = new SEScalarPressure();
      return inspiration_limit_pressure;
    }
    public bool HasInspirationLimitPressure()
    {
      return inspiration_limit_pressure == null ? false : inspiration_limit_pressure.IsValid();
    }

    public SEScalarVolume GetInspirationLimitVolume()
    {
      if (inspiration_limit_volume == null)
        inspiration_limit_volume = new SEScalarVolume();
      return inspiration_limit_volume;
    }
    public bool HasInspirationLimitVolume()
    {
      return inspiration_limit_volume == null ? false : inspiration_limit_volume.IsValid();
    }

    public SEScalarTime GetInspirationPauseTime()
    {
      if (inspiration_pause_time == null)
        inspiration_pause_time = new SEScalarTime();
      return inspiration_pause_time;
    }
    public bool HasInspirationPauseTime()
    {
      return inspiration_pause_time == null ? false : inspiration_pause_time.IsValid();
    }

    public SEScalarPressure GetPeakInspiratoryPressure()
    {
      if (peak_inpiratory_pressure == null)
        peak_inpiratory_pressure = new SEScalarPressure();
      return peak_inpiratory_pressure;
    }
    public bool HasPeakInspiratoryPressure()
    {
      return peak_inpiratory_pressure == null ? false : peak_inpiratory_pressure.IsValid();
    }

    public SEScalarVolumePerTime GetInspirationTargetFlow()
    {
      if (inspiration_target_flow == null)
        inspiration_target_flow = new SEScalarVolumePerTime();
      return inspiration_target_flow;
    }
    public bool HasInspirationTargetFlow()
    {
      return inspiration_target_flow == null ? false : inspiration_target_flow.IsValid();
    }

    public SEScalarTime GetInspirationMachineTriggerTime()
    {
      if (inspiration_machine_trigger_time == null)
        inspiration_machine_trigger_time = new SEScalarTime();
      return inspiration_machine_trigger_time;
    }
    public bool HasInspirationMachineTriggerTime()
    {
      return inspiration_machine_trigger_time == null ? false : inspiration_machine_trigger_time.IsValid();
    }

    public SEScalarVolumePerTime GetInspirationPatientTriggerFlow()
    {
      if (inspiration_patient_trigger_flow == null)
        inspiration_patient_trigger_flow = new SEScalarVolumePerTime();
      return inspiration_patient_trigger_flow;
    }
    public bool HasInspirationPatientTriggerFlow()
    {
      return inspiration_patient_trigger_flow == null ? false : inspiration_patient_trigger_flow.IsValid();
    }

    public SEScalarPressure GetInspirationPatientTriggerPressure()
    {
      if (inspiration_patient_trigger_pressure == null)
        inspiration_patient_trigger_pressure = new SEScalarPressure();
      return inspiration_patient_trigger_pressure;
    }
    public bool HasInspirationPatientTriggerPressure()
    {
      return inspiration_patient_trigger_pressure == null ? false : inspiration_patient_trigger_pressure.IsValid();
    }

    public SEScalarPressureTimePerVolume GetInspirationTubeResistance()
    {
      if (inspiration_tube_resistance == null)
        inspiration_tube_resistance = new SEScalarPressureTimePerVolume();
      return inspiration_tube_resistance;
    }
    public bool HasInspirationTubeResistance()
    {
      return inspiration_tube_resistance == null ? false : inspiration_tube_resistance.IsValid();
    }

    public SEScalarPressureTimePerVolume GetInspirationValveResistance()
    {
      if (inspiration_valve_resistance == null)
        inspiration_valve_resistance = new SEScalarPressureTimePerVolume();
      return inspiration_valve_resistance;
    }
    public bool HasInspirationValveResistance()
    {
      return inspiration_valve_resistance == null ? false : inspiration_valve_resistance.IsValid();
    }


    public DriverWaveform GetInspirationWaveform()
    {
      return inspiration_waveform;
    }
    public void SetInspirationWaveform(DriverWaveform w)
    {
      inspiration_waveform = w;
    }
    public bool HasInspirationWaveform()
    {
      return inspiration_waveform != DriverWaveform.NullDriverWaveform;
    }


    public SESubstanceFraction CreateFractionInspiredGas(string substance)
    {
      return GetFractionInspiredGas(substance);
    }
    public SESubstanceFraction GetFractionInspiredGas(string substance)
    {
      foreach (SESubstanceFraction sf in fraction_inspired_gases)
      {
        if (sf.GetSubstance() == substance)
        {
          return sf;
        }
      }
      SESubstanceFraction nsf = new SESubstanceFraction(substance);
      fraction_inspired_gases.Add(nsf);
      return nsf;
    }
    public bool HasFractionInspiredGas()
    {
      return fraction_inspired_gases.Count != 0;
    }
    public bool HasFractionInspiredGas(string substance)
    {
      foreach (SESubstanceFraction sf in fraction_inspired_gases)
      {
        if (sf.GetSubstance() == substance)
        {
          return true;
        }
      }
      return false;
    }
    public List<SESubstanceFraction> GetFractionInspiredGases()
    {
      return fraction_inspired_gases;
    }
    public void RemoveFractionInspiredGas(string substance)
    {
      foreach (SESubstanceFraction sf in fraction_inspired_gases)
      {
        if (sf.GetSubstance() == substance)
        {
          fraction_inspired_gases.Remove(sf);
          return;
        }
      }
    }
    public void RemoveFractionInspiredGases()
    {
      fraction_inspired_gases.Clear();
    }

    public SESubstanceConcentration CreateConcentrationInspiredAerosol(string substance)
    {
      return GetConcentrationInspiredAerosol(substance);
    }
    public SESubstanceConcentration GetConcentrationInspiredAerosol(string substance)
    {
      foreach (SESubstanceConcentration sc in concentration_inspired_aerosols)
      {
        if (sc.GetSubstance() == substance)
        {
          return sc;
        }
      }
      SESubstanceConcentration nsc = new SESubstanceConcentration(substance);
      concentration_inspired_aerosols.Add(nsc);
      return nsc;
    }
    public bool HasConcentrationInspiredAerosol()
    {
      return concentration_inspired_aerosols.Count != 0;
    }
    public bool HasConcentrationInspiredAerosol(string substance)
    {
      foreach (SESubstanceConcentration sc in concentration_inspired_aerosols)
      {
        if (sc.GetSubstance() == substance)
        {
          return true;
        }
      }
      return false;
    }
    public List<SESubstanceConcentration> GetConcentrationInspiredAerosols()
    {
      return concentration_inspired_aerosols;
    }
    public void RemoveConcentrationInspiredAerosol(string substance)
    {
      foreach (SESubstanceConcentration sc in concentration_inspired_aerosols)
      {
        if (sc.GetSubstance() == substance)
        {
          concentration_inspired_aerosols.Remove(sc);
          return;
        }
      }
    }
    public void RemoveConcentrationInspiredAerosols()
    {
      concentration_inspired_aerosols.Clear();
    }

    public override string ToString()
    {
      string str = "Mechanical Ventilator"
      + "\n\tConnection: " + (HasConnection() ? PBMechanicalVentilator.Connection_Name(GetConnection()) : "NotProvided")
      + "\n\tEndotrachealTubeResistance: " + (HasEndotrachealTubeResistance() ? GetEndotrachealTubeResistance().ToString() : "Not Provided")
      + "\n\tPositiveEndExpiredPressure: " + (HasPositiveEndExpiredPressure() ? GetPositiveEndExpiredPressure().ToString() : "Not Provided")
      + "\n\tFunctionalResidualCapacity: " + (HasFunctionalResidualCapacity() ? GetFunctionalResidualCapacity().ToString() : "Not Provided")
      + "\n\tExpirationCycleFlow: " + (HasExpirationCycleFlow() ? GetExpirationCycleFlow().ToString() : "Not Provided")
      + "\n\tExpirationCyclePressure: " + (HasExpirationCyclePressure() ? GetExpirationCyclePressure().ToString() : "Not Provided")
      + "\n\tExpirationCycleTime: " + (HasExpirationCycleTime() ? GetExpirationCycleTime().ToString() : "Not Provided")
      + "\n\tExpirationCycleVolume: " + (HasExpirationCycleVolume() ? GetExpirationCycleVolume().ToString() : "Not Provided")
      + "\n\tExpirationTubeResistance: " + (HasExpirationTubeResistance() ? GetExpirationTubeResistance().ToString() : "Not Provided")
      + "\n\tExpirationValveResistance: " + (HasExpirationValveResistance() ? GetExpirationValveResistance().ToString() : "Not Provided")
      + "\n\tExpirationWaveform: " + (HasExpirationWaveform() ? PBMechanicalVentilator.DriverWaveform_Name(expiration_waveform) : "Not Provided")
      + "\n\tInspirationLimitFlow: " + (HasInspirationLimitFlow() ? GetInspirationLimitFlow().ToString() : "Not Provided")
      + "\n\tInspirationLimitPressure: " + (HasInspirationLimitPressure() ? GetInspirationLimitPressure().ToString() : "Not Provided")
      + "\n\tInspirationLimitVolume: " + (HasInspirationLimitVolume() ? GetInspirationLimitVolume().ToString() : "Not Provided")
      + "\n\tInspirationPauseTime: " + (HasInspirationPauseTime() ? GetInspirationPauseTime().ToString() : "Not Provided")
      + "\n\tPeakInspiratoryPressure: " + (HasPeakInspiratoryPressure() ? GetPeakInspiratoryPressure().ToString() : "Not Provided")
      + "\n\tInspirationTargetFlow: " + (HasInspirationTargetFlow() ? GetInspirationTargetFlow().ToString() : "Not Provided")
      + "\n\tInspirationMachineTriggerTime: " + (HasInspirationMachineTriggerTime() ? GetInspirationMachineTriggerTime().ToString() : "Not Provided")
      + "\n\tInspirationPatientTriggerFlow: " + (HasInspirationPatientTriggerFlow() ? GetInspirationPatientTriggerFlow().ToString() : "Not Provided")
      + "\n\tInspirationPatientTriggerPressure: " + (HasInspirationPatientTriggerPressure() ? GetInspirationPatientTriggerPressure().ToString() : "Not Provided")
      + "\n\tInspirationTubeResistance: " + (HasInspirationTubeResistance() ? GetInspirationTubeResistance().ToString() : "Not Provided")
      + "\n\tInspirationValveResistance: " + (HasInspirationValveResistance() ? GetInspirationValveResistance().ToString() : "Not Provided")
      + "\n\tInspirationWaveform: " + (HasInspirationWaveform() ? PBMechanicalVentilator.DriverWaveform_Name(inspiration_waveform) : "Not Provided");
      foreach (SESubstanceFraction sf in this.fraction_inspired_gases)
        str += "\n\t" + sf.GetSubstance();
      foreach (SESubstanceConcentration sc in this.concentration_inspired_aerosols)
        str += "\n\t" + sc.GetSubstance();

      return str;
    }
  }
}