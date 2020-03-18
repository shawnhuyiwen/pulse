
add_executable_ex(PulseScenarioDriver cpp/PulseScenarioDriver.cpp
                                      cpp/PulseScenarioDriver.h)

target_link_libraries(PulseScenarioDriver PulseEngine)
