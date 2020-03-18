# Files in the project 

include(${CMAKE_CURRENT_SOURCE_DIR}/GenerateCDMBindings.cmake)

add_library_ex(DataModelBindings
  H_FILES
# cdm bindings
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Actions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachine.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachineActions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Circuit.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Compartment.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Conditions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/ElectroCardioGram.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Engine.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Enums.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Environment.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentActions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentConditions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Events.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Inhaler.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/InhalerActions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Patient.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientActions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientAssessments.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientConditions.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientNutrition.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Physiology.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Properties.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Scenario.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Substance.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/SubstanceQuantity.pb.h"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/TestReport.pb.h"
# pulse bindings
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/Pulse.pb.h"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseConfiguration.pb.h"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEnvironment.pb.h"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEquipment.pb.h"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulsePhysiology.pb.h"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseState.pb.h"
  CPP_FILES
# cdm bindings
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Actions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachine.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/AnesthesiaMachineActions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Circuit.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Compartment.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/ElectroCardioGram.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Conditions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Engine.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Enums.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Environment.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentActions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/EnvironmentConditions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Events.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Inhaler.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/InhalerActions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Patient.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientActions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientAssessments.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientConditions.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Physiology.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/PatientNutrition.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Properties.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Scenario.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/Substance.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/SubstanceQuantity.pb.cc"
    "${SCHEMA_DST}/cpp/${CDM_PACKAGE}/TestReport.pb.cc"
# pulse bindings
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/Pulse.pb.cc"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseConfiguration.pb.cc"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEnvironment.pb.cc"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseEquipment.pb.cc"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulsePhysiology.pb.cc"
    "${SCHEMA_DST}/cpp/${ENGINE_PACKAGE}/PulseState.pb.cc"
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  GROUP_ROOT
    ${SCHEMA_DST}/cpp
)
target_include_directories(DataModelBindings PRIVATE ${CMAKE_SOURCE_DIR}/schema/bind/cpp)
