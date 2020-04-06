
add_executable_ex(MultiplexVentilationDriver main.cpp
                                             MVController.cpp
                                             MVController.h
                                             MVGenData.cpp)

target_link_libraries(MultiplexVentilationDriver CommonPulseModels protobuf::libprotobuf)