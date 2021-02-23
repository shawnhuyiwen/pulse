/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class CDM_DECL SEAutoSerialization : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  SEAutoSerialization(Logger* logger=nullptr);
  virtual ~SEAutoSerialization();

  virtual void Clear();
  virtual bool IsValid() const;

  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

  virtual eSwitch GetPeriodTimeStamps() const;
  virtual void SetPeriodTimeStamps(eSwitch v);

  virtual eSwitch GetAfterActions() const;
  virtual void SetAfterActions(eSwitch v);

  virtual eSwitch GetReloadState() const;
  virtual void SetReloadState(eSwitch v);

  virtual bool HasDirectory() const;
  virtual std::string GetDirectory() const;
  virtual void SetDirectory(const std::string& dir);
  virtual void InvalidateDirectory();

  virtual bool HasFileName() const;
  virtual std::string GetFileName() const;
  virtual void SetFileName(const std::string& dir);
  virtual void InvalidateFileName();


protected:

  SEScalarTime*    m_Period;
  eSwitch          m_PeriodTimeStamps;
  eSwitch          m_AfterActions;
  eSwitch          m_ReloadState;
  std::string      m_Directory;
  std::string      m_FileName;
};
