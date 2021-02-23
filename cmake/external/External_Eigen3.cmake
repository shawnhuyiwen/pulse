#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
include(AddExternalProject)
define_external_dirs_ex( Eigen3 )
add_external_project_ex( Eigen3
  URL "https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.gz"
  URL_HASH MD5=609286804b0f79be622ccf7f9ff2b660
  CMAKE_CACHE_ARGS 
    -DBUILD_TESTING:BOOL=OFF
  RELATIVE_INCLUDE_PATH ""
  DEPENDENCIES ""
  #VERBOSE
  )
if (NOT USE_SYSTEM_Eigen3)
  set(Eigen3_DIR ${Eigen3_PREFIX}/install/share/eigen3/cmake)
  message(STATUS "Eigen3_DIR : ${Eigen3_DIR}")
endif()
