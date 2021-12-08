if(NOT Pulse_CSHARP_API)
  return()
endif()

if(NOT MSVC)
  # Not sure yet how to support C# outside of MSVC
  return()
endif()

# Build the Protobuf DLL if we have not already
set(protobuf_REF "${protobuf_SRC}/csharp/src/Google.Protobuf/bin/Release/net45/Google.Protobuf.dll")
if(NOT EXISTS "${protobuf_REF}")
  message(STATUS "Did not find Google.Protobuf.dll, attempting to build it...")
  execute_process(
    COMMAND ${CMAKE_VS_MSBUILD_COMMAND} -t:restore Google.Protobuf.sln
    WORKING_DIRECTORY "${protobuf_SRC}/csharp/src"
    )
  execute_process(
    COMMAND ${CMAKE_VS_MSBUILD_COMMAND} -p:Configuration=Release Google.Protobuf/Google.Protobuf.csproj
    WORKING_DIRECTORY "${protobuf_SRC}/csharp/src"
    )
endif()

include(CSharpUtilities)
enable_language(CSharp)

include(PulseFiles.cmake)

file(COPY ${protobuf_REF} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

# Bindings
#add_library(PulseCSharpBindings SHARED
#  ${PULSE_CSHARP_BIND_FILES})
#source_group(TREE "${PULSE_CSHARP_BIND_DIR}" FILES ${PULSE_CSHARP_BIND_FILES})
#set_target_properties (PulseCSharpBindings PROPERTIES FOLDER ${PROJECT_NAME})
#set_property(TARGET PulseCSharpBindings PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
#set_property(TARGET PulseCSharpBindings PROPERTY VS_DOTNET_REFERENCES
#  ${protobuf_REF})
#add_custom_command(TARGET PulseCSharpBindings POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy $(TargetPath) ${CMAKE_INSTALL_PREFIX}/bin)
#install(TARGETS PulseCSharpBindings EXPORT ${PROJECT_NAME}Targets
#      RUNTIME DESTINATION bin COMPONENT RuntimeLibraries)

# Pulse API
add_library(PulseCSharp SHARED
  ${PULSE_CSHARP_FILES} ${PULSE_CSHARP_BIND_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${PULSE_CSHARP_FILES})
source_group(TREE "${PULSE_CSHARP_BIND_DIR}" FILES ${PULSE_CSHARP_BIND_FILES})
set_target_properties (PulseCSharp PROPERTIES FOLDER ${PROJECT_NAME})
set_property(TARGET PulseCSharp PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
set_property(TARGET PulseCSharp PROPERTY VS_DOTNET_REFERENCES
  "System"
  "${protobuf_REF}")
add_custom_command(TARGET PulseCSharp POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $(TargetPath) ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS PulseCSharp EXPORT ${PROJECT_NAME}Targets
      RUNTIME DESTINATION bin COMPONENT RuntimeLibraries)

# Pulse HowTo
add_executable(PulseCSharpHowTo
  ${PULSE_CSHARP_HOWTO_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${PULSE_CSHARP_HOWTO_FILES})
set_target_properties (PulseCSharpHowTo PROPERTIES FOLDER ${PROJECT_NAME})
set_property(TARGET PulseCSharpHowTo PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
set_property(TARGET PulseCSharpHowTo PROPERTY VS_DOTNET_REFERENCES
  "System")
add_dependencies(PulseCSharpHowTo PulseCSharp)
add_custom_command(TARGET PulseCSharpHowTo POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $(TargetPath) ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS PulseCSharpHowTo EXPORT ${PROJECT_NAME}Targets
      RUNTIME DESTINATION bin COMPONENT RuntimeLibraries)
