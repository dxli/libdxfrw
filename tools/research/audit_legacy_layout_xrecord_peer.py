#!/usr/bin/env python3
"""Audit a locally generated R14 layout/dictionary/XRECORD wire control.

Uses LibreDWG for object discovery, then independently walks the selected
frames, CRCs, class and following-frame envelopes. Does not qualify output or
native cloning behavior.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

from generate_ac1012_dictionary_witness import Bits
from verify_oda_legacy_layer_graph import class_frame, dictionary_frame, traced_frame


def require(condition, message):
    if not condition:
        raise ValueError(message)


def read_record(frame, type_code, handle, parent):
    bits = Bits(frame)
    require(bits.bs() == type_code and bits.handle(0, own=True)['value'] == handle,
            'XRECORD frame identity mismatch')
    require(bits.bs() == 0, 'unexpected XRECORD EED')
    end = bits.little(4)
    reactors, size = bits.bl(), bits.bl()
    require(reactors == 0 and size == 14, 'unexpected reactor or payload size')
    start = bits.position
    require(bits.little(2) == 1, 'unexpected first group')
    length_bit = bits.position
    length, page = bits.little(2), bits.take(8)
    require(length == 5 and page == 30, 'unexpected counted-text header')
    require(bytes(bits.take(8) for _ in range(length)) == b'CHECK', 'unexpected text')
    require(bits.little(2) == 70 and bits.little(2) == 37, 'unexpected following short')
    require(bits.position == end and bits.position - start == 8 * size, 'payload boundary mismatch')
    owner = bits.handle(4, handle)
    extension = bits.handle(3, handle)
    require(owner['value'] == parent and extension['value'] == 0
            and len(frame) * 8 - bits.position < 8, 'common handle tail mismatch')
    return {'bodyEndBit': end, 'payloadStartBit': start, 'lengthBit': length_bit,
            'length': length, 'codepage': page, 'followingGroup70': 37,
            'owner': owner, 'extension': extension,
            'frameSha256': hashlib.sha256(frame).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--dwgread', required=True)
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    require(source.read_bytes().startswith(b'AC1014'), 'source is not R14')
    with tempfile.TemporaryDirectory(prefix='layout-xrecord-peer-') as directory:
        peer = Path(directory) / 'peer.json'
        process = subprocess.run([args.dwgread, '-O', 'JSON', '-o', str(peer), str(source)],
                                 capture_output=True, check=False)
        require(process.returncode == 0 and peer.is_file(), 'LibreDWG read failed')
        model = json.loads(peer.read_text())
    objects = model['OBJECTS']
    layouts = [row for row in objects if row.get('object') == 'LAYOUT' and row.get('layout_name') == 'Layout1']
    require(len(layouts) == 1, 'paper layout missing or ambiguous')
    layout = layouts[0]
    ext = layout['xdicobjhandle'][-1]
    dictionaries = [row for row in objects if row.get('object') == 'DICTIONARY' and row['handle'][-1] == ext]
    require(len(dictionaries) == 1 and dictionaries[0]['ownerhandle'][-1] == layout['handle'][-1]
            and set(dictionaries[0]['items']) == {'RESEARCH'}, 'extension dictionary mismatch')
    dictionary = dictionaries[0]
    child = dictionary['items']['RESEARCH'][-1]
    xrecords = [row for row in objects if row.get('object') == 'XRECORD' and row['handle'][-1] == child]
    require(len(xrecords) == 1 and xrecords[0]['ownerhandle'][-1] == ext
            and xrecords[0]['xdata'] == [[1, 'CHECK'], [70, 37]], 'peer XRECORD mismatch')
    xrecord = xrecords[0]
    dictionary_proof = dictionary_frame(args.dwgread, source, 'r14', dictionary, None, None)
    require(not dictionary_proof['eed'] and dictionary_proof['entryCount'] == 1
            and dictionary_proof['r14Byte'] == 0, 'unsupported dictionary profile')
    selected_class = class_frame(source, model['CLASSES'], xrecord['type'])
    require(selected_class['selected']['dxf'] == 'XRECORD'
            and selected_class['selected']['cpp'] == 'AcDbXrecord', 'XRECORD class mismatch')
    frame, offset, following = traced_frame(args.dwgread, source, child, xrecord['type'])
    proof = read_record(frame, xrecord['type'], child, ext)
    damaged = bytearray(frame)
    bit = proof['lengthBit']
    damaged[bit // 8] ^= 0x80 >> (bit % 8)
    try:
        read_record(damaged, xrecord['type'], child, ext)
    except ValueError:
        pass
    else:
        raise ValueError('corrupted physical XRECORD was accepted')
    require(read_record(frame, xrecord['type'], child, ext) == proof, 'original retry failed')
    print(json.dumps({'schema': 1, 'sourceSha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                      'layout': layout['handle'][-1], 'dictionary': dictionary_proof,
                      'class': selected_class, 'xrecord': {'handle': child, 'offset': offset,
                      'size': len(frame), 'crcValid': True, 'following': following, **proof},
                      'negativeLengthRefusedAndOriginalRetryPassed': True,
                      'scope': 'source-only local producer control; no native semantics or output admission'}, indent=2))


if __name__ == '__main__':
    main()
