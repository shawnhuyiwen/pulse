
add_library_ex(PulseC SHARED
  CPP_FILES
    PulseEngineC.cpp
  PUBLIC_DEPENDS
    PulseEngine
  #VERBOSE
)
