/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/Logger.h"
#include "utils/FileUtils.h"

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <iomanip>

#include "properties/SEScalarTime.h"

const std::string Loggable::empty("");

//logger constructor
Logger::Logger(const std::string& logFilename) : m_Log(log4cplus::Logger::getRoot())
{
  m_Forward = nullptr;
  m_time = nullptr;
  //log4cplus::initialize();
  ResetLogFile(logFilename);
}

void Logger::LogToConsole(bool b)
{
  (b) ? m_Log.addAppender(m_ConsoleAppender) : m_Log.removeAppender(m_ConsoleAppender);
}

void Logger::ResetLogFile(const std::string& logFilename)
{
  if (m_Log.getAllAppenders().empty())
    m_Log.removeAllAppenders();
  m_Log.setLogLevel(log4cplus::INFO_LOG_LEVEL);

  if (!logFilename.empty())
  {
    CreateFilePath(logFilename);

    //delete previous log contents if it exists
    FILE* FilePointer = fopen(logFilename.c_str(), "wt+");
    if (FilePointer)
      fclose(FilePointer);

    m_FileAppender = m_Log.getAppender(logFilename);
    if (m_FileAppender == nullptr)
    {
      m_FileAppender = new log4cplus::FileAppender(logFilename);
      m_FileAppender->setName(logFilename);
      m_FileAppender->setLayout(std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout("%d [%p] %m%n")));
      m_Log.addAppender(m_FileAppender);
    }
  }

  m_ConsoleAppender = m_Log.getAppender(logFilename+"_console");
  if (m_ConsoleAppender == nullptr)
  {
    m_ConsoleAppender = new log4cplus::ConsoleAppender();
    m_ConsoleAppender->setName(logFilename + "_console");
    m_ConsoleAppender->setLayout(std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout("%d [%p] %m%n")));
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
void Logger::SetLogLevel(log4cplus::LogLevel level)
{
  m_Log.setLogLevel(level);
}

//This function will return the priority of the logger
log4cplus::LogLevel Logger::GetLogLevel()
{
  return m_Log.getLogLevel();
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
  LOG4CPLUS_DEBUG(m_Log, FormatLogMessage(msg,origin));
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
  LOG4CPLUS_INFO(m_Log, FormatLogMessage(msg, origin));
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
  LOG4CPLUS_WARN(m_Log, FormatLogMessage(msg, origin));
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
  LOG4CPLUS_ERROR(m_Log, FormatLogMessage(msg, origin));
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
  LOG4CPLUS_FATAL(m_Log, FormatLogMessage(msg, origin));
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
