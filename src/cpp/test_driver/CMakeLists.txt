
add_executable_ex(PulseTestDriver main.cpp)

target_link_libraries(PulseTestDriver CommonDataModelTests
                                      CommonPulseModelsTests
                                      RespiratoryEngineTests)
