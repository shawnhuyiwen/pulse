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
    CommonDataModelUnitTests
    PulseEngineUnitTests
)
target_include_directories(PulseJNI PRIVATE ${JNI_INCLUDE_DIRS})
target_include_directories(PulseJNI PRIVATE ${CMAKE_SOURCE_DIR}/test/cdm/cpp)
target_include_directories(PulseJNI PRIVATE ${CMAKE_SOURCE_DIR}/test/engine/cpp)

file(GLOB_RECURSE JAVA_FILES
  "com/*.java"
  "pulse/*.java")
add_jar(PulseJava ${JAVA_FILES}
    INCLUDE_JARS 
      "jar/jcommon-1.0.16.jar"
      "jar/jdom-2.0.2.jar"
      "jar/jfreechart-1.0.13.jar"
      "jar/gson-2.8.5.jar"
      "jar/guava-26.0-jre.jar"
      "jar/log4j-1.2.17.jar"
      "jar/poi-3.13-20150929.jar"
      "jar/poi-ooxml-3.13-20150929.jar"
      "jar/poi-ooxml-schemas-3.13-20150929.jar"
      "jar/pdfbox-2.0.0-RC3.jar"
      "jar/reflections-0.9.9-RC1-uberjar.jar"
      "jar/zip4j-1.3.1.jar"
      "jar/org.eclipse.jgit-4.11.0.201803080745-r.jar"
      "jar/slf4j-api-1.7.25.jar"
      "jar/slf4j-simple-1.7.25.jar"
      "jar/error_prone_annotations-2.3.2.jar"
    OUTPUT_NAME Pulse)
get_target_property(_jarFile PulseJava JAR_FILE)
add_custom_command(TARGET PulseJava POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${_jarFile} ${CMAKE_INSTALL_PREFIX}/bin)
install_jar(PulseJava ${CMAKE_INSTALL_PREFIX}/bin)
