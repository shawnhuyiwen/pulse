
add_library_ex(RespiratoryEngine
  H_FILES
    RespiratoryEngine.h
    controller/Controller.h
    controller/Engine.h
  CPP_FILES
    RespiratoryEngine.cpp
    controller/Controller.cpp
    controller/Engine.cpp
    controller/SetupCircuitsAndCompartments.cpp
  PUBLIC_DEPENDS
    CommonPulseModels
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  #VERBOSE
)
set_target_properties(RespiratoryEngine PROPERTIES FOLDER "RespiratoryEngine")

add_subdirectory(test)
