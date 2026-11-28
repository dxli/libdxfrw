#!/usr/bin/env python3
"""Prove that a DXF graph audit rejects changed dictionary wire roles.

Inputs are matching locally generated ASCII and binary diagnostic outputs with
one selected Layout1 extension dictionary. All mutations stay in a temporary
directory; the original files must pass again after every negative control.
"""
import argparse
from pathlib import Path
import subprocess
import sys
import tempfile

import ezdxf


def require(condition, message):
    if not condition:
        raise ValueError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ascii', type=Path, required=True)
    parser.add_argument('--binary', type=Path, required=True)
    args = parser.parse_args()
    audit = Path(__file__).with_name('audit_legacy_layout_two_xrecords_dxf.py')
    with tempfile.TemporaryDirectory(prefix='legacy-dxf-roles-') as directory:
        output = Path(directory)
        for encoding in ('ascii', 'binary'):
            source = getattr(args, encoding).resolve(strict=True)
            raw = source.read_bytes()
            document = ezdxf.readfile(source)
            extension = document.layouts.get('Layout1').dxf_layout.get_extension_dict().dictionary
            handle = extension.dxf.handle
            first_child = extension['RESEARCH'].dxf.handle
            if encoding == 'ascii':
                lines = raw.decode('utf-8').splitlines(keepends=True)
                require(len(lines) % 2 == 0, 'unpaired ASCII group')
                pairs = [(lines[i], lines[i + 1]) for i in range(0, len(lines), 2)]
                candidates = [i for i, (code, value) in enumerate(pairs[:-1])
                              if code.strip() == '0' and value.strip() == 'DICTIONARY'
                              and pairs[i + 1][0].strip() == '5'
                              and pairs[i + 1][1].strip().upper() == handle.upper()]
                require(len(candidates) == 1, 'ambiguous dictionary')
                start = candidates[0]
                end = next(i for i in range(start + 1, len(pairs)) if pairs[i][0].strip() == '0')
                for case, code in [('hard', 280), ('clone', 281), ('edge', 350)]:
                    indexes = [i for i in range(start, end) if pairs[i][0].strip() == str(code)]
                    require(indexes, 'missing selected role')
                    i = indexes[0]
                    edited = list(pairs)
                    if case == 'edge':
                        edited[i] = (pairs[i][0].replace('350', '360'), pairs[i][1])
                    else:
                        edited[i] = (pairs[i][0], '1\n')
                    (output / f'{encoding}-{case}.dxf').write_text(''.join(c + v for c, v in edited))
            else:
                marker = b'\x00\x00DICTIONARY\x00\x05\x00' + handle.encode('ascii') + b'\x00'
                require(raw.count(marker) == 1, 'ambiguous binary dictionary')
                start = raw.index(marker)
                cases = [('hard', b'\x18\x01\x00', b'\x18\x01\x01'),
                         ('clone', b'\x19\x01\x00', b'\x19\x01\x01'),
                         ('edge', b'\x5e\x01' + first_child.encode('ascii') + b'\x00',
                          b'\x68\x01' + first_child.encode('ascii') + b'\x00')]
                for case, needle, replacement in cases:
                    position = raw.find(needle, start + len(marker))
                    require(start < position < start + 512, 'missing selected binary role')
                    edited = bytearray(raw)
                    edited[position:position + len(needle)] = replacement
                    (output / f'{encoding}-{case}.dxf').write_bytes(edited)
            for case in ('hard', 'clone', 'edge'):
                result = subprocess.run([sys.executable, '-B', str(audit), str(output / f'{encoding}-{case}.dxf')],
                                        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)
                require(result.returncode != 0, f'{encoding} {case} was accepted')
                result = subprocess.run([sys.executable, '-B', str(audit), str(source)],
                                        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=False)
                require(result.returncode == 0, f'{encoding} original did not retry')
            print(f'{encoding}: hard-owner, cloning and edge mutations refused; originals retry')


if __name__ == '__main__':
    main()
