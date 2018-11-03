if(NOT MSVC)
  return()
endif()

add_library(PulseCLR SHARED engine/clr/PulseEngine.h
                            engine/clr/PulseEngine.cpp)

target_include_directories(PulseCLR PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/)
target_include_directories(PulseCLR PRIVATE ${CMAKE_BINARY_DIR}/schema/cpp/bind/)
target_include_directories(PulseCLR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/cpp)
target_include_directories(PulseCLR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cdm/cpp)
target_include_directories(PulseCLR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/engine/cpp)
target_include_directories(PulseCLR PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/cdm/cpp)
target_include_directories(PulseCLR PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(PulseCLR PRIVATE ${LOG4CPLUS_INCLUDE_DIR})
target_include_directories(PulseCLR PRIVATE ${PROTOBUF_INCLUDE_DIR})

set(PulseCLR_FLAGS "/EHa")
string( REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
set_target_properties(PulseCLR PROPERTIES COMPILE_FLAGS "${PulseCLR_FLAGS}" PREFIX "")
set_target_properties(PulseCLR PROPERTIES COMMON_LANGUAGE_RUNTIME "") # Also turn on CLR
message(STATUS "PulseCLR COMPILE_FLAGS : ${CMAKE_CXX_FLAGS_DEBUG}")

set_target_properties(PulseCLR PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

target_link_libraries(PulseCLR PulseEngine)

add_custom_command(TARGET PulseCLR POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PulseCLR> ${INSTALL_BIN})

                   
if(PULSE_BUILD_CLR)
  install(TARGETS PulseCLR
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})
else()
  set_target_properties(PulseCLR PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()