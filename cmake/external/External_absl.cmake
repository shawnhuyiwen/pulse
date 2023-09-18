#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
if(${PROJECT_NAME}_MULTI_BUILD)
  message(STATUS "Building multiple abls configurations at once")
  set(ABSL_DEBUG_BLD COMMAND ${CMAKE_COMMAND} --build . --config debug)
  set(ABSL_RELEASE_BLD COMMAND ${CMAKE_COMMAND} --build . --config release)
  set(ABSL_MULTI_BUILD BUILD_COMMAND ${ABSL_DEBUG_BLD} ${ABSL_RELEASE_BLD})
  set(ABSL_DEBUG_INSTALL COMMAND ${CMAKE_COMMAND} --build . --target install --config debug)
  set(ABSL_RELEASE_INSTALL COMMAND ${CMAKE_COMMAND} --build . --target install --config release)
  set(ABSL_MULTI_INSTALL BUILD_COMMAND ${ABSL_DEBUG_INSTALL} ${ABSL_RELEASE_INSTALL})
endif()

define_external_dirs_ex(absl)
add_external_project_ex(absl
  URL https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.0.zip
  URL_MD5 "3d4793b3d644fd184a2b9f57da1c4839"
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=${absl_PREFIX}/install
    -DABSL_PROPAGATE_CXX_STD:BOOL=ON
  RELATIVE_INCLUDE_PATH "include"
  #DEPENDENCIES ""
  #VERBOSE
  ${ABSL_MULTI_BUILD}
  ${ABSL_MULTI_INSTALL}
)

if (NOT USE_SYSTEM_abls)
  set(absl_DIR "${absl_PREFIX}/install/lib/cmake/absl")
  message(STATUS "absl_DIR : ${absl_DIR}")
endif()
