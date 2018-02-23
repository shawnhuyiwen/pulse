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
  "${SCHEMA_DST}/cpp/bind/engine/Engine.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/Engine.pb.cc"
  "${SCHEMA_DST}/cpp/bind/engine/EngineConfiguration.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/EngineConfiguration.pb.cc"
  "${SCHEMA_DST}/cpp/bind/engine/EngineEnums.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/EngineEnums.pb.cc"
  "${SCHEMA_DST}/cpp/bind/engine/EngineEnvironment.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/EngineEnvironment.pb.cc"
  "${SCHEMA_DST}/cpp/bind/engine/EngineEquipment.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/EngineEquipment.pb.cc"
  "${SCHEMA_DST}/cpp/bind/engine/EnginePhysiology.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/EnginePhysiology.pb.cc"
  "${SCHEMA_DST}/cpp/bind/engine/EngineState.pb.h"
  "${SCHEMA_DST}/cpp/bind/engine/EngineState.pb.cc"
)
source_group("" FILES ${BIND_FILES})
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

add_custom_command(TARGET DataModelBindings POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/proto ${INSTALL_BIN}/proto)
add_custom_command(TARGET DataModelBindings POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E touch ${SCHEMA_DST}/schema_last_built)
                   
if(${BUILD_SHARED_LIBS})
  add_custom_command(TARGET DataModelBindings POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_BIN}/${CONFIGURATION}${EX_CONFIG}
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DataModelBindings> ${INSTALL_BIN}/${CONFIGURATION}${EX_CONFIG})
  if(WIN32)# Copy dll files to the bin
    install(TARGETS DataModelBindings 
            RUNTIME CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release${EX_CONFIG}
            LIBRARY CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release${EX_CONFIG})
    install(TARGETS DataModelBindings 
            RUNTIME CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG}
            LIBRARY CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG})
    install(TARGETS DataModelBindings 
            RUNTIME CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG}
            LIBRARY CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG})
  else()# Copy so files to the bin
    install(TARGETS DataModelBindings 
            LIBRARY CONFIGURATIONS Release DESTINATION ${INSTALL_BIN}/release${EX_CONFIG})
    install(TARGETS DataModelBindings 
            LIBRARY CONFIGURATIONS Debug DESTINATION ${INSTALL_BIN}/debug${EX_CONFIG})
    install(TARGETS DataModelBindings 
            LIBRARY CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_BIN}/relwithdebinfo${EX_CONFIG})
  endif()
endif()
# Copy lib/so files to the sdk/lib
install(TARGETS DataModelBindings         
        LIBRARY CONFIGURATIONS Release DESTINATION ${INSTALL_LIB}/release${EX_CONFIG}
        ARCHIVE CONFIGURATIONS Release DESTINATION ${INSTALL_LIB}/release${EX_CONFIG})
install(TARGETS DataModelBindings 
        LIBRARY CONFIGURATIONS Debug DESTINATION ${INSTALL_LIB}/debug${EX_CONFIG}
        ARCHIVE CONFIGURATIONS Debug DESTINATION ${INSTALL_LIB}/debug${EX_CONFIG})
install(TARGETS DataModelBindings  
        LIBRARY CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_LIB}/relwithdebinfo${EX_CONFIG}
        ARCHIVE CONFIGURATIONS RelWithDebInfo DESTINATION ${INSTALL_LIB}/relwithdebinfo${EX_CONFIG})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/proto DESTINATION ${INSTALL_BIN})
install_headers("${CMAKE_BINARY_DIR}/schema/cpp/bind" "bind")
