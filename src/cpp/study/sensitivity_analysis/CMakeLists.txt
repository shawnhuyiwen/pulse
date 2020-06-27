
add_library_ex(SensitivityAnalysisEngine
  H_FILES
    SARunner.h
  CPP_FILES
    SARunner.cpp
  PUBLIC_DEPENDS
    CommonPulseModels
  PRIVATE_DEPENDS
    protobuf::libprotobuf
  INSTALL_HEADER_DIR
    study/sensitivity_analysis
  )
set_target_properties(SensitivityAnalysisEngine PROPERTIES FOLDER "Studies//SensitivityAnalysis")

add_executable_ex(SensitivityAnalysisDriver main.cpp)
set_target_properties(SensitivityAnalysisDriver PROPERTIES FOLDER "Studies//SensitivityAnalysis")
target_link_libraries(SensitivityAnalysisDriver SensitivityAnalysisEngine protobuf::libprotobuf)