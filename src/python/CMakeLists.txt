if(NOT PULSE_PYTHON_BINDINGS)
  return()
endif()

pybind11_add_module(PyPulse
  pybind/PyPulse.cpp)

target_include_directories(PyPulse PRIVATE ${CMAKE_SOURCE_DIR}/engine/cpp)
target_include_directories(PyPulse PRIVATE ${CMAKE_SOURCE_DIR}/cdm/cpp)
target_include_directories(PyPulse PRIVATE ${pybind11_INCLUDE_DIRS})

set(PyPulse_FLAGS)
set_target_properties(PyPulse PROPERTIES COMPILE_FLAGS "${PyPulse_FLAGS}" PREFIX "")

if(APPLE)
    set_target_properties(PyPulse PROPERTIES MACOSX_RPATH ON)
endif()

target_link_libraries(PyPulse
                      PUBLIC PulseEngine pybind11::module
)

set_target_properties(PyPulse PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

add_custom_command(TARGET PyPulse POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PyPulse> ${INSTALL_BIN})

install(TARGETS PyPulse
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})