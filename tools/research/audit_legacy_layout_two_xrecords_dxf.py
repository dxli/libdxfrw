#!/usr/bin/env python3
"""Independently verify the selected two-XRECORD diagnostic DXF graph."""
import argparse
import hashlib
import io
import json
from pathlib import Path

import ezdxf
from ezdxf.lldxf.tagger import ascii_tags_loader, binary_tags_loader


def require(condition, message):
    if not condition:
        raise ValueError(message)


def physical_dictionary(path, handle, expected_entries, hard_owner, cloning, edge_code):
    raw = path.read_bytes()
    if raw.startswith(b'AutoCAD Binary DXF\r\n\x1a\x00'):
        tags = list(binary_tags_loader(raw))
    else:
        tags = list(ascii_tags_loader(io.StringIO(raw.decode('utf-8'), newline=None)))
    records = []
    for index, tag in enumerate(tags):
        if tag.code != 0 or tag.value != 'DICTIONARY':
            continue
        end = index + 1
        while end < len(tags) and tags[end].code != 0:
            end += 1
        record = tags[index:end]
        if any(field.code == 5 and str(field.value).upper() == handle.upper() for field in record):
            records.append(record)
    require(len(records) == 1, 'dictionary identity is missing or ambiguous')
    record = records[0]
    require([str(tag.value).strip() for tag in record if tag.code == 280] == [str(hard_owner)], 'physical hard-owner mismatch')
    require([str(tag.value).strip() for tag in record if tag.code == 281] == [str(cloning)], 'physical cloning mismatch')
    edges = [(record[i].value, record[i + 1].code, str(record[i + 1].value).upper())
             for i in range(len(record) - 1) if record[i].code == 3]
    require(len(edges) == len(expected_entries), 'physical dictionary entry count mismatch')
    require({(name, code, target) for name, code, target in edges} == {
        (name, edge_code, target.upper()) for name, target in expected_entries.items()}, 'physical dictionary edge mismatch')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--many', action='store_true', help='check the bounded 32-child profile')
    parser.add_argument('--hard-owner', type=int, choices=(0, 1), default=0)
    parser.add_argument('--cloning', type=int, default=0)
    parser.add_argument('--edge-code', type=int, choices=(350, 360), default=350)
    parser.add_argument('files', nargs='+', type=Path)
    args = parser.parse_args()
    outputs = []
    for path in args.files:
        document = ezdxf.readfile(path)
        require(document.dxfversion == 'AC1015', 'wrong DXF version')
        require(set(document.layout_names()) == {'Model', 'Layout1'}, 'layout names differ')
        layout = document.layouts.get('Layout1').dxf_layout
        require(layout.has_extension_dict, 'missing extension dictionary')
        extension = layout.get_extension_dict().dictionary
        require(extension.dxf.owner == layout.dxf.handle, 'extension owner mismatch')
        expected = [('RESEARCH', 'CHECK', 37), ('SECOND', 'OTHER', 73)]
        if args.many:
            expected += [(f'CHILD{i:02}', f'VAL{i:02}', 100 + i) for i in range(30)]
        require(set(extension.keys()) == {name for name, _, _ in expected}, 'extension entries differ')
        require(layout.get_reactors() == [layout.dxf.owner], 'layout reactor mismatch')
        children = []
        for name, text, number in expected:
            child = extension[name]
            require(child.dxftype() == 'XRECORD' and child.dxf.owner == extension.dxf.handle, 'XRECORD owner or type mismatch')
            require([(tag.code, tag.value) for tag in child.tags] == [(1, text), (70, number)], 'XRECORD values differ')
            children.append(child.dxf.handle)
        require(len(set(children)) == len(expected), 'XRECORD identities are aliased')
        require(extension.dxf.hard_owned == args.hard_owner and extension.dxf.cloning == args.cloning, 'dictionary ownership or cloning mismatch')
        physical_dictionary(path, extension.dxf.handle,
                            {name: extension[name].dxf.handle for name, _, _ in expected},
                            args.hard_owner, args.cloning, args.edge_code)
        lines = list(document.modelspace().query('LINE'))
        require(len(lines) == 1 and tuple(lines[0].dxf.start) == (1.0, 2.0, 0.0), 'line start differs')
        require(tuple(lines[0].dxf.end) == (4.0, 5.0, 0.0), 'line end differs')
        audit = document.audit()
        require(not audit.errors and not audit.fixes, 'DXF audit errors or fixes')
        outputs.append({'name': path.name, 'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
                        'layout': layout.dxf.handle, 'dictionary': extension.dxf.handle,
                        'children': children})
    print(json.dumps({'schema': 1, 'ezdxf': ezdxf.__version__, 'outputs': outputs,
                      'supportClaimEffect': 'none'}, indent=2))


if __name__ == '__main__':
    main()
