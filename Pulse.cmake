project(Pulse)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Policy to address @foo@ variable expansion
set(PULSE_VERSION_MAJOR 1)
set(PULSE_VERSION_MINOR 0)
set(PULSE_VERSION_PATCH 0)
set(PULSE_VERSION "${PULSE_VERSION_MAJOR}.${PULSE_VERSION_MINOR}.${PULSE_VERSION_PATCH}")

# Build the core libraries as shared or static (DataModel, CDM, Engine)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CONFIGURATION ${CMAKE_CFG_INTDIR})


set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo CACHE TYPE INTERNAL FORCE )
if(MSVC)  
# Using MD as that seems to be what I run into alot, you could change these to /MT and /MTd if you want...
  set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Zi /Ob2 /Oi /Od /RTC1" CACHE TYPE INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "/MD" CACHE TYPE INTERNAL FORCE)
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MDd /Zi" CACHE TYPE INTERNAL FORCE)
  set(CONFIGURATION "$(Configuration)")
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

if(NOT eigen_INSTALL)
  set(eigen_INSTALL ${CMAKE_BINARY_DIR}/../install/include)
endif()
list(APPEND CMAKE_PREFIX_PATH ${eigen_INSTALL})
find_package(Eigen3 REQUIRED)

if(WIN32)
  if(NOT dirent_INSTALL)
    set(dirent_INSTALL ${CMAKE_BINARY_DIR}/../dirent/install)
  endif()
  list(APPEND CMAKE_PREFIX_PATH ${dirent_INSTALL})
  find_package(Dirent REQUIRED)
endif()

# The outer build does some custom installing of dependent libraries
# Instead of using find_package, I will make sure things are where expected
if(NOT protobuf_DIR)
  set(protobuf_DIR ${CMAKE_BINARY_DIR}/../protobuf/src/protobuf)
  set(protobuf_Header ${protobuf_DIR}/src/google/protobuf/package_info.h)
  # Proto headers should have been installed here by the outer build
  if(NOT EXISTS ${protobuf_Header})
    message(FATAL_ERROR "I cannot find protobuf header, ${protobuf_Header}, make sure you make the install target of the super build")
  endif()
endif()

# log4cplus src should have been download to somewhere
if(NOT log4cplus_DIR)
  # It should be here if the outer build ran
  set(log4cplus_DIR ${CMAKE_BINARY_DIR}/../log4cplus/src/log4cplus)
  set(log4cplus_HEADER ${log4cplus_DIR}/include/log4cplus/logger.h)
  if(NOT EXISTS ${log4cplus_HEADER})
    message(FATAL_ERROR "I cannot find log4cplus header, ${log4cplus_HEADER}, make sure you make the install target of the super build")
  endif()
endif()
list(APPEND LOG4CPLUS_INCLUDE_DIR ${log4cplus_DIR}/include)
list(APPEND LOG4CPLUS_INCLUDE_DIR ${log4cplus_DIR}-build/include)

list(APPEND CMAKE_PREFIX_PATH ${protobuf_INSTALL})
list(APPEND CMAKE_PREFIX_PATH ${log4cplus_INSTALL})

set(SCHEMA_SRC "${CMAKE_SOURCE_DIR}/schema")
set(SCHEMA_DST "${CMAKE_BINARY_DIR}/schema")
# Settings for protobuf configuration
set(protobuf_BUILD_SHARED_LIBS OFF CACHE TYPE INTERNAL FORCE)
set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE TYPE INTERNAL FORCE)#Use our MSVC runtime settings (/MD or /MT)
set(protobuf_BUILD_TESTS OFF CACHE TYPE INTERNAL FORCE)
set(protobuf_BUILD_EXAMPLES OFF CACHE TYPE INTERNAL FORCE)
add_subdirectory("${protobuf_DIR}/cmake" "${protobuf_DIR}-build" EXCLUDE_FROM_ALL)
# Setting for log4cplus configuration
set(UNICODE OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_BUILD_LOGGINGSERVER OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_BUILD_TESTING OFF CACHE TYPE INTERNAL FORCE)
set(LOG4CPLUS_ENABLE_DECORATED_LIBRARY_NAME OFF CACHE TYPE INTERNAL FORCE)
set(WITH_UNIT_TESTS OFF CACHE TYPE INTERNAL FORCE)
# Get log4cplus to install to a location so we can customize the install for Pulse
add_subdirectory(${log4cplus_DIR} ${log4cplus_DIR}-build)
# Include the rest of the Pulse projects
add_subdirectory(schema)
add_subdirectory(cdm)
add_subdirectory(engine)
add_subdirectory(test)
add_subdirectory(sdk)
add_subdirectory(verification)
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseJNI.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/PulseCLR.cmake)
# Take some projects out of the build
set_target_properties(libprotobuf-lite libprotoc protoc PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties (libprotobuf libprotobuf-lite libprotoc protoc js_embed PROPERTIES FOLDER protobufs)


file(COPY ${CMAKE_SOURCE_DIR}/bin DESTINATION ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/bin/run.cmake.in ${CMAKE_INSTALL_PREFIX}/bin/run.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/bin/run.config.in ${CMAKE_INSTALL_PREFIX}/bin/run.config @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/docs/Doxygen/full.doxy.in ${CMAKE_INSTALL_PREFIX}/bin/docs/full.doxy @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/cmake/PulseConfig.cmake.in ${CMAKE_INSTALL_PREFIX}/PulseConfig.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/bin/Rebase.py.in ${CMAKE_INSTALL_PREFIX}/bin/Rebase.py @ONLY)

# Install Eigen
install(DIRECTORY ${eigen_INSTALL}/include
        DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${eigen_INSTALL}/share/eigen3/cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX})
# Install Dirent
if(WIN32)
  install(DIRECTORY ${dirent_INSTALL}/include
          DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
# Install Protobuf
install(DIRECTORY ${protobuf_INSTALL}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
if(WIN32)
  if(BUILD_SHARED_LIBS)
    install(FILES "${protobuf_DIR}-build/Release/libprotobuf.dll"
      CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release)  
    install(FILES "${protobuf_DIR}-build/Debug/libprotobufd.dll"
      CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug)
    install(FILES "${protobuf_DIR}-build/RelWithDebInfo/libprotobuf.dll"
      CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo)
  endif()

  install(FILES "${protobuf_DIR}-build/Release/libprotobuf.lib"
    CONFIGURATIONS Release DESTINATION ${INSTALL_LIB}/release)
  install(FILES "${protobuf_DIR}-build/Debug/libprotobufd.lib"
    CONFIGURATIONS Debug DESTINATION ${INSTALL_LIB}/debug)
  install(FILES "${protobuf_DIR}-build/RelWithDebInfo/libprotobuf.lib"
    CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_LIB}/relwithdebinfo)
else()
  if(BUILD_SHARED_LIBS)
    install(FILES ${protobuf_DIR}-build/libprotobuf.so
      CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release)  
    install(FILES ${protobuf_DIR}-build/libprotobufd.so
      CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug)
    install(FILES ${protobuf_DIR}-build/libprotobuf.so
      CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo)
  endif()

  
  install(FILES ${protobuf_DIR}-build/libprotobuf.a
    CONFIGURATIONS Release DESTINATION ${INSTALL_LIB}/release)
  install(FILES ${protobuf_DIR}-build/libprotobufd.a
    CONFIGURATIONS Debug DESTINATION ${INSTALL_LIB}/debug)
  install(FILES ${protobuf_DIR}-build/libprotobuf.a
    CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_LIB}/relwithdebinfo)  
endif()

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
