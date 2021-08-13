
add_library_ex(HumanAdultVentilationMechanics
  H_FILES
    Engine.h
    controller/CompartmentManager.h
    controller/Controller.h
  CPP_FILES
    Engine.cpp
    controller/CompartmentManager.cpp
    controller/Controller.cpp
    controller/SetupCircuitsAndCompartments.cpp
  PUBLIC_DEPENDS
    HumanAdultWholeBody
  PRIVATE_DEPENDS
    protobuf::libprotobuf
)

add_library_ex(HumanAdultVentilationMechanicsTests
  H_FILES
    test/EngineTest.h
  CPP_FILES
    test/EngineTest.cpp
  PUBLIC_DEPENDS
    HumanAdultVentilationMechanics
  INSTALL_HEADER_DIR
    test/
)
