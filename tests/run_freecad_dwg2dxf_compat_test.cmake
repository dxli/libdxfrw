if(NOT DEFINED DWG2DXF OR NOT DEFINED SAMPLE OR NOT DEFINED OUTPUT_DIR)
    message(FATAL_ERROR "DWG2DXF, SAMPLE, and OUTPUT_DIR are required")
endif()

set(_test_dir "${OUTPUT_DIR}/freecad dwg2dxf cli test")
file(MAKE_DIRECTORY "${_test_dir}")
set(_input "${_test_dir}/source drawing AC1027.dwg")
execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy "${SAMPLE}" "${_input}"
    RESULT_VARIABLE _copy_result
)
if(NOT "${_copy_result}" STREQUAL "0")
    message(FATAL_ERROR "Could not prepare the test-local DWG input (${_copy_result})")
endif()
set(_output "${_test_dir}/ordinary encoding AC1027.dxf")
file(REMOVE "${_output}")

# This is the exact argv shape used by FreeCAD: no shell, version, or -y flag.
execute_process(
    COMMAND "${DWG2DXF}" "${_input}" -o "${_output}"
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 60
)
if(NOT "${_result}" STREQUAL "0")
    message(FATAL_ERROR
        "FreeCAD-form dwg2dxf invocation failed (${_result})\n${_stdout}\n${_stderr}")
endif()
if(NOT EXISTS "${_output}")
    message(FATAL_ERROR "FreeCAD-form invocation did not create its DXF output")
endif()

file(READ "${_output}" _header LIMIT 2048)
string(FIND "${_header}" "SECTION" _section_pos)
string(FIND "${_header}" "AC1027" _version_pos)
if(_section_pos EQUAL -1 OR _version_pos EQUAL -1)
    message(FATAL_ERROR
        "Expected ASCII DXF structure and source revision AC1027 in output header")
endif()

# Preserve the established positional form and its explicit-version behavior.
set(_legacy_output "${_test_dir}/legacy positional AC1024.dxf")
file(REMOVE "${_legacy_output}")
execute_process(
    COMMAND "${DWG2DXF}" "${_input}" -v2010 "${_legacy_output}"
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 60
)
if(NOT "${_result}" STREQUAL "0")
    message(FATAL_ERROR
        "Legacy positional invocation failed (${_result})\n${_stdout}\n${_stderr}")
endif()
file(READ "${_legacy_output}" _legacy_header LIMIT 2048)
string(FIND "${_legacy_header}" "AC1024" _legacy_version_pos)
if(_legacy_version_pos EQUAL -1)
    message(FATAL_ERROR "Legacy positional invocation ignored -v2010")
endif()

# The normal FreeCAD argv has no -y. It must fail promptly without prompting,
# and must preserve an existing output instead of truncating it.
file(WRITE "${_output}" "freecad-output-sentinel\n")
execute_process(
    COMMAND "${DWG2DXF}" "${_input}" -o "${_output}"
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 5
)
if("${_result}" STREQUAL "0")
    message(FATAL_ERROR "No-overwrite FreeCAD-form invocation unexpectedly succeeded")
endif()
file(READ "${_output}" _sentinel)
if(NOT "${_sentinel}" STREQUAL "freecad-output-sentinel\n")
    message(FATAL_ERROR "No-overwrite failure modified the existing output")
endif()

# An explicit -y authorizes replacement. The output remains test-local.
execute_process(
    COMMAND "${DWG2DXF}" "${_input}" -o "${_output}" -y
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 60
)
if(NOT "${_result}" STREQUAL "0")
    message(FATAL_ERROR
        "Explicit overwrite invocation failed (${_result})\n${_stdout}\n${_stderr}")
endif()
file(READ "${_output}" _header LIMIT 2048)
string(FIND "${_header}" "AC1027" _version_pos)
if(_version_pos EQUAL -1)
    message(FATAL_ERROR "Explicit overwrite did not replace the sentinel with a DXF")
endif()
