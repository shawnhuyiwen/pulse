if(NOT PULSE_BUILD_JAVA_UTILS)
  return()
endif()

find_package(Java REQUIRED)
include(UseJava)
# TODO put a USE_JAVA option in (you would get no JNI, jar, and test suite, but maybe you just want the C++ SDK)
# Java Compiling

add_library(PulseJNI SHARED jni/CommonDataModelJNI.cpp
                            jni/PulseEngineJNI.h
                            jni/PulseEngineJNI.cpp)
find_package(JNI REQUIRED)
target_include_directories(PulseJNI PRIVATE ${JNI_INCLUDE_DIRS})
target_include_directories(PulseJNI PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(PulseJNI PRIVATE ${CMAKE_SOURCE_DIR}/engine/cpp)
target_include_directories(PulseJNI PRIVATE ${CMAKE_SOURCE_DIR}/cdm/cpp)
target_include_directories(PulseJNI PRIVATE ${CMAKE_SOURCE_DIR}/test/engine/cpp)
target_include_directories(PulseJNI PRIVATE ${CMAKE_SOURCE_DIR}/test/cdm/cpp)

set(PulseJNI_FLAGS)
set_target_properties(PulseJNI PROPERTIES COMPILE_FLAGS "${PulseJNI_FLAGS}" PREFIX "")

if(APPLE)
    set_target_properties(PulseJNI PROPERTIES MACOSX_RPATH ON)
endif()

set_target_properties(PulseJNI PROPERTIES
    DEBUG_POSTFIX "${PULSE_DEBUG_POSTFIX}"
    RELWITHDEBINFO_POSTFIX "${PULSE_RELWITHDEBINFO_POSTFIX}")

target_link_libraries(PulseJNI PulseEngineUnitTests)
target_link_libraries(PulseJNI CommonDataModelUnitTests)

add_custom_command(TARGET PulseJNI POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PulseJNI> ${INSTALL_BIN})

install(TARGETS PulseJNI
        RUNTIME DESTINATION ${INSTALL_BIN}
        LIBRARY DESTINATION ${INSTALL_LIB}
        ARCHIVE DESTINATION ${INSTALL_LIB})


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
    COMMAND ${CMAKE_COMMAND} -E make_directory ${INSTALL_BIN}
    COMMAND ${CMAKE_COMMAND} -E copy ${_jarFile} ${INSTALL_BIN})
install_jar(PulseJava ${INSTALL_BIN})
