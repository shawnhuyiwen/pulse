
add_library_ex(PulseC SHARED
  CPP_FILES
    PulseEngineC.cpp
  PUBLIC_DEPENDS
    PulseEngine
  #VERBOSE
)
set_target_properties (PulseC PROPERTIES FOLDER ${PROJECT_NAME})
