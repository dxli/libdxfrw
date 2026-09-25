foreach(_required IN ITEMS ADAPTER_TEST DWG2DXF OUTPUT_DIR)
    if(NOT DEFINED ${_required})
        message(FATAL_ERROR "${_required} is required")
    endif()
endforeach()
foreach(_path IN ITEMS ADAPTER_TEST DWG2DXF)
    if(NOT EXISTS "${${_path}}")
        message(FATAL_ERROR "${_path} does not exist: ${${_path}}")
    endif()
endforeach()

string(RANDOM LENGTH 12 ALPHABET 0123456789abcdef _run_id)
set(_test_root "${OUTPUT_DIR}/dwg2dxf-image-path-${_run_id}")
file(MAKE_DIRECTORY "${_test_root}")
set(_input "${_test_root}/image source.dxf")
set(_first "${_test_root}/image first pass.dxf")
set(_second "${_test_root}/image second pass.dxf")

execute_process(
    COMMAND "${ADAPTER_TEST}" --generate "${_input}"
    RESULT_VARIABLE _generate_result
    OUTPUT_VARIABLE _generate_stdout
    ERROR_VARIABLE _generate_stderr
    TIMEOUT 10
)
if(NOT "${_generate_result}" STREQUAL "0")
    message(FATAL_ERROR
        "Could not create image-path control (${_generate_result}).\n"
        "${_generate_stdout}\n${_generate_stderr}\nEvidence: ${_test_root}")
endif()

execute_process(
    COMMAND "${DWG2DXF}" "${_input}" -o "${_first}"
    RESULT_VARIABLE _first_result
    OUTPUT_VARIABLE _first_stdout
    ERROR_VARIABLE _first_stderr
    TIMEOUT 10
)
if(NOT "${_first_result}" STREQUAL "0")
    message(FATAL_ERROR
        "First exact-argv image-path conversion failed (${_first_result}).\n"
        "${_first_stdout}\n${_first_stderr}\nEvidence: ${_test_root}")
endif()

execute_process(
    COMMAND "${DWG2DXF}" "${_first}" -o "${_second}"
    RESULT_VARIABLE _second_result
    OUTPUT_VARIABLE _second_stdout
    ERROR_VARIABLE _second_stderr
    TIMEOUT 10
)
if(NOT "${_second_result}" STREQUAL "0")
    message(FATAL_ERROR
        "Second exact-argv image-path conversion failed (${_second_result}).\n"
        "${_second_stdout}\n${_second_stderr}\nEvidence: ${_test_root}")
endif()

foreach(_output IN ITEMS "${_first}" "${_second}")
    execute_process(
        COMMAND "${ADAPTER_TEST}" --verify "${_output}"
        RESULT_VARIABLE _verify_result
        OUTPUT_VARIABLE _verify_stdout
        ERROR_VARIABLE _verify_stderr
        TIMEOUT 10
    )
    if(NOT "${_verify_result}" STREQUAL "0")
        message(FATAL_ERROR
            "Image-path read-back failed for ${_output} (${_verify_result}).\n"
            "${_verify_stdout}\n${_verify_stderr}\nEvidence: ${_test_root}")
    endif()
endforeach()

file(REMOVE_RECURSE "${_test_root}")
message(STATUS
    "Image-definition path survived generated adapter export, two exact-argv CLI passes, and public read-back.")
