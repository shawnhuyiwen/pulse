/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

// Keep enums in sync with appropriate schema/cdm/Enums.proto file !!
namespace Pulse.CDM
{
  public enum eSerializationFormat : int   // State file serialization format
  {
    JSON = 0,
    BINARY
  }

  public enum eSide : int
  {
    NullSide = 0,
    Left,
    Right
  }

  public enum eGate : int
  {
    NullGate = 0,
    Open,
    Closed
  }

  public enum eSwitch : int
  {
    NullSwitch = 0,
    Off,
    On
  }

  public enum eCharge : int
  {
    NullCharge = 0,
    Negative,
    Neutral,
    Positive
  }

  public enum eDecimalFormatType : int
  {
    SystemFormatting = 0,
    DefaultFloat,
    FixedMantissa,
    SignificantDigits
  }

  public enum eBreathState : int
  {
    NoBreath = 0,
    PatientInitiatedInhale,
    PatientInitiatedPause,
    PatientInitiatedExhale,
    EquipmentInitiatedInhale,
    EquipmentInitiatedPause,
    EquipmentInitiatedExhale,
    ExpiratoryHold,
    InspiratoryHold
  }

  public enum eDefaultType : int
  {
    Model = 0,
    Zero
  }

  public enum eDriverWaveform : int
  {
    NullDriverWaveform = 0,/** Signals not provided, or no change */
    Square,
    AscendingRamp,
    DecendingRamp,
    ExponentialGrowth,
    ExponentialDecay,
    SinusoidalRise,
    SinusoidalFall,
    SigmoidalRise,
    SigmoidalFall
  }
}
