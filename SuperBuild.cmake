
include(ExternalProject)
include(CMakeDetermineSystem)

set(BUILD_SHARED_LIBS OFF)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
if(MSVC OR XCode)
# For multi configuration IDE environments start with release
  set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING INTERNAL FORCE )
endif()
##################################
## EIGEN                        ##
## Used for general matrix math ##
##################################

message( STATUS "External project - Eigen" )
set(eigen_VERSION "3.3.7" )
set(eigen_SRC "${CMAKE_BINARY_DIR}/eigen/src/eigen")
set(eigen_Patch "${CMAKE_SOURCE_DIR}/cmake/eigen-patches")

ExternalProject_Add( eigen
  PREFIX eigen
  URL "http://bitbucket.org/eigen/eigen/get/${eigen_VERSION}.tar.gz"
  URL_HASH MD5=f2a417d083fe8ca4b8ed2bc613d20f07
  #UPDATE_COMMAND 
  #  COMMAND ${CMAKE_COMMAND} -Deigen_source=${eigen_SRC} -Deigen_patch=${eigen_Patch} -P ${eigen_Patch}/Patch.cmake
  INSTALL_DIR "${CMAKE_INSTALL_PREFIX}"
  CMAKE_ARGS
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DINCLUDE_INSTALL_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
)
list(APPEND Pulse_DEPENDENCIES eigen)
# Install Headers
message(STATUS "Eigen is here : ${eigen_SRC}" )

###################################################
## log4cplus                                      ##
## General logging utility                       ##
## We have our own cmake file to build as dll/so ##
###################################################

message( STATUS "External project - log4cplus" )
set(log4cplus_SRC "${CMAKE_BINARY_DIR}/log4cplus/src/log4cplus" CACHE PATH "Log4cplus Source")

ExternalProject_Add( log4cplus
  PREFIX log4cplus
  URL "https://github.com/log4cplus/log4cplus/releases/download/REL_2_0_4/log4cplus-2.0.4.zip"
  URL_HASH MD5=cb075cd19ce561273b1c74907cc66b6a
  #GIT_REPOSITORY "https://github.com/log4cplus/log4cplus.git"
  #GIT_TAG 411a262b92a7bdc26348d6ae6b15f76763c21b44
#  GIT_SHALLOW TRUE
# Build this in the Inner build
# It will be easier to switch cofigurations in MSVC/XCode
  CONFIGURE_COMMAND "" 
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)
list(APPEND Pulse_DEPENDENCIES log4cplus)

###################################################
## Google Proto Buffers                          ##
## Multi-language serialization                  ##
###################################################

message( STATUS "External project - protobuf" )
#This is a working 3.7.0-rc2 hash, the tag is bunk :(
set(protobuf_URL "https://github.com/protocolbuffers/protobuf/releases/download/v3.9.0/protobuf-all-3.9.0.zip")
set(protobuf_MD5 "4f042c8b46823a69db3dcbc7381b73f4" )
set(protobuf_SRC "${CMAKE_BINARY_DIR}/protobuf/src/protobuf")
set(protobuf_Patch "${CMAKE_SOURCE_DIR}/cmake/protobuf-patches")

message(STATUS "Patching protobuf ${PULSE_IL2CPP_PATCH}")
ExternalProject_Add( protobuf
  PREFIX protobuf
  URL ${protobuf_URL}
  URL_MD5 ${protobuf_MD5}
  DOWNLOAD_DIR ${protobuf_SRC}
  SOURCE_SUBDIR ./cmake
  UPDATE_COMMAND 
    COMMAND ${CMAKE_COMMAND} -DPULSE_IL2CPP_PATCH=${PULSE_IL2CPP_PATCH} -Dprotobuf_source=${protobuf_SRC} -Dprotobuf_patch=${protobuf_Patch} -P ${protobuf_Patch}/Patch.cmake
  CMAKE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -Dprotobuf_BUILD_TESTS:BOOL=OFF
    -Dprotobuf_BUILD_EXAMPLES:BOOL=OFF
    -Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF
    -Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF#Don't change MSVC runtime settings (/MD or /MT)
    -Dprotobuf_WITH_ZLIB:BOOL=OFF
)
list(APPEND Pulse_DEPENDENCIES protobuf)

message(STATUS "protobuf is here : ${protobuf_SRC}" )

if(WIN32)
  ##########################################
  ## DIRENT                               ##
  ## Platform generic file system utility,##
  ## that is not included in msvc/mingw   ##
  ##########################################

  message( STATUS "External project - dirent" )
  set(dirent_DIR "${CMAKE_BINARY_DIR}/dirent/src/dirent")
  set(dirent_MD5 "cf5b4499d163604732f4dc91654056be" )
  ExternalProject_Add( dirent
    PREFIX dirent
    URL "https://github.com/tronkko/dirent/archive/1.22.zip"
    URL_MD5 ${dirent_MD5}
    CMAKE_ARGS
          -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
          -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
          -DINCLUDE_INSTALL_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
  )
  message(STATUS "dirent is here : ${dirent_DIR}" )
  list(APPEND Pulse_DEPENDENCIES dirent)
endif()

# ExternalProject_Add doesn't like to work with lists: it keeps only the first element
string(REPLACE ";" "::" CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")

# Generate the Pulse project after dependencies have been built
ExternalProject_Add( Pulse
  PREFIX Pulse
  DEPENDS ${Pulse_DEPENDENCIES}
  DOWNLOAD_COMMAND ""
  DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  BINARY_DIR ${CMAKE_BINARY_DIR}/Pulse
  CMAKE_ARGS
    -DSUPERBUILD:BOOL=OFF
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DPULSE_BUILD_CLR:BOOL=${PULSE_BUILD_CLR}
    # Let InnerBuild build and install these
    -Dlog4cplus_SRC=${log4cplus_SRC}
    -Dprotobuf_SRC=${protobuf_SRC}
)

add_custom_target(PulseData ALL)
add_dependencies(PulseData Pulse)
add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=genData -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=genStates -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
if(PULSE_DOWNLOAD_BASELINES)
  message(STATUS "I am going to pull the latest Pulse Baselines!")
  add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=updateBaselines -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
else()
  message(STATUS "I am NOT going to pull the latest Pulse Baselines!")
endif()
