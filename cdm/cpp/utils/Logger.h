/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

class Logger;
class SEScalarTime;
class log_lib; // Encapsulates 3rd party logging library
#include <sstream>

class CDM_DECL Loggable
{
public:
  static const std::string empty;

  Loggable();
  Loggable(Logger* log);
  virtual ~Loggable();

  virtual Logger* GetLogger() const;

  virtual void Debug(std::string const&  msg, std::string const&  origin = empty) const;
  virtual void Debug(std::stringstream &msg, std::string const&  origin = empty) const;
  virtual void Debug(std::ostream &msg, std::string const&  origin = empty) const;

  virtual void Info(std::string const&  msg, std::string const&  origin = empty) const;
  virtual void Info(std::stringstream &msg, std::string const&  origin = empty) const;
  virtual void Info(const std::stringstream &msg, std::string const&  origin = empty) const;
  virtual void Info(std::ostream &msg, std::string const&  origin = empty) const;

  virtual void Warning(std::string const&  msg, std::string const&  origin = empty) const;
  virtual void Warning(std::stringstream &msg, std::string const&  origin = empty) const;
  virtual void Warning(std::ostream &msg, std::string const&  origin = empty) const;

  virtual void Error(std::string const&  msg, std::string const&  origin = empty) const;
  virtual void Error(std::stringstream &msg, std::string const&  origin = empty) const;
  virtual void Error(std::ostream &msg, std::string const&  origin = empty) const;

  virtual void Fatal(std::string const&  msg, std::string const&  origin = empty) const;
  virtual void Fatal(std::stringstream &msg, std::string const&  origin = empty) const;
  virtual void Fatal(std::ostream &msg, std::string const&  origin = empty) const;

protected:
  Logger* m_Logger;
};

#pragma warning(push)
#pragma warning(disable:4100)
class CDM_DECL LoggerForward
{
public:
  virtual void ForwardDebug(std::string const&  msg, std::string const&  origin){};
  virtual void ForwardInfo(std::string const&  msg, std::string const&  origin){};
  virtual void ForwardWarning(std::string const&  msg, std::string const&  origin){};
  virtual void ForwardError(std::string const&  msg, std::string const&  origin){};
  virtual void ForwardFatal(std::string const&  msg, std::string const&  origin){};
};

#pragma warning(pop)

class CDM_DECL Logger
{
  friend Loggable;
public:
  Logger(std::string const&  logFilename = Loggable::empty);
  virtual ~Logger();

  void LogToConsole(bool b);

  void ResetLogFile(std::string const&  logFilename = Loggable::empty);

  enum class level
  {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
  };
  void SetLogLevel(level  level);
  level GetLogLevel();

  virtual void SetLogTime(const SEScalarTime* time);

  virtual void SetForward(LoggerForward* forward);
  virtual bool HasForward();

  virtual void Debug(std::string const&  msg, std::string const&  origin = Loggable::empty);
  virtual void Debug(std::stringstream &msg, std::string const&  origin = Loggable::empty);
  virtual void Debug(std::ostream &msg, std::string const&  origin = Loggable::empty);

  virtual void Info(std::string const&  msg, std::string const&  origin = Loggable::empty);
  virtual void Info(std::stringstream &msg, std::string const&  origin = Loggable::empty);
  virtual void Info(const std::stringstream &msg, std::string const&  origin = Loggable::empty);
  virtual void Info(std::ostream &msg, std::string const&  origin = Loggable::empty);

  virtual void Warning(std::string const&  msg, std::string const&  origin = Loggable::empty);
  virtual void Warning(std::stringstream &msg, std::string const&  origin = Loggable::empty);
  virtual void Warning(std::ostream &msg, std::string const&  origin = Loggable::empty);

  virtual void Error(std::string const&  msg, std::string const&  origin = Loggable::empty);
  virtual void Error(std::stringstream &msg, std::string const&  origin = Loggable::empty);
  virtual void Error(std::ostream &msg, std::string const&  origin = Loggable::empty);

  virtual void Fatal(std::string const&  msg, std::string const&  origin = Loggable::empty);
  virtual void Fatal(std::stringstream &msg, std::string const&  origin = Loggable::empty);
  virtual void Fatal(std::ostream &msg, std::string const&  origin = Loggable::empty);

protected:

  virtual std::string FormatLogMessage(std::string const&  origin, std::string const&  msg);

  LoggerForward*                m_Forward;
  const SEScalarTime*           m_time;
  std::stringstream             m_ss;

private:
   log_lib*                     _log_lib;
};

