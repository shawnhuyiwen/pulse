#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
include(AddExternalProject)
define_external_dirs_ex( Eigen3 )
add_external_project_ex( Eigen3
  URL "https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz"
  URL_HASH MD5=9e30f67e8531477de4117506fe44669b
  CMAKE_CACHE_ARGS
        -DBUILD_TESTING:BOOL=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=${Eigen3_PREFIX}/install
  RELATIVE_INCLUDE_PATH ""
  DEPENDENCIES ""
  #VERBOSE
  )
if (NOT USE_SYSTEM_Eigen3)
  set(Eigen3_DIR ${Eigen3_PREFIX}/install/share/eigen3/cmake)
  message(STATUS "Eigen3_DIR : ${Eigen3_DIR}")
endif()
