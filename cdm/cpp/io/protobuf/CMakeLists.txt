set( _cur_dir cpp/io/protobuf)

set( IO_PB_FILES ${_cur_dir}/PBUtils.h
                 ${_cur_dir}/PBUtils.cpp
                 )
source_group("IO\\Protobuf" FILES ${IO_PB_FILES})
list(APPEND SOURCE ${IO_PB_FILES})

set( IO_PB_CDM_FILES ${_cur_dir}/cdm/PBActions.h
                     ${_cur_dir}/cdm/PBActions.cpp
                     ${_cur_dir}/cdm/PBActionEnums.cpp
                     ${_cur_dir}/cdm/PBAnesthesiaMachine.h
                     ${_cur_dir}/cdm/PBAnesthesiaMachine.cpp
                     ${_cur_dir}/cdm/PBAnesthesiaMachineActions.h
                     ${_cur_dir}/cdm/PBAnesthesiaMachineActions.cpp
                     ${_cur_dir}/cdm/PBAnesthesiaMachineEnums.cpp
                     ${_cur_dir}/cdm/PBCircuit.h
                     ${_cur_dir}/cdm/PBCircuit.cpp
                     ${_cur_dir}/cdm/PBCompartment.h
                     ${_cur_dir}/cdm/PBCompartment.cpp
                     ${_cur_dir}/cdm/PBCompartmentEnums.cpp
                     ${_cur_dir}/cdm/PBConditions.h
                     ${_cur_dir}/cdm/PBConditions.cpp
                     ${_cur_dir}/cdm/PBElectroCardioGram.h
                     ${_cur_dir}/cdm/PBElectroCardioGram.cpp
                     ${_cur_dir}/cdm/PBElectroCardioGramEnums.cpp
                     ${_cur_dir}/cdm/PBEngine.h
                     ${_cur_dir}/cdm/PBEngine.cpp
                     ${_cur_dir}/cdm/PBEnums.cpp
                     ${_cur_dir}/cdm/PBEnvironment.h
                     ${_cur_dir}/cdm/PBEnvironment.cpp
                     ${_cur_dir}/cdm/PBEnvironmentActions.h
                     ${_cur_dir}/cdm/PBEnvironmentActions.cpp
                     ${_cur_dir}/cdm/PBEnvironmentConditions.h
                     ${_cur_dir}/cdm/PBEnvironmentConditions.cpp
                     ${_cur_dir}/cdm/PBEnvironmentEnums.cpp
                     ${_cur_dir}/cdm/PBInhaler.h
                     ${_cur_dir}/cdm/PBInhaler.cpp
                     ${_cur_dir}/cdm/PBInhalerActions.h
                     ${_cur_dir}/cdm/PBInhalerActions.cpp
                     ${_cur_dir}/cdm/PBPatient.h
                     ${_cur_dir}/cdm/PBPatient.cpp
                     ${_cur_dir}/cdm/PBPatientEnums.cpp
                     ${_cur_dir}/cdm/PBPatientActions.cpp
                     ${_cur_dir}/cdm/PBPatientActions.h
                     ${_cur_dir}/cdm/PBPatientActionEnums.cpp
                     ${_cur_dir}/cdm/PBPatientAssessments.cpp
                     ${_cur_dir}/cdm/PBPatientAssessments.h
                     ${_cur_dir}/cdm/PBPatientAssessmentEnums.cpp
                     ${_cur_dir}/cdm/PBPatientConditions.cpp
                     ${_cur_dir}/cdm/PBPatientConditions.h
                     ${_cur_dir}/cdm/PBPatientNutrition.h
                     ${_cur_dir}/cdm/PBPatientNutrition.cpp
                     ${_cur_dir}/cdm/PBPhysiology.h
                     ${_cur_dir}/cdm/PBPhysiology.cpp
                     ${_cur_dir}/cdm/PBPhysiologyEnums.cpp
                     ${_cur_dir}/cdm/PBProperties.h
                     ${_cur_dir}/cdm/PBProperties.cpp
                     ${_cur_dir}/cdm/PBScenario.h
                     ${_cur_dir}/cdm/PBScenario.cpp
                     ${_cur_dir}/cdm/PBScenarioEnums.cpp
                     ${_cur_dir}/cdm/PBSubstance.h
                     ${_cur_dir}/cdm/PBSubstance.cpp
                     ${_cur_dir}/cdm/PBSubstanceEnums.cpp
                     ${_cur_dir}/cdm/PBSubstanceQuantity.h
                     ${_cur_dir}/cdm/PBSubstanceQuantity.cpp
                     ${_cur_dir}/cdm/PBTestReport.h
                     ${_cur_dir}/cdm/PBTestReport.cpp
                     )
source_group("IO\\Protobuf\\CDM" FILES ${IO_PB_CDM_FILES})
list(APPEND SOURCE ${IO_PB_CDM_FILES})

set( IO_PB_PULSE_FILES ${_cur_dir}/pulse/PBPulse.h
                       ${_cur_dir}/pulse/PBPulse.cpp
                       ${_cur_dir}/pulse/PBPulseEnums.cpp
                       ${_cur_dir}/pulse/PBPulseConfiguration.h
                       ${_cur_dir}/pulse/PBPulseConfiguration.cpp
                       ${_cur_dir}/pulse/PBPulseEnvironment.h
                       ${_cur_dir}/pulse/PBPulseEnvironment.cpp
                       ${_cur_dir}/pulse/PBPulseEquipment.h
                       ${_cur_dir}/pulse/PBPulseEquipment.cpp
                       ${_cur_dir}/pulse/PBPulsePhysiology.h
                       ${_cur_dir}/pulse/PBPulsePhysiology.cpp
                       ${_cur_dir}/pulse/PBPulseState.h
                       ${_cur_dir}/pulse/PBPulseState.cpp
                       )
source_group("IO\\Protobuf\\Pulse" FILES ${IO_PB_PULSE_FILES})
list(APPEND SOURCE ${IO_PB_PULSE_FILES})
