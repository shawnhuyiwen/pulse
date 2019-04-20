# Files in the project
add_executable(PulseScenarioDriver cpp/PulseScenarioDriver.cpp
                                   cpp/PulseScenarioDriver.h)

# Preprocessor Definitions and Include Paths
target_include_directories(PulseScenarioDriver PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp)
target_include_directories(PulseScenarioDriver PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind)
target_include_directories(PulseScenarioDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cpp)
target_include_directories(PulseScenarioDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(PulseScenarioDriver PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(PulseScenarioDriver PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(PulseScenarioDriver PRIVATE ${PROTOBUF_INCLUDE_DIR})

# Dependent Libraries
target_link_libraries(PulseScenarioDriver PulseEngine)

IF(UNIX)
    SET(CMAKE_INSTLL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN")
ENDIF()


set_target_properties(PulseScenarioDriver PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

add_custom_command(TARGET PulseScenarioDriver POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PulseScenarioDriver> ${INSTALL_BIN})
                   
if(MSVC) # Configure running executable out of MSVC
  set_property(TARGET PulseScenarioDriver PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${INSTALL_BIN}")
endif()
