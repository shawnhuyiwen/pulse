
add_library_ex(CircuitOptimizationEngine
  H_FILES
    CircuitOptimizer.h
    SEValidationTarget.h
  CPP_FILES
    CircuitOptimizer.cpp
    SEValidationTarget.cpp
  PUBLIC_DEPENDS
    PulseEngine
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  INSTALL_HEADER_DIR
    study/circuit_optimization
  NO_INSTALL
  )
set_target_properties(CircuitOptimizationEngine PROPERTIES FOLDER "Studies//Circuit Optimization")

add_executable_ex(CircuitOptimizationDriver main.cpp)
set_target_properties(CircuitOptimizationDriver PROPERTIES FOLDER "Studies//Circuit Optimization")
target_link_libraries(CircuitOptimizationDriver CircuitOptimizationEngine protobuf::libprotobuf)