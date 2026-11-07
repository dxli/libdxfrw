#!/usr/bin/env python3
"""Decode Cover TABLESTYLE scalar/string/handle spans from retained raw bytes.

The diagnostic log contains the current reader's parser-reported bit ranges
and the exact decompressed object-frame bytes retained from the input DWG.
This verifier independently decodes values at those coordinates. It does not
independently discover the coordinates or establish physical file offsets.
"""

import argparse
import hashlib
import json
import re
from pathlib import Path


SOURCE_SHA256 = "3a6c23151ebec25320722f0ecaaf20a10073c60079d3616181395f1c73deee42"
MAX_FRAME_BYTES = 1024 * 1024


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


class Bits:
    def __init__(self, data):
        self.data = data
        self.total = len(data) * 8

    def read(self, position, count):
        require(position >= 0 and count >= 0
                and position + count <= self.total,
                "bit read escapes raw source frame")
        value = 0
        for bit in range(position, position + count):
            value = (value << 1) | (
                (self.data[bit // 8] >> (7 - bit % 8)) & 1)
        return value

    def bit_long(self, start):
        tag = self.read(start, 2)
        if tag == 0:
            return int.from_bytes(
                bytes(self.read(start + 2 + i * 8, 8) for i in range(4)),
                "little", signed=True), start + 34
        if tag == 1:
            return self.read(start + 2, 8), start + 10
        if tag == 2:
            return 0, start + 2
        raise ValueError("BL reserved selector encountered")

    def bit_short(self, start):
        tag = self.read(start, 2)
        if tag == 0:
            value = int.from_bytes(
                bytes(self.read(start + 2 + i * 8, 8) for i in range(2)),
                "little", signed=False)
            return value, start + 18
        if tag == 1:
            return self.read(start + 2, 8), start + 10
        if tag == 2:
            return 0, start + 2
        return 256, start + 2

    def variable_text(self, start):
        units, cursor = self.bit_short(start)
        require(units <= 0x7fff, "TU length exceeds profile bound")
        raw = bytes(self.read(cursor + index * 8, 8)
                    for index in range(units * 2))
        cursor += units * 16
        return raw.decode("utf-16le", errors="strict"), cursor

    def handle(self, start, owner):
        code = self.read(start, 4)
        length = self.read(start + 4, 4)
        require(length <= 8, "handle payload exceeds 64-bit profile")
        raw = bytes(self.read(start + 8 + index * 8, 8)
                    for index in range(length))
        value = int.from_bytes(raw, "big") if raw else 0
        if code == 0x0C:
            value = owner - value
        elif code == 0x0A:
            value = owner + value
        elif code == 0x08:
            value = owner - 1
        elif code == 0x06:
            value = owner + 1
        return value, start + 8 + length * 8


def span_rows(log):
    rows = []
    pattern = re.compile(
        r"^ typedBodySpan owner=(\d+) streamField=(\S+) startBit=(\d+) "
        r"bitSize=(\d+) valueOrIndex=(\d+) complete=(\d+)$", re.MULTILINE)
    for match in pattern.finditer(log):
        rows.append({
            "owner": int(match[1]), "field": match[2],
            "start": int(match[3]), "size": int(match[4]),
            "value": int(match[5]), "complete": match[6] == "1",
        })
    return rows


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", required=True)
    parser.add_argument("--inventory-log", required=True)
    parser.add_argument("--peer-json", required=True)
    parser.add_argument("--inventory-binary", required=True)
    args = parser.parse_args()

    source_hash = sha256(args.source)
    require(source_hash == SOURCE_SHA256,
            f"source digest mismatch: {source_hash}")
    log = Path(args.inventory_log).read_text(encoding="utf-8")
    raw_match = re.search(
        r"^ typedBodyRawFrame owner=106 byteSize=(\d+) "
        r"handleStreamBits=(\d+) dataStreamBits=(\d+) hex=([0-9a-f]+)$",
        log, re.MULTILINE)
    require(raw_match is not None, "raw TABLESTYLE frame not found")
    raw_size, handle_bits, data_bits = map(int, raw_match.group(1, 2, 3))
    raw = bytes.fromhex(raw_match[4])
    require(0 < raw_size <= MAX_FRAME_BYTES and len(raw) == raw_size,
            "raw frame size does not match capture metadata")
    bits = Bits(raw)
    require(data_bits + handle_bits == bits.total,
            "data and handle stream sizes do not cover the captured frame")
    rows = span_rows(log)
    require(len(rows) >= 40 and all(row["owner"] == 106
                                    and row["complete"]
                                    and row["size"] > 0 for row in rows),
            "TABLESTYLE span inventory is incomplete")

    peer_doc = json.loads(Path(args.peer_json).read_text(encoding="utf-8"))
    peer_matches = [
        obj for obj in peer_doc["OBJECTS"]
        if isinstance(obj.get("handle"), list)
        and obj["handle"][-1] == 106
    ]
    require(len(peer_matches) == 1,
            "peer JSON does not contain one TABLESTYLE 0x6A frame")
    peer = peer_matches[0]
    peer_unknown = bytes.fromhex(peer["unknown_bits"])
    peer_unknown_bits = peer["num_unknown_bits"]
    require(peer["object"] == "TABLESTYLE"
            and peer["size"] == raw_size
            and peer_unknown_bits == bits.total - 40
            and peer_unknown[:-1] == raw[5:]
            and peer_unknown[-1] == 0,
            "LibreDWG unknown-bit carrier does not match the raw frame tail")
    peer_bits = Bits(peer_unknown[:-1])
    peer_offset = 40

    def named(field):
        return sorted((row for row in rows if row["field"] == field),
                      key=lambda row: row["start"])

    table_ids = named("table-style-body-table-cell-id")
    table_classes = named("table-style-body-table-cell-class")
    repeated_ids = named("table-style-body-cell-id")
    repeated_classes = named("table-style-body-cell-class")
    table_names = named("table-style-string-table-cell-name")
    repeated_names = named("table-style-string-cell-name")
    require(len(table_ids) == len(table_classes) == len(table_names) == 1
            and len(repeated_ids) == len(repeated_classes)
            == len(repeated_names) == 3,
            "expected one Table and three repeated identities")

    decoded_ids = []
    for row in table_ids + repeated_ids:
        value, end = bits.bit_long(row["start"])
        peer_value, peer_end = peer_bits.bit_long(row["start"] - peer_offset)
        require(end == row["start"] + row["size"],
                "BL token length differs from reported span")
        require(peer_value == value and peer_end == end - peer_offset,
                "peer unknown-bit tail disagrees at a parser-reported BL span")
        decoded_ids.append(value)
    decoded_classes = []
    for row in table_classes + repeated_classes:
        value, end = bits.bit_long(row["start"])
        peer_value, peer_end = peer_bits.bit_long(row["start"] - peer_offset)
        require(end == row["start"] + row["size"],
                "BL token length differs from reported span")
        require(peer_value == value and peer_end == end - peer_offset,
                "peer unknown-bit tail disagrees at a parser-reported BL span")
        decoded_classes.append(value)
    decoded_names = []
    for row in table_names + repeated_names:
        value, end = bits.variable_text(row["start"])
        peer_value, peer_end = peer_bits.variable_text(
            row["start"] - peer_offset)
        require(end == row["start"] + row["size"],
                "TU token length differs from reported span")
        require(peer_value == value and peer_end == end - peer_offset,
                "peer unknown-bit tail disagrees at a parser-reported TU span")
        decoded_names.append(value)

    require(decoded_ids == [4, 1, 2, 3],
            f"raw IDs disagree with parser model: {decoded_ids}")
    require(decoded_classes == [2, 1, 1, 2],
            f"raw classes disagree with parser model: {decoded_classes}")
    require(decoded_names == ["Table", "_TITLE", "_HEADER", "_DATA"],
            f"raw names disagree with parser model: {decoded_names}")

    handle_spans = sorted(
        (row for row in rows if row["field"].startswith("table-style-handle-")),
        key=lambda row: row["start"])
    require(len(handle_spans) == 28,
            f"expected 28 handle spans; found {len(handle_spans)}")
    decoded_handles = []
    decoded_text_styles = []
    decoded_border_ltypes = []
    for row in handle_spans:
        require(row["start"] >= data_bits
                and row["start"] + row["size"] <= bits.total,
                "handle span is outside the captured handle stream")
        value, end = bits.handle(row["start"], 106)
        peer_value, peer_end = peer_bits.handle(
            row["start"] - peer_offset, 106)
        require(end == row["start"] + row["size"],
                "H token length differs from reported span")
        require(peer_value == value and peer_end == end - peer_offset,
                "peer unknown-bit tail disagrees at a parser-reported H span")
        decoded_handles.append(value)
        if row["field"] == "table-style-handle-text-style":
            decoded_text_styles.append(value)
        else:
            decoded_border_ltypes.append(value)
    require(decoded_text_styles == [0, 17, 17, 17],
            f"text-style raw handles disagree: {decoded_text_styles}")
    require(decoded_border_ltypes == [0] * 6 + [20] * 18,
            "border-linetype raw handles disagree with parsed source model")

    report = {
        "schema": "libdxfrw-cover-tablestyle-raw-span-check-v1",
        "sourceSha256": source_hash,
        "inventoryLogSha256": sha256(args.inventory_log),
        "peerJsonSha256": sha256(args.peer_json),
        "inventoryBinarySha256": sha256(args.inventory_binary),
        "verifierSha256": sha256(__file__),
        "rawFrameSha256": hashlib.sha256(raw).hexdigest(),
        "tableStyleHandle": 106,
        "rawFrameBytes": raw_size,
        "dataStreamBits": data_bits,
        "handleStreamBits": handle_bits,
        "decodedIds": decoded_ids,
        "decodedClasses": decoded_classes,
        "decodedNames": decoded_names,
        "decodedTextStyleHandles": decoded_text_styles,
        "decodedBorderLinetypeHandles": decoded_border_ltypes,
        "decodedHandleReferences": len(decoded_handles),
        "rawByteSpanChecks": len(decoded_ids) + len(decoded_classes)
            + len(decoded_names) + len(decoded_handles),
        "peerUnknownBits": peer_unknown_bits,
        "peerUnknownTailOffset": peer_offset,
        "peerUnknownTailMatchesRawFrame": True,
        "result": "PASS parser-reported bit coordinates decode to the expected raw frame values",
        "limit": (
            "The parser supplies field coordinates; this verifier decodes the "
            "captured raw frame at those coordinates. It does not independently "
            "discover field boundaries, prove physical compressed-file offsets, "
            "or resolve ODA's repeated-order/class-meaning discrepancy."
        ),
    }
    print(json.dumps(report, indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
