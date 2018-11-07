
add_library(PulseC SHARED engine/cpp/PulseEngineC.h
                          engine/cpp/PulseEngineC.cpp)

target_include_directories(PulseC PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/)
target_include_directories(PulseC PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind/)
target_include_directories(PulseC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/cpp)
target_include_directories(PulseC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cdm/cpp)
target_include_directories(PulseC PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(PulseC PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(PulseC PRIVATE ${PROTOBUF_INCLUDE_DIR})

set(PulseC_FLAGS)
set_target_properties(PulseC PROPERTIES COMPILE_FLAGS "${PulseC_FLAGS}" PREFIX "")

if(APPLE)
    set_target_properties(PulseC PROPERTIES MACOSX_RPATH ON)
endif()

set_target_properties(PulseC PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

add_custom_command(TARGET PulseC POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PulseC> ${INSTALL_BIN})

install(TARGETS PulseC
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})
