
set(from "${SCHEMA_SRC}/proto")
set(to   "${SCHEMA_DST}")
if( protobuf_DIR )
  set(BINDER "${protobuf_DIR}/bin/protoc")
else()#
  message(FATAL_ERROR "Where is the protoc binder?")
endif()

if( NOT protobuf_SRC )
  message(FATAL_ERROR "Where is the protoc source?")
endif()


message(STATUS "Generating Schema Bindings" )
message(STATUS "Using : ${BINDER}")
message(STATUS "From ${from}")
message(STATUS "To ${to}")

set(CDM_PACKAGE "pulse/cdm/bind")
set(ENGINE_PACKAGE "pulse/engine/bind")
file(GLOB_RECURSE _FILES "${from}/*.proto")

set(_RUN_PROTOC OFF)
foreach(f ${_FILES})
  if(${f} IS_NEWER_THAN ${to}/schema_last_built)
    message(STATUS "${f} has changed since the last build")
    set(_RUN_PROTOC ON)
  endif()
endforeach()

if(NOT _RUN_PROTOC)
  message(STATUS "Not generating bindings, nothing has changed since last build")
  return()
endif()

##################
## C++ Bindings ##
##################

# Remove all previously generated files
file(GLOB_RECURSE OLD_FILES "${to}/*.h" "${to}/*.cc" "${to}/*.cs" "${to}/*.java")
file(REMOVE "${OLD_FILES}")

set(cpp_bindings_DIR "${to}/cpp")
file(MAKE_DIRECTORY "${cpp_bindings_DIR}")
file(GLOB_RECURSE _OLD_CPP_FILES "${cpp_bindings_DIR}/*.*")
if(_OLD_CPP_FILES)
  file(REMOVE ${_OLD_CPP_FILES})
endif() 
foreach(f ${_FILES})
  message(STATUS "C++ Binding file ${f}")
  execute_process(COMMAND ${BINDER} --proto_path=${from}
                                    --cpp_out=${cpp_bindings_DIR}
                                    # or 
                                    #--cpp_out=dllexport_decl=_DECL:${cpp_bindings_DIR}
                                    ${f})
endforeach()
message(STATUS "cpp bindings are here : ${cpp_bindings_DIR}" )

###################
## Java Bindings ##
###################

set(java_bindings_DIR "${to}/java")
file(MAKE_DIRECTORY "${java_bindings_DIR}")
file(GLOB_RECURSE _OLD_FILES "${java_bindings_DIR}/*.*")
if(_OLD_FILES)
  file(REMOVE ${_OLD_FILES})
endif()
file(MAKE_DIRECTORY "${to}/build")
file(GLOB_RECURSE _OLD_FILES "${to}/build/*.*")
if(_OLD_FILES)
  file(REMOVE ${_OLD_FILES})
endif()

#if(NOT EXISTS "${protobuf_SRC}/java/core/src/main/java/com/google/protobuf/Any.java")
  message(STATUS "Generating Java Protobuf files")
  set(__API_PROTO_FILES any.proto
                        any_test.proto
                        api.proto
                        descriptor.proto
                        duration.proto
                        empty.proto
                        field_mask.proto
                        #map_lite_unittest.proto
                        map_proto2_unittest.proto
                        map_unittest.proto
                        source_context.proto
                        struct.proto
                        test_messages_proto2.proto
                        test_messages_proto3.proto
                        timestamp.proto
                        type.proto
                        unittest.proto
                        unittest_arena.proto
                        unittest_custom_options.proto
                        unittest_drop_unknown_fields.proto
                        unittest_embed_optimize_for.proto
                        unittest_empty.proto
                        unittest_enormous_descriptor.proto
                        unittest_import.proto
                        #unittest_import_lite.proto
                        unittest_import_public.proto
                        #unittest_import_public_lite.proto
                        unittest_lazy_dependencies.proto
                        unittest_lazy_dependencies_custom_option.proto
                        unittest_lazy_dependencies_enum.proto
                        #unittest_lite.proto
                        #unittest_lite_imports_nonlite.proto
                        unittest_mset.proto
                        unittest_mset_wire_format.proto
                        unittest_no_arena.proto
                        unittest_no_arena_import.proto
                        #unittest_no_arena_lite.proto
                        unittest_no_field_presence.proto
                        unittest_no_generic_services.proto
                        unittest_optimize_for.proto
                        unittest_preserve_unknown_enum.proto
                        unittest_preserve_unknown_enum2.proto
                        unittest_proto3.proto
                        unittest_proto3_arena.proto
                        #unittest_proto3_arena_lite.proto
                        #unittest_proto3_lite.proto
                        unittest_well_known_types.proto
                        wrappers.proto
                        )
  #Generate the java API files from their proto files
  foreach(f ${__API_PROTO_FILES})
    execute_process(COMMAND ${BINDER} --proto_path=${protobuf_SRC}/src/
                                      --java_out=${protobuf_SRC}/java/core/src/main/java/
                                        "${protobuf_SRC}/src/google/protobuf/${f}")
    message(STATUS "Java Binding file ${protobuf_SRC}/src/google/protobuf/${f}")
  endforeach()
  
# Copy these files to our source directory
file(COPY "${protobuf_SRC}/java/core/src/main/java/com"
     DESTINATION ${java_bindings_DIR}
)
file(COPY "${protobuf_SRC}/java/util/src/main/java/com"
     DESTINATION ${java_bindings_DIR}
)

foreach(f ${_FILES})
  message(STATUS "Java Binding file ${f}")
  execute_process(COMMAND ${BINDER} --proto_path=${from}
                                    --java_out=${java_bindings_DIR}
                                    ${f})
endforeach()
message(STATUS "java bindings are here : ${java_bindings_DIR}" )

#################
## C# Bindings ##
#################

set(csharp_bindings_DIR "${to}/csharp")
file(MAKE_DIRECTORY "${csharp_bindings_DIR}")
file(GLOB_RECURSE _OLD_CSHARP_FILES "${csharp_bindings_DIR}/*.*")
if(_OLD_CSHARP_FILES)
  file(REMOVE ${_OLD_CSHARP_FILES})
endif() 
foreach(f ${_FILES})
  message(STATUS "C# Binding file ${f}")
  execute_process(COMMAND ${BINDER} --proto_path=${from}
                                    --csharp_out=${csharp_bindings_DIR}
                                    ${f})
endforeach()
message(STATUS "csharp bindings are here : ${csharp_bindings_DIR}" )

#####################
## Python Bindings ##
#####################

set(python_bindings_DIR "${SRC_ROOT}/python")
file(GLOB _OLD_CDM_BIND_FILES "${python_bindings_DIR}/${CDM_PACKAGE}/*")
if(_OLD_CDM_BIND_FILES)
  file(REMOVE ${_OLD_CDM_BIND_FILES})
endif()
file(GLOB _OLD_ENGINE_BIND_FILES "${python_bindings_DIR}/${ENGINE_PACKAGE}/*")
if(_OLD_ENGINE_BIND_FILES)
  file(REMOVE ${_OLD_ENGINE_BIND_FILES})
endif()
foreach(f ${_FILES})
  message(STATUS "Python Binding file ${f}")
  execute_process(COMMAND ${BINDER} --proto_path=${from}
                                    --python_out=${python_bindings_DIR}
                                    ${f})
endforeach()
message(STATUS "python bindings are here : ${python_bindings_DIR}" )

file(TOUCH ${to}/schema_last_built)
message(STATUS "Touch file ${to}/schema_last_built")
