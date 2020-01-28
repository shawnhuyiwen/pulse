# Files in the project 

include(${CMAKE_CURRENT_SOURCE_DIR}/GenerateCDMBindings.cmake)

set(BIND_FILES
# cdm bindings
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Actions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Actions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachine.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachine.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachineActions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachineActions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Circuit.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Circuit.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Compartment.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Compartment.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Conditions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Conditions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/ElectroCardioGram.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/ElectroCardioGram.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Engine.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Engine.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Enums.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Enums.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Environment.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Environment.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentActions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentActions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentConditions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentConditions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Events.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Events.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Inhaler.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Inhaler.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/InhalerActions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/InhalerActions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Patient.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Patient.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientActions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientActions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientAssessments.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientAssessments.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientConditions.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientConditions.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientNutrition.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientNutrition.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Physiology.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Physiology.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Properties.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Properties.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Scenario.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Scenario.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Substance.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Substance.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/SubstanceQuantity.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/SubstanceQuantity.pb.h"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/TestReport.pb.cc"
  "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/TestReport.pb.h"
# pulse bindings
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/Pulse.pb.h"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/Pulse.pb.cc"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseConfiguration.pb.h"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseConfiguration.pb.cc"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEnvironment.pb.h"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEnvironment.pb.cc"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEquipment.pb.h"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEquipment.pb.cc"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulsePhysiology.pb.h"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulsePhysiology.pb.cc"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseState.pb.h"
  "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseState.pb.cc"
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
