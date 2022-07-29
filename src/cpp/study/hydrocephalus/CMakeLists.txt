
add_library_ex(HydrocephalusEngine
  H_FILES
    HRunner.h
  CPP_FILES
    HRunner.cpp
  PUBLIC_DEPENDS
    PulseEngine
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  INSTALL_HEADER_DIR
    study/hydrocephalus
  NO_INSTALL
  )
set_target_properties(HydrocephalusEngine PROPERTIES FOLDER "Studies//Hydrocephalus")

if(${PROJECT_NAME}_LIBS_ONLY)
  return()
endif()
add_executable_ex(HydrocephalusDriver main.cpp)
set_target_properties(HydrocephalusDriver PROPERTIES FOLDER "Studies//Hydrocephalus")
target_link_libraries(HydrocephalusDriver HydrocephalusEngine protobuf::libprotobuf)