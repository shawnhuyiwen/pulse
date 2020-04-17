
add_executable_ex(MultiplexVentilationDriver main.cpp
                                             MVController.cpp
                                             MVController.h
                                             MVGenerator.cpp
                                             MVGenerator.h
                                             MVGenData.cpp
                                             MVSimulation.cpp
                                             MVRunner.cpp
                                             MVRunner.h)
                                             
set_target_properties (MultiplexVentilationDriver PROPERTIES FOLDER "Studies")
target_link_libraries(MultiplexVentilationDriver CommonPulseModels protobuf::libprotobuf)