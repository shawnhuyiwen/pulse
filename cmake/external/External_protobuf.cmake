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

set(Protobuf_VERSION "3.18.0" CACHE STRING "Select the  version of ProtoBuf to build.")
set_property(CACHE Protobuf_VERSION PROPERTY STRINGS "3.18.0" "3.15.8" "3.10.0")

if (Protobuf_VERSION VERSION_EQUAL 3.18.0)# Latest, Can change
  set(Protobuf_url "https://github.com/protocolbuffers/protobuf/releases/download/v3.18.0/protobuf-all-3.18.0.zip" )
  set(Protobuf_md5 "4fb72c818808b56244c85e9433c80416" )
elseif(Protobuf_VERSION VERSION_EQUAL 3.15.8) # Requested by DarkSlope
  set(Protobuf_url "https://github.com/protocolbuffers/protobuf/releases/download/v3.15.8/protobuf-all-3.15.8.zip")
  set(Protobuf_md5 "5249754276f08be7fef1421f418c1e70")
elseif (Protobuf_VERSION VERSION_EQUAL 3.10.0)# Requested by Swansea for compatibility with YarnSpinner v1.2.6
  set(Protobuf_url "https://github.com/protocolbuffers/protobuf/releases/download/v3.10.0/protobuf-all-3.10.0.zip" )
  set(Protobuf_md5 "46171f4afc4828b55b5a46dd02f5ef15" )
else()
  message(STATUS "Using Protobuf Version ${Protobuf_VERSION}, with no git hash, will redownload if you rebuild.")
  set(Protobuf_url "https://github.com/protocolbuffers/protobuf/releases/download/v${Protobuf_VERSION}/protobuf-all-${Protobuf_VERSION}.zip" )
  set(Protobuf_md5 "" )
  # Old versions we have used
  # Generally, We only support the latest version at the time of a release
  # And any release that somebody has requested we support for compatibility with their application
  #set(Protobuf_md5 "5cda6b2a21148df72ca1832005e89b13" ) - 3.17.1
endif()

include(AddExternalProject)
define_external_dirs_ex(protobuf)
add_external_project_ex( protobuf
  URL ${Protobuf_url}
  URL_MD5 ${Protobuf_md5}
  SOURCE_SUBDIR ./cmake
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${protobuf_PREFIX}/install
    -Dprotobuf_BUILD_TESTS:BOOL=OFF
    -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
    -Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF
    -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF#Don't change MSVC runtime settings (/MD or /MT)
    -Dprotobuf_WITH_ZLIB:BOOL=OFF
  ${PROTOBUF_MULTI_BUILD}
  ${PROTOBUF_MULTI_INSTALL}
  RELATIVE_INCLUDE_PATH "include"
  #DEPENDENCIES ""
  #VERBOSE
)
if (NOT USE_SYSTEM_protobuf)
  if(WIN32)
    set(protobuf_DIR ${protobuf_PREFIX}/install/cmake)
  else()
    set(protobuf_DIR ${protobuf_PREFIX}/install/${CMAKE_INSTALL_LIBDIR}/cmake/protobuf)
  endif()
  message(STATUS "protobuf_DIR : ${protobuf_DIR}")
  set(protobuf_SRC ${protobuf_PREFIX}/src)
endif()
