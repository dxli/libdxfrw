# 3D format and version status

This is an evidence/status matrix, not a promise of general 3D CAD support.
Reader or writer route presence, local round-trips, retained raw bytes, and
derived previews are distinct from independently qualified semantic support.
No row below promotes a stable format/version semantic-support claim.

Evidence labels:

- SELFTEST: runtime-generated fields round-trip through libdxfrw's own DXF
  reader and writer.
- CONSUMER-PROBE: generated DXF is read through the public callback API into a
  headless sink that copies the listed fields. This establishes data delivery
  for that probe, not correctness against an independent producer.
- INDEPENDENT-READ: the same DWG sample was read by libdxfrw and a separate
  reader, and the listed fields matched by entity handle. This narrows read
  evidence to that sample/version/field subset; it says nothing about writing.
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

## Consumer capability is a separate dimension

Existing 2D consumers retain the source-compatible `DRW_Interface` callback
surface: [`lc3_compat_check`](../tests/lc3_compat_check.cpp) compiles a
LibreCAD-style implementation without requiring overrides for optional newer
callbacks. This does not promise binary ABI compatibility or validate
LibreCAD's UI behavior. Its current filter reads DXF and DWG with `ext == true`;
the generated-DXF consumer probe also guards one legacy LWPOLYLINE extrusion
case while separately checking the `ext == false` typed-data path. LibreCAD's
current filter remains a 2D adapter: its
source audit found projection/preview paths for 3DFACE, MESH, polygon meshes,
and HELIX, with some native values retained only in sidecar/advanced metadata.
Those consumer choices do not constrain the library's typed data callbacks.

3D-aware clients can use the public callbacks to copy typed values without an
implicit global XY projection. The fast
[`libdxfrw_3d_consumer_probe`](../tests/CMakeLists.txt) writes runtime-generated
AC1027 ASCII and binary DXF, reads with `ext == false`, and checks callback
delivery for 3DFACE, 3D POLYLINE, MESH, INSERT/MINSERT, SPLINE, OCS/elevation
LWPOLYLINE, and selected LOFTED-surface fields plus a separately identified
group-310 carrier. The same vectors are also read with `ext == true` for a
single legacy LWPOLYLINE coordinate regression; this is not a LibreCAD UI test.
This is a producer-self-generated callback-path check; it
does not prove interoperability, independent semantics, full family coverage,
display/edit support, or any DWG version. Run just this probe with
`ctest --test-dir build -R '^libdxfrw_3d_consumer_probe$' --output-on-failure`.
The complete consumer-handling guidance is in the
[3D consumer contract](3D_CONSUMER_CONTRACT.md).

An optional local comparator, [`compare_dwg_3d_consumer_oracle.py`](../tools/compare_dwg_3d_consumer_oracle.py),
uses LibreDWG `dwgread -O minJSON` as a separate read implementation. It matched
six INSERT placement records and two fit-only SPLINE records by handle in the
locally available AC1024/R2010 conference-room sample. That sample is not
committed here; the comparison is read-only and does not qualify writing,
other DWG versions, or other entity families. The same sample's two
PLANESURFACE records disagree on typed modeler fields between readers; because
the reviewed ODA DWG specification does not define that modern subtype layout,
those results are excluded rather than guessed.

The comparator also matched all 48 3DFACE and 3,002 LINE records in the local
AC1021/R2007 `tablet.dwg` sample by handle against LibreDWG 0.14. For 3DFACE it
checked four 3D corner tuples and invisible-edge flags (ODA v5.4.1 §20.4.32);
for LINE it checked start/end XYZ, thickness, and extrusion (§20.4.21). Of the
LINE records, 670 have nonzero endpoint Z. This read-only sample witness does
not establish writes, other AC1021 files, or other DWG 3D families; the sample
remains uncommitted.

In the local AC1024/R2010 `visualization_-_condominium_with_skylight.dwg`,
both LINE records also match LibreDWG 0.14 by handle for start/end XYZ,
thickness, and extrusion; both have nonzero endpoint Z. ODA v5.4.1 §20.4.21
describes the layout. Its minJSON contains a bare `nan` in an unrelated
surface field; the comparator normalizes only that non-standard token outside
strings and checks that every compared LINE number is finite. This
uncommitted sample provides two-record read evidence only, not write or general
AC1024 support.

| Geometry family | Format / version | Read disposition | Write disposition | Evidence and boundary |
| --- | --- | --- | --- | --- |
| WCS primitives / 3DFACE | DXF AC1027, ASCII and binary | Experimental field read for 3DFACE only | Experimental field write for 3DFACE only | SELFTEST and CONSUMER-PROBE; EXT-ACCEPT for generated topology conversion. POINT, LINE, RAY, and XLINE are not qualified by this 3DFACE slice. |
| WCS primitives / 3DFACE | DWG R set / W set | UNQUALIFIED outside the narrow AC1021 field row below | UNQUALIFIED per writer version | Fixed/custom dispatch routes and local tests do not establish general per-version field semantics. |
| 3DFACE corner/edge-field subset | DWG AC1021 / R2007 | Experimental read: four 3D corner tuples and invisible-edge flags on 48 handles in local `tablet.dwg` | UNQUALIFIED | INDEPENDENT-READ via LibreDWG 0.14, joined by handle; ODA v5.4.1 §20.4.32 describes the layout. One uncommitted sample only; no write, other-file, other-version, or general family claim. |
| LINE endpoint field subset | DWG AC1021 / R2007 | Experimental read: start/end XYZ, thickness, and extrusion on 3,002 handles in local `tablet.dwg`; 670 lines have nonzero endpoint Z | UNQUALIFIED | INDEPENDENT-READ via LibreDWG 0.14, joined by handle; ODA v5.4.1 §20.4.21 describes the layout. One uncommitted sample only; no write, other-file, other-version, or general family claim. |
| LINE endpoint field subset | DWG AC1024 / R2010 | Experimental read: start/end XYZ, thickness, and extrusion on two handles in local `visualization_-_condominium_with_skylight.dwg`; both have nonzero endpoint Z | UNQUALIFIED | INDEPENDENT-READ via LibreDWG 0.14, joined by handle; ODA v5.4.1 §20.4.21 describes the layout. One uncommitted sample only; no write or general AC1024 claim. |
| Legacy/custom 3DLINE | DXF: no qualified version/encoding | UNQUALIFIED | UNQUALIFIED | No portable DXF spelling or independent target witness is established; do not equate it with 3D LINE. |
| Legacy/custom 3DLINE | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | Pre-R13 and custom-class paths require separate version-specific evidence. |
| Planar entities placed in 3D | DXF AC1027, ASCII and binary | Experimental field read for LWPOLYLINE OCS/elevation subset | Experimental field write for LWPOLYLINE OCS/elevation subset | SELFTEST and CONSUMER-PROBE for local XY/elevation/extrusion/bulge delivery; does not qualify ARC, CIRCLE, ELLIPSE, SOLID, TRACE, or HATCH as a family. |
| Planar entities placed in 3D | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | No independent per-version OCS/WCS semantic comparison. |
| Placed block geometry | DXF AC1027, ASCII and binary | Experimental INSERT/MINSERT field read | Experimental INSERT/MINSERT field write | SELFTEST and CONSUMER-PROBE for insertion/scale/grid/OCS values, plus an independent matrix oracle for nested placement and array offset; external acceptance is not semantic comparison. |
| Placed block geometry | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | Block expansion and versioned transform semantics remain unqualified; see the narrower INSERT scalar-field read row below. |
| INSERT placement field subset | DWG AC1024 / R2010 | Experimental read: insertion XYZ, scale, rotation, extrusion for the six matching INSERT handles in the local sample | UNQUALIFIED | INDEPENDENT-READ via LibreDWG 0.14; no block expansion, array offset, ATTRIB semantics, write evidence, or other-version claim. The sample is not committed. |
| Classic 3D topology | DXF AC1027, ASCII and binary | Experimental 3D POLYLINE/VERTEX and polyface read | Experimental 3D POLYLINE/VERTEX and polyface write | SELFTEST and CONSUMER-PROBE for 3D POLYLINE XYZ delivery; EXT-ACCEPT for generated input conversion. Polyface self-tests are not all represented in the consumer probe; DWG child ownership/count claims remain separate. |
| Classic 3D topology | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | No independent per-version semantic witness. |
| Subdivision topology / MESH | DXF AC1027, ASCII and binary | Experimental MESH field read | Experimental MESH field write | SELFTEST and CONSUMER-PROBE for XYZ, face/edge indices, and creases; EXT-ACCEPT for generated input conversion; not an independent topology comparison. |
| Subdivision topology / MESH | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | DWG class/layout support is not established by local self-read. |
| Curves and analytic/NURBS surfaces | DXF AC1027, ASCII and binary | Experimental mapped-field read for SPLINE, HELIX, and six surface subtypes | Experimental mapped-field write for those tested records | SELFTEST; CONSUMER-PROBE covers SPLINE knots/control points and selected LOFTED-surface fields/carrier distinction only. No curve evaluation, surface evaluation, or independent semantic oracle. |
| Curves and analytic/NURBS surfaces | DWG R set / W set | UNQUALIFIED per reader version | UNQUALIFIED per writer version | Except for the scoped AC1024 scenario-2 fit-only row below, authentic per-version semantic witnesses remain missing. No named modern surface layout was found in the reviewed ODA v5.4.1 text. |
| SPLINE fit-only field subset | DWG AC1024 / R2010 | Experimental read: degree, scenario, fit tolerance, tangent vectors, and all seven XYZ fit points on both sample records | UNQUALIFIED | INDEPENDENT-READ via LibreDWG 0.14, joined by handle. This sample contains scenario-2 fit-only splines; it does not validate control points/knots, writes, or other versions. The sample is not committed. |
| ACIS 3DSOLID / REGION / BODY carriers | DXF AC1015 ASCII; AC1018 ASCII; AC1021 binary; AC1024 ASCII | Experimental opaque SAT chunk read/retention | Experimental opaque SAT chunk write through AC1024 | SELFTEST; verifies carrier/chunk identity only, not ACIS semantics. Unassociated AC1027+ SAB writes are rejected; entity-to-ACDSDATA association is unqualified. |
| ACIS 3DSOLID / REGION / BODY carriers | DWG AC1024 / R2010 inline version-2 SAB | Experimental exact opaque payload extraction for five 3DSOLID records in one local sample only; other entities/variants remain unqualified | No typed DWG modeler-geometry writer route | INDEPENDENT-CARRIER-MATCH via LibreDWG 0.14: payload size and SHA-256 match by handle after accounting for the separately reported `ACIS BinaryFile` signature. ODA v5.4.1 §20.4.41 anchors the container boundary. This is byte-preservation evidence, not ACIS semantic decoding. The local DWG is not committed. |
| ACIS 3DSOLID / REGION / BODY carriers | DWG other versions, AC1024 SAT/other variants, and AC1027+ DataStorage | UNQUALIFIED per version/path | No typed DWG modeler-geometry writer route | R13/R14/R2000 version-1 SAT blocks, AC1018/AC1021/AC1027/AC1032 inline variants, DataStorage association, and exact cross-version handle/frame accounting remain unqualified. S3.1's marker validation and the narrow AC1024 SAB carrier match do not promote them. |
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
