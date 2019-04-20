# If you want to build your own CMake project for your code that uses Pulse
# Check out this wiki article to get started : 
# https://gitlab.kitware.com/physiology/engine/wikis/how-to-connect-to-pulse


file(GLOB SRC_FILES howto/cpp/EngineHowTo.cpp
                    howto/cpp/EngineHowTo.h
                    howto/cpp/HowTo-AirwayObstruction.cpp
                    howto/cpp/HowTo-AnesthesiaMachine.cpp
                    howto/cpp/HowTo-Asthma.cpp
                    howto/cpp/HowTo-BolusDrug.cpp
                    howto/cpp/HowTo-BrainInjury.cpp
                    howto/cpp/HowTo-ConcurrentEngines.cpp
                    howto/cpp/HowTo-ConsumeNutrients.cpp
                    howto/cpp/HowTo-COPD.cpp
                    howto/cpp/HowTo-CPR.cpp
                    howto/cpp/HowTo-CreateAPatient.cpp
                    howto/cpp/HowTo-EngineUse.cpp
                    howto/cpp/HowTo-EnvironmentChange.cpp
                    howto/cpp/HowTo-Exercise.cpp
                    howto/cpp/HowTo-Hemorrhage.cpp
                    howto/cpp/HowTo-LobarPneumonia.cpp
                    howto/cpp/HowTo-MechanicalVentilation.cpp
                    howto/cpp/HowTo-PulmonaryFunctionTest.cpp
                    howto/cpp/HowTo-RunScenario.cpp
                    howto/cpp/HowTo-RunThreaded.cpp
                    howto/cpp/HowTo-RunThreaded.h
                    howto/cpp/HowTo-Sandbox.cpp
                    howto/cpp/HowTo-SceanrioBase.cpp
                    howto/cpp/HowTo-Serialize.cpp
                    howto/cpp/HowTo-Smoke.cpp
                    howto/cpp/HowTo-TensionPneumothorax.cpp
)
source_group("" FILES ${SRC_FILES})
set(SOURCE ${SRC_FILES})
add_executable(HowToDriver ${SOURCE})

# Preprocessor Definitions and Include Paths
target_include_directories(HowToDriver PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp)
target_include_directories(HowToDriver PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind)
target_include_directories(HowToDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../engine/cpp)
target_include_directories(HowToDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(HowToDriver PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(HowToDriver PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(HowToDriver PRIVATE ${PROTOBUF_INCLUDE_DIR})

# Dependent Libraries
target_link_libraries(HowToDriver PulseEngine)

set_target_properties(HowToDriver PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

IF(UNIX)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN")
ENDIF()

add_custom_command(TARGET HowToDriver POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:HowToDriver> ${INSTALL_BIN})
                   
if(MSVC) # Configure running executable out of MSVC
  set_property(TARGET HowToDriver PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${INSTALL_BIN}")
endif()
