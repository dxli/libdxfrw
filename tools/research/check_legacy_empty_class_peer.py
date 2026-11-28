#!/usr/bin/env python3
"""Check scratch R13/R14 CLASS retention against source frames and DXF tags."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile

from audit_ac1012_class_census import census
from check_legacy_layout_xdictionary_peer import check as check_graph
from check_r2000_layout_projection_peer import physical_records, require
from verify_oda_legacy_layer_graph import class_frame

FAMILIES = {'GEODATA', 'RASTERVARIABLES', 'WIPEOUTVARIABLES'}


def output_classes(path, binary):
    result = []
    for record in physical_records(path, binary):
        if record.section != 'CLASSES' or record.type != 'CLASS':
            continue
        fields = {}
        for code, value in record.values:
            if code in (1, 2, 3, 90, 91, 280, 281):
                require(code not in fields, 'duplicate CLASS field')
                fields[code] = str(value)
        require(set(fields) == {1, 2, 3, 90, 280, 281}, 'unexpected R2000 CLASS shape')
        result.append(fields)
    return result


def check(source, directory, baseline, dwgread):
    with tempfile.TemporaryDirectory(prefix='legacy-class-peer-') as tmp:
        output = Path(tmp) / 'source.json'
        run = subprocess.run([dwgread, '-O', 'JSON', '-o', str(output), str(source)],
                             capture_output=True, check=False)
        require(run.returncode == 0 and output.is_file(), 'LibreDWG read failed')
        model = json.loads(output.read_text())
    source_classes = [row for row in model['CLASSES'] if row.get('dxfname') in FAMILIES]
    require(len(source_classes) == 3 and {row['dxfname'] for row in source_classes} == FAMILIES,
            'source family classes missing or duplicated')
    frames = census(source.read_bytes())['frames']
    expected = []
    for row in source_classes:
        require(not any(frame['type'] == row['number'] for frame in frames),
                'source family has a mapped instance')
        proof = class_frame(source, model['CLASSES'], row['number'])
        require(proof['selected']['dxf'] == row['dxfname']
                and proof['selected']['cpp'] == row['cppname'], 'source class frame mismatch')
        expected.append({1: row['dxfname'], 2: row['cppname'], 3: row['appname'],
                         90: str(row['proxyflag']), 280: str(row['is_zombie']), 281: '0'})
    for encoding in ('ascii', 'binary'):
        path = directory / (encoding + '.dxf')
        before = output_classes(baseline / (encoding + '.dxf'), encoding == 'binary')
        after = output_classes(path, encoding == 'binary')
        require(after == before + expected, 'output CLASS metadata or prior ordinals differ')
        check_graph(path, encoding == 'binary', True)
    return expected


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output-dir', type=Path, required=True)
    parser.add_argument('--baseline-dir', type=Path, required=True)
    parser.add_argument('--dwgread', required=True)
    args = parser.parse_args()
    rows = check(args.source, args.output_dir, args.baseline_dir, args.dwgread)
    print(json.dumps({'classes': rows, 'encodings': ['ascii', 'binary'],
                      'scope': 'scratch layout guard bypass; no native or whole-file claim'}))


if __name__ == '__main__':
    main()
