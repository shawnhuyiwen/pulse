/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/Logger.h"
#include "utils/FileUtils.h"

//#pragma  warning(push)
//#pragma warning(disable:4512) // assignment operator could not be generated
//#pragma warning(disable:4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/configurator.h"
#include "log4cplus/initializer.h"
#include "log4cplus/helpers/pointer.h"
#include "log4cplus/appender.h"
#include "log4cplus/consoleappender.h"
#include "log4cplus/fileappender.h"
//#pragma  warning(pop)

#include <iomanip>

#include "properties/SEScalarTime.h"

const std::string Loggable::empty("");

class log_lib
{
public:
  log_lib() : logger(log4cplus::Logger::getRoot())
  {

  }
  virtual ~log_lib() { }
  log4cplus::Initializer        initializer;
  log4cplus::Logger             logger;
  log4cplus::SharedAppenderPtr  fileAppender;
  log4cplus::SharedAppenderPtr  consoleAppender;
};

//logger constructor
Logger::Logger(const std::string& logFilename) 
{
  m_Forward = nullptr;
  m_time = nullptr;
  _log_lib = new log_lib();
  ResetLogFile(logFilename);
}

void Logger::LogToConsole(bool b)
{
  (b) ? _log_lib->logger.addAppender(_log_lib->consoleAppender) : _log_lib->logger.removeAppender(_log_lib->consoleAppender);
}

void Logger::ResetLogFile(const std::string& logFilename)
{
  if (_log_lib->logger.getAllAppenders().empty())
    _log_lib->logger.removeAllAppenders();
  _log_lib->logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);

  if (!logFilename.empty())
  {
    CreateFilePath(logFilename);

    //delete previous log contents if it exists
    FILE* FilePointer = fopen(logFilename.c_str(), "wt+");
    if (FilePointer)
      fclose(FilePointer);

    _log_lib->fileAppender = _log_lib->logger.getAppender(logFilename);
    if (_log_lib->fileAppender == nullptr)
    {
      _log_lib->fileAppender = new log4cplus::FileAppender(logFilename);
      _log_lib->fileAppender->setName(logFilename);
      _log_lib->fileAppender->setLayout(std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout("%d [%p] %m%n")));
      _log_lib->logger.addAppender(_log_lib->fileAppender);
    }
  }

  _log_lib->consoleAppender = _log_lib->logger.getAppender(logFilename+"_console");
  if (_log_lib->consoleAppender == nullptr)
  {
    _log_lib->consoleAppender = new log4cplus::ConsoleAppender();
    _log_lib->consoleAppender->setName(logFilename + "_console");
    _log_lib->consoleAppender->setLayout(std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout("%d [%p] %m%n")));
  }
  LogToConsole(true);
}

Logger::~Logger()
{
 
}

void Logger::SetLogTime(const SEScalarTime* time)
{
  m_time = time;
}

// TODO SetGlobalPriority(log4cpp::Priority::Value priority)
// TODO log4cpp::Priority::Value priority GetGlobalPriority()

//This function will change the priority of the logger
void Logger::SetLogLevel(Logger::level l)
{
  log4cplus::LogLevel L;
  switch (l)
  {
  case TRACE:
    L = log4cplus::TRACE_LOG_LEVEL;
    break;
  case DEBUG:
    L = log4cplus::DEBUG_LOG_LEVEL;
    break;
  case INFO:
    L = log4cplus::INFO_LOG_LEVEL;
    break;
  case WARN:
    L = log4cplus::WARN_LOG_LEVEL;
    break;
  case ERROR:
    L = log4cplus::ERROR_LOG_LEVEL;
    break;
  case FATAL:
    L = log4cplus::FATAL_LOG_LEVEL;
    break;
  default:
    L = log4cplus::INFO_LOG_LEVEL;
  }
  _log_lib->logger.setLogLevel(L);
}

//This function will return the priority of the logger
Logger::level Logger::GetLogLevel()
{
  switch (_log_lib->logger.getLogLevel())
  {
  case log4cplus::TRACE_LOG_LEVEL:
      return TRACE;
  case log4cplus::DEBUG_LOG_LEVEL:
    return DEBUG;
  case log4cplus::INFO_LOG_LEVEL:
    return INFO;
  case log4cplus::WARN_LOG_LEVEL:
    return WARN;
  case log4cplus::ERROR_LOG_LEVEL:
    return ERROR;
  case log4cplus::FATAL_LOG_LEVEL:
    return FATAL;
  }
  return INFO;
}

void Logger::SetForward(LoggerForward* forward)
{
  m_Forward = forward;
}

bool Logger::HasForward()
{
  return m_Forward == nullptr ? false : true;
}

std::string Logger::FormatLogMessage(const std::string&  msg, const std::string&  origin)
{
  m_ss.str("");
  m_ss.clear();
  if (m_time != nullptr && m_time->IsValid())
    m_ss << "[" << *m_time << "] " << msg;
  else
    m_ss << msg;
  if (msg.empty())
    return origin;
  return origin + " : " + m_ss.str();
}


void Logger::Debug(std::string const&  msg, const std::string&  origin)
{
  LOG4CPLUS_DEBUG(_log_lib->logger, FormatLogMessage(msg,origin));
  if (m_Forward != nullptr)
    m_Forward->ForwardDebug(m_ss.str().c_str(), origin.c_str());
}

void Logger::Debug(std::stringstream &msg, const std::string&  origin)
{
  Debug(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Debug(std::ostream &msg, const std::string&  origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}

void Logger::Info(const std::string&  msg, const std::string&  origin)
{
  LOG4CPLUS_INFO(_log_lib->logger, FormatLogMessage(msg, origin));
  if (m_Forward != nullptr)
    m_Forward->ForwardInfo(m_ss.str().c_str(), origin.c_str());
}

void Logger::Info(std::stringstream &msg, const std::string&  origin)
{
  Info(msg.str(), origin);
  msg.str("");
  msg.clear();
}

void Logger::Info(const std::stringstream &msg, const std::string&  origin)
{
  Info(msg.str(), origin);
}

void Logger::Info(std::ostream &msg, const std::string&  origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Logger::Warning(const std::string&  msg, const std::string&  origin)
{
  LOG4CPLUS_WARN(_log_lib->logger, FormatLogMessage(msg, origin));
  if (m_Forward != nullptr)
    m_Forward->ForwardWarning(m_ss.str().c_str(), origin.c_str());
}
void Logger::Warning(std::stringstream &msg, const std::string&  origin)
{
  Warning(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Warning(std::ostream &msg, const std::string&  origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}

void Logger::Error(const std::string&  msg, const std::string&  origin)
{
  LOG4CPLUS_ERROR(_log_lib->logger, FormatLogMessage(msg, origin));
  if (m_Forward != nullptr)
    m_Forward->ForwardError(m_ss.str().c_str(), origin.c_str());
}
void Logger::Error(std::stringstream &msg, const std::string&  origin)
{
  Error(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Error(std::ostream &msg, const std::string&  origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Logger::Fatal(const std::string&  msg, const std::string&  origin)
{
  LOG4CPLUS_FATAL(_log_lib->logger, FormatLogMessage(msg, origin));
  if (m_Forward != nullptr)
    m_Forward->ForwardFatal(m_ss.str().c_str(), origin.c_str());
}
void Logger::Fatal(std::stringstream &msg, const std::string&  origin)
{
  Fatal(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Fatal(std::ostream &msg, const std::string&  origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

Loggable::Loggable()
{
  m_Logger = nullptr;
}

Loggable::Loggable(Logger* logger)
{
  m_Logger = logger;
}

Loggable::~Loggable()
{

}

Logger* Loggable::GetLogger() const
{
  return m_Logger;
}

void Loggable::Error(const std::string&  msg, const std::string&  origin) const
{
  if (m_Logger)
    m_Logger->Error(msg, origin);
  else// if(stdOut) TODO support
    std::cerr << "ERROR:" << msg << " : " << origin << std::endl;
}
void Loggable::Error(std::stringstream &msg, const std::string&  origin) const
{
  Error(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Error(std::ostream &msg, const std::string&  origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Loggable::Info(const std::string&  msg, const std::string&  origin) const
{
  if (m_Logger)
    m_Logger->Info(msg, origin);
  else //if(stdOut) TODO support
    std::cout << "INFO:" << msg << " : " << origin << std::endl;
}
void Loggable::Info(std::stringstream &msg, const std::string&  origin) const
{
  Info(msg.str(), origin);
  msg.str("");
  msg.clear();
}

void Loggable::Info(const std::stringstream &msg, const std::string&  origin) const
{
  Info(msg.str(), origin);
}

void Loggable::Info(std::ostream &msg, const std::string&  origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Loggable::Warning(const std::string&  msg, const std::string&  origin) const
{
  if (m_Logger)
    m_Logger->Warning(msg, origin);
  else// if(stdOut) TODO support
    std::cout << "WARN:" << msg << " : " << origin << std::endl;
}
void Loggable::Warning(std::stringstream &msg, const std::string&  origin) const
{
  Warning(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Warning(std::ostream &msg, const std::string&  origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}

void Loggable::Fatal(const std::string&  msg, const std::string&  origin) const
{
  std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
  if (m_Logger)
    m_Logger->Fatal(msg, origin);
  else// if(stdOut) TODO support
    std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
}
void Loggable::Fatal(std::stringstream &msg, const std::string&  origin) const
{
  Fatal(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Fatal(std::ostream &msg, const std::string&  origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

void Loggable::Debug(const std::string&  msg, const std::string&  origin) const
{
  if (m_Logger)
    m_Logger->Debug(msg, origin);
  // Not writing out DEBUG to console, only to log
  //else// if(stdOut) TODO support
  //  std::cout<<"DEBUG:"<<msg<<" : "<<origin<<std::endl;
}
void Loggable::Debug(std::stringstream &msg, const std::string&  origin) const
{
  Debug(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Debug(std::ostream &msg, const std::string&  origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}
