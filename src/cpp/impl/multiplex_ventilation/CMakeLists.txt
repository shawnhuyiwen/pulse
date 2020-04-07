
add_executable_ex(MultiplexVentilationDriver main.cpp
                                             MVController.cpp
                                             MVController.h
                                             MVGenData.cpp
                                             MVSimulation.cpp)

target_link_libraries(MultiplexVentilationDriver CommonPulseModels protobuf::libprotobuf)