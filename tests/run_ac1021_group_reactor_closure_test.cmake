if(NOT DEFINED AUTHOR OR NOT EXISTS "${AUTHOR}")
    message(FATAL_ERROR "AC1021 GROUP author executable is unavailable")
endif()
if(NOT DEFINED ADAPTER OR NOT EXISTS "${ADAPTER}")
    message(FATAL_ERROR "DWG/DXF adapter test executable is unavailable")
endif()
if(NOT DEFINED CONVERTER OR NOT EXISTS "${CONVERTER}")
    message(FATAL_ERROR "dwg2dxf converter executable is unavailable")
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

set(dxf_output "${scratch_dir}/strict-output.dxf")
execute_process(
    COMMAND "${CONVERTER}" "${source}" -y -v2007 "${dxf_output}"
    RESULT_VARIABLE converter_result
    OUTPUT_VARIABLE converter_stdout
    ERROR_VARIABLE converter_stderr)
if(NOT converter_result EQUAL 0 OR NOT EXISTS "${dxf_output}")
    message(FATAL_ERROR
        "Generated AC1021 strict conversion failed: ${converter_stdout}${converter_stderr}")
endif()
file(READ "${dxf_output}" dxf_contents)
string(FIND "${dxf_contents}" "ACAD_PLOTSTYLENAME" plot_style_root)
string(FIND "${dxf_contents}" "Normal" plot_style_default)
if(plot_style_root EQUAL -1 OR plot_style_default EQUAL -1)
    message(FATAL_ERROR "Strict DXF output omitted the default plot-style graph")
endif()

file(REMOVE_RECURSE "${scratch_dir}")
message(STATUS
    "Generated AC1021 GROUP and default plot-style reactor graph closure, refusal, retry and strict DXF output passed")
