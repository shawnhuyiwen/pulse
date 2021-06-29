
add_library_ex(RespiratoryEngineTests
  H_FILES
    RespiratoryEngineTest.h
  CPP_FILES
    RespiratoryEngineTest.cpp
  PUBLIC_DEPENDS
    RespiratoryEngine
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  #VERBOSE
)
set_target_properties(RespiratoryEngineTests PROPERTIES FOLDER "RespiratoryEngine")
