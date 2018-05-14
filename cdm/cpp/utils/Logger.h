/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#pragma  warning(push)
#pragma warning(disable:4512) // assignment operator could not be generated
#pragma warning(disable:4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/configurator.h"
#include "log4cplus/helpers/pointer.h"
#include "log4cplus/appender.h"
#pragma  warning(pop)

class Logger;
class SEScalarTime;
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

  void SetLogLevel(log4cplus::LogLevel  level);
  log4cplus::LogLevel GetLogLevel();

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
  log4cplus::Logger             m_Log;
  log4cplus::SharedAppenderPtr  m_FileAppender;
  log4cplus::SharedAppenderPtr  m_ConsoleAppender;
  const SEScalarTime*           m_time;
  std::stringstream             m_ss;
};

