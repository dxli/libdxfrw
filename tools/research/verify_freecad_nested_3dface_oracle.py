#!/usr/bin/env python3
"""Independently verify a generated nested 3DFACE test's WCS coordinates.

Requires ezdxf; writes the generated DXF only to a temporary directory. The
result is a geometric control, not producer or native-GUI qualification.
"""
import hashlib
import json
from pathlib import Path
import tempfile

import ezdxf

EXPECTED = (
    (15.149892735097104, 6.400813184350863, 51.628440700337116),
    (13.417841927528226, 3.961655595595431, 50.60349667395479),
    (7.801423506747567, 9.367941017339081, 56.009782095698434),
)
SOURCE_VERTICES = ((1, 2, 3), (3, 2, 4), (1, 4, 4))


def main():
    doc = ezdxf.new("R2007")
    inner = doc.blocks.new("INNER", base_point=(1, 2, 3))
    inner.add_3dface(SOURCE_VERTICES)
    outer = doc.blocks.new("OUTER", base_point=(4, -1, 2))
    outer.add_blockref("INNER", (10, 5, 7), dxfattribs={
        "xscale": -2, "yscale": 3, "zscale": .5, "rotation": 90, "extrusion": (0, 1, 0)})
    doc.modelspace().add_blockref("OUTER", (20, 30, 40), dxfattribs={
        "xscale": 1.5, "yscale": 2, "zscale": .75, "rotation": 30, "extrusion": (1, 1, 1)})
    cases = []
    with tempfile.TemporaryDirectory(prefix="freecad-3dface-oracle-") as directory:
        for units, factor in ((4, 1.0), (1, 25.4)):
            doc.header["$INSUNITS"] = units
            path = Path(directory) / "nested.dxf"
            doc.saveas(path)
            source_hash = hashlib.sha256(path.read_bytes()).hexdigest()
            loaded = ezdxf.readfile(path)
            audit = loaded.audit()
            if audit.errors or audit.fixes:
                raise RuntimeError("generated source failed ezdxf audit")
            nested = list(loaded.blocks["OUTER"].query("INSERT"))
            top = list(loaded.modelspace().query("INSERT"))
            faces = list(loaded.blocks["INNER"].query("3DFACE"))
            if len(nested) != 1 or len(top) != 1 or len(faces) != 1:
                raise RuntimeError("unexpected source entity graph")
            vertices = [tuple(top[0].matrix44().transform(nested[0].matrix44().transform(
                faces[0].dxf.get("vtx" + str(i))))) for i in range(3)]
            for actual, expected in zip(vertices, EXPECTED):
                if any(abs(a - e) > 1e-10 for a, e in zip(actual, expected)):
                    raise RuntimeError("independent WCS coordinate differs from test oracle")
            cases.append({"units": units, "millimetreFactor": factor,
                          "sourceSha256": source_hash,
                          "verticesMillimetres": [[factor * x for x in vertex] for vertex in vertices]})
    print(json.dumps({"ok": True, "ezdxfVersion": ezdxf.__version__, "cases": cases}, sort_keys=True))


if __name__ == "__main__":
    main()
