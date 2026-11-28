#!/usr/bin/env python3
"""Independently inspect a generated legacy layout-UCS DXF control.

The input is a scratch-only diagnostic output, not a production qualification.
Requires the separately installed ezdxf package.
"""
import argparse
from pathlib import Path

import ezdxf

from audit_binary_dxf_layout_nod import normalize_handle
from check_r2000_layout_projection_peer import (
    physical_graph, physical_layout_class, physical_records, peer_graph, require,
)


def check(path: Path, binary: bool) -> None:
    records = physical_records(path, binary)
    ucs = {}
    for record in records:
        if record.section == 'TABLES' and record.type == 'UCS':
            names = [str(value) for code, value in record.values if code == 2]
            require(record.handle and len(names) == 1 and names[0] not in ucs,
                    'invalid UCS table identity')
            ucs[names[0]] = record.handle
    require(set(ucs) == {'Research_UCS', 'Research_BASE_UCS'}
            and len(set(ucs.values())) == 2, 'wrong UCS table profile')
    layouts = [r for r in records if r.section == 'OBJECTS' and r.type == 'LAYOUT']
    require(len(layouts) == 2, 'wrong layout count')
    for layout in layouts:
        for code, name in ((345, 'Research_UCS'), (346, 'Research_BASE_UCS')):
            values = [normalize_handle(value) for group, value in layout.values if group == code]
            require(values == [ucs[name]], 'physical layout UCS target mismatch')
        require([int(value) for code, value in layout.values if code == 76] == [1],
                'wrong orthographic UCS mode')
    graph = physical_graph(path, binary, {'Model': '*Model_Space', 'Layout1': '*Paper_Space'})
    physical_layout_class(path, binary)
    peer, errors, fixes = peer_graph(path, ezdxf)
    require(graph == peer and errors == 0 and fixes == 0, 'peer graph or audit mismatch')
    doc = ezdxf.readfile(path)
    for name in graph:
        layout = doc.rootdict['ACAD_LAYOUT'][name]
        require(normalize_handle(layout.dxf.ucs_handle) == ucs['Research_UCS']
                and normalize_handle(layout.dxf.base_ucs_handle) == ucs['Research_BASE_UCS']
                and layout.dxf.ucs_type == 1, 'peer layout UCS mismatch')
    lines = list(doc.modelspace().query('LINE'))
    require(len(lines) == 1 and tuple(lines[0].dxf.start) == (1., 2., 0.)
            and tuple(lines[0].dxf.end) == (4., 5., 0.), 'line geometry mismatch')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path, help='scratch probe output directory')
    args = parser.parse_args()
    for encoding in ('ascii', 'binary'):
        check(args.directory / (encoding + '.dxf'), encoding == 'binary')
    print('physical tags, ezdxf UCS targets and audit: ASCII and binary PASS')


if __name__ == '__main__':
    main()
