
add_library_ex(MultiplexVentilationEngine
  H_FILES
    MVEngine.h
    MVGenerator.h
    MVRunner.h
  CPP_FILES
    MVEngine.cpp
    MVGenerator.cpp
    MVRunner.cpp
  PUBLIC_DEPENDS
    CommonPulseModels
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  INSTALL_HEADER_DIR
    study/multiplex_ventilation
  )
set_target_properties(MultiplexVentilationEngine PROPERTIES FOLDER "Studies//MultiplexVentilation")

add_executable_ex(MultiplexVentilationDriver main.cpp)
set_target_properties(MultiplexVentilationDriver PROPERTIES FOLDER "Studies//MultiplexVentilation")
target_link_libraries(MultiplexVentilationDriver MultiplexVentilationEngine protobuf::libprotobuf)