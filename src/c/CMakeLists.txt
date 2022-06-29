set(_lib_type SHARED)
if(${PROJECT_NAME}_LIBS_ONLY)
set(_lib_type STATIC)
endif()

add_library_ex(PulseC ${_lib_type}
  CPP_FILES
    PulseEngineC.cpp
  PUBLIC_DEPENDS
    PulseEngine
  #VERBOSE
)
set_target_properties (PulseC PROPERTIES FOLDER ${PROJECT_NAME})
