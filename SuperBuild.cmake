
include(ExternalProject)
include(CMakeDetermineSystem)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_GENERATION
     CMAKE_GENERATOR ${CMAKE_GENERATOR}
     CMAKE_GENERATOR_PLATFORM ${CMAKE_GENERATOR_PLATFORM}
     CMAKE_GENERATOR_TOOLSET ${CMAKE_GENERATOR_TOOLSET}
     CMAKE_GENERATOR_INSTANCE ${CMAKE_GENERATOR_INSTANCE}
   )

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
set(Eigen3_DIR "${CMAKE_BINARY_DIR}/eigen/install")

ExternalProject_Add( eigen
  PREFIX eigen
  URL "https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz"
  URL_HASH MD5=9e30f67e8531477de4117506fe44669b
  #UPDATE_COMMAND 
  #  COMMAND ${CMAKE_COMMAND} -Deigen_source=${eigen_SRC} -Deigen_patch=${eigen_Patch} -P ${eigen_Patch}/Patch.cmake
  INSTALL_DIR "${Eigen3_DIR}"
  ${CMAKE_GENERATION}
  CMAKE_ARGS
        -DCMAKE_TOOLCHAIN_FILE:FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:STRING=${Eigen3_DIR}
        -DINCLUDE_INSTALL_DIR:STRING=${Eigen3_DIR}/include
)
list(APPEND Pulse_DEPENDENCIES eigen)
# Install Headers
message(STATUS "Eigen is here : ${eigen_SRC}" )

# Loggers are built in the Inner build
# It will be easier to switch cofigurations in MSVC/XCode
set(PULSE_LOGGER "simple" CACHE STRING "Select Logger Library")
set(AVAILABLE_PULSE_LOGGERS "simple;log4cpp;log4cplus" CACHE INTERNAL "List of available loggers")
set_property(CACHE PULSE_LOGGER PROPERTY STRINGS ${AVAILABLE_PULSE_LOGGERS})

set(logger_SRC)
if (${PULSE_LOGGER} STREQUAL "log4cpp")
  ###################################################
  ## log4cpp                                       ##
  ## General logging utility                       ##
  ###################################################

  message( STATUS "External project - log4cpp" )
  set(logger_SRC "${CMAKE_BINARY_DIR}/log4cpp/src/log4cpp" CACHE INTERNAL "Log4cpp Source")

  if(UNIX)
    set(CONFIGURE "./configure")
  endif()

  ExternalProject_Add( log4cpp
    PREFIX log4cpp
    URL "https://data.kitware.com/api/v1/file/5d577c0e85f25b11ff34dbf8/download"
    URL_HASH MD5=b9e2cee932da987212f2c74b767b4d8b
    #GIT_REPOSITORY "https://github.com/log4cplus/log4cpp.git"
    #GIT_TAG 411a262b92a7bdc26348d6ae6b15f76763c21b44
    #GIT_SHALLOW TRUE
    ${CMAKE_GENERATION}
    UPDATE_COMMAND 
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/log4cpp-patches/CMakeLists.txt ${logger_SRC}/CMakeLists.txt
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/log4cpp-patches/config.guess ${logger_SRC}/config/config.guess
      #COMMAND ${CONFIGURE}
    CONFIGURE_COMMAND "" 
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  list(APPEND Pulse_DEPENDENCIES log4cpp)
elseif(${PULSE_LOGGER} STREQUAL "log4cplus")
  ###################################################
  ## log4cplus                                     ##
  ## General logging utility                       ##
  ###################################################

  message( STATUS "External project - log4cplus" )
  set(logger_SRC "${CMAKE_BINARY_DIR}/log4cplus/src/log4cplus" CACHE INTERNAL "Log4cplus Source")

  ExternalProject_Add( log4cplus
    PREFIX log4cplus
    URL "https://github.com/log4cplus/log4cplus/releases/download/REL_2_0_4/log4cplus-2.0.4.zip"
    URL_HASH MD5=cb075cd19ce561273b1c74907cc66b6a
    #GIT_REPOSITORY "https://github.com/log4cplus/log4cplus.git"
    #GIT_TAG 411a262b92a7bdc26348d6ae6b15f76763c21b44
    #  GIT_SHALLOW TRUE
    # Build this in the Inner build
    # It will be easier to switch cofigurations in MSVC/XCode
    ${CMAKE_GENERATION}
    CONFIGURE_COMMAND "" 
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  list(APPEND Pulse_DEPENDENCIES log4cplus)
endif()

###################################################
## Google Proto Buffers                          ##
## Multi-language serialization                  ##
###################################################

message( STATUS "External project - protobuf" )
set(protobuf_URL "https://github.com/protocolbuffers/protobuf/releases/download/v3.11.1/protobuf-all-3.11.1.zip")
set(protobuf_MD5 "01f0e58bb432727d494b390e62f877d5" )
set(protobuf_SRC "${CMAKE_BINARY_DIR}/protobuf/src/protobuf")
set(protobuf_DIR "${CMAKE_BINARY_DIR}/protobuf/install")
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
  ${CMAKE_GENERATION}
  CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE:FILE=${CMAKE_TOOLCHAIN_FILE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:STRING=${protobuf_DIR}
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
    ${CMAKE_GENERATION}
    CMAKE_ARGS
      -DCMAKE_TOOLCHAIN_FILE:FILE=${CMAKE_TOOLCHAIN_FILE}
      -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DINCLUDE_INSTALL_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
  )
  message(STATUS "dirent is here : ${dirent_DIR}" )
  list(APPEND Pulse_DEPENDENCIES dirent)
endif()

###################################################
## PyBind11                                      ##
## Creation of Pulse Python Bindings             ##
###################################################
if(PULSE_PYTHON_BINDINGS)
  message( STATUS "External project - pybind11" )
  set(pybind11_install "${CMAKE_BINARY_DIR}/pybind11/install/")
  set(pybind11_version "2.4.3")
  set(pybind11_url "https://github.com/pybind/pybind11/archive/v${pybind11_version}.tar.gz")
  set(pybind11_md5 "62254c40f89925bb894be421fe4cdef2")

  ExternalProject_Add(pybind11
    PREFIX pybind11
    URL ${pybind11_url}
    URL_MD5 ${pybind11_md5}
    DOWNLOAD_NAME ${pybind11_dlname}
    ${CMAKE_GENERATION}
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${pybind11_install}
      -DPYBIND11_TEST:BOOL=OFF 
      -DPYBIND11_PYTHON_VERSION=3.6
      -DPYBIND11_CPP_STANDARD:STRING=/std:c++11
  )
  
  set(pybind11_DIR "${pybind11_install}/share/cmake/pybind11")
  message(STATUS "pybind11 is here : ${pybind11_DIR}" )
  list(APPEND Pulse_DEPENDENCIES pybind11)
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
  ${CMAKE_GENERATION}
  CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE:FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    # Pulse Specific
    -DPULSE_SUPERBUILD:BOOL=OFF
    -DPULSE_BUILD_JAVA_UTILS:BOOL=${PULSE_BUILD_JAVA_UTILS}
    -DPULSE_BUILD_CLR:BOOL=${PULSE_BUILD_CLR}
    -DPULSE_LOGGER:STRING=${PULSE_LOGGER}
    -DPULSE_PYTHON_BINDINGS:BOOL=${PULSE_PYTHON_BINDINGS}
    -Deigen_DIR:PATH=${eigen_DIR}
    # Let InnerBuild build and install these
    -Dlogger_SRC:PATH=${logger_SRC}
    -Dprotobuf_SRC:PATH=${protobuf_SRC}
    -Dprotobuf_DIR:PATH=${protobuf_DIR}
)

# Need Java Utils to generate data
if (PULSE_BUILD_JAVA_UTILS)
add_custom_target(PulseData ALL)
add_dependencies(PulseData Pulse)
add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=genData -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=genStates -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
else()
  message(WARNING "Without Java Utils, this build will not generate required data files needed for Pulse to execute")
  message(WARNING "You will need to get these required data files from another build/source")
endif()

if(PULSE_DOWNLOAD_BASELINES)
  message(STATUS "I am going to pull the latest Pulse Baselines!")
  add_custom_command(TARGET PulseData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=updateBaselines -P run.cmake WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
            WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)
else()
  message(STATUS "I am NOT going to pull the latest Pulse Baselines!")
endif()
