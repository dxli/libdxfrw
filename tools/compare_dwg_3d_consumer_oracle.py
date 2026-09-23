#!/usr/bin/env python3
"""Compare a narrow DWG callback subset with LibreDWG's JSON reader.

This is an optional local qualification helper, not a CTest dependency. It
compares fields by entity handle so callback order is irrelevant. The accepted
scope is intentionally limited to INSERT placement and SPLINE fit data from
an AC1024 (R2010) sample; modeler and surface fields are not compared here.
"""

from __future__ import annotations

import argparse
import json
import math
import pathlib
import subprocess
import sys
from typing import Any


class OracleError(RuntimeError):
    pass


def run_json(command: list[str], label: str) -> dict[str, Any]:
    result = subprocess.run(command, capture_output=True, text=True, check=False)
    if result.returncode != 0:
        detail = result.stderr.strip() or result.stdout.strip()
        raise OracleError(f"{label} failed ({result.returncode}): {detail}")
    try:
        value = json.loads(result.stdout)
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


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--adapter", required=True,
                        help="standalone libdxfrw semantic adapter executable")
    parser.add_argument("--dwgread", default="dwgread",
                        help="LibreDWG dwgread executable (default: PATH lookup)")
    parser.add_argument("--input", required=True, type=pathlib.Path,
                        help="existing AC1024/R2010 DWG sample; never modified")
    args = parser.parse_args()

    with args.input.open("rb") as source:
        version = source.read(6).decode("ascii", errors="replace")
    if version != "AC1024":
        raise OracleError(f"expected AC1024/R2010 sample, got {version!r}")

    external = run_json([args.dwgread, "-O", "minJSON", str(args.input)],
                        "LibreDWG dwgread")
    adapter = run_json([args.adapter, "--input", str(args.input),
                        "--facade", "dwgRW"], "libdxfrw semantic adapter")

    results = []
    for entity, count, comparator in (
        ("INSERT", 6, compare_insert),
        ("SPLINE", 2, compare_spline),
    ):
        rows = compare_entity_set(index_external(external, entity),
                                  index_adapter(adapter, entity), entity, count)
        for row in rows:
            comparator(row)
        results.append({"entity": entity, "count": len(rows),
                        "comparedBy": "handle", "semanticFieldsMatched": True})

    version_result = subprocess.run([args.dwgread, "--version"],
                                    capture_output=True, text=True, check=False)
    oracle_version = (version_result.stdout.strip()
                      if version_result.returncode == 0 else "unknown")
    print(json.dumps({
        "result": "matched",
        "dwgVersion": version,
        "sample": args.input.name,
        "independentReader": oracle_version,
        "rows": results,
        "excluded": ["PLANESURFACE", "3DSOLID", "REGION", "BODY",
                     "MESH", "other versions and families"],
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
