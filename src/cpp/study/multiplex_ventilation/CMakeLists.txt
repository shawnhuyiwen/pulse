
add_library_ex(MultiplexVentilationEngine MVEngine.cpp
                                          MVEngine.h
                                          MVGenerator.cpp
                                          MVGenerator.h
                                          MVRunner.cpp
                                          MVRunner.h)
set_target_properties(MultiplexVentilationEngine PROPERTIES FOLDER "Studies//MultiplexVentilation")
target_link_libraries(MultiplexVentilationEngine CommonPulseModels protobuf::libprotobuf)

add_executable_ex(MultiplexVentilationDriver main.cpp)
set_target_properties(MultiplexVentilationDriver PROPERTIES FOLDER "Studies//MultiplexVentilation")
target_link_libraries(MultiplexVentilationDriver MultiplexVentilationEngine)