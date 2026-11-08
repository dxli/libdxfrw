#!/usr/bin/env python3
"""Hash-bind libdxfrw's retained-frame/common-link diagnostic for one DWG."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
COVER_AC1027_SHA256 = (
    "3a6c23151ebec25320722f0ecaaf20a10073c60079d3616181395f1c73deee42"
)
sys.path.insert(0, str(REPO / "tools"))
from bounded_process import run_bounded  # noqa: E402

SUMMARY = re.compile(
    r"^opaqueSourceFrameInventory schema=(\d+) sourceVersion=(\S+) "
    r"sourceCallbacks=(\d+) captured=(\d+) retained=(\d+) "
    r"standardCaptured=(\d+).*?sourceHashBoundByUtility=(\d+)$",
    re.MULTILINE,
)
FRAME = re.compile(
    r"^ (opaque|standard) handle=(\d+) type=(\d+) record=(\S*) "
    r"class=(\S*) custom=(\d+) "
    r"(?:payloadMatched=\d+ opaquePayload=\d+ )?"
    r"owner=(\d+) blockOwner=(\d+) "
    r"xdict=(\d+) reactors=([^ ]*) commonEvidence=(\d+) "
    r"offset=(\d+) size=(\d+) dataStorage=(\d+)$",
    re.MULTILINE,
)
BODY_FRAME = re.compile(
    r"^ typedBodyFrame type=(\S+) handle=(\d+)(?: |$)", re.MULTILINE
)
MATERIAL_PROFILE = re.compile(
    r"^ typedBodyFrame type=MATERIAL handle=(\d+) .*?"
    r"semanticFieldsComplete=(\d+) rawTailValidated=(\d+) .*?"
    r"dictionaryKey=(ByLayer|ByBlock|Global) bodyHandleFields=0 "
    r"bodyReferenceRule=LIBREDWG-dwg2\.spec-MATERIAL$",
    re.MULTILINE,
)
MATERIAL_FIELDS = re.compile(
    r"^ typedMaterialFields handle=(\d+) name=(ByLayer|ByBlock|Global) "
    r"rawVersion=17 ambient=0/1/0 diffuse=0/1/0 refractionSource=0 "
    r"channelFlags=127 mapModels=diffuse,specular,reflection,opacity,bump,refraction$",
    re.MULTILINE,
)
NO_BODY_HANDLE_PROFILE = re.compile(
    r"^ typedBodyFrame type=(SCALE|DICTIONARYVAR|RASTERVARIABLES|MLINESTYLE|ACDBPLACEHOLDER|VISUALSTYLE|MATERIAL|SUN) "
    r"handle=(\d+) .*?"
    r"bodyHandleFields=0 bodyReferenceRule="
    r"(ODA-20\.4\.(?:92|74|91|73)|LIBREDWG-dwg2\.spec-(?:PLACEHOLDER|VISUALSTYLE|MATERIAL|SUN))$",
    re.MULTILINE,
)
HANDLE_BODY_PROFILE = re.compile(
    r"^ typedBodyFrame type=(LAYOUT|DICTIONARY|DICTIONARYWDFLT) handle=(\d+) .*?"
    r"bodyHandleFields=(\d+) bodyReferenceRule=(ODA-20\.4\.(?:84|44|45))"
    r"(?: defaultEntryRole=hard-pointer)?$",
    re.MULTILINE,
)
DICTIONARY_MEMBERSHIP = re.compile(
    r"^ typedBodyFrame type=(SCALE|DICTIONARYVAR|MLINESTYLE|ACDBPLACEHOLDER|MATERIAL) handle=(\d+) .*?"
    r"dictionaryOwner=(\d+) dictionaryKey=(.*?) "
    r"bodyHandleFields=0 bodyReferenceRule="
    r"(ODA-20\.4\.(?:92|74|73)|LIBREDWG-dwg2\.spec-(?:PLACEHOLDER|MATERIAL))$",
    re.MULTILINE,
)
NOD_ENTRY = re.compile(r"^ NOD key=(.*?) target=(\d+)(?: |$)", re.MULTILINE)
BODY_REFERENCE = re.compile(
    r"^ typedBodyReference owner=(\d+) (?:role=(.*?) )?target=(\d+)$",
    re.MULTILINE,
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def peer_handle(value: object) -> int | None:
    if isinstance(value, list) and value:
        tail = value[-1]
        return int(tail) if isinstance(tail, int) else None
    return None


def verify_peer_profiles(
    peer_json_path: Path,
    peer_schema_path: Path,
    source_hash: str,
    frames: list[dict[str, object]],
) -> dict[str, object]:
    peer_json = json.loads(peer_json_path.read_text())
    peer_objects = peer_json.get("OBJECTS")
    if not isinstance(peer_objects, list):
        raise SystemExit("LibreDWG minJSON has no OBJECTS array")
    peer_by_handle: dict[int, dict[str, object]] = {}
    for obj in peer_objects:
        if not isinstance(obj, dict):
            continue
        handle = peer_handle(obj.get("handle"))
        if handle is None or int(obj.get("type", -1)) not in (73, 80, 501, 502, 506):
            continue
        if handle in peer_by_handle:
            raise SystemExit("LibreDWG minJSON repeats a profiled object handle")
        peer_by_handle[handle] = obj

    names = {
        73: "MLINESTYLE", 80: "ACDBPLACEHOLDER",
        501: "VISUALSTYLE", 502: "MATERIAL", 506: "SUN",
    }
    rows = []
    for frame in frames:
        object_type = int(frame["encodedType"])
        if object_type not in names:
            continue
        obj = peer_by_handle.get(int(frame["handle"]))
        expected_object = {
            73: "MLINESTYLE",
            80: "PLACEHOLDER",
            501: "VISUALSTYLE",
            502: "MATERIAL",
            506: "SUN",
        }[object_type]
        expected_dxf = {
            73: None,
            80: "ACDBPLACEHOLDER",
            501: None,
            502: None,
            506: None,
        }[object_type]
        reactors = obj.get("reactors", []) if obj else []
        peer_reactors = [
            handle for handle in (peer_handle(item) for item in reactors)
            if handle is not None
        ]
        peer_owner = peer_handle(obj.get("ownerhandle")) if obj else None
        if (
            obj is None
            or obj.get("object") != expected_object
            or obj.get("dxfname") != expected_dxf
            or int(obj.get("size", -1)) != int(frame["objectSize"])
            or peer_owner != int(frame["ownerHandle"])
            or peer_reactors != frame["reactorHandles"]
        ):
            raise SystemExit(
                f"LibreDWG minJSON identity/common-link mismatch for "
                f"{names[object_type]} handle {frame['handle']}"
            )
        rows.append({
            "handle": int(frame["handle"]),
            "encodedType": object_type,
            "object": expected_object,
            "dxfName": expected_dxf,
            "size": int(obj["size"]),
            "ownerHandle": peer_owner,
            "reactorHandles": peer_reactors,
            "matchesLibdxfrwFrame": True,
        })

    schema_text = peer_schema_path.read_text(errors="replace")
    schema_blocks: dict[str, str] = {}
    for name in ("PLACEHOLDER", "VISUALSTYLE", "SUN"):
        start = schema_text.find(f"DWG_OBJECT ({name})")
        end = schema_text.find("DWG_OBJECT_END", start)
        if start < 0 or end < 0:
            raise SystemExit(f"LibreDWG dwg2.spec lacks {name} object block")
        block = schema_text[start:end]
        body = block.split("START_OBJECT_HANDLE_STREAM", 1)[0]
        if "START_OBJECT_HANDLE_STREAM" not in block or re.search(
            r"\b(?:FIELD_H|HANDLE_VECTOR)\b", body
        ):
            raise SystemExit(
                f"LibreDWG {name} body schema has handle fields before the common handle stream"
            )
        if name == "PLACEHOLDER" and "no own data members" not in body:
            raise SystemExit("LibreDWG PLACEHOLDER schema no longer declares an empty body")
        schema_blocks[name] = hashlib.sha256(block.encode()).hexdigest()

    material_macro_start = schema_text.find("#define MAT_COLOR")
    material_start = schema_text.find("DWG_OBJECT (MATERIAL)", material_macro_start)
    material_end = schema_text.find("DWG_OBJECT_END", material_start)
    if min(material_macro_start, material_start, material_end) < 0:
        raise SystemExit("LibreDWG dwg2.spec lacks MATERIAL body macros/object")
    material_block = schema_text[material_macro_start:material_end]
    material_body = material_block.split("START_OBJECT_HANDLE_STREAM", 1)[0]
    if "START_OBJECT_HANDLE_STREAM" not in material_block or re.search(
        r"\b(?:FIELD_H|HANDLE_VECTOR)\b", material_body
    ):
        raise SystemExit(
            "LibreDWG MATERIAL color/map schema has body-handle fields before common handles"
        )
    schema_blocks["MATERIAL"] = hashlib.sha256(
        material_block.encode()
    ).hexdigest()

    source_types = Counter(int(frame["encodedType"]) for frame in frames)
    peer_types = Counter(int(obj.get("type", -1)) for obj in peer_by_handle.values())
    if source_hash == COVER_AC1027_SHA256 and any(
        peer_types[object_type] != source_types[object_type]
        for object_type in (73, 80, 501, 502, 506)
    ):
        raise SystemExit("LibreDWG minJSON profile frame counts differ from Cover source inventory")
    return {
        "createdBy": peer_json.get("created_by"),
        "objectCountsByEncodedType": {
            str(object_type): peer_types[object_type]
            for object_type in (73, 80, 501, 502, 506)
        },
        "matchedFrames": rows,
        "schemaPath": str(peer_schema_path),
        "schemaSha256": sha256(peer_schema_path),
        "schemaBlockSha256": schema_blocks,
        "schemaBodyHandleFieldsBeforeCommonStream": False,
        "sourceSha256": source_hash,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--executable", required=True, type=Path)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--peer-json", type=Path)
    parser.add_argument("--peer-schema", type=Path)
    parser.add_argument("--timeout", type=float, default=30.0)
    args = parser.parse_args()
    executable = args.executable.resolve(strict=True)
    source = args.source.resolve(strict=True)
    if not executable.is_file() or not source.is_file():
        parser.error("executable and source must be regular files")
    if (args.peer_json is None) != (args.peer_schema is None):
        parser.error("--peer-json and --peer-schema must be supplied together")

    source_before = sha256(source)
    result = run_bounded(
        [str(executable), "--opaque-dwg-source-inventory", str(source)],
        timeout=args.timeout,
        output_limit=16 * 1024 * 1024,
        cwd=REPO,
    )
    source_after = sha256(source)
    combined = (result.stdout + b"\n" + result.stderr).decode(
        "utf-8", errors="replace"
    )
    summary = SUMMARY.search(combined)
    if result.returncode != 0 or source_before != source_after or summary is None:
        sys.stderr.write(combined[-12000:])
        raise SystemExit("source inventory failed, changed its input, or lacked its summary")

    frames = []
    for match in FRAME.finditer(combined):
        kind, handle, object_type, record, class_name, custom, owner, block_owner, xdict, reactors, evidence, offset, size, data_storage = match.groups()
        frames.append({
            "captureKind": kind,
            "handle": int(handle),
            "encodedType": int(object_type),
            "recordName": record or None,
            "className": class_name or None,
            "customClass": bool(int(custom)),
            "ownerHandle": int(owner),
            "blockOwnerHandle": int(block_owner),
            "extensionDictionaryHandle": int(xdict),
            "reactorHandles": [int(item) for item in reactors.split(",") if item],
            "commonLinkEvidence": int(evidence),
            "sourceOffset": int(offset),
            "objectSize": int(size),
            "hasDataStorage": bool(int(data_storage)),
        })
    if len({frame["handle"] for frame in frames}) != len(frames):
        raise SystemExit("source inventory contains duplicate captured frame handles")

    typed_frames = {
        int(handle): name for name, handle in BODY_FRAME.findall(combined)
    }
    no_body_handle_profiles = {
        int(handle): rule
        for _record_type, handle, rule in NO_BODY_HANDLE_PROFILE.findall(combined)
    }
    handle_body_profiles = {
        int(handle): {"rule": rule, "fieldCount": int(field_count),
                      "objectType": record_type}
        for record_type, handle, field_count, rule
        in HANDLE_BODY_PROFILE.findall(combined)
    }
    dictionary_memberships = {
        int(handle): {
            "objectType": record_type,
            "dictionaryHandle": int(dictionary),
            "dictionaryKey": key,
            "bodyReferenceRule": rule,
        }
        for record_type, handle, dictionary, key, rule
        in DICTIONARY_MEMBERSHIP.findall(combined)
    }
    dictionary_membership_edges = [
        {"sourceFrameHandle": handle, **membership, "targetHandle": handle}
        for handle, membership in sorted(dictionary_memberships.items())
    ]
    named_object_dictionary_entries = [
        {"key": key, "targetHandle": int(target)}
        for key, target in NOD_ENTRY.findall(combined)
    ]
    typed_references = []
    for owner, role, target in BODY_REFERENCE.findall(combined):
        typed_references.append({
            "ownerHandle": int(owner),
            "role": role or "unlabeled",
            "targetHandle": int(target),
        })
    reference_counts = Counter(item["ownerHandle"] for item in typed_references)
    retained_handles = {frame["handle"] for frame in frames}
    for frame in frames:
        frame["typedBodyReport"] = typed_frames.get(frame["handle"])
        frame["reportedTypedBodyReferenceCount"] = reference_counts[frame["handle"]]
        no_body_rule = no_body_handle_profiles.get(frame["handle"])
        frame["bodyReferenceDisposition"] = (
            "peer-profile-no-body-handles"
            if no_body_rule is not None and no_body_rule.startswith("LIBREDWG-")
            else "spec-profile-no-body-handles"
            if no_body_rule is not None
            else "spec-profile-handle-references"
            if frame["handle"] in handle_body_profiles
            else "selected-typed-report-only"
            if frame["handle"] in typed_frames or reference_counts[frame["handle"]]
            else "not-exhaustively-decoded"
        )
        frame["bodyReferenceRule"] = no_body_handle_profiles.get(frame["handle"])
        if frame["handle"] in handle_body_profiles:
            handle_profile = handle_body_profiles[frame["handle"]]
            frame["bodyReferenceRule"] = handle_profile["rule"]
            frame["bodyHandleFieldCount"] = handle_profile["fieldCount"]
        frame["dictionaryMembership"] = dictionary_memberships.get(frame["handle"])
        frame["ownerTargetsRetainedObjectFrame"] = (
            frame["ownerHandle"] in retained_handles
            if frame["ownerHandle"] not in (0, 0xFFFFFFFF) else None
        )

    schema, version, callbacks, captured, opaque_retained, standard_retained, utility_bound = summary.groups()
    declared_frames = int(opaque_retained) + int(standard_retained)
    if int(schema) != 2 or len(frames) != declared_frames or declared_frames != int(captured):
        raise SystemExit(
            f"captured identity rows mismatch: rows={len(frames)} "
            f"declared={declared_frames} callbacksCaptured={captured}"
        )
    peer_evidence = None
    if args.peer_json is not None and args.peer_schema is not None:
        peer_evidence = verify_peer_profiles(
            args.peer_json.resolve(strict=True),
            args.peer_schema.resolve(strict=True),
            source_before,
            frames,
        )
    if version == "AC1027":
        fixed_mline_styles = [
            frame for frame in frames if frame["encodedType"] == 73
        ]
        for frame in fixed_mline_styles:
            membership = frame["dictionaryMembership"]
            if (
                frame["typedBodyReport"] != "MLINESTYLE"
                or frame["bodyReferenceDisposition"]
                != "spec-profile-no-body-handles"
                or frame["bodyReferenceRule"] != "ODA-20.4.73"
                or membership is None
                or membership["objectType"] != "MLINESTYLE"
                or membership["dictionaryHandle"] != frame["ownerHandle"]
                or not any(
                    edge["sourceFrameHandle"] == frame["handle"]
                    and edge["dictionaryHandle"] == frame["ownerHandle"]
                    and edge["targetHandle"] == frame["handle"]
                    for edge in dictionary_membership_edges
                )
            ):
                raise SystemExit(
                    "AC1027 type-73 frame lacks its typed identity, ODA body "
                    "profile, or exact ACAD_MLINESTYLE membership edge"
                )
        visual_styles = [
            frame for frame in frames if frame["encodedType"] == 501
        ]
        for frame in visual_styles:
            if (
                frame["className"] != "AcDbVisualStyle"
                or frame["typedBodyReport"] != "VISUALSTYLE"
                or frame["bodyReferenceDisposition"]
                != "peer-profile-no-body-handles"
                or frame["bodyReferenceRule"]
                != "LIBREDWG-dwg2.spec-VISUALSTYLE"
            ):
                raise SystemExit(
                    "AC1027 VISUALSTYLE frame lacks its typed identity "
                    "or peer-supported no-body-handle profile"
                )
        materials = [
            frame for frame in frames if frame["encodedType"] == 502
        ]
        material_profiles = {
            int(handle): (int(semantic), int(raw_tail), key)
            for handle, semantic, raw_tail, key in MATERIAL_PROFILE.findall(
                combined
            )
        }
        material_fields = {
            int(handle): name
            for handle, name in MATERIAL_FIELDS.findall(combined)
        }
        for frame in materials:
            membership = frame["dictionaryMembership"]
            if (
                frame["className"] != "AcDbMaterial"
                or frame["typedBodyReport"] != "MATERIAL"
                or frame["bodyReferenceDisposition"]
                != "peer-profile-no-body-handles"
                or frame["bodyReferenceRule"] != "LIBREDWG-dwg2.spec-MATERIAL"
                or membership is None
                or membership["objectType"] != "MATERIAL"
                or membership["dictionaryHandle"] != frame["ownerHandle"]
                or material_profiles.get(int(frame["handle"]))
                != (1, 1, membership["dictionaryKey"])
                or material_fields.get(int(frame["handle"]))
                != membership["dictionaryKey"]
                or not any(
                    edge["sourceFrameHandle"] == frame["handle"]
                    and edge["dictionaryHandle"] == frame["ownerHandle"]
                    and edge["targetHandle"] == frame["handle"]
                    for edge in dictionary_membership_edges
                )
            ):
                raise SystemExit(
                    "AC1027 MATERIAL frame lacks its typed semantic profile, "
                    "peer body profile, or exact ACAD_MATERIAL membership"
                )
        suns = [frame for frame in frames if frame["encodedType"] == 506]
        sun_reports = {
            int(handle): rule
            for _object_type, handle, rule
            in NO_BODY_HANDLE_PROFILE.findall(combined)
            if rule == "LIBREDWG-dwg2.spec-SUN"
        }
        for frame in suns:
            sun_owner_refs = [
                edge for edge in typed_references
                if edge["ownerHandle"] == frame["ownerHandle"]
                and edge["role"] == "sun"
                and edge["targetHandle"] == frame["handle"]
            ]
            if (
                frame["className"] != "AcDbSun"
                or frame["typedBodyReport"] != "SUN"
                or frame["bodyReferenceDisposition"]
                != "peer-profile-no-body-handles"
                or frame["bodyReferenceRule"] != "LIBREDWG-dwg2.spec-SUN"
                or sun_reports.get(int(frame["handle"]))
                != "LIBREDWG-dwg2.spec-SUN"
                or frame["reactorHandles"]
                or frame["extensionDictionaryHandle"] != 0
                or len(sun_owner_refs) != 1
            ):
                raise SystemExit(
                    "AC1027 SUN lacks its source-matched no-body-handle profile "
                    "or exact VPORT hard-owner reference"
                )
            if source_before == COVER_AC1027_SHA256 and (
                frame["handle"] != 109 or frame["ownerHandle"] != 41
                or sun_owner_refs[0]["ownerHandle"] != 41
            ):
                raise SystemExit(
                    "hash-bound Cover SUN/VPORT owner identities changed"
                )
        default_dictionaries = [
            frame for frame in frames if frame["encodedType"] == 500
        ]
        default_profiles = {
            int(handle): int(fields)
            for object_type, handle, fields, _rule
            in HANDLE_BODY_PROFILE.findall(combined)
            if object_type == "DICTIONARYWDFLT"
        }
        for frame in default_dictionaries:
            dictionary_refs = [
                edge for edge in typed_references
                if edge["ownerHandle"] == frame["handle"]
                and edge["role"].startswith("dictionary-entry key=")
            ]
            default_refs = [
                edge for edge in typed_references
                if edge["ownerHandle"] == frame["handle"]
                and edge["role"] == "default-entry"
            ]
            if (
                frame["recordName"] != "ACDBDICTIONARYWDFLT"
                or frame["className"] != "AcDbDictionaryWithDefault"
                or frame["typedBodyReport"] != "DICTIONARYWDFLT"
                or frame["bodyReferenceDisposition"]
                != "spec-profile-handle-references"
                or frame["bodyReferenceRule"] != "ODA-20.4.45"
                or default_profiles.get(int(frame["handle"]))
                != len(dictionary_refs) + 1
                or len(default_refs) != 1
                or not any(
                    edge["targetHandle"] == default_refs[0]["targetHandle"]
                    for edge in dictionary_refs
                )
            ):
                raise SystemExit(
                    "AC1027 DICTIONARYWDFLT lacks ODA default-handle field "
                    "or matching dictionary-entry role evidence: "
                    f"handle={frame['handle']} name={frame['recordName']} "
                    f"class={frame['className']} "
                    f"report={frame['typedBodyReport']} "
                    f"disposition={frame['bodyReferenceDisposition']} "
                    f"rule={frame['bodyReferenceRule']} "
                    f"profile={default_profiles.get(int(frame['handle']))} "
                    f"dictRefs={len(dictionary_refs)} "
                    f"defaultRefs={len(default_refs)}"
                )
            if source_before == COVER_AC1027_SHA256 and (
                frame["handle"] != 14 or frame["ownerHandle"] != 12
                or frame["reactorHandles"] != [12]
                or default_refs[0]["targetHandle"] != 15
                or [entry for entry in named_object_dictionary_entries
                    if entry["targetHandle"] == 14]
                    != [{"key": "ACAD_PLOTSTYLENAME", "targetHandle": 14}]
            ):
                raise SystemExit(
                    "hash-bound Cover DICTIONARYWDFLT common/default roles changed"
                )
        placeholders = [
            frame for frame in frames if frame["encodedType"] == 80
        ]
        if source_before == COVER_AC1027_SHA256 and len(placeholders) != 1:
            raise SystemExit(
                "hash-bound Cover source did not publish exactly one type-80 frame"
            )
        for frame in placeholders:
            if frame["bodyReferenceDisposition"] != "peer-profile-no-body-handles":
                if source_before == COVER_AC1027_SHA256:
                    raise SystemExit(
                        "hash-bound Cover type-80 frame lacks its peer profile"
                    )
                continue
            membership = frame["dictionaryMembership"]
            if (
                frame["handle"] != 15
                or frame["typedBodyReport"] != "ACDBPLACEHOLDER"
                or frame["bodyReferenceDisposition"]
                != "peer-profile-no-body-handles"
                or frame["bodyReferenceRule"]
                != "LIBREDWG-dwg2.spec-PLACEHOLDER"
                or membership is None
                or membership["objectType"] != "ACDBPLACEHOLDER"
                or membership["dictionaryHandle"] != frame["ownerHandle"]
                or not any(
                    edge["sourceFrameHandle"] == frame["handle"]
                    and edge["dictionaryHandle"] == frame["ownerHandle"]
                    and edge["targetHandle"] == frame["handle"]
                    for edge in dictionary_membership_edges
                )
                or not any(
                    item["ownerHandle"] == frame["ownerHandle"]
                    and item["targetHandle"] == frame["handle"]
                    and item["role"] == "default-entry"
                    for item in typed_references
                )
            ):
                raise SystemExit(
                    "AC1027 type-80 frame lacks its source-matched "
                    "PLACEHOLDER peer profile or exact dictionary roles"
                )

    report = {
        "schema": "libdxfrw-ac1027-source-reference-audit-v1",
        "verifier": str(Path(__file__).resolve().relative_to(REPO)),
        "verifierSha256": sha256(Path(__file__).resolve()),
        "executable": str(executable),
        "executableSha256": sha256(executable),
        "source": str(source),
        "sourceSha256Before": source_before,
        "sourceSha256After": source_after,
        "sourceUnchangedDuringRun": source_before == source_after,
        "diagnosticUtilityHashBindsInput": bool(int(utility_bound)),
        "verifierBindsUnchangedSourceBytes": source_before == source_after,
        "command": [str(executable), "--opaque-dwg-source-inventory", str(source)],
        "diagnosticOutputSha256": hashlib.sha256(
            result.stdout + b"\n" + result.stderr
        ).hexdigest(),
        "exitCode": result.returncode,
        "sourceVersion": version,
        "independentPeerEvidence": peer_evidence,
        "opaqueCallbacks": int(callbacks),
        "capturedOpaqueCallbacks": int(captured),
        "retainedOpaqueFrames": int(opaque_retained),
        "retainedStandardObjects": int(standard_retained),
        "capturedFrameIdentities": len(frames),
        "typedBodyReferenceRows": len(typed_references),
        "framesWithSelectedTypedBodyReport": sum(
            frame["bodyReferenceDisposition"] == "selected-typed-report-only"
            for frame in frames
        ),
        "framesWithoutExhaustiveBodyReferenceDecode": sum(
            frame["bodyReferenceDisposition"] == "not-exhaustively-decoded"
            for frame in frames
        ),
        "framesWithPeerNoBodyHandleProfile": sum(
            frame["bodyReferenceDisposition"] == "peer-profile-no-body-handles"
            for frame in frames
        ),
        "typedBodyReferences": typed_references,
        "dictionaryMembershipEdges": dictionary_membership_edges,
        "namedObjectDictionaryEntries": named_object_dictionary_entries,
        "capturedFrames": frames,
        "coverageLimit": (
            "This binds the diagnostic run to unchanged input bytes and verifies "
            "the emitted identity inventory. ODA profiles and source-matched "
            "LibreDWG peer profiles are kept distinct. Typed body rows outside "
            "those bounded profiles are selected reports only; "
            "not-exhaustively-decoded does not mean no references. Common "
            "header values are not independent role proof, and no DXF output route "
            "or semantic support claim is qualified."
        ),
    }
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
