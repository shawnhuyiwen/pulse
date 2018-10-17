/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEEventHandler;
class SENutrition;
CDM_BIND_DECL(PatientData)

// Keep enums in sync with appropriate schema/cdm/PatientEnums.proto file !!
enum class ePatient_Sex { Male = 0, Female };
extern const std::string& ePatient_Sex_Name(ePatient_Sex m);

// Keep enums in sync with appropriate schema/cdm/PatientEnums.proto file !!
enum class ePatient_Event
{
  Antidiuresis = 0,
  Asystole = 1,
  Bradycardia = 2,
  Bradypnea = 3,
  BrainOxygenDeficit = 4,
  CardiacArrest = 5,
  CardiogenicShock = 6,
  CriticalBrainOxygenDeficit = 7,
  Dehydration = 8,
  Diuresis = 9,
  Fasciculation = 10,
  Fatigue = 11,
  FunctionalIncontinence = 12,
  Hypercapnia = 13,
  Hyperglycemia = 14,
  Hyperthermia = 15,
  Hypoglycemia = 16,
  Hypothermia = 17,
  Hypoxia = 18,
  HypovolemicShock = 19,
  IntracranialHypertension = 20,
  IntracranialHypotension = 21,
  IrreversibleState = 22,
  Ketoacidosis = 23,
  LacticAcidosis = 24,
  MaximumPulmonaryVentilationRate = 25,
  MetabolicAcidosis = 26,
  MetabolicAlkalosis = 27,
  MildAcuteRespiratoryDistress = 28,
  ModerateAcuteRespiratoryDistress = 29,
  MyocardiumOxygenDeficit = 30,
  Natriuresis = 31,
  NutritionDepleted = 32,
  PulselessRhythm = 33,
  RenalHypoperfusion = 34,
  RespiratoryAcidosis = 35,
  RespiratoryAlkalosis = 36,
  StartOfCardiacCycle = 37,
  StartOfExhale = 38,
  StartOfInhale = 39,
  SevereAcuteRespiratoryDistress = 40,
  Tachycardia = 41,
  Tachypnea = 42
};
extern const std::string& ePatient_Event_Name(ePatient_Event m);

class CDM_DECL SEPatient : public Loggable
{
public:

  SEPatient(Logger* logger);
  virtual ~SEPatient();

  virtual void Clear();

  bool Load(const std::string& str);
  bool LoadFile(const std::string& patientFile);
  void SaveFile(const std::string& filename);

  static void Load(const cdm::PatientData& src, SEPatient& dst);
  static cdm::PatientData* Unload(const SEPatient& src);
protected:
  static void Serialize(const cdm::PatientData& src, SEPatient& dst);
  static void Serialize(const SEPatient& src, cdm::PatientData& dst);

public:
  /** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual const std::map<ePatient_Event, bool>& GetEventStates() const { return m_EventState; }
  virtual void SetEvent(ePatient_Event type, bool active, const SEScalarTime& time);
  virtual bool IsEventActive(ePatient_Event state) const;
  virtual double GetEventDuration(ePatient_Event type, const TimeUnit& unit) const;
  virtual void UpdateEvents(const SEScalarTime& timeStep);
  /** @name ForwardEvents
   *  @brief - Set a callback class to invoke when any event changes
   *  @details - Note that the handler callback can and will be called in the middle of a time step
   *             So system and compartment objects may not be completely up to date when called.
   *             Use the PhysiologyEngineInterface::SetEventHandler to ensure that all engine 
   *             data is up to date at the time the callback is invoked
   */
  virtual void ForwardEvents(SEEventHandler* handler) const;
  
  virtual std::string GetName() const;
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual ePatient_Sex GetSex() const;
  virtual void SetSex(ePatient_Sex sex);
  
  virtual bool HasAge() const;
  virtual SEScalarTime& GetAge();
  virtual double GetAge(const TimeUnit& unit) const;
  
  virtual bool HasWeight() const;
  virtual SEScalarMass& GetWeight();
  virtual double GetWeight(const MassUnit& unit) const;
  
  virtual bool HasHeight() const;
  virtual SEScalarLength& GetHeight();
  virtual double GetHeight(const LengthUnit& unit) const;

  virtual bool HasAlveoliSurfaceArea() const;
  virtual SEScalarArea& GetAlveoliSurfaceArea();
  virtual double GetAlveoliSurfaceArea(const AreaUnit& unit) const;
  
  virtual bool HasBasalMetabolicRate() const;
  virtual SEScalarPower& GetBasalMetabolicRate();
  virtual double GetBasalMetabolicRate(const PowerUnit& unit) const;

  virtual bool HasBloodVolumeBaseline() const;
  virtual SEScalarVolume& GetBloodVolumeBaseline();
  virtual double GetBloodVolumeBaseline(const VolumeUnit& unit) const;

  virtual bool HasBodyDensity() const;
  virtual SEScalarMassPerVolume& GetBodyDensity();
  virtual double GetBodyDensity(const MassPerVolumeUnit& unit) const;

  virtual bool HasBodyFatFraction() const;
  virtual SEScalar0To1& GetBodyFatFraction();
  virtual double GetBodyFatFraction() const;

  virtual bool HasDiastolicArterialPressureBaseline() const;
  virtual SEScalarPressure& GetDiastolicArterialPressureBaseline();
  virtual double GetDiastolicArterialPressureBaseline(const PressureUnit& unit) const;

  virtual bool HasExpiratoryReserveVolume() const;
  virtual SEScalarVolume& GetExpiratoryReserveVolume();
  virtual double GetExpiratoryReserveVolume(const VolumeUnit& unit) const;
    
  virtual bool HasFunctionalResidualCapacity() const;
  virtual SEScalarVolume& GetFunctionalResidualCapacity();
  virtual double GetFunctionalResidualCapacity(const VolumeUnit& unit) const;

  virtual bool HasHeartRateBaseline() const;
  virtual SEScalarFrequency& GetHeartRateBaseline();
  virtual double GetHeartRateBaseline(const FrequencyUnit& unit) const;

  virtual bool HasHeartRateMaximum() const;
  virtual SEScalarFrequency& GetHeartRateMaximum();
  virtual double GetHeartRateMaximum(const FrequencyUnit& unit) const;

  virtual bool HasHeartRateMinimum() const;
  virtual SEScalarFrequency& GetHeartRateMinimum();
  virtual double GetHeartRateMinimum(const FrequencyUnit& unit) const;
  
  virtual bool HasInspiratoryCapacity() const;
  virtual SEScalarVolume& GetInspiratoryCapacity();
  virtual double GetInspiratoryCapacity(const VolumeUnit& unit) const;

  virtual bool HasInspiratoryReserveVolume() const;
  virtual SEScalarVolume& GetInspiratoryReserveVolume();
  virtual double GetInspiratoryReserveVolume(const VolumeUnit& unit) const;

  virtual bool HasLeanBodyMass() const;
  virtual SEScalarMass& GetLeanBodyMass();
  virtual double GetLeanBodyMass(const MassUnit& unit) const;

  virtual bool HasMeanArterialPressureBaseline() const;
  virtual SEScalarPressure& GetMeanArterialPressureBaseline();
  virtual double GetMeanArterialPressureBaseline(const PressureUnit& unit) const;    

  virtual bool HasResidualVolume() const;
  virtual SEScalarVolume& GetResidualVolume();
  virtual double GetResidualVolume(const VolumeUnit& unit) const;
  
  virtual bool HasRespirationRateBaseline() const;
  virtual SEScalarFrequency& GetRespirationRateBaseline();
  virtual double GetRespirationRateBaseline(const FrequencyUnit& unit) const;
  
  virtual bool HasRightLungRatio() const;
  virtual SEScalar0To1& GetRightLungRatio();
  virtual double GetRightLungRatio() const;

  virtual bool HasSkinSurfaceArea() const;
  virtual SEScalarArea& GetSkinSurfaceArea();
  virtual double GetSkinSurfaceArea(const AreaUnit& unit) const;
  
  virtual bool HasSystolicArterialPressureBaseline() const;
  virtual SEScalarPressure& GetSystolicArterialPressureBaseline();
  virtual double GetSystolicArterialPressureBaseline(const PressureUnit& unit) const;

  virtual bool HasTidalVolumeBaseline() const;
  virtual SEScalarVolume& GetTidalVolumeBaseline();
  virtual double GetTidalVolumeBaseline(const VolumeUnit& unit) const;
  
  virtual bool HasTotalLungCapacity() const;
  virtual SEScalarVolume& GetTotalLungCapacity();
  virtual double GetTotalLungCapacity(const VolumeUnit& unit) const;

  virtual bool HasVitalCapacity() const;
  virtual SEScalarVolume& GetVitalCapacity();
  virtual double GetVitalCapacity(const VolumeUnit& unit) const;

protected:

  std::stringstream                m_ss;
  mutable SEEventHandler*          m_EventHandler;
  std::map<ePatient_Event, bool>   m_EventState;
  std::map<ePatient_Event, double> m_EventDuration_s;

  std::string                m_Name;
  ePatient_Sex               m_Sex;
  SEScalarTime*              m_Age;
  SEScalarMass*              m_Weight;
  SEScalarLength*            m_Height;
  SEScalarMassPerVolume*     m_BodyDensity;
  SEScalar0To1*              m_BodyFatFraction;
  SEScalarMass*              m_LeanBodyMass;

  SEScalarArea*              m_AlveoliSurfaceArea;
  SEScalar0To1*              m_RightLungRatio;
  SEScalarArea*              m_SkinSurfaceArea;

  SEScalarPower*             m_BasalMetabolicRate;
  SEScalarVolume*            m_BloodVolumeBaseline;
  SEScalarPressure*          m_DiastolicArterialPressureBaseline;
  SEScalarFrequency*         m_HeartRateBaseline;
  SEScalarPressure*          m_MeanArterialPressureBaseline;
  SEScalarFrequency*         m_RespirationRateBaseline;
  SEScalarPressure*          m_SystolicArterialPressureBaseline;
  SEScalarVolume*            m_TidalVolumeBaseline;

  SEScalarFrequency*         m_HeartRateMaximum;
  SEScalarFrequency*         m_HeartRateMinimum;
  SEScalarVolume*            m_ExpiratoryReserveVolume;
  SEScalarVolume*            m_FunctionalResidualCapacity;
  SEScalarVolume*            m_InspiratoryCapacity;
  SEScalarVolume*            m_InspiratoryReserveVolume;
  SEScalarVolume*            m_ResidualVolume;
  SEScalarVolume*            m_TotalLungCapacity;
  SEScalarVolume*            m_VitalCapacity;
};