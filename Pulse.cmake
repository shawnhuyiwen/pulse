project(Pulse)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Policy to address @foo@ variable expansion
set(PULSE_VERSION_MAJOR 1)
set(PULSE_VERSION_MINOR 1)
set(PULSE_VERSION_PATCH 0)
set(PULSE_VERSION "${PULSE_VERSION_MAJOR}.${PULSE_VERSION_MINOR}.${PULSE_VERSION_PATCH}")

# Build the core libraries as shared or static (DataModel, CDM, Engine)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 11)

set(PULSE_DEBUG_POSTFIX "d")
set(PULSE_RELWITHDEBINFO_POSTFIX "di")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo" CACHE STRING INTERNAL FORCE )
if(MSVC)  
# Using MD as that seems to be what I run into alot, you could change these to /MT and /MTd if you want...
  set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob2 /Oi /Od /RTC1 /MP" CACHE STRING INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "/MD /MP" CACHE STRING INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MDd /Zi /MP" CACHE STRING INTERNAL FORCE)
endif()

if(MINGW)

endif()

if(APPLE)

endif()

if(UNIX)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN")
endif()

if ( CMAKE_COMPILER_IS_GNUCC )
  # Protobuf is not using the same variable name in its swap specificiation definitions
  # Resulting in a LOT of warnings per file that includes protobuf headers, slowing the build down
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-attributes")
  if(CMAKE_BUILD_TYPE EQUAL "Debug")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
  endif()
endif()

message(STATUS "Looking for modules here : ${CMAKE_PREFIX_PATH}")
set(CMAKE_CXX_STANDARD_LIBRARIES "" CACHE STRING INTERNAL FORCE)
set(CMAKE_C_STANDARD_LIBRARIES "" CACHE STRING INTERNAL FORCE)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

set(SCHEMA_SRC "${CMAKE_SOURCE_DIR}/schema")
set(SCHEMA_DST "${CMAKE_SOURCE_DIR}/schema/bind")

if(NOT Eigen3_DIR)
  set(Eigen3_DIR ${CMAKE_BINARY_DIR}/../eigen/install)
endif()
find_package(Eigen3 REQUIRED)

if(WIN32)
  find_package(Dirent REQUIRED)
endif()

# The outer build does some custom installing of dependent libraries
# Instead of using find_package, I will make sure things are where expected

if(NOT protobuf_SRC)
  set(protobuf_SRC ${CMAKE_BINARY_DIR}/../protobuf/src/protobuf)
endif()
set(protobuf_Header ${protobuf_SRC}/src/google/protobuf/package_info.h)
# Proto headers should have been installed here by the outer build
if(NOT EXISTS ${protobuf_Header})
  message(FATAL_ERROR "I cannot find protobuf source, please set protobuf_SRC to its root directory")
endif()
if(NOT protobuf_DIR)
  set(protobuf_DIR ${CMAKE_BINARY_DIR}/../protobuf/install)
endif()
list(APPEND CMAKE_PREFIX_PATH ${protobuf_DIR})
# Settings for protobuf configuration
set(protobuf_BUILD_PROTOC_BINARIES OFF CACHE BOOL INTERNAL FORCE)
set(protobuf_BUILD_SHARED_LIBS OFF CACHE BOOL INTERNAL FORCE)
set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE BOOL INTERNAL FORCE)#Use our MSVC runtime settings (/MD or /MT)
set(protobuf_BUILD_TESTS OFF CACHE BOOL INTERNAL FORCE)
set(protobuf_BUILD_EXAMPLES OFF CACHE BOOL INTERNAL FORCE)
add_subdirectory("${protobuf_SRC}/cmake" "${protobuf_SRC}-build")
set_target_properties (libprotobuf libprotobuf-lite PROPERTIES FOLDER protobufs)

set(logger_lib)
set(logger_SRC)
if (${PULSE_LOGGER} STREQUAL "log4cpp")
    # log4cpp src should have been download to somewhere
  if(NOT logger_SRC)
    # It should be here if the outer build ran
    set(logger_SRC ${CMAKE_BINARY_DIR}/../log4cpp/src/log4cpp)
  endif()
  set(log4cpp_HEADER ${logger_SRC}/include/log4cpp/FileAppender.hh)
  if(NOT EXISTS ${log4cpp_HEADER})
    message(FATAL_ERROR "I cannot find log4cpp source, please set logger_SRC to its root directory")
  endif()
  list(APPEND LOGGER_INCLUDE_DIR ${logger_SRC}/include)
  list(APPEND LOGGER_INCLUDE_DIR ${logger_SRC}-build/include)
  set(BUILD_TESTING OFF CACHE BOOL INTERNAL FORCE)
  add_subdirectory(${logger_SRC} ${logger_SRC}-build)
  set(logger_lib log4cpp)
elseif (${PULSE_LOGGER} STREQUAL "log4cplus")
  # log4cplus src should have been download to somewhere
  if(NOT logger_SRC)
    # It should be here if the outer build ran
    set(logger_SRC ${CMAKE_BINARY_DIR}/../log4cplus/src/log4cplus)
  endif()
  set(log4cplus_HEADER ${logger_SRC}/include/log4cplus/logger.h)
  if(NOT EXISTS ${log4cplus_HEADER})
    message(FATAL_ERROR "I cannot find log4cplus source, please set logger_SRC to its root directory")
  endif()
  list(APPEND LOGGER_INCLUDE_DIR ${logger_SRC}/include)
  list(APPEND LOGGER_INCLUDE_DIR ${logger_SRC}-build/include)
  # Setting for log4cplus configuration
  set(UNICODE OFF CACHE BOOL INTERNAL FORCE)
  set(LOG4CPLUS_SINGLE_THREADED OFF CACHE BOOL INTERNAL FORCE)
  set(LOG4CPLUS_BUILD_LOGGINGSERVER OFF CACHE BOOL INTERNAL FORCE)
  set(LOG4CPLUS_BUILD_TESTING OFF CACHE BOOL INTERNAL FORCE)
  set(LOG4CPLUS_ENABLE_DECORATED_LIBRARY_NAME OFF CACHE BOOL INTERNAL FORCE)
  set(WITH_UNIT_TESTS OFF CACHE BOOL INTERNAL FORCE)
  # Get log4cplus to install to a location so we can customize the install for Pulse
  add_subdirectory(${logger_SRC} ${logger_SRC}-build)
  set(logger_lib log4cplus)
endif()

if(PULSE_PYTHON_BINDINGS)
  if(NOT pybind11_DIR)
    set(pybind11_DIR ${CMAKE_BINARY_DIR}/../pybind11/install/share/cmake/pybind11)
  endif()
  find_package(pybind11 CONFIG REQUIRED)
endif()


# Include the rest of the Pulse projects
add_subdirectory(schema)
add_subdirectory(cdm)
add_subdirectory(engine)
add_subdirectory(test)
add_subdirectory(sdk)
add_subdirectory(verification)
# Various interfaces for different languages
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseC.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseCLR.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseJNI.cmake)
if(PULSE_PYTHON_BINDINGS)
  include(${CMAKE_CURRENT_SOURCE_DIR}/PulsePython.cmake)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(COPY ${CMAKE_SOURCE_DIR}/bin DESTINATION ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/bin/run.cmake.in ${CMAKE_INSTALL_PREFIX}/bin/run.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/bin/run.config.in ${CMAKE_INSTALL_PREFIX}/bin/run.config @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/docs/Doxygen/full.doxy.in ${CMAKE_INSTALL_PREFIX}/bin/docs/full.doxy @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/cmake/PulseConfig.cmake.in ${CMAKE_INSTALL_PREFIX}/PulseConfig.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/bin/Rebase.py.in ${CMAKE_INSTALL_PREFIX}/bin/Rebase.py @ONLY)

# Pulse Testing
enable_testing()
add_test(NAME runCDMUnitTests
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=CDMUnitTests -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
add_test(NAME runBGEUnitTests
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=EngineUnitTests -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
add_test(NAME runSystemValidation
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=SystemValidation -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
add_test(NAME runPatientValidation
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=PatientValidation -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
add_test(NAME runVerification
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=VerificationScenarios -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
add_test(NAME runDrugValidation
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=ValidationDrugs -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
add_test(NAME runLongVerification
  COMMAND ${CMAKE_COMMAND} -DTYPE:STRING=LongVerificationScenarios -P ${CMAKE_BINARY_DIR}/install/bin/run.cmake
  WORKING_DIRECTORY ${INSTALL_BIN})
# TODO Make Doc generation a test too!
