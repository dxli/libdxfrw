#!/usr/bin/env python3
"""Independently walk a locally generated two-XRECORD legacy layout graph."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

from audit_legacy_layout_xrecord_peer import read_record, require
from verify_oda_legacy_layer_graph import class_frame, dictionary_frame, traced_frame


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--dwgread', required=True)
    parser.add_argument('--many', action='store_true', help='check the bounded 32-child profile')
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    magic = source.read_bytes()[:6]
    require(magic in (b'AC1012', b'AC1014'), 'source is not R13/R14')
    version = 'r13' if magic == b'AC1012' else 'r14'
    with tempfile.TemporaryDirectory(prefix='layout-two-xrecords-') as directory:
        peer = Path(directory) / 'peer.json'
        result = subprocess.run([args.dwgread, '-O', 'JSON', '-o', str(peer), str(source)],
                                capture_output=True, check=False)
        require(result.returncode == 0 and peer.is_file(), 'LibreDWG read failed')
        model = json.loads(peer.read_text())
    objects = model['OBJECTS']
    layouts = [o for o in objects if o.get('object') == 'LAYOUT' and o.get('layout_name') == 'Layout1']
    require(len(layouts) == 1, 'paper layout missing or ambiguous')
    owner = layouts[0]['handle'][-1]
    extension = layouts[0]['xdicobjhandle'][-1]
    dictionaries = [o for o in objects if o.get('object') == 'DICTIONARY' and o['handle'][-1] == extension]
    expected = [('RESEARCH', b'CHECK', 37), ('SECOND', b'OTHER', 73)]
    if args.many:
        expected += [(f'CHILD{i:02}', f'VAL{i:02}'.encode(), 100 + i) for i in range(30)]
    require(len(dictionaries) == 1 and dictionaries[0]['ownerhandle'][-1] == owner
            and set(dictionaries[0]['items']) == {key for key, _, _ in expected}, 'extension dictionary mismatch')
    dictionary = dictionary_frame(args.dwgread, source, version, dictionaries[0], None, None)
    require(not dictionary['eed'] and dictionary['entryCount'] == len(expected)
            and dictionary['r14Byte'] == (None if version == 'r13' else 0), 'dictionary profile mismatch')
    proofs = []
    for key, text, short in expected:
        handle = dictionaries[0]['items'][key][-1]
        records = [o for o in objects if o.get('object') == 'XRECORD' and o['handle'][-1] == handle]
        require(len(records) == 1 and records[0]['ownerhandle'][-1] == extension
                and records[0]['xdata'] == [[1, text.decode()], [70, short]], 'XRECORD mismatch')
        record = records[0]
        cls = class_frame(source, model['CLASSES'], record['type'])
        require(cls['selected']['dxf'] == 'XRECORD' and cls['selected']['cpp'] == 'AcDbXrecord', 'class mismatch')
        frame, offset, following = traced_frame(args.dwgread, source, handle, record['type'])
        proof = read_record(frame, record['type'], handle, extension, text, short)
        damaged = bytearray(frame)
        bit = proof['lengthBit']
        damaged[bit // 8] ^= 0x80 >> (bit % 8)
        try:
            read_record(damaged, record['type'], handle, extension, text, short)
        except ValueError:
            pass
        else:
            raise ValueError('corrupted XRECORD accepted')
        require(read_record(frame, record['type'], handle, extension, text, short) == proof, 'retry failed')
        proofs.append({'key': key, 'handle': handle, 'offset': offset, 'size': len(frame),
                       'crcValid': True, 'following': following, **proof})
    require(len({proof['handle'] for proof in proofs}) == len(expected), 'aliased child identity')
    print(json.dumps({'schema': 1, 'sourceSha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                      'version': version, 'layout': owner, 'dictionary': dictionary,
                      'records': proofs, 'supportClaimEffect': 'none'}, indent=2))


if __name__ == '__main__':
    main()
