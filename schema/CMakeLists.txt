# Files in the project 

include(${CMAKE_CURRENT_SOURCE_DIR}/GenerateCDMBindings.cmake)

set(BIND_FILES
# cdm bindings
  "${SCHEMA_DST}/cpp/bind/cdm/ActionEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/ActionEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Actions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Actions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/AnesthesiaMachine.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/AnesthesiaMachine.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/AnesthesiaMachineActions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/AnesthesiaMachineActions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/AnesthesiaMachineEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/AnesthesiaMachineEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Circuit.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Circuit.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Compartment.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Compartment.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/CompartmentEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/CompartmentEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Conditions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Conditions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/ElectroCardioGram.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/ElectroCardioGram.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/ElectroCardioGramEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/ElectroCardioGramEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Engine.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Engine.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Enums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Enums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Environment.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Environment.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/EnvironmentActions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/EnvironmentActions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/EnvironmentConditions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/EnvironmentConditions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/EnvironmentEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/EnvironmentEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Inhaler.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Inhaler.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/InhalerActions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/InhalerActions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Patient.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Patient.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientActionEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientActionEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientActions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientActions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientAssessmentEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientAssessmentEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientAssessments.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientAssessments.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientConditions.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientConditions.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientNutrition.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PatientNutrition.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Physiology.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Physiology.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/PhysiologyEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/PhysiologyEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Properties.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Properties.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Scenario.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Scenario.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/ScenarioEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/ScenarioEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/Substance.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/Substance.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/SubstanceEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/SubstanceEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/SubstanceQuantity.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/SubstanceQuantity.pb.h"
  "${SCHEMA_DST}/cpp/bind/cdm/TestReport.pb.cc"
  "${SCHEMA_DST}/cpp/bind/cdm/TestReport.pb.h"
# pulse bindings
  "${SCHEMA_DST}/cpp/bind/pulse/Pulse.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/Pulse.pb.cc"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseConfiguration.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseConfiguration.pb.cc"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseEnvironment.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseEnvironment.pb.cc"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseEquipment.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseEquipment.pb.cc"
  "${SCHEMA_DST}/cpp/bind/pulse/PulsePhysiology.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/PulsePhysiology.pb.cc"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseState.pb.h"
  "${SCHEMA_DST}/cpp/bind/pulse/PulseState.pb.cc"
)
source_group("ProtoBuffers" FILES ${BIND_FILES})
set(SOURCE ${BIND_FILES})

if(NOT SOURCE)
  list(APPEND SOURCE "Error_No_Bindings_Created")
endif()

# The DLL we are building
add_library(DataModelBindings ${SOURCE})
# Preprocessor Definitions and Include Paths
target_include_directories(DataModelBindings PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind)
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
