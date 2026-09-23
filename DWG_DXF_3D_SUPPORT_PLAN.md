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

## Findings from the current source review

These are observations about code paths and existing tests, not blanket support
claims.

| Area | Current code evidence | Review consequence |
| --- | --- | --- |
| `3DFACE` | DXF dispatch and read/write routes exist. The reader requires XY components for the first three corners, accepts the fourth corner as optional and copies corner 3 when absent. The DWG parser bounds its invisible-edge flags to `0x0f`. | This aligns with Autodesk's WCS-corner and group-70 edge-bit definition. Add field-level and cross-reader checks, including the absent-fourth-corner form; do not spend time reimplementing behavior already present. |
| `3DLINE` | There are typed DXF/DWG routes and a DWG custom-class number. | Verify exact class registration and per-version availability against a real trace; third-party class numbers are not adequate DWG evidence. The searchable ODA v5.4.1 text reviewed here has no `3DLINE` entry, so modern custom-class layouts need a newer primary reference or target-produced, independently checked witness; keep pre-R13 forms on their own evidence lane. |
| 3D point/line families | `POINT`, `LINE`, `RAY`, and `XLINE` carry WCS 3D data; the DWG implementation also has distinct legacy reader code for pre-R13 3DLINE and a modern custom-class route. | Do not equate a 3D `LINE` with the implementation-specific `3DLINE` record. Audit legacy reader versions separately from custom-class DWG versions. Autodesk's current DXF ENTITIES index does not list `3DLINE`; treat that DXF spelling as an extension until a target-application witness establishes portability. |
| Planar entities placed in 3D | ARC/CIRCLE/ELLIPSE, SOLID/TRACE, 2D POLYLINE/LWPOLYLINE, HATCH, and INSERT use OCS/elevation/extrusion in different ways. INSERT adds scale, rotation, array spacing, and block-base transforms. | The current plan covered OCS but not the whole nested placement chain. Add default/non-world/negative extrusion plus nonuniform/mirrored and nested INSERT vectors; compare against Autodesk's arbitrary-axis and OCS rules, not a transform helper round-trip alone. |
| Classic `POLYLINE` 3D forms | The model stores 3D polylines, polygon meshes, and polyfaces in the `POLYLINE`/`VERTEX`/`SEQEND` family. DWG dispatch has separate vertex and face types, owned-child handling, and subtype checks. | Qualify flags, sequence order, closure, signed face indices, ownership, and R2004+ owned-object count by dialect/version. DXF readers must remain tolerant of legal child ordering; writers should emit the documented vertex-before-face order. |
| `MESH` / `AcDbSubDMesh` | Typed vertex/face/edge/crease/property-override data, topology validation, DXF and DWG encode/decode paths, and generated local round-trip tests exist. | Local self-round-trips are useful fast checks but are not an independent oracle. Autodesk's DXF table is useful for DXF group codes; the searchable ODA v5.4.1 text reviewed here has no named `AcDbSubDMesh` DWG layout. Keep DWG MESH layout/version claims unqualified until primary DWG evidence or a target-produced, independently checked witness exists. Confirm counts/indices separately for DXF and DWG. |
| Six analytic/NURBS surface classes | Typed DXF paths exist. The DWG surface parser retains a bounded raw ACIS body and links DataStorage; DWG surface encoding rejects versions before AC1021. Class registration and modern DWG read/write paths exist. | The searchable ODA v5.4.1 text reviewed here has no named modern `AcDb*Surface` layouts. Keep DWG surface layout/version claims unqualified until feature-specific primary evidence or target-produced, independently checked witnesses exist. Keep the AC1021+ writer gate meanwhile; check each typed field, handle, transform, and ACIS carrier separately. Do not imply surface evaluation. |
| `3DSOLID` / `REGION` / `BODY` and ACIS | DXF R2000–R2010 stores SAT groups 1/3 on the entity; R2013+ may place SAB in `ACDSDATA`. At baseline, `writeModelerGeometry()` treated SAT as text only through AC1018 and wrote binary chunks for later versions. S2.2 now routes textual payloads through groups 1/3 for AC1015/1018/1021/1024 and rejects binary, mixed, DWG-frame, and AC1027+ inline payloads rather than guessing. Generic raw-DXF-section preservation remains independent; typed association between a modeler entity and ACDSDATA is still not demonstrated. In DWG, the parser reads modeler status/version/history and skips bounded body data; the dispatcher later assigns the entire DWG object frame body to `DRW_ModelerGeometry::m_rawBytes`. For AC1027+ it also attempts DataStorage linking into a separate field. `decodeWireframe()` still chooses DataStorage when linked; the DXF writer no longer treats DWG frame/DataStorage metadata as SAT. `DRW_ModelerGeometry` has no DWG encoder, and `dwgRW` has no typed modeler-geometry write route. | Keep frame bytes, inline ACIS bytes, ACDSDATA/AcDsPrototype bytes, and proxy bytes separate. SAT routing is supported only through the explicitly tested AC1024 lane; AC1027+ SAB-to-entity association and DWG modeler writing remain unsupported/unqualified. |
| NURBS/spline curve path | The repository instructions record a known R2010+ DWG spline bit-layout discrepancy (`splFlag1` read as a bit-long where the ODA specification describes one bit). No AC1027/AC1032 sample is currently in the tracked fixture corpus. | Treat 3D spline/NURBS support as unqualified for affected DWG versions until the ODA chapter and a sample trace agree. Add a local-from-scratch bit-vector test for speed, but do not use it alone to promote a version support claim. |
| Semantic comparison | `tests/semantic_differential_adapter.cpp` currently routes MESH, modeler geometry, and surfaces through opaque serializers. | A successful opaque comparison cannot establish vertex, face, NURBS, transform, or payload-carrier correctness. Add canonical field serializers and compare payload identity separately from derived geometry. |
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

State: `IN_PROGRESS`; baseline sync is complete, but the DWG authority/evidence
inventory remains incomplete.

Dependencies: none. Output: updated capability ledger in this file, with a
baseline pinned to the rebased branch.

Files: this plan; `src/drw_entities.h`; `src/drw_interface.h`;
`src/libdxfrw.{h,cpp}`; `src/libdwgr.{h,cpp}`; `src/intern/dwgreader*`;
`src/intern/dwgwriter*`; `tests/`.

Steps:

1. Synchronize/rebase the implementation branch onto current `origin/master`;
   record `HEAD`, `origin/master`, dirty paths, and whether any user-owned
   untracked files were left untouched.
2. Resolve and read the authoritative ODA v5.4.1 PDF required by `AGENTS.md` at
   `~/doc/dwg/OpenDesign_Specification_for_.dwg_files.pdf`; the documented
   local path was absent during this review. Verify it matches the official
   [published ODA specification](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf).
   Read the relevant chapter before changing any `parseDwg(...)` body or any
   file under `src/intern/dwgreader*`, exactly as `AGENTS.md` requires. Until
   the local PDF is available, continue DXF/API/test-adapter work but do not
   edit any `src/intern/dwgreader*` file. A local copy alone does not prove a
   feature layout: record the applicable section (or that the reviewed v5.4.1
   text has no applicable section) for every DWG family/version row.
3. Build a capability ledger with rows for DXF ASCII/binary read/write and
   each DWG read/write version, including the legacy R1.4/R11 reader paths
   found in this source tree. For every row record the concrete dispatch,
   typed fields, raw carriers, version gate, class identity evidence, test
   type, and support-claim ceiling. Unknown means unknown, not supported.
4. Capture empirical DWG custom-class/type identities only from real local
   files/traces, per `AGENTS.md`; do not copy class numbers from third-party
   documentation into the reader.
5. Add an entity-specific compatibility column: normative DXF/DWG spelling,
   core versus vendor/custom class, legacy versus modern encoding, and
   whether the selected LibreDWG/ezdxf oracle marks the path stable. Treat the
   absence of an entity from an Autodesk reference index as a portability risk,
   not as proof that no application ever emits it.
6. Maintain an evidence map, not just a generic “ODA checked” flag. The online
   v5.4.1 review located classic vertex/polyline and ACIS modeler sections
   (including §20.4.13–20.4.17, §20.4.33–20.4.36, and §20.4.41), but found no
   searchable named layout for `AcDbSubDMesh`, modern `AcDb*Surface`, or
   `3DLINE`. Verify those apparent gaps against the local PDF itself; do not
   infer a byte layout from an absent section, DXF definitions, or another
   project's implementation. For a genuine gap, require a newer primary
   format reference or target-generated DWG plus trace and independent-reader
   confirmation. Pre-R13 readers require their own era-appropriate authority
   and sample; R13+ ODA layouts cannot qualify legacy forms.

Positive gate: one reviewed matrix ties each in-scope row to code, normative
paragraphs (or a recorded normative-source gap), and existing or planned
tests. Negative gate: any unsupported family/version or DWG reader change
without its applicable authority and authentic sample/trace is explicitly
blocked, not silently counted as implemented. For layouts absent from the
available ODA edition, keep only evidence-backed lanes moving; the missing
authority is a per-family blocker, not permission to extrapolate.

### S1 — Canonical semantic evidence for 3D entities

State: `READY` after S0 inventory.

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

State: `READY` after S1; highest priority format-independent carrier slice.

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

State: `BLOCKED_ON_S0_SPEC_AND_TRACE` for DWG reader edits; DXF and
test-adapter lanes continue independently.

Dependencies: S0 local spec/trace gate; S1; S2 carrier contract.

Files: `src/drw_entities.{h,cpp}`; `src/libdwgr.{h,cpp}`;
`src/intern/dwgreader*`; local runtime-generated vectors and semantic adapter.

Steps:

1. For R13/R14/R2000/R2004/R2007/R2010/R2013/R2018, use applicable ODA
   sections and actual per-version traces to define exact body/data
   boundaries, empty/unknown bits, modeler version, ACIS data encoding,
   history-handle behavior, and DataStorage presence/identity rules. This
   package covers ACIS modeler entities only; do not absorb MESH or surfaces
   into it merely because those entities also use modern class records. ODA
   §20.4.41 describes modeler data but notes that the ACIS stream is not fully
   decrypted; use it to bound/step/preserve the data, not as authority for
   arbitrary SAT/SAB semantics. Do not use §20.4.41 as evidence for MESH or
   analytic/NURBS surface layouts; the searchable v5.4.1 text reviewed here
   did not expose named sections for those modern classes. Keep each such DWG
   subtype `BLOCKED_ON_PRIMARY_LAYOUT_EVIDENCE` until an applicable authority
   and independently checked witness are available; proceed with DXF lanes.
2. Inventory the R1.4/R11 pre-R13 readers separately. Do not apply R13+
   layouts to their records; use an appropriate legacy specification and
   authentic version-specific sample trace before changing them. If no such
   source exists, leave the code unchanged and keep the row unqualified.
3. Implement bounded extraction and validation with transactional publication;
   malformed entity-level payloads must not consume adjacent frame/handle
   data or publish partial geometry.
4. Attach recovered ACIS payload to the typed modeler entity without
   overwriting its DWG object-frame carrier. For AC1027+ reconcile the
   entity's DataStorage marker, handle/key, exactly one selected data record,
   payload marker, and section version before publication.
5. Keep raw same-version frame replay as a separate operation and report
   whether a result is typed, raw-replayed, typed-and-raw, or unsupported.
6. Treat the open [LibreDWG ACDS/ACIS report](https://github.com/LibreDWG/libredwg/issues/1411)
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

State: `READY` after S1; coordinate-placement checks may proceed while DWG
reader lanes are blocked.

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
   instead of silently truncating flags on write.
3. Verify old `POLYLINE`/`VERTEX` subtypes: open/closed 3D polyline, M/N
   polygon mesh, PFACE vertex then face streams, signed one-based face
   references, hidden edge bits, `SEQEND`, and inconsistent declared counts.
4. Verify `MESH` face-list item counts independently of face count, n-gons,
   valid vertex references, edge/crease alignment, property overrides,
   subdivision level, and supported DXF versions.
5. Exercise OCS-to-WCS transforms using known arbitrary-axis expected values
   for default, oblique, and negative normals; assert each transform happens
   exactly once. Include elevation and thickness separately. Do not transform
   WCS-only `LINE`/`3DFACE`/3D `POLYLINE`/`MESH` coordinates as OCS data.
6. Exercise INSERT/MINSERT placement with a nonzero block base point, nested
   block, rotation, nonuniform and negative scales, oblique extrusion, arrays,
   and attributes. Compare the composed transform against a small independent
   matrix oracle; file read/write equality alone will not catch paired errors.
7. Reconcile the `SOLID`/`TRACE` four-point order with format rules and with
   the DXF writer/reader independently; do not normalize source ordering in a
   way that changes visible edge topology.

Positive gate: round trips preserve point order, flags, topology, frame, and
finite values. Negative gate: invalid vertex indices, impossible counts,
non-finite coordinates, half-present points, or overflowed count arithmetic
are rejected before callback publication.

### S6 — Surface/NURBS version qualification and known spline defect

State: `READY` for DXF inventory/field tests; DWG spline edits are
`BLOCKED_ON_S0_SPEC_AND_SAMPLE`; modern surface DWG layout claims are
`BLOCKED_ON_PRIMARY_LAYOUT_EVIDENCE`.

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
3. Fix the documented R2010+ `DRW_Spline::parseDwg` `splFlag1` width only after
   confirming ODA's one-bit rule and tracing an authentic sample. Add a
   generated bit-boundary regression and an independent field comparison;
   until an authentic AC1024/AC1027/AC1032 witness exists for the version in
   question, label that row experimental/unqualified. Do not use an AC1024
   trace to qualify AC1027 or the AC1032 pass-through stub.
4. Make unsupported NURBS/surface operations explicit. Retaining control
   points/knots or ACIS is not surface evaluation or tessellation.

Positive gate: generated fields read back exactly and available independent
readers agree on semantic fields/carrier placement. Negative gate:
truncated arrays, count disagreement, invalid handles, unknown subtype, and
version-incompatible output are rejected or retained as opaque with a
diagnostic.

### S7 — Independent qualification, claims, and release closeout

State: depends on S1-S6; qualify each row independently.

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

The execution sequence is therefore readiness-first, not table-order-first:
S0 → S1 → S2 and the DXF portions of S5/S6; then S3 → S4 after DWG
spec/trace readiness; finally S7. Continue any remaining independent DXF work
while a DWG dependency is blocked.

Current implementation-item ledger (update in every corresponding slice
commit):

| Item | State | Evidence / next action |
| --- | --- | --- |
| S0.1 | COMMITTED | Rebased onto `origin/master`; HEAD and origin are identical. Existing untracked paths remain untouched. |
| S0.2 | BLOCKED | `/Users/dli/doc/dwg/OpenDesign_Specification_for_.dwg_files.pdf` is unavailable; required before editing `parseDwg(...)` or `src/intern/dwgreader*`. Continue independent DXF/API/tests. |
| S0.3 | IN_PROGRESS | Map dispatch, versions, fields, and claim ceiling for every 3D family; do not promote unknown DWG routes. |
| S0.4 | READY | Record custom DWG class/type identity only from local target traces; run after applicable spec/sample access. |
| S0.5 | IN_PROGRESS | DXF/core-vendor/legacy inventory is underway; compare selected independent implementations without treating them as normative. |
| S0.6 | BLOCKED | Verify searchable-text gaps against the required local ODA PDF; modern MESH/surface/3DLINE DWG rows stay unqualified. |
| S1.1 | COMMITTED | Field-level serializers now cover 3DFACE, 3DLINE, POLYLINE/VERTEX, MESH, HELIX/SPLINE, modeler geometry, all surface subtypes, INSERT placement, and nested ATTRIB fields. Loft reference values are typed; binary values remain digest carriers. |
| S1.2 | COMMITTED | Runtime self-tests verify mesh-coordinate mutation, polyface index serialization, INSERT/ATTRIB placement, loft-reference typed/binary separation, and modeler frame-body labeling/digest. Manual C++17 `-Wall -Wextra -Werror` adapter build and `--self-test` pass; `ctest -R '^libdxfrw_dwg_local_roundtrip$'` passes 1/1. Existing generated malformed DXF modeler checks remain in the fast round-trip test. No downloaded fixtures added. |
| S2.1 | COMMITTED | DXF modeler reads now retain ordered group-1/3 text and group-310 binary chunk metadata as bounded views into `m_rawBytes`. The semantic adapter reports chunk bounds, keeps surface bytes explicitly unclassified, and gives DWG frame/DataStorage payloads distinct digests; `ACDSDATA` stays an independent raw-section carrier with no invented entity link. Strict C++17 adapter build/self-test and focused round-trip CTest pass for text, binary, and mixed chunk sequences. |
| S2.2 | COMMITTED | Modeler and surface writers emit SAT text groups 1/3 only through AC1024, reject binary/mixed/unqualified DWG carriers, and reject AC1027+ inline payloads until ACDSDATA association is known. Runtime tests pass for AC1015/1018/1021/1024, ASCII/binary DXF file encodings, surface SAT, mixed-input rejection, and unsupported SAB/frame payloads. LibreDWG 0.14 `dxf2dwg --as r2000` independently read all four generated version vectors and wrote DWG output (non-fatal unknown `HEADER.DIMLDRBLK` warnings only). Generic ACDSDATA capture/replay remains a separate opaque-section path; no entity association/support claim is added. |
| S5.1 | READY_AFTER_S1 | DXF ASCII/binary topology, OCS/WCS, MESH and compound POLYLINE checks. |
| S5.2 | READY_AFTER_S1 | INSERT/MINSERT independent transform oracle and malformed-input checks. |
| S6.1 | READY_AFTER_S1 | DXF surface/HELIX/SPLINE field inventory and focused tests. |
| S3.1–S3.6 | BLOCKED_ON_S0.2 | DWG ACIS extraction/version work; do not touch `src/intern/dwgreader*` until the local authority and per-version evidence gates are met. |
| S4.1–S4.4 | BLOCKED_ON_S3 | Opaque DWG modeler payload writing follows only verified read layouts. |
| S7.1–S7.5 | BLOCKED_ON_READY_SLICES | Qualify only completed format/version rows, then narrow README/support claims accordingly. |

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

## References and evidence hierarchy

Normative references take precedence over implementation analogies. Open-source
implementations are cross-checks, not authorities when they conflict with the
format specification.

- [Open Design Specification for .dwg files, v5.4.1](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf) — relevant coverage includes §20.4.13–20.4.17 for vertex/3D-polyline families, §20.4.33–20.4.36 for PFACE/classic POLYLINE mesh/SOLID/TRACE, and §20.4.41 for REGION/3DSOLID/BODY ACIS layout. Searchable-text review found no named modern `AcDbSubDMesh`, `AcDb*Surface`, or `3DLINE` layout; verify those apparent omissions against the local PDF and keep the corresponding DWG rows unqualified absent another primary source. The local copy required by `AGENTS.md` was not found during this review.
- [AutoCAD 2010 DXF Reference](https://images.autodesk.com/adsk/files/acad_dxf1.pdf) and [current Autodesk 3DSOLID DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm) — 3DSOLID/SURFACE ACIS payloads use groups 1/3; group 310 is not specified for the entity-inline ACIS body.
- [Autodesk DXF ENTITIES reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-7D07C886-FD1D-4A0C-A7AB-B4D21F18E484.htm) — record-family index and group-code reference.
- [Autodesk 3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm) — WCS corners, optional fourth vertex, invisible-edge bits.
- [Autodesk POLYLINE DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-ABF6B778-BE20-4B49-9B58-A94E64CEFFF3.htm) — flags, 2D OCS versus 3D WCS, mesh counts, extrusion.
- [Autodesk MESH DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-4B9ADA67-87C8-4673-A579-6E4C76FF7025.htm) — group-code sequence for `AcDbSubDMesh`.
- [Autodesk 3DSOLID DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm) — ACIS/modeler version and proprietary payload groups.
- [Autodesk object-name reference](https://help.autodesk.com/cloudhelp/2016/ENU/AutoCAD-Customization/files/GUID-ECB6F2FF-6680-4514-86A7-7AD5551E378D.htm) — confirms DXF/object names for the AutoCAD surface families, `MESH`, `3DSOLID`, and other extension-sensitive classes; use as a class-name inventory, not a substitute for group-code or DWG byte-layout documentation.
- [Autodesk OCS in DXF](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm) — arbitrary-axis basis and distinction between WCS 3D entities and planar OCS entities.
- [Autodesk SOLID DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-E0C5F04E-D0C5-48F5-AC09-32733E8848F2.htm) and [INSERT DXF reference](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-28FA4CFB-9D5E-4880-9F11-36C97578252F.htm) — OCS/extrusion, thickness, insertion scale/rotation/array fields.
- [ezdxf ACIS FAQ](https://ezdxf.readthedocs.io/en/stable/faq.html) — cross-project statement that DXF R2000–R2010 uses SAT in entity records and R2013+ uses SAB in ACDSDATA; treat as a high-value implementation cross-check and confirm acceptance with Autodesk/real-file witnesses.
- [Autodesk DXF ENTITIES index](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-7D07C886-FD1D-4A0C-A7AB-B4D21F18E484.htm) — current listed record families; `3DLINE` is not listed there, so qualify it as a portability-sensitive extension.
- [LibreDWG entity/object definitions](https://github.com/LibreDWG/libredwg/blob/master/src/objects.in) and [LibreDWG manual](https://www.gnu.org/software/libredwg/manual/LibreDWG.html) — open-source entity names and implementation coverage; check stability per family.
- [ezdxf POLYLINE reference](https://ezdxf.readthedocs.io/en/stable/dxfentities/polyline.html), [Polyface tutorial](https://ezdxf.readthedocs.io/en/stable/tutorials/polyface.html), [MESH reference](https://ezdxf.readthedocs.io/en/stable/dxfentities/mesh.html), and [ACIS documentation](https://ezdxf.readthedocs.io/en/stable/acis.html) — independent DXF parser/writer cross-checks and explicit limits on arbitrary ACIS support.
- [LibreDWG issue #1411](https://github.com/LibreDWG/libredwg/issues/1411) — recent ACIS/DataStorage report retained as an investigation lead only; not a format authority or proof of libdxfrw behavior.
