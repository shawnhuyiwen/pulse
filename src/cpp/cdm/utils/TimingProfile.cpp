/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/utils/TimingProfile.h"

void TimingProfile::Clear()
{
  m_timers.clear();
}

TimingProfile::Timer& TimingProfile::GetTimer(const std::string& label)
{
  if (label.empty())
  {
    return m_timers[""];
  }

  return m_timers[label];
}

void TimingProfile::Reset(Timer& timer)
{
  if (timer.state == State::Running)
  {
    timer.start = Clock::now();
  }
  else if (timer.state == State::Ran)
  {
    timer.state = State::Ready;
  }
}

void TimingProfile::Start(Timer& timer)
{
  timer.start = Clock::now();
  timer.state = State::Running;
}

void TimingProfile::Stop(Timer& timer)
{
  if (timer.state == State::Running)
  {
    timer.end = Clock::now();
    timer.state = State::Ran;
  }
}

double TimingProfile::GetElapsedTime_s(Timer& timer)
{
  std::chrono::milliseconds::rep milliseconds = GetElapsedTime<std::chrono::milliseconds>(timer);
  double seconds = milliseconds / 1000.0;
  return seconds;
}

TimingProfile::Clock::duration TimingProfile::GetDuration(Timer& timer)
{
  if (timer.state == State::Running)
  {
    return Clock::now() - timer.start;
  }
  else if (timer.state == State::Ran)
  {
    return timer.end - timer.start;
  }
  else
  {
    return TimingProfile::Clock::duration(0);
  }
}
