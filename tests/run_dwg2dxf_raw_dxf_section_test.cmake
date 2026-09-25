if(NOT EXISTS "${DWG2DXF}")
    message(FATAL_ERROR "dwg2dxf executable not found: ${DWG2DXF}")
endif()

if(NOT IS_DIRECTORY "${OUTPUT_DIR}")
    file(MAKE_DIRECTORY "${OUTPUT_DIR}")
endif()

string(RANDOM LENGTH 12 ALPHABET 0123456789abcdef _token)
set(_test_dir "${OUTPUT_DIR}/dwg2dxf-raw-section-${_token}")
file(MAKE_DIRECTORY "${_test_dir}")
set(_input "${_test_dir}/opaque-acdsdata-input.dxf")
set(_output "${_test_dir}/opaque-acdsdata-output.dxf")
set(_output2 "${_test_dir}/opaque-acdsdata-output2.dxf")

# This locally authored vector exercises only lossless opaque-section
# passthrough. Its ACDSDATA-shaped records are deliberately unassociated and
# do not claim to represent a valid solid carrier or ACDS schema.
file(WRITE "${_input}"
    "0\nSECTION\n2\nHEADER\n9\n$ACADVER\n1\nAC1032\n0\nENDSEC\n"
    "0\nSECTION\n2\nENTITIES\n0\nLINE\n5\n30\n330\n1F\n"
    "100\nAcDbEntity\n8\n0\n100\nAcDbLine\n"
    "10\n1.0\n20\n2.0\n30\n3.0\n11\n4.0\n21\n5.0\n31\n6.0\n"
    "0\nENDSEC\n0\nSECTION\n2\nACDSDATA\n"
    "0\nACDSSCHEMA\n90\n7\n1\nOpaqueSchema\n"
    "0\nACDSRECORD\n90\n9\n2\nOpaqueRecord\n"
    "320\n30\n94\n4\n310\n41434453\n"
    "0\nENDSEC\n0\nEOF\n"
)

execute_process(
    COMMAND "${DWG2DXF}" "${_input}" -o "${_output}"
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _stdout
    ERROR_VARIABLE _stderr
    TIMEOUT 10
)
if(NOT _result EQUAL 0 OR NOT EXISTS "${_output}")
    message(FATAL_ERROR
        "first conversion failed (${_result}):\n${_stdout}\n${_stderr}")
endif()

file(READ "${_output}" _contents)
foreach(_required IN ITEMS
        "AC1032" "LINE"
        "ACDSDATA" "ACDSSCHEMA" "7" "OpaqueSchema"
        "ACDSRECORD" "9" "OpaqueRecord" "320" "30"
        "94" "4" "310" "41434453")
    string(FIND "${_contents}" "${_required}" _found)
    if(_found EQUAL -1)
        message(FATAL_ERROR
            "first conversion dropped required opaque section value: ${_required}")
    endif()
endforeach()
string(FIND "${_contents}" " 10\n1\n" _line_start)
string(FIND "${_contents}" " 31\n6\n" _line_end)
if(_line_start EQUAL -1 OR _line_end EQUAL -1)
    message(FATAL_ERROR "first conversion changed the control LINE endpoints")
endif()

execute_process(
    COMMAND "${DWG2DXF}" "${_output}" -o "${_output2}"
    RESULT_VARIABLE _result2
    OUTPUT_VARIABLE _stdout2
    ERROR_VARIABLE _stderr2
    TIMEOUT 10
)
if(NOT _result2 EQUAL 0 OR NOT EXISTS "${_output2}")
    message(FATAL_ERROR
        "second conversion failed (${_result2}):\n${_stdout2}\n${_stderr2}")
endif()

file(READ "${_output2}" _contents2)
foreach(_required IN ITEMS
        "AC1032" "LINE"
        "ACDSDATA" "ACDSSCHEMA" "7" "OpaqueSchema"
        "ACDSRECORD" "9" "OpaqueRecord" "320" "30"
        "94" "4" "310" "41434453")
    string(FIND "${_contents2}" "${_required}" _found)
    if(_found EQUAL -1)
        message(FATAL_ERROR
            "second conversion dropped required opaque section value: ${_required}")
    endif()
endforeach()
string(FIND "${_contents2}" " 10\n1\n" _line_start2)
string(FIND "${_contents2}" " 31\n6\n" _line_end2)
if(_line_start2 EQUAL -1 OR _line_end2 EQUAL -1)
    message(FATAL_ERROR "second conversion changed the control LINE endpoints")
endif()

file(REMOVE_RECURSE "${_test_dir}")
