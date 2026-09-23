# 3D format and version status

This is an evidence/status matrix, not a promise of general 3D CAD support.
Reader or writer route presence, local round-trips, retained raw bytes, and
derived previews are distinct from independently qualified semantic support.
No row below promotes a stable 3D support claim.

Evidence labels:

- SELFTEST: runtime-generated fields round-trip through libdxfrw's own DXF
  reader and writer.
- EXT-ACCEPT: LibreDWG 0.14 accepted generated input/conversion, but no
  independent field-by-field semantic comparison was established.
- SPEC: primary DXF/DWG documentation or source inspection identifies a
  layout or route; this alone does not prove implementation correctness.
- UNQUALIFIED: a semantic support claim is not justified for that
  family/version/direction.

For DXF, a listed ACxxxx value is the exact version passed to fileExport; only
the listed ASCII/binary encodings and subtypes are covered. Unlisted DXF
versions remain unqualified. For DWG, R is the current reader dispatch set
and W is the current writer target set:

- R: AC1009, AC1012, AC1014, AC1015, AC1018, AC1021, AC1024, AC1027, AC1032.
- W: AC1015, AC1018, AC1021, AC1024, AC1027, AC1032.

Every DWG version in those sets remains unqualified for 3D semantic
interoperability unless a row explicitly narrows the implementation route.
Local writer/self-read tests are regression checks, not independent evidence.

| Geometry family | Format / version | Read disposition | Write disposition | Evidence and boundary |
| --- | --- | --- | --- | --- |
| WCS primitives / 3DFACE | DXF AC1027, ASCII and binary | Experimental field read for 3DFACE only | Experimental field write for 3DFACE only | SELFTEST; EXT-ACCEPT for generated topology conversion. POINT, LINE, RAY, and XLINE are not qualified by this 3DFACE slice. |
| WCS primitives / 3DFACE | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | Fixed/custom dispatch routes and local tests do not establish independent per-version field semantics. |
| Legacy/custom 3DLINE | DXF: no qualified version/encoding | UNQUALIFIED | UNQUALIFIED | No portable DXF spelling or independent target witness is established; do not equate it with 3D LINE. |
| Legacy/custom 3DLINE | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | Pre-R13 and custom-class paths require separate version-specific evidence. |
| Planar entities placed in 3D | DXF AC1027, ASCII and binary | Experimental field read for LWPOLYLINE OCS/elevation subset | Experimental field write for LWPOLYLINE OCS/elevation subset | SELFTEST; does not qualify ARC, CIRCLE, ELLIPSE, SOLID, TRACE, or HATCH as a family. |
| Planar entities placed in 3D | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | No independent per-version OCS/WCS semantic comparison. |
| Placed block geometry | DXF AC1027, ASCII and binary | Experimental INSERT/MINSERT field read | Experimental INSERT/MINSERT field write | SELFTEST plus an independent matrix oracle for nested placement and array offset; external acceptance is not semantic comparison. |
| Placed block geometry | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | Local paths do not independently establish versioned transform semantics. |
| Classic 3D topology | DXF AC1027, ASCII and binary | Experimental 3D POLYLINE/VERTEX and polyface read | Experimental 3D POLYLINE/VERTEX and polyface write | SELFTEST; EXT-ACCEPT for generated input conversion. DWG child ownership/count claims remain separate. |
| Classic 3D topology | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | No independent per-version semantic witness. |
| Subdivision topology / MESH | DXF AC1027, ASCII and binary | Experimental MESH field read | Experimental MESH field write | SELFTEST; EXT-ACCEPT for generated input conversion; not an independent topology comparison. |
| Subdivision topology / MESH | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | DWG class/layout support is not established by local self-read. |
| Curves and analytic/NURBS surfaces | DXF AC1027, ASCII and binary | Experimental mapped-field read for SPLINE, HELIX, and six surface subtypes | Experimental mapped-field write for those tested records | SELFTEST; no curve evaluation, surface evaluation, or independent semantic oracle. |
| Curves and analytic/NURBS surfaces | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | ODA §20.4.40 supports the reviewed R2013+ spline flag widths; authentic per-version witnesses are still missing. No named modern surface layout was found in the reviewed ODA v5.4.1 text. |
| ACIS 3DSOLID / REGION / BODY carriers | DXF AC1015 ASCII; AC1018 ASCII; AC1021 binary; AC1024 ASCII | Experimental opaque SAT chunk read/retention | Experimental opaque SAT chunk write through AC1024 | SELFTEST; verifies carrier/chunk identity only, not ACIS semantics. Unassociated AC1027+ SAB writes are rejected; entity-to-ACDSDATA association is unqualified. |
| ACIS 3DSOLID / REGION / BODY carriers | DWG R set | Opaque frame/envelope handling only; typed ACIS payload extraction is UNQUALIFIED per version | No typed DWG modeler-geometry writer route | S3.1 validates the general modeler version marker; it does not decode SAT/SAB or qualify DataStorage association. |
| Proxy and derived geometry | DXF: no 3D-family/version qualification | UNQUALIFIED for 3D-specific proxy semantics | UNQUALIFIED for 3D-specific proxy semantics | General raw-carrier preservation is not a 3D geometry claim. |
| Proxy and derived geometry | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | A derived wireframe is not a replacement for its original entity or opaque payload. |

The compact R set / W set notation means every version explicitly listed
above is recorded as unqualified, not that all route implementations are
identical. Version-specific support promotion requires an exact class/version
identity, an independent semantic witness, passing negative/boundary tests,
and (for consumer-visible display/edit claims) a separate LibreCAD callback
review. The implementation detail and evidence log are in
[DWG_DXF_3D_SUPPORT_PLAN.md](../DWG_DXF_3D_SUPPORT_PLAN.md), especially S3,
S5, S6, and S7.

Primary layout references: [ODA Open Design Specification for DWG v5.4.1,
§§20.4.40–20.4.41 and §24](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf);
Autodesk's [EXTRUDED SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-9218F5A6-3AE4-4EA4-854E-E15E1946AE88),
[REVOLVED SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-844D8EC8-318D-4721-AFF2-82923DB10678),
[SWEPT SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-55DCEC23-9286-4A32-AAFA-E1F945B10A19),
[LOFTED SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-3D9D8A87-1E46-48AE-B482-BAD1C4D460CA),
[NURBS SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-E1F884F8-AA90-4864-A215-3182D47A9C74), and
[HELIX](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-76DB3ABF-3C8C-47D1-8AFB-72942D9AE1FF)
DXF reference tables.
