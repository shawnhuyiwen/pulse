#+
# This file is called as CMake -P script for the patch step of
# protobuf-patches/Patch.cmake protobuf_patch and protobuf_source are defined on the command
# line along with the call.
#-

message("Patching Protobuf ${protobuf_patch} AND ${protobuf_source}")

if(PULSE_IL2CPP_PATCH)
  message("Applying protobuf PULSE_IL2CPP_PATCH")
  # Patch Protobuf C# ReflectionUtil to be compatible with the Unity IL2CPP compiler
  # This file address the issue where, in Unity, the il2cpp compiler is unable
  # To properly generate code associated with protobuf enums.
  # Protobuf reflection expects the enum to have a ctor, which it does not.
  configure_file(
    ${protobuf_patch}/ReflectionUtil.cs
    ${protobuf_source}/csharp/src/Google.Protobuf/Reflection/ReflectionUtil.cs
    COPYONLY
  )
endif()

