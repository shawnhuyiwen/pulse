#+
# This file is called as CMake -P script for the patch step of
# eigen-patches/Patch.cmake eigen_patch and eigen_source are defined on the command
# line along with the call.
#-

message("Patching Eigen ${eigen_patch} AND ${eigen_source}")
configure_file(
  ${eigen_patch}/cmake/FindBLAS.cmake
  ${eigen_source}/cmake/
  COPYONLY
  )

# Apply language_support patch from upstream commit
# https://bitbucket.org/eigen/eigen/commits/ba14974d054a
configure_file(
  ${eigen_patch}/cmake/language_support.cmake
  ${eigen_source}/cmake/
  COPYONLY
)