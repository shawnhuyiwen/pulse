# Files in the project (Relative to this CMAKE file)
set(PULSE_FILES
  "cpp/PulseConfiguration.h"
  "cpp/PulseConfiguration.cpp"
  "cpp/PulsePhysiologyEngine.h"
  "cpp/PulsePhysiologySystems.h"
  "cpp/PulseScenario.h"
  "cpp/PulseScenario.cpp"
)
source_group("" FILES ${PULSE_FILES})
set(SOURCE ${PULSE_FILES})

set(PULSE_CONTROLLER_FILES
  "cpp/controller/Circuits.h"
  "cpp/controller/Circuits.cpp"
  "cpp/controller/Compartments.h"
  "cpp/controller/Compartments.cpp"
  "cpp/controller/Controller.h"
  "cpp/controller/Controller.cpp"
  "cpp/controller/Engine.h"
  "cpp/controller/Engine.cpp"
  "cpp/controller/ScenarioExec.h"
  "cpp/controller/ScenarioExec.cpp"
  "cpp/controller/Substances.h"
  "cpp/controller/Substances.cpp"
  "cpp/controller/System.h"
)
list(APPEND SOURCE ${PULSE_CONTROLLER_FILES})
source_group("Controller" FILES ${PULSE_CONTROLLER_FILES})

set(PULSE_ENVIRONMENT_FILES
  "cpp/environment/Environment.h"
  "cpp/environment/Environment.cpp"
)
list(APPEND SOURCE ${PULSE_ENVIRONMENT_FILES})
source_group("Environment" FILES ${PULSE_ENVIRONMENT_FILES})

set(PULSE_EQUIPMENT_FILES
  "cpp/equipment/AnesthesiaMachine.h"
  "cpp/equipment/AnesthesiaMachine.cpp"
  "cpp/equipment/ECG.h"
  "cpp/equipment/ECG.cpp"
  "cpp/equipment/Inhaler.h"
  "cpp/equipment/Inhaler.cpp"
)
list(APPEND SOURCE ${PULSE_EQUIPMENT_FILES})
source_group("Equipment" FILES ${PULSE_EQUIPMENT_FILES})

set(PULSE_PHYSIOLOGY_FILES
  "cpp/physiology/BloodChemistry.h"
  "cpp/physiology/BloodChemistry.cpp"
  "cpp/physiology/Cardiovascular.h"
  "cpp/physiology/Cardiovascular.cpp"
  "cpp/physiology/Drugs.h"
  "cpp/physiology/Drugs.cpp"
  "cpp/physiology/Endocrine.h"
  "cpp/physiology/Endocrine.cpp"
  "cpp/physiology/Energy.h"
  "cpp/physiology/Energy.cpp"
  "cpp/physiology/Gastrointestinal.h"
  "cpp/physiology/Gastrointestinal.cpp"
  "cpp/physiology/Hepatic.h"
  "cpp/physiology/Hepatic.cpp"
  "cpp/physiology/Nervous.h"
  "cpp/physiology/Nervous.cpp"
  "cpp/physiology/Renal.h"
  "cpp/physiology/Renal.cpp"
  "cpp/physiology/Respiratory.h"
  "cpp/physiology/Respiratory.cpp"
  "cpp/physiology/Saturation.h"
  "cpp/physiology/Saturation.cpp"
  "cpp/physiology/Tissue.h"
  "cpp/physiology/Tissue.cpp"
)
list(APPEND SOURCE ${PULSE_PHYSIOLOGY_FILES})
source_group("Physiology" FILES ${PULSE_PHYSIOLOGY_FILES})

# I/O
set( IO_PB_PULSE_FILES 
  "cpp/io/protobuf/PBPulse.h"
  "cpp/io/protobuf/PBPulse.cpp"
  "cpp/io/protobuf/PBPulseEnums.cpp"
  "cpp/io/protobuf/PBPulseConfiguration.h"
  "cpp/io/protobuf/PBPulseConfiguration.cpp"
  "cpp/io/protobuf/PBPulseEnvironment.h"
  "cpp/io/protobuf/PBPulseEnvironment.cpp"
  "cpp/io/protobuf/PBPulseEquipment.h"
  "cpp/io/protobuf/PBPulseEquipment.cpp"
  "cpp/io/protobuf/PBPulsePhysiology.h"
  "cpp/io/protobuf/PBPulsePhysiology.cpp"
  "cpp/io/protobuf/PBPulseState.h"
  "cpp/io/protobuf/PBPulseState.cpp"
)
source_group("IO\\Protobuf" FILES ${IO_PB_PULSE_FILES})
list(APPEND SOURCE ${IO_PB_PULSE_FILES})

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
