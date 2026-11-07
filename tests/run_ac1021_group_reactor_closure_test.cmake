if(NOT DEFINED AUTHOR OR NOT EXISTS "${AUTHOR}")
    message(FATAL_ERROR "AC1021 GROUP author executable is unavailable")
endif()
if(NOT DEFINED ADAPTER OR NOT EXISTS "${ADAPTER}")
    message(FATAL_ERROR "DWG/DXF adapter test executable is unavailable")
endif()
if(NOT DEFINED OUTPUT_DIR)
    message(FATAL_ERROR "scratch output directory was not provided")
endif()

set(scratch_dir "${OUTPUT_DIR}/ac1021-group-adapter-scratch")
set(source "${scratch_dir}/vertex2d.dwg")
file(REMOVE_RECURSE "${scratch_dir}")

execute_process(
    COMMAND "${AUTHOR}" "${scratch_dir}"
    RESULT_VARIABLE author_result
    OUTPUT_VARIABLE author_stdout
    ERROR_VARIABLE author_stderr)
if(NOT author_result EQUAL 0 OR NOT EXISTS "${source}")
    message(FATAL_ERROR
        "Could not author scratch AC1021 GROUP source: ${author_stdout}${author_stderr}")
endif()

execute_process(
    COMMAND "${ADAPTER}" --generated-group-ac1021-reactor-closure "${source}"
    RESULT_VARIABLE adapter_result
    OUTPUT_VARIABLE adapter_stdout
    ERROR_VARIABLE adapter_stderr)
if(NOT adapter_result EQUAL 0)
    message(FATAL_ERROR
        "Generated AC1021 GROUP output/readback failed: ${adapter_stdout}${adapter_stderr}")
endif()

file(REMOVE_RECURSE "${scratch_dir}")
message(STATUS
    "Generated AC1021 GROUP rooted reactor closure, refusal and retry passed")
