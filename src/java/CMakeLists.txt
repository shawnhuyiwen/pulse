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

file(GLOB_RECURSE JAVA_FILES
  "com/*.java"
  "pulse/*.java")
add_jar(PulseJava ${JAVA_FILES}
    INCLUDE_JARS 
      "jar/commons-collections4-4.4.jar"
      "jar/commons-compress-1.19.jar"
      "jar/commons-math3-3.6.1.jar"
      "jar/error_prone_annotations-2.3.4.jar"
      "jar/gson-2.8.6.jar"
      "jar/guava-29.0-jre.jar"
      "jar/jfreechart-1.5.0.jar"
      "jar/poi-4.1.2.jar"
      "jar/poi-ooxml-4.1.2.jar"
      "jar/poi-ooxml-schemas-3.13-20150929.jar"
      "jar/poi-ooxml-schemas-4.1.2.jar"
      "jar/reflections-0.9.12.jar"
      "jar/xmlbeans-3.1.0.jar"
      "jar/slf4j-api-1.7.30.jar"
      "jar/logback-classic-1.2.3.jar"
      "jar/logback-core-1.2.3.jar"
    OUTPUT_NAME Pulse)
get_target_property(_jarFile PulseJava JAR_FILE)
add_custom_command(TARGET PulseJava POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${_jarFile} ${CMAKE_INSTALL_PREFIX}/bin)
install_jar(PulseJava ${CMAKE_INSTALL_PREFIX}/bin)
set_target_properties (PulseJava PROPERTIES FOLDER ${PROJECT_NAME})
