/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "utils/Logger.h"
#include "utils/FileUtils.h"
#include "properties/SEScalarTime.h"

const std::string Loggable::empty("");

class log_lib
{
public:
  log_lib()
  {

  }
  virtual ~log_lib() 
  {
    
  }

  bool log(Logger::level requested_level)
  {
    if (_log_level < requested_level)
      return false;
    if (!_log_to_console && !_log_to_file)
      return false;
    return true;
  }
  void log(Logger::level requested_level, const std::string& out)
  {
    if (_last_requested_level != requested_level)
    {
      switch (requested_level)
      {
      case Logger::level::Debug:
        _str_requested_level = "[DEBUG]";
        break;
      case Logger::level::Info:
        _str_requested_level = "[INFO]";
        break;
      case Logger::level::Warn:
        _str_requested_level = "[WARN]";
        break;
      case Logger::level::Error:
        _str_requested_level = "[ERROR]";
        break;
      case Logger::level::Fatal:
        _str_requested_level = "[FATAL]";
        break;
      }
      _last_requested_level = requested_level;
    }
    if (_log_to_console)
      std::cout << _str_requested_level << out << "\n";
    if (_log_to_file)
      _file << _str_requested_level << out << std::endl;
  }

  bool _log_to_console = true;
  bool _log_to_file = true;
  Logger::level _log_level = Logger::level::Info;
  std::string _str_requested_level = "[INFO]";
  Logger::level _last_requested_level = Logger::level::Info;

  std::ofstream _file;
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
  _log_lib->_log_to_console = b;
}

void Logger::ResetLogFile(const std::string& logFilename)
{
  if (logFilename.empty())
  {
    _log_lib->_log_to_file = false;
    return;
  }
  _log_lib->_log_to_file = true;

  SetLogLevel(level::Info);

  CreateFilePath(logFilename);
  _log_lib->_file.close();
  // delete previous log contents if it exists
  _log_lib->_file.open(logFilename.c_str(), std::ofstream::out | std::ofstream::trunc);
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
  _log_lib->_log_level = l;
}

//This function will return the priority of the logger
Logger::level Logger::GetLogLevel()
{
  return _log_lib->_log_level;
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
  if (_log_lib->log(level::Debug))
  {
    _log_lib->log(level::Debug, FormatLogMessage(msg, origin));
    if (m_Forward != nullptr)
      m_Forward->ForwardDebug(m_ss.str().c_str(), origin.c_str());
  }
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
  if (_log_lib->log(level::Info))
  {
    _log_lib->log(level::Info, FormatLogMessage(msg, origin));
    if (m_Forward != nullptr)
      m_Forward->ForwardInfo(m_ss.str().c_str(), origin.c_str());
  }
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
  if (_log_lib->log(level::Warn))
  {
    _log_lib->log(level::Warn, FormatLogMessage(msg, origin));
    if (m_Forward != nullptr)
      m_Forward->ForwardWarning(m_ss.str().c_str(), origin.c_str());
  }
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
  if (_log_lib->log(level::Error))
  {
    _log_lib->log(level::Error, FormatLogMessage(msg, origin));
    if (m_Forward != nullptr)
      m_Forward->ForwardError(m_ss.str().c_str(), origin.c_str());
  }
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
  if (_log_lib->log(level::Fatal))
  {
    _log_lib->log(level::Fatal, FormatLogMessage(msg, origin));
    if (m_Forward != nullptr)
      m_Forward->ForwardFatal(m_ss.str().c_str(), origin.c_str());
  }
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

void Loggable::Debug(const std::string& msg, const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Debug(msg, origin);
}
void Loggable::Debug(std::stringstream& msg, const std::string& origin) const
{
  Debug(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Debug(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
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

void Loggable::Error(const std::string& msg, const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Error(msg, origin);
}
void Loggable::Error(std::stringstream& msg, const std::string& origin) const
{
  Error(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Error(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Loggable::Info(const std::string& msg, const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Info(msg, origin);
}
void Loggable::Info(std::stringstream& msg, const std::string& origin) const
{
  Info(msg.str(), origin);
  msg.str("");
  msg.clear();
}

void Loggable::Fatal(const std::string&  msg, const std::string&  origin) const
{
  std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
  if (m_Logger)
    m_Logger->Fatal(msg, origin);
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
