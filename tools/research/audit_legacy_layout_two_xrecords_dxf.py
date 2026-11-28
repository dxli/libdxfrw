#!/usr/bin/env python3
"""Independently verify the selected two-XRECORD diagnostic DXF graph."""
import argparse
import hashlib
import io
import json
from pathlib import Path

import ezdxf
from ezdxf.lldxf.tagger import ascii_tags_loader, binary_tags_loader


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
    assert len(records) == 1
    record = records[0]
    assert [str(tag.value).strip() for tag in record if tag.code == 280] == [str(hard_owner)]
    assert [str(tag.value).strip() for tag in record if tag.code == 281] == [str(cloning)]
    edges = [(record[i].value, record[i + 1].code, str(record[i + 1].value).upper())
             for i in range(len(record) - 1) if record[i].code == 3]
    assert len(edges) == len(expected_entries)
    assert {(name, code, target) for name, code, target in edges} == {
        (name, edge_code, target.upper()) for name, target in expected_entries.items()}


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
        assert document.dxfversion == 'AC1015'
        assert set(document.layout_names()) == {'Model', 'Layout1'}
        layout = document.layouts.get('Layout1').dxf_layout
        assert layout.has_extension_dict
        extension = layout.get_extension_dict().dictionary
        assert extension.dxf.owner == layout.dxf.handle
        expected = [('RESEARCH', 'CHECK', 37), ('SECOND', 'OTHER', 73)]
        if args.many:
            expected += [(f'CHILD{i:02}', f'VAL{i:02}', 100 + i) for i in range(30)]
        assert set(extension.keys()) == {name for name, _, _ in expected}
        assert layout.get_reactors() == [layout.dxf.owner]
        children = []
        for name, text, number in expected:
            child = extension[name]
            assert child.dxftype() == 'XRECORD' and child.dxf.owner == extension.dxf.handle
            assert [(tag.code, tag.value) for tag in child.tags] == [(1, text), (70, number)]
            children.append(child.dxf.handle)
        assert len(set(children)) == len(expected)
        assert extension.dxf.hard_owned == args.hard_owner and extension.dxf.cloning == args.cloning
        physical_dictionary(path, extension.dxf.handle,
                            {name: extension[name].dxf.handle for name, _, _ in expected},
                            args.hard_owner, args.cloning, args.edge_code)
        lines = list(document.modelspace().query('LINE'))
        assert len(lines) == 1 and tuple(lines[0].dxf.start) == (1.0, 2.0, 0.0)
        assert tuple(lines[0].dxf.end) == (4.0, 5.0, 0.0)
        audit = document.audit()
        assert not audit.errors and not audit.fixes
        outputs.append({'name': path.name, 'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
                        'layout': layout.dxf.handle, 'dictionary': extension.dxf.handle,
                        'children': children})
    print(json.dumps({'schema': 1, 'ezdxf': ezdxf.__version__, 'outputs': outputs,
                      'supportClaimEffect': 'none'}, indent=2))


if __name__ == '__main__':
    main()
