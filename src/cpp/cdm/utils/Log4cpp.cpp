/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/Logger.h"
#include "utils/FileUtils.h"
#include "properties/SEScalarTime.h"

#pragma warning(push, 0)
#pragma warning(disable : 4512) // assignment operator could not be generated
#pragma warning(disable : 4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>
#pragma warning(pop)

const std::string Loggable::empty("");

class log_lib
{
public:
  log_lib()
  {

  }
  virtual ~log_lib() 
  {
    delete FileAppender;
    delete ConsoleAppender;
  }
  log4cpp::Category* Log;
  log4cpp::Appender* FileAppender;
  log4cpp::Appender* ConsoleAppender;
};

void Logger::Initialize() {}
void Logger::Deinitialize() {}

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
  (b) ? _log_lib->Log->addAppender(*_log_lib->ConsoleAppender) : _log_lib->Log->removeAppender(_log_lib->ConsoleAppender);
}

void Logger::ResetLogFile(const std::string& logFilename)
{
  log4cpp::Category& category = log4cpp::Category::getInstance(logFilename);
  _log_lib->Log = &category;
  _log_lib->Log->removeAllAppenders();
  _log_lib->Log->setPriority(log4cpp::Priority::INFO);

  if (!logFilename.empty())
  {
    CreateFilePath(logFilename);
    // delete previous log contents if it exists
    FILE* FilePointer = fopen(logFilename.c_str(), "wt+");
    if (FilePointer)
      fclose(FilePointer);

    _log_lib->FileAppender = log4cpp::Appender::getAppender(logFilename);
    if (_log_lib->FileAppender == nullptr && !logFilename.empty()) {
      _log_lib->FileAppender = new log4cpp::FileAppender(logFilename, logFilename);
      log4cpp::PatternLayout* myLayout = new log4cpp::PatternLayout();
      myLayout->setConversionPattern("%d [%p] %m%n");
      _log_lib->FileAppender->setLayout(myLayout);
      _log_lib->Log->addAppender(_log_lib->FileAppender);
    }
  }

  _log_lib->ConsoleAppender = log4cpp::Appender::getAppender(logFilename + "_console");
  if (_log_lib->ConsoleAppender == nullptr) {
    _log_lib->ConsoleAppender = new log4cpp::OstreamAppender(logFilename + "_console", &std::cout);
    log4cpp::PatternLayout* cLayout = new log4cpp::PatternLayout();
    cLayout->setConversionPattern("%d [%p] %m%n");
    _log_lib->ConsoleAppender->setLayout(cLayout);
    _log_lib->ConsoleAppender->setThreshold(log4cpp::Priority::INFO);
  }
  LogToConsole(true);
}

Logger::~Logger()
{
  delete _log_lib;
}

void Logger::SetLogTime(const SEScalarTime* time)
{
  m_time = time;
}

//This function will change the priority of the logger
void Logger::SetLogLevel(Logger::level l)
{
  log4cpp::Priority::Value priority;
  switch (l)
  {
  case Logger::level::Trace:
    priority = log4cpp::Priority::NOTSET;
    break;
  case Logger::level::Debug:
    priority = log4cpp::Priority::DEBUG;
    break;
  case Logger::level::Info:
    priority = log4cpp::Priority::INFO;
    break;
  case Logger::level::Warn:
    priority = log4cpp::Priority::WARN;
    break;
  case Logger::level::Error:
    priority = log4cpp::Priority::ERROR;
    break;
  case Logger::level::Fatal:
    priority = log4cpp::Priority::FATAL;
    break;
  default:
    priority = log4cpp::Priority::INFO;
  }
  _log_lib->Log->setPriority(priority);
}

//This function will return the priority of the logger
Logger::level Logger::GetLogLevel()
{
  switch (_log_lib->Log->getPriority())
  {
  case log4cpp::Priority::NOTSET:
      return Logger::level::Trace;
  case log4cpp::Priority::DEBUG:
    return Logger::level::Debug;
  case log4cpp::Priority::INFO:
    return Logger::level::Info;
  case log4cpp::Priority::WARN:
    return Logger::level::Warn;
  case log4cpp::Priority::ERROR:
    return Logger::level::Error;
  case log4cpp::Priority::FATAL:
    return Logger::level::Fatal;
  }
  return Logger::level::Info;
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
  _log_lib->Log->debug(FormatLogMessage(msg,origin));
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
  _log_lib->Log->info(FormatLogMessage(msg, origin));
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
  _log_lib->Log->warn(FormatLogMessage(msg, origin));
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
  _log_lib->Log->error(FormatLogMessage(msg, origin));
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
  _log_lib->Log->fatal(FormatLogMessage(msg, origin));
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
