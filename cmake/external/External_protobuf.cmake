#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
if(${PROJECT_NAME}_MULTI_BUILD)
  message(STATUS "Building multiple protobuf configurations at once")
  set(PROTOBUF_DEBUG_BLD COMMAND ${CMAKE_COMMAND} --build . --config debug)
  set(PROTOBUF_RELEASE_BLD COMMAND ${CMAKE_COMMAND} --build . --config release)
  set(PROTOBUF_MULTI_BUILD BUILD_COMMAND ${PROTOBUF_DEBUG_BLD} ${PROTOBUF_RELEASE_BLD})
  set(PROTOBUF_DEBUG_INSTALL COMMAND ${CMAKE_COMMAND} --build . --target install --config debug)
  set(PROTOBUF_RELEASE_INSTALL COMMAND ${CMAKE_COMMAND} --build . --target install --config release)
  set(PROTOBUF_MULTI_INSTALL BUILD_COMMAND ${PROTOBUF_DEBUG_INSTALL} ${PROTOBUF_RELEASE_INSTALL})
endif()

# Support getting various versions, as end users could be using a different versions
# And their application runtime environment needs to have all consistent dlls, or they fight!
# Note that the Protobuf C++ API changed and required Pulse updates
# Versions older than 22.0 are not supported anymore

set(Protobuf_VERSION "v24.3" CACHE STRING "Select the  version of ProtoBuf to build.")
set_property(CACHE Protobuf_VERSION PROPERTY STRINGS "24.3")

set(BUILD_PROTOC_BINARIES ON)
if(${PROJECT_NAME}_LIBS_ONLY)
  set(BUILD_PROTOC_BINARIES OFF)
endif()

include(AddExternalProject)
define_external_dirs_ex(protobuf)
add_external_project_ex( protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG ${Protobuf_VERSION}
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${protobuf_PREFIX}/install
    -Dprotobuf_BUILD_TESTS:BOOL=OFF
    -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
    -Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF
    -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF#Don't change MSVC runtime settings (/MD or /MT)
    -Dprotobuf_WITH_ZLIB:BOOL=OFF
    -Dprotobuf_BUILD_PROTOC_BINARIES:BOOL=${BUILD_PROTOC_BINARIES}
  ${PROTOBUF_MULTI_BUILD}
  ${PROTOBUF_MULTI_INSTALL}
  RELATIVE_INCLUDE_PATH "include"
  #DEPENDENCIES ""
  #VERBOSE
)
if (NOT USE_SYSTEM_protobuf)
  set(protobuf_SRC ${protobuf_PREFIX}/src)
  set(protobuf_INSTALL ${protobuf_PREFIX}/install)
  if(WIN32)
    set(protobuf_DIR ${protobuf_PREFIX}/install/cmake)
  else()
    set(protobuf_DIR ${protobuf_PREFIX}/install/${CMAKE_INSTALL_LIBDIR}/cmake/protobuf)
  endif()
  message(STATUS "protobuf_DIR : ${protobuf_DIR}")
endif()
