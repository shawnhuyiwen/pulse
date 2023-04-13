#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------

include(AddExternalProject)
define_external_dirs_ex(pybind11)
add_external_project_ex( pybind11
  URL https://github.com/pybind/pybind11/archive/refs/tags/v2.10.3.zip
  URL_MD5 85f96e10010371b65e1be14a648fd7b5
  CMAKE_CACHE_ARGS
    -DCMAKE_INSTALL_PREFIX:STRING=${pybind11_PREFIX}/install
    -DPYBIND11_TEST:BOOL=OFF 
  RELATIVE_INCLUDE_PATH "include"
  #DEPENDENCIES ""
  #VERBOSE
)
if (NOT USE_SYSTEM_pybind11)
  set(pybind11_DIR ${pybind11_PREFIX}/install/share/cmake/pybind11)
  message(STATUS "pybind11_DIR : ${pybind11_DIR}")
endif()