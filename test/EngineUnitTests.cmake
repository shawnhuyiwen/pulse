# Files in the project (Relative to this CMAKE file)
file(GLOB PULSE_TEST_FILES
  "engine/cpp/*.h"
  "engine/cpp/*.cpp"
)
set(SOURCE ${PULSE_TEST_FILES})
source_group("" FILES ${PULSE_TEST_FILES})
# The DLL we are building
add_library(PulseEngineUnitTests ${SOURCE})
# Preprocessor Definitions and Include Paths
target_include_directories(PulseEngineUnitTests PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp)
target_include_directories(PulseEngineUnitTests PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind)
target_include_directories(PulseEngineUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cdm/cpp)
target_include_directories(PulseEngineUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(PulseEngineUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/cpp)
target_include_directories(PulseEngineUnitTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../engine/cpp)
target_include_directories(PulseEngineUnitTests PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(PulseEngineUnitTests PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(PulseEngineUnitTests PRIVATE ${PROTOBUF_INCLUDE_DIR})

set(ENGINE_TEST_FLAGS)
if(${BUILD_SHARED_LIBS}) 
  set(ENGINE_TEST_FLAGS "${ENGINE_TEST_FLAGS} -D SHARED_ENGINE_TEST")
endif()
set_target_properties(PulseEngineUnitTests PROPERTIES COMPILE_FLAGS "${ENGINE_TEST_FLAGS}" PREFIX "")

IF(APPLE)
    set_target_properties(PulseEngineUnitTests PROPERTIES MACOSX_RPATH ON)
ENDIF()

set_target_properties(PulseEngineUnitTests PROPERTIES
    OUTPUT_NAME ${LIB_PREFIX}PulseEngineUnitTests
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

# Dependent Libraries
target_link_libraries(PulseEngineUnitTests PulseEngine)

if(${BUILD_SHARED_LIBS})
  add_custom_command(TARGET PulseEngineUnitTests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PulseEngineUnitTests> ${INSTALL_BIN})
endif()

install(TARGETS PulseEngineUnitTests
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})