/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class CDM_DECL SERespiratoryMechanics : public Loggable
{
  friend class PBPhysiology;
public:

  SERespiratoryMechanics(Logger* logger);
  virtual ~SERespiratoryMechanics();

  virtual void Clear();// Deletes all members

  bool HasLeftComplianceCurve() const;
  SECurve& GetLeftComplianceCurve();
  const SECurve* GetLeftComplianceCurve() const;

  bool HasRightComplianceCurve() const;
  SECurve& GetRightComplianceCurve();
  const SECurve* GetRightComplianceCurve() const;

  /** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool HasLeftExpiratoryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetLeftExpiratoryResistance();
  virtual double GetLeftExpiratoryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasLeftInspiratoryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetLeftInspiratoryResistance();
  virtual double GetLeftInspiratoryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasRightExpiratoryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetRightExpiratoryResistance();
  virtual double GetRightExpiratoryResistance(const PressureTimePerVolumeUnit& unit) const;;

  virtual bool HasRightInspiratoryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetRightInspiratoryResistance();
  virtual double GetRightInspiratoryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasUpperExpiratoryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetUpperExpiratoryResistance();
  virtual double GetUpperExpiratoryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasUpperInspiratoryResistance() const;
  virtual SEScalarPressureTimePerVolume& GetUpperInspiratoryResistance();
  virtual double GetUpperInspiratoryResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasInspiratoryPeakPressure() const;
  virtual SEScalarPressure& GetInspiratoryPeakPressure();
  virtual double GetInspiratoryPeakPressure(const PressureUnit& unit) const;

  virtual bool HasExpiratoryPeakPressure() const;
  virtual SEScalarPressure& GetExpiratoryPeakPressure();
  virtual double GetExpiratoryPeakPressure(const PressureUnit& unit) const;

  virtual bool HasInspiratoryRiseTime() const;
  virtual SEScalarTime& GetInspiratoryRiseTime();
  virtual double GetInspiratoryRiseTime(const TimeUnit& unit) const;

  virtual bool HasInspiratoryHoldTime() const;
  virtual SEScalarTime& GetInspiratoryHoldTime();
  virtual double GetInspiratoryHoldTime(const TimeUnit& unit) const;

  virtual bool HasInspiratoryReleaseTime() const;
  virtual SEScalarTime& GetInspiratoryReleaseTime();
  virtual double GetInspiratoryReleaseTime(const TimeUnit& unit) const;

  virtual bool HasInspiratoryToExpiratoryPauseTime() const;
  virtual SEScalarTime& GetInspiratoryToExpiratoryPauseTime();
  virtual double GetInspiratoryToExpiratoryPauseTime(const TimeUnit& unit) const;

  virtual bool HasExpiratoryRiseTime() const;
  virtual SEScalarTime& GetExpiratoryRiseTime();
  virtual double GetExpiratoryRiseTime(const TimeUnit& unit) const;

  virtual bool HasExpiratoryHoldTime() const;
  virtual SEScalarTime& GetExpiratoryHoldTime();
  virtual double GetExpiratoryHoldTime(const TimeUnit& unit) const;

  virtual bool HasExpiratoryReleaseTime() const;
  virtual SEScalarTime& GetExpiratoryReleaseTime();
  virtual double GetExpiratoryReleaseTime(const TimeUnit& unit) const;

  virtual bool HasResidueTime() const;
  virtual SEScalarTime& GetResidueTime();
  virtual double GetResidueTime(const TimeUnit& unit) const;

  virtual std::string ToString() const;
  virtual void ToString(std::ostream& str) const;

protected:

  SECurve*                       m_LeftComplianceCurve;
  SECurve*                       m_RightComplianceCurve;

  SEScalarPressureTimePerVolume* m_LeftExpiratoryResistance;
  SEScalarPressureTimePerVolume* m_LeftInspiratoryResistance;
  SEScalarPressureTimePerVolume* m_RightExpiratoryResistance;
  SEScalarPressureTimePerVolume* m_RightInspiratoryResistance;
  SEScalarPressureTimePerVolume* m_UpperExpiratoryResistance;
  SEScalarPressureTimePerVolume* m_UpperInspiratoryResistance;
  SEScalarPressure*              m_InspiratoryPeakPressure;
  SEScalarPressure*              m_ExpiratoryPeakPressure;
  SEScalarTime*                  m_InspiratoryRiseTime;
  SEScalarTime*                  m_InspiratoryHoldTime;
  SEScalarTime*                  m_InspiratoryReleaseTime;
  SEScalarTime*                  m_InspiratoryToExpiratoryPauseTime;
  SEScalarTime*                  m_ExpiratoryRiseTime;
  SEScalarTime*                  m_ExpiratoryHoldTime;
  SEScalarTime*                  m_ExpiratoryReleaseTime;
  SEScalarTime*                  m_ResidueTime;
};

inline std::ostream& operator<< (std::ostream& out, const SERespiratoryMechanics* s)
{
  if (s == nullptr)
    out << "Null Respiratory Mechanics" << std::flush;
  else
    (*s).ToString(out);
  return out;
}
inline std::ostream& operator<< (std::ostream& out, const SERespiratoryMechanics& s)
{
  s.ToString(out);
  return out;
}
