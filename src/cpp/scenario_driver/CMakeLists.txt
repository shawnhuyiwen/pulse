if(${PROJECT_NAME}_LIBS_ONLY)
  return()
endif()

add_executable_ex(PulseScenarioDriver PulseScenarioDriver.cpp
                                      PulseScenarioDriver.h)

target_link_libraries(PulseScenarioDriver PulseEngine)
