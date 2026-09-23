#!/usr/bin/env python3
"""Compare a narrow DWG callback subset with LibreDWG's JSON reader.

This is an optional local qualification helper, not a CTest dependency. It
compares fields by entity handle so callback order is irrelevant. The accepted
scope is intentionally limited to AC1015 (R2000) 3D POLYLINE compound records,
AC1024 (R2010) INSERT placement and SPLINE fit data and one sample's LINE
endpoints, plus AC1021 (R2007) LINE endpoints and 3DFACE corners/edge flags.
Modeler, surface, and other version/family fields are not compared here.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import pathlib
import subprocess
import sys
from typing import Any


class OracleError(RuntimeError):
    pass


def is_json_word_character(value: str) -> bool:
    return value.isalnum() or value == "_"


def reject_nonstandard_json_constant(value: str) -> None:
    raise OracleError(f"non-standard JSON number {value}")


def normalize_bare_nan_tokens(value: str) -> str:
    """Replace non-standard bare NaN tokens without touching JSON strings."""
    output: list[str] = []
    in_string = False
    escaped = False
    index = 0
    while index < len(value):
        char = value[index]
        if in_string:
            output.append(char)
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                in_string = False
            index += 1
            continue
        if char == '"':
            in_string = True
            output.append(char)
            index += 1
            continue

        token_start = index
        token_end = index
        if char in "+-" and value[index + 1:index + 4].lower() == "nan":
            token_end = index + 4
        elif value[index:index + 3].lower() == "nan":
            token_end = index + 3
        if token_end > token_start:
            before = value[token_start - 1] if token_start else ""
            after = value[token_end] if token_end < len(value) else ""
            if (not is_json_word_character(before)
                    and not is_json_word_character(after)):
                output.append("null")
                index = token_end
                continue

        output.append(char)
        index += 1
    return "".join(output)


def run_json(command: list[str], label: str) -> dict[str, Any]:
    result = subprocess.run(command, capture_output=True, text=True, check=False)
    if result.returncode != 0:
        detail = result.stderr.strip() or result.stdout.strip()
        raise OracleError(f"{label} failed ({result.returncode}): {detail}")
    json_text = (normalize_bare_nan_tokens(result.stdout)
                 if label == "LibreDWG dwgread" else result.stdout)
    try:
        value = json.loads(json_text,
                           parse_constant=reject_nonstandard_json_constant)
    except json.JSONDecodeError as error:
        raise OracleError(f"{label} did not return JSON: {error}") from error
    if not isinstance(value, dict):
        raise OracleError(f"{label} returned a non-object JSON root")
    return value


def finite_number(value: Any, label: str) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise OracleError(f"{label} is not numeric: {value!r}")
    result = float(value)
    if not math.isfinite(result):
        raise OracleError(f"{label} is not finite")
    return result


def compare_number(actual: Any, expected: Any, label: str) -> None:
    lhs = finite_number(actual, label + " (libdxfrw)")
    rhs = finite_number(expected, label + " (LibreDWG)")
    if not math.isclose(lhs, rhs, rel_tol=1e-13, abs_tol=1e-12):
        raise OracleError(f"{label} differs: libdxfrw={lhs!r}, LibreDWG={rhs!r}")


def compare_vector(actual: Any, expected: Any, label: str) -> None:
    if not isinstance(actual, list) or not isinstance(expected, list):
        raise OracleError(f"{label} is not a vector on both readers")
    if len(actual) != len(expected):
        raise OracleError(f"{label} length differs: {len(actual)} != {len(expected)}")
    for index, (lhs, rhs) in enumerate(zip(actual, expected)):
        compare_number(lhs, rhs, f"{label}[{index}]")


def compare_point(actual: Any, expected: Any, label: str) -> None:
    if not isinstance(actual, dict) or set(actual) != {"x", "y", "z"}:
        raise OracleError(f"{label} is not a typed XYZ point in libdxfrw output")
    compare_vector([actual[axis] for axis in ("x", "y", "z")], expected, label)


def handle_key(value: Any, label: str) -> str:
    if isinstance(value, list) and value and isinstance(value[-1], int):
        return format(value[-1], "x")
    raise OracleError(f"{label} has no decoded numeric handle: {value!r}")


def index_external(document: dict[str, Any], entity: str) -> dict[str, dict[str, Any]]:
    objects = document.get("OBJECTS")
    if not isinstance(objects, list):
        raise OracleError("LibreDWG JSON has no OBJECTS array")
    rows: dict[str, dict[str, Any]] = {}
    for value in objects:
        if not isinstance(value, dict) or value.get("entity") != entity:
            continue
        key = handle_key(value.get("handle"), f"LibreDWG {entity}")
        if key in rows:
            raise OracleError(f"duplicate LibreDWG {entity} handle {key}")
        rows[key] = value
    return rows


def index_adapter(document: dict[str, Any], entity: str) -> dict[str, dict[str, Any]]:
    status = document.get("status", {})
    if status.get("outcome") != "success" or status.get("operationSucceeded") is not True:
        raise OracleError(f"libdxfrw DWG read did not complete successfully: {status!r}")
    records = document.get("records")
    if not isinstance(records, list):
        raise OracleError("libdxfrw adapter JSON has no records array")
    rows: dict[str, dict[str, Any]] = {}
    for value in records:
        if (not isinstance(value, dict) or value.get("kind") != "entity"
                or value.get("entity") != entity):
            continue
        key = value.get("sourceHandle")
        if not isinstance(key, str):
            raise OracleError(f"libdxfrw {entity} has no source handle")
        key = key.lower().lstrip("0") or "0"
        if key in rows:
            raise OracleError(f"duplicate libdxfrw {entity} handle {key}")
        fields = value.get("fields")
        if not isinstance(fields, list):
            raise OracleError(f"libdxfrw {entity} {key} has no typed field list")
        rows[key] = {
            "record": value,
            "fields": {field.get("name"): field.get("value")
                       for field in fields if isinstance(field, dict)},
        }
    return rows


def compare_entity_set(external: dict[str, dict[str, Any]],
                       adapter: dict[str, dict[str, Any]], entity: str,
                       expected_count: int | None) -> list[dict[str, Any]]:
    if not external:
        raise OracleError(f"LibreDWG returned no {entity} rows")
    if set(external) != set(adapter):
        missing = sorted(set(external) - set(adapter))
        extra = sorted(set(adapter) - set(external))
        raise OracleError(
            f"{entity} handle sets differ; absent in libdxfrw={missing}, "
            f"absent in LibreDWG={extra}")
    if expected_count is not None and len(external) != expected_count:
        raise OracleError(
            f"unexpected {entity} count {len(external)}; expected {expected_count}")
    return [
        {"handle": key, "external": external[key], "adapter": adapter[key]}
        for key in sorted(external, key=lambda item: int(item, 16))
    ]


def compare_insert(row: dict[str, Any]) -> None:
    external = row["external"]
    fields = row["adapter"]["fields"]
    compare_point(fields.get("insertionPoint"), external.get("ins_pt"),
                  "INSERT.insertionPoint")
    compare_vector([fields.get("xScale"), fields.get("yScale"),
                    fields.get("zScale")], external.get("scale"), "INSERT.scale")
    compare_number(fields.get("rotationAngle"), external.get("rotation"),
                   "INSERT.rotation")
    compare_point(fields.get("extrusion"), external.get("extrusion"),
                  "INSERT.extrusion")


def compare_spline(row: dict[str, Any]) -> None:
    external = row["external"]
    fields = row["adapter"]["fields"]
    compare_number(fields.get("degree"), external.get("degree"), "SPLINE.degree")
    compare_number(fields.get("scenario"), external.get("scenario"),
                   "SPLINE.scenario")
    compare_number(fields.get("fitTolerance"), external.get("fit_tol"),
                   "SPLINE.fitTolerance")
    compare_vector(
        [fields.get("startTangent", {}).get(axis) for axis in ("x", "y", "z")],
        external.get("beg_tan_vec"), "SPLINE.startTangent")
    compare_vector(
        [fields.get("endTangent", {}).get(axis) for axis in ("x", "y", "z")],
        external.get("end_tan_vec"), "SPLINE.endTangent")
    fit_points = external.get("fit_pts")
    if not isinstance(fit_points, list):
        raise OracleError("LibreDWG SPLINE row has no fit_pts array")
    compare_number(fields.get("fitPointCount"), len(fit_points),
                   "SPLINE.fitPointCount")
    for index, point in enumerate(fit_points):
        adapter_point = fields.get(f"fitPoint.{index}.position")
        compare_point(adapter_point, point, f"SPLINE.fitPoint[{index}]")


def compare_3dface(row: dict[str, Any]) -> None:
    external = row["external"]
    fields = row["adapter"]["fields"]
    for index in range(1, 5):
        compare_point(fields.get(f"corner.{index - 1}"),
                      external.get(f"corner{index}"),
                      f"3DFACE.corner{index}")

    has_no_flags = external.get("has_no_flags")
    flags = external.get("invis_flags")
    if isinstance(has_no_flags, bool) or has_no_flags not in (0, 1):
        raise OracleError("LibreDWG 3DFACE has no valid has_no_flags indicator")
    if has_no_flags == 1:
        if flags is not None:
            raise OracleError(
                "LibreDWG 3DFACE has flags with has_no_flags=1")
        flags = 0
    elif flags is None:
        raise OracleError("LibreDWG 3DFACE omits required invisible-edge flags")
    numeric_flags = finite_number(flags, "3DFACE.invisibleEdgeFlags (LibreDWG)")
    if (not numeric_flags.is_integer() or numeric_flags < 0
            or numeric_flags > 0x0F):
        raise OracleError(f"LibreDWG 3DFACE has invalid edge flags: {flags!r}")
    compare_number(fields.get("invisibleEdgeFlags"), int(numeric_flags),
                   "3DFACE.invisibleEdgeFlags")


def compare_line(row: dict[str, Any]) -> None:
    external = row["external"]
    fields = row["adapter"]["fields"]
    compare_point(fields.get("start"), external.get("start"), "LINE.start")
    compare_point(fields.get("end"), external.get("end"), "LINE.end")
    compare_number(fields.get("thickness"), external.get("thickness"),
                   "LINE.thickness")
    compare_point(fields.get("extrusion"), external.get("extrusion"),
                  "LINE.extrusion")


def compare_polyline3d(row: dict[str, Any],
                       external_vertices: dict[str, dict[str, Any]],
                       expected_vertex_count: int) -> int:
    external = row["external"]
    fields = row["adapter"]["fields"]
    parent_handle = handle_key(external.get("handle"),
                               "LibreDWG POLYLINE_3D")
    if external.get("type") != 16 or external.get("_subclass") != "AcDb3dPolyline":
        raise OracleError("sample parent is not DWG type 16 AcDb3dPolyline")
    if fields.get("flags") != 8:
        raise OracleError("libdxfrw did not preserve the sample's 3D POLYLINE flag")
    if fields.get("curveType") != external.get("curve_type"):
        raise OracleError("3D POLYLINE curve type differs")
    if fields.get("vertexCount") != expected_vertex_count:
        raise OracleError(
            f"unexpected libdxfrw 3D POLYLINE vertex count "
            f"{fields.get('vertexCount')!r}; expected {expected_vertex_count}")
    if fields.get("seqEndHandle") != handle_key(external.get("seqend"),
                                                "LibreDWG POLYLINE_3D SEQEND"):
        raise OracleError("3D POLYLINE SEQEND handle differs")

    owned_vertices = []
    for vertex_handle, vertex in external_vertices.items():
        owner_handle = handle_key(vertex.get("ownerhandle"),
                                   "LibreDWG VERTEX_3D owner")
        if owner_handle != parent_handle:
            continue
        object_index = vertex.get("index")
        if isinstance(object_index, bool) or not isinstance(object_index, int):
            raise OracleError("LibreDWG VERTEX_3D has no integer object index")
        if (vertex.get("type") != 11
                or vertex.get("_subclass") != "AcDb3dPolylineVertex"):
            raise OracleError(f"vertex {vertex_handle} is not a 3D polyline vertex")
        owned_vertices.append((object_index, vertex_handle, vertex))
    owned_vertices.sort(key=lambda item: item[0])
    if len(owned_vertices) != expected_vertex_count:
        raise OracleError(
            f"unexpected LibreDWG 3D POLYLINE child count {len(owned_vertices)}; "
            f"expected {expected_vertex_count}")

    first_handle = handle_key(external.get("first_vertex"),
                              "LibreDWG POLYLINE_3D first vertex")
    last_handle = handle_key(external.get("last_vertex"),
                             "LibreDWG POLYLINE_3D last vertex")
    if owned_vertices[0][1] != first_handle or owned_vertices[-1][1] != last_handle:
        raise OracleError("3D POLYLINE child order disagrees with first/last handles")

    nonzero_z = 0
    for index, (_, vertex_handle, vertex) in enumerate(owned_vertices):
        prefix = f"vertex.{index}"
        if fields.get(prefix + ".handle") != vertex_handle:
            raise OracleError(f"3D POLYLINE ordered vertex handle differs at {index}")
        if fields.get(prefix + ".ownerHandle") != parent_handle:
            raise OracleError(f"3D POLYLINE vertex owner differs at {index}")
        compare_point(fields.get(prefix + ".position"), vertex.get("point"),
                      f"3D POLYLINE.vertex[{index}].position")
        compare_number(fields.get(prefix + ".flags"), vertex.get("flag"),
                       f"3D POLYLINE.vertex[{index}].flags")
        point = vertex.get("point")
        if not isinstance(point, list) or len(point) != 3:
            raise OracleError(f"LibreDWG vertex {vertex_handle} has no XYZ point")
        if finite_number(point[2], "3D POLYLINE.vertex.z") != 0.0:
            nonzero_z += 1

    return nonzero_z


def sha256_file(path: pathlib.Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--adapter", required=True,
                        help="standalone libdxfrw semantic adapter executable")
    parser.add_argument("--dwgread", default="dwgread",
                        help="LibreDWG dwgread executable (default: PATH lookup)")
    parser.add_argument("--input", required=True, type=pathlib.Path,
                        help="existing supported DWG sample; never modified")
    args = parser.parse_args()

    with args.input.open("rb") as source:
        version = source.read(6).decode("ascii", errors="replace")
    expected_nonzero_z_line_count = None
    expected_nonzero_z_vertex_count = None
    expected_vertex_count = None
    sample_source = None
    if version == "AC1015" and args.input.name == "PolyLine3D.dwg":
        expected_digest = (
            "f51f4f65ba027bf1a001480d3c7b5bc5667960050081c67f9bb9c7bdbcfe815a"
        )
        actual_digest = sha256_file(args.input)
        if actual_digest != expected_digest:
            raise OracleError(
                "AC1015 PolyLine3D sample digest differs from the pinned "
                f"AutoCAD-authored input: {actual_digest}")
        cases = (("POLYLINE_3D", 1, compare_polyline3d),)
        expected_vertex_count = 6
        expected_nonzero_z_vertex_count = 0
        excluded = ["nonzero-Z coordinate preservation", "PFACE/POLYGON_MESH",
                    "other families and versions", "DWG writing"]
        sample_source = {
            "repository": "LibreDWG/libredwg",
            "commit": "34f02f54b9aacb5708c1d3d2070efb3e4b2d8c43",
            "path": "test/test-data/2000/PolyLine3D.dwg",
            "gitBlob": "bd1b3dde9daf91cd833c4745f8c35be119e2cf32",
            "sha256": expected_digest,
            "autocadPropertyDump": "test/test-data/2000/PolyLine3D.txt",
            "autocadDxfPair": "test/test-data/2000/PolyLine3D.dxf",
        }
    elif (version == "AC1024"
            and args.input.name == "visualization_-_conference_room.dwg"):
        cases = (("INSERT", 6, compare_insert),
                 ("SPLINE", 2, compare_spline))
        excluded = ["PLANESURFACE", "3DSOLID", "REGION", "BODY", "MESH",
                    "other versions and families"]
    elif (version == "AC1024"
            and args.input.name == "visualization_-_condominium_with_skylight.dwg"):
        cases = (("LINE", 2, compare_line),)
        expected_nonzero_z_line_count = 2
        excluded = ["all entities other than LINE", "other versions"]
    elif version == "AC1021" and args.input.name == "tablet.dwg":
        cases = (("3DFACE", 48, compare_3dface),
                 ("LINE", 3002, compare_line))
        expected_nonzero_z_line_count = 670
        excluded = ["all entities other than LINE and 3DFACE", "other versions"]
    else:
        raise OracleError(
            "unsupported sample profile: expected AC1015 PolyLine3D.dwg, "
            "AC1021 tablet.dwg, or AC1024 "
            "visualization_-_conference_room.dwg / "
            "visualization_-_condominium_with_skylight.dwg; "
            f"got {version!r} {args.input.name!r}")

    external = run_json([args.dwgread, "-O", "minJSON", str(args.input)],
                        "LibreDWG dwgread")
    adapter = run_json([args.adapter, "--input", str(args.input),
                        "--facade", "dwgRW"], "libdxfrw semantic adapter")

    results = []
    for entity, count, comparator in cases:
        adapter_entity = "POLYLINE" if entity == "POLYLINE_3D" else entity
        rows = compare_entity_set(index_external(external, entity),
                                  index_adapter(adapter, adapter_entity), entity,
                                  count)
        for row in rows:
            if entity == "POLYLINE_3D":
                nonzero_z = comparator(
                    row, index_external(external, "VERTEX_3D"),
                    expected_vertex_count)
                if (expected_nonzero_z_vertex_count is not None
                        and nonzero_z != expected_nonzero_z_vertex_count):
                    raise OracleError(
                        "unexpected nonzero-Z 3D POLYLINE vertex count "
                        f"{nonzero_z}; expected "
                        f"{expected_nonzero_z_vertex_count}")
            else:
                comparator(row)
        result = {"entity": entity, "count": len(rows),
                  "comparedBy": "handle", "semanticFieldsMatched": True}
        if entity == "POLYLINE_3D":
            result["vertexCount"] = expected_vertex_count
            result["recordsWithNonzeroVertexZ"] = nonzero_z
        elif entity == "LINE":
            nonzero_z = sum(
                any(finite_number(point[2], "LINE.endpoint.z") != 0.0
                    for point in (row["external"]["start"],
                                  row["external"]["end"]))
                for row in rows)
            if (expected_nonzero_z_line_count is not None
                    and nonzero_z != expected_nonzero_z_line_count):
                raise OracleError(
                    "unexpected nonzero-Z LINE count "
                    f"{nonzero_z}; expected {expected_nonzero_z_line_count}")
            result["recordsWithNonzeroEndpointZ"] = nonzero_z
        results.append(result)

    version_result = subprocess.run([args.dwgread, "--version"],
                                    capture_output=True, text=True, check=False)
    oracle_version = (version_result.stdout.strip()
                      if version_result.returncode == 0 else "unknown")
    layout_authority = {
        "3DFACE": "ODA v5.4.1 §20.4.32",
        "LINE": "ODA v5.4.1 §20.4.21",
        "POLYLINE_3D": "ODA v5.4.1 §§20.4.12, 20.4.17",
        "INSERT": "ODA v5.4.1 §§20.4.9-20.4.10",
        "SPLINE": "ODA v5.4.1 §20.4.40",
    }
    print(json.dumps({
        "result": "matched",
        "dwgVersion": version,
        "sample": args.input.name,
        "independentReader": oracle_version,
        "rows": results,
        "layoutAuthority": {row["entity"]: layout_authority[row["entity"]]
                            for row in results},
        "sampleSource": sample_source,
        "excluded": excluded,
        "claimBoundary": "read-field comparison on this sample only; no writer, "
                         "interoperability, or general version support claim",
    }, sort_keys=True, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, OracleError) as error:
        print(f"DWG consumer oracle: {error}", file=sys.stderr)
        raise SystemExit(1)
