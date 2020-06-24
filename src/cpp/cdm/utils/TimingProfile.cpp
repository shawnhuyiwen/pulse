/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "TimingProfile.h"

void TimingProfile::Clear()
{
  m_timers.clear();
}

void TimingProfile::Reset(const std::string& label)
{
  if (label.empty())
  {
    return;
  }
  if (m_timers[label].state == State::Running)
  {
    m_timers[label].start = Clock::now();
  }
  else if (m_timers[label].state == State::Ran)
  {
    m_timers[label].state = State::Ready;
  }
}

void TimingProfile::Start(const std::string& label)
{
  if (label.empty())
  {
    return;
  }

  m_timers[label].start = Clock::now();
  m_timers[label].state = State::Running;
}

void TimingProfile::Stop(const std::string& label)
{
  if (label.empty())
  {
    return;
  }

  if (m_timers[label].state == State::Running)
  {
    m_timers[label].end = Clock::now();
    m_timers[label].state = State::Ran;
  }
}

double TimingProfile::GetElapsedTime_s(const std::string& label)
{
  if (label.empty())
  {
    return 0;
  }

  std::chrono::milliseconds::rep milliseconds = GetElapsedTime<std::chrono::milliseconds>(label);
  double seconds = milliseconds / 1000.0;
  return seconds;
}
