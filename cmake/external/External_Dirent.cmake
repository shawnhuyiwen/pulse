#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------
include(AddExternalProject)
define_external_dirs_ex( Dirent )
add_external_project_ex( Dirent
  URL "https://github.com/tronkko/dirent/archive/1.22.zip"
  URL_HASH MD5=cf5b4499d163604732f4dc91654056be
  CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  RELATIVE_INCLUDE_PATH ""
  DEPENDENCIES ""
  #VERBOSE
  )
if (NOT USE_SYSTEM_Dirent)
  set(Dirent_DIR ${CMAKE_INSTALL_PREFIX}/include)
  message(STATUS "Dirent_DIR : ${Dirent_DIR}")
endif()