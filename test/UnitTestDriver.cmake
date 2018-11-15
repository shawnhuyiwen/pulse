# Files in the project (Relative to this CMAKE file)
source_group("" FILES driver/cpp/main.cpp)
# The DLL we are building
add_executable(UnitTestDriver driver/cpp/main.cpp)

target_include_directories(UnitTestDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cdm/cpp)
target_include_directories(UnitTestDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../cdm/cpp)
target_include_directories(UnitTestDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/cpp)
target_include_directories(UnitTestDriver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../engine/cpp)
target_include_directories(UnitTestDriver PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(UnitTestDriver PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(UnitTestDriver PRIVATE ${PROTOBUF_INCLUDE_DIR})
target_link_libraries(UnitTestDriver CommonDataModelUnitTests PulseEngineUnitTests)


set_target_properties(UnitTestDriver PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

add_custom_command(TARGET UnitTestDriver POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:UnitTestDriver> ${INSTALL_BIN})
