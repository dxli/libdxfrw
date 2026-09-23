# DWG/DXF 3D Support Review and Fix Plan

Status: implementation in progress; DWG reader edits remain evidence-gated.
Review date: 2026-09-23.
Implementation baseline: `423cf99fd26bc9938dc259907e2889666d672d1f`
(`origin/master`, `codex/pr100-clean` after rebase). The local commit was
patch-equivalent to this origin commit and was skipped by rebase. Existing
untracked workspace files, including this plan and `tests/samples/AC1021/` and
`tests/samples/AC1024/`, were preserved and not staged.

## Objective and limits

Close evidence-backed correctness and interoperability gaps in libdxfrw's 3D
geometry paths for DXF and DWG. Treat these as separate capabilities:

1. **Semantic geometry** — typed, correctly interpreted and editable data.
2. **Opaque payload preservation** — exact data can be retained and written
   without claiming that it was decoded or edited.
3. **Derived display geometry** — a preview/wireframe approximation, never a
   replacement for the original geometry payload.

The plan does not promise a general ACIS kernel, boolean operations, SAT/SAB
authoring, NURBS evaluation, subdivision, rendering, or full 3D CAD modeling.
Only expose those capabilities if an implementation and independent evidence
actually support them. This is a focused supplement to
`LIBRECAD_DXFRW_UPGRADE_PLAN.md`; it does not reopen unrelated format families.

## Consumer compatibility and enablement target

The compatibility rule is **additive 3D support, not a 2D-to-3D behavior
replacement**. Keep two independently tested consumer lanes:

1. **Existing 2D consumers, including LibreCAD:** preserve current source-level
   `DRW_Interface` compatibility and callback behavior. In the audited
   LibreCAD filter, both `dwgRW::read` and `dxfRW::read` are called with
   `ext == true`; preserve that established 2D-oriented mode and the meaning
   of existing callbacks when adding 3D data access. Do not remove or rename
   existing virtuals, add new pure-virtual requirements, silently change
   existing callback dispatch/field semantics, or project coordinates in the
   library. A 2D adapter may deliberately drop Z for display, approximate a
   curve, or retain source fields in its own metadata, but that behavior is
   not a libdxfrw data-support claim. S7.5 records the current LibreCAD
   boundary; no 3D UI migration is implied.
2. **3D-capable consumers:** make typed 3D fields and topology available without
   lossy XY projection through the opt-in `ext == false` read path, document
   each field's coordinate frame and ownership, and keep opaque modeler bytes
   distinguishable from decoded semantics. Add a headless 3D-aware
   consumer/test adapter that demonstrates full-coordinate delivery through
   the public API. New callbacks remain additive with default implementations
   (or delegation where appropriate), so a 3D consumer can opt in without
   forcing changes on an existing 2D adapter. This enables downstream
   consumers to build scene geometry; it does not add a renderer, tessellator,
   or ACIS kernel to libdxfrw.
3. **FreeCAD's DWG conversion/import path:** support the concrete
   `Draft.importDWG.open()` workflow as three separate contracts:
   `dwg2dxf` discovery/argv/output publication, correct DXF entity and field
   serialization by the converter, and the selected FreeCAD DXF importer's
   construction of expected geometry. The converter must preserve legal DXF
   entity types and 3D coordinates; it must not rewrite a rejected entity to a
   different 2D type merely to suppress FreeCAD's unsupported-feature report.
   Require at least one nonzero-Z geometry witness to pass the complete
   DWG→`dwg2dxf`→DXF→FreeCAD route before claiming FreeCAD 3D-consumer coverage.
   Pin FreeCAD release/revision, OS/architecture, importer implementation and
   settings for each runtime result. This is an optional integration lane, not
   a build dependency or a claim that libdxfrw changes FreeCAD's own importer.

Acceptance contract for every public API/callback change:

- An unchanged legacy 2D adapter still compiles and receives its established
  callbacks and values. New callbacks are optional, with default behavior;
  do not require a LibreCAD source change just to keep the current 2D path
  working. This is a source-compatibility promise, not a binary-ABI promise.
- A 3D-aware adapter can explicitly select the unprojected read path
  (`ext == false`) and receive available XYZ coordinates, coordinate-frame
  metadata, topology, transforms, and separately labeled opaque payloads.
  Do not flatten, discard, or reinterpret that data to accommodate a 2D
  consumer.
- Where a vector is meaningful in both lanes, test the same input through
  both modes: assert the legacy `ext == true` result is unchanged and the
  `ext == false` result retains the parsed 3D/OCS values. Keep expected values
  independent of a writer-reader round trip when a coordinate transform is
  involved.
- LibreCAD remains a supported 2D consumer without a forced migration. Its
  choice to project, approximate, or retain sidecar metadata stays in its
  adapter; enabling library-level 3D data access does not claim that LibreCAD
  displays or edits 3D geometry.

These lanes are orthogonal to format qualification: source-level data access,
callback delivery, semantic correctness per format/version, and GUI display or
editing are four distinct evidence levels. Keep the existing 2D route stable
and make typed 3D delivery additive; do not make consumers switch modes based
on entity type or mutate shared parser data to suit either rendering policy.
Existing 2D behavior must not hide Z or topology from the new 3D adapter, and a
successful 3D adapter test must not be read as proof that LibreCAD displays or
edits those fields. Preserve source compatibility; do not claim binary ABI
compatibility from defaulted virtual callbacks.

## Findings from the current source review

These are observations about code paths and existing tests, not blanket support
claims.

| Area | Current code evidence | Review consequence |
| --- | --- | --- |
| `3DFACE` | DXF library read/write routes exist. S5.1 found the in-tree `dx_iface` did not dispatch `E3DFACE` on output; it now does. The reader requires XY components for the first three corners, accepts the fourth corner as optional and copies corner 3 when absent. The DWG parser bounds its invisible-edge flags to `0x0f`; the DXF writer now rejects group-70 values outside `0..15` before the 16-bit write. | Generated ASCII/binary DXF vectors check WCS corners and group-70 invisible-edge bits. The in-tree writer integration is fixed. A fast in-memory ASCII vector verifies the legal omitted-fourth-corner fallback, while a half-present fourth corner is rejected before callback publication. S5.3 verifies invalid flag values fail without publishing output. This does not qualify DWG layout/version support. |
| `3DLINE` | There are typed DXF/DWG routes and a DWG custom-class number. | Verify exact class registration and per-version availability against a real trace; third-party class numbers are not adequate DWG evidence. The searchable ODA v5.4.1 text reviewed here has no `3DLINE` entry, so modern custom-class layouts need a newer primary reference or target-produced, independently checked witness; keep pre-R13 forms on their own evidence lane. |
| 3D point/line families | `POINT`, `LINE`, `RAY`, and `XLINE` carry WCS 3D data; the DWG implementation also has distinct legacy reader code for pre-R13 3DLINE and a modern custom-class route. | Do not equate a 3D `LINE` with the implementation-specific `3DLINE` record. Audit legacy reader versions separately from custom-class DWG versions. Autodesk's current DXF ENTITIES index does not list `3DLINE`; treat that DXF spelling as an extension until a target-application witness establishes portability. |
| Planar entities placed in 3D | ARC/CIRCLE, SOLID/TRACE, 2D POLYLINE/LWPOLYLINE, HATCH, and INSERT use OCS/elevation/extrusion in different ways. DXF ELLIPSE center and major-axis vector are WCS, with extrusion providing its plane normal. INSERT adds scale, rotation, array spacing, and block-base transforms. | INSERT's nested placement matrix is covered in S5.2. S5.1 covers SOLID/TRACE corner fields and TRACE projection; S5.4 corrects the DXF ELLIPSE `ext=true` double-transform and checks WCS invariance in both modes. Broader ARC/CIRCLE/HATCH OCS, thickness, and DWG ELLIPSE qualification remain open. Continue to use Autodesk's arbitrary-axis and per-entity rules, not a transform helper round-trip alone. |
| Classic `POLYLINE` 3D forms | The model stores 3D polylines, polygon meshes, and polyfaces in the `POLYLINE`/`VERTEX`/`SEQEND` family. DWG dispatch has separate vertex and face types, owned-child handling, and subtype checks. S5.1 corrected DXF polyface output to include groups 71/72 for declared vertex/face counts, use the polyface/face-record subclass markers, omit vertex group 91 from polyface records, and emit the legal SEQEND subclass set. S5.5 closes the DWG→DXF PFACE face gap: DWG face subtype (whose layout has no flags field) now supplies DXF face-record bit 128 and signed index serialization. | Generated ASCII/binary vectors verify WCS 3D-polyline points and signed polyface indices; LibreDWG 0.14 independently reads both. A locally generated AC1015 control exercises this conversion path only; it is not target interoperability evidence. Keep versioned DWG ownership/count qualification separate. DXF readers must remain tolerant of legal child ordering; writers emit coordinate vertices before faces. |
| `MESH` / `AcDbSubDMesh` | Typed vertex/face/edge/crease/property-override data, topology validation, DXF and DWG encode/decode paths, and generated local round-trip tests exist. S5.1 added the missing in-tree `dx_iface` MESH read callback and write dispatch. | Generated ASCII/binary DXF vectors compare typed vertices/faces/edges/creases; LibreDWG 0.14 independently reads both. Self-round-trips remain consistency checks, not DWG-layout evidence. Autodesk's DXF table is useful for DXF group codes; the searchable ODA v5.4.1 text reviewed here has no named `AcDbSubDMesh` DWG layout. Keep DWG MESH layout/version claims unqualified until primary DWG evidence or a target-produced, independently checked witness exists. |
| Six analytic/NURBS surface classes | Typed DXF paths exist. The DWG surface parser retains a bounded raw ACIS body and links DataStorage; DWG surface encoding rejects versions before AC1021. Class registration and modern DWG read/write paths exist. | The searchable ODA v5.4.1 text reviewed here has no named modern `AcDb*Surface` layouts. Keep DWG surface layout/version claims unqualified until feature-specific primary evidence or target-produced, independently checked witnesses exist. Keep the AC1021+ writer gate meanwhile; check each typed field, handle, transform, and ACIS carrier separately. Do not imply surface evaluation. |
| `3DSOLID` / `REGION` / `BODY` and ACIS | DXF R2000–R2010 stores SAT groups 1/3 on the entity; R2013+ may place SAB in `ACDSDATA`. At baseline, `writeModelerGeometry()` treated SAT as text only through AC1018 and wrote binary chunks for later versions. S2.2 now routes textual payloads through groups 1/3 for AC1015/1018/1021/1024 and rejects binary, mixed, DWG-frame, and AC1027+ inline payloads rather than guessing. Generic raw-DXF-section preservation remains independent; typed association between a modeler entity and ACDSDATA is still not demonstrated. In DWG, the parser reads modeler status/version/history and skips bounded body data; the dispatcher later assigns the entire DWG object frame body to `DRW_ModelerGeometry::m_rawBytes`. For AC1027+ it also attempts DataStorage linking into a separate field. `decodeWireframe()` still chooses DataStorage when linked; the DXF writer no longer treats DWG frame/DataStorage metadata as SAT. `DRW_ModelerGeometry` has no DWG encoder, and `dwgRW` has no typed modeler-geometry write route. | Keep frame bytes, inline ACIS bytes, ACDSDATA/AcDsPrototype bytes, and proxy bytes separate. SAT routing is supported only through the explicitly tested AC1024 lane; AC1027+ SAB-to-entity association and DWG modeler writing remain unsupported/unqualified. |
| NURBS/spline curve path | Local ODA v5.4.1 §20.4.40 specifies R2013+ `Spline flags 1` as BL and the subsequent `Rational`, `Closed`, and `Periodic` values as individual B fields in scenario 1. The current `parseDwgSplineBody()` reads BL for `splFlag1`/`knotParam` only when `version > AC1024`, matching that stated version boundary; the one-bit fields are read separately. The initial suspected width defect is not supported by the cited ODA text. No authentic AC1027/AC1032 spline witness has been identified. | Do not change the DWG flag width based on the stale issue note. Keep the per-version DWG spline path unqualified until authentic target samples/field traces verify the current parse; do not infer AC1032 from the AC1027 pass-through reader. |
| Semantic comparison | S1.1 added field-level serializers for MESH, modeler geometry, and surfaces; binary values and raw carriers remain explicitly separate digest/opaque fields. | Field-level serializer output improves diagnostics and mutation sensitivity, but still compares adapter observations rather than independently proving format semantics. S7/S8 witnesses remain necessary for support claims. |
| Unknown DXF sections | `dxfRW::processRawDxfSection()` captures unrecognized sections as `DRW_RawDxfSection`; the writer can re-emit supplied raw sections. | This is a useful opaque-preservation route for `ACDSDATA`, but the 3D plan must test the complete read/callback/consumer/write chain and must not call it a typed ACIS link. |
| DXF ACIS version routing | S2.2 now writes SAT text chunks as groups 1/3 through AC1024, records/validates DXF chunk identity, and refuses unassociated AC1027+ inline bytes. The surface writer applies the same SAT gate while keeping subtype-specific binary group-310 data distinct. | Autodesk's AutoCAD 2010/current DXF references list proprietary payload groups 1/3; ezdxf independently documents SAT inline through R2010 and SAB in ACDSDATA from R2013+. LibreDWG 0.14 `dxf2dwg` successfully read locally generated AC1015/1018/1021/1024 SAT vectors. Keep AC1027+ SAB-to-entity association opaque until a valid identity/link contract is implemented. |
| Existing tests | `tests/dwg_local_roundtrip_tests.cpp` constructs local DXF/DWG vectors and covers 3D faces, MESH, surfaces, and ACIS/SAB-carrier cases. | Keep these as the fast inner loop. They prove internal consistency for their vectors, not conformance to AutoCAD/ODA or another reader. |

### Core format distinctions that the implementation must preserve

- `SOLID` and `TRACE` are not `3DSOLID`; ACIS-based `3DSOLID`, `REGION`, and
  `BODY` must not be treated as ordinary quadrilateral faces.
- DXF `3DFACE` corners are WCS points; the optional fourth corner duplicates
  the third. Its group-70 bits mark individual invisible edges.
- Classic `POLYLINE` flags distinguish 3D polylines, polygon meshes, and
  polyface meshes. A 2D polyline's coordinate/elevation and extrusion semantics
  are not interchangeable with 3D WCS vertex semantics.
- `LINE` can contain WCS Z coordinates; the `3DLINE` spelling is a separate
  legacy/custom family and must not be used as the portable synonym for LINE.
- OCS entities use the DXF arbitrary-axis construction and per-entity
  elevation/extrusion rules. An INSERT's insertion point is OCS and its scale,
  rotation, extrusion, array spacing, and block base point compose with
  geometry in the referenced block.
- `MESH` is one subdivision-surface entity with base-cage topology; it is not
  a polyface `POLYLINE`, and its flat face-list count is not the number of
  polygon faces.
- ACIS bytes, DWG encoded object-frame bytes, proxy-graphics bytes, and
  DataStorage payload bytes are separate carriers. DXF `ACDSDATA` raw sections
  also need separate identity and association evidence. Store and qualify each
  independently, including source version, carrier encoding, size, and
  identity. Only compare byte digests where the compared representation is
  meant to be byte-identical; cross-container conversion may change framing or
  SAT/SAB carrier form.

## Completeness boundary and review matrix

The following inventory closes the “3D” scope before coding. Each row must
receive a per-format/version disposition in S0, even if the disposition is
`unsupported`, `opaque`, or `not applicable`.

| Family | In-scope members | Completeness checks | Current disposition |
| --- | --- | --- | --- |
| WCS primitives | POINT, LINE with nonzero Z, RAY, XLINE, 3DFACE | WCS endpoints/corners, optional values, edge flags, finite values | Typed routes exist; qualification incomplete |
| 3DLINE variants | Pre-R13 legacy 3DLINE; modern custom DWG class and DXF `3DLINE` spelling | Keep legacy type 21 separate from modern class identity; determine DXF portability and supported versions from witnesses | Typed paths exist; extension/version support unqualified |
| Planar geometry in 3D | ARC, CIRCLE, ELLIPSE, SOLID, TRACE, 2D POLYLINE, LWPOLYLINE, HATCH | OCS arbitrary-axis frame, elevation, extrusion, thickness, angle direction, vertex order | Existing routes; coordinate/round-trip matrix missing |
| Placed block geometry | INSERT/MINSERT and block contents | OCS insertion point, block base point, nested transform composition, nonuniform/mirrored scales, rows/columns/spacings, attributes/ownership | Existing generic routes; 3D transform-chain qualification missing |
| Classic 3D topology | 3D POLYLINE, polygon mesh, polyface, VERTEX, SEQEND | WCS vertices, flags, closure, M/N order, signed one-based face indices, edge visibility, child ordering, counts and handles | Typed routes exist; independent qualification incomplete |
| Subdivision topology | MESH / AcDbSubDMesh | Base-cage vertices, flat face-list counts, n-gons, edges, crease values, property overrides, version gate | Typed routes and generated tests exist; oracle/version checks incomplete |
| Curve/surface geometry | SPLINE, HELIX, plane/extruded/revolved/swept/lofted/NURBS surfaces | Degrees, knots, weights, control point order, closure/periodicity where represented, sweep/profile handles, matrices, flags, version gates | Partial typed routes; DWG spline discrepancy known; Helix and all field mappings still need inventory |
| ACIS modeler geometry | 3DSOLID, REGION, BODY; SAT and SAB carriers | Entity-inline payload vs DXF ACDSDATA vs DWG DataStorage; frame vs payload separation; empty/history state; preservation versus semantic decode | Critical carrier gap; no typed DWG writer route observed |
| Proxy/derived geometry | Proxy graphic data associated with 3D entities | Preserve raw carrier separately; transformations/styles/stop reason/caps; derived wireframe never suppresses source payload | Existing general proxy path; 3D-specific interactions need cross-check |
| Excluded from geometry claims | Camera/view state, lights, point clouds, Navisworks/reference models, geodata | Record scope boundary; these may contain 3D positions but are not this geometry plan's native surface/solid topology target | Handle under their existing feature plans, not counted as 3D-model support |

The matrix is complete only when S0 adds the actual legacy reader names and
writer gates for each row. In particular, do not omit R1.4/R11-era reader
paths merely because the current ODA document focuses on later DWG layouts.

## Implementation work packages

Each package is split into a small green slice/commit. IDs and states below are
the initial ledger; update this section in the same commit as each implemented
slice. `READY` means dependencies are committed and the positive and negative
gates are known.

### S0 — Baseline, evidence lock, and capability inventory

State: source/version/authority/oracle inventory is complete; per-family DWG
layout and target-witness gaps remain isolated in S3/S6 rather than blocking
the independent DXF lanes.

Dependencies: none. Output: updated capability ledger in this file, with a
baseline pinned to the rebased branch.

Files: this plan; `src/drw_entities.h`; `src/drw_interface.h`;
`src/libdxfrw.{h,cpp}`; `src/libdwgr.{h,cpp}`; `src/intern/dwgreader*`;
`src/intern/dwgwriter*`; `tests/`.

Steps:

1. Synchronize/rebase the implementation branch onto current `origin/master`;
   record `HEAD`, `origin/master`, dirty paths, and whether any user-owned
   untracked files were left untouched.
2. Resolve and read the authoritative ODA v5.4.1 PDF required by `AGENTS.md`.
   The canonical basename was absent, but
   `/Users/dli/doc/dwg/OpenDesign_Specification_for_.dwg_files (1).pdf` is
   readable, identifies itself as Version 5.4.1, and has 279 pages matching
   the official [published ODA specification](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf).
   Read the relevant chapter before changing any `parseDwg(...)` body or any
   file under `src/intern/dwgreader*`, exactly as `AGENTS.md` requires. This
   resolves the authority-file blocker, not feature-layout gaps: record the
   applicable section (or that the reviewed v5.4.1 text has no applicable
   section) for every DWG family/version row.
3. Build a capability ledger with rows for DXF ASCII/binary read/write and
   each DWG read/write version, including the legacy R1.4/R11 reader paths
   found in this source tree. For every row record the concrete dispatch,
   typed fields, raw carriers, version gate, class identity evidence, test
   type, and support-claim ceiling. Unknown means unknown, not supported.
4. Capture empirical DWG custom-class/type identities only from real local
   files/traces, per `AGENTS.md`; do not copy class numbers from third-party
   documentation into the reader. The AC1024 `visualization_-_conference_room.dwg`
   trace records class/type 524 as `AcDbPlaneSurface`, record name
   `PLANESURFACE`, entity flag 1, and two instances; conversion emits two
   `PLANESURFACE` records. This is one-file identity/dispatch evidence only,
   not a layout or semantic qualification. No 3DLINE class identity was found.
5. Add an entity-specific compatibility column: normative DXF/DWG spelling,
   core versus vendor/custom class, legacy versus modern encoding, and
   whether the selected LibreDWG/ezdxf oracle marks the path stable. Treat the
   absence of an entity from an Autodesk reference index as a portability risk,
   not as proof that no application ever emits it.
6. Maintain an evidence map, not just a generic “ODA checked” flag. The local
   v5.4.1 searchable text confirms classic vertex/polyline, SPLINE (§20.4.40),
   ACIS modeler (§20.4.41), and DataStorage (§24) material, but has no named
   layout for `AcDbSubDMesh`, modern `AcDb*Surface`, or `3DLINE`. Do not infer
   a byte layout from an absent section, DXF definitions, or another project's
   implementation. For a genuine gap, require a newer primary format
   reference or target-generated DWG plus trace and independent-reader
   confirmation. Pre-R13 readers require their own era-appropriate authority
   and sample; R13+ ODA layouts cannot qualify legacy forms.
7. **S0.7 — Accept null dictionary child references without weakening DXF
   validation.** ODA v5.4.1 §§2.13 and 20.4.44 allow a null DWG H reference;
   accept it only with a zero counter and a valid null/item-handle code.
   Keep the separate `DICTIONARYWDFLT` default hard-pointer required per
   §20.4.45. Add runtime-generated DWG writer/reader vectors for both a regular
   dictionary and dictionary-with-default membership, preserve zero in the
   callback, and keep DXF group-350 validation strict until a valid DXF mapping
   is separately established. Do not stage the local authentic DWG that
   motivated the correction.

Source-derived dispatch inventory (routes are code evidence, not proof of
complete format conformance or support):

| DWG signature / family | Read route | Typed-write route | Qualification ceiling / compatibility note |
| --- | --- | --- | --- |
| AC14 (R1.40) | `dwgReaderR1_40`, dedicated pre-R2.0b container | No writer; `dwgRW::write()` rejects this version | Separate era/container; no applicable ODA v5.4.1 layout, and no current target witness for this plan. |
| AC1003 / AC1004 / AC1006 / AC1009 (R2.10/R9/R10/R11) | `dwgReaderR11`; legacy shared entity/table paths, with version gates | No writer; the writer accepts AC1015 and later only | `createReaderForVersion()` currently routes AC1009 to `dwgReaderR11`, contrary to the stale “AC1009 unsupported” row in the local guidance. Treat read availability as implementation evidence only; it does not establish broad reliability. Legacy 3DLINE is a distinct pre-R13 body and needs its own sample/era-specific authority. |
| AC1012 / AC1014 / AC1015 (R13/R14/R2000) | `dwgReader15` | `dwgWriter15` only for AC1015 output | AC1012/1014 are read-only in this writer API. ODA §20.4.41 covers the R13+ modeler family; older entity routes still need per-version tests. |
| AC1018 (R2004) | `dwgReader18` owns the R2004+ page/file-header lineage | `dwgWriter18` | Modeler body is bounded by object/handle streams, but payload recovery still needs a valid sample and independent carrier check. |
| AC1021 (R2007) | `dwgReader21`, independent page/compressor lineage | `dwgWriter21` | R2007+ modeler/SAB body path is not interchangeable with R2004. Keep per-version results separate. |
| AC1024 (R2010) | `dwgReader24` over reader18 | `dwgWriter24` | Local successful traces exist; individual family/layout evidence still controls qualification. |
| AC1027 (R2013) | `dwgReader27` over reader24/18 | `dwgWriter27` | ODA §24 describes AcDsPrototype/DataStorage; typed object association must still be one-to-one and sample-verified. |
| AC1032 (R2018+) | `dwgReader32`, currently a pass-through stub over reader27 | `dwgWriter32` | Never infer R2018 correctness from an R2013 trace; version deltas remain unqualified without a target witness. |
| Other signatures | `UNKNOWNV`, MC00, AC12, AC150, and AC1002 are rejected by `createReaderForVersion()` | Rejected by writer version gate | Unsupported, not an implicit fallback. |

The family-level code routes currently visible in `src/intern/dwgreader.cpp`
and `src/drw_entities.cpp` are: standard WCS primitives/3DFACE; legacy plus
modern custom-class 3DLINE; compound POLYLINE/VERTEX/SEQEND; custom-class MESH;
SPLINE plus custom-class HELIX; modeler REGION/3DSOLID/BODY; and surfaces
dispatched by the DWG class record name. Writers exist for several typed paths,
but no `DRW_ModelerGeometry::encodeDwg()` exists; the current modeler output
claim is therefore read/opaque-frame retention only. MESH/surfaces have
encoders but stay unqualified on DWG layout semantics because the applicable
named primary layouts and independent target checks are absent. DXF ASCII and
binary share the per-entity parse/write routes, with group-code storage and
version gates handled below those routes; a format-specific positive result
does not imply the paired DWG route.

Oracle compatibility evidence (reviewed 2026-09-23; use for test selection,
not as a substitute for Autodesk/ODA format requirements):

| Family/path | ezdxf reference behavior | LibreDWG reference/runtime behavior | Plan consequence |
| --- | --- | --- | --- |
| `3DFACE` | The current entity reference describes four WCS vertices and the four independent invisible-edge bits. | Prior S5.1 ASCII/binary DXF vectors were accepted by LibreDWG 0.14. | Suitable independent DXF field-acceptance oracle for corners/edge flags; does not establish DWG support. |
| Classic `POLYLINE`/polyface | The reference distinguishes 2D OCS from 3D WCS and exposes separate 2D/3D/polygon-mesh/polyface modes. | Prior S5.1 classic 3D-polyline and polyface ASCII/binary vectors were accepted by LibreDWG 0.14. | Compare subtype flags, vertex order, and face indices; keep child ownership/versioned DWG checks separate. |
| DXF `MESH` | The entity reference describes a single subdivision cage with vertices, edges, and faces; n-gons are distinct from classic polyface limits. | Prior S5.1 vectors were accepted by LibreDWG 0.14. LibreDWG 0.11 NEWS historically lists MESH as stable, but that old label is not a 0.14 guarantee. | Current tested DXF vectors may use LibreDWG for acceptance; DWG layout remains unqualified. |
| DXF `HELIX` / `SPLINE` | HELIX is documented as a cubic B-spline-derived entity, required from R2000, with WCS points and constraint values 0–2. | LibreDWG 0.11 NEWS lists HELIX under unstable classes; this historical note is not a current compatibility statement. No matching authentic per-version DWG spline run is in this pass. | Use ezdxf docs for DXF field expectations; use runtime self-roundtrips only for internal consistency; leave DWG version claims unqualified. |
| Analytic/NURBS surfaces | ezdxf exposes DXF factories/entity classes from R2007+, but those APIs are not proof of surface evaluation or all subtype semantics. | LibreDWG 0.11 NEWS lists PLANESURFACE as debugging-only; in this pass LibreDWG 0.14 surface/NURBS DXF conversions emitted unsupported-code/parse diagnostics. On AC1024 conference-room and AC1027 Cover DWGs, `dwgread -O DXF` exited successfully but emitted zero of the checked 3D entity names; libdxfrw's conversions emitted 37 and 1 respectively. | No positive cross-reader claim for surface fields or DWG surfaces; any “success” exit without the expected records is a failure to qualify. |
| `3DSOLID` / `REGION` / `BODY` ACIS | ezdxf documents only a limited supported ACIS subset, states unsupported bodies load as `NONE_ENTITY` with data loss, and explicitly warns arbitrary ACIS cannot be loaded/re-exported. | LibreDWG 0.11 NEWS says its R2013+ AcDsPrototype datastore was not fully decoded and SAB was then extracted by brute force; local 0.14 target-DWG runs above did not emit the checked modeler entities. | Neither oracle can promote arbitrary ACIS semantic read/write. Test byte/carrier preservation separately and require exact identity plus a more capable witness. |

The installed `dwgread` reports LibreDWG 0.14. The 0.14 local tests above are
reproducible observations on those named files, not a support matrix. LibreDWG
NEWS stability labels are explicitly historical (0.11), not current claims.
ezdxf 1.4.4 documentation was reviewed as the selected DXF field oracle, but
the `ezdxf` package is not installed here, so no executable ezdxf result is
claimed. Relevant primary project references:
[ezdxf 3DFACE](https://ezdxf.readthedocs.io/en/stable/dxfentities/3dface.html),
[POLYLINE](https://ezdxf.readthedocs.io/en/stable/dxfentities/polyline.html),
[MESH](https://ezdxf.readthedocs.io/en/stable/dxfentities/mesh.html),
[HELIX](https://ezdxf.readthedocs.io/en/stable/dxfentities/helix.html),
[ACIS limitations](https://ezdxf.readthedocs.io/en/stable/acis.html), and
[LibreDWG NEWS](https://github.com/LibreDWG/libredwg/blob/master/NEWS).

Positive gate: one reviewed matrix ties each in-scope row to code, normative
paragraphs (or a recorded normative-source gap), and existing or planned
tests. Negative gate: any unsupported family/version or DWG reader change
without its applicable authority and authentic sample/trace is explicitly
blocked, not silently counted as implemented. For layouts absent from the
available ODA edition, keep only evidence-backed lanes moving; the missing
authority is a per-family blocker, not permission to extrapolate.

### S1 — Canonical semantic evidence for 3D entities

State: the planned canonical serializer and adapter self-test slices are
implemented (S1.1/S1.2). Independent format qualification remains separate in
S7/S8.

Dependencies: S0. Keep this as a test/adapter slice; it must not edit any
`src/intern/dwgreader*` file before the local ODA reference gate is met.

Files: `tests/semantic_differential_adapter.cpp`;
`tests/dwg_local_roundtrip_tests.cpp`; focused helpers under `tests/`.

Steps:

1. Replace opaque semantic output for `DRW_3Dface`, `DRW_3DLine`,
   `DRW_Polyline`/`DRW_Vertex`, `DRW_Mesh`, `DRW_Helix`, `DRW_Insert` and its
   ATTRIB children, modeler geometry, and each surface subtype with
   deterministic field-level JSON.
2. Serialize geometry separately from raw bytes: WCS/OCS values, extrusion,
   thickness, flags, ordered vertices/faces, signed polyface indices, mesh
   topology/creases/overrides, curve knots/weights/control points where
   represented, surface scalars/points/matrices/handles, modeler status and
   version, nested INSERT/ATTRIB placement and text fields, typed loft-reference
   values, and ACIS/proxy/DataStorage/raw-section carrier length plus digest.
   Include common layer/owner/handle and compound-child identity in separate
   fields. Never include raw payload contents in semantic fields or diagnostics.
3. Make unknown fields/version-specific omissions visible. Keep a distinct
   “opaque-preserved” result where a semantic parse is not available.
4. Add generated positive vectors and malformed/truncated negatives without
   storing new DWG/DXF fixtures in the repository.

Positive gate: self-read tests compare every emitted 3D field, and a one-field
mutation causes a semantic diff. Negative gate: raw bytes in a source DWG
object frame cannot compare equal to its ACIS payload merely because both use
the same public object.

### S2 — Separate DXF modeler payloads from sections and other carriers

State: the bounded DXF carrier/read/write slices are implemented (S2.1/S2.2).
Typed AC1027+ entity-to-DataStorage association remains limited to S3.2.2 and
does not imply generic section association.

Dependencies: S0, S1. This slice may edit public entity fields and DXF paths,
but must not edit `src/intern/dwgreader*` until the DWG spec gate is met.

Files: `src/drw_entities.h/.cpp`; `src/libdxfrw.{h,cpp}`;
`tests/dwg_local_roundtrip_tests.cpp`;
`tests/semantic_differential_adapter.cpp`.

Steps:

1. Define/document distinct carriers for DXF entity-inline text groups 1/3,
   DXF entity-inline binary group 310 chunks, the `ACDSDATA` raw section,
   DWG encoded object-frame bytes, DWG inline payload bytes, proxy graphics,
   and linked DWG DataStorage bytes. Reuse generic raw-section preservation
   for opaque `ACDSDATA` until there is evidence for typed section/entity
   association. Preserve the DXF modeler groups as ordered `(group code,
   offset, length)` views into the existing bounded byte buffer; do not merge
   group identity into a single unlabeled byte string.
2. Preserve source compatibility: keep existing public fields/callbacks where
   practical, add an explicit typed payload carrier/status, and keep any new
   `DRW_Interface` virtual non-pure with a no-op default.
3. Use an explicit DXF version matrix: SAT groups 1/3 through R2010; for
   R2013+ verify whether SAB resides in `ACDSDATA`, whether the
   associated `3DSOLID`/`REGION`/`BODY` record carries a resolvable identity,
   and whether the full generic raw-section route retains it. Do not infer
   typed association from an opaque section callback.
4. Correct/qualify writer routing against Autodesk's version-specific DXF
   contract and independent readers: AC1015/AC1018/AC1021/AC1024 SAT vectors
   should use the text groups required by their DXF version; verify/correct
   AC1021/AC1024 rather than assuming the current group-310 fallback is valid.
   For AC1027+ SAB vectors, implement only the carrier and entity-identity
   relationship demonstrated by authoritative/target evidence; never silently
   flatten SAB to group 310. This is carrier transport for existing opaque
   payloads, not generation or editing of ACIS geometry. Verify every
   surface/modeler writer separately.
5. Preserve unknown raw sections independently; if a carrier cannot be
   represented or safely associated, retain it opaquely or fail explicitly
   rather than silently moving bytes into an entity record.

Positive gate: each version's DXF carrier is accepted by an independent reader;
same-version SAT/text and raw ACDSDATA retain their bytes and section/entity
identity through the documented path. Cross-version
or cross-container output is compared semantically only where an independent
ACIS decoder can establish equivalence; otherwise report opaque preservation
and the representation change. Negative gate: a DWG frame, proxy-only,
truncated, ambiguous, or orphaned section is never emitted as entity ACIS.

### S3 — Recover ACIS carriers from DWG entities and DataStorage

State: ODA v5.4.1 authority is resolved; S3.1 marker validation and S3.2.1's
AC1024/R2010 inline SAB carrier are committed and independently byte-matched
on 129 solids across four local samples. S3.2.2 also unblocks one AC1027/R2013
3DSOLID with `has_ds_data`: the entity callback receives the uniquely
handle-linked AcDs SAB record, and its exact bytes match LibreDWG 0.14. This is
opaque carrier identity evidence only. All other DWG parser/carrier changes
remain `BLOCKED_PER_VERSION` until the relevant section, successful target
trace, and independent witness are tied to the exact version/field; the AC1024
PlaneSurface class-table trace is identity evidence only. DXF and test-adapter
lanes remain independent.

Dependencies: S0 local spec/trace gate; S1; S2 carrier contract.

Files: `src/drw_entities.{h,cpp}`; `src/libdwgr.{h,cpp}`;
`src/intern/dwgreader*`; local runtime-generated vectors and semantic adapter.

Steps:

1. **S3.1 — modeler version marker validation.** ODA v5.4.1 §20.4.41
   states that a non-empty modeler body carries a BS `Version` of 1 or 2;
   the field is absent when the ACIS Empty bit is set. Validate this bounded
   field and test empty, versions 1/2, and out-of-range values with
   runtime-generated AC1018 frames. This is envelope validation only: the
   version-2 byte is opaque test filler, and no ACIS payload is decoded or
   qualified.
2. **S3.2 — per-version layouts.** For R13/R14/R2000/R2004/R2007/R2010/R2013/R2018,
   use applicable ODA sections and actual per-version traces to define exact
   body/data boundaries, remaining empty/unknown bits, ACIS data encoding,
   history-handle behavior, and DataStorage presence/identity rules. This
   package covers ACIS modeler entities only; do not absorb MESH or surfaces
   into it merely because those entities also use modern class records. ODA
   §20.4.41 notes that the ACIS stream is not fully decrypted; use it to
   bound/step/preserve data, not as authority for arbitrary SAT/SAB semantics.
   Do not use §20.4.41 as evidence for MESH or analytic/NURBS surface layouts;
   the searchable v5.4.1 text reviewed here did not expose named sections for
   those modern classes. Keep each such DWG subtype
   `BLOCKED_ON_PRIMARY_LAYOUT_EVIDENCE` until an applicable authority and
   independently checked witness are available; proceed with DXF lanes.
   **Completed sub-slice S3.2.1:** AC1024/R2010 non-empty version-2 inline SAB
   starts immediately after the modeler-version field, has the exact
   `ACIS BinaryFile` signature, and is bounded by the unique tagged
   `End-of-ACIS-data` marker within the entity data body. The returned payload
   is a separate opaque carrier; this does not establish SAT v2, other entity
   variants, other DWG versions, or ACIS semantics.
   **Completed sub-slice S3.2.2:** For AC1027/R2013, `has_ds_data` causes an
   entity-handle reference into `AcDb:AcDsPrototype_1b`; the local entity BS
   can decode as 168 even though the exact handle-linked `ACIS BinaryFile`
   record is SAB v2. The parser now tolerates that entity-local value only
   when the external-data flag is present, keeps the complete frame bytes
   separate, and sets effective modeler version 2 only after exact record
   association and signature validation. On the untracked `Cover.dwg`, handle
   `0x6f` receives 22,983 bytes whose SHA-256 matches LibreDWG 0.14 exactly.
   ODA v5.4.1 §§20.4.41 and 24 describe the modeler carrier and DataStorage
   record; LibreDWG's [`common_entity_data.spec`](https://github.com/LibreDWG/libredwg/blob/master/src/common_entity_data.spec),
   [`dwg.spec`](https://github.com/LibreDWG/libredwg/blob/master/src/dwg.spec),
   and [`acds.spec`](https://github.com/LibreDWG/libredwg/blob/master/src/acds.spec)
   corroborate the `has_ds_data` handle and post-link v2 interpretation. The
   LibreDWG comparison corroborates the one sample's payload bytes, not an
   independent/general association rule: LibreDWG's
   [NEWS](https://github.com/LibreDWG/libredwg/blob/master/NEWS) records
   incomplete, brute-force AcDs extraction in v0.11, while open
   [issue #1411](https://github.com/LibreDWG/libredwg/issues/1411) reports
   missing AC1027+ AcDs extraction with LibreDWG 0.14.8593 (including an
   AC1032 case). That issue is a
   blocker signal, not a positive sample or format authority. Runtime tests
   now cover unique selection, exact handle/key agreement, disagreement,
   duplicate-section ambiguity, version mismatch/orphan accounting, malformed
   SAB signature, idempotent replay, and two records linked while entity
   traversal order is reversed. Another open-source implementation's
   [AcDs round-trip code](https://github.com/hakanaktt/acadrust/blob/main/tests/roundtrip.rs#L3785-L3800)
   describes positional record/entity mispairing as a failure mode; that is a
   test-design lead, not format evidence. It also recognizes `ASM BinaryFile`
   (including the `BinaryFile4/8` form) alongside `ACIS BinaryFile`, but ODA
   v5.4.1 §24 documents the latter prefix for this carrier. Accordingly,
   alternate ASM-prefixed bytes remain attached
   only as opaque DataStorage, without effective ACIS version normalization,
   pending an applicable primary layout and authentic sample. This does not qualify another entity,
   any other AC1027/AC1032 variant, malformed
   or ambiguous DataStorage association, geometry semantics, or DWG writing.
3. **S3.3 — pre-R13 separation.** Inventory the R1.4/R11 readers separately.
   Do not apply R13+ layouts to their records; use an appropriate legacy
   specification and authentic version-specific sample trace before changing
   them. If no such source exists, leave code unchanged and keep the row
   unqualified.
4. **S3.4 — bounded extraction.** Implement bounded extraction and validation
   with transactional publication; malformed entity-level payloads must not
   consume adjacent frame/handle data or publish partial geometry. The
   AC1024 sub-slice scans the body linearly, requires the exact SAB signature
   and one complete compound marker, and publishes only after common handle
   parsing succeeds. Missing/duplicate markers remain opaque; the whole frame
   stays separate in `m_rawBytes`.
5. **S3.5 — DataStorage association.** Attach recovered ACIS payload to the
   typed modeler entity without overwriting its DWG object-frame carrier. For
   AC1027+ reconcile the entity's DataStorage marker, handle/key, exactly one
   selected data record, payload marker, and section version before
   publication. S3.2.2 completes only one AC1027 3DSOLID handle-linked SAB
   record; missing/conflicting/orphan records, other modeler entities,
   AC1032, and every writer route remain unqualified.
6. **S3.6 — raw replay.** Keep raw same-version frame replay as a separate
   operation and report whether a result is typed, raw-replayed, typed-and-raw,
   or unsupported. Treat the open [LibreDWG ACDS/ACIS report](https://github.com/LibreDWG/libredwg/issues/1411)
   as an investigation lead only. Confirm any R2013+ `AcDb:AcDsPrototype_1b`
   behavior against ODA and local bytes before changing carrier rules.

Positive gate: local-from-scratch parser vectors and any available authentic
sample agree on declared lengths, payload identity, data/handle boundaries,
DataStorage linkage, and frame accounting. Negative gate: wrong class/version,
conflicting handle/key identity, duplicate or orphan DataStorage, truncated
body, and deliberately mislabelled frame bytes fail closed. No semantic
solid-editing claim follows from raw pass-through.

### S4 — Add a DWG opaque modeler-payload writer where evidenced

State: `BLOCKED_ON_S3`; implement only after S3 has a verified read layout.

Dependencies: S0; S2 carrier contract; S3 verified per-version reader evidence.
Files: `src/drw_entities.{h,cpp}`; `src/libdwgr.{h,cpp}`;
`src/intern/dwgwriter*`; writer/local semantic tests.

Steps:

1. Add the public `dwgRW` route for a typed modeler-entity envelope carrying
   an explicitly opaque, unchanged ACIS payload; do not imply or implement
   ACIS authoring. Enable only versions with confirmed envelope/class and
   payload layouts. Preserve `DRW_Interface` compatibility (any new virtual
   has an empty default).
2. Register custom classes using the project writer's class-registration
   conventions, but validate emitted class identity, handles, history links,
   and DataStorage presence/record output against observed target files.
3. Keep inline body payloads and DataStorage section records as separate
   writer operations; ensure rollback removes both on a rejected entity.
4. Make an unsupported target version return an explicit unsupported-write
   result. Never substitute a raw source object frame for a newly encoded
   entity; exact raw replay, if supported, is a separately named operation and
   cannot be combined with field edits.

Positive gate: writer output is accepted by an independent reader for every
enabled version and has matching envelope-field/carrier evidence; compare an
opaque payload by its exact digest, not by claiming decoded ACIS semantics.
Negative gate:
unregistered class, absent/unbound payload, bad handle, unsupported version,
or failed DataStorage emission rolls back the entire entity and does not leave
an orphan record.

### S5 — DXF topology, coordinates, and finite-value semantics

State: planned DXF topology, placement, flags, and conversion-boundary fixes
are implemented in S5.1-S5.5. Their evidence remains vector/family-specific;
S7/S8 independently gate broader semantic claims and blocked DWG lanes.

Dependencies: S0, S1. Keep DWG-specific parser changes in S3.

Files: `src/drw_entities.{h,cpp}`; `src/libdxfrw.cpp`;
`tests/dwg_local_roundtrip_tests.cpp`; field serializers.

Steps:

1. Audit both ASCII and binary DXF records against Autodesk group codes for
   `3DFACE`, `POLYLINE`, `VERTEX`, `MESH`, `SOLID`, `TRACE`, `INSERT`, `HATCH`,
   `HELIX`, and surface entities. Preserve distinct WCS/OCS conventions,
   optional defaults, and version-specific entity availability.
2. Verify `3DFACE` three-corner fallback, all four invisible-edge bits,
   coordinate order, and WCS round trips. Reject or diagnose invalid values
   instead of silently truncating flags on write. S5.3 closes the output-side
   guard; the DXF reader remains permissive and retains the source integer so
   future/reserved bits are not silently discarded on input.
3. Verify old `POLYLINE`/`VERTEX` subtypes: open/closed 3D polyline, M/N
   polygon mesh, PFACE vertex then face streams, signed one-based face
   references, hidden edge bits, `SEQEND`, and inconsistent declared counts.
4. Verify `MESH` face-list item counts independently of face count, n-gons,
   valid vertex references, edge/crease alignment, property overrides,
   subdivision level, and supported DXF versions.
5. Exercise OCS-to-WCS transforms using known arbitrary-axis expected values
   for default, oblique, and negative normals; assert each transform happens
   exactly once. Include elevation and thickness separately. Do not transform
   WCS-only `LINE`/`3DFACE`/3D `POLYLINE`/`MESH`/DXF `ELLIPSE` coordinates as
   OCS data. DXF ELLIPSE center and major-axis coordinates stay WCS under both
   `ext` settings; keep the separate DWG delivery path version-qualified.
6. Exercise INSERT/MINSERT placement with a nonzero block base point, nested
   block, rotation, nonuniform and negative scales, oblique extrusion, arrays,
   and attributes. Compare the composed transform against a small independent
   matrix oracle; file read/write equality alone will not catch paired errors.
7. Preserve the group-code corner mapping for `SOLID`/`TRACE` in both DXF
   encodings. Autodesk documents SOLID group 13 as optional with corner four
   duplicating corner three; TRACE defines four OCS corners. The reader/writer
   round-trip vectors verify numbered-field preservation, not surface winding
   or rendered topology. LibreDWG accepts these generated DXFs but its tested
   R2000 conversion/re-export drops the nonzero corner Z values, so it is not
   an independent semantic oracle for those fields. Do not reorder corners or
   promote topology claims absent a suitable independent witness.
8. When converting DWG classic `POLYLINE` data to DXF, map the typed DWG
   `PolyfaceFace` subtype to DXF's `AcDbFaceRecord`, group-70 bit 128, and
   signed one-based groups 71–74. DWG type-14 face records carry indices but
   no flags field; do not make DXF face emission depend on a DWG flags value.
   Preserve the existing DXF-input path for explicit group-70 bit 128. Add a
   fast ASCII/binary regression whose source vertex has the typed subtype and
   zero flags, then check the emitted DXF marker/indices and parsed result.
   Separately smoke-test one locally generated DWG→DXF control; treat it only
   as conversion-path evidence, never independent writer qualification.

Positive gate: round trips preserve point order, flags, topology, frame, and
finite values. Negative gate: invalid vertex indices, impossible counts,
non-finite coordinates, half-present points, or overflowed count arithmetic
are rejected before callback publication.

### S6 — Surface/NURBS version qualification and spline layout verification

State: the bounded DXF field and negative-test slice is implemented in S6.1;
DWG spline edits are `BLOCKED_ON_AUTHENTIC_PER_VERSION_SPLINE_WITNESS`;
modern surface DWG layout claims are `BLOCKED_ON_PRIMARY_LAYOUT_EVIDENCE`.

DXF evidence boundary: the S6.1 slice below verifies the implemented DXF
group-code mappings through libdxfrw's own ASCII and binary reader/writer. It
does not qualify surface evaluation, solid modeling, DWG layouts, or
independent semantic interoperability. Generated surface vectors were also
tried with LibreDWG 0.14, but its reader reported unsupported surface/NURBS
cases and rejected several mapped group codes; treat that as an unsuccessful
oracle attempt, not positive support evidence.

Dependencies: S0, S1 for DXF; local ODA/reference gate before any
`src/intern/dwgreader*` or DWG `parseDwg(...)` edit; feature-specific primary
layout evidence plus an authentic target witness for DWG surface qualification.

Files: `src/drw_entities.cpp` surface and spline parsers/writers;
`src/libdxfrw.cpp`; `src/intern/dwgreader.cpp` class routes; focused tests.

Steps:

1. Inventory `PLANESURFACE`, `EXTRUDEDSURFACE`, `REVOLVEDSURFACE`,
   `SWEPTSURFACE`, `LOFTEDSURFACE`, and `NURBSURFACE`, plus HELIX and 3D
   SPLINE separately. Map each DXF group code and DWG field
   to a named `DRW_*` field. Include transforms, profile/path/cross-section
   handles, flags, count arithmetic, knot/control-point/weight ordering,
   closure/periodicity where represented, modeler format, and ACIS carrier
   boundaries.
2. Keep the existing surface DWG output gate at `AC1021` or later, but do not
   treat that gate as proof of supported layout: until a surface-specific
   primary layout and independent witness are available, keep DWG surface
   read/write rows unqualified and avoid changing their DWG byte layout.
3. Reconcile the stale local `AGENTS.md` note about an R2010+ one-bit
   `splFlag1`: ODA v5.4.1 §20.4.40 specifies R2013+ `Spline flags 1` and `Knot
   parameter` as BL fields; in scenario 1, `Rational`, `Closed`, and `Periodic`
   are separate B fields. `DRW_Spline::parseDwgSplineBody()` reads the BL fields
   only when `version > AC1024` and separately reads the three scenario bits,
   matching that source. Do not change the field width absent contradictory
   primary evidence. Add/retain local bit-boundary tests for the actual branch,
   then verify with authentic target splines separately for AC1024, AC1027, and
   AC1032; do not let an AC1024 trace qualify AC1027 or the AC1032
   pass-through stub. Until each version has a witness, leave that row
   unqualified.
4. Make unsupported NURBS/surface operations explicit. Retaining control
   points/knots or ACIS is not surface evaluation or tessellation.

DXF mapping notes established by the S6.1 source/test pass (not a claim of
complete surface semantics):

- `PLANESURFACE` carries its four optional U/V direction vectors in groups
  10/20/30, 11/21/31, 12/22/32, and 13/23/33; group 170 and 290 are bounded
  to their signed-short and Boolean representations. Missing vector triplets
  remain optional; a partially present triplet is malformed.
- `EXTRUDEDSURFACE` distinguishes the common `AcDbModelerGeometry` carrier
  (group-310 entity proxy graphics) from subtype data after
  `AcDbExtrudedSurface`; subtype group 90 carries class ID and binary-data
  length, and subtype group 310 chunks are retained byte-for-byte. Vectors,
  transforms, flags, and Boolean fields use their DXF groups; no meaning is
  inferred from the retained bytes.
- `REVOLVEDSURFACE` uses subtype group 90 first for entity ID and then for
  group-310 payload size. Keep that ID distinct from the DWG-side class ID.
  Group 310 payload bytes are preserved but not decoded as modeler geometry.
- `SWEPTSURFACE` group-90 pairs represent sweep ID/size and path ID/size;
  group-310 chunks are assigned to the active payload. Legacy group 91 path-ID
  input remains accepted when the newer group-90 pair is absent. Declared
  sizes are exact-checked when present. This compatibility path is based on
  official DXF tables and runtime self-roundtrips, not an independent oracle.
- `LOFTEDSURFACE` group-290–297 values are strict Booleans; optional transform
  arrays may be absent or complete but cannot be partial. `NURBSURFACE`
  accepts only complete coordinate triplets for each optional vector and
  validates finite values. These fields do not amount to NURBS evaluation.
- DXF Boolean groups 290–299 use one-byte binary-DXF storage; 16-bit group-70
  surface flags remain signed-short fields. Spline counts/flags are constrained
  to the signed-short range; rational splines require one finite weight per
  control point. HELIX's constraint type is limited to the documented 0–2
  values. The in-tree `dx_iface` now retains HELIX callbacks on import and
  dispatches HELIX on export.

S6.1 focused negative gates: declared subtype payload length mismatch,
non-Boolean values, partial coordinate/matrix groups, out-of-range HELIX
constraint, incomplete rational weights, and invalid extrusion alignment are
rejected. All DXF vectors are generated in the test at runtime; no fixture is
added to the repository. Autodesk DXF reference tables consulted:
[EXTRUDED SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-9218F5A6-3AE4-4EA4-854E-E15E1946AE88),
[REVOLVED SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-844D8EC8-318D-4721-AFF2-82923DB10678),
[SWEPT SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-55DCEC23-9286-4A32-AAFA-E1F945B10A19),
[LOFTED SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-3D9D8A87-1E46-48AE-B482-BAD1C4D460CA),
[NURBS SURFACE](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-E1F884F8-AA90-4864-A215-3182D47A9C74), and
[HELIX](https://help.autodesk.com/view/OARX/2024/ENU/?guid=GUID-76DB3ABF-3C8C-47D1-8AFB-72942D9AE1FF).

Positive gate: generated fields read back exactly and available independent
readers agree on semantic fields/carrier placement. Negative gate:
truncated arrays, count disagreement, invalid handles, unknown subtype, and
version-incompatible output are rejected or retained as opaque with a
diagnostic.

### S7 — Independent qualification, claims, and release closeout

State: the S7.4 status matrix and S7.5 static LibreCAD callback audit are
committed. All remaining promotion stays per-family/per-version
evidence-gated; the callback audit does not qualify LibreCAD UI behavior.

Dependencies: S1-S6. An unavailable DWG witness does not block completed DXF
rows; it leaves only the corresponding DWG row unqualified.

Steps:

1. DXF oracle lane: compare field-level libdxfrw results with Autodesk DXF
   definitions and current ezdxf behavior for WCS/OCS primitives, block
   transforms, classic 3D/polyface/polygon-mesh polylines, MESH, HELIX/splines,
   surfaces, SAT, and the R2013+ ACDSDATA section path. ezdxf explicitly
   documents that arbitrary ACIS cannot be loaded and re-exported; use it for
   supported carriers/fields, not as proof of arbitrary ACIS semantics.
2. DWG oracle lane: use ODA spec and real trace evidence as normative/primary
   references; use LibreDWG source/behavior as a cross-check only when that
   entity/version is marked stable. Do not treat unstable/debugging entries
   or a self-round-trip as independent confirmation.
3. Record source version, object class identity, raw digest, typed semantic
   digest, carrier category, and diagnostic outcome separately. Fail closed
   on unknown class or a source-version mismatch.
4. Update README/support metadata with one row per entity family, format,
   version, read/write mode, and evidence grade. Distinguish `typed`,
   `opaque-preserved`, `derived-display`, `unsupported`, and `unqualified`.
5. Review LibreCAD callback handling separately from library parsing: a
   no-op default callback preserves source compatibility but does not mean the
   consumer displays or edits that 3D family.

Positive gate: every public 3D claim maps to a completed per-version row with
independent semantic evidence or is narrowed to opaque/experimental. Negative
gate: an opaque/raw-only result, self-generated vector, or advisory external
sample cannot promote a semantic read/write claim.

### S8 — Preserve 2D consumers and enable 3D-aware consumers

State: newly added consumer-enablement lane. It is independent of GUI/rendering
work and can proceed for generated DXF vectors while versioned DWG evidence is
blocked. No support claim is promoted by the adapter alone.

Dependencies: S1 public typed-field inventory; S5/S6 DXF entity paths; S7.5
LibreCAD callback-boundary audit. DWG coverage additionally depends on the
relevant S3/S4 reader/writer slice and its authentic per-version evidence.

Steps:

1. **S8.1 — Public consumer contract matrix.** For every in-scope family, map
   the callback and public `DRW_*` fields to WCS, OCS, entity-local, or opaque
   coordinates; record Z-bearing values, normals/extrusion, topology/index
   ownership, block transforms, and raw carrier fields. Mark callback defaults
   (`no-op`, delegation, or required override) and pointer lifetime/copying
   expectations. Reuse the S0 family inventory; do not duplicate parser
   qualification. Keep unverified coordinate interpretations marked unknown.
2. **S8.2 — Existing 2D source-compatibility gate (required after each API
   slice).** Compile the locked `lc3_compat_check` consumer and representative
   library/test adapters after callback or public-field changes. Preserve
   default implementations for new callbacks and test that no new pure virtual
   is required. Run the applicable legacy-mode regression before committing a
   slice that can affect callback dispatch or values. Keep LibreCAD's
   projection/metadata behavior in its adapter; do not change its sibling
   checkout or require a migration as part of this library lane. Treat this
   as a source-compatibility check only; it does not establish binary ABI or
   LibreCAD runtime/UI behavior.
3. **S8.2a — Legacy 2D read-mode regression guard.** Add a fast, runtime-
   generated DXF contrast through a test `DRW_Interface` sink: `ext == true`
   must retain the existing callback values for the covered planar/OCS case,
   while `ext == false` delivers the unprojected parsed values for the 3D
   consumer lane. Choose a small non-world extrusion/elevation vector with
   independently calculated expected coordinates; cover ASCII and binary DXF
   using the same semantic vector. Do not broaden this into a LibreCAD GUI test
   or change its checkout. Extend the guard only when a code slice changes an
   additional `applyExt` path; include DWG only with an already-authorized,
   locally generated fixture and the relevant DWG layout evidence. This
   distinguishes a preserved 2D behavior from the new 3D data-delivery path
   without asserting universal 2D/3D support.
4. **S8.3 — Headless 3D consumer probe (DXF-ready).** Extend/reuse the
   semantic-adapter/test infrastructure to copy complete typed callback data
   into a neutral scene-record model, retaining full XYZ, coordinate-frame
   tags, normals, face indices/edge flags, transforms, and opaque-carrier
   identity separately. Exercise generated ASCII and binary DXF vectors from
   S5/S6, including nonzero Z, OCS normals, nested INSERT/MINSERT transforms,
   polyface/MESH topology, spline/surface parameters, and SAT/SAB/ACDSDATA
   carrier distinctions. Assert the consumer sees native values before any
   projection; for overlapping vectors, pair this with the S8.2a legacy-mode
   assertion. Keep the probe independent of DWG so DXF work continues while a
   DWG dependency is blocked; runtime-generated fixtures only.
5. **S8.4a — Sample-backed DWG consumer comparison (AC1024 INSERT/SPLINE).**
   The locally available `tests/samples/AC1024/visualization_-_conference_room.dwg`
   completes both the libdxfrw read and LibreDWG 0.14 `dwgread -O minJSON`
   read. ODA v5.4.1 §§20.4.9/20.4.10/20.4.40 give the applicable INSERT,
   MINSERT, and SPLINE layouts. Compare records by handle with
   `tools/compare_dwg_3d_consumer_oracle.py`: six INSERTs match insertion XYZ,
   scale, rotation, and extrusion; two scenario-2 SPLINEs match degree,
   scenario, fit tolerance, tangent vectors, and all seven XYZ fit points.
   This is a narrowly scoped AC1024 read-field witness, not write or general
   DWG support. Do not copy the local DWG into the commit.
6. **S8.4a.1 — Sample-backed DWG 3DFACE comparison (AC1021).** Extend the
   optional comparator to accept the local AC1021 `tablet.dwg` sample and
   compare all 48 3DFACE records by handle against LibreDWG 0.14. Compare four
   3D corner tuples and invisible-edge flags, interpreting LibreDWG's
   `has_no_flags` representation only as the zero/default flag state. ODA
   v5.4.1 §20.4.32 documents the R2000+ presence bits, corner fields, and
   optional group-70 flags used by this R2007 path. This is a read-field
   witness for one uncommitted local sample only; it does not qualify writes,
   other AC1021 files, or other DWG versions. Never stage or commit the DWG.
7. **S8.4a.2 — Sample-backed DWG LINE comparison (AC1021).** Compare all
   3,002 standard LINE records from the same local AC1021 `tablet.dwg` sample
   by handle against LibreDWG 0.14. Check start/end XYZ, thickness, and
   extrusion; require the expected 670 records with nonzero endpoint Z so this
   is a real 3D-coordinate witness. ODA v5.4.1 §20.4.21 specifies the R2000+
   fields. This is limited to those fields in this single read-only sample;
   it does not qualify LINE writes, other AC1021 files, or other versions.
   Never stage or commit the local DWG.
8. **S8.4a.3 — Sample-backed DWG LINE comparison (AC1024).** Compare both
   LINE records in the local AC1024 `visualization_-_condominium_with_skylight.dwg`
   by handle against LibreDWG 0.14, including start/end XYZ, thickness, and
   extrusion; both records have nonzero endpoint Z. ODA v5.4.1 §20.4.21
   specifies the applicable layout. LibreDWG's minJSON for this file contains
   a bare non-standard `nan` in an unrelated surface record, so the optional
   comparator replaces only bare NaN tokens outside JSON strings with `null`;
   every compared LINE value remains subject to finite-number validation.
   This is a read-only two-record sample witness, not a general AC1024 or LINE
   support claim. Never stage or commit the local DWG.
9. **S8.4a.4 — Sample-backed DWG 3D POLYLINE compound-record comparison
   (AC1015).** Add an optional comparator profile for the pinned
   LibreDWG/libredwg `PolyLine3D.dwg` sample at commit
   `34f02f54b9aacb5708c1d3d2070efb3e4b2d8c43`. Require its exact SHA-256
   (`f51f4f65ba027bf1a001480d3c7b5bc5667960050081c67f9bb9c7bdbcfe815a`)
   before comparing the AC1015/R2000 read against LibreDWG 0.14. Match the
   parent type/subclass, 3D flag and curve type, six ordered child vertices,
   first/last/SEQEND handles, child handles and owners, vertex flags, and XYZ
   values. ODA v5.4.1 §§20.4.12 and 20.4.17 define the applicable VERTEX and
   POLYLINE layouts; the pinned source also includes an AutoCAD VLA property
   dump and paired DXF. All six sample Z values are zero, so qualify only this
   planar compound-record read subset—not nonzero-Z preservation, PFACE/MESH,
   writing, other versions, or general AC1015 support. Download/use the sample
   in a temporary path; never add it as a repository fixture.
10. **S8.4a.5 — Locally generated AC1015 3D topology control.** Keep a
   locally authored `.dwgadd` recipe under `tests/fixtures/dwg/` for one
   nonzero-Z 3D POLYLINE, one 3×2 legacy POLYLINE_MESH, and one non-planar
   PFACE with signed/invisible face indices. Generate a temporary AC1015 file
   using LibreDWG `dwgadd 0.14`; compare libdxfrw callback values, child
   ownership/order/handles, and SEQEND against LibreDWG `dwgread 0.14` and the
   recipe's expected coordinates/topology. ODA v5.4.1 §§20.4.12/.17,
   20.4.13/.34, and 20.4.14/.15/.33 anchor these layouts. LibreDWG is both
   generator and reader, so this is a cross-implementation parser control—not
   independent target-generated evidence, not an AutoCAD interoperability
   result, and not a support-claim promotion. Do not check in the generated
   DWG; the recipe is generated locally from scratch and is the reproducible
   source artifact.
11. **S8.4b — Remaining DWG consumer rows (blocked per family/version).** Add
   comparisons only where the parser layout is verified against ODA, a
   suitable authentic target sample is available, and an independent semantic
   witness returns comparable fields. The AutoCAD-authored AC1015 planar
   3D-POLYLINE sample in S8.4a.4 does not qualify nonzero-Z 3D POLYLINE,
   PFACE, or MESH; S8.4a.5 supplies only a locally generated parser control,
   not target interoperability evidence. It also does not qualify
   ARC/CIRCLE OCS, ACIS/modeler, PLANESURFACE, modern surface, or other-version
   behavior. The same sample's LibreDWG and libdxfrw PLANESURFACE fields
   disagree, and ODA v5.4.1 does not describe that named modern subtype. Never
   fabricate DWG records or infer Z semantics from a DXF analogue. To unblock
   any of these rows, obtain an authentic target-generated sample exercising
   the exact feature/version, its authoritative layout, and an independent
   field-level oracle; otherwise leave it unqualified. Modeler rows still
   depend on S3/S4 evidence. Deep research found a promising PFACE candidate:
   [CADforum's Enneper10 block](https://www.cadforum.cz/catalog/block.asp?blk=15264)
   is described as made with 3DPLOT and identified as DWG2018; the
   [3DPlot reference](https://www.cadforum.cz/en/3dplot-3d-math-surfaces-in-autocad-tip15090)
   says generated surfaces are `AcDbPolyFaceMesh`, and its Enneper definition
   gives `Z = U² - V²`. This is a candidate only: the catalog requires a free
   registered-member download, so the binary was not available for hash,
   header, entity-class, or vertex inspection. It is AC1032, while the reviewed
   ODA v5.4.1 material ends at R2013 and this repo's AC1032 reader is a
   pass-through stub. If the artifact becomes available, verify provenance,
   actual class, and non-planar vertices, then source the R2018 layout and
   resolve the AC1032 reader before using it as a witness. It does not address
   nonzero-Z 3D POLYLINE or modern `AcDbSubDMesh`; keep those and PFACE
   interoperability claims blocked until their evidence gates are met.
12. **S8.5 — Consumer-facing contract and release claims.** After S8.1-S8.4a.5,
   document how a 3D-aware client consumes typed geometry and separates opaque
   modeler payloads from decoded fields, and how a 2D client can retain its
   existing projection policy. Update `docs/3D_SUPPORT_STATUS.md` only with
   evidence-backed distinctions among library data delivery, 2D consumer
   mapping, 3D consumer field access, semantic format/version qualification,
   and actual display/edit behavior. Keep all DWG rows not explicitly narrowed
   by S8.4a, S8.4a.1-S8.4a.5, or corresponding S8.4b evidence explicitly
   unqualified; do not block the completed DXF consumer contract on
   unavailable DWG witnesses. Do not imply that this library performs scene
   rendering or parametric/NURBS/ACIS evaluation.
13. **S8.6 — Preserve both consumer lanes across implementation slices.** For
   every later change that can alter public fields, callback dispatch, or
   coordinate handling, rerun `lc3_compat_check` and the affected fast
   consumer test before committing. When one vector exercises both policies,
   feed that same vector through `ext == true` and `ext == false`: assert the
   established 2D callback values remain unchanged and the opt-in 3D callback
   retains available coordinates/topology without projection. Add a paired
   vector only for a newly affected path; do not require a LibreCAD checkout,
   UI change, or renderer. Keep source compatibility distinct from ABI and
   display/edit claims.
14. **S8.7 — Complete selected ARC/CIRCLE 3D-consumer probe fields.** Extend
   the semantic sink to retain CIRCLE center, radius, thickness, and extrusion,
   plus ARC's same fields and start/end angles in radians. Extend the
   runtime-generated AC1027 ASCII/binary recipe with a default-normal CIRCLE,
   an oblique-normal CIRCLE, and a negative-Z-normal ARC. Read every vector
   with both `ext == false` and `ext == true`: the former must retain parsed
   OCS fields; the latter must preserve the existing arbitrary-axis center
   mapping and negative-Z ARC angle mirror/swap for these exact vectors. Keep
   the already-covered LWPOLYLINE legacy and 3DFACE invariants. Run only the
   focused consumer CTest and `lc3_compat_check`; no DWG parser or public API
   change is needed. This self-generated writer/readback checks callback field
   delivery only; ARC/CIRCLE family support and third-party interoperability
   remain unqualified. Runtime files stay temporary.
15. **S8.8 — Cover selected PFACE values in the 3D-consumer probe.** Extend
   the runtime-generated AC1027 ASCII/binary vector with a PFACE POLYLINE,
   four nonzero-Z vertex records, and one typed face record whose zero source
   flags require the writer to emit the DXF face-record marker. At the
   consumer, assert the parent PFACE flag/declaration, all relevant XYZ, the
   emitted/parsed group-70 marker, and all four signed one-based face indices.
   Read the same vector through both `ext` modes to guard WCS vertex delivery.
   Keep these fields explicitly distinct from DWG child ownership/handles and
   target interoperability; the vector is generated at test runtime.
16. **S8.9 — Match FreeCAD's `dwg2dxf` converter invocation.** FreeCAD's
   `Draft/importDWG.py` resolves `dwg2dxf` and invokes exactly
   `dwg2dxf <input.dwg> -o <output.dxf>` (no `-y`, output-version flag, or
   shell quoting layer), then passes the result to FreeCAD's DXF importer.
   Add this `-o` form without removing the existing positional
   `<input> [-b] <-version> <output>` syntax. The FreeCAD form must default to
   ASCII, never prompt on stdout/stdin, and fail quickly without replacing an
   existing output unless an explicit overwrite option is supplied. If the
   caller omits the DXF version for DWG input, carry the imported DWG revision
   when the writer supports it; map the reader's AC1012/R13 value to the closest
   supported AC1014/R14 DXF, and reject unknown/unmapped versions instead of
   selecting an arbitrary one. Preserve an explicit version override. Add a
   CTest using the already tracked
   `tests/fixtures/dwg/ordinary_enc_AC1027.dwg`, invoke the exact FreeCAD argv
   with input and output paths containing spaces, verify successful ASCII output and
   `$ACADVER` preservation, and exercise no-prompt no-overwrite plus explicit
   overwrite behavior without leaving fixture/output files in source. Retain
   existing positional CLI tests as the backwards-compatibility gate; update
   `dwg2dxf` usage/man text. This qualifies invocation and converter output
   only—not FreeCAD's parser, object mapping, visual display, or general DWG
   support.
17. **S8.10 — Qualify actual FreeCAD DWG import (external/runtime gate).**
   Execute the real `Draft/importDWG.py` path with this `dwg2dxf`, then assert
   conversion success and expected imported entities/coordinates using the
   selected FreeCAD DXF importer mode (native C++ versus legacy Python). Keep
   converter correctness, DXF import mapping, and GUI display as separate
   results. The reusable opt-in macro is
   `tests/freecad_dwg2dxf_import_check.FCMacro`; it imports an existing tracked
   ordinary-encoding fixture and asserts all three LINE extents. Run it for
   AC1015, AC1018, AC1021, and AC1027 by setting
   `LIBDXFRW_FREECAD_DWG` to each fixture path and placing this build's
   `dwg2dxf` directory on `PATH`; replay the matrix with:

   ```sh
   for version in AC1015 AC1018 AC1021 AC1027; do
     PATH="/path/to/build/dwg2dxf:$PATH" \
     LIBDXFRW_FREECAD_DWG="/path/to/libdxfrw/tests/fixtures/dwg/ordinary_enc_${version}.dwg" \
     "/path/to/freecadcmd" --user-cfg "/tmp/freecad-user.cfg" \
       --log-file "/tmp/freecad.log" \
       "/path/to/libdxfrw/tests/freecad_dwg2dxf_import_check.FCMacro" || exit $?
   done
   ```

   For all four fixtures, LibreDWG 0.14's independent DXF export gives the
   endpoints `(1,2,0)-(3,4,0)`, `(5,6,0)-(7,8,0)`, and
   `(9,10,0)-(11,12,0)`. FreeCAD 1.1.3 revision `20260725` on arm64 / Qt
   6.8.3 selected its C++ DXF importer and created three `Part::Feature`
   objects with those bounds for each source revision; all four summaries
   report three LINEs and no unsupported features. This qualifies only that
   runtime, importer mode, four fixtures/revisions, converter route, and LINE
   field subset—not GUI display or general FreeCAD/DWG/3D support.

   An exploratory run through the same DWG helper on the existing untracked
   AC1021 `tests/samples/AC1021/tablet.dwg` exposes a downstream consumer
   boundary. The libdxfrw DXF and [ODA File Converter](https://www.opendesign.com/guestfiles/oda_file_Converter)
   27.1.0.0's independent
   export each contain 48 3DFACE, 81 SOLID, and 24 HATCH records. FreeCAD's
   C++ importer creates the same 4,868 imported objects for either output but
   marks 38 3DFACE, 69 SOLID, and 22 HATCH records unsupported and logs
   `ReadEntity` exceptions. The shared result is evidence against attributing
   these omissions solely to libdxfrw's conversion, not proof of equivalent
   fields or consumer usability; this user-owned sample remains uncommitted.
   The optional legacy Python importer was not tested because its
   `dxfReader` dependencies are not installed, and no addon download was
   triggered. Keep the FreeCAD qualification limited to the four ordinary
   LINE fixtures and default C++ importer until a separate feature-level
   consumer expectation is established.

   In this managed macOS sandbox,
   the original `neon` startup failure came from sandbox denial of
   `hw.optional.neon`, not an unsupported host or FreeCAD binary; the same
   version check and import pass outside the restricted sandbox. See the
   matching [Codex sandbox issue #7099](https://github.com/openai/codex/issues/7099).
   FreeCAD issue [#19247](https://github.com/FreeCAD/FreeCAD/issues/19247)
   remains an adjacent reproduction lead only: it supplies neither the
   affected DWG nor converter output, so it is not evidence of a libdxfrw
   defect.
18. **S8.11 — Unit-test the implicit output-version policy.** Move the
   source-to-output DXF revision decision into a small CLI-private pure helper
   used by `dwg2dxf`, then test every accepted revision, the AC1012/R13 to
   AC1014/R14 mapping, and fail-closed UNKNOWN/unsupported revisions. Keep
   explicit `-version` parsing covered by S8.9's integration test. This test
   validates policy branches without fabricating or relabeling a DWG sample;
   it does not replace per-version DWG reader tests.
19. **S8.12 — Verify failure-safe output publication through FreeCAD's CLI.**
   Current FreeCAD `Draft/importDWG.py` resolves `dwg2dxf` from its configured
   converter path or `PATH`, starts it with argv equivalent to
   `[dwg2dxf, input.dwg, -o, output.dxf]`, waits for completion, then checks
   whether the expected output path exists; it does not use the child exit
   status as the success test. See the current
   [FreeCAD converter route](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py).

   The DXF writer already routes `dxfRW::write()` through
   `DwgDxfOutputTransaction`: it writes an exclusive sibling temporary,
   publishes after a successful flush/commit, and aborts failed writes without
   changing an existing destination. Existing `libdxfrw_writer_primitives`
   tests cover transaction publish/rollback and target preservation. Do not
   add a second CLI transaction layer unless an end-to-end test finds a real
   gap. Close the missing consumer-contract evidence instead: extend
   `tests/run_freecad_dwg2dxf_compat_test.cmake` with a tiny malformed DWG
   created at test runtime, call the exact `input -o output` form, require a
   nonzero exit and no output path, then repeat with `-y` and a sentinel target
   to require the old target remains byte-identical. Check that the writer's
   temporary-name pattern leaves no debris. Keep the existing valid exact-argv,
   spaces, no-prompt/no-overwrite, and explicit-overwrite cases. The focused
   fast gate is:

   ```sh
   cmake --build build --target dwg2dxf libdxfrw_writer_primitives_tests
   ctest --test-dir build -R '^(dwg2dxf_(version_policy|freecad_cli_compat)|libdxfrw_writer_primitives)$' --output-on-failure
   ```

   Probe non-ASCII paths per supported OS and
   record or fix any platform-specific argv encoding limitation before
   claiming them. Keep batch and legacy positional CLI behavior unchanged.
   Use existing tracked input or runtime-generated/local-from-scratch data;
   do not add downloaded samples.

20. **S8.13 — Preserve typed records through `dwg2dxf` and scope FreeCAD
   importer claims.** `Draft.importDWG.open()` invokes the converter and passes
   the resulting DXF to `importDXF.open()`; treat those as separate boundaries.
   First test the exact `dwg2dxf <input> -o <output>` argv on already-tracked
   fixtures and inspect emitted entity records. Then independently check that
   the in-tree DXF reader recovers subtype and expected fields from the output.
   Finally run the real FreeCAD route in default C++ mode and record imported,
   unsupported, ignored, and exception outcomes. The opt-in
   `tests/freecad_dwg2dxf_feature_audit.FCMacro` captures FreeCAD revision,
   platform, converter resolution, importer settings, source/DXF hashes,
   record counts, importer statistics, and resulting object types. Keep the
   four tracked ordinary-encoding LINE fixtures as positive smoke rows.

   For `mpolygon_solid.dwg`, conversion/readback must retain one MPOLYGON with
   `solid=1` and fill ACI 256; FreeCAD 1.1.3 C++ recognizes the record but
   reports it unsupported and creates no entity object. For
   `rtext_arctext.dwg`, retain one RTEXT with payload `RTEXT-DIESEL-TEST` and
   one ARCALIGNEDTEXT with payload `ARC-TEXT-TEST` and radius 25; FreeCAD
   reports both custom records unsupported and creates no entity objects. For
   `large_radial.dwg`, retain the DIMENSION record and independently preserve
   jog `(8,2)`, center `(5,6)`, and chord `(10,0)` in the source callback; the
   FreeCAD run reports dimension type 4 unsupported (and its current settings
   have dimension import disabled), so this is not positive dimension-import
   coverage. These results are converter-pass-through evidence plus a
   downstream limitation, not a DWG support promotion or a reason to downgrade
   the DXF records to generic TEXT/HATCH entities.

   The correctness gap found was in `dwg2dxf/dx_iface`: base-class copies
   erased RTEXT/ARCALIGNEDTEXT and MPOLYGON dynamic types, and the entity writer
   sent them through generic TEXT/HATCH writers or omitted MPOLYGON. Preserve
   derived values at callback storage and dispatch to their specialized DXF
   writers. Guard the contract in fast tests: exact-argv CLI record checks for
   the existing fixtures plus DWG→DXF→DXF readback assertions for subtype and
   stable identifying fields. Do not add external fixtures; generated DXF
   outputs stay in the build/temp directory. Add further FreeCAD-positive rows
   only when source identity, independent expected values, and importer mode
   are established. For this 3D plan, prioritize the DWG→converter→FreeCAD
   nonzero-Z witness and then only 3D families the pinned importer maps, using
   the dependency-ordered S8.14-S8.15 slices below. Keep GUI display,
   editable/native solids, legacy Python import, and general DWG/DXF 3D support
   separate and unqualified. Run fast CLI and public-reader
   tests per affected slice; reserve real FreeCAD runs for the external
   integration checkpoint and do not make FreeCAD an always-on test dependency.

21. **S8.14 — Establish the minimum FreeCAD 3D end-to-end witness (nonzero-Z
   LINE).** Extend the existing opt-in FreeCAD import check so a DWG containing
   a standard `LINE` with distinct, nonzero-Z endpoints is converted by this
   build's `dwg2dxf` using exactly the argv FreeCAD uses, imported through
   `Draft.importDWG.open()`, and represented as a FreeCAD shape whose two
   endpoint coordinates match the independent recipe/source values. Assert
   finite XYZ, nonzero Z, expected edge count and bounding box; record
   unsupported/exception statistics so a partial import cannot pass by bounds
   alone. Use an existing in-scope sample if it is suitable; otherwise add
   only a locally authored, reproducible `.dwgadd` recipe and generate all DWG
   and DXF outputs in a temporary/build directory. Do not commit generated
   DWG/DXF files or stage user-owned `tests/samples/` data. Keep a fast gate
   independent of FreeCAD that checks exact `dwg2dxf input -o output` behavior,
   output `$ACADVER`, emitted LINE endpoint group codes and libdxfrw readback;
   the optional runtime gate additionally checks the resulting FreeCAD B-rep
   vertices. Pin FreeCAD release/revision, architecture, default C++ importer,
   import mode, scaling and relevant point/layout/annotation settings in the
   result. This is the first end-to-end nonzero-Z consumer witness only; it
   does not qualify other entity families or DWG versions. A generated control
   tests the route but is not target-authored interoperability evidence.

22. **S8.15 — Build a per-family FreeCAD DXF consumer matrix and expand only
   mapped 3D families.** Begin from the exact FreeCAD revision's C++ importer
   source and runtime audit, not from DXF-reader acceptance or entity names
   alone. For each candidate, record: converter source entity and emitted DXF
   type; expected WCS/OCS fields/topology from the source or independent
   witness; FreeCAD importer callback/shape construction path; settings; actual
   imported object/shape type; coordinates/topology; unsupported, ignored and
   exception counts; and the evidence ceiling. After S8.14, proceed one small
   family-scoped slice at a time, prioritizing FreeCAD-mapped WCS `POINT` and
   `LINE`, 3D POLYLINE wires, OCS `ARC`/`CIRCLE`, `SPLINE`, and INSERT/block
   transforms only where source inspection and a runtime witness confirm the
   path. Add `3DFACE`, PFACE, MESH, surfaces, HATCH, or ACIS only if their
   actual importer path creates the intended FreeCAD geometry and independent
   field/topology expectations are available. The existing AC1021 audit found
   that `3DFACE`, `SOLID`, and `HATCH` records from both libdxfrw and ODA
   File Converter outputs are reported unsupported by FreeCAD 1.1.3's C++
   importer; this is a downstream limitation to re-check against the pinned
   runtime, not a converter instruction to substitute or flatten those
   entities. Preserve the spec-correct DXF record, classify the runtime result,
   and leave the FreeCAD row unsupported until FreeCAD or an approved
   semantics-preserving route changes that outcome. Reuse existing tracked
   inputs or locally authored recipes only; keep external DWG/DXF samples out
   of commits. Keep FreeCAD out of default CI; run the small affected converter
   and readback tests on each slice, with the exact FreeCAD macro at the
   integration checkpoint. No general 3D-import, GUI display, native-solid,
   or editing claim follows from this matrix.

23. **S8.15.1 — Qualify the FreeCAD C++ 3D POLYLINE route with one isolated
   generated control.** Add a locally authored AC1015 `.dwgadd` recipe with
   three nonzero-Z WCS vertices. Keep the generator-controlled fast test
   opt-in: create the DWG in the build tree, call the exact FreeCAD argv,
   assert one `POLYLINE`, its 3D flag, three ordered `VERTEX` records and
   `SEQEND`, then read the DXF back and reassert the same coordinates. Add an
   opt-in macro that runs `Draft.importDWG.open()` with the default C++ importer
   and checks the resulting valid FreeCAD shape has exactly two connected
   edges matching each consecutive expected WCS vertex pair. Record unsupported
   entities and FreeCAD/runtime identity; neither raw DXF record counts nor
   bounds alone satisfy this gate. Generated files remain under build/temp and
   the recipe is locally authored, not target-authored interoperability
   evidence. Treat any importer warnings separately from shape correctness and
   keep MESH/PFACE and other POLYLINE subtypes on their own rows.

Positive gate: an old source consumer still compiles, and the headless 3D probe
receives all asserted native typed values/carrier identities without an
implicit projection; the S8.2a `ext == true` baseline remains intact for its
covered 2D case. Negative gate: a default no-op, 2D preview entity, or
opaque-byte digest alone cannot satisfy the 3D consumer gate or promote a
format/version support claim. Neither gate claims binary ABI compatibility or
LibreCAD display/edit support.

## Fast validation and fixture policy

Keep the normal inner loop small; reserve full matrices for integration
checkpoints and release review.

1. Per code slice: build only the affected executable/library under the
   project's existing `-Werror` policy. Use `libdxfrw_hardening_tests` for
   parser/cap/resource negatives and `libdxfrw_dwg_local_roundtrip` for
   generated DXF/DWG entity paths where relevant.
2. Use CTest names (not executable names) when selecting registered tests:
   `libdxfrw_hardening`, `libdxfrw_dwg_local_roundtrip`,
   `libdxfrw_writer_primitives`, `libdxfrw_dwg_reader_matrix`, and
   `libdxfrw_qualified_semantic_fields` when its optional target is configured.
   Run a row only when the current slice changes that contract. Avoid a full
   `ctest` run after every item.
3. Run the full test suite after the first completed modeler carrier/read/write
   integration (S2-S4, if DWG lanes are available) and again at S7 closeout;
   if DWG work is blocked, run one DXF integration checkpoint after S2/S5/S6
   and leave DWG rows unqualified. Record elapsed time and any test that is
   not fast enough for the inner loop; split long qualification into its own
   checkpoint.
4. Prefer vectors constructed at test runtime or files created locally from
   scratch. Do not commit downloaded DWG/DXF fixtures. Do not commit a new
   fixture unless it already exists in LibreCAD/libdxfrw or was generated
   locally from scratch. Preserve all pre-existing untracked user files and
   do not stage the current untracked `tests/samples/AC1021/` or
   `tests/samples/AC1024/` paths as part of this plan.
5. If an external reader or authentic DWG file is unavailable, continue
   independent DXF/static/vector tasks, record the missing witness, and keep
   affected support rows unqualified. Do not let a missing oracle halt
   independent ready slices or become a reason to guess DWG bytes.

## Execution and progress contract

Initial dependency/readiness order:

| Work | Dependencies | Initial readiness / self-unblock path |
| --- | --- | --- |
| S0 | None | Baseline synchronized. Finish the capability/evidence inventory. The local ODA PDF gates DWG reader edits only; locate/verify it while DXF inventory proceeds. |
| S1 | S0 inventory | Start after inventory; test/adapter-only and safe while DWG byte-layout edits are gated. |
| S2 | S0, S1 | Start after S1; DXF SAT/ACDSDATA and carrier contract only. Do not touch DWG parser files. |
| S3 | S0 applicable ODA chapter, per-version authentic trace, S1, S2 | Ready only for modeler entities whose layout is actually described and witnessed. MESH/surface/modern 3DLINE layout gaps stay separately blocked; continue S5/S6 DXF lanes and keep those DWG claims unqualified. |
| S4 | S3 verified payload reading, S2 | Blocked until S3 read evidence; do not build writer layout by mirroring an unverified reader. |
| S5 | S0, S1 | DXF topology/coordinate portion ready after S1; only the DXF portion may proceed while the ODA gate is unresolved. |
| S6 | S0, S1 | DXF surfaces/HELIX ready after S1. DWG spline edits require the local ODA chapter and authentic per-version trace; modern DWG surface edits additionally require a surface-specific primary layout and independent witness. |
| S7 | S1-S6 | Qualify completed rows independently. A blocked DWG row does not block completed DXF evidence or docs; it remains unqualified. |
| S8 | S1, S5-S7.5 | Consumer-contract matrix, 2D source-compatibility guard, generated-DXF `ext=true` regression, and DXF 3D-consumer probe are committed. S8.7 adds selected generated AC1027 ARC/CIRCLE callback fields under both `ext` modes; S8.8 adds selected PFACE values; neither qualifies those entity families. Narrow target-sample DWG read evidence covers AC1024 INSERT/SPLINE and LINE fields, AC1021 3DFACE/LINE fields, and the planar AC1015 3D-POLYLINE subset. A separate LibreDWG-generated AC1015 control exercises nonzero-Z 3D POLYLINE, legacy POLYLINE_MESH, and PFACE across libdxfrw/LibreDWG readers but does not qualify AutoCAD interoperability or promote support claims. S8.9 brings the helper CLI into FreeCAD's exact `input -o output` converter contract while preserving its old syntax; S8.11 directly tests its source-version mapping; S8.10 verifies FreeCAD 1.1.3's macOS arm64 C++ importer against tracked AC1015/AC1018/AC1021/AC1027 LINE fixtures and independent LibreDWG DXF exports. S8.12 covers transactional failure publication. S8.13 fixes typed-entity loss in the concrete CLI adapter, adds exact FreeCAD-argv record-preservation and DWG→DXF→DXF field regressions, and records actual importer outcomes for three tracked advanced fixtures. FreeCAD currently reports those advanced custom/dimension entities as unsupported; preserving correct DXF types is the converter's contract, not proof of import. S8.14 now verifies one locally generated nonzero-Z LINE through the full DWG→converter→FreeCAD C++ importer path, including both B-rep endpoints. S8.15 expands only to entity families the pinned FreeCAD importer demonstrably maps and keeps unsupported rows explicit. FreeCAD remains an integration-only dependency. Other FreeCAD runtime/import modes and all other DWG rows retain their own gates. Modeler rows additionally wait for S3/S4. Keep adapters outside parser semantics and do not require GUI/rendering code. |

The execution sequence is therefore readiness-first, not table-order-first:
S0 → S1 → S2 and the DXF portions of S5/S6; then S3 → S4 after DWG
spec/trace readiness; S7 and S8 proceed per completed rows, with S8's DXF
consumer probe independent of DWG. S8.14's nonzero-Z LINE integration and
S8.15.1's isolated 3D POLYLINE integration slices are committed. S8.15 remains
READY for the next family-isolated importer probe; advance one proven FreeCAD
entity mapping at a time. Continue any remaining independent DXF work while a
DWG dependency is blocked.

FreeCAD integration is an additional bounded S8 consumer lane, not a new
format-support claim. The exact converter invocation, four-revision planar
LINE smoke matrix, and failure-safe output publication are implemented. S8.14
adds one locally generated AC1015 nonzero-Z LINE control, which FreeCAD 1.1.3's
default C++ importer converts to one valid B-rep edge with endpoints
`(1,2,3)` and `(4,6,9)`. This demonstrates this generated vector through
the configured converter/importer path, not target-authored DWG interoperability
or broader family support. S8.15.1 adds an isolated AC1015 3D POLYLINE control;
the same pinned FreeCAD importer creates one valid shape with two edges and
preserves each expected vertex. It emits four `Entity has zero-length extrusion
direction` warnings for that generated file, despite reporting no unsupported
entities and passing exact edge checks; retain and investigate those warnings
before broadening the profile. S8.13 adds an
opt-in feature audit and fast converter/readback regressions for existing
tracked `mpolygon_solid.dwg`, `rtext_arctext.dwg`, and `large_radial.dwg` files.
For every end-to-end result, record converter discovery (`PATH` here), source
identity, FreeCAD release/revision and OS/architecture, selected importer,
settings, and per-entity imported/unsupported/exception outcomes. The observed
FreeCAD C++ importer reports MPOLYGON, RTEXT, ARCALIGNEDTEXT, and radial
dimension type 4 unsupported; do not rewrite or erase their DXF types to make
the downstream importer appear to accept them. A DXF entity FreeCAD ignores or
reports unsupported remains a downstream limitation; it does not excuse
incorrect converter output, and a present DXF record alone does not prove that
FreeCAD imported or displayed it. The optional legacy Python importer remains
untested because its dependencies are absent. New FreeCAD-supported matrix rows
must have independent source-field expectations and a named runtime/import
mode; do not extend the routine test dependency set.

Current implementation-item ledger (update in every corresponding slice
commit; 42/56 committed, 12 blocked, 1 verified, 0 in progress, and 1 ready):

| Item | State | Evidence / next action |
| --- | --- | --- |
| S8.12 | COMMITTED | Extended `tests/run_freecad_dwg2dxf_compat_test.cmake` with a runtime-generated malformed DWG. The exact `-o` invocation fails nonzero without publishing a final DXF; the same failure with `-y` preserves an existing sentinel, and no `.libdxfrw-*` output temp remains. Added a UTF-8 input/output path case, which passes on this macOS host; Windows is explicitly skipped because narrow `main(argc, argv)` encoding needs native qualification. Existing writer-primitives tests independently cover transactional publish/rollback and destination preservation. `cmake --build build --target dwg2dxf libdxfrw_writer_primitives_tests` passed; focused CTest `dwg2dxf_version_policy`, `dwg2dxf_freecad_cli_compat`, and `libdxfrw_writer_primitives` passed 3/3; `git diff --check` passed. No fixtures added. The converter now has tested failure-safe publication through FreeCAD's file-existence check on this host; Windows Unicode paths remain unqualified. |
| S8.13 | COMMITTED | Fixed typed DXF pass-through in `dwg2dxf/dx_iface`: preserve derived RTEXT/ARCALIGNEDTEXT/MPOLYGON objects and dispatch to their specialized writers rather than generic TEXT/HATCH or omission. `tests/run_freecad_dwg2dxf_compat_test.cmake` now invokes exact FreeCAD argv on tracked `rtext_arctext.dwg` and `mpolygon_solid.dwg` and requires RTEXT, ARCALIGNEDTEXT, and MPOLYGON records. `tests/dwg_fixture_tests.cpp` checks DWG→DXF→DXF subtype and stable payload/radius/solid/fill fields. Added opt-in `tests/freecad_dwg2dxf_feature_audit.FCMacro` to record source/output hashes, converter path, FreeCAD/importer settings, record counts, unsupported reports, and created object types. FreeCAD 1.1.3 (rev 20260725), macOS 27 arm64, default C++ importer / converter from PATH: MPOLYGON 1, RTEXT 1, ARCALIGNEDTEXT 1, and DIMENSION 1 are emitted; FreeCAD reports MPOLYGON, RTEXT, ARCALIGNEDTEXT and dimension type 4 unsupported (0 entity objects for these rows). The existing four AC1015/AC1018/AC1021/AC1027 LINE imports remain the only positive FreeCAD import subset. `cmake --build build --target dwg2dxf libdxfrw_dwg_fixture_tests` passed; focused CTest `libdxfrw_dwg_fixtures`, `dwg2dxf_freecad_cli_compat`, and `dwg2dxf_version_policy` passed 3/3; `git diff --check` passed. No fixtures were added. Optional legacy Python import, GUI/rendering, and general feature support remain unqualified; next add matrix rows only with independent expected fields and an established importer mode. |
| S8.14 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_3d_line_control.dwgadd`, optional `LIBDXFRW_ENABLE_DWGADD_FREECAD_CONTROL` CTest and `tests/freecad_dwg2dxf_3d_line_check.FCMacro`. The fast test uses LibreDWG 0.14 `dwgadd` to create an AC1015 DWG only in the build tree, invokes exact FreeCAD argv (`dwg2dxf input -o output`), verifies ASCII `$ACADVER`, exactly one LINE with endpoints `(1,2,3)`/`(4,6,9)`, and repeats through libdxfrw DXF readback. Optional real runtime passed on FreeCAD 1.1.3 revision `145529e` / macOS 27 arm64 / default C++ importer mode 2: `Draft.importDWG.open()` resolved this build's `dwg2dxf` via `PATH`, imported exactly one LINE and one valid B-rep edge, matched both endpoint XYZ tuples, and reported no unsupported features. `cmake --build build --target dwg2dxf libdxfrw_dwg_fixture_tests libdxfrw_dwg2dxf_version_tests` passed; focused CTest (`libdxfrw_dwg_fixtures`, `dwg2dxf_version_policy`, `dwg2dxf_freecad_cli_compat`, `dwg2dxf_freecad_3d_line_cli`) passed 4/4; `git diff --check` passed. DWG/DXF outputs stayed under ignored `build/` or temporary paths; the only committed sample artifact is the locally authored recipe. This is a generated route control, not AutoCAD-authored DWG interoperability or general LINE/FreeCAD 3D support. |
| S8.15 | READY | S8.14 pins the first runtime profile (FreeCAD 1.1.3 revision `145529e`, macOS 27 arm64, C++ importer mode 2). A mixed, locally generated AC1015 control containing 3D POLYLINE, legacy POLYLINE_MESH, and PFACE was also converted/imported: FreeCAD counted three POLYLINE records and created two objects, but logged repeated zero-length-extrusion warnings and an unknown entity-read exception. Because mixed input does not identify which subtype caused the exception or which objects correspond to which subtype, it qualifies none of those families. S8.15.1 now independently qualifies one generated 3D POLYLINE: the exact converter path plus DXF readback preserved 3D flag/three vertices/SEQEND, and FreeCAD produced the expected two-edge WCS wire; four zero-length-extrusion warnings remain noted. Next isolate legacy POLYLINE_MESH and PFACE independently and determine which source record emits the unknown exception; retain separate support gates. Keep `3DFACE`/`SOLID`/`HATCH`, MPOLYGON, RTEXT, ARCALIGNEDTEXT, dimensions, ACIS/modeler, and any other rejected type faithful and explicitly downstream-unsupported unless the pinned runtime proves acceptance or a semantics-preserving implementation is separately designed. FreeCAD is not added to default CI; no downloaded/generated DWG/DXF file is committed. |
| S8.15.1 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_3d_polyline_freecad_control.dwgadd`, optional `dwg2dxf_freecad_3d_polyline_cli` CTest, and `tests/freecad_dwg2dxf_3d_polyline_check.FCMacro`. The CTest generates an AC1015 DWG in the build tree, invokes exact FreeCAD argv, verifies one POLYLINE with 3D flag, three ordered coordinate VERTEX records and SEQEND, then repeats those checks after libdxfrw DXF readback; it passes 1/1. FreeCAD 1.1.3 revision `145529e` / macOS 27 arm64 / C++ importer mode 2 resolved this build's converter via PATH and created one valid shape with the two expected edges `(0,0,1)-(2,3,4)` and `(2,3,4)-(5,1,-2)`, with no unsupported entities. Four `Entity has zero-length extrusion direction` warnings were printed; exact geometry passed but the warning cause is still open. The mixed topology experiment separately logged an unknown entity-read exception and cannot identify a subtype. No generated DWG/DXF or foreign sample was committed; only the local-from-scratch recipe is tracked. This is a generated control and one pinned FreeCAD profile, not AutoCAD-authored DWG interoperability or all classic POLYLINE subtype support. |
| S0.1 | COMMITTED | Rebased onto `origin/master`. The last ancestry check before the S8.11 slice (HEAD `aa5a8fb`) found `origin/master` to be an ancestor of `HEAD` (zero behind, 44 local commits ahead); subsequent plan/code slices are local branch commits. Existing user-owned untracked paths remain untouched. |
| S0.2 | COMMITTED | Resolved the authoritative local ODA v5.4.1 PDF at `/Users/dli/doc/dwg/OpenDesign_Specification_for_.dwg_files (1).pdf`; title/version/page count (279) match the official download. Read §§20.4.40 SPLINE and 20.4.41 REGION/3DSOLID/BODY; continue reading each relevant section immediately before any DWG parser change. Authority lookup is unblocked; this alone does not qualify unlisted layouts. |
| S0.3 | COMMITTED | Recorded read/write versions and reader/writer lineage, source-visible routes for each 3D family, read-only legacy AC1009 behavior, the missing modeler DWG encoder, and unsupported/unqualified claim ceilings. Unknown class/layout/version identities remain explicitly unknown; this inventory is not interoperability qualification. |
| S0.4 | COMMITTED | AC1024 `visualization_-_conference_room.dwg` debug trace records class/type 524=`AcDbPlaneSurface`, DXF record `PLANESURFACE`, entity flag 1, two instances; successful conversion emits two `PLANESURFACE` records. This is class identity/dispatch evidence for this file/version only, not byte-layout or semantic qualification. No 3DLINE identity found. Candidate AC1018 `Extruder2.dwg` stops at Tables error 9 and AC1021 `dwgreader21_230.dwg` is too small (header error 5); neither supplied target-type evidence. |
| S0.5 | COMMITTED | Added path-specific ezdxf/LibreDWG compatibility evidence. Prior S5.1 LibreDWG 0.14 runs accept the exact generated 3DFACE/polyline/MESH DXF vectors. In this pass LibreDWG 0.14 omitted checked 3D families while exporting local AC1024/AC1027 target DWGs and rejected the generated surface/NURBS cases; these are not positives. S8.4a later compared a direct LibreDWG JSON read with libdxfrw on the same local AC1024 DWG and matched only six INSERT placement records and two scenario-2 SPLINE fit records; this is distinct from the earlier DXF-export coverage check and does not qualify other families. ezdxf 1.4.4 docs establish field/API scope and ACIS limitations, but no executable ezdxf run was possible because the package is not installed. LibreDWG 0.11 stability labels remain historical only. |
| S0.6 | COMMITTED | Searchable local v5.4.1 confirms §§20.4.40 SPLINE, 20.4.41 ACIS modeler, and §24 DataStorage. No named `AcDbSubDMesh`, modern `AcDb*Surface`, or `3DLINE` layout is present; those DWG rows stay unqualified. The spec does not fully decrypt ACIS and does not qualify pre-R13 forms. |
| S0.7 | COMMITTED | ODA v5.4.1 §§2.13, 20.4.44, and 20.4.45 plus LibreDWG 0.14 JSON for the untracked AC1024 conference-room DWG establish a valid named DICTIONARY entry with a null child reference. DWG parsing now accepts only a zero-reference/zero-counter handle with code 0 or the expected item-handle code; DICTIONARYWDFLT still requires its separate default handle. Runtime-generated regular/DWFDT dictionary vectors preserve null members; the local round-trip and hardening tests pass. DXF group-350 validation remains strict. The full 72-target build, `lc3_compat_check`, and 3D consumer probe pass. The spline integration checkpoint now reaches DXF output but is blocked on three unqualified E3DSOLID DWG-frame payloads in the sample's `Fluorescent Fixture` block; this does not qualify spline or modeler conversion. The local DWG was not staged. |
| S1.1 | COMMITTED | Field-level serializers now cover 3DFACE, 3DLINE, POLYLINE/VERTEX, MESH, HELIX/SPLINE, modeler geometry, all surface subtypes, INSERT placement, and nested ATTRIB fields. Loft reference values are typed; binary values remain digest carriers. |
| S1.2 | COMMITTED | Runtime self-tests verify mesh-coordinate mutation, polyface index serialization, INSERT/ATTRIB placement, loft-reference typed/binary separation, and modeler frame-body labeling/digest. Manual C++17 `-Wall -Wextra -Werror` adapter build and `--self-test` pass; `ctest -R '^libdxfrw_dwg_local_roundtrip$'` passes 1/1. Existing generated malformed DXF modeler checks remain in the fast round-trip test. No downloaded fixtures added. |
| S2.1 | COMMITTED | DXF modeler reads now retain ordered group-1/3 text and group-310 binary chunk metadata as bounded views into `m_rawBytes`. The semantic adapter reports chunk bounds, keeps surface bytes explicitly unclassified, and gives DWG frame/DataStorage payloads distinct digests; `ACDSDATA` stays an independent raw-section carrier with no invented entity link. Strict C++17 adapter build/self-test and focused round-trip CTest pass for text, binary, and mixed chunk sequences. |
| S2.2 | COMMITTED | Modeler and surface writers emit SAT text groups 1/3 only through AC1024, reject binary/mixed/unqualified DWG carriers, and reject AC1027+ inline payloads until ACDSDATA association is known. Runtime tests pass for AC1015/1018/1021/1024, ASCII/binary DXF file encodings, surface SAT, mixed-input rejection, and unsupported SAB/frame payloads. LibreDWG 0.14 `dxf2dwg --as r2000` independently read all four generated version vectors and wrote DWG output (non-fatal unknown `HEADER.DIMLDRBLK` warnings only). Generic ACDSDATA capture/replay remains a separate opaque-section path; no entity association/support claim is added. |
| S5.1 | COMMITTED | Generated ASCII/binary DXF vectors verify 3DFACE WCS corners/invisible-edge flags, SOLID and TRACE numbered corner fields, 3D POLYLINE WCS vertices, polyface counts/subclass typing/signed invisible-edge indices, LWPOLYLINE OCS elevation/normal/local vertices, and MESH vertices/faces/edges/creases. Fast in-memory ASCII inputs verify omitted corner 4 duplicates corner 3 for 3DFACE and SOLID, reject half-present fourth corners for 3DFACE/SOLID, and check TRACE OCS values plus the negative-normal `ext=true` projection. Fixed in-tree `dx_iface` 3DFACE/MESH/TRACE output routes and MESH import callback; corrected polyface groups 71/72, subclass selection, and group-91 omission; removed the invalid `AcDbSequenceEnd` marker rejected by LibreDWG. The focused round-trip and hardening CTests pass, the 4-test fast regression slice passes 4/4, and `lc3_compat_check` builds. LibreDWG 0.14 accepts the generated ASCII/binary DXF through its R2000 converter, but the DWG→DXF check drops nonzero SOLID/TRACE corner Z values and therefore does not independently qualify those semantics; no support claim is based on that lossy result. Vectors are generated at runtime and not committed. |
| S5.2 | COMMITTED | Generated ASCII/binary DXF vectors exercise nested INSERTs with nonzero block base points, attached ATTRIB, oblique OCS, 90-degree rotation, nonuniform/mirrored scales, and MINSERT arrays. An independent arbitrary-axis/matrix oracle checks a nested world point and an array-cell offset; malformed non-finite insertion points are rejected. Writer array counts now stop at the signed 16-bit group-code limit accepted by the reader. Focused CTest passes 1/1, and LibreDWG 0.14 independently converts the exact generated ASCII/binary files to R2000 DWG (non-fatal unknown `HEADER.DIMLDRBLK` warnings only). This validates DXF acceptance, not the transform oracle; vectors are runtime-generated and not committed. |
| S5.3 | COMMITTED | Closed the 3DFACE group-70 writer truncation gap: DXF defines only the four edge bits (`1`, `2`, `4`, `8`), but serialization used a signed 16-bit field without validating the `int` source. The writer now fails closed before record emission for values below zero or above `0x0f`. Runtime-generated ASCII and binary exports reject `-1`, reserved bit `16`, and `65536` (which would otherwise narrow to zero); tests verify no output file is published. DXF parsing remains permissive and keeps the parsed integer in the typed callback for forward compatibility. The official [Autodesk 3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm) anchors the four defined flags. Focused round-trip CTest passes; no DWG claim or fixture added. |
| S5.4 | COMMITTED | Autodesk defines DXF ELLIPSE center and major-axis vector in WCS, but `processEllipse()` previously called the OCS-to-WCS helper whenever `ext=true`, rotating already-world coordinates and mirroring parameter ranges for negative Z normals. The DXF path now preserves the WCS tuples and parameters in both modes; the DWG `emitWithExtrusion()` path is deliberately unchanged and remains version-qualified. Runtime in-memory DXF test checks WCS center/axis, normal, ratio, and partial-ellipse parameters with `ext=false` and `ext=true`; the focused hardening CTest and 4-test fast regression slice pass, and `lc3_compat_check` builds. Updated the 3D consumer contract and status table without promoting family-level support. No API, DWG parser, or fixture added. |
| S5.5 | COMMITTED | Fixed DWG→DXF PFACE face emission: DWG type-14 face records encode signed indices but no flags field, so `writePolyline()` now recognizes the typed `PolyfaceFace` subtype and synthesizes DXF group-70 bit 128, `AcDbFaceRecord`, and groups 71–74. Existing DXF-input vertices with explicit bit 128 retain their route. The ASCII/binary round-trip regression starts with subtype set and flags zero; both variants reparse with flags 128 and preserved signed indices. Build succeeded; `libdxfrw_dwg_local_roundtrip` passes 1/1. The regenerated local AC1015 control matches libdxfrw, LibreDWG 0.14, and its recipe using valid one-based indices across nonzero-Z 3D POLYLINE, 3×2 legacy MESH, and five-vertex/three-face PFACE. DWG→DXF inspection shows three `AcDbFaceRecord`s with group 70=128 and indices `(1,2,3,4)`, `(2,3,-4,5)`, and `(3,-4,5,0)`. All four pre-existing optional DWG comparator profiles also pass; `git diff --check` passes. This closes one conversion-path gap only; no general PFACE/DWG writer or target-interoperability claim is promoted. |
| S6.1 | COMMITTED | ASCII and binary runtime round-trips cover PLANESURFACE, EXTRUDED, REVOLVED, SWEPT, LOFTED, NURBSURFACE, SPLINE, and HELIX fields. Added bounded subtype group-90 sizes/group-310 byte retention; corrected SWEPT ID/size ordering and legacy group-91 acceptance; corrected one-byte binary-DXF Boolean encoding; tightened field/count/transform/constraint validation; made `dx_iface` preserve HELIX callbacks. Focused CTest `libdxfrw_dwg_local_roundtrip` passes 1/1, including malformed lengths/booleans/partial vectors and invalid writer fields. Runtime vectors are not committed. LibreDWG 0.14 rejected/does not handle the generated surface/NURBS cases, so it provides no independent semantic qualification. ODA v5.4.1 §20.4.40 says `splFlag1` is BL for R2013+; current code matches; no width fix is justified. Authentic per-version spline qualification and all DWG surface layouts remain outstanding. The integration checkpoint also found and corrected the REVOLVEDSURFACE copy/assignment hardening vector to enter `AcDbRevolvedSurface` before testing group-90 subtype-ID state; the corrected focused hardening CTest passes. |
| S3.1 | COMMITTED | Validated ODA v5.4.1 §20.4.41's non-empty modeler version range (1 or 2); empty ACIS bodies retain the absent-version/default-zero case. Runtime-generated AC1018 frames cover empty, 1, 2, 0, and 3; build and focused round-trip CTest pass. A local AC1024 conference-room debug conversion reached modeler parsers and retained 3DSOLID history handles, but the overall CLI failed on an OBJECTS-pass type-42 frame, so it is not an end-to-end positive. The version-2 byte is opaque filler; no payload extraction or semantic ACIS claim follows. |
| S3.2.1 | COMMITTED | Implemented the AC1024/R2010 non-empty version-2 inline SAB carrier slice in commit `c7c8eea`. The parser requires the exact `ACIS BinaryFile` signature and a unique tagged ACIS end marker bounded by the entity data body; publishes the extracted bytes and source bit range separately from the whole DWG frame; and leaves missing/duplicate-marker cases opaque. A local-from-scratch AC1024 frame covers exact extraction, tail exclusion, marker absence/ambiguity, no DWG-frame decoder fallback, and no unqualified DXF SAB write. A fresh read-only comparison against LibreDWG 0.14 now matches every payload by handle, length, and SHA-256 across four untracked local AC1024/R2010 samples: `visualization_-_aerial.dwg` (5; 29,268 carrier bytes), `visualization_-_conference_room.dwg` (33; 700,746 bytes), `visualization_-_condominium_with_skylight.dwg` (76; 1,694,840 bytes), and `visualization_-_sun_and_sky_demo.dwg` (15; 188,240 bytes); 129 3DSOLID carriers total (2,613,094 bytes including the SAB signature). LibreDWG reports the `ACIS BinaryFile` signature separately, which is rejoined for the digest comparison. `libdxfrw_dwg_local_roundtrip`, `libdxfrw_graph_preservation`, `libdxfrw_hardening`, and `libdxfrw_3d_consumer_probe` pass; `lc3_compat_check` builds. This is exact opaque-carrier evidence only, not semantic solid support, a DWG writer, a claim for other entity types/versions, or AC1024 SAT/alternate-variant coverage. No sample fixture was staged or committed. |
| S3.2.2 | COMMITTED | AC1027/R2013 `has_ds_data` modeler entities no longer fail solely because the entity-local BS is outside 1/2; effective version 2 is set only after exactly one handle-linked record starts with the exact ODA-documented `ACIS BinaryFile` SAB prefix. Runtime-generated AC1027 frame with raw value 168 passes the typed parser regression. `libdxfrw_hardening` exercises unique selection, exact numeric/key identity, disagreement rejection, duplicate-section ambiguity, wrong-version/orphan accounting, malformed-signature non-normalization, alternate `ASM BinaryFile4` retention without ACIS normalization, entity-handle fallback, idempotent replay, and two records linked while entity traversal order is reversed; these use in-memory records only. The local untracked `Cover.dwg` emits one `MODELER_GEOMETRY` callback for handle `0x6f`, effective version 2, non-empty modeler state, a linked 22,983-byte record, and handle key `6F`; its carrier SHA-256 (`e0a5e069175edafd980c942bb5766091705534b1e6e3cfe43f5fc982e17b9eda`) matches LibreDWG 0.14's entity `acis_data` after rejoining the signature. Treat that as one-sample payload corroboration, not an independent/general association oracle: LibreDWG's [NEWS](https://github.com/LibreDWG/libredwg/blob/master/NEWS) records incomplete, brute-force AcDs extraction in v0.11, while open [issue #1411](https://github.com/LibreDWG/libredwg/issues/1411) reports missing AC1027+ AcDs extraction with LibreDWG 0.14.8593, including an AC1032 case. An open-source [AcDs round-trip note](https://github.com/hakanaktt/acadrust/blob/main/tests/roundtrip.rs#L3785-L3800) identifies positional record/entity mispairing as a failure mode; we use it only to motivate an order-reversed local vector, not as format evidence. Alternate ASM-prefixed records remain opaque until primary layout/sample evidence is available. Focused round-trip, graph, hardening, and consumer tests pass; `lc3_compat_check` builds. The DWG sample remains untracked; no fixture added. Opaque byte identity only, not geometry semantics, other AC1027 records/variants, AC1032, or writing. |
| S3.2–S3.6 (remaining versions and paths) | BLOCKED_PER_VERSION | R13/R14/R2000 version-1 SAT block decoding; AC1018/AC1021/AC1027/AC1032 inline variants; AC1027+ external DataStorage association beyond the single S3.2.2 record (including missing/conflicting/orphan cases and other entities); cross-version handle/frame accounting; and modeler DWG writing remain unimplemented or unqualified. Keep the three `E3DSOLID` spline-checkpoint objects distinct from spline correctness; do not relabel frame bytes as SAT or weaken the writer gate. R1.4/R11 remains blocked on era-appropriate reference/sample. Continue with any available per-version ODA/trace/independent-witness lane; do not infer a neighboring version's layout. |
| S4.1–S4.4 | BLOCKED_ON_S3 | Opaque DWG modeler payload writing follows only verified read layouts. |
| S7.1–S7.3 | BLOCKED_ON_INDEPENDENT_WITNESS | Target-sample comparisons cover AC1024 INSERT placement and scenario-2 SPLINE fit fields, AC1021 3DFACE/LINE fields, and only the planar AC1015 3D-POLYLINE subset (S8.4a-S8.4a.4). S8.4a.5 additionally checks nonzero-Z 3D POLYLINE, legacy MESH, and PFACE using a locally generated LibreDWG control; since LibreDWG both generates and reads it, this is not an independent target witness. The broad required family/version/direction matrix remains blocked; all corresponding support claims stay unqualified. |
| S7.4 | COMMITTED | Added docs/3D_SUPPORT_STATUS.md, linked from README, with family-specific DXF test versions/encodings, explicit DWG reader/writer version sets, direction-specific status, evidence grade, and unqualified/unsupported boundaries. Does not modify frozen metadata/qualified-format-claims-v1.json or metadata/qualified-format-status-v1.json, and promotes no semantic claim. |
| S7.5 | COMMITTED | Read-only audit of `../LibreCAD/librecad/src/lib/filters/rs_filterdxfrw.cpp` at LibreCAD HEAD `c67c02a01`: `add3dFace` and `addMesh` project XY into 2D polylines; 3DFACE preserves 3D corners/edge flags in a sidecar, while MESH renders base-cage faces without a native editable 3D mesh representation. Polygon-mesh polylines record counts/flags in advanced metadata, attach source 3D vertices to a fallback XData anchor, and render XY row/column polylines; smooth polygon meshes are not rendered. `addTrace`/`addSolid` produce 2D solids and retain native TRACE/SOLID corners/thickness in sidecars only for supported axial extrusion; non-axial extrusion is skipped. `addHelix` delegates to spline approximation; LibreCAD's callback documents axis/turn metadata as not represented in its entity model and dropped on import. `addSurface` and `addModelerGeometry` retain advanced metadata and render decoded SAB wireframe edges when available; neither creates an editable parametric/native 3D surface or solid. Generic spline handling creates LibreCAD 2D spline/conic entities and may approximate higher degrees. Base `DRW_Interface` defaults for `addMesh`, `addHelix`, `addSurface`, and `addModelerGeometry` are no-ops; only source-compatible delivery is guaranteed to other adapters. This is source-level callback evidence only: no LibreCAD build/UI interaction or 3D editing behavior was tested, and no public semantic 3D claim is promoted. |
| S8.1 | COMMITTED | Added `docs/3D_CONSUMER_CONTRACT.md` mapping each in-scope family to public callbacks/fields, WCS/OCS/subtype/opaque handling, topology/transforms, default callbacks, and conservative copy/lifetime guidance. The source audit found `ext=false` is the required path for 3D adapters; `ext=true` mutates only selected entities under the legacy 2D extrusion option. Autodesk OCS/INSERT/SPLINE/3DFACE/POINT/LINE references anchor DXF coordinate claims; DWG coordinate semantics remain per-version unqualified. No API or format-support claim changed. |
| S8.2 | COMMITTED | `cmake --build build --target lc3_compat_check --parallel 2` passed (exit 0); its static assertion verifies an older 2D-style `DRW_Interface` implementation remains concrete without overriding later optional callbacks. This is compile-time source-compatibility evidence, not a LibreCAD UI/runtime test or binary-ABI guarantee. No sibling checkout changes. |
| S8.2a | COMMITTED | Extended the runtime-generated AC1027 ASCII/binary DXF consumer probe to read the same LWPOLYLINE with both `ext=false` and `ext=true`. For normal `(0,1,0)`, elevation `5`, and local points `(2,3)` / `(4,5)`, the unprojected callback preserves OCS/elevation fields, while the established extrusion path emits callback XY `(-2,5)` / `(-4,5)`; 3DFACE WCS corners remain unchanged. This is a focused regression for one legacy mode, not a claim of full LibreCAD runtime compatibility. No DWG parser or LibreCAD source changed; no fixtures were committed. |
| S8.3 | COMMITTED | Added the standalone-only `libdxfrw_3d_consumer_probe` CTest, using `SemanticSink` as a headless consumer. It writes runtime-generated AC1027 ASCII and binary DXF, reads with `ext=false`, and checks callback delivery of 3DFACE XYZ/edge flags, 3D POLYLINE vertex Z, MESH XYZ/face-edge topology/creases, INSERT/MINSERT placement/scales/grid/OCS normal, SPLINE knots/control XYZ, LWPOLYLINE elevation/local XY/extrusion/bulge, and LOFTED surface typed fields plus a separately identified group-310 carrier. Added the missing LWPOLYLINE typed-field serialization to the semantic adapter. CTest probe passes 1/1; `libdxfrw_dwg_local_roundtrip` passes 1/1; `lc3_compat_check` builds. S8.2a additionally checks the same LWPOLYLINE legacy `ext=true` result in both DXF encodings. This is generated-DXF callback-delivery evidence only: it does not qualify third-party interoperability, DWG versions, surface evaluation, or a renderer. No fixture files are committed. S8.4a supplies a separate narrow DWG read comparison; all other DWG consumer rows remain unqualified. |
| S8.4a | COMMITTED | Added `tools/compare_dwg_3d_consumer_oracle.py`, an optional read-only comparator keyed by entity handle. Against the locally available AC1024 conference-room sample, libdxfrw's semantic adapter and LibreDWG `dwgread 0.14` match all six INSERTs for insertion XYZ, scale, rotation, and extrusion, plus both scenario-2 SPLINEs for degree, scenario, fit tolerance, start/end tangent, and all seven XYZ fit points. The libdxfrw read and `dwgread -O minJSON` both complete successfully. ODA v5.4.1 §§20.4.9, 20.4.10, and 20.4.40 anchor the relevant layouts. This narrows only experimental AC1024 read-field evidence for those fields; it adds no write, other-version, modeler, or general 3D claim. PLANESURFACE values disagree between the readers (libdxfrw emits zero typed fields while LibreDWG reports nonzero modeler fields) and have no named layout in the reviewed ODA text, so that family remains unqualified. The local DWG is not staged or committed. |
| S8.4a.1 | COMMITTED | Extended `tools/compare_dwg_3d_consumer_oracle.py` to accept AC1021 samples and match all 48 3DFACE handles from the local `tablet.dwg` against LibreDWG 0.14, including four 3D corners and invisible-edge flags. `has_no_flags=1` is normalized only to the default zero flag state. ODA v5.4.1 §20.4.32 describes the R2000+ layout. The AC1021 comparison and the existing AC1024 INSERT/SPLINE comparison both pass; Python syntax and `git diff --check` pass. Added exact read-only evidence to the consumer contract and status matrix. The untracked DWG was not staged. This narrows one-sample reads only and qualifies no write or general DWG support. |
| S8.4a.2 | COMMITTED | Extended the optional comparator and support docs to cover AC1021 LINE fields. All 3,002 handles match LibreDWG 0.14 for start/end XYZ, thickness, and extrusion; 670 records have nonzero endpoint Z. ODA v5.4.1 §20.4.21 defines the R2000+ layout. AC1021 (LINE and 3DFACE) and AC1024 (INSERT and SPLINE) comparator cases pass; Python syntax and `git diff --check` pass. The local DWG remains unstaged. This is a one-sample read subset only, not write or general version support. |
| S8.4a.3 | COMMITTED | Added an exact AC1024 condominium-sample profile to the optional comparator. Both LINE handles match LibreDWG 0.14 for start/end XYZ, thickness, and extrusion; both have nonzero endpoint Z. ODA v5.4.1 §20.4.21 defines the layout. The external minJSON has a bare `nan` in an unrelated surface record; the comparator normalizes only bare NaN tokens outside JSON strings and rejects non-finite values in all compared LINE fields. All three supported local sample profiles pass, as do the sanitizer assertion, Python syntax, and `git diff --check`. Documented the precise caveat and sample-only boundary; the DWG remains unstaged. No write or general AC1024 support claim. |
| S8.4a.4 | COMMITTED | Added an exact AC1015/R2000 profile for the pinned LibreDWG `PolyLine3D.dwg` blob; the comparator verifies SHA-256 before joining the libdxfrw adapter to LibreDWG 0.14. The parent 3D-POLYLINE and six ordered VERTEX children match for subtype/flags, curve type, handles/ownership, vertex flags, XYZ, and SEQEND. ODA v5.4.1 §§20.4.12/.17 supply the layout authority; the pinned source has an AutoCAD VLA property dump and paired DXF. All Z values are zero, so this qualifies only the planar compound-record read subset. The external sample stays in `/private/tmp`; no fixture, writer, nonzero-Z, PFACE/MESH, or general AC1015 claim is added. Existing supported AC1021/AC1024 oracle profiles and the new profile pass; Python syntax and `git diff --check` pass. |
| S8.4a.5 | COMMITTED | Added a locally authored `.dwgadd` recipe and optional AC1015 comparator profile. LibreDWG `dwgadd 0.14` generates one nonzero-Z 3D POLYLINE, a 3×2 legacy POLYLINE_MESH, and a non-planar PFACE with five XYZ vertices and three faces, including signed one-based face indices. The libdxfrw adapter and LibreDWG `dwgread 0.14` match parent classes/flags, child order/handles/owners, SEQEND, XYZ, mesh dimensions/density, and PFACE face indices against the recipe. ODA v5.4.1 §§20.4.12/.17, .13/.34, and .14/.15/.33 anchor the layouts. LibreDWG is both writer and reader, so this is a generated cross-reader parser control only; it does not remove target-sample gates or promote support claims. Temporary DWG not committed. The profile and Python CLI/syntax check pass. |
| S8.4b | BLOCKED_PER_FAMILY_VERSION | Broaden DWG consumer qualification only when the exact reader layout, authentic target sample, and independent semantic oracle are all available. S8.4a.5 now supplies a local parser control for nonzero-Z 3D POLYLINE, legacy MESH, and PFACE, but LibreDWG both generated and read that file, so target interoperability remains unqualified. The AutoCAD-authored S8.4a.4 sample is planar and does not verify those cases. The local AC1021 polygon-mesh sample has two meshes and 20 vertices, all with Z=0; it is not a non-planar/topology witness. The authoritative public ODA v5.4.1 specification covers R13–R2013 and its searchable text has no named `AcDbSubDMesh` or modern `AcDb*Surface` layout; the LibreDWG-maintained 5.4.2 diff is project-specific, not normative authority. ARC/CIRCLE OCS, modeler, modern surface, and other-version rows remain gated; modern surface fields currently disagree and remain unqualified. Next action: obtain an authentic target-generated sample exercising the exact feature/version, its authoritative layout source, and independent field-level comparison. Do not fabricate DWGs, infer unsupported fields from neighboring families, or promote support from flat/absent corpus cases. |
| S8.5 | COMMITTED | Updated README, `docs/3D_CONSUMER_CONTRACT.md`, and `docs/3D_SUPPORT_STATUS.md` to separate the existing 2D source-compatibility lane, 3D typed-data callback access, format/version semantic qualification, and consumer display/edit behavior. Documented the exact AC1027 generated ASCII/binary consumer-probe families and its self-generated evidence ceiling; S8.4a-S8.4a.5 separately document the narrow target-sample comparisons and local generated topology control with distinct evidence ceilings. Other DWG rows remain unqualified. No general 3D, renderer, evaluator, editing, or binary-ABI claim is added. |
| S8.6 | COMMITTED | Made additive 3D support with preserved 2D behavior an explicit cross-slice acceptance rule. `lc3_compat_check` and `libdxfrw_3d_consumer_probe` build/pass; the generated ASCII/binary probe compares the same LWPOLYLINE under `ext == true` and `ext == false`, and S5.4 adds a DXF ELLIPSE WCS-invariance check in both modes. Future affected paths must repeat the applicable fast gate. This is source/callback evidence only; no LibreCAD code/UI, ABI, or general semantic format claim is added. |
| S8.7 | COMMITTED | The semantic sink now records ARC center/radius/thickness/extrusion/start/end radians and CIRCLE center/radius/thickness/extrusion. Runtime-generated AC1027 ASCII and binary DXF include a default-normal CIRCLE, oblique-normal CIRCLE, and negative-Z ARC. The probe verifies native OCS fields with `ext == false` and the exact established `ext == true` oblique center and negative-Z ARC angle mirror/swap values; the existing LWPOLYLINE/3DFACE invariants also remain passing. Focused consumer CTest passes 1/1 and `lc3_compat_check` builds. This is writer-self-generated callback-field evidence, not independent interoperability or family qualification; no source API/DWG parser changes or fixture files. |
| S8.8 | COMMITTED | Extended the runtime-generated AC1027 ASCII/binary consumer probe with a PFACE POLYLINE containing four nonzero-Z vertices and a typed face record whose zero source flags cause the writer to emit DXF group-70 bit 128. The sink verifies PFACE declaration/count fields, first/last vertex XYZ, face marker, and all four signed one-based face indices through both `ext == false` and `ext == true`. Focused consumer CTest passes 1/1; `git diff --check` passes. This is generated writer/readback field evidence only, not independent PFACE topology/interoperability or DWG child ownership evidence; no fixtures committed. |
| S8.9 | COMMITTED | Added FreeCAD's exact `dwg2dxf <input> -o <output>` invocation while retaining the old positional form. The converter captures reader version, defaults to the source revision for supported versions (AC1012/R13 maps to supported AC1014/R14), emits ASCII by default, refuses existing outputs without prompting, and accepts explicit `-y`; explicit output-version overrides remain available. The new fast CTest uses the repository-tracked `tests/fixtures/dwg/ordinary_enc_AC1027.dwg` copied only into the build tree so both input and output paths contain spaces; it checks AC1027 `$ACADVER` preservation, legacy `-v2010` output AC1024, no-prompt/no-overwrite sentinel preservation within a 5-second timeout, and explicit overwrite. `cmake --build build --target dwg2dxf lc3_compat_check` succeeds; `ctest --test-dir build -R '^dwg2dxf_freecad_cli_compat$' --output-on-failure` passes 1/1; `git diff --check` passes. No DWG/DXF fixture was added. S8.11 supplies unit coverage of the AC1012→AC1014 mapping; no local authentic AC1012 DWG is available, so that reader/version path remains under its existing witness gate. This verifies converter CLI/output only, not FreeCAD import or display. |
| S8.10 | VERIFIED | Resolved the apparent runtime blocker: macOS sandboxing hid `hw.optional.neon`, causing Qt's false incompatibility abort; outside the restricted sandbox FreeCAD 1.1.3 revision `20260725` / arm64 / Qt 6.8.3 starts. `tests/freecad_dwg2dxf_import_check.FCMacro` runs the actual `Draft.importDWG.open()` route on each already tracked ordinary-encoding fixture (AC1015/AC1018/AC1021/AC1027) with this build's `dwg2dxf` on `PATH`. For all four, the C++ DXF importer reports 3 LINEs, creates 3 `Part::Feature`s, and reports no unsupported features. Each resulting XYZ bounding box matches LibreDWG 0.14's independent direct DXF export: `(1,2,0)-(3,4,0)`, `(5,6,0)-(7,8,0)`, `(9,10,0)-(11,12,0)`. An exploratory read of the existing untracked AC1021 `tablet.dwg` further shows both libdxfrw and ODA File Converter 27.1.0.0 exports contain 48 3DFACE/81 SOLID/24 HATCH records, while FreeCAD's C++ importer creates 4,868 objects from each and reports 38 3DFACE/69 SOLID/22 HATCH as unsupported with entity-read exceptions. This shared behavior is not attributable solely to libdxfrw conversion, but it does not establish semantic equivalence or FreeCAD usability; the sample remains user-owned and unstaged. The optional legacy Python importer was not tested because its `dxfReader` dependencies are absent. No fixture added. Keep qualification limited to this one host/default importer/four LINE fixtures; no GUI, general DWG, or 3D consumer claim. Issue #19247 remains unreproduced because it has no affected DWG/output pair. |
| S8.11 | COMMITTED | Extracted the implicit source-revision policy into CLI-private `dwg2dxf/dx_cli.h` and directly tested every supported revision, AC1012→AC1014, and UNKNOWN/unsupported rejection in `dwg2dxf_version_tests.cpp`. `cmake --build build --target dwg2dxf libdxfrw_dwg2dxf_version_tests` succeeds; `ctest --test-dir build -R '^dwg2dxf_(version_policy|freecad_cli_compat)$' --output-on-failure` passes 2/2; `git diff --check` passes. The AC1012 mapping policy is unit-tested, but no authentic AC1012 DWG fixture was available to validate that reader path. No fixtures added. |

- Before implementation, convert the work packages into dependency-closed
  items with `READY`, `IN_PROGRESS`, `BLOCKED`, `VERIFIED`, and `COMMITTED`
  states and positive/negative gates. After every item, update its evidence,
  state, dependencies, and next action in this file in the same slice commit.
- Give each item a stable ID (`S0.1`, `S1.1`, …) matching its package and
  numbered step. Keep commits family-scoped and small; combine adjacent items
  only when they form one atomic invariant (for example, a writer plus its
  rollback test). Do not split a parser/writer contract so one commit leaves
  the tree knowingly inconsistent. The progress report counts these items,
  not the coarse S0–S7 headings.
- Self-unblock after each item: inspect the smallest failing boundary, add a
  local-from-scratch focused vector or source/spec trace, narrow unsupported
  claims, and proceed to another independent ready item. A blocked DWG byte
  layout does not block DXF semantics, adapter fields, docs, or other tested
  lanes.
- Each green slice is committed independently with its plan update. After
  every successful slice commit, report completed/total slices, item-state
  counts, evidence gained, and the next ready slice, then continue without
  pausing while a safe dependency-ready slice remains.
- Do not commit partial code simply to show activity. Stop only when no
  independent slice remains or an unresolved decision/authority is required;
  record that blocker and the attempted self-unblocking steps in the plan.
- Never turn `BLOCKED`, `EXPERIMENTAL`, `OPAQUE`, or advisory evidence into a
  completed support claim.

## Completion criteria

This plan is complete when:

1. Every in-scope geometry family in the completeness matrix has explicit
   DXF ASCII/binary and DWG-version status for read, typed write, opaque
   preservation, and derived display; custom/legacy forms are clearly
   distinguished from core entities.
2. DXF R2000/R2004/R2007/R2010 ACIS carriers and R2013+ ACDSDATA behavior are
   correctly routed or explicitly opaque/unsupported. Generic raw-section
   preservation is never presented as typed entity-to-ACIS association.
3. DWG frame bytes, inline ACIS bytes, proxy graphics, and DataStorage bytes
   have separate identities and cannot be substituted for one another.
4. Supported DWG ACIS/modeler versions pass bounded parse/write and
   independent read checks; unsupported/legacy/stub versions fail explicitly
   or remain raw-only/unqualified.
5. 3DFACE, 3DLINE variants, classic 3D POLYLINE variants, MESH, INSERT block
   transforms, HELIX/SPLINE, surfaces, and modeler geometry have field-wise
   semantic checks, with topology and coordinate-frame semantics covered by
   positive and malformed negatives.
6. README and support metadata claim only what those rows establish. No claim
   of full ACIS kernel semantics, NURBS evaluation, tessellation, or editing is
   implied by byte retention, wireframe extraction, or successful parsing.
7. `dwg2dxf` accepts FreeCAD's exact noninteractive `input -o output` call and
   preserves the existing positional interface. Because FreeCAD currently
   checks output-path existence rather than the converter's exit status,
   failed conversions must not publish a final/partial DXF at that path.
   Report converter output validation separately from an actual FreeCAD DXF
   import; neither alone promotes broad DWG/3D support.
8. The opt-in FreeCAD lane includes a verified nonzero-Z geometry result from
   the complete DWG→`dwg2dxf`→DXF→pinned FreeCAD importer path. Broader family
   rows record imported shape semantics and failures independently; a correct
   but unsupported DXF record remains converter success plus a downstream
   limitation, not a reason to substitute another entity or claim import.

## References and evidence hierarchy

Normative references take precedence over implementation analogies. Open-source
implementations are cross-checks, not authorities when they conflict with the
format specification.

- [Open Design Specification for .dwg files, v5.4.1](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf) — relevant coverage includes §20.4.13–20.4.17 for vertex/3D-polyline families, §20.4.33–20.4.36 for PFACE/classic POLYLINE mesh/SOLID/TRACE, and §20.4.41 for REGION/3DSOLID/BODY ACIS layout. Searchable-text review found no named modern `AcDbSubDMesh`, `AcDb*Surface`, or `3DLINE` layout; keep those corresponding DWG rows unqualified absent another primary source. The required local v5.4.1 copy was verified under S0.2 at `/Users/dli/doc/dwg/OpenDesign_Specification_for_.dwg_files (1).pdf`.
- [AutoCAD 2010 DXF Reference](https://images.autodesk.com/adsk/files/acad_dxf1.pdf) and [current Autodesk 3DSOLID DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm) — 3DSOLID/SURFACE ACIS payloads use groups 1/3; group 310 is not specified for the entity-inline ACIS body.
- [Autodesk DXF ENTITIES reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-7D07C886-FD1D-4A0C-A7AB-B4D21F18E484.htm) — record-family index and group-code reference.
- [Autodesk 3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm) — WCS corners, optional fourth vertex, invisible-edge bits.
- [Autodesk POLYLINE DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-ABF6B778-BE20-4B49-9B58-A94E64CEFFF3.htm) — flags, 2D OCS versus 3D WCS, mesh counts, extrusion.
- [Autodesk VERTEX DXF reference](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-0741E831-599E-4CBF-91E1-8ADBCFD6556D.htm) and [Polyface Meshes DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-96B6288E-F413-46C0-968A-A314171C0AAE.htm) — polyface vertex/face flags, signed invisible-edge indices, face ordering, and vertex/face counts.
- [Autodesk SEQEND DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-FD4FAA74-1F6D-45F6-B132-BF0C4BE6CC3B.htm) — sequence terminator fields; SEQEND is an R12-era entity and has no class-specific subclass data.
- [Autodesk MESH DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-4B9ADA67-87C8-4673-A579-6E4C76FF7025.htm) — group-code sequence for `AcDbSubDMesh`.
- [Autodesk 3DSOLID DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm) — ACIS/modeler version and proprietary payload groups.
- [Autodesk object-name reference](https://help.autodesk.com/cloudhelp/2016/ENU/AutoCAD-Customization/files/GUID-ECB6F2FF-6680-4514-86A7-7AD5551E378D.htm) — confirms DXF/object names for the AutoCAD surface families, `MESH`, `3DSOLID`, and other extension-sensitive classes; use as a class-name inventory, not a substitute for group-code or DWG byte-layout documentation.
- [Autodesk OCS in DXF](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm) — arbitrary-axis basis and distinction between WCS 3D entities and planar OCS entities.
- [Autodesk SOLID DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-E0C5F04E-D0C5-48F5-AC09-32733E8848F2.htm), [TRACE DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-EA6FBCA8-1AD6-4FB2-B149-770313E93511.htm), [ELLIPSE DXF reference](https://help.autodesk.com/cloudhelp/2023/ENU/AutoCAD-DXF/files/GUID-107CB04F-AD4D-4D2F-8EC9-AC90888063AB.htm), and [INSERT DXF reference](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-28FA4CFB-9D5E-4880-9F11-36C97578252F.htm) — SOLID's optional fourth-corner fallback, TRACE's OCS corner fields, ELLIPSE WCS axes/center, and INSERT scale/rotation/array fields.
- [Autodesk arbitrary-axis algorithm](https://help.autodesk.com/view/OARX/2026/ENU/?guid=GUID-E19E5B42-0CC7-4EBA-B29F-5E1D595149EE) and [BlockReference.BlockTransform](https://help.autodesk.com/cloudhelp/2018/ENU/OARX-ManagedRefGuide/files/OREFNET-Autodesk_AutoCAD_DatabaseServices_BlockReference_BlockTransform.html) — independent basis and block-transform semantics used by the INSERT/MINSERT expected-value oracle.
- [ezdxf INSERT implementation](https://github.com/mozman/ezdxf/blob/master/src/ezdxf/entities/insert.py) — open-source cross-check for nested INSERT and MINSERT grid offsets (array spacing is rotated by insertion angle without applying block scale); implementation analogy only, not normative evidence.
- [ezdxf ACIS FAQ](https://ezdxf.readthedocs.io/en/stable/faq.html) — cross-project statement that DXF R2000–R2010 uses SAT in entity records and R2013+ uses SAB in ACDSDATA; treat as a high-value implementation cross-check and confirm acceptance with Autodesk/real-file witnesses.
- [Autodesk DXF ENTITIES index](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-7D07C886-FD1D-4A0C-A7AB-B4D21F18E484.htm) — current listed record families; `3DLINE` is not listed there, so qualify it as a portability-sensitive extension.
- [LibreDWG entity/object definitions](https://github.com/LibreDWG/libredwg/blob/master/src/objects.in) and [LibreDWG manual](https://www.gnu.org/software/libredwg/manual/LibreDWG.html) — open-source entity names and implementation coverage; check stability per family.
- [FreeCAD `Draft/importDWG.py`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py) — current configured-path/PATH discovery, external `dwg2dxf <input> -o <output>` invocation, output-existence success check, and subsequent `importDXF` handoff; this establishes the converter CLI/publication contract, not entity support.
- [FreeCAD C++ DXF importer](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/ImpExpDxf.cpp) and [FreeCAD Draft DXF importer](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDXF.py) — primary implementation references for the C++ shape-construction path and legacy Python importer; audit the exact runtime revision and preferences because support differs by importer and release.
- [FreeCAD Import/Export Preferences](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Import_Export_Preferences.md) — DWG conversion is external and inherits DXF settings; distinguishes the faster C++ importer from the legacy Python importer.
- [FreeCAD issue #19247](https://github.com/FreeCAD/FreeCAD/issues/19247) — adjacent LibreDWG-produced-DXF import report without a reproducible input/output pair; investigation lead only.
- [ezdxf POLYLINE reference](https://ezdxf.readthedocs.io/en/stable/dxfentities/polyline.html), [Polyface tutorial](https://ezdxf.readthedocs.io/en/stable/tutorials/polyface.html), [MESH reference](https://ezdxf.readthedocs.io/en/stable/dxfentities/mesh.html), and [ACIS documentation](https://ezdxf.readthedocs.io/en/stable/acis.html) — independent DXF parser/writer cross-checks and explicit limits on arbitrary ACIS support.
- [LibreDWG issue #1411](https://github.com/LibreDWG/libredwg/issues/1411) — recent ACIS/DataStorage report retained as an investigation lead only; not a format authority or proof of libdxfrw behavior.
