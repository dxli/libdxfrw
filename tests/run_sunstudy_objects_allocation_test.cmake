set(source "${OUTPUT_DIR}/sunstudy-objects-allocation-source.dxf")
set(destination "${OUTPUT_DIR}/sunstudy-objects-allocation-output.dxf")
file(REMOVE "${source}" "${destination}")

execute_process(
    COMMAND "${PYTHON}" "${AUTHOR}" author "${source}" --version AC1027
    RESULT_VARIABLE authored)
if(NOT authored EQUAL 0)
    message(FATAL_ERROR "SUNSTUDY allocation source author failed: ${authored}")
endif()

execute_process(
    COMMAND "${ALLOCATION_TEST}" "${source}" "${destination}"
    RESULT_VARIABLE result)
file(REMOVE "${source}" "${destination}")
if(NOT result EQUAL 0)
    message(FATAL_ERROR "SUNSTUDY OBJECTS allocation retry failed: ${result}")
endif()
