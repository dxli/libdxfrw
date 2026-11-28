"""Reproduce INSERT geometry overflow in a disposable headless FreeCADCmd.

Run the candidate FreeCADCmd with this script and disposable user/system config.
The generated DXF is temporary. ``ok`` means both modes refused the overflowing
instance with a diagnostic and retained the following valid instance; process
success alone is not evidence of a pass.
"""
import json
import math
from pathlib import Path
import tempfile
import traceback

import FreeCAD as App
import Import


def main():
    report = {"ok": False, "cases": []}
    try:
        if App.GuiUp:
            raise RuntimeError("headless FreeCADCmd required")
        tags = [(0, "SECTION"), (2, "HEADER"), (9, "$ACADVER"), (1, "AC1021"),
                (9, "$INSUNITS"), (70, 4), (0, "ENDSEC"), (0, "SECTION"), (2, "BLOCKS"),
                (0, "BLOCK"), (8, "0"), (2, "B"), (70, 0), (10, 0), (20, 0), (30, 0),
                (0, "LINE"), (8, "0"), (10, 0), (20, 0), (30, 0), (11, 2), (21, 0), (31, 0),
                (0, "ENDBLK"), (8, "0"), (0, "ENDSEC"), (0, "SECTION"), (2, "ENTITIES"),
                (0, "INSERT"), (8, "0"), (2, "B"), (41, "1e308"),
                (0, "INSERT"), (8, "0"), (2, "B"), (10, 10), (0, "ENDSEC"), (0, "EOF")]
        with tempfile.TemporaryDirectory(prefix="freecad-insert-overflow-") as directory:
            path = Path(directory) / "source.dxf"
            path.write_text("".join(f"{code}\n{value}\n" for code, value in tags), encoding="ascii")
            group = "User parameter:BaseApp/Preferences/Mod/Import/CodexInsertOverflowProbe"
            for mode in (2, 3):
                document = App.newDocument("InsertOverflow" + str(mode))
                try:
                    options = App.ParamGet(group)
                    options.SetInt("DxfImportMode", mode)
                    options.SetBool("dxfUseDraftVisGroups", False)
                    stats = Import.readDXF(str(path), document.Name, True, group)
                    document.recompute()
                    links = [obj for obj in document.Objects if obj.Name.startswith("Link_B")]
                    endpoints = [[[v.Point.x, v.Point.y, v.Point.z] for v in edge.Vertexes]
                                 for link in links for edge in link.Shape.Edges]
                    finite = all(math.isfinite(value) for edge in endpoints for point in edge for value in point)
                    valid = len(endpoints) == 1 and sorted(point[0] for point in endpoints[0]) == [10, 12]
                    refused = bool(stats["unsupportedFeatures"])
                    # Strict JSON carries nonfinite observations as strings.
                    sanitized = [[[x if math.isfinite(x) else str(x) for x in point]
                                  for point in edge] for edge in endpoints]
                    report["cases"].append({"mode": mode, "links": len(links), "endpoints": sanitized,
                                            "finite": finite, "validRetry": valid,
                                            "unsupportedFeatures": stats["unsupportedFeatures"],
                                            "pass": finite and valid and refused})
                finally:
                    App.closeDocument(document.Name)
        report["ok"] = len(report["cases"]) == 2 and all(case["pass"] for case in report["cases"])
    except Exception:
        report["error"] = traceback.format_exc()
    print("FREECAD_INSERT_SCALE_OVERFLOW=" + json.dumps(report, sort_keys=True, allow_nan=False), flush=True)


if __name__ in ("__main__", Path(__file__).stem):
    main()
