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
    if (_file.is_open())
      _file.close();
  }

  bool log(Logger::Level requested_level)
  {
    return requested_level >= _log_level;
  }
  void log(Logger::Level requested_level, const std::string& out)
  {
    if (!_log_to_console && !_log_to_file)
      return;
    if (_last_requested_level != requested_level)
    {
      switch (requested_level)
      {
      case Logger::Level::Debug:
        _str_requested_level = "[DEBUG]";
        break;
      case Logger::Level::Info:
        _str_requested_level = "[INFO]";
        break;
      case Logger::Level::Warn:
        _str_requested_level = "[WARN]";
        break;
      case Logger::Level::Error:
        _str_requested_level = "[ERROR]";
        break;
      case Logger::Level::Fatal:
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

  bool _log_to_console = false;
  bool _log_to_file = false;
  Logger::Level _log_level = Logger::Level::Info;
  std::string _str_requested_level = "[INFO]";
  Logger::Level _last_requested_level = Logger::Level::Info;

  std::ofstream _file;
};

void Logger::Initialize() {}
void Logger::Deinitialize() {}

//logger constructor
Logger::Logger(const std::string& logFilename)
{
  m_time = nullptr;
  _log_lib = new log_lib();
  SetLogFile(logFilename);
}

void Logger::LogToConsole(bool b)
{
  _log_lib->_log_to_console = b;
}

bool Logger::IsLoggingToConsole()
{
  return _log_lib->_log_to_console;
}

void Logger::SetLogFile(const std::string& logFilename)
{
  if (logFilename.empty())
  {
    _log_lib->_log_to_file = false;
    return;
  }
  _log_lib->_log_to_file = true;

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
void Logger::SetLogLevel(Logger::Level l)
{
  _log_lib->_log_level = l;
}

//This function will return the priority of the logger
Logger::Level Logger::GetLogLevel() const
{
  return _log_lib->_log_level;
}

bool Logger::HasForward() const
{
  return !m_Forwards.empty();
}
void Logger::AddForward(LoggerForward* forward)
{
  if (forward != nullptr && std::find(m_Forwards.begin(), m_Forwards.end(), forward) == m_Forwards.end())
    m_Forwards.push_back(forward);
}

void Logger::RemoveForward(LoggerForward* forward)
{
  auto idx = std::find(m_Forwards.begin(), m_Forwards.end(), forward);
  if (idx != m_Forwards.end())
    m_Forwards.erase(idx);
}

void Logger::RemoveForwards()
{
  m_Forwards.clear();
}

std::string Logger::FormatLogMessage(const std::string& msg, const std::string& origin)
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

void Logger::Debug(std::string const& msg, const std::string& origin)
{
  if (_log_lib->log(Level::Debug))
  {
    _log_lib->log(Level::Debug, FormatLogMessage(msg, origin));
    for (auto fwd : m_Forwards)
      fwd->ForwardDebug(m_ss.str().c_str(), origin.c_str());
  }
}

void Logger::Debug(std::stringstream& msg, const std::string& origin)
{
  Debug(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Debug(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Debug(ss.str(), origin);
}

void Logger::Info(const std::string& msg, const std::string& origin)
{
  if (_log_lib->log(Level::Info))
  {
    _log_lib->log(Level::Info, FormatLogMessage(msg, origin));
    for (auto fwd : m_Forwards)
      fwd->ForwardInfo(m_ss.str().c_str(), origin.c_str());
  }
}

void Logger::Info(std::stringstream& msg, const std::string& origin)
{
  Info(msg.str(), origin);
  msg.str("");
  msg.clear();
}

void Logger::Info(const std::stringstream& msg, const std::string& origin)
{
  Info(msg.str(), origin);
}

void Logger::Info(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Logger::Warning(const std::string& msg, const std::string& origin)
{
  if (_log_lib->log(Level::Warn))
  {
    _log_lib->log(Level::Warn, FormatLogMessage(msg, origin));
    for (auto fwd : m_Forwards)
      fwd->ForwardWarning(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Warning(std::stringstream& msg, const std::string& origin)
{
  Warning(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Warning(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Warning(ss.str(), origin);
}

void Logger::Error(const std::string& msg, const std::string& origin)
{
  if (_log_lib->log(Level::Error))
  {
    _log_lib->log(Level::Error, FormatLogMessage(msg, origin));
    for (auto fwd : m_Forwards)
      fwd->ForwardError(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Error(std::stringstream& msg, const std::string& origin)
{
  Error(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Error(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Error(ss.str(), origin);
}

void Logger::Fatal(const std::string& msg, const std::string& origin)
{
  if (_log_lib->log(Level::Fatal))
  {
    _log_lib->log(Level::Fatal, FormatLogMessage(msg, origin));
    for (auto fwd : m_Forwards)
      fwd->ForwardFatal(m_ss.str().c_str(), origin.c_str());
  }
}
void Logger::Fatal(std::stringstream& msg, const std::string& origin)
{
  Fatal(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Logger::Fatal(std::ostream& msg, const std::string& origin)
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}

Loggable::Loggable(Logger* logger)
{
  myLogger = false;
  m_Logger = logger;
  if (m_Logger == nullptr)
  {
    myLogger = true;
    m_Logger = new Logger();
  }
}

Loggable::Loggable(std::string const& logfile)
{
  myLogger = true;
  m_Logger = new Logger(logfile);
}

Loggable::~Loggable()
{
  if (myLogger)
    delete m_Logger;
}

Logger* Loggable::GetLogger() const
{
  return m_Logger;
}
void Loggable::SetLogger(Logger& logger)
{
  if (myLogger)
    delete m_Logger;
  myLogger = false;
  m_Logger = &logger;
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

void Loggable::Info(const std::stringstream& msg, const std::string& origin) const
{
  Info(msg.str(), origin);
}

void Loggable::Info(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Info(ss.str(), origin);
}

void Loggable::Warning(const std::string& msg, const std::string& origin) const
{
  if (m_Logger)
    m_Logger->Warning(msg, origin);
}
void Loggable::Warning(std::stringstream& msg, const std::string& origin) const
{
  Warning(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Warning(std::ostream& msg, const std::string& origin) const
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

void Loggable::Fatal(const std::string& msg, const std::string& origin) const
{
  std::cerr << "FATAL:" << msg << " : " << origin << std::endl;
  if (m_Logger)
    m_Logger->Fatal(msg, origin);
}
void Loggable::Fatal(std::stringstream& msg, const std::string& origin) const
{
  Fatal(msg.str(), origin);
  msg.str("");
  msg.clear();
}
void Loggable::Fatal(std::ostream& msg, const std::string& origin) const
{
  std::stringstream ss;
  ss << msg.rdbuf();
  Fatal(ss.str(), origin);
}
