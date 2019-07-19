# Files in the project 

include(${CMAKE_CURRENT_SOURCE_DIR}/GenerateCDMBindings.cmake)

set(BIND_FILES
# cdm bindings
  "${SCHEMA_DST}/cpp/cdm/Actions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Actions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/AnesthesiaMachine.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/AnesthesiaMachine.pb.h"
  "${SCHEMA_DST}/cpp/cdm/AnesthesiaMachineActions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/AnesthesiaMachineActions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Circuit.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Circuit.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Compartment.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Compartment.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Conditions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Conditions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/ElectroCardioGram.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/ElectroCardioGram.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Engine.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Engine.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Enums.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Enums.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Environment.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Environment.pb.h"
  "${SCHEMA_DST}/cpp/cdm/EnvironmentActions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/EnvironmentActions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/EnvironmentConditions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/EnvironmentConditions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Events.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Events.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Inhaler.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Inhaler.pb.h"
  "${SCHEMA_DST}/cpp/cdm/InhalerActions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/InhalerActions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Patient.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Patient.pb.h"
  "${SCHEMA_DST}/cpp/cdm/PatientActions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/PatientActions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/PatientAssessments.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/PatientAssessments.pb.h"
  "${SCHEMA_DST}/cpp/cdm/PatientConditions.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/PatientConditions.pb.h"
  "${SCHEMA_DST}/cpp/cdm/PatientNutrition.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/PatientNutrition.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Physiology.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Physiology.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Properties.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Properties.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Scenario.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Scenario.pb.h"
  "${SCHEMA_DST}/cpp/cdm/Substance.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/Substance.pb.h"
  "${SCHEMA_DST}/cpp/cdm/SubstanceQuantity.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/SubstanceQuantity.pb.h"
  "${SCHEMA_DST}/cpp/cdm/TestReport.pb.cc"
  "${SCHEMA_DST}/cpp/cdm/TestReport.pb.h"
# pulse bindings
  "${SCHEMA_DST}/cpp/pulse/Pulse.pb.h"
  "${SCHEMA_DST}/cpp/pulse/Pulse.pb.cc"
  "${SCHEMA_DST}/cpp/pulse/PulseConfiguration.pb.h"
  "${SCHEMA_DST}/cpp/pulse/PulseConfiguration.pb.cc"
  "${SCHEMA_DST}/cpp/pulse/PulseEnvironment.pb.h"
  "${SCHEMA_DST}/cpp/pulse/PulseEnvironment.pb.cc"
  "${SCHEMA_DST}/cpp/pulse/PulseEquipment.pb.h"
  "${SCHEMA_DST}/cpp/pulse/PulseEquipment.pb.cc"
  "${SCHEMA_DST}/cpp/pulse/PulsePhysiology.pb.h"
  "${SCHEMA_DST}/cpp/pulse/PulsePhysiology.pb.cc"
  "${SCHEMA_DST}/cpp/pulse/PulseState.pb.h"
  "${SCHEMA_DST}/cpp/pulse/PulseState.pb.cc"
)
source_group("ProtoBuffers" FILES ${BIND_FILES})
set(SOURCE ${BIND_FILES})

if(NOT SOURCE)
  list(APPEND SOURCE "Error_No_Bindings_Created")
endif()

# The DLL we are building
add_library(DataModelBindings ${SOURCE})
# Preprocessor Definitions and Include Paths

target_include_directories(DataModelBindings PRIVATE ${CMAKE_SOURCE_DIR}/schema/bind/cpp)
target_include_directories(DataModelBindings PRIVATE ${CMAKE_SOURCE_DIR}/schema)
target_include_directories(DataModelBindings PRIVATE ${PROTOBUF_INCLUDE_DIR})
set(FLAGS)
set_target_properties(DataModelBindings PROPERTIES COMPILE_FLAGS "${FLAGS}" PREFIX "")

if(APPLE)
    set_target_properties(DataModelBindings PROPERTIES MACOSX_RPATH ON)
endif()
target_link_libraries(DataModelBindings libprotobuf)
if(MSVC)
  target_link_libraries(DataModelBindings ws2_32.lib)
  target_link_libraries(DataModelBindings advapi32.lib)
endif()

set_target_properties(DataModelBindings PROPERTIES
    OUTPUT_NAME ${LIB_PREFIX}DataModelBindings
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

add_custom_command(TARGET DataModelBindings POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E touch ${SCHEMA_DST}/schema_last_built)
                   
if(${BUILD_SHARED_LIBS})
  add_custom_command(TARGET DataModelBindings POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DataModelBindings> ${INSTALL_BIN})
endif()

install(TARGETS DataModelBindings
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})
install_headers("${CMAKE_BINARY_DIR}/schema/cpp/bind" "bind")
