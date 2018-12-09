/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/



public class SEAnesthesiaMachine // : SESystem
{
  // Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
  public enum Connection : int
  {
    NullConnection = 0,/** Signals not provided, or no change */
    Off,
    Mask,
    Tube
  }
  extern string Connection_Name(Connection c);
  // Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
  public enum OxygenSource : int
  {
    NullSource = 0,/** Signals not provided, or no change */
    NoSource,
    Wall,
    BottleOne,
    BottleTwo
  }
  extern string OxygenSource_Name(OxygenSource os);
  // Keep enums in sync with appropriate schema/cdm/AnesthesiaMachineEnums.proto file !!
  public enum PrimaryGas : int
  {
    NullGas = 0,/** Signals not provided, or no change */
    NoGas,
    Air,
    Nitrogen
  }
  extern string PrimaryGas_Name(PrimaryGas pg);

  protected Connection                        connection;
  protected SEScalarVolumePerTime             inletFlow;
  protected SEScalar                          inspiratoryExpiratoryRatio;
  protected SEScalar0To1                      oxygenFraction;
  protected OxygenSource                      oxygenSource;
  protected SEScalarPressure                  positiveEndExpiredPressure;
  protected PrimaryGas                        primaryGas;
  protected SEScalarFrequency                 respiratoryRate;
  protected SEScalarPressure                  reliefValvePressure;

  protected SEScalarPressure                  ventilatorPressure;

  //protected SEAnesthesiaMachineChamber        leftChamber;
  //protected SEAnesthesiaMachineChamber        rightChamber;

  //protected SEAnesthesiaMachineOxygenBottle   oxygenBottleOne;
  //protected SEAnesthesiaMachineOxygenBottle   oxygenBottleTwo;

  //protected Map<Event,Boolean> events = new HashMap<Event,Boolean>();

  public SEAnesthesiaMachine()
  {
    connection = Connection.NullConnection;
    inletFlow = null;
    inspiratoryExpiratoryRatio = null;
    oxygenFraction = null;
    oxygenSource = OxygenSource.NullSource;
    positiveEndExpiredPressure = null;
    primaryGas = PrimaryGas.NullGas;
    respiratoryRate = null;
    reliefValvePressure = null;

    ventilatorPressure = null;

    //leftChamber = null;
    //rightChamber = null;
    
    //oxygenBottleOne = null;
    //oxygenBottleTwo = null;
  }

  public void Clear()
  {
    connection = Connection.NullConnection;
    if (inletFlow != null)
      inletFlow.Invalidate();
    if (inspiratoryExpiratoryRatio != null)
      inspiratoryExpiratoryRatio.Invalidate();
    if (oxygenFraction != null)
      oxygenFraction.Invalidate();
    oxygenSource = OxygenSource.NullSource;
    if (positiveEndExpiredPressure != null)
      positiveEndExpiredPressure.Invalidate();
    primaryGas = PrimaryGas.NullGas;
    if (respiratoryRate != null)
      respiratoryRate.Invalidate();
    if (reliefValvePressure != null)
      reliefValvePressure.Invalidate();
    if (ventilatorPressure != null)
      ventilatorPressure.Invalidate();

    //if (HasLeftChamber())
    //  leftChamber.Clear();
    //if (HasRightChamber())
    //  rightChamber.Clear();
    //if (HasOxygenBottleOne())
    //  oxygenBottleOne.Clear();
    //if (HasOxygenBottleTwo())
    //  oxygenBottleTwo.Clear();

    //events.clear();
  }

  //public void Copy(SEAnesthesiaMachine from)
  //{
  //  Clear();
  //  if(from.connection!=null && from.connection != Connection.NullConnection)
  //  	this.connection=from.connection;      
  //  if(from.HasInletFlow())
  //    this.GetInletFlow().Set(from.GetInletFlow());
  //  if(from.HasInspiratoryExpiratoryRatio())
  //    this.GetInspiratoryExpiratoryRatio().Set(from.GetInspiratoryExpiratoryRatio());
  //  if(from.HasOxygenFraction())
  //    this.GetOxygenFraction().Set(from.GetOxygenFraction());
  //  if(from.oxygenSource!=null && from.oxygenSource != OxygenSource.NullSource)
  //  	this.setOxygenSource(from.GetOxygenSource());
  //  if(from.HasPositiveEndExpiredPressure())
  //    this.GetPositiveEndExpiredPressure().Set(from.GetPositiveEndExpiredPressure());
  //  if(from.primaryGas!=null && from.primaryGas != PrimaryGas.NullGas)
  //  	this.setPrimaryGas(from.GetPrimaryGas());
  //  if(from.HasReliefValvePressure())
  //    this.GetReliefValvePressure().Set(from.GetReliefValvePressure());
  //  if(from.HasRespiratoryRate())
  //    this.GetRespiratoryRate().Set(from.GetRespiratoryRate());
  //  if(from.HasVentilatorPressure())
  //    this.GetVentilatorPressure().Set(from.GetVentilatorPressure());    

  //  if(from.HasLeftChamber())    
  //    GetLeftChamber().copy(from.GetLeftChamber());
  //  if(from.HasRightChamber())
  //    GetRightChamber().copy(from.GetRightChamber());
  //  if(from.HasOxygenBottleOne())
  //    GetOxygenBottleOne().copy(from.GetOxygenBottleOne());
  //  if(from.HasOxygenBottleTwo())
  //    GetOxygenBottleTwo().copy(from.GetOxygenBottleTwo());

  //  events.putAll(from.events);
  //}

  //public void SetEvent(Event type, bool active)
  //{
  //  this.events.put(type, active);
  //}
  //public bool IsEventActive(Event type)
  //{
  //  if(!this.events.containsKey(type))
  //    return false;
  //  return this.events.Get(type);
  //}
  
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

  public SEScalarVolumePerTime GetInletFlow()
  {
    if (inletFlow == null)
      inletFlow = new SEScalarVolumePerTime();
    return inletFlow;
  }
  public bool HasInletFlow()
  {
    return inletFlow == null ? false : inletFlow.IsValid();
  }

  public SEScalar0To1 GetOxygenFraction()
  {
    if (oxygenFraction == null)
      oxygenFraction = new SEScalar0To1();
    return oxygenFraction;
  }
  public bool HasOxygenFraction()
  {
    return oxygenFraction == null ? false : oxygenFraction.IsValid();
  }

  public OxygenSource GetOxygenSource()
  {
    return oxygenSource;
  }
  public void SetOxygenSource(OxygenSource os)
  {
  	oxygenSource = os;
  }
  public bool HasOxygenSource()
  {
    return oxygenSource != OxygenSource.NullSource;
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

  public PrimaryGas GetPrimaryGas()
  {
    return primaryGas;
  }
  public void SetPrimaryGas(PrimaryGas pg)
  {
  	primaryGas = pg;
  }
  public bool HasPrimaryGas()
  {
    return primaryGas != PrimaryGas.NullGas;
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

  public SEScalarPressure GetReliefValvePressure()
  {
    if (reliefValvePressure == null)
      reliefValvePressure = new SEScalarPressure();
    return reliefValvePressure;
  }
  public bool HasReliefValvePressure()
  {
    return reliefValvePressure == null ? false : reliefValvePressure.IsValid();
  }
  
  public SEScalarPressure GetVentilatorPressure()
  {
    if (ventilatorPressure == null)
      ventilatorPressure = new SEScalarPressure();
    return ventilatorPressure;
  }
  public bool HasVentilatorPressure()
  {
    return ventilatorPressure == null ? false : ventilatorPressure.IsValid();
  }


  //public bool HasLeftChamber()
  //{
  //  return leftChamber == null ? false : true;
  //}
  //public SEAnesthesiaMachineChamber GetLeftChamber()
  //{
  //  if(leftChamber==null)
  //    this.leftChamber=new SEAnesthesiaMachineChamber();
  //  return leftChamber;
  //}
  //public void RemoveLeftChamber()
  //{
  //  leftChamber = null;
  //}

  //public bool HasRightChamber()
  //{
  //  return rightChamber == null ? false : true;
  //}
  //public SEAnesthesiaMachineChamber GetRightChamber()
  //{
  //  if(rightChamber==null)
  //    this.rightChamber=new SEAnesthesiaMachineChamber();
  //  return rightChamber;
  //}
  //public void RemoveRightChamber()
  //{
  //  rightChamber = null;
  //}

  //public SEAnesthesiaMachineOxygenBottle GetOxygenBottleOne()
  //{
  //  if (oxygenBottleOne == null)
  //    oxygenBottleOne = new SEAnesthesiaMachineOxygenBottle();
  //  return oxygenBottleOne;
  //}
  //public bool HasOxygenBottleOne()
  //{
  //  return oxygenBottleOne == null ? false : true;
  //}

  //public SEAnesthesiaMachineOxygenBottle GetOxygenBottleTwo()
  //{
  //  if (oxygenBottleTwo == null)
  //    oxygenBottleTwo = new SEAnesthesiaMachineOxygenBottle();

  //  return oxygenBottleTwo;
  //}
  //public bool HasOxygenBottleTwo()
  //{
  //  return oxygenBottleTwo == null ? false : true;
  //}
  
  public string ToString()
  {
    string leftChamber = "Left Chamber: NotProvided";
    //if(HasLeftChamber())
    //{
    //  leftChamber = GetLeftChamber().ToString();
    //  leftChamber = leftChamber.ReplaceAll("Chamber", "Left Chamber");
    //}
    string rightChamber = "Right Chamber: NotProvided";
    //if(HasRightChamber())
    //{
    //  rightChamber = GetRightChamber().ToString();
    //  rightChamber = rightChamber.ReplaceAll("Chamber", "Right Chamber");
    //}
    string o2BottleOne = "Oxygen Bottle One: NotProvided";
    //if(HasOxygenBottleOne())
    //{
    //  o2BottleOne = GetOxygenBottleOne().ToString();
    //  o2BottleOne = o2BottleOne.ReplaceAll("Bottle", "Bottle One");
    //}
    string o2BottleTwo = "Oxygen Bottle Two: NotProvided";
    //if(HasOxygenBottleTwo())
    //{
    //  o2BottleTwo = GetOxygenBottleTwo().ToString();
    //  o2BottleTwo = o2BottleTwo.ReplaceAll("Bottle", "Bottle Two");
    //}
    
      return "Anesthesia Machine"
      + "\n\tConnection: " + (HasConnection()? Connection_Name(GetConnection()):"NotProvided")
      + "\n\tInletFlow: " + GetInletFlow()
      + "\n\tInspiratoryExpiratoryRatio: " + GetInspiratoryExpiratoryRatio()
      + "\n\tOxygenFraction: " + GetOxygenFraction()
      + "\n\tOxygenSource: " + (HasOxygenSource()? OxygenSource_Name(GetOxygenSource()):"NotProvided")
      + "\n\tPositiveEndExpiredPressure: " + GetPositiveEndExpiredPressure()
      + "\n\tPrimaryGas: " + (HasPrimaryGas()? PrimaryGas_Name(GetPrimaryGas()):"NotProvided")
      + "\n\tRespiratoryRate: " + GetRespiratoryRate()
      + "\n\tReliefValvePressure: " + GetReliefValvePressure()
      + "\n\tVentilatorPressure: " + GetVentilatorPressure()
      + "\n\t" + leftChamber
      + "\n\t" + rightChamber
      + "\n\t" + o2BottleOne
      + "\n\t" + o2BottleTwo;
  }
}