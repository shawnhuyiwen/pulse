if(NOT Pulse_JAVA_API)
  return()
endif()

find_package(Java REQUIRED)
include(UseJava)
find_package(JNI REQUIRED)

add_library_ex(PulseJNI SHARED
  H_FILES
    jni/PulseEngineJNI.h
  CPP_FILES
    jni/CommonDataModelJNI.cpp
    jni/PulseEngineJNI.cpp
  PUBLIC_DEPENDS
    CommonDataModelTests
    CommonPulseModelsTests
)
target_include_directories(PulseJNI PRIVATE ${JNI_INCLUDE_DIRS})
set_target_properties (PulseJNI PROPERTIES FOLDER ${PROJECT_NAME})

include(PulseFiles.cmake)
# Still going to glob google protobuf java files
file(GLOB_RECURSE GOOGLE_JAVA_FILES "com/google/*.java")
add_jar(PulseJava ${PULSE_JAVA_FILES} ${GOOGLE_JAVA_FILES}
    INCLUDE_JARS 
      "jar/commons-collections4-4.4.jar"
      "jar/commons-compress-1.19.jar"
      "jar/commons-math3-3.6.1.jar"
      "jar/error_prone_annotations-2.3.4.jar"
      "jar/gson-2.8.6.jar"
      "jar/guava-29.0-jre.jar"
      "jar/java-ordered-properties-1.0.3.jar"
      "jar/javassist.jar"
      "jar/jfreechart-1.5.0.jar"
      "jar/logback-classic-1.2.3.jar"
      "jar/logback-core-1.2.3.jar"
      "jar/poi-4.1.2.jar"
      "jar/poi-ooxml-4.1.2.jar"
      "jar/poi-ooxml-schemas-4.1.2.jar"
      "jar/reflections-0.9.12.jar"
      "jar/slf4j-api-1.7.30.jar"
      "jar/xmlbeans-3.1.0.jar"
    OUTPUT_NAME Pulse
    MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST.MF)
get_target_property(_jarFile PulseJava JAR_FILE)
add_custom_command(TARGET PulseJava POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${_jarFile} ${CMAKE_INSTALL_PREFIX}/bin)
install_jar(PulseJava ${CMAKE_INSTALL_PREFIX}/bin)
set_target_properties (PulseJava PROPERTIES FOLDER ${PROJECT_NAME})
