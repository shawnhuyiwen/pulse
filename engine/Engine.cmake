# Files in the project (Relative to this CMAKE file)
file(GLOB PULSE_FILES
  "cpp/*.h"
  "cpp/*.cpp"
)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulseEngineJNI.h)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulseEngineJNI.cpp)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulseScenarioDriver.cpp)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulseScenarioDriver.h)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/Verification.cpp)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/Verification.h)
list(REMOVE_ITEM PULSE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/stdafx.cpp)#PreCompiled Header Generator, will add later
source_group("" FILES ${PULSE_FILES})
set(SOURCE ${PULSE_FILES})

file(GLOB PULSE_CONTROLLER_FILES
  "cpp/controller/*.h"
  "cpp/controller/*.cpp"
)
list(APPEND SOURCE ${PULSE_CONTROLLER_FILES})
source_group("Controller" FILES ${PULSE_CONTROLLER_FILES})

file(GLOB PULSE_SCENARIO_FILES
  "cpp/controller/Scenario/*.h"
  "cpp/controller/Scenario/*.cpp"
)
list(APPEND SOURCE ${PULSE_SCENARIO_FILES})
source_group("Controller\\Scenario" FILES ${PULSE_SCENARIO_FILES})

file(GLOB PULSE_ENVIRONMENT_FILES
  "cpp/environment/*.h"
  "cpp/environment/*.cpp"
)
list(APPEND SOURCE ${PULSE_ENVIRONMENT_FILES})
source_group("Environment" FILES ${PULSE_ENVIRONMENT_FILES})

file(GLOB PULSE_EQUIPMENT_FILES
  "cpp/equipment/*.h"
  "cpp/equipment/*.cpp"
)
list(APPEND SOURCE ${PULSE_EQUIPMENT_FILES})
source_group("Equipment" FILES ${PULSE_EQUIPMENT_FILES})

file(GLOB PULSE_PHYSIOLOGY_FILES
  "cpp/physiology/*.h"
  "cpp/physiology/*.cpp"
)
list(APPEND SOURCE ${PULSE_PHYSIOLOGY_FILES})
source_group("Physiology" FILES ${PULSE_PHYSIOLOGY_FILES})

# I/O
include(cpp/io/protobuf/CMakeLists.txt)

# The DLL we are building
add_library(PulseEngine ${SOURCE})
# Preprocessor Definitions and Include Paths
set(ENG_FLAGS)
if(${BUILD_SHARED_LIBS}) 
  set(ENG_FLAGS "${ENG_FLAGS} -D SHARED_PULSE")
endif()
if(MSVC)  
  set(ENG_FLAGS "${ENG_FLAGS} -Zm120")
endif(MSVC)

target_include_directories(PulseEngine PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp)
target_include_directories(PulseEngine PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind)
target_include_directories(PulseEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cpp)
target_include_directories(PulseEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(PulseEngine PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(PulseEngine PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(PulseEngine PRIVATE ${PROTOBUF_INCLUDE_DIR})

set_target_properties(PulseEngine PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(PulseEngine PROPERTIES COMPILE_FLAGS "${ENG_FLAGS}"  PREFIX "")
set_target_properties(PulseEngine PROPERTIES
    OUTPUT_NAME ${LIB_PREFIX}PulseEngine
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

if(APPLE)
    set_target_properties(PulseEngine PROPERTIES MACOSX_RPATH ON)
endif()

# Dependent Libraries
target_link_libraries(PulseEngine CommonDataModel)

if(${BUILD_SHARED_LIBS})
  add_custom_command(TARGET PulseEngine POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PulseEngine> ${INSTALL_BIN})
endif()

install(TARGETS PulseEngine
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulsePhysiologyEngine.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulseConfiguration.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cpp/PulseScenario.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
