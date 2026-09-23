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

# Unix command-line arguments are byte strings (UTF-8 on the supported host
# environments exercised here). Keep Windows Unicode argv qualification
# separate because the narrow main(argc, argv) entry point has a distinct
# encoding contract there.
if(NOT CMAKE_HOST_WIN32)
    set(_unicode_dir "${_test_dir}/entrée café")
    file(MAKE_DIRECTORY "${_unicode_dir}")
    set(_unicode_input "${_unicode_dir}/drawing été.dwg")
    set(_unicode_output "${_unicode_dir}/résultat été.dxf")
    execute_process(
        COMMAND "${CMAKE_COMMAND}" -E copy "${SAMPLE}" "${_unicode_input}"
        RESULT_VARIABLE _copy_result
    )
    if(NOT "${_copy_result}" STREQUAL "0")
        message(FATAL_ERROR
            "Could not prepare Unicode-path DWG input (${_copy_result})")
    endif()
    execute_process(
        COMMAND "${DWG2DXF}" "${_unicode_input}" -o "${_unicode_output}"
        RESULT_VARIABLE _result
        OUTPUT_VARIABLE _stdout
        ERROR_VARIABLE _stderr
        TIMEOUT 60
    )
    if(NOT "${_result}" STREQUAL "0" OR NOT EXISTS "${_unicode_output}")
        message(FATAL_ERROR
            "FreeCAD-form conversion failed for UTF-8 paths (${_result})\n${_stdout}\n${_stderr}")
    endif()
    file(READ "${_unicode_output}" _unicode_header LIMIT 2048)
    string(FIND "${_unicode_header}" "AC1027" _unicode_version_pos)
    if(_unicode_version_pos EQUAL -1)
        message(FATAL_ERROR "UTF-8-path output is not a valid AC1027 DXF")
    endif()
else()
    message(STATUS
        "Skipping non-ASCII argv test on Windows; narrow argv encoding needs native qualification")
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

# FreeCAD ignores the converter exit status and accepts a path that exists.
# A malformed input must therefore leave no apparently successful output.
set(_bad_input "${_test_dir}/malformed source.dwg")
set(_bad_output "${_test_dir}/malformed result.dxf")
file(WRITE "${_bad_input}" "AC1027")
file(REMOVE "${_bad_output}")
execute_process(
    COMMAND "${DWG2DXF}" "${_bad_input}" -o "${_bad_output}"
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 5
)
if("${_result}" STREQUAL "0")
    message(FATAL_ERROR
        "Malformed FreeCAD-form conversion unexpectedly succeeded\n${_stdout}\n${_stderr}")
endif()
if(EXISTS "${_bad_output}")
    message(FATAL_ERROR
        "Malformed FreeCAD-form conversion left an output path FreeCAD could import")
endif()

# Even with -y, failed input must not replace a previous complete result.
file(WRITE "${_bad_output}" "preserve-after-conversion-failure\n")
execute_process(
    COMMAND "${DWG2DXF}" "${_bad_input}" -o "${_bad_output}" -y
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 5
)
if("${_result}" STREQUAL "0")
    message(FATAL_ERROR
        "Malformed explicit-overwrite conversion unexpectedly succeeded\n${_stdout}\n${_stderr}")
endif()
file(READ "${_bad_output}" _preserved)
if(NOT "${_preserved}" STREQUAL "preserve-after-conversion-failure\n")
    message(FATAL_ERROR
        "Failed explicit-overwrite conversion changed the previous output")
endif()

file(GLOB _failed_temporary_files "${_bad_output}.libdxfrw-*")
if(_failed_temporary_files)
    message(FATAL_ERROR
        "Failed conversion left output temporaries: ${_failed_temporary_files}")
endif()
