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
NO_BODY_HANDLE_PROFILE = re.compile(
    r"^ typedBodyFrame type=(SCALE|DICTIONARYVAR|RASTERVARIABLES) "
    r"handle=(\d+) .*?"
    r"bodyHandleFields=0 bodyReferenceRule=(ODA-20\.4\.(?:92|74|91))$",
    re.MULTILINE,
)
HANDLE_BODY_PROFILE = re.compile(
    r"^ typedBodyFrame type=(LAYOUT|DICTIONARY) handle=(\d+) .*?"
    r"bodyHandleFields=(\d+) bodyReferenceRule=(ODA-20\.4\.(?:84|44))$",
    re.MULTILINE,
)
DICTIONARY_MEMBERSHIP = re.compile(
    r"^ typedBodyFrame type=(SCALE|DICTIONARYVAR) handle=(\d+) .*?"
    r"dictionaryOwner=(\d+) dictionaryKey=(.*?) "
    r"bodyHandleFields=0 bodyReferenceRule=(ODA-20\.4\.(?:92|74))$",
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


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--executable", required=True, type=Path)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--timeout", type=float, default=30.0)
    args = parser.parse_args()
    executable = args.executable.resolve(strict=True)
    source = args.source.resolve(strict=True)
    if not executable.is_file() or not source.is_file():
        parser.error("executable and source must be regular files")

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
        frame["bodyReferenceDisposition"] = (
            "spec-profile-no-body-handles"
            if frame["handle"] in no_body_handle_profiles
            else "spec-profile-handle-references"
            if frame["handle"] in handle_body_profiles
            else "selected-typed-report-only"
            if frame["handle"] in typed_frames or reference_counts[frame["handle"]]
            else "not-exhaustively-decoded"
        )
        frame["bodyReferenceRule"] = no_body_handle_profiles.get(frame["handle"])
        if frame["handle"] in handle_body_profiles:
            frame["bodyReferenceRule"] = handle_body_profiles[frame["handle"]]
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
        "typedBodyReferences": typed_references,
        "dictionaryMembershipEdges": dictionary_membership_edges,
        "namedObjectDictionaryEntries": named_object_dictionary_entries,
        "capturedFrames": frames,
        "coverageLimit": (
            "This binds the diagnostic run to unchanged input bytes and verifies "
            "the emitted identity inventory. Typed body rows are selected reports "
            "only; not-exhaustively-decoded does not mean no references. Common "
            "header values are not independent role proof, and no DXF output route "
            "or semantic support claim is qualified."
        ),
    }
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
