if(${PROJECT_NAME}_LIBS_ONLY)
  return()
endif()

add_executable_ex(PulseTestDriver main.cpp)
target_link_libraries(PulseTestDriver CommonDataModelTests
                                      PulseEngineTests)
