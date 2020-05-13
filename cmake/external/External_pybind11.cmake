#-----------------------------------------------------------------------------
# Add External Project
#-----------------------------------------------------------------------------

include(AddExternalProject)
define_external_dirs_ex(pybind11)
add_external_project_ex( pybind11
  URL https://github.com/pybind/pybind11/archive/v2.4.3.tar.gz
  URL_MD5 62254c40f89925bb894be421fe4cdef2
  CMAKE_CACHE_ARGS
    -DCMAKE_INSTALL_PREFIX:STRING=${pybind11_PREFIX}/install
    -DPYBIND11_TEST:BOOL=OFF 
    -DPYBIND11_PYTHON_VERSION:STRING=3.6
    -DPYBIND11_CPP_STANDARD:STRING=/std:c++11
  RELATIVE_INCLUDE_PATH "include"
  #DEPENDENCIES ""
  #VERBOSE
)
if (NOT USE_SYSTEM_pybind11)
  set(pybind11_DIR ${pybind11_PREFIX}/install/share/cmake/pybind11)
  message(STATUS "pybind11_DIR : ${pybind11_DIR}")
endif()