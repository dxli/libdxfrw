#!/usr/bin/env python3
"""Independently verify the selected two-XRECORD diagnostic DXF graph."""
import argparse
import hashlib
import json
from pathlib import Path

import ezdxf


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--many', action='store_true', help='check the bounded 32-child profile')
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
