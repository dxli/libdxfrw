# DWG read tests for libdxfrw.
#
# Tests invoke the dwg2dxf binary against sample .dwg files and verify
# the produced .dxf is non-empty and structurally well-formed.
#
# Sample files live in tests/samples/<AC_VERSION>/. A test is registered
# for every .dwg under each version directory at configure time. Drop a
# new .dwg in to register a new test on next CMake reconfigure.
#
# Each registered test cross-converts to multiple output DXF versions
# (R12 / v2000 / v2010) so the writer side gets exercised too. The harness
# verifies:
#   1. The sample's first 6 bytes literally match its declared AC version.
#   2. dwg2dxf returns 0 and produces a non-empty DXF.
#   3. The DXF contains required structural markers (SECTION / ENDSEC / EOF).
#   4. The DXF contains at least one entity OR table block.
#
# Test labels: "dwg_read", the AC version (AC1021 / AC1024 / ...), and
# the output version (out_R12 / out_v2000 / out_v2010).
#
# Run:
#   cd build && ctest -L dwg_read --output-on-failure
#   cd build && ctest -L AC1024 --output-on-failure
#   cd build && ctest -L out_v2010 --output-on-failure

if(NOT TARGET dwg2dxf)
    message(STATUS "tests/: dwg2dxf target not built; skipping DWG read tests")
    return()
endif()

# Keep the no-version FreeCAD CLI revision policy independently tested from
# full DWG input availability. The source-version mapping is pure policy.
add_executable(libdxfrw_dwg2dxf_version_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/dwg2dxf_version_tests.cpp
)
target_compile_features(libdxfrw_dwg2dxf_version_tests PRIVATE cxx_std_17)
target_compile_options(libdxfrw_dwg2dxf_version_tests
    PRIVATE ${LIBDXFRW_WARNING_FLAGS})
target_include_directories(libdxfrw_dwg2dxf_version_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/dwg2dxf
    ${CMAKE_SOURCE_DIR}/src
)
add_test(NAME dwg2dxf_version_policy
    COMMAND libdxfrw_dwg2dxf_version_tests)
set_tests_properties(dwg2dxf_version_policy PROPERTIES
    LABELS "cli;fast")

# Match FreeCAD's Draft/importDWG.py subprocess contract without requiring a
# FreeCAD installation. The DWG fixture is already tracked in this repository.
set(_freecad_cli_sample
    ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/ordinary_enc_AC1027.dwg
)
set(_freecad_cli_rtext_sample
    ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/rtext_arctext.dwg
)
set(_freecad_cli_mpolygon_sample
    ${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/mpolygon_solid.dwg
)
if(EXISTS "${_freecad_cli_sample}"
        AND EXISTS "${_freecad_cli_rtext_sample}"
        AND EXISTS "${_freecad_cli_mpolygon_sample}")
    add_test(
        NAME dwg2dxf_freecad_cli_compat
        COMMAND ${CMAKE_COMMAND}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DSAMPLE=${_freecad_cli_sample}
            -DRTEXT_SAMPLE=${_freecad_cli_rtext_sample}
            -DMPOLYGON_SAMPLE=${_freecad_cli_mpolygon_sample}
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_dwg2dxf_compat_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_cli_compat PROPERTIES
        LABELS "dwg_read;freecad;cli;fast"
    )
endif()

# Optional locally generated DWG control for the FreeCAD nonzero-Z path.
# LibreDWG dwgadd and FreeCAD remain external integration tools, not required
# dependencies of the normal build/test configuration.
option(LIBDXFRW_ENABLE_DWGADD_FREECAD_CONTROL
    "Enable the LibreDWG-generated FreeCAD 3D LINE converter control" OFF)
if(LIBDXFRW_ENABLE_DWGADD_FREECAD_CONTROL)
    find_program(LIBDXFRW_DWGADD_EXECUTABLE NAMES dwgadd)
    if(NOT LIBDXFRW_DWGADD_EXECUTABLE)
        message(FATAL_ERROR
            "LIBDXFRW_ENABLE_DWGADD_FREECAD_CONTROL requires LibreDWG dwgadd")
    endif()
    add_test(
        NAME dwg2dxf_freecad_3d_line_cli
        COMMAND ${CMAKE_COMMAND}
            -DDWGADD=${LIBDXFRW_DWGADD_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/ac1015_3d_line_control.dwgadd
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_line_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_line_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated")
    add_test(
        NAME dwg2dxf_freecad_3d_point_cli
        COMMAND ${CMAKE_COMMAND}
            -DDWGADD=${LIBDXFRW_DWGADD_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/ac1015_3d_point_freecad_control.dwgadd
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_point_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_point_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated")
    add_test(
        NAME dwg2dxf_freecad_3d_arc_circle_cli
        COMMAND ${CMAKE_COMMAND}
            -DDWGADD=${LIBDXFRW_DWGADD_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/ac1015_arc_circle_freecad_control.dwgadd
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_arc_circle_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_arc_circle_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated")
    add_test(
        NAME dwg2dxf_freecad_3d_polyline_cli
        COMMAND ${CMAKE_COMMAND}
            -DDWGADD=${LIBDXFRW_DWGADD_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/ac1015_3d_polyline_freecad_control.dwgadd
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_polyline_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_polyline_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated")
    foreach(_classic_family mesh pface pface_multiface)
        add_test(
            NAME dwg2dxf_freecad_3d_${_classic_family}_cli
            COMMAND ${CMAKE_COMMAND}
                -DDWGADD=${LIBDXFRW_DWGADD_EXECUTABLE}
                -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
                -DRECIPE=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dwg/ac1015_${_classic_family}_freecad_control.dwgadd
                -DFAMILY=${_classic_family}
                -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_classic_polyline_cli_test.cmake
        )
        set_tests_properties(dwg2dxf_freecad_3d_${_classic_family}_cli PROPERTIES
            LABELS "dwg_read;freecad;cli;fast;generated")
    endforeach()
endif()

# Optional independent-writer DWG controls for FreeCAD entity routes.
# DXF sources are locally authored; ODA and FreeCAD remain external tools and
# are not dependencies of the normal build/test configuration.
option(LIBDXFRW_ENABLE_ODAFILECONVERTER_FREECAD_CONTROL
    "Enable ODA-generated FreeCAD 3D converter controls" OFF)
if(LIBDXFRW_ENABLE_ODAFILECONVERTER_FREECAD_CONTROL)
    find_program(LIBDXFRW_ODAFILECONVERTER_EXECUTABLE NAMES ODAFileConverter)
    if(NOT LIBDXFRW_ODAFILECONVERTER_EXECUTABLE)
        message(FATAL_ERROR
            "LIBDXFRW_ENABLE_ODAFILECONVERTER_FREECAD_CONTROL requires ODAFileConverter; "
            "set LIBDXFRW_ODAFILECONVERTER_EXECUTABLE to its path")
    endif()
    add_test(
        NAME dwg2dxf_freecad_3d_insert_cli
        COMMAND ${CMAKE_COMMAND}
            -DODAFILECONVERTER=${LIBDXFRW_ODAFILECONVERTER_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DSOURCE_DXF=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dxf/ac1015_insert_freecad_control.dxf
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_insert_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_insert_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated;oda")
    add_test(
        NAME dwg2dxf_freecad_3d_spline_cli
        COMMAND ${CMAKE_COMMAND}
            -DODAFILECONVERTER=${LIBDXFRW_ODAFILECONVERTER_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            "-DSOURCE_DXF=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dxf/ac1015_nonplanar_spline_freecad_control.dxf"
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_spline_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_spline_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated;oda")
    add_test(
        NAME dwg2dxf_freecad_3d_helix_cli
        COMMAND ${CMAKE_COMMAND}
            -DODAFILECONVERTER=${LIBDXFRW_ODAFILECONVERTER_EXECUTABLE}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            "-DSOURCE_DXF=${CMAKE_CURRENT_SOURCE_DIR}/fixtures/dxf/ac1015_helix_freecad_control.dxf"
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_freecad_3d_helix_cli_test.cmake
    )
    set_tests_properties(dwg2dxf_freecad_3d_helix_cli PROPERTIES
        LABELS "dwg_read;freecad;cli;fast;generated;oda")
endif()

# LibreCAD_3 ABI compatibility check.
# Mirrors LibreCAD_3's DXFimpl override surface; fails to compile if upstream
# introduces a new pure virtual on DRW_Interface or removes/renames a public
# DRW_* field LibreCAD_3 reads. Built as an OBJECT library — no installed
# binary, no link-time impact, just a compile-only ABI gate. See the file
# header in lc3_compat_check.cpp for the sync procedure.
add_library(lc3_compat_check OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/lc3_compat_check.cpp
)
target_include_directories(lc3_compat_check PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/intern
)
target_compile_features(lc3_compat_check PRIVATE cxx_std_17)

# Exercise a public DRW_Interface sink against locally generated 3D DXF
# records. The probe writes ASCII and binary DXFs at runtime, reads them back
# through dxfRW, and checks that native Z/topology/OCS values reach the sink.
# It deliberately compiles only against the standalone library API; the
# separate lc3_compat_check above remains the older 2D-consumer guard.
add_executable(libdxfrw_3d_consumer_probe
    ${CMAKE_CURRENT_SOURCE_DIR}/semantic_differential_adapter.cpp
)
target_compile_features(libdxfrw_3d_consumer_probe PRIVATE cxx_std_17)
target_compile_options(libdxfrw_3d_consumer_probe
    PRIVATE ${LIBDXFRW_ADAPTER_WARNING_FLAGS})
target_compile_definitions(libdxfrw_3d_consumer_probe PRIVATE
    LIBDXFRW_SEMANTIC_3D_CONSUMER_PROBE=1)
target_include_directories(libdxfrw_3d_consumer_probe PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/intern)
target_link_libraries(libdxfrw_3d_consumer_probe PRIVATE dxfrw)
add_test(NAME libdxfrw_3d_consumer_probe
    COMMAND libdxfrw_3d_consumer_probe --self-test)
set_tests_properties(libdxfrw_3d_consumer_probe PROPERTIES
    LABELS "dxf;consumer;fast")

# DXF splinepoints round-trip regression test.
# Verifies Band BC: code 44 (tolfit) + fit points (11/21/31) survive a
# DWG -> DXF -> DXF round-trip. Picks a known-spline-bearing sample.
set(_dxf_roundtrip_spline_sample
    ${CMAKE_CURRENT_SOURCE_DIR}/samples/AC1024/visualization_-_conference_room.dwg
)
if(EXISTS "${_dxf_roundtrip_spline_sample}")
    add_test(
        NAME dxf_roundtrip_splinepoints
        COMMAND ${CMAKE_COMMAND}
            -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
            -DSPLINE_DWG=${_dxf_roundtrip_spline_sample}
            -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/run_dxf_roundtrip_splinepoints_test.cmake
    )
    set_tests_properties(dxf_roundtrip_splinepoints PROPERTIES
        LABELS "dxf_roundtrip;BC"
    )
endif()

set(_dwg_test_versions AC1021 AC1024 AC1027 AC1032)
set(_dwg_test_outputs R12 v2000 v2010)

foreach(_ver IN LISTS _dwg_test_versions)
    file(GLOB _samples
        LIST_DIRECTORIES false
        "${CMAKE_CURRENT_SOURCE_DIR}/samples/${_ver}/*.dwg"
    )
    list(LENGTH _samples _n)
    if(_n EQUAL 0)
        message(STATUS "tests/: no ${_ver} samples in tests/samples/${_ver}/ — drop *.dwg files there to enable")
        continue()
    endif()
    list(LENGTH _dwg_test_outputs _outs)
    math(EXPR _total "${_n} * ${_outs}")
    message(STATUS "tests/: registering ${_n} ${_ver} sample(s) × ${_outs} output version(s) = ${_total} test(s)")
    foreach(_sample IN LISTS _samples)
        get_filename_component(_name "${_sample}" NAME_WE)
        foreach(_out IN LISTS _dwg_test_outputs)
            add_test(
                NAME dwg_read_${_ver}_${_name}_${_out}
                COMMAND ${CMAKE_COMMAND}
                    -DDWG2DXF=$<TARGET_FILE:dwg2dxf>
                    -DSAMPLE=${_sample}
                    -DEXPECTED_VERSION=${_ver}
                    -DOUTPUT_VERSION=${_out}
                    -DOUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}
                    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_dwg_read_test.cmake
            )
            set_tests_properties(dwg_read_${_ver}_${_name}_${_out} PROPERTIES
                LABELS "dwg_read;${_ver};out_${_out}"
            )
        endforeach()
    endforeach()
endforeach()
