#!/usr/bin/env python3
"""Inspect a scratch legacy layout extension-dictionary control."""
import argparse
from pathlib import Path

import ezdxf

from audit_binary_dxf_layout_nod import normalize_handle
from check_r2000_layout_projection_peer import (
    physical_graph, physical_layout_class, physical_records, peer_graph, require,
)


def check(path: Path, binary: bool, with_xrecord: bool = False, xrecord_short: int = 37) -> None:
    graph = physical_graph(path, binary, {'Model': '*Model_Space', 'Layout1': '*Paper_Space'})
    physical_layout_class(path, binary)
    records = physical_records(path, binary)
    paper = [r for r in records if r.section == 'OBJECTS' and r.type == 'LAYOUT'
             and r.handle == graph['Layout1']['handle']]
    require(len(paper) == 1, 'wrong paper layout count')
    groups = list(paper[0].values)
    opens = [i for i, (code, value) in enumerate(groups)
             if code == 102 and value == '{ACAD_XDICTIONARY']
    require(len(opens) == 1 and opens[0] + 2 < len(groups), 'missing extension group')
    i = opens[0]
    require(groups[i + 1][0] == 360 and groups[i + 2] == (102, '}'), 'invalid extension group')
    handle = normalize_handle(groups[i + 1][1])
    targets = [r for r in records if r.section == 'OBJECTS' and r.type == 'DICTIONARY'
               and r.handle == handle]
    require(len(targets) == 1, 'extension dictionary target missing')
    target = targets[0]
    require([normalize_handle(v) for c, v in target.values if c == 330] == [paper[0].handle],
            'physical extension dictionary owner mismatch')
    require([int(v) for c, v in target.values if c == 280] == [0]
            and [int(v) for c, v in target.values if c == 281] == [0],
            'physical extension dictionary flags mismatch')
    entries = [(c, v) for c, v in target.values if c in (3, 350, 360)]
    if with_xrecord:
        require(len(entries) == 2 and entries[0] == (3, 'RESEARCH') and entries[1][0] == 350,
                'physical XRECORD dictionary entry mismatch')
        child = normalize_handle(entries[1][1])
        xrecords = [r for r in records if r.section == 'OBJECTS' and r.type == 'XRECORD'
                    and r.handle == child]
        require(len(xrecords) == 1, 'physical XRECORD target missing')
        record = xrecords[0]
        require([normalize_handle(v) for c, v in record.values if c == 330] == [handle]
                and [str(v) for c, v in record.values if c == 1] == ['CHECK']
                and [int(v) for c, v in record.values if c == 70] == [xrecord_short]
                and [int(v) for c, v in record.values if c == 280] == [0],
                'physical XRECORD owner or body mismatch')
    else:
        require(not entries, 'physical extension dictionary is not empty')
    peer, errors, fixes = peer_graph(path, ezdxf)
    require(graph == peer and errors == 0 and fixes == 0, 'peer graph or audit mismatch')
    doc = ezdxf.readfile(path)
    layout = doc.rootdict['ACAD_LAYOUT']['Layout1']
    extension = layout.get_extension_dict().dictionary
    require(normalize_handle(extension.dxf.handle) == handle
            and normalize_handle(extension.dxf.owner) == paper[0].handle
            and extension.dxf.hard_owned == 0 and extension.dxf.cloning == 0,
            'peer extension dictionary mismatch')
    if with_xrecord:
        require(list(extension.keys()) == ['RESEARCH'], 'peer XRECORD dictionary entry mismatch')
        record = extension['RESEARCH']
        require(record.dxftype() == 'XRECORD' and normalize_handle(record.dxf.handle) == child
                and normalize_handle(record.dxf.owner) == handle
                and [(tag.code, tag.value) for tag in record.tags] == [(1, 'CHECK'), (70, xrecord_short)],
                'peer XRECORD body mismatch')
    else:
        require(len(extension) == 0, 'peer extension dictionary is not empty')
    lines = list(doc.modelspace().query('LINE'))
    require(len(lines) == 1 and tuple(lines[0].dxf.start) == (1., 2., 0.)
            and tuple(lines[0].dxf.end) == (4., 5., 0.), 'line geometry mismatch')


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('directory', type=Path)
    parser.add_argument('--xrecord', action='store_true')
    parser.add_argument('--xrecord-short', type=int, default=37)
    args = parser.parse_args()
    for encoding in ('ascii', 'binary'):
        check(args.directory / (encoding + '.dxf'), encoding == 'binary', args.xrecord, args.xrecord_short)
    print('physical tags, ezdxf extension graph and audit: ASCII and binary PASS')


if __name__ == '__main__':
    main()
