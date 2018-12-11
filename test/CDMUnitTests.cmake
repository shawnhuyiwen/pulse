# Files in the project (Relative to this CMAKE file)
file(GLOB CDM_TEST_FILES
  "cdm/cpp/*.h"
  "cdm/cpp/*.cpp"
)
set(SOURCE ${CDM_TEST_FILES})
source_group("" FILES ${CDM_TEST_FILES})

# The DLL we are building
add_library(CommonDataModelUnitTests ${SOURCE})
# Preprocessor Definitions and Include Paths
set(CDM_TEST_FLAGS)
if(${BUILD_SHARED_LIBS}) 
  set(CDM_TEST_FLAGS "${CDM_TEST_FLAGS} -D SHARED_CDM_TEST")
endif()
target_include_directories(CommonDataModelUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cdm/cpp)
target_include_directories(CommonDataModelUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(CommonDataModelUnitTests PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(CommonDataModelUnitTests PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(CommonDataModelUnitTests PRIVATE ${PROTOBUF_INCLUDE_DIR})
set_target_properties(CommonDataModelUnitTests PROPERTIES COMPILE_FLAGS "${CDM_TEST_FLAGS}" PREFIX "")

IF(APPLE)
    set_target_properties(CommonDataModelUnitTests PROPERTIES MACOSX_RPATH ON)
ENDIF()

set_target_properties(CommonDataModelUnitTests PROPERTIES
    OUTPUT_NAME ${LIB_PREFIX}CommonDataModelUnitTests
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

# Dependent Libraries
target_link_libraries(CommonDataModelUnitTests CommonDataModel)

if(${BUILD_SHARED_LIBS})
  add_custom_command(TARGET CommonDataModelUnitTests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CommonDataModelUnitTests> ${INSTALL_BIN})
endif()

install(TARGETS CommonDataModelUnitTests
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})