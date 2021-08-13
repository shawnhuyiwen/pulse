# Files in the project 

add_library_ex(DataModelBindings
  H_FILES
# cdm bindings
    "${CDM_PACKAGE}/Actions.pb.h"
    "${CDM_PACKAGE}/AnesthesiaMachine.pb.h"
    "${CDM_PACKAGE}/AnesthesiaMachineActions.pb.h"
    "${CDM_PACKAGE}/BagValveMask.pb.h"
    "${CDM_PACKAGE}/BagValveMaskActions.pb.h"
    "${CDM_PACKAGE}/BlackBox.pb.h"
    "${CDM_PACKAGE}/Circuit.pb.h"
    "${CDM_PACKAGE}/Compartment.pb.h"
    "${CDM_PACKAGE}/Conditions.pb.h"
    "${CDM_PACKAGE}/ElectroCardioGram.pb.h"
    "${CDM_PACKAGE}/Engine.pb.h"
    "${CDM_PACKAGE}/Enums.pb.h"
    "${CDM_PACKAGE}/Environment.pb.h"
    "${CDM_PACKAGE}/EnvironmentActions.pb.h"
    "${CDM_PACKAGE}/EnvironmentConditions.pb.h"
    "${CDM_PACKAGE}/EquipmentActions.pb.h"
    "${CDM_PACKAGE}/Events.pb.h"
    "${CDM_PACKAGE}/Inhaler.pb.h"
    "${CDM_PACKAGE}/InhalerActions.pb.h"
    "${CDM_PACKAGE}/MechanicalVentilator.pb.h"
    "${CDM_PACKAGE}/MechanicalVentilatorActions.pb.h"
    "${CDM_PACKAGE}/Patient.pb.h"
    "${CDM_PACKAGE}/PatientActions.pb.h"
    "${CDM_PACKAGE}/PatientAssessments.pb.h"
    "${CDM_PACKAGE}/PatientConditions.pb.h"
    "${CDM_PACKAGE}/PatientNutrition.pb.h"
    "${CDM_PACKAGE}/Physiology.pb.h"
    "${CDM_PACKAGE}/Properties.pb.h"
    "${CDM_PACKAGE}/Scenario.pb.h"
    "${CDM_PACKAGE}/Substance.pb.h"
    "${CDM_PACKAGE}/SubstanceQuantity.pb.h"
    "${CDM_PACKAGE}/TestReport.pb.h"
# engine bindings
    "${ENGINE_PACKAGE}/Configuration.pb.h"
    "${ENGINE_PACKAGE}/Enums.pb.h"
    "${ENGINE_PACKAGE}/Environment.pb.h"
    "${ENGINE_PACKAGE}/Equipment.pb.h"
    "${ENGINE_PACKAGE}/Physiology.pb.h"
    "${ENGINE_PACKAGE}/Scenario.pb.h"
    "${ENGINE_PACKAGE}/State.pb.h"
# study bindings
    "${STUDY_PACKAGE}/Hydrocephalus.pb.h"
    "${STUDY_PACKAGE}/MultiplexVentilation.pb.h"
    "${STUDY_PACKAGE}/SensitivityAnalysis.pb.h"
  CPP_FILES
# cdm bindings
    "${CDM_PACKAGE}/Actions.pb.cc"
    "${CDM_PACKAGE}/AnesthesiaMachine.pb.cc"
    "${CDM_PACKAGE}/AnesthesiaMachineActions.pb.cc"
    "${CDM_PACKAGE}/BagValveMask.pb.cc"
    "${CDM_PACKAGE}/BagValveMaskActions.pb.cc"
    "${CDM_PACKAGE}/BlackBox.pb.cc"
    "${CDM_PACKAGE}/Circuit.pb.cc"
    "${CDM_PACKAGE}/Compartment.pb.cc"
    "${CDM_PACKAGE}/Conditions.pb.cc"
    "${CDM_PACKAGE}/ElectroCardioGram.pb.cc"
    "${CDM_PACKAGE}/Engine.pb.cc"
    "${CDM_PACKAGE}/Enums.pb.cc"
    "${CDM_PACKAGE}/Environment.pb.cc"
    "${CDM_PACKAGE}/EnvironmentActions.pb.cc"
    "${CDM_PACKAGE}/EnvironmentConditions.pb.cc"
    "${CDM_PACKAGE}/EquipmentActions.pb.cc"
    "${CDM_PACKAGE}/Events.pb.cc"
    "${CDM_PACKAGE}/Inhaler.pb.cc"
    "${CDM_PACKAGE}/InhalerActions.pb.cc"
    "${CDM_PACKAGE}/MechanicalVentilator.pb.cc"
    "${CDM_PACKAGE}/MechanicalVentilatorActions.pb.cc"
    "${CDM_PACKAGE}/Patient.pb.cc"
    "${CDM_PACKAGE}/PatientActions.pb.cc"
    "${CDM_PACKAGE}/PatientAssessments.pb.cc"
    "${CDM_PACKAGE}/PatientConditions.pb.cc"
    "${CDM_PACKAGE}/PatientNutrition.pb.cc"
    "${CDM_PACKAGE}/Physiology.pb.cc"
    "${CDM_PACKAGE}/Properties.pb.cc"
    "${CDM_PACKAGE}/Scenario.pb.cc"
    "${CDM_PACKAGE}/Substance.pb.cc"
    "${CDM_PACKAGE}/SubstanceQuantity.pb.cc"
    "${CDM_PACKAGE}/TestReport.pb.cc"
# engine bindings
    "${ENGINE_PACKAGE}/Configuration.pb.cc"
    "${ENGINE_PACKAGE}/Enums.pb.cc"
    "${ENGINE_PACKAGE}/Environment.pb.cc"
    "${ENGINE_PACKAGE}/Equipment.pb.cc"
    "${ENGINE_PACKAGE}/Physiology.pb.cc"
    "${ENGINE_PACKAGE}/Scenario.pb.cc"
    "${ENGINE_PACKAGE}/State.pb.cc"
# study bindings
    "${STUDY_PACKAGE}/Hydrocephalus.pb.cc"
    "${STUDY_PACKAGE}/MultiplexVentilation.pb.cc"
    "${STUDY_PACKAGE}/SensitivityAnalysis.pb.cc"
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  LIB_INSTALL_ONLY
)

if (MSVC)
    target_compile_options(DataModelBindings PRIVATE  "/wd4100") # 'identifier' : unreferenced formal parameter
    target_compile_options(DataModelBindings PRIVATE  "/wd4125") # decimal digit terminates octal escape sequence
    target_compile_options(DataModelBindings PRIVATE  "/wd4127") # conditional expression is constant
    target_compile_options(DataModelBindings PRIVATE  "/wd4244") # 'conversion' conversion from 'type1' to 'type2', possible loss of data
    target_compile_options(DataModelBindings PRIVATE  "/wd4267") # 'var' : conversion from 'size_t' to 'type', possible loss of data
endif()