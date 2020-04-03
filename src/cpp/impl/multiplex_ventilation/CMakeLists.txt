
add_executable_ex(MultiplexVentilationDriver main.cpp
                                             MVCircuits.cpp
                                             MVCircuits.h
                                             MVCompartments.cpp
                                             MVCompartments.h
                                             MVController.cpp
                                             MVController.h
                                             MVPulseController.cpp
                                             MVPulseController.h)

target_link_libraries(MultiplexVentilationDriver CommonPulseModels)