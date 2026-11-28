#!/usr/bin/env python3
"""Inspect the scratch legacy paper-VIEWPORT control with physical tags and ezdxf."""
import argparse
from pathlib import Path

import ezdxf

from audit_binary_dxf_layout_nod import normalize_handle
from check_r2000_layout_projection_peer import (
    physical_graph, physical_layout_class, physical_records, peer_graph, require,
)


def check(path: Path, binary: bool) -> None:
    graph = physical_graph(path, binary, {'Model': '*Model_Space', 'Layout1': '*Paper_Space'})
    physical_layout_class(path, binary)
    records = physical_records(path, binary)
    paper = [r for r in records if r.section == 'OBJECTS' and r.type == 'LAYOUT'
             and r.handle == graph['Layout1']['handle']]
    viewports = [r for r in records if r.section in ('BLOCKS', 'ENTITIES')
                 and r.type == 'VIEWPORT']
    require(len(paper) == len(viewports) == 1, 'wrong viewport profile')
    viewport = viewports[0]
    require([normalize_handle(v) for c, v in paper[0].values if c == 331] == [viewport.handle],
            'physical layout viewport reference mismatch')
    require([normalize_handle(v) for c, v in viewport.values if c == 330] == [graph['Layout1']['block']],
            'physical viewport owner mismatch')
    for code in (67, 68, 69):
        require([int(v) for c, v in viewport.values if c == code] == [1],
                'physical viewport paper/status/id mismatch')
    peer, errors, fixes = peer_graph(path, ezdxf)
    require(graph == peer and errors == 0 and fixes == 0, 'peer graph or audit mismatch')
    doc = ezdxf.readfile(path)
    layout = doc.rootdict['ACAD_LAYOUT']['Layout1']
    entities = list(doc.layouts.get('Layout1').query('VIEWPORT'))
    require(len(entities) == 1
            and normalize_handle(layout.dxf.viewport_handle) == viewport.handle
            and normalize_handle(entities[0].dxf.handle) == viewport.handle
            and normalize_handle(entities[0].dxf.owner) == graph['Layout1']['block'],
            'peer paper viewport graph mismatch')
    lines = list(doc.modelspace().query('LINE'))
    require(len(lines) == 1 and tuple(lines[0].dxf.start) == (1., 2., 0.)
            and tuple(lines[0].dxf.end) == (4., 5., 0.), 'line geometry mismatch')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    args = parser.parse_args()
    for encoding in ('ascii', 'binary'):
        check(args.directory / (encoding + '.dxf'), encoding == 'binary')
    print('physical tags, ezdxf viewport graph and audit: ASCII and binary PASS')


if __name__ == '__main__':
    main()
