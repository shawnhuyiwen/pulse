#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
include(AddExternalProject)
define_external_dirs_ex( Dirent )
add_external_project_ex( Dirent
  URL "https://github.com/tronkko/dirent/archive/1.23.2.zip"
  URL_HASH MD5=43514791ab73ef5ac7c490afc7c3bab2
  CMAKE_CACHE_ARGS
    -DDIRENT_BUILD_TESTS:BOOL=OFF
  RELATIVE_INCLUDE_PATH ""
  DEPENDENCIES ""
  #VERBOSE
  )
if (NOT USE_SYSTEM_Dirent)
  set(Dirent_DIR ${CMAKE_INSTALL_PREFIX}/include)
  message(STATUS "Dirent_DIR : ${Dirent_DIR}")
endif()