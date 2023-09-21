add_subdirectory(bind)
add_subdirectory(cdm)
add_subdirectory(engine)

if(${PROJECT_NAME}_C_STATIC)
  return()
endif()

add_subdirectory(howto)
add_subdirectory(scenario_driver)
add_subdirectory(test_driver)

add_subdirectory(study/circuit_optimization)
add_subdirectory(study/hydrocephalus)
add_subdirectory(study/multiplex_ventilation)
add_subdirectory(study/patient_variability)
add_subdirectory(study/sensitivity_analysis)