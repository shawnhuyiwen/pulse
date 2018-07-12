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
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "RelWithDebInfo" CACHE TYPE INTERNAL FORCE )
if(MSVC)  
# Using MD as that seems to be what I run into alot, you could change these to /MT and /MTd if you want...
  set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob2 /Oi /Od /RTC1" CACHE TYPE INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "/MD" CACHE TYPE INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MDd /Zi" CACHE TYPE INTERNAL FORCE)
endif()

if(MINGW)

endif()

if(APPLE)

endif()

if(UNIX)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN")
endif()



message(STATUS "Looking for modules here : ${CMAKE_PREFIX_PATH}")
set(CMAKE_CXX_STANDARD_LIBRARIES "" CACHE TYPE INTERNAL FORCE)
set(CMAKE_C_STANDARD_LIBRARIES "" CACHE TYPE INTERNAL FORCE)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})

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
if(NOT protobuf_INSTALL)
  set(protobuf_INSTALL ${CMAKE_BINARY_DIR}/../protobuf/install)
endif()
list(APPEND CMAKE_PREFIX_PATH ${protobuf_INSTALL})
# log4cplus src should have been download to somewhere
if(NOT log4cplus_SRC)
  # It should be here if the outer build ran
  set(log4cplus_SRC ${CMAKE_BINARY_DIR}/../log4cplus/src/log4cplus)
endif()
set(log4cplus_HEADER ${log4cplus_SRC}/include/log4cplus/logger.h)
if(NOT EXISTS ${log4cplus_HEADER})
  message(FATAL_ERROR "I cannot find log4cplus source, please set log4cplus_SRC to its root directory")
endif()
list(APPEND LOG4CPLUS_INCLUDE_DIR ${log4cplus_SRC}/include)
list(APPEND LOG4CPLUS_INCLUDE_DIR ${log4cplus_SRC}-build/include)

set(SCHEMA_SRC "${CMAKE_SOURCE_DIR}/schema")
set(SCHEMA_DST "${CMAKE_BINARY_DIR}/schema")
# Settings for protobuf configuration
set(protobuf_BUILD_SHARED_LIBS OFF CACHE TYPE INTERNAL FORCE)
set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE TYPE INTERNAL FORCE)#Use our MSVC runtime settings (/MD or /MT)
set(protobuf_BUILD_TESTS OFF CACHE TYPE INTERNAL FORCE)
set(protobuf_BUILD_EXAMPLES OFF CACHE TYPE INTERNAL FORCE)
add_subdirectory("${protobuf_SRC}/cmake" "${protobuf_SRC}-build")
# Setting for log4cplus configuration
set(UNICODE OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_SINGLE_THREADED OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_BUILD_LOGGINGSERVER OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_BUILD_TESTING OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_ENABLE_DECORATED_LIBRARY_NAME OFF CACHE TYPE INTERNAL FORCE)
set(WITH_UNIT_TESTS OFF CACHE TYPE INTERNAL FORCE)
# Get log4cplus to install to a location so we can customize the install for Pulse
add_subdirectory(${log4cplus_SRC} ${log4cplus_SRC}-build)
# Include the rest of the Pulse projects
add_subdirectory(schema)
add_subdirectory(cdm)
add_subdirectory(engine)
add_subdirectory(test)
add_subdirectory(sdk)
add_subdirectory(verification)
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseJNI.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseCLR.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties (libprotobuf libprotobuf-lite libprotoc protoc js_embed PROPERTIES FOLDER protobufs)


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
