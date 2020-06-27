/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "Logger.h"

#include <chrono>

class CDM_DECL TimingProfile
{

public:
    TimingProfile() {}
    virtual ~TimingProfile() {}

    void Clear();

    /**
     * Resets the timer, this can have different effects based on the state of the timer:
     *    1) Ready   - No effect
     *    2) Running - Resets the elapsed time to zero and continues running
     *    3) Ran     - Resets the elapsed time to zero and places the timer back into the Ready state
     */
    void Reset(const std::string& label);

    /** 
     * Starts the timer.  Calling Start while the timer is running will reset its elapsed time.  Calling Start
     * after the timer has been started and stopped will reset its elapsed time, it does not pause and unpause
     * the running elapsed time.
     */
    void Start(const std::string& label);

    /** 
     * Stops the timer, elapsed time will remain constant until calling Reset or Start
     */
    void Stop(const std::string& label);

    /**
     * Returns the total elapsed time in seconds to three decimal places.  If the timer is running, this function
     * returns the elapsed time up until now.  If the timer has been stopped, this function returns the elapsed time
     * between the start and stop.  If the timer hasn't been run yet (Ready state), this function returns 0.
     */
    double GetElapsedTime_s(const std::string& label);

    /**
     * Returns the elapsed time for a particular timer
     */
    template<typename Duration>
    typename Duration::rep GetElapsedTime(const std::string& label)
    {
      Timer timer = m_timers[label];
      if (timer.state == State::Running)
      {
          return std::chrono::duration_cast<Duration>(Clock::now() - timer.start).count();
      }
      else if (timer.state == State::Ran)
      {
          return std::chrono::duration_cast<Duration>(timer.end - timer.start).count();
      }
      else
      {
          return typename Duration::rep(0);
      }
    }

private:

  using Clock = std::chrono::high_resolution_clock;

  enum class State
  {
    Ready,      // Timer has no data and is not running
    Running,    // Timer is currently running
    Ran         // Timer has been stopped and has data
  };

  struct Timer
  {
    Clock::time_point start;
    Clock::time_point end;
    State state = State::Ready;
  };

  std::map<std::string, Timer> m_timers;
};