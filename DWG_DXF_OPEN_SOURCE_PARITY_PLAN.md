# DWG/DXF parity — remaining implementation plan

Updated 2026-11-03. This file contains open work only. Implementation history and
completed-item evidence belong in Git and `docs/research/`, not in this queue.

The objective remains full, evidence-backed DWG/DXF parity with the intended
LibreCAD/libdxfrw routes, preserving LibreCAD and LibreCAD_3 as 2D consumers while
enabling 3D consumers and FreeCAD's `dwg2dxf` workflow. Support includes the
implemented read/write version surface, ASCII/Binary DXF, metadata, ownership,
geometry and explicit loss/refusal behavior. A smaller transport subset does not
replace this objective.

## Authority and completion gates

Use current code and source-matching receipts before treating an old PAR/7* finding
as an unfixed defect. Source parity, retained bytes, typed fields, complete document
conversion and native semantic acceptance are separate evidence levels.

| Gate | Current unresolved state | Required completion evidence |
| --- | --- | --- |
| Original source bodies | Full original-record family coverage remains open. | Classify raw/typed normalization separately from actual missing source fields; census original records and independently match field/reference roles, including unsupported/dropped records |
| Ownership | `ownerTargetsResolved=false` | Every required target exists, has the correct role/type/owner, and is emitted |
| Recursive closure | `recursiveClosureMatched=false` | Required targets' dependencies close without omissions, aliasing or unbounded work |
| Output qualification | `outputEligible=false`; support ledger **CLOSED** | Frozen source → planned identity/fields → actual emitted bytes → consumer meaning |
| External evidence | Six J items **DEFERRED_EXTERNAL** | Eligible independent semantic evidence and platform execution; external success alone cannot promote |
| Whole objective | `fullObjectiveComplete=false` | All required local, consumer, platform and claim gates meet their actual scope |

Use the resource ledger as an inventory aid, not as proof of completeness;
expand it when evidence identifies another family. Preserve the authorities in
[the support policy](docs/UPGRADE_SUPPORT.md),
[the route matrix](metadata/support-matrix-v1.json),
[claim status](metadata/qualified-format-status-v1.json),
[external dispositions](metadata/external-advisory-report.json), and the
[3D plan](DWG_DXF_3D_SUPPORT_PLAN.md). Zero advertised claims is not completion.

## Execution queue

This is an open-work queue. Completed implementations and their evidence stay in
Git history and `docs/research/`; do not restate them here. A phase remains open
only while its explicitly listed residual scope blocks its exit gate.

P4–P9 are umbrella exit gates, not individual implementation tasks. A landed
slice is removed from this queue and recorded by its commit and research
receipt; the parent phase stays OPEN until its full gate passes. P9 is
external-only and cannot close through ordinary local implementation work.
Phase rows express aggregate exit gates, not item-level completion counts.

| ID | State | Remaining result | Dependency |
| --- | --- | --- | --- |
| P4 | OPEN — source/graph closure | Close reproduced source-required bodies and dependencies that still lack strict output/readback proof. On tracked AC1027 `ordinary_enc_AC1027.dwg`, strict output remains refused because the independent source map reports handles `0x35`–`0x38` as absent targets. Current LibreDWG master consumes the ENC reference before common handles, contrary to ODA's stated order; keep that peer differential unqualified until a conforming source with resolvable references can adjudicate it. Require valid target kinds, ownership and complete readback before graph admission. See the [AC1027 handle-role evidence receipt](docs/research/p4-ac1027-ordinary-classes-boundary-and-graph-blocker-2026-11-01.json). For AC1018 `ET-Drawing-with-Border.dwg`, DICTIONARY `0x2CD` remains unqualified because its sole `ACAD_SORTENTS` edge reaches SORTENTSTABLE `0x2CE` under named BLOCK_RECORD `0x24C` (`ET Border block`); the documented DXF owner scope does not admit that named-block table, so preserve/refuse the parent and child as one graph unless an authoritative eligible mapping is found. See the [AC1018 SORTENTSTABLE dependency receipt](docs/research/p4-ac1018-named-block-sortents-dependency-2026-11-01.json) and [owner-scope receipt](docs/research/p4-ac1024-sortents-owner-scope-qualification-2026-11-01.json). For AC1024 `title_block-arch.dwg`, retain refusal for the three remaining named-block SORTENTSTABLEs (handles 4202, 4380, 4637; owners `*U3`, `Title_Block-ARCH`, and `*U6`) under the same owner-scope rule; see the [current residual receipt](docs/research/p4-ac1024-title-block-named-block-sortents-residual-2026-11-01.json). The earlier XRECORD `0x121F` group-1071 blocker now passes this AC1024 DWG-origin compatibility route; LibreDWG 0.14 emits the same ordered group-1071 payload values for handle `0x121F`. Autodesk's standard XRECORD range excludes 1071, so this remains producer-compatibility evidence only, without native-consumer qualification. Current strict conversion reaches finalization at 106/109 and remains atomically refused for those three SORTENTSTABLEs; see the [XRECORD recheck receipt](docs/research/p4-ac1024-title-block-xrecord-1071-compatibility-recheck-2026-11-02.json). Whole-source AC1018 output remains blocked by the invalid non-shape empty-name STYLE `0xDF`; keep it refused until a source- and consumer-backed output route is established. A bounded same-revision AC1018+ `ACAD_PROXY_ENTITY` route now retains typed callbacks, rebuilds and remaps group-91 ordinals from complete ordered source class rows, and writes the modeled payload for the no-reference/no-unknown-data profile. Generated ASCII/Binary readback covers class ordinal remapping, a 19-bit payload, atomic object-ID refusal, and corrected retry ([transport receipt](docs/research/p4-dwg-proxy-entity-bounded-dxf-transport-2026-11-02.json)). The low-level writer has generated resolver-backed object-ID serialization and typed readback for groups 330/340/350/360. A bounded adapter graph path now plans DWG handle-code 4/5 pointer and offset-pointer codes 6/8/10/12 (groups 330/340) to unique top-level typed entity records: it checks block ownership, source-namespace collisions and raw-record collisions, preallocates target identities, resolves through the proxy-scoped callback, and requires each target and proxy to appear once with the expected type, section, owner and remapped handle during finalization. ASCII/Binary controls cover missing, duplicate, colliding, unsupported-owner and deliberately omitted targets, destination preservation, corrected retry, and fresh typed readback ([adapter graph receipt](docs/research/p4-dwg-proxy-entity-object-id-adapter-graph-closure-2026-11-02.json); [low-level object-ID receipt](docs/research/p4-dwg-proxy-entity-object-id-dxf-transport-2026-11-02.json)). This does not close nested/raw/OBJECTS targets, owner-role codes 2/3 or unknown handle codes, recursive target dependencies, unknown-data slots, EED/common references, mixed class/ACDS graphs, physical producer coverage, or native semantics; unsupported profiles still fail closed and no support claim is promoted. Generated AC1027 GEODATA version-2 adapter transport now has bounded typed capture, exact source CLASS/common-owner binding, host-pointer failure/retry, per-frame output binding and ASCII/Binary readback ([receipt](docs/research/p6-geodata-dwg-adapter-transport-2026-11-02.json]); physical producer breadth, native geospatial semantics and whole-document qualification remain open. Generated AC1027 RASTERVARIABLES adapter output is bounded to a same-revision generated profile with rooted dictionary ownership, atomic field refusal/retry, and ASCII/Binary typed readback ([receipt](docs/research/p4-rastervariables-dwg-adapter-transport-2026-11-02.json)); physical producer breadth, native image-display semantics and whole-document qualification remain open. Generated AC1027 WIPEOUTVARIABLES adapter output now has bounded typed capture, exact source CLASS/frame binding, unique NOD → ACAD_IMAGE_VARS dictionary ownership, source-frame qualification, and ASCII/Binary refusal/retry/readback ([receipt](docs/research/p4-wipeoutvariables-dwg-adapter-transport-2026-11-02.json)). Retain DWG mode 2 on input but refuse DXF group 70 modes outside 0/1: Autodesk's WIPEOUTVARIABLES DXF table lists 0/1 and conflicts with the WIPEOUTFRAME system variable's 0/1/2 values; the DXF table lists subclass AcDbRasterVariables and group-90 class version 0, while current and bundled writers emit AcDbWipeoutVariables and omit group 90. No crosswalk or native semantics is established; physical producer breadth remains open. The DXF reader now rejects group-70 input outside signed 16-bit before narrowing into the public unsigned carrier, while retaining representable unsupported values for strict-output refusal; see the [width-guard receipt](docs/research/p4-wipeoutvariables-dxf-group70-width-guard-2026-11-02.json). For the admitted AC1024 LayerStates profile, independently establish native consumer restoration/evaluation semantics. Keep the exact selected AC1024 XRECORD group-370 and nested SCALE-reference rules source-bounded; do not generalize them to other XRECORD application profiles. | Keep read retention separate from output qualification. Preserve unknown tails and strict refusal; use existing repository inputs or locally authored fixtures, never add investigation DWGs. |
| P5 | OPEN — dynamic-block semantics, rendering and TABLE residual profiles | The absent `EvalExpr.parentId` default is now normalized to unsigned -1 on the covered AC1024 typed DXF import profiles, matching LibreDWG's importer default; no DXF field was added and semantic equivalence remains unqualified. A source-bound physical control pins `BLOCKLINEARGRIP` handle `0x1111` to the DWG sentinel and checks the normalized typed ASCII/Binary readback; see the [default-alignment receipt](docs/research/p5-ac1024-dynamic-evalexpr-default-normalization-2026-11-01.json). The older whole-document two-root failure/retry selector no longer reaches readback on current code because strict completion refuses six `SORTENTSTABLE` callbacks with non-model/paper-space owners (two named blocks and four anonymous `*U` blocks; 484/490 qualified); keep that check dependent on P4's owner-scope gate and preserve the refusal. See the [current graph recheck](docs/research/p5-ac1024-dynamic-graph-failure-readback-2026-10-31.json). Native evaluation evidence for the AC1024 graph remains open. Also qualify remaining rendering profiles, physical FIELD producer/version profiles and native FIELD evaluation, block-cell geometry (the AC1027 standalone TABLEGEOMETRY null-cell cache profile now has bounded generated output/readback proof; other profiles and native semantics remain open), CONTENTFORMAT/CELLSTYLEMAP combinations, overlapping edge semantics and remaining AC1018 TABLE profiles. The ODA-defined AC1018 text-cell decoder now maps text height and indexed color, lineweight and visibility for top, right, bottom and left edges; generated vectors cover the full DWG field order, all edge roles, cursor alignment and unsupported virtual edges. Strict DXF writing emits only representable profiles whose legacy group-177 mask fits 16 bits, and now refuses missing selected visibility values rather than dropping them. The exact all-edge ASCII/Binary profile and physical-source exclusion are recorded in the [AC1018 TABLE edge receipt](docs/research/p5-ac1018-table-cell-four-edge-overrides-2026-11-02.json). No eligible physical TABLE producer callback or source-to-output proof is established, and higher masks remain refused; qualify native rendering separately. Generated LAYOUT shade modes 4/5 and selected VIEWPORT shade-graph transport now pass focused ASCII/Binary output/readback controls; this does not qualify physical producer support or native rendering semantics. See the [current shade transport recheck](docs/research/p5-shade-consumer-transport-current-recheck-2026-11-01.json). The local AC1021 architectural sample's flags=3/type=0 and flags=4/type=8 root profiles have bounded transport coverage. A generated exact-stream vector covers AC1021 flags=4/type=2 root BD transport. A bounded physical AC1021 FIELD profile now decodes 71 `ACFD_FIELDTEXT_CHECKSUM` child values (flags=2/type=2) as finite BD doubles: ODA §20.4.99 says only flag bit 1 (`0x01`) suppresses the type-specific payload, and current LibreDWG master applies the same mask. The focused physical census now sees 86 typed FIELD entries, 56 raw-only shells among 142 source FIELD frames, and all 71 checksum children complete; this parser correction does not establish physical source-to-output or native FIELD semantics; a separate generated strict DXF transport/readback control covers the exact flags=2/type=2 checksum child and refuses an unqualified key (see the [generated transport receipt](docs/research/p5-ac1021-field-checksum-generated-dxf-transport-2026-11-02.json)). See the [flags=2 checksum payload resolution receipt](docs/research/p5-ac1021-field-flags2-checksum-payload-resolution-2026-11-02.json). The 42 LibreDWG-selected `AcObjProp.16.2` handles match exactly to local raw-only shells. Their object-data spans leave 258 bits on 41 records and 210 bits on handle `0x1481E`; all four peer-disputed `ObjectPropertyId` roots have 258 remaining bits. Keep them incomplete while their body and handle schemas remain unresolved. Their child type-2/type-64 differential and target roles also remain unresolved; ODA §20.4.76/§20.4.99 says type 2 is BD and type 64 is a handle-stream soft pointer. Resolve these spans and roles before enabling source-profile output; see the [flags=4/type=2 census receipt](docs/research/p5-ac1021-field-flags4-type2-root-census-2026-11-01.json). Keep four flags=4/type=4 roots refused: the type-4 payload agrees with ODA and LibreDWG, but the group-98 dispute persists (ODA says TV; LibreDWG says BL). A bounded ODA-TV probe reads an empty TV for all four roots, while the existing body-side BL candidates still do not yield valid child-count framing. See the [FIELD transport receipt](docs/research/p5-ac1021-field-format-flag4-root-type4-8-transport-2026-11-01.json). Other flags/child-value profiles remain open. The proxy decoder now maps ODA §29 32-bit BYBLOCK/BYLAYER sentinel values and retains the existing low-16-bit values in the same 4-byte RL field; generated controls cover both forms. The local AC1021 source has 36 MLINE proxy caches with SUBENT_LINETYPE opcode 18 value 0x7FFF. ODA §20.4.53 and independent LibreDWG JSON establish 54 ordered LAYER_CONTROL slots, including null slots 44–48; slots 51–53 resolve to `Mleader @ 48`, `Mleader @ 24`, and `Mleader @ 4`, matching the 36 caches' 24/8/4 index counts. The reader now retains null control slots for source-order layer lookup while keeping the ownership-reference list compact; all 36 proxy streams decode with zero unresolved resource references. The bounded control-slot and proxy layer-name vectors now use the exception-catching `DRW::reserve` helper and return through existing failure paths on allocation failure; the generated control-slot selector passes (see the [allocation-safety receipt](docs/research/p5-control-handle-slot-reserve-guard-2026-11-02.json)). Linetype references are the 0x7FFF BYLAYER sentinel, so linetype null-slot ordering remains unqualified. This does not qualify whole-cache replay, destination DXF table-index remapping, or native rendering; strict physical conversion still refuses the 36 typed proxy caches at preflight. See the [layer-slot resolution receipt](docs/research/p5-proxygraphic-layer-control-slot-resolution-2026-11-02.json), [resource-index receipt](docs/research/p5-proxygraphic-resource-index-validation-2026-11-02.json), [sentinel receipt](docs/research/p5-proxygraphic-linetype-sentinel-dual-profile-2026-11-02.json) and [suppressed type-0 receipt](docs/research/p5-field-kunknown-suppressed-transport-2026-11-01.json). Generated Layer viewport-override XRECORD controls now cover ASCII/Binary parsing, strict atomic refusal for groups 420/440 outside Autodesk's documented XRECORD payload envelope, and ASCII/Binary output/readback/remapping for the in-envelope group-91/343 profiles. This does not establish physical producer support, dictionary collation, native override semantics, or VIEWPORT entity field references; see the [Layer override receipt](docs/research/p5-layer-viewport-override-xrecord-transport-2026-11-02.json). A generated ASCII parser control now ensures undocumented VIEWPORT group 46 cannot overwrite the group 72 circle-zoom field; see the [field-isolation receipt](docs/research/p5-viewport-undocumented-group46-isolation-2026-11-02.json). Native VIEWPORT rendering and handle-reference qualification remain open. | For other profiles, admit only source- or consumer-backed fields and edges; preserve role/handle dependencies and fail closed on unrepresented data. |
| P6 | OPEN — version, schema and semantic residuals | Resolve only DWG failures still reproduced against current code and close a source-backed representative family. The current focused local round-trip CTest passes its selected DIMASSOC and object-carrier readback controls, surface subtype/retry assertions, and generated ASCII/Binary DIMENSION frame round-trips across raw and `applyExtrusion` reads. The DIMENSION writer now projects transformed OCS text/clone/arc points back to native OCS for both DXF and DWG serialization, without changing the public object layout. This is generated local transport evidence only; native dimension rendering and physical producer coverage remain open. Other gaps include the AC1027 Cover suffix and the AC1032 ODA-resave HEADER suffix (the bounded AC1027-origin DataStorage table-role profile now parses, but strict output still refuses), TABLEGEOMETRY BD/BL producer-span evidence, route/version parity, associative meaning, HATCH/MPOLYGON/MESH semantics, annotation and geometry beyond the bounded selected-record profiles, units/inheritance, unknown/proxy data, safety/caller state, and CLI/oracle contracts. See the [AC1032 DataStorage profile recheck](docs/research/p6-ac1027-cover-ac1032-datastorage-role-profile-2026-11-02.json). Generated strict CLI issue-mask controls exercise all 15 defined `DxfTextIssue` flags across 14 locally authored profiles, with exact-mask checks and atomic refusal; see the [CLI issue-mask matrix receipt](docs/research/p6-cli-text-decoder-issue-mask-matrix-2026-11-02.json). Whole-document output and native dimension/associative behavior remain unqualified. | Read ODA v5.4.1 and trace a real source before changing DWG layouts. |
| P7 | OPEN | Close required ACIS/ACDS source-to-output associations and obtain independent 3D kernel meaning for claimed profiles. | Consume P4's qualified graph/output bindings when available; bind ACIS/ACDS source associations to emitted output for qualification. |
| P8 | OPEN — LibreCAD/LibreCAD_3, Linux/Windows, 3D consumers and release gates | Qualify LibreCAD/LibreCAD_3 callback behavior, native Linux/Windows FreeCAD, intended 3D consumers, ABI, packaging and release contracts on a composed candidate. Local API/3D probes and the generated direct-adapter/serializer runtime-subtype guard pass their focused controls; the latter still has no native semantic effect. All six generated surface subtypes now also pass named-BLOCK ASCII/Binary staging and raw/`applyExtrusion` readback with subtype sentinels; see the [BLOCK surface staging receipt](docs/research/s8-1a-block-surface-staging-2026-11-02.json). See the [compatibility and 3D probe receipt](docs/research/p8-local-compat-and-generated-3d-probe-2026-11-02.json) and [surface runtime-type guard receipt](docs/research/s8-1a-direct-surface-runtime-type-guard-2026-11-02.json). Committed slice `1b5c496c12adc6c49703b0660e5f1a2e4bc672e0` now retains and dispatches GeoPositionMarker and SectionObject callbacks through the concrete BLOCK adapter. Generated ASCII/Binary controls cover marker fields including embedded MTEXT, section fields, raw and `applyExtrusion` callback reads, and refusal to retain an oversized section; see the [GeoPositionMarker/SectionObject receipt](docs/research/s8-1a-geo-section-block-adapter-2026-11-02.json). This remains a generated AC1027 transport profile and does not close BLOCK callback inventory or native semantics. Committed slice `c6ab3b3d8d768c9a001239f83db0817c185cb072` adds bounded capture and writer dispatch for OLE2FRAME and OLEFRAME BLOCK entities. The generated ASCII/Binary named-BLOCK round-trip verifies opaque payload bytes and OLE2 client/frame metadata; see the [OLE BLOCK adapter receipt](docs/research/s8-1b-ole-block-adapter-2026-11-02.json). This is transport evidence only; it does not qualify native OLE behavior or close BLOCK callback inventory. Committed slice `dc2e10366a32368764088ef99a85304e8bff3e22` captures and dispatches named DXF SHAPE BLOCK callbacks. The writer requires the group-2 SHX shape name; generated ASCII/Binary controls prove readback plus nameless-write destination preservation and corrected retry. DWG-origin shapes with only a SHAPEFILE/STYLE handle stay refused until an actual glyph-name mapping is available; see the [SHAPE adapter receipt](docs/research/s8-1c-shape-block-adapter-2026-11-02.json). Native glyph rendering and physical DWG-to-DXF SHAPE output remain open. The narrow macOS ARM64 FreeCAD 2D open/insert path has a standalone evidence receipt; it does not close these broader gates. | Relevant local conversion and output paths must be source-stable. |
| P9 | DEFERRED_EXTERNAL | Resolve J256, J260, J268, J284, J293 and J295 only with eligible independent evidence; external results remain non-promoting until claim gates pass. | Follow `metadata/external-evidence-resolution-v1.json`. |

P6–P9 qualification can proceed when its inputs are ready; it does not block
independently ready local code work.

**SORTENTSTABLE owner-scope restriction:** Autodesk's DXF definition limits the
owner reference to `*MODEL_SPACE` or `*PAPER_SPACE`. Physical AC1024 testing
therefore qualifies the Model/Paper Space table only; named-block tables remain
unqualified even when their source graph and emitted bytes are internally
consistent. The previous multi-table output receipt records physical output
evidence, but its named-block callbacks are not eligible DXF support claims
under this owner schema. See the [owner-scope receipt](docs/research/p4-ac1024-sortents-owner-scope-qualification-2026-11-01.json).

**Open external semantic gate — native dynamic-block evaluation.**
The AC1024 source has `EvalExpr.parentId=0xFFFFFFFF` for `BLOCKLINEARGRIP`
handle `0x1111`, while the current DXF readback model reports default `0`.
The updated consumer-admission [receipt](docs/research/p5-dynamic-block-consumer-admission-2026-10-31.json)
records that Autodesk's public references describe expression-graph behavior
but do not define a `parentId` DXF mapping or sentinel semantics. LibreDWG's
field map identifies `parentid` as a DWG `BLd` field with no DXF group mapping;
that is implementation evidence, not normative proof of semantic irrelevance.
Do not invent a DXF group code or equate the normalization with native meaning.
Keep the gate open until an authoritative format source or an independent
evaluator bound to exact source/output bytes, executable and configuration
establishes the required behavior for both roots, visibility, parameter/action
behavior and reachable geometry. The available FreeCAD, ODA converter and
LibreDWG paths do not currently supply that evaluator.

Separate unresolved gates remain: AC1021 proxy-graphics refusal belongs to P6.
For the AC1018 source, do not bypass the invalid empty-name STYLE refusal by
omitting, renaming or replaying the row without source and consumer evidence.

## P4 — original bodies and complete document closure

P4 supplies source/context/role providers and composes them with the existing
local record providers and P5 output. Do not create parallel capture or graph
engines.

1. **Source admission:** open another associative parser/schema packet only when
   a physical source identifies a required nonempty or mismatched body,
   action-dependency, owned-parameter, or versioned-registry profile. Prove its
   class, target role, owner route, fields and bounded span before decoding; reuse
   qualified providers. Keep version-2 nonzero-seed registry layouts raw until
   their framing is proven. Keep investigation DWGs out of committed fixtures;
   bind regressions to eligible repository inputs or locally authored fixtures.
2. **Graph and output admission:** for each reproduced source-required body,
   owner edge, extension-dictionary edge or descendant not already admitted,
   bind source identities and fields to planned records, emitted bytes and fresh
   readback.
   Use the existing census and profile receipts to trace each remaining callback
   through owner/dependency planning and writer admission. Keep reader
   completeness distinct from output qualification: neither a typed callback nor
   retained raw bytes alone proves output preservation. Admit only complete
   typed output or a proven end-to-end replay path. Keep unresolved records
   refused. Never weaken the completeness predicate, emit a partial graph, or
   infer edges from adjacent handle values.
   HATCH and MPOLYGON group-330 boundary references now reject zero and
   `DRW::NoHandle` sources and outputs. Generated ASCII/Binary controls cover
   mapped IDs, invalid remaps, rollback, and refusal before publishing a
   boundary record ([receipt](docs/research/p4-hatch-mpolygon-boundary-output-id-guard-2026-11-03.json)).
   This validates ID representation only; target existence, target kind,
   boundary association and native fill semantics remain open.
   The shared typed common-metadata writer validates reactor and
   extension-dictionary source IDs and rejects zero IDs from the direct remap
   path before emitting those groups ([receipt](docs/research/p4-common-object-reference-output-id-guard-2026-11-03.json)).
   `DRW::NoHandle` aliases zero. A configured callback resolver's existing
   zero-result behavior omits that optional typed edge and its empty wrapper;
   it is not treated as an invalid emitted ID. Explicitly retained
   ACAD_REACTORS/ACAD_XDICTIONARY app-data groups now receive the same source-ID
   and direct-remap zero checks, while resolver-zero values/groups preserve the
   optional omission behavior ([receipt](docs/research/p4-common-appdata-reference-output-id-guard-2026-11-03.json)).
   These guards prove ID representation and framing only. They do not prove
   target existence, target kind, ownership, or graph closure; malformed or
   unparseable retained values remain on the opaque preservation route.
   Ordinary entity reactor and extension-dictionary groups now use the same
   preflight contract: zero source IDs and zero direct-remap results refuse
   atomically, while resolver-zero optional edges are omitted. Generated
   ASCII/Binary controls verify exact mapped IDs, omission framing, and rollback
   ([receipt](docs/research/p4-entity-common-reference-output-id-guard-2026-11-03.json)).
   This is still ID transport only and does not establish target existence,
   owner validity, or recursive closure.
   For `PARTIALVIEWINGINDEX`, the generated low-level writer now remaps and
   reserves or allocates group-5 identity, binding the source identity only
   after successful staged emission; group-340 targets must resolve to one
   emitted record and match an unambiguous typed entity/object/table source
   identity. Generated ASCII/Binary controls cover
   resolver precedence, missing and mismatched targets, rollback/retry, and
   typed readback ([receipt](docs/research/p4-partialviewingindex-reference-closure-2026-11-03.json)).
   The exact physical producer profile and indexed-target kind/owner semantics
   remain unqualified; this generated inventory gate does not qualify adapter
   graph closure or promote support.
   DWG entities are delivered before DBCOLOR OBJECTS callbacks; the adapter now
   reconciles generation-keyed RGB/name appearance, retains a bounded typed
   DBCOLOR source model, and refuses unresolved references (see the [late
   callback](docs/research/p4-dbcolor-late-callback-binding-2026-11-02.json)
   and [typed capture](docs/research/p4-dbcolor-typed-source-capture-2026-11-02.json)
   receipts). A generated AC1027 DWG authored and read by this same library now
   provides a fast self-consistency control for typed capture and the NOD's
   `ACAD_COLOR` root. It is not independent source evidence. A preflight gate
   refuses DBCOLOR output until an independent producer profile, exact
   `ACAD_COLOR` entry semantics, and fresh DXF readback are evidenced (see the
   [external-evidence gate receipt](docs/research/p4-dbcolor-external-evidence-gate-2026-11-03.json)).
   Research confirms that Autodesk exposes a color dictionary containing
   `AcDbColor` entries and that the root drawing dictionary uses the
   `ACAD_COLOR` key, while the Open Design Specification describes entity
   color references but does not specify the complete DBCOLOR object body or
   dictionary-key serialization. LibreDWG's object schema independently
   models DBCOLOR as common parent plus CMC color; this corroborates the field
   family, not producer interoperability. The DBCOLOR physical field span,
   actual dictionary key/value relation, cross-producer output semantics, and
   native color-book behavior therefore remain unqualified. No fixture is
   committed; the authored DWG exists only during the generated test.
   The `DICTIONARYWDFLT` default edge is checked as a hard pointer before it can
   become a typed reference; the generated wrong-role, rollback, and cursor
   alignment control is in the [default-handle role receipt](docs/research/p4-dictionary-default-handle-role-guard-2026-11-02.json).
   This does not establish target existence, ownership, or output closure.
   The shared OBJECTS group-330 owner emitter now uses resolver-first final IDs,
   with source-handle-remap fallback, and refuses unresolved nonzero owners.
   Generated ASCII/Binary controls cover both mappings and atomic refusal with
   corrected fresh-writer retry ([receipt](docs/research/p4-object-owner-reference-remapping-2026-11-03.json)).
   This does not establish target existence, ownership validity, or graph
   closure.
   LAYER's optional material hard pointer (group 347) now uses resolver-first
   output identity with source-handle-remap fallback; the generated ASCII/Binary
   control verifies both paths ([receipt](docs/research/p4-layer-material-reference-remapping-2026-11-03.json)).
   For 3DSOLID, the modeled group-350 history soft-owner reference now uses the
   same resolver/remap policy. Generated ASCII/Binary controls verify both
   output paths and atomic refusal of an unresolved nonzero history target;
   corrected output succeeds with a fresh low-level writer because failed
   `dxfRW` writes retain their sticky error state ([receipt](docs/research/p4-solid-history-reference-remapping-2026-11-03.json)).
   This does not establish history-target existence/role, source association,
   recursive graph closure, or ACIS semantics.
   The BLOCK and ENDBLK entity records now apply the same resolver-first,
   remap-fallback policy to their modeled material (347), visual-style (348),
   and plot-style (390) references. Focused generated ASCII/Binary controls
   verify exact emitted IDs and atomic refusal of unresolved nonzero targets
   ([receipt](docs/research/p4-block-reference-remapping-2026-11-03.json)).
   This closes low-level identity translation only; target kinds, existence,
   ownership, recursive closure, physical source profiles, and native behavior
   remain open.
   SECTIONOBJECT's modeled group-360 SECTION_SETTINGS hard-owner reference now
   also refuses zero/NoHandle resolver results before publishing a record;
   generated ASCII/Binary checks cover both invalid results and corrected retry
   ([receipt](docs/research/p4-sectionobject-settings-reference-remapping-2026-11-03.json)).
   This does not qualify source binding, target ownership/existence, or section
   semantics.
   SECTIONMANAGER's repeated group-330 soft pointers now use final resolver
   identities, with source-handle-remap fallback; output rejects invalid target
   IDs and a section-count/list mismatch before publication. Generated
   ASCII/Binary controls distinguish the list from common owner/reactor group
   330s and cover mapping, refusal, and fresh-writer retry
   ([receipt](docs/research/p4-sectionmanager-section-pointer-remapping-2026-11-03.json)).
   Autodesk documents these list entries as pointers to section entities, but
   source binding, target existence/owner, and complete graph closure remain
   unqualified.
   SECTIONSETTINGS nested source-object group-330 lists and destination-block
   group-331 links now emit resolver/remap identities; the legacy flat
   destination-block field follows the same rule. The writer rejects oversized
   source lists and invalid non-null results before record output. Focused
   generated ASCII/Binary checks cover nested and flat projections, resolver
   precedence, refusal, and fresh-writer retry
   ([receipt](docs/research/p4-sectionsettings-reference-remapping-2026-11-03.json)).
   The DXF schema establishes the pointer roles, but not source target
   existence, owner validity, complete SECTIONOBJECT/SECTIONSETTINGS graph
   closure, or native section-generation semantics.
   SECTIONMANAGER/SECTIONSETTINGS record identities now follow the source
   handle remap, reserve their emitted ID, and bind valid source object IDs after
   successful publication; newly authored no-source records receive an
   allocated identity. Generated ASCII/Binary checks verify group-5 record IDs,
   common owner group 330, and the body-reference projections
   ([receipt](docs/research/p4-section-record-identity-remapping-2026-11-03.json)).
   This does not prove that all section objects are emitted or close source-owner
   and generation-semantic gates.
   SUNSTUDY, MOTIONPATH, and ACDBCURVEPATH modeled links now also refuse
   unresolved nonzero output IDs before publishing records. Their low-level
   record scopes cover common-owner and write failures; generated ASCII/Binary
   controls exercise all eight references, destination preservation, and
   corrected fresh-writer retry ([receipt](docs/research/p4-scene-object-reference-remapping-2026-11-03.json)).
   Target kinds, source dependencies, physical producer profiles, and native
   scene semantics remain open.
   LEADER group 340 now keeps the whole-file preplanned annotation identity as
   its first choice, then supports the configured final-ID resolver or an
   explicit source-handle remap for direct record writes. Zero/NoHandle and
   unmapped references still refuse atomically. Focused generated ASCII/Binary
   controls cover resolver precedence over remap, invalid results, destination
   preservation, and the existing adapter's forward-reference/readback path
   ([receipt](docs/research/p4-leader-annotation-reference-remapping-2026-11-03.json)).
   This closes low-level identity translation only; target kind/existence,
   ownership, recursive closure, and native annotation semantics remain open.
   Direct `MULTILEADER` records now use the configured final-ID resolver for
   nested style, linetype, arrowhead, text-style, block, and legacy-reference
   handles when no whole-file write is active. Whole-file output retains its
   existing planned/remapped identities. Resolved IDs are cached once per
   source handle, and zero/NoHandle results refuse atomically. Generated
   ASCII/Binary checks cover resolver precedence across all nested reference
   families, duplicate-handle single resolution, and failure preservation
   ([receipt](docs/research/p4-mleader-direct-reference-resolver-2026-11-03.json)).
   This remains ID translation evidence only; target kind/existence, ownership,
   physical profile breadth, recursive closure, and native annotation semantics
   remain open.
   Continue the remaining reference-ID audit from the current field receipts,
   prioritizing typed owner/dependency pointers on supported 3D records. Confirm
   each field against its parser contract and applicable format mapping before
   editing; add focused ASCII/Binary controls for resolver precedence and remap
   fallback. Correct output IDs alone do not establish target kind, ownership,
   recursive closure, physical producer breadth, or native-consumer semantics.
   **BLOCK_RECORD BLKREFS residual:** `dx_iface::writeBlockRecords()` now passes
   the retained list to the low-level writer. Adapter preflight requires each
   handle to identify exactly one typed INSERT, match the target BLOCK_RECORD
   name, and have a valid source/output owner; finalization checks the remapped
   INSERT identity plus the emitted BLOCK_RECORD/BLOCK/ENDBLK identities. The
   bounded sorted-index lookup and ASCII/Binary wrong-target refusal control are
   recorded in the [adapter graph receipt](docs/research/p4-blockrecord-blkrefs-adapter-graph-closure-2026-11-03.json);
   low-level group-331 identity transport remains in the
   [BLKREFS transport receipt](docs/research/p4-blockrecord-blkrefs-reference-remapping-2026-11-03.json).
   This does not yet prove a valid adapter write/readback for the selected
   physical source, source-bound complete membership, producer breadth or native
   meaning. Autodesk's BLOCK_RECORD field table does not enumerate BLKREFS, and
   generic group-331 classification is not evidence of required membership
   semantics. Keep whole-document output unqualified until those gates close.
3. **Recursive closure:** follow only source-required edges reached from the
   admitted graph. Reuse a qualified provider when it closes the exact target
   role; otherwise resolve that source-required body and its descendants, and
   add only the missing edge/profile. Distinguish legal reference cycles from
   invalid ownership and enforce bounded work once.
4. **Failure behavior:** for each newly admitted graph profile, reject missing
   reachable bodies, wrong owners/types/roles, aliases and late mutation wherever
   they invalidate conversion. Verify whole-operation destination publication,
   caller-state preservation, temporary cleanup, and corrected retry at the
   narrowest selector that exercises the changed graph boundary.

**Proxy object-ID writer boundary:** the low-level DXF writer now preflights a
bounded ordered list of typed proxy references, shares the parser's DWG-handle
to-DXF-group role mapping, and has a per-proxy resolver overload requiring a
nonzero final handle for every edge. Generated ASCII/Binary controls cover
groups 330/340/350/360, target refusal, wrong-role refusal, destination
preservation, corrected retry, and typed readback
([object-ID transport receipt](docs/research/p4-dwg-proxy-entity-object-id-dxf-transport-2026-11-02.json)).
The explicit resolver overload is scoped to proxy object IDs; the generic
resolver also serves reactors, XDATA, and other records, so it is not used as
an adapter shortcut. `writeBlockRecords` now plans DWG pointer codes 4/5 and
offset soft-pointer codes 6/8/10/12 to unique top-level typed entities, mapping
them to DXF groups 330/340. It checks the source block container and owner,
rejects known OBJECTS/TABLES and raw-entity handle
collisions, preallocates each target handle, and passes only that closed map to
the proxy-scoped resolver. The collision inventory also covers retained
standard/raster/plot-settings/opaque frame maps, opaque source/output identity
sets, raster/standard ordinal and location indexes, and ACDS-history frames.
BLOCK and ENDBLK source handles are checked only after the full proxy reference
inventory is known, so a proxy in a later named block cannot bypass a
collision with an earlier block identity.
Finalization proves that every planned target and proxy was written once with
the expected type, section, owner and handle. Generated ASCII/Binary coverage
exercises missing targets, duplicate typed handles, collisions across those
source namespaces, unsupported owner-role edges,
preallocated targets omitted at emission, post-plan proxy-reference mutation,
an earlier BLOCK identity colliding with a target referenced by a later block,
atomic destination preservation, same-adapter retry and typed readback
([adapter graph receipt](docs/research/p4-dwg-proxy-entity-object-id-adapter-graph-closure-2026-11-02.json)).
The admitted adapter profile is deliberately limited to pointer codes 4/5 and
offset-pointer codes 6/8/10/12 targeting top-level typed entities. Owner-role
edges (350/360), unknown handle codes, nested subentities, raw targets, OBJECTS
targets, recursive target dependencies, physical producer support and native
semantics remain unqualified or refused. No support claim is promoted.

**Standalone PLOTSETTINGS boundary:** a generated AC1024/AC1027/AC1032 codec
vector now checks the modeled AcDbPlotSettings prefix fields, common owner/
reactor/extension-dictionary handles, plot-view/shade handle roles, and
truncated-handle reset/retry. The vector is encoder/parser consistency evidence;
it does not replace independent producer field-span/role proof. Current strict
AC1015 DWG-to-DXF output and a fresh DXF re-import also pass for one locally
generated writer profile: LibreDWG 0.14 independently reads NOD `0x0C` →
dictionary `0x5E` → `LIBDXFRW_PLOTSETTINGS_90` → PLOTSETTINGS `0x5A`, and the
current output preserves the decoded fields and reference roles. Because that
DWG was generated through the LibreCAD/libdxfrw writer path, this qualifies only
that bounded generated profile. The tracked `ordinary_enc_AC1015.dwg` remains
refused because both readers identify its extension-dictionary target `0x38` as
missing; do not synthesize it. Neither result qualifies external producer
breadth, native plotting, or whole-document closure. See the [codec round-trip
receipt](docs/research/p4-plotsettings-r2007plus-dwg-codec-decode-roundtrip-2026-11-02.json)
and [current strict readback receipt](docs/research/p4-plotsettings-ac1015-current-strict-readback-2026-11-02.json).
The low-level PLOTSETTINGS and LAYOUT writers now resolve their modeled shade-
plot, paper-space BLOCK_RECORD, last-active VIEWPORT, and UCS references to
final output IDs, with handle-remap fallback, and reject zero/NoHandle targets
before publishing records. LAYOUT now rolls back the record and writer state on
a late common-owner failure. Generated ASCII/Binary controls cover exact fields,
per-reference refusal, owner-failure rollback and corrected retry; see the
[reference-remapping receipt](docs/research/p4-layout-plotsettings-reference-remapping-2026-11-03.json).
This is transport only; it does not establish those target records' presence,
roles, ownership, or native plotting behavior.

**AC1018 DICTIONARYVAR isolation boundary:** the current physical selector first
refuses the unrelated `ET Border block` HATCH (handle `0x2BA`) because it has
`isGradient=0` with two retained stops. It verifies destination preservation,
clears only those stops in the local imported model, retries through the same
adapter, and verifies the zero-stop HATCH plus the DICTIONARYVAR graph in ASCII
and Binary output/readback. This qualifies the bounded DICTIONARYVAR graph only;
the HATCH stop profile remains refused under P6. See the [current selector
recheck receipt](docs/research/p4-ac1018-dictionaryvar-hatch-resolution-recheck-2026-11-02.json).

After actual prerequisites close, run one composed strict CLI checkpoint. A
successful process exit or record count alone is not closure. For AC1018 opt-in
capture, keep default legacy consumers opted out until codec, source, dependency and
output gates are met.

## P5 — rendering and TABLE

**FIELD output reference IDs:** `writeField` now rejects zero and
`DRW::NoHandle` for child hard-owner and object soft-pointer identities, and
preflights each remapped ID before emitting the FIELD record. Active type-64
CadValues require a positive source ID and a nonzero/non-sentinel output ID;
suppressed payload behavior is unchanged. FIELDLIST group-330 members receive
the same source/output ID validation. Generated ASCII/Binary controls cover
root and nested CadValues, group-360/group-331 FIELD vectors, FIELDLIST members,
invalid source IDs, invalid remap results, and destination preservation. See the
[output-ID guard receipt](docs/research/p4-field-reference-output-id-guard-2026-11-03.json).
This is a writer correctness guard only: it does not establish target existence,
target kind, recursive FIELD graph closure, physical producer breadth, or native
FIELD semantics. The disputed `AcObjProp.16.2` spans and child-role profiles
remain refused.

**Dynamic blocks:** the exact AC1024 oriented-grip and visibility-parameter DXF
readers now use `0xFFFFFFFF` for the absent `EvalExpr.parentId` default, matching
LibreDWG's importer initialization. The DXF stream still has no mapped field;
this in-memory default alignment does not establish equivalence or native
evaluation semantics. See the [default-normalization receipt](docs/research/p5-ac1024-dynamic-evalexpr-default-normalization-2026-11-01.json).
Qualify independent native evaluation for the existing two-root AC1024 graph.
Admit other physical subclasses only when a current source identifies a
required field, role, or output gap.

**Rendering:** close only the unqualified engine/class-revision/schema and
optional-presence profiles; establish source-backed active/global/environment
and background/live-section relations; then qualify native rendering semantics.
SUNSTUDY/MOTIONPATH/CURVEPATH/POINTPATH/OBJECTPTR physical producer coverage
and native plotting/scene semantics remain open; generated record-identity and
reference round-trips are transport evidence only (see the [SUNSTUDY receipt](docs/research/p5-sunstudy-record-identity-remapping-2026-11-03.json),
[scene-path receipt](docs/research/p5-scene-path-record-identity-remapping-2026-11-03.json),
and [path-carrier receipt](docs/research/p5-pointpath-objectptr-record-identity-remapping-2026-11-03.json)).
Keep physical SUN ownership constrained to the documented VPORT/VIEWPORT
hard-owner route. The AC1024 aerial sample's SUN `0x24C` is owned by XRECORD
`0x246`, so it is not evidence for a valid alternate source-owner route; keep
strict adapter/output refusal for that sample and reopen source-graph admission
only if a conforming source demonstrates a missing route ([outlier receipt](docs/research/p5-ac1024-sun-xrecord-owner-outlier-2026-11-01.json)).
The low-level SKYLIGHTBACKGROUND writer now translates a registered nonzero
group-340 SUN handle through the configured resolver or source-handle remap,
and refuses unresolved targets and pre-AC1021 output. Generated ASCII/Binary
controls cover exact reference readback, destination preservation, corrected
fresh-writer retry, and target-version refusal ([receipt](docs/research/p5-skylight-background-sun-reference-remapping-2026-11-03.json)).
This closes ID transport only: registration does not prove target emission, and
physical SUN owner/link qualification and native lighting semantics remain open.
The earlier direct-writer refusal is superseded only for this registered low-level
profile. For IBL backgrounds, physical producer coverage and native lighting
semantics remain unqualified. Generated reference and record-identity
round-trips do not close those gates; see the [IBL reference receipt](docs/research/p5-background-reference-remapping-2026-11-03.json)
and [record-identity receipt](docs/research/p5-background-record-identity-remapping-2026-11-03.json).
Unsupported RGB/name/book CMC states remain refused unless a lossless documented
route is established. Generated ASCII/Binary tests already cover selected
LAYOUT shade-mode 4/5 provider graphs and VIEWPORT shade-mode transport, including
target remapping and refusal controls; the two-profile AC1024 physical VIEWPORT
readback does not establish a producer carrying mode 4/5. Autodesk's published
shade tables remain 0..3, so reconcile any producer-backed mode 4/5 profile and
qualify its native meaning before promoting it. See the [shade transport
recheck](docs/research/p5-shade-consumer-transport-current-recheck-2026-11-01.json)
and [physical viewport receipt](docs/research/p5-ac1024-viewport-physical-output-readback-2026-10-31.json).
Generated Layer viewport-override XRECORD controls cover the four ezdxf-described
dictionary-key profiles as ASCII/Binary input, including group-335 VIEWPORT
references, group-343 LTYPE references and group-91 lineweight values. The
group-91/343 profiles round-trip with target remapping; groups 420/440 parse
and retain but remain atomically refused because Autodesk's XRECORD payload
envelope is 1..369 except 5/105. Keep the generic XRECORD domain unchanged.
This generated, non-normative profile does not qualify physical producers,
native override behavior, dictionary key collation, or the separate VIEWPORT
entity fields 335/343/344/91. The producer location and group-91 type remain
unresolved: Autodesk's VIEWPORT table lists all four as soft pointers, while
its generic group-code table types 90–99 as 32-bit integers; ezdxf's concrete
Layer-owned XRECORD profile instead uses 335 as the viewport handle, 343 as a
linetype handle, and 91 as a lineweight integer inside the XRECORD. Keep that
profile separate from direct VIEWPORT fields until an eligible producer sample
resolves the record location, types and target roles. See the [code-location
review](docs/research/p5-viewport-override-code-location-review-2026-11-02.json)
and [Layer override transport receipt](docs/research/p5-layer-viewport-override-xrecord-transport-2026-11-02.json).

The low-level VIEWPORT writer now uses resolver-first/fallback-remap output
identities for its modeled frozen-layer, background, shade-plot, clipping,
named/base UCS, visual-style and SUN references. A generated ASCII/Binary
control checks every field and preserves the explicit common-entity group-348
null carrier beside the viewport's nonnull visual-style reference. Nonzero
references now refuse zero/NoHandle output identities before record emission;
per-edge refusal and corrected retry are covered in both encodings; see the
[reference-remapping receipt](docs/research/p5-viewport-reference-remapping-2026-11-03.json).
This closes only low-level ID translation. Physical producer/target-role proof,
strict graph closure and native VIEWPORT rendering remain open, as do the
separate ambiguous VIEWPORT 335/343/344/91 fields described above.

The VIEW writer now translates its admitted Background, VisualStyle, named UCS
and base UCS references through the configured resolver or handle-remap
fallback, and refuses zero/NoHandle resolutions before record output. Generated
ASCII/Binary checks verify exact output IDs, per-edge refusal, corrected retry,
and the VIEW table-row identity; see the [VIEW reference receipt](docs/research/p5-view-reference-remapping-2026-11-03.json).
The writer continues to refuse SUN and live-section references without a
qualified owner/dependency route. These controls prove ID translation only;
physical target/owner closure and native rendering remain open.

The VPORT table writer now translates its modeled named/base UCS, Background,
VisualStyle and SUN references through the configured resolver or handle-remap
fallback, retaining the existing source-registration checks for Background and
SUN. Emitted nonzero targets now fail closed on zero/NoHandle identities.
Generated ASCII/Binary controls verify exact values, per-edge refusal, retry,
and the VPORT row's remapped identity; see the [VPORT reference receipt](docs/research/p5-vport-reference-remapping-2026-11-03.json).
This is serializer transport only. The nonzero VPORT shade-plot handle remains
refused, and physical target/owner closure and native rendering are open.

The ACAD_TABLE writer now resolves both modeled hard pointers—TABLESTYLE group
342 and owner BLOCK_RECORD group 343—to final output IDs, with source-handle
remap fallback. Generated ASCII/Binary controls cover exact values and atomic
refusal of either unresolved target, followed by corrected output from a fresh
writer ([receipt](docs/research/p5-table-reference-remapping-2026-11-03.json)).
This is low-level identity transport only; it does not establish target record
emission, ownership consistency, recursive graph closure, physical producer
breadth, or native table behavior.

Remaining implementation/evidence work includes native dictionary collation,
eligible physical source profiles for the layer overrides, independent native
consumer evaluation, any narrowly scoped producer-backed 420/440 compatibility
route, and source-backed VIEWPORT entity references 335/343/344/91. For the
direct VIEWPORT fields, first resolve Autodesk's group-91 type conflict and
distinguish entity fields from Layer-owned XRECORD payload tags; the [code-
location review](docs/research/p5-viewport-override-code-location-review-2026-11-02.json)
records the current evidence. Verify coupled dictionary keys, names, body
fields, target roles and consumers before promoting support.

**VIEWPORT layer-override reference census:** The nine available local AC1024
DWGs were converted and inspected with LibreDWG 0.14. Its DXF outputs contain
16 VIEWPORT records and none of codes 335/343/344/91. Its direct JSON exposes
12 VIEWPORT records across the parseable files, with no corresponding override
members. This is not evidence that the DWGs lack those fields: the peer may
omit them, one JSON output is malformed, and two parseable inputs expose no
VIEWPORT records. Do not implement a guessed mapping or promote support from
this absence; obtain an eligible source with the physical references, target
roles, coupled layer/dictionary records and native behavior. See the [local
census receipt](docs/research/p5-viewport-layer-override-local-corpus-census-2026-11-01.json).

**VIEWPORT group-46 field isolation:** Autodesk documents group 72 as the
circle-zoom field and does not list group 46 for VIEWPORT. The parser now leaves
group 46 on the tolerant unknown-code path, so it cannot overwrite `circleZoom`;
a generated ASCII control verifies that group 72 value 725 survives a following
group 46 value 2500. This is field-isolation evidence only, not physical producer
or native-rendering qualification. See the [field-isolation receipt](docs/research/p5-viewport-undocumented-group46-isolation-2026-11-02.json)
and [Autodesk's VIEWPORT DXF table](https://help.autodesk.com/cloudhelp/2025/ENU/AutoCAD-DXF/files/GUID-2602B0FB-02E4-4B9A-B03C-B1D904753D34.htm).

The three locally available AC1021 strict-conversion witnesses that previously
stopped at LAYOUT viewport-list validation now pass that check after typed
entity space is derived from its validated model/paper BLOCK_RECORD owner.
They then stop at `VISUALSTYLE carrier coverage`. This is the expected
fail-closed result: the DWG-origin DXF writer admits only the complete AC1024
VisualStyle source profile, while these AC1021 callbacks have no exact DXF body
mapping. Keep output refused; any wider route requires source-span/field
mapping and output/readback evidence. The bounded correction and peer
differential are recorded in the
[AC1021 space-classification receipt](docs/research/p4-ac1021-space-from-block-record-2026-11-03.json).
Strict output still fails, and no support claim is promoted.

**TABLE:**

1. **Nested FIELD graphs:** qualify physical producer/version profiles and
   source gaps; do not infer native FIELD evaluation from generated values.
   Use the existing `p5-ac1027-*` receipts, both type-0 transport receipts,
   the [AC1021 flags=4/type=8 receipt](docs/research/p5-ac1021-field-format-flag4-root-type4-8-transport-2026-11-01.json)
   and the [AC1021 flags=4/type=2 census receipt](docs/research/p5-ac1021-field-flags4-type2-root-census-2026-11-01.json)
   as exclusion inventories: the [payload-present receipt](docs/research/p5-field-kunknown-bl-transport-2026-11-01.json)
   covers type-0 BL payloads; the [payload-suppressed receipt](docs/research/p5-field-kunknown-suppressed-transport-2026-11-01.json)
   covers only generated R2007+ FIELD type-0 transport with low format flags and
   no modeled payload. Keep FIELD suppression separate from TABLE payload
   handling: the AC1021 TABLE flags=2/type=2 checksum profile is source-backed
   in the [TABLE CadValue receipt](docs/research/rpp5ag-cadvalue-framing-and-native-table-retention-fast-slice-2026-10-22.json).
   Physical AC1021 FIELD flags=2/type=2 checksum children now follow the
   generic present-payload rule: ODA §20.4.99 says flag bit 1 (`0x01`) suppresses
   the type-specific value, and current LibreDWG master uses the same mask.
   The physical census confirms all 71 `ACFD_FIELDTEXT_CHECKSUM` children decode
   as finite BD values and their containing FIELD records are complete. The
   superseding [resolution receipt](docs/research/p5-ac1021-field-flags2-checksum-payload-resolution-2026-11-02.json)
   corrects the earlier reader-conflict interpretation; retain the older
   [conflict receipt](docs/research/p5-ac1021-field-flags2-child-reader-conflict-2026-11-01.json)
   as historical evidence. The generated vector verifies both complete payload
   consumption and refusal of an additional unexplained 34-bit tail, followed
   by successful same-object correction retry. No DWG/DXF fixture was added.
   This closes only the checksum-child span profile, not physical FIELD
   output/readback or native evaluation. A separate generated strict DXF
   control now writes the exact flags=2/type=2 `ACFD_FIELDTEXT_CHECKSUM`
   child profile in ASCII and Binary; an AC1021 core-writer vector checks the
   group-93/group-140 emission. It refuses the same value under an
   unqualified key without replacing the destination, retries after correction,
   and verifies fresh typed readback. Its [transport receipt](docs/research/p5-ac1021-field-checksum-generated-dxf-transport-2026-11-02.json)
   records the generated-only boundary. The pre-write root checks use the
   requested target version; reading `dxfW->getVersion()` there used an unset
   codec version and incorrectly rejected the qualified target. This still
   does not establish physical DWG source-to-output or native FIELD semantics.
   The type-8 receipt records
   the group-98 wire-type
   disagreement and unresolved child-count framing for type-4 roots. For the
   physical AC1021 FIELD population, 15 `AcExpr` roots parse as complete; the
   71 checksum children are also now complete, leaving 56/142 records raw-only.
   The 42 peer-selected
   `AcObjProp` handles match local raw-only shells. Their object-data suffixes
   remain unexplained:
   41 records leave 258 bits and handle `0x1481E` leaves 210 bits. Do not mark
   these records complete or enable source-profile output. Resolve the suffix
   schema, child type disagreement and target roles against ODA and an eligible
   independent source. Neither receipt
   resolves other flags, child-count/framing failures, the full physical AC1021
   FIELD population or native evaluation.
   Preserve unsupported compositions with explicit refusal. Native expression
   evaluation and TABLE rendering remain separate semantic qualifications.
2. **Block-cell geometry:** implement payloads only when a source or consumer
   witness identifies missing fields or semantics. Bind the exact field layout
   and semantics to that witness; keep unsupported payloads refused while
   preserving current block-reference and ATTDEF ownership validation.
3. **Standalone TABLEGEOMETRY cache transport:** the AC1027 witness at
   `TABLEGEOMETRY 0xC95` and ACadSharp's AC1021/AC1024 samples now support a
   bounded DXF writer profile for AC1021 through AC1032: observed cell flags
   3/4/7, null cell soft pointers, planar coordinates, zero unknown content
   values, and no unrepresented common-object metadata. The generated fast
   test checks both witnessed flag profiles, ASCII/Binary raw groups and typed
   readback, out-of-profile refusal, destination preservation, and same-exporter
   retry. See the [AC1027 differential and transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json)
   and the [ACadSharp flag-profile receipt](docs/research/p5-acadsharp-tablegeometry-flag-profile-2026-11-01.json).
   This is isolated cache transport only: the generated owner is synthetic,
   the source graph is not yet closed through strict output, and neither native
   TABLE rendering nor cell-geometry semantics are qualified. The AC1027
   sample's final content value is zero; its DWG bit-stream type remains
   unresolved because ODA §20.4.103 says BD while the current parser reads BL.
4. Use the existing `p5-ac1027-*` receipts as the exclusion inventory. Implement
   only distinct uncovered source- or consumer-backed CONTENTFORMAT and
   CELLSTYLEMAP combinations, extended CMC/name-color or linetype profiles, and
   source-backed precedence for overlapping TABLECONTENT edge declarations.
   Keep unsupported combinations refused until schema, version gates, wire order
   and readback behavior are established.
5. The AC1018 text-cell reader maps the ODA §20.4.96.1 text-height and four
   physical-edge color/lineweight/visibility field sets into `DRW_TableCell`.
   It admits only indexed CMC states, bounded lineweights, Boolean visibility,
   matching edge flags and nonvirtual selected edges. Generated controls cover
   all four fields in wire order and preserve body cursor/retry behavior.
   Strict DXF output supports these fields only where the legacy group-177 mask
   fits its 16-bit range; it requires every selected visibility value and
   rejects higher masks atomically. The [implementation receipt](docs/research/p5-ac1018-table-cell-four-edge-overrides-2026-11-02.json)
   records ASCII/Binary group emission and readback. This is not physical
   producer or source-to-output proof: the local `ET-Drawing-with-Border.dwg`
   hash differs from the earlier paired-DXF receipt and has no TABLE callback
   carrying this profile. Find a byte-bound physical producer witness, then
   bind its callback, typed fields, output and fresh readback. Qualify native
   rendering separately; do not infer visibility defaults from field transport.
6. Qualify native TABLE rendering independently. This gate concerns consumer
   geometry, merge visibility and rendered style; serializer/readback evidence
   remains scoped to its witnessed source profiles and does not close this gate.
7. **AC1027 source TABLECONTENT span discrepancy:** on the public
   `example_2013.dwg` source, object `0xC94` (type 529) parses its 9 columns and
   20 rows, then fails at row 0/cell 0's first ATTDEF reference. ODA §20.4.97
   specifies that reference as a soft pointer. The bounded probe reads
   attribute count 208 at body bit 990/string bit 18176; the first handle starts
   at handle bit 115929 and decodes as hard-pointer code `0x05`, reference
   `0x11` (end bit 115945), where soft-pointer code `0x04` is expected.
   The preceding custom key is `ACAD_ROUNDTRIP_2008_CELL_CHECKSUM` (string
   bits 17556–18094); its flags=2/type=2 value spans body bits 840–928, followed
   by a flags=6/type=4 cell value at body bits 950–980/string bits 18098–18176.
   LibreDWG 0.14's paired DXF independently identifies handle `0x11` as a
   `STYLE`, not an `ATTDEF`; this confirms the current cursor is on the wrong
   record and does not identify the intended target. Treat this as unresolved
   stream alignment, not proof that the producer violates ODA. Keep
   the TABLECONTENT frame incomplete and strict output refused until the prior
   body/string/handle spans and the target role are reconciled against an
   independent decoder that implements this class. LibreDWG 0.14 reports the
   object as `UNKNOWN_OBJ`, so it does not adjudicate the schema. A pinned
   ACadSharp reader independently opens the same file and reads the 20×9
   `TABLE` entity, but handle `0xC94` is absent from its document; its
   `readTableContent` path is called only for embedded `TABLE` entity data, not
   this standalone type-529 object. It cannot adjudicate this span. A separate
   scratch attempt to enable LibreDWG's unstable TABLECONTENT decoder also
   failed as an adjudicator: it required a forced class route and ODA-guided
   schema edits. Its initial non-Boolean `has_linked_data` value came from a
   stale April 2026 scratch checkout using `format_flags & 3`; current LibreDWG
   master and ODA use `format_flags & 1`. Re-running the scratch copy with that
   mask makes the value Boolean 0, but wrapped string offsets, an attribute
   count of 208, the unrelated STYLE handle, handle-buffer overflows and a
   process trap remain. Current master still routes standalone TABLECONTENT as
   unstable, so this does not qualify it as an independent decoder. Preserve
   the result as a decoder limitation, not independent wire proof; see the
   [scratch decoder limitation receipt](docs/research/p5-ac1027-tablecontent-scratch-libredwg-decoder-limits-2026-11-02.json).
   The source and all traces remain external to the repository; see the
   [source-bound span
   mismatch receipt](docs/research/p5-ac1027-tablecontent-attdef-handle-span-mismatch-2026-11-02.json).

**TABLE CadValue flag scope:** ODA §20.4.99 uses one-based bit names: bit 1
(`0x01`) marks `kGeneral` and suppresses the type-specific payload; setting
only bit 2 (`0x02`) does not suppress it. The AC1021 flags=2/type=2 checksum payload
therefore follows the ordinary present-payload rule; its custom key records
source lineage but does not require a parser exception. The AC1021 flags=4/6,
type=4 native-string carrier has a distinct body encoding. Generated vectors
cover flags 0–3 and following-field alignment. The current flag-numbering
recheck is recorded in the
[flag-scope receipt](docs/research/p5-table-cadvalue-oda-flags-and-ac1021-checksum-scope-2026-11-02.json).
It does not resolve the AC1027 TABLECONTENT span discrepancy, qualify native
TABLE semantics, or promote any support claim.

**Legacy FIELD DXF flag guard:** AC1018 serialization now refuses nonzero root
or child CadValue format flags before emitting bytes. Autodesk's FIELD DXF
group-code table identifies group 93 as the data-set count and lists no
CadValue format-flags carrier. The focused ASCII/Binary control verifies
atomic refusal and same-writer correction retry, while the AC1021 checksum
control still emits flags=2 with its BD value. This generated representability
guard does not qualify physical legacy FIELD prevalence, source-to-output
conversion, or native evaluation; see the [guard receipt](docs/research/p5-field-legacy-dxf-format-flag-guard-2026-11-02.json).

## P6 — residual semantic and version audit

Audit only a current failing source/profile; do not revive historical defects whose
receipt and code path already agree.

| Boundary | Remaining decision or proof |
| --- | --- |
| Version/route parity | Finish the standalone versus pinned LibreCAD/bundled/LibreCAD_3 route differential. Establish authoritative DXF fields for the derived `AcDbAssocValueDependency` body, validate it with a reader that implements that subclass, and establish native acceptance for the associative profile. See [the P6 differential receipt](docs/research/p6-libre-dwg-associative-dxf-import-differential-2026-10-31.json). |
| DIMASSOC DWG-to-DXF transport | AC1018 inline-text and AC1021+ detached-string readers map bounded main/intersection fields into the existing DXF model, with fail-closed handling for unknown classes and cardinalities the model cannot represent. The opt-in physical-source harness verifies resolved source entity references and ASCII/Binary writer output plus typed readback: 23/23 AC1018 `Extruder2.dwg` records and 6/7 AC1021 architectural-sample records passed; the remaining AC1021 record carries unrepresentable detail and stays refused. A generated AC1024 vector independently checks owner/reactor/dimension/main/intersection remapping and all modeled fields. LibreDWG's current `dwg2.spec` declares the same ordered sequence; on AC1018, 23 libdxfrw callbacks/33 refs matched LibreDWG 0.14 minJSON for handles, types, subentity/GS values, points, last-point flags and the intersection edge, with one trace-rounded near-parameter difference. These prove selected-record transport only. AC1018 whole-file output remains refused by 474 entity parse failures; AC1021 whole-file output remains refused by 36 MLINE proxy caches. ODA v5.4.1 does not document this body, and native associative behavior remains unqualified. Detailed DWG encoding remains refused. See the [DIMASSOC transport receipt](docs/research/p6-dimassoc-dwg-detail-to-dxf-transport-2026-11-02.json). |
| DWG source profiles | **AC1027 Cover:** resolve the 731-byte suffix after the HEADER end sentinel. ODA v5.4.1 defines the CRC/sentinel boundary; LibreDWG 0.14 parses the known handle stream through byte 815, before the envelope trailer at byte 826. Neither source explains bytes 844–1575. Find producer-backed layout evidence or retain the suffix as unclaimed and strict-complete output refused; do not assume it is padding or corruption. Continue independent per-version reader/container/HEADER/CLASSES and producer qualification only for uncovered profiles, including R2007+ carrier differences and class counts. The exact AC1027 app-maintenance-29 HEADER envelope in the public `example_2013.dwg` witness now passes header parsing; do not reopen this envelope without a new failure. After the bounded TABLEGEOMETRY writer was added, a strict AC1024-target replay passed that object and then refused at ModelDocViewStyle serialization because it requires the exact source DWG version. An AC1027-target replay was refused earlier because the strict read disposition remains incomplete (three object parse failures and one orphan DataStorage record). This does not close AC1027 producer, SPLINE or whole-output qualification; see the [TABLEGEOMETRY transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json). See the [exact HEADER receipt](docs/research/p6-ac1027-exact-header-size-exclusion-2026-11-01.json). The alleged AC1024 CLASSES high-size read failure is not reproduced in the current local corpus: all nine available AC1024 inputs reach `dwgReader24::readDwgClasses END` and report `operationSucceeded=true`, `legacyError=0`, and no CLASSES CRC mismatch. Only `mechanical_example-imperial.dwg` exits successfully and publishes a strict DXF; the other eight exit nonzero without publishing a DXF. Explicit strict callback-finalization failures are present for the architectural and two title-block samples, and the condominium and conference-room samples report three object parse failures each; these later outcomes are not CLASSES failures. The exact 2026-11-03 command, source/tool fingerprints, inputs, trace hashes, and per-file outcomes are recorded in the [current AC1024 corpus rerun receipt](docs/research/p6-ac1024-classes-current-corpus-rerun-2026-11-03.json); the earlier extended-size and synthetic-footer selector remains in the [prior CLASSES recheck](docs/research/p6-ac1024-classes-extended-size-current-corpus-recheck-2026-11-01.json). Do not reopen this parser path without a new failing sample; the local drawings were not added to the repository. See also the [current Cover receipt](docs/research/p6-ac1027-cover-reader-success-output-refusal-2026-11-01.json). The recorded ODA File Converter 27.1.0 GUI run accepts this source and rewrites it as AC1032, but does not explain the 731-byte AC1027 suffix. The latest current-code AC1032 trace reaches the CLASSES end marker and parses all 115 frames with no entity/object parse failures or DataStorage link/orphan failures: the bounded AC1027-origin alternate schema-index table-role profile is retained in the resave. Strict-complete output still refuses before output preparation because HEADER reports a compatibility warning at file offset 16 for the zero high-size word (declared size 802; payload end 826 versus 822 if the word were included) and an unclaimed-data warning at offset 844 (739 suffix bytes through 1583); no DXF was created. Both suffixes remain unexplained, so keep strict-output refusal. See the [AC1032 DataStorage profile recheck receipt](docs/research/p6-ac1027-cover-ac1032-datastorage-role-profile-2026-11-02.json) and the [ODA resave/retry receipt](docs/research/p6-ac1027-cover-oda-resave-recheck-2026-11-02.json). The installed converter's post-unlock retry still fails strict code-signature verification and aborts with PasteBoard/XPC errors, producing no new output; do not bypass signature checks. A fresh unlocked-session verification at 2026-11-03 11:24 UTC again fails for the unchanged executable; no invocation was attempted. |
| TABLEGEOMETRY DWG field encoding | ODA v5.4.1 contradicts itself: §20.4.98 calls the final shared cell-geometry field BL, while §20.4.103 lists the standalone TABLEGEOMETRY group-95 field as BD. LibreDWG's 2020 schema change and the pinned acadrust writer use BL for the shared geometry payload. A wider local corpus scan covered 184 unique AC1021+ DWGs; nine readable files contained 14 TABLEGEOMETRY objects, all with final value zero. A generated AC1021 direct-parser vector now checks nonzero BL `0x12345`, the geometry fields before it, cell framing and exact following content-handle resolution to `0xA720` (`libdxfrw_tablegeometry_nonzero_bl_tail`). This synthetic vector protects the bounded-BL parser but cannot adjudicate the standalone source encoding. Keep the parser unchanged and retain the standalone source-span gate until a traceable independent producer yields a nonzero value. Do not conflate this DWG-stream question with DXF group-95 int32 output mapping or native TABLE semantics; see the [group-95 discrepancy/census receipt](docs/research/p6-tablegeometry-group95-profile-census-2026-11-02.json), [AC1027 transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json), and [ACadSharp profile receipt](docs/research/p5-acadsharp-tablegeometry-flag-profile-2026-11-01.json). |
| HATCH/MPOLYGON/MESH | Do not claim AC1018 HATCH `0x173` or ET Border HATCH `0x2BA` inactive-gradient stops as supported: both retained-stop profiles remain refused. The physical ET Border profile has a bounded DICTIONARYVAR-only selector that verifies atomic HATCH refusal, explicit local stop removal, and corrected object-graph output/readback; it does not qualify the stops. See [the selector recheck receipt](docs/research/p4-ac1018-dictionaryvar-hatch-resolution-recheck-2026-11-02.json). After explicit stop removal, generated HATCH/MPOLYGON controls and the selected physical HATCH record have bounded ASCII/Binary output/readback coverage for representable companion metadata and boundary fields. This does not establish the stops' native meaning or whole-file output; see the [loss-guard and selected-source transport receipt](docs/research/p6-ac1018-hatch-inactive-gradient-stop-dxf-loss-guard-2026-11-02.json). `DRW_Hatch::boundaryPointToWcs` and `boundaryVectorToWcs` expose the Autodesk OCS/elevation transform for HATCH/MPOLYGON boundaries without changing the stored 2D model or callback behavior; this is coordinate mapping evidence only, not native geometry qualification. See the [HATCH OCS mapping receipt](docs/research/p6-hatch-ocs-wcs-coordinate-mapping-2026-11-02.json). Generated ASCII/Binary HATCH controls enforce base-HATCH group 70/71 as 0/1, group 75/76 as 0..2, and group 77 as 0/1 in the parser; typed writer admission also bounds groups 75/76. MPOLYGON retains its distinct group-70 version and group-71 fill meanings. See the [Boolean-domain receipt](docs/research/p6-hatch-boolean-dxf-domain-2026-11-02.json) and [pattern-domain receipt](docs/research/p6-hatch-pattern-dxf-domains-2026-11-02.json). MPOLYGON group 99 now requires a nonnegative count within the existing 10,000-item parser budget at both reader and typed-writer admission; invalid values preserve the model/destination and corrected same-writer retry plus typed readback pass. Autodesk defines the field as a count of ignored degenerate borders but gives no relation to group 91, so no such relation is inferred and this scalar guard does not qualify native geometry semantics; see the [group-99 domain receipt](docs/research/p6-mpolygon-degenerate-count-domain-2026-11-02.json). MPOLYGON group 63 is now bounded to Autodesk's 0..256 CMC/ACI index domain on parse, typed validation and writer admission, with ASCII/Binary atomic refusal, same-writer retry and typed readback; this scalar guard does not qualify fill rendering (see the [ACI domain receipt](docs/research/p6-mpolygon-aci-dxf-domain-2026-11-02.json)). Generated HATCH gradient parsing now rejects explicit RGB outside 0..0xFFFFFF and ACI outside 0..256 before mutating the current stop; callback validation applies the same scalar bounds, and generated fast controls cover endpoints, preserved state and corrected same-group retry. This validates scalar transport only, not gradient rendering (see the [gradient stop color-domain receipt](docs/research/p6-hatch-gradient-stop-color-domain-2026-11-02.json)). MESH crease values are restricted to finite -1 or nonnegative values; generated parser/writer controls accept a shorter edge-order crease vector. Autodesk identifies group 95 as the level-0 edge-crease count, and ACadSharp's independent DWG reader applies each value to the same-index edge. The count is bounded by level-0 edge count at DXF/DWG read and write admission to prevent nonexistent edge references. A focused generated AC1024 DWG vector now exercises valid one-edge/one-crease parsing, malformed one-edge/two-crease refusal, state clearing, and corrected same-object retry; see the [reader-guard receipt](docs/research/p6-mesh-dwg-crease-count-reader-guard-2026-11-02.json). This establishes bounded positional transport only; physical producer breadth, native crease effect, subdivision, geometry and annotation semantics remain open. See the [MESH crease and edge-count receipt](docs/research/p6-mesh-crease-value-domain-2026-11-02.json). Other witnessed zero-fit/producer layout disagreements, extended CMC meaning and unrepresented overrides remain; qualify boundary geometry and annotation behavior beyond framing. |
| Text/annotation/coordinates | Qualify whole-document conversion and native dimension/text display; generated ASCII/Binary DXF reader controls now cover `applyExtrusion` conversion of TEXT and INSERT-owned ATTRIB insertion/alignment points, INSERT insertion points, and the OCS text points of all seven documented standard DIMENSION subtypes (0–6), including selected clone/arc points for linear and angular records, while preserving their WCS definition points. These controls do not establish native orientation/display or physical-producer breadth. See the [TEXT/ATTRIB receipt](docs/research/p6-dxf-text-attrib-ocs-extrusion-2026-11-02.json), [INSERT OCS receipt](docs/research/p6-dxf-insert-ocs-extrusion-2026-11-02.json), the original [linear/angular DIMENSION receipt](docs/research/p6-dxf-dimension-ocs-extrusion-2026-11-02.json), and the [standard-subtype coverage receipt](docs/research/p6-dxf-dimension-standard-subtypes-ocs-2026-11-02.json). Generated AC1027 ASCII/Binary MLEADER text-content and block-content profiles use the existing adapter serializer after strict owner/style/reference closure. Typed readback verifies selected context/root/line fields, remapped style/text-style/LTYPE/BLOCK_RECORD roles, the block transform, and the referenced content BLOCK_RECORD body. Unrepresentable CMC data remains an atomic refusal with same-adapter correction/retry. This is generated transport evidence only; physical producer profiles and native annotation semantics remain open. See the [text-content adapter receipt](docs/research/p6-mleader-selected-adapter-dxf-transport-2026-11-02.json) and [block-content adapter receipt](docs/research/p6-mleader-block-content-adapter-dxf-transport-2026-11-02.json). A non-mutating typed view now exposes the pre-R2018 ACAD_MTEXT_COLUMN_INFO, ACAD_MTEXT_COLUMNS and ACAD_MTEXT_DEFINED_HEIGHT XDATA brackets while retaining raw XDATA, separate linked MTEXT callbacks and the distinct metadata, height-list, declared-link and actual-handle counts; generated ASCII/Binary round trips also verify remapping. This does not qualify source-producer breadth, linked-column aggregation, or native layout/rendering; see the [typed-view receipt](docs/research/p6-legacy-mtext-column-xdata-typed-view-2026-11-02.json). Continue qualifying those consumer and graph semantics outside the bounded R2018 embedded ATTRIB/ATTDEF context; other ATTRIB/ATTDEF variants; native dimension display, nonstandard DIMARC/LARGE_RADIAL placement, and DSTYLE/picture graphs; SHAPE glyph/font identity; other LEADER/MLEADER profiles and native annotation semantics; and derived-R12 appearance. Do not reopen the selected large-radial picture-block or R2018 embedded-MTEXT parser profiles without a new regression; see the [picture-block graph receipt](docs/research/p6-ac1032-large-radial-picture-block-resolution-2026-11-02.json) and [R2018 embedded-MTEXT receipt](docs/research/p6-ac1032-r2018-embedded-mtext-context-2026-11-02.json). |
| Geometry and clipping | R2013+ DWG fit SPLINEs with non-default square-root-chord or uniform knot parameterization now fail closed on DXF write because DXF has no selector and the choice changes curve shape; the documented chord default remains writable. A separate hand-encoded S6.2 AC1027/AC1024 parser control checks the versioned BL fields and following-sentinel alignment; the read-only local reference scan found AC1021 scenario-1/2 and AC1024 scenario-2 parser-smoke traces, but producer provenance is unverified and strict CLI export published no DXFs. This scan found no additional AC1027/AC1032 SPLINE record; the separate public AC1027 `example_2013.dwg` field differential remains bounded to its recorded fields and does not qualify whole-file support. Keep authentic per-version qualification open (see the [local corpus receipt](docs/research/p6-spline-local-reference-corpus-scan-2026-11-02.json), [AC1027 differential receipt](docs/research/p6-ac1027-fit-spline-field-differential-2026-11-01.json), and [boundary-test receipt](docs/research/p6-spline-r2013-bitlong-boundary-fast-test-2026-11-02.json)). ASCII/Binary refusal and corrected retry are covered. A generated ASCII/Binary reader control now covers classic 2D POLYLINE OCS conversion using the owning elevation while preserving 3D POLYLINE WCS vertices under `applyExtrusion`; this is not physical-producer or native-renderer qualification. See the [classic POLYLINE OCS receipt](docs/research/p6-dxf-classic-polyline-ocs-extrusion-2026-11-02.json). Generated ASCII/Binary controls also verify opt-in conversion of PDF/DGN/DWF UNDERLAY insertion positions from OCS/ECS to WCS while retaining local rotation and 2D clipping coordinates; no physical producer or native consumer claim follows. See the [UNDERLAY OCS receipt](docs/research/p6-dxf-underlay-ocs-extrusion-2026-11-02.json). Autodesk's SubDMesh API crease domain is now enforced at DXF/DWG read and write admission: -1, 0, or any positive finite value; generated controls cover negative-value refusal and the all-version DWG writer boundary. This closes only the scalar domain; crease-to-edge association and subdivision behavior remain unqualified. See the [MESH crease-domain receipt](docs/research/p6-mesh-crease-value-domain-2026-11-02.json). Curve equivalence/native interpretation, other SPLINE/NURBS/ELLIPSE/OCS/polyface/mesh semantics, remaining completed-pool/domain validation, physical SPATIAL_FILTER producer profiles, actual clipping application and native consumer semantics remain open. Generated local DXF writer admission now rejects self-intersecting SPATIAL_FILTER polygons per Autodesk API requirements; this is field-admission evidence only. See the [polygon guard receipt](docs/research/p6-spatial-filter-simple-polygon-dxf-guard-2026-11-02.json). See the [knot-parameterization guard receipt](docs/research/p6-ac1027-spline-fit-knot-parameterization-dxf-guard-2026-11-02.json). |
| Units and inheritance | Generated ASCII/Binary LTYPE controls enforce Autodesk's fixed-A group-72 alignment, signed-16-bit group-75 shape-number domain, and group-70 standard flags (16/32/64, with 32 requiring 16) on typed write admission; parsing preserves unsupported values. See the [group-72 receipt](docs/research/p6-ltype-alignment-dxf-domain-2026-11-02.json), [group-75 receipt](docs/research/p6-ltype-shape-number-dxf-width-2026-11-02.json), [group-70 receipt](docs/research/p6-ltype-standard-flags-dxf-domain-2026-11-02.json), and [group-74 read-tolerance receipt](docs/research/p6-ltype-element-flag-read-tolerance-2026-11-02.json). The group-74 parser retains nonnegative signed-16-bit values outside its modeled writer range; no interpretation or support claim follows. LTYPE description and complex-dash strings already share the retained-byte lease, with replacement, reset, copy/move, over-budget refusal, and corrected retry covered by the [retained-text receipt](docs/research/ltype-retained-text-owner-fast-slice-2026-10-17.json); do not reopen that implementation. Typed LTYPE output now refuses group-9 complex text above 255 encoded bytes after DXF version/code-page conversion, with ASCII/Binary boundaries, Unicode conversion, readback, whole-record rollback and corrected writer retry covered by the [group-9 guard receipt](docs/research/p6-ltype-group9-encoded-length-guard-2026-11-02.json). Autodesk's 2027 ObjectARX table gives group 9 a 255-character maximum (less for Unicode), while its 2024 group-value-type table gives codes 0-9 a 2049-single-byte-character allowance from AutoCAD 2000 onward; the direct-field/group-range references conflict. The 255-byte writer bound is a conservative interim admission rule, not proof of the maximum valid producer value; independent producer/native acceptance and the larger-string profile remain unqualified. DXF HEADER output now refuses explicit zero or non-finite `$LTSCALE` while retaining tolerant input and the absent-value default of 1.0; ASCII/Binary refusal, read retention and same-writer correction retry are covered by the [LTSCALE receipt](docs/research/p6-ltscale-header-zero-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls now also reject malformed explicit `$CELTSCALE` group/type, conflicting aliases, and non-finite values instead of silently replacing a wrong-typed value with the absent default 1.0; finite positive, zero and negative values remain writable because Autodesk documents no sign/zero domain. Tolerant input, ASCII/Binary rollback and corrected retry are covered by the [CELTSCALE receipt](docs/research/p6-celtscale-header-field-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls also enforce the documented 0/1 domains for `$PSLTSCALE` and `$MSLTSCALE`, retain unsupported values on input, keep the absent PSLTSCALE default of 1, leave absent MSLTSCALE omitted, and refuse MSLTSCALE for pre-AC1021 output. Autodesk dates MSLTSCALE to AutoCAD 2008, but AC1021 spans AutoCAD 2007–2009, so the current target enum cannot distinguish that product-version boundary. See the [linetype display-scale receipt](docs/research/p6-linetype-display-scale-dxf-domain-2026-11-02.json). These are field-admission controls only; native linetype appearance remains unqualified. Generated ASCII/Binary HEADER controls now require explicit `$MEASUREMENT` to be the documented integer 0 (imperial) or 1 (metric), retain unsupported values on tolerant input, and preserve absence independently of `$INSUNITS`; the existing AC1015/AC1032 presence and unchanged-geometry selector also passes. See the [MEASUREMENT domain receipt](docs/research/p6-measurement-header-dxf-domain-2026-11-02.json). This establishes scalar admission only; hatch/linetype resource selection remains unqualified. Generated SCALE controls require positive finite paper/drawing units on DXF read/write; group 70 remains preserved on input but nonzero values are refused on typed output because ODA v5.4.1 identifies it as unknown and writes zero. These are field-admission guards only, not native annotation-scale qualification. See the [SCALE units receipt](docs/research/p6-scale-units-dxf-domain-2026-11-02.json) and [SCALE flag receipt](docs/research/p6-scale-unknown-flag-dxf-output-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls now enforce the documented group-2 string representation for `$UCSBASE` and `$PUCSBASE`, retain malformed values tolerantly, and refuse wrong types/groups, aliases, and case-folded keys with atomic rollback and corrected retry; this does not resolve a named UCS table target or qualify orthographic restoration. See the [UCS base field receipt](docs/research/p6-ucs-base-header-fields-guard-2026-11-02.json). Strict output now resolves nonempty ASCII UCSBASE/PUCSBASE values to unique output UCS records, admits blank and built-in WORLD values, and keeps differing non-ASCII case variants refused; see the [UCS base target receipt](docs/research/p6-ucs-base-name-target-closure-2026-11-02.json). Remaining work: native linetype resource selection/rendering across `$MEASUREMENT`, LTSCALE/CELTSCALE, `$PSLTSCALE` and `$MSLTSCALE`, including exact behavior for AutoCAD 2007 versus 2008–2009 inside the shared AC1021 target; native orthographic restoration and non-ASCII UCS case collation; transparency/ByLayer/ByBlock meaning; remaining SCALE version/API boundaries; and Template description/survey policies. ODA §22 byte-count framing remains supported across all six reader versions. A well-bounded non-empty byte-count description is decoded with the recognized source codepage and delivered to tolerant DWG consumers through an optional trailing `DRW_Interface` callback; the following MEASUREMENT is also published. A warning still makes strict-complete DXF output refuse the description because DXF has no matching HEADER field. Modern TU16-shaped descriptions remain readable only with a warning and no guessed MEASUREMENT: LibreDWG 0.13.3 uses TU16 in its R2007+ T16 path, which conflicts with ODA §22. Its local writer/reader round trip preserves a non-empty description, but libdxfrw rejects that generated DWG earlier at the file-header stage, so it is not parser validation. Generated controls cover maximum signed length, exact logical-section bounds, empty descriptions, legacy modern UTF-16-NUL compatibility, codepage decoding/callback delivery, unknown-codepage fail-closed behavior, byte-count measurement publication, TU16-shaped warning-only behavior and malformed framing. Missing mandatory AcDb:Template sections remain tolerant on read but now refuse strict-complete output for AC1018+; absence remains optional for AC1012-AC1015. See the [missing-section strict-output receipt](docs/research/p6-template-missing-section-strict-output-guard-2026-11-02.json). Description text still has no DXF representation; strict DXF preservation, producer breadth, native semantics and broader survey policies remain open. See the [Template frame receipt](docs/research/p6-template-description-frame-2026-11-02.json) and [description strict-output guard receipt](docs/research/p6-template-description-strict-output-guard-2026-11-02.json). |
| INSUNITS scalar domain | Autodesk documents integer group 70 values 0–24. The public `DRW_Header::Units` enum now includes codes 21–24, and the measurement helper classifies all four as English units; DXF output validates the canonical scalar and refuses it for pre-AC1015 targets, while DWG HEADER encoding rejects malformed or out-of-domain values before writing bits. Generated ASCII/Binary controls round-trip 0–24, preserve invalid input tolerantly, and verify failure/retry; direct DWG HEADER controls cover all six writer revisions. This is scalar representation only. Autodesk notes code 21 has release-specific support and codes 22–24 are AutoCAD for Mac only, so insertion scaling and native platform behavior remain unqualified. See the [INSUNITS receipt](docs/research/p6-insunits-domain-guard-2026-11-02.json). |
| Lineweight and display defaults | DXF HEADER output now validates `$CELWEIGHT` against the enumerated values (including special -3/-2/-1 values), `$ENDCAPS`/`$JOINSTYLE` as 0–3, and `$LWDISPLAY`/`$XEDIT` as 0/1. Generated ASCII/Binary tests cover every admitted value, malformed types/groups/aliases, atomic refusal and same-writer retry; tolerant input retains unsupported integer values. This is scalar admission only; rendering, plotting, and native behavior remain unqualified. See the [lineweight HEADER receipt](docs/research/p6-lineweight-header-domain-guard-2026-11-02.json). |
| Orientation and attribute visibility | DXF HEADER output now validates `$ANGDIR` as 0/1 and `$ATTMODE` as 0/1/2. Generated ASCII/Binary controls cover valid readback, malformed type/group/aliases, atomic refusal, corrected retry, and tolerant input retention. Legacy `$ATTDIA` and `$ATTREQ` now enforce the Autodesk R12 DXF group-70 domain 0/1 only below AC1012, matching the existing writer emission branch; AC1012, AC1014, and AC1027 controls verify these omitted values do not block output. This verifies legacy field-domain transport only and does not qualify native INSERT or prompt behavior; see the [legacy attribute-prompt receipt](docs/research/p6-attdia-attreq-legacy-header-domain-guard-2026-11-02.json). Autodesk's system-variable reference enumerates `$MIRRTEXT` as integer 0/1, while the DXF table's “if nonzero” wording describes the Boolean effect; the statements are compatible. HEADER output now enforces the 0/1 domain with ASCII/Binary readback, refusal, retry, and tolerant-input controls; see the [MIRRTEXT resolution receipt](docs/research/p6-mirrtext-header-domain-resolution-2026-11-02.json). These field-domain checks do not qualify native mirroring or attribute display behavior. See the [orientation/attribute HEADER receipt](docs/research/p6-angdir-attmode-header-domain-guard-2026-11-02.json). |
| Unknown/proxy data | A bounded same-revision AC1018+ `ACAD_PROXY_ENTITY` serializer now transports the modeled class ordinal and entity-data payload for generated no-reference profiles with ASCII/Binary readback. Its low-level writer also round-trips resolver-backed object-ID groups in generated controls; the dwg2dxf adapter now binds unique top-level typed entity targets and final emission for pointer handle codes 4/5 and offset soft-pointer codes 6/8/10/12, mapped to groups 330/340. Owner-role/unknown codes, nested/raw/OBJECTS targets, recursive dependencies, physical source profiles, and native semantics remain fail-closed or unqualified. Unknown-data slots, EED/common reference closure, mixed ACDS graphs, and native semantics remain open; unsupported profiles refuse atomically. Additional proxy primitives need independent semantics, not cache-byte agreement. Associative output and replay belong to P4. See the [bounded proxy transport receipt](docs/research/p4-dwg-proxy-entity-bounded-dxf-transport-2026-11-02.json) and [object-ID transport receipt](docs/research/p4-dwg-proxy-entity-object-id-dxf-transport-2026-11-02.json). |
| Safety and caller state | OPEN. Continue auditing remaining setup, assembly and work bounds, especially other `writeObjects` maps/vectors; inject allocation failures across independent builders; broaden rollback/failure injection to other staged families; current controls cover the core IMAGEDEF OBJECTS allocation-offset sweep, rooted named-dictionary OBJECTS final-allocation rollback/retry, and IMAGEDEF callback validation/bad_alloc/retry. The ASCII/Binary IMAGEDEF sweep now checks each calibrated allocation before the consumer OBJECTS callback for atomic rollback, destination and identity preservation, same-writer retry, and typed readback ([IMAGEDEF allocation sweep](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json)). Generated NOD/GROUP/IMAGE dictionary-source admission also sweeps allocation failures through root-only and rooted child-dictionary profiles, preserving prior owned state and retrying the same codec after each injected failure ([source admission sweep](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json)). Still audit source lifetime, platform numeric/string policies and scoped concurrency. Completed bounded staging families: GROUP, IMAGEDEF plan, named/root dictionaries, CLASS, raw sections, resolved references, TABLESTYLE/BG/SUN registries, source BLOCK_RECORD and canonical LTYPE. The controls use internal logical resource estimates and do not create format maxima or support claims. Evidence: [GROUP count](docs/research/p6-dxf-group-vector-setter-count-guard-2026-11-03.json), [GROUP nested bounds](docs/research/p6-dxf-group-nested-setter-bounds-2026-11-03.json), [GROUP plan allocation retry](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json), [generated dictionary source allocation](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json), [IMAGEDEF count](docs/research/p6-dxf-image-definition-count-and-staging-guard-2026-11-03.json), [IMAGEDEF plan bounds](docs/research/p6-dxf-image-definition-plan-nested-setter-bounds-2026-11-03.json), [IMAGEDEF OBJECTS rollback](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json), [core named-dictionary OBJECTS rollback](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json), [named-dictionary count](docs/research/p6-dxf-named-dictionary-object-setter-count-guard-2026-11-03.json), [named-dictionary nested bounds](docs/research/p6-dxf-named-dictionary-object-nested-setter-bounds-2026-11-03.json), [root dictionary count](docs/research/p6-dxf-root-dictionary-setter-count-guard-2026-11-03.json), [root dictionary byte bounds](docs/research/p6-dxf-root-dictionary-byte-bound-2026-11-03.json), [CLASS count](docs/research/p6-dxf-class-setter-count-guard-2026-11-03.json), [CLASS byte bounds](docs/research/p6-dxf-class-metadata-setter-byte-bound-2026-11-03.json), [raw sections](docs/research/p6-dxf-raw-section-setter-bounds-2026-11-03.json), [resolved references](docs/research/p6-dxf-resolved-write-reference-setter-bounds-2026-11-03.json), [handle remap](docs/research/p6-dxf-handle-remap-setter-bound-2026-11-03.json), and [source table metadata](docs/research/p6-dxf-source-table-metadata-setter-bounds-2026-11-03.json). | Keep validation focused; do not reopen a passed guard without a new failure. |
| CLI and oracle contracts | Single-file output/input filesystem-alias refusal is implemented for positional and FreeCAD `-o` routes; the fast CLI selector covers same-path, symlink and hard-link aliases and verifies source preservation (see the [alias guard receipt](docs/research/p6-cli-single-file-input-output-alias-guard-2026-11-02.json)). Generated source-DXF FreeCAD `-o` controls now bind default target selection for all nine supported revision headers in ASCII and Binary, including AC1012→AC1014. Locally authored DWGs now cover every DWG-writer version (AC1015, AC1018, AC1021, AC1024, AC1027 and AC1032). AC1015/21/24/27/32 succeed at their default targets in ASCII/Binary, with binary readback at the same revision. AC1018 is refused atomically at both its default AC1018 target and explicit AC1024 because two opaque callbacks remain unqualified (qualified=0/2); this is P4 source/output closure work. A debug-only preflight projection now exposes source format/revision, selected target revision, implicit-versus-explicit selection, encoding and input-appropriate read-completeness mode before writer setup. Generated AC1012 DXF input verifies its AC1014 default; AC1006 input verifies a fail-closed unavailable default; generated AC1021 DWG success and AC1018 default/explicit refusal verify the projection before output preflight. An external AC1014 sample also selects AC1014 but is refused because two source objects fail parsing and the frame disposition is incomplete; this is diagnostic evidence only, and the sample is not committed. See the [default target-version receipt](docs/research/p6-cli-freecad-default-target-version-matrix-2026-11-02.json). Remaining contracts: eligible AC1012/AC1014 DWG-source output/readback; other partial-read failure profiles (the six-writer-version generated control is recorded in the [all-version receipt](docs/research/p6-cli-partial-dwg-read-all-writer-versions-2026-11-02.json)); and broader CLI projections: broader HEADER structure/repairability profiles beyond generated duplicate `$ACADVER`/`$LTSCALE` variables and repeated group 10 in `$INSBASE`/`$EXTMIN`; the API repair workflow for duplicate variables/repeated group codes is covered by the ASCII/Binary same-adapter replacement/retry control in [the HEADER receipt](docs/research/par27-header-repeated-group-refusal-fast-slice-2026-10-22.json); broader malformed structures remain open; `$REQUIREDVERSIONS` malformed/repeated diagnostic controls now cover duplicate variable, repeated group 160, and wrong group code with AC1032 atomic refusal; feature semantics/native acceptance beyond one generated bit preserved through AC1032 typed DXF output/readback plus AC1024 refusal remain open; decoder issue-mask projection now exercises all 15 defined `DxfTextIssue` flags across 14 locally authored profiles with exact-mask and atomic-refusal checks; see the [CLI issue-mask matrix receipt](docs/research/p6-cli-text-decoder-issue-mask-matrix-2026-11-02.json); and skipped-record projections cover one and three same-class instances plus three distinct AC1027 custom class names, with exact source-name and aggregate counts, strict refusal, and destination preservation; other skip/recovery categories and revisions remain open. See the [multi-class receipt](docs/research/p6-cli-skipped-record-multiple-class-aggregation-2026-11-02.json). The CLI applies no repair; these projections diagnose only. See the [source-disposition projection receipt](docs/research/p6-cli-source-disposition-projection-2026-11-02.json).

**ODA ARM64 package integrity recheck:** the official ODA File Converter page advertises macOS ARM64 and currently links a 27.9.0.0 package. The downloaded package reports an invalid signature; its extracted app also fails strict code-signature verification. It was not installed or run. The prior 27.1.0.0 GUI conversion remains the only ODA run and does not explain the AC1027 Cover suffix. After the user's latest unlock, the 2026-11-03 11:24 UTC strict check again reports an invalid signature on the installed 27.1.0.0 bundle (executable SHA-256 unchanged); the converter was not invoked. Resume this experiment only with an integrity-verified ODA artifact or another eligible producer; see the [package integrity receipt](docs/research/p6-ac1027-cover-oda-arm64-package-integrity-2026-11-02.json) and [ODA resave/retry receipt](docs/research/p6-ac1027-cover-oda-resave-recheck-2026-11-02.json).

**MESH DWG face-stream guards:** `DRW_Mesh::parseDwg` reserves the parsed-face vector to the validated face-stream item bound and rejects face records with fewer than three vertex indices, matching the typed geometry invariant and DXF reader. Generated AC1024 vectors cover triangle readback, two-index face refusal with cleared state and same-object retry, plus the existing malformed crease-count refusal/retry selector. These are bounded parsing and failure-state controls only; the ODA v5.4.1 PDF has no `AcDbSubDMesh` layout entry, and no DWG layout or native geometry support claim is inferred. See the [face-record reserve receipt](docs/research/p6-mesh-dwg-face-record-reserve-guard-2026-11-02.json) and [face-count domain receipt](docs/research/p6-mesh-dwg-face-count-domain-2026-11-02.json).

**MESH trailing flags, unresolved subdivision vectors and allocation guards:** The DWG trailing-field schema remains unresolved. The only physically traced pattern is `10`: it is compatible with two one-bit fields `(1,0)` and with one BitLong zero. LibreDWG lists two B fields but labels the body undocumented/unstable; ACadSharp and dwgTs read one BitLong, with dwgTs describing itself as inspired by ACadSharp. ODA v5.4.1 has no AcDbSubDMesh body layout, and following-handle alignment cannot distinguish these two-bit-consumption cases. The parser now admits only the observed `10` pattern and refuses the other three two-bit candidates before publishing a typed record. The writer emits canonical `10` for the model default (`unknown=0`) and the observed parsed mask (`unknown=1`), and refuses masks 2 and 3. Generated parser controls cover all four candidates; writer controls check the exact canonical tail and atomic refusal across all six writer revisions. This bounds transport to the shared observed profile without asserting whether its fields are flags or BitLong zero; see the [canonical profile guard receipt](docs/research/p6-mesh-tail-canonical-profile-guard-2026-11-02.json) and [peer-span ambiguity receipt](docs/research/p6-mesh-tail-peer-span-ambiguity-2026-11-02.json). DXF output atomically refuses any nonzero mask because the Autodesk MESH DXF group list has no corresponding fields. The legacy public `subdivVertices` vector is rejected by both output routes when nonempty. Its mapping is unresolved: the [Autodesk MESH DXF reference](https://help.autodesk.com/cloudhelp/2016/ENU/AutoCAD-DXF/files/GUID-4B9ADA67-87C8-4673-A579-6E4C76FF7025.htm) describes group 91 as subdivision-level count, while LibreDWG 0.14's schema labels group 91 `num_subdiv_vertex` and declares a following point vector. An expanded census scanned 941 DWGs (799 valid LibreDWG minJSON reads) across six local document roots and LibreDWG's test corpus; it found 36 MESH records in 18 paths, representing 16 records in eight unique source byte streams. Verbose DWG traces for all eight streams, spanning AC1014 through AC1032, report candidate group-91 vector counts of zero. The local ASCII DXF search found 30 MESH records in 18 paths (15 source-distinct records across eight unique file hashes); every group's first group-91 value is zero. These corpora cannot adjudicate nonzero field order or meaning; retain the loss guard until an eligible nonzero source resolves it. See the [subdivision-vector corpus receipt](docs/research/p6-mesh-subdivvertices-corpus-census-2026-11-02.json). The DXF reader reserves the bounded face list before advancing parser state and rolls back a newly opened face if reserving its index vector fails; injected failures at vertex, face-list and per-face reservations preserve same-group retry. See the [DWG flag-field receipt](docs/research/p6-mesh-dwg-trailing-bit-fields-recheck-2026-11-02.json), [subdivision-vector loss guard receipt](docs/research/p6-mesh-subdivvertices-output-loss-guard-2026-11-02.json), [DXF tail/loss receipt](docs/research/p6-mesh-dwg-unrepresented-tail-guard-2026-11-02.json) and [DXF allocation receipt](docs/research/p6-mesh-dxf-face-allocation-rollback-2026-11-02.json). The earlier [DWG writer refusal receipt](docs/research/p6-mesh-dwg-opaque-tail-output-guard-2026-11-02.json) is superseded only for the observed two-bit DWG layout; retain it as historical evidence.

**SCALE unit invariant:** DWG SCALE parsing and encoding now require finite, positive paper and drawing units, following Autodesk's AcDbAnnotationScale API domain and the ODA §20.4.92 field layout. Generated invalid AC1014 parser vectors and positive/invalid writer controls across all six DWG writer revisions pass `libdxfrw_scale_dwg_unit_domains`; see the [DWG SCALE API-domain receipt](docs/research/p6-scale-dwg-positive-unit-api-domain-2026-11-02.json). This closes only the unit-value invariant. Version-specific native acceptance and annotation rendering remain unqualified.

**SPLINE profile recheck:** Two fit-only AC1024 SPLINEs in the local conference-room drawing match LibreDWG 0.14 by handle, scenario, degree, fit-point count and ordered coordinates (the libdxfrw trace rounds coordinates to six decimal places); see the [AC1024 differential receipt](docs/research/p6-ac1024-spline-field-differential-2026-11-01.json). A public AC1027 `example_2013.dwg` witness adds two fit-only records. Their typed libdxfrw reads match LibreDWG 0.14 for scenario, R2013+ `splFlag1`/knot-parameter BL fields, degree, tolerance, tangents and ordered fit points within 1e-12. ODA v5.4.1 §20.4.40 specifies both added fields as BL, and the current parser already reads them as bounded bit-long; this witness supports keeping that implementation and does not justify a wire change. The two captured records also survive isolated AC1027 ASCII/Binary DXF export and typed readback for flags, degree, fit count, tolerance, tangents and ordered fit points; the generated fast control is registered as `libdxfrw_spline_fit_only_dxf`. A new writer guard rejects R2013+ fit-only records using square-root-chord or uniform parameterization, which the DXF SPLINE fields cannot express; it admits the documented chord default and tests refusal plus same-adapter retry in both encodings. The physical records' `splFlag1=9`/`knotParam=0` match that admitted profile, but DXF readback cannot carry those DWG-only fields independently. Curve equivalence, native interpretation, and broader producer/version qualification remain open. See the [knot-parameterization guard receipt](docs/research/p6-ac1027-spline-fit-knot-parameterization-dxf-guard-2026-11-02.json) and the [AC1027 differential receipt](docs/research/p6-ac1027-fit-spline-field-differential-2026-11-01.json). The public sample still has unrelated object failures and an orphaned DataStorage record. Strict AC1024-target output passes TABLEGEOMETRY but stops at ModelDocViewStyle exact-version serialization; strict AC1027-target output is refused by the incomplete read disposition. See the [TABLEGEOMETRY transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json). Neither physical field nor isolated transport evidence promotes a geometry or support claim.

**LOFTEDSURFACE path-curve DXF projection:** `DRW_LoftedSurface::pathCurveHandle`
is a DWG handle reference, but Autodesk's DXF table documents no group-5
path-reference field, and LibreDWG's field mapping gives `path_curve` no DXF
code (unlike the cross-section/guide handle vectors). The writer therefore
refuses a nonzero path handle before publication instead of emitting a second
group 5 that collides with the entity identity; zero-handle ASCII/Binary output
retains exactly the ordinary entity handle. See the [path-curve refusal
receipt](docs/research/p6-lofted-surface-dwg-path-handle-dxf-refusal-2026-11-03.json).
The DWG `crossSectionHandles`, `guideCurveHandles`, and their counts are now
also refused atomically by DXF output. The local LibreDWG
`0.13.3-553-g6d6a3398` source maps these vectors to repeated group-310 values,
but libdxfrw currently retains DXF-origin group-90/group-310
tokens as opaque variants and has no verified handle-to-token encoding,
reference-remap, or dependency-closure path for projecting the DWG vectors.
Existing captured DXF tokens still pass through unchanged. See the [expanded
reference refusal receipt](docs/research/p6-lofted-surface-dwg-reference-dxf-refusal-2026-11-03.json).
Before relaxing either refusal, prove exact group-310 encoding and handle
remapping for supported source profiles, then require graph closure and typed
readback. These guards do not qualify the surface's source graph, ACIS shape,
or native semantics.

Use the ODA v5.4.1 PDF in `~/doc/dwg/` before DWG layout changes; discover type
codes from real traces. Preserve tolerant record-level reading and the first
failing-stage diagnostic.

**Generated DIMENSION mixed-frame re-read correction (2026-11-02; generated transport slice):** `DRW_Dimension::applyExtrusion()` transforms its native OCS text, clone, and angular-arc points for callers that request WCS coordinates. Both DXF and DWG serializers now project those transformed points back to native OCS before emitting the original extrusion vector; untouched WCS definition points remain unchanged. The existing `haveExtrusion` state records the coordinate frame, so `DRW_Dimension` object layout and virtual interface stay unchanged. Generated ASCII/Binary controls cover nine dimension subtypes, four extrusion normals, both read modes, writer output/readback, and a following-entity sentinel. A generated AC1015 DWG writer/readback control also verifies transformed text/clone and angular-arc points. Build/compatibility targets pass and the selected nine CTests pass in 9.57 seconds. Receipt: `docs/research/p6-dimension-mixed-frame-writeback-2026-11-02.json`. This closes only the generated mixed-frame transport regression; physical producer fields, native rendering, associative meaning, and support claims remain unqualified.

**UNDERLAY clip parser allocation retry:** The DXF parser now commits clip and inverse-clip occurrence/count state only after bounded vector growth succeeds. Injected allocation failures at clip growth, inverse-clip count reserve, and inverse-clip vertex growth leave logical state retryable; focused UNDERLAY OCS/read and MLINE allocation controls pass 4/4. This is parser safety evidence only, not native UNDERLAY or producer qualification. See [the allocation/retry receipt](docs/research/p6-underlay-dxf-clip-allocation-retry-2026-11-02.json).

**LEADER per-vertex DXF allocation retry:** The parser stages all missing coordinate objects before publishing the vertices and advances a field occurrence only after the growth succeeds. Injected failures at temporary storage, coordinate allocation, and destination reserve leave the point list and ordinal unchanged; retrying the same group yields the expected 3D point. The six focused LEADER/UNDERLAY/MLINE fast controls pass. This covers parser safety only, not DWG parsing or native annotation semantics. See [the LEADER allocation receipt](docs/research/p6-leader-dxf-vertex-allocation-retry-2026-11-02.json).

**SPLINE DXF parser allocation retry:** Control/fit point coordinates are staged until point storage and the component cursor are ready; knot and weight insertion failures return through the parser without advancing state. Generated fault injection covers cursor creation, coordinate creation, point-vector reserve, knot growth and weight growth, then retries the same groups. The focused SPLINE/LEADER/UNDERLAY/MLINE controls pass 7/7. This is parser safety evidence only; broader SPLINE semantics, producer prevalence and native curve interpretation remain open. See [the allocation/retry receipt](docs/research/p6-spline-dxf-allocation-failure-retry-2026-11-02.json).

**AC1027 DataStorage table-role profile in the AC1032 resave:** The ODA-resaved
AC1032 sample retains the exact bounded schema-index table-role profile
observed in the AC1027 source. The parser admits that profile for both observed
revisions while preserving ODA's documented order as the default and rejecting
malformed references. The local AC1032 Cover read still refuses strict output
because of its unresolved HEADER suffix; this establishes structural profile
recognition only. See the [profile recheck receipt](docs/research/p6-ac1027-cover-ac1032-datastorage-role-profile-2026-11-02.json).

**Disabled HATCH/MPOLYGON gradient-stop refusal and retry:** Autodesk's HATCH
DXF table defines group 450 value 0 as solid fill and group 453 value 0 as
zero colors. A nonempty color-stop list therefore has no valid carrier while
gradient fill is disabled. `writeHatch` and `writeMPolygon` now reject that
payload before common preflight makes the refusal sticky. Generated ASCII and
Binary controls verify atomic refusal and same-writer retry after clearing the
stops. This closes only the writer retry path; physical producer prevalence,
fill semantics and native rendering remain open. See the [retry guard receipt](docs/research/p6-hatch-inactive-gradient-stop-writer-retry-2026-11-02.json)
and [Autodesk's HATCH DXF table](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-C6C71CED-CE0F-4184-82A5-07AD6241F15B.htm).

**HATCH/MPOLYGON validation through whole-file callbacks:** `DRW_Hatch::validateDxf`
is intentionally non-virtual, so the shared validator now explicitly invokes
`DRW_MPolygon::validateDxf` for derived polygons without changing the public
vtable. If payload validation fails during a whole-file callback, the active
transaction is latched as failed; direct record-level callers retain corrected
same-writer retry behavior. This closes a path where the void callback contract
could otherwise publish a file after an entity writer returned false. The five
focused HATCH/MPOLYGON round-trip, CMC and boundary graph tests pass; see the
[atomic callback refusal receipt](docs/research/p6-hatch-mpolygon-callback-refusal-atomicity-2026-11-02.json).
This is validation and transaction evidence only, not native fill/rendering or
broader HATCH/MPOLYGON qualification.

**GEODATA residual qualification:** The bounded generated AC1027 version-2
adapter path is implemented: it retains the typed callback, binds the exact
source CLASS and common owner, closes the model-space host-block reference,
plans per-frame output bindings, and verifies fresh ASCII/Binary readback plus
atomic refusal and corrected retry. Do not repeat adapter-retention or output
work without a new failing witness. Physical producer breadth, independent
source-span/target-role evidence for other profiles, native coordinate and
projection semantics, and composed whole-document qualification remain open.
See the [adapter transport receipt](docs/research/p6-geodata-dwg-adapter-transport-2026-11-02.json)
and [host-block remap receipt](docs/research/p6-geodata-host-block-reference-remap-2026-11-02.json).
The low-level GEODATA writer also remaps or allocates its own group-5 identity,
binds source identity after staged output commits, and passes focused ASCII/
Binary typed readback, fixed-handle refusal, XDATA rollback, and fresh-writer
retry controls ([identity receipt](docs/research/p8-geodata-identity-remapping-2026-11-03.json)).
This closes only own-identity transport; producer breadth and native coordinate
and projection semantics remain open.
The low-level DBCOLOR writer now remaps or allocates its own group-5 identity
and binds source identity after staged output commits. ASCII/Binary controls
cover ACI/book-color typed readback, fixed-ID refusal, late-XDATA rollback, and
fresh-writer retry ([identity receipt](docs/research/p8-dbcolor-identity-remapping-2026-11-03.json)).
This closes only DBCOLOR identity transport; physical producer prevalence and
native color-book acceptance remain open.
The shared low-level RENDERSETTINGS-family writer now remaps or allocates its
group-5 identity and binds source identity after staged output commits. The
focused ASCII/Binary identity control and existing preset output, graph, and
transaction controls pass ([identity receipt](docs/research/p8-render-settings-identity-remapping-2026-11-03.json)).
This closes only identity transport; physical producer breadth and native
rendering semantics remain open.

## P7 — ACIS/ACDS and independent 3D meaning

- Complete the original DWG entity → owner/key → AcDs segment → SAT/SAB join,
  versioned framing/index/suffix/prototype assembly and exact output association.
- Complete source placement, units/transforms, topology, trims, periodicity and
  spline geometry beyond locally verified analytic helpers. A local wireframe or
  repaired/count-only graph is not B-rep support evidence.
- Qualify at least one genuinely independent semantic/kernel route for each
  claimed carrier/profile. Check dimensions, placed bounds, topology and applicable
  area/volume/orientation; record executable, modules, input, config and actual
  before-repair outcome.
- Preserve unsupported required bodies through a qualified route or refuse complete
  conversion atomically. Do not silently omit, guess joins, synthesize entities or
  flatten required 3D data.

Research a new reader only for a named missing semantic operation; wrapping the
same parser is not an independent oracle.

**Independent reader preflight:** The user-selected official CAD Assistant
macOS ARM64 DMG was fetched and its disk-image CRC verified. This shell cannot
mount it because DiskManagement is unavailable; the 7-Zip extracted app and
main executable fail strict `codesign` verification, so neither was launched.
Treat extraction damage as possible and the candidate as unqualified; do not
bypass signature checks. Reattempt only with a native verified mount or another
eligible reader. The exact package and environment result are in the
[CAD Assistant preflight receipt](docs/research/p7-cad-assistant-arm64-integrity-preflight-2026-11-01.json).

## P8 — consumer, platform, packaging and release gates

- Preserve LibreCAD/LibreCAD_3 callback/default behavior; new callbacks remain
  non-pure. The compile-only LibreCAD_3 facade guard and generated ASCII/Binary
  3D-consumer probe pass; see the [local API and 3D probe receipt](docs/research/p8-local-compat-and-generated-3d-probe-2026-11-02.json).
  These checks cover API shape and generated record delivery only, not native
  application behavior. Qualify intended 3D consumers and remaining FreeCAD
  `dwg2dxf` profiles without dropping required geometry.
  A bounded generated CAMERA reference profile now binds source VIEW handles
  to emitted VIEW table rows. CAMERA code 340 is emitted only for a VIEW row
  serialized in the current attempt, and uses its remapped handle; missing,
  wrong-kind, late missing-target, derived-runtime, and mismatched-public-tag
  cases preserve the destination and recover on same-adapter retry in ASCII
  and Binary DXF. The focused
  compatibility cohort passes; no fixture was added. This closes only
  generated reference transport, not physical producer coverage or native
  camera semantics. See the [CAMERA VIEW receipt](docs/research/p8-camera-view-reference-closure-2026-11-02.json).
  The concrete BLOCK
  serializer now runtime-checks `DRW_ModelerGeometry`, `DRW_Mesh`, `DRW_Helix`,
  and `DRW_Spline` before dispatching public `eType` tags `3DSOLID`, `REGION`,
  `BODY`, `MESH`, `HELIX`, and `SPLINE`; a mismatched object fails atomically
  instead of being reinterpreted through an invalid cast. Generated ASCII/Binary controls
  cover all six tags, existing-file preservation, same-adapter correction/
  retry, and line-geometry readback; see the [3D dispatch receipt](docs/research/p8-3d-entity-runtime-type-dispatch-2026-11-02.json).
  This protects callback integrity only; it does not qualify 3D payload syntax,
  topology/kernel meaning, native consumers, or whole BLOCK inventory.
  The same adapter now checks the HATCH/MPOLYGON subtype boundary in both
  directions: a plain HATCH cannot enter the MPOLYGON writer, and an MPOLYGON
  cannot be serialized as HATCH with its distinct fields dropped. Generated
  ASCII/Binary refusal, destination-preservation, corrected-retry and typed
  readback controls retain fill ACI, annotated-boundary and offset-vector
  values; see the [HATCH/MPOLYGON dispatch receipt](docs/research/p8-hatch-mpolygon-runtime-dispatch-2026-11-02.json).
  This is adapter integrity/transport evidence only and does not qualify native
  fill, boundary or rendering semantics.
  The BLOCK callback also refuses a non-VIEWPORT runtime entity tagged as
  VIEWPORT before reaching the DXF writer's static cast. Generated ASCII/Binary
  controls verify atomic refusal, destination preservation, corrected retry,
  and POINT readback; see the [VIEWPORT dispatch receipt](docs/research/p8-viewport-runtime-dispatch-2026-11-02.json).
  The INSERT writer dispatch now requires the exact `DRW_Insert` runtime type
  before accessing the larger INSERT payload through its public `eType` tag.
  A POINT tagged as INSERT is refused atomically; correcting it permits a
  same-adapter retry and typed POINT readback in ASCII/Binary DXF. See the
  [INSERT dispatch receipt](docs/research/p8-insert-runtime-dispatch-2026-11-02.json).
  The remaining former raw entity-writer casts now pass through an exact
  runtime-type/public-tag gate, including the primitive inheritance families,
  annotation types, all dimension tags, MLINE, IMAGE and WIPEOUT. The supported
  DRW_DimLargeRadial and dx_ifaceImg routes remain explicit. Generated ASCII/
  Binary controls cover invalid base-to-derived dispatch, derived-to-base
  truncation, destination preservation, corrected retry and typed readback;
  see the [entity runtime-dispatch receipt](docs/research/p8-entity-runtime-dispatch-2026-11-02.json).
  This closes the unchecked static-cast inventory in `writeEntity`; native
  consumer behavior and semantic support remain open. The separate TABLE
  fallback-graph preflight now validates the exact `DRW_Insert` runtime class
  before reading its INSERT projection through the public tag. A locally
  generated POINT tagged INSERT is refused with destination preservation, and
  restoring the projection permits same-adapter ASCII/Binary retry and typed
  TABLE readback; see the [TABLE fallback dispatch receipt](docs/research/p8-table-fallback-runtime-dispatch-2026-11-02.json).
  `dx_iface` now retains and dispatches POINTCLOUD, POINTCLOUDEX, and
  NAVISWORKSMODEL through existing bounded typed capture and DXF writer paths.
  Generated ASCII/Binary controls check AC1024 refusal for the AC1027-only
  POINTCLOUDEX writer profile, destination preservation, corrected retry, and
  typed field readback; see the [point-cloud adapter receipt](docs/research/p8-point-cloud-adapter-transport-2026-11-02.json).
  The low-level POINTCLOUD and POINTCLOUDEX serializers now resolve their
  definition/reactor references through the configured resolver (final output
  IDs) or source-handle remap fallback. Generated ASCII/Binary controls verify
  both paths; see the [reference remapping receipt](docs/research/p8-point-cloud-dxf-reference-remapping-2026-11-03.json).
  The four low-level POINTCLOUDDEFINITION, POINTCLOUDDEFINITIONEX,
  POINTCLOUDDEFREACTOR, and POINTCLOUDDEFREACTOREX records now remap and
  reserve/allocate their group-5 identity, then bind source identity only after
  staged record commit. Generated ASCII/Binary controls cover all four typed
  variants, allocated identities, fixed-handle refusal, late-XDATA rollback,
  fresh-writer retry and typed readback ([receipt](docs/research/p8-pointcloud-definition-identity-remapping-2026-11-03.json)).
  This closes identity transport only; physical producer coverage, exact
  definition/reactor association, target ownership, asset loading and native
  3D behavior remain open.
  The NAVISWORKSMODEL low-level writer now applies the same resolver/remap
  policy to its optional definition reference; its ASCII/Binary control is
  recorded in the [NAVISWORKS reference receipt](docs/research/p8-navisworks-dxf-reference-remapping-2026-11-03.json).
  The `NAVISWORKSMODELDEF` and `POINTCLOUDCOLORMAP` low-level writers also
  remap or allocate their own group-5 identities and bind source identities
  after staged record commit. Focused ASCII/Binary controls cover typed payload
  readback, fixed-handle refusal, byte/inventory/identity-map rollback after
  late XDATA failure, and corrected fresh-writer retry ([identity receipt](docs/research/p8-external-definition-identity-remapping-2026-11-03.json)).
  This closes only these records' identity transport; source producer coverage,
  definition/owner graph closure, asset loading, and native consumer semantics
  remain open.
  The low-level `RASTERVARIABLES` and `WIPEOUTVARIABLES` writers now remap or
  allocate their group-5 identities and bind source identities after staged
  output commits. Focused ASCII/Binary controls cover typed readback, fixed-ID
  refusal, byte/inventory/identity-map rollback on late XDATA failure, and
  corrected fresh-writer retry ([identity receipt](docs/research/p8-raster-wipeout-variables-identity-remapping-2026-11-03.json)).
  This closes only identity transport; physical producer coverage, native
  image/wipeout display behavior, and complete drawing qualification remain
  open.
  The low-level WIPEOUT writer now translates both IMAGEDEF and IMAGEDEF_REACTOR
  references through the configured resolver or remap fallback; see the
  [WIPEOUT reference receipt](docs/research/p8-wipeout-dxf-reference-remapping-2026-11-03.json).
  The low-level MLINE writer now gives its group-340 MLINESTYLE pointer the
  same resolver precedence and remap fallback; the [MLINE reference receipt](docs/research/p4-mline-dxf-reference-remapping-2026-11-03.json)
  records the focused ASCII/Binary control, including unresolved-target refusal
  and corrected retry. This does not prove target
  existence or close the MLINESTYLE dependency graph.
  The concrete adapter still atomically refuses nonzero definition/reactor
  handles until P4's source-bound object graph providers and output bindings
  qualify them.
  This does not close external-definition graph, asset loading, native 3D
  consumer, or broader BLOCK callback qualification.
  The local adapter now retains LIGHT callbacks through modelspace and named-
  BLOCK delivery, with bounded DXF read/write support for the documented core
  fields. Both callback-delivery wrappers forward LIGHT instead of dropping
  it. Generated ASCII/Binary controls verify pre-AC1021 and unrepresented
  metadata refusal, destination preservation, corrected-target retry, and
  typed readback in both scopes;
  see the [LIGHT adapter receipt](docs/research/p8-light-adapter-transport-2026-11-02.json).
  Follow-up slice: `DRW_Light::validateDxf` now centralizes the bounded
  representable profile for both the adapter and the public `writeLight`
  method. The direct writer refuses packed DWG CMC values, CMC metadata,
  photometric payloads and other unrepresented fields before writing the
  record; it emits no unqualified group 421. Generated ASCII/Binary controls
  verify atomic refusal, destination preservation, corrected same-adapter retry
  and typed readback. The focused test passes; no fixture was added. The same
  receipt records this direct-writer boundary.
  This establishes generated transport only, not physical producer breadth or
  native lighting/rendering semantics; LIGHT truecolor and photometric fields
  remain refused pending a qualified mapping.
  The limited macOS ARM64
  2D open/insert profile is recorded in
  [the FreeCAD route receipt](docs/research/p8-macos-freecad-dwg2dxf-open-insert-qualification-2026-11-01.json);
  it does not qualify 3D semantics or other platforms. The generated Underlay
  adapter route closes PDF/DGN/DWF definitions and references through the
  named-object dictionary, with AC1024/AC1032 ASCII/Binary readback and atomic
  missing-target/wrong-kind refusal plus retry; see the [Underlay graph
  receipt](docs/research/p8-underlay-definition-graph-adapter-2026-11-02.json).
  The low-level UNDERLAY writer now also honors the configured resolver for its
  group-340 definition ID before falling back to source-handle remapping; its
  focused ASCII/Binary control is recorded in the [UNDERLAY reference
  receipt](docs/research/p8-underlay-dxf-reference-remapping-2026-11-03.json).
  Both definition and reference writers now enforce kind-specific target
  floors: DWF at AC1021 and DGN/PDF at AC1024. Autodesk documents DWF before
  AutoCAD 2007 as unsupported, DGN before AutoCAD 2008 as unsupported, and PDF
  before AutoCAD 2010 as unsupported. Since AC1021 spans 2007–2009, requiring
  AC1024 for DGN conservatively avoids writing to the ambiguous 2007 target;
  this version mapping is an implementation safeguard, not a new format claim.
  Generated ASCII/Binary controls verify lower-version refusal preserves the
  destination and a same-adapter retry at the first admitted target reads back
  typed definitions and references; see the [Underlay target-version receipt](docs/research/p8-underlay-target-version-gates-2026-11-02.json).
  This proves transport only, not native consumer semantics or remaining BLOCK
  callback coverage.
- The optional desktop and native Linux/Windows FreeCAD CMake gates now bind to
  the existing tracked `ordinary_enc_AC1027.dwg`; a registration-only configure
  lists the desktop, open, and insert controls. Native execution is still open;
  see the [tracked-sample binding receipt](docs/research/p8-freecad-tracked-sample-binding-2026-11-02.json).
- Complete native Linux/Windows FreeCAD qualification and bind the actual loaded
  library/converter child. Verify Windows UTF-8/Qt paths, filesystem collisions,
  MSVC/DLL/shared/static/relocated consumers and package/toolchain contracts.
- Run the pending native Linux GCC source-release job. Complete ABI/API and native
  application checks, controlled RSS/performance/work-cap measurements and any
  required older-CMake/Conan/PIC/export gates.
- At integration, pin origin/master and LibreCAD target revisions, refresh only
  invalidated evidence, run required CI/release gates and check claims against the
  exact receipts. Packaging success does not establish format support.

## P9 — six external dispositions

Use the [retained successor registry](metadata/external-evidence-resolution-v1.json).
All rows remain **DEFERRED_EXTERNAL** and **non-promoting** until the existing
independent semantic/platform admission rules are met. None blocks unrelated
local implementation.

| Item | Exact remaining action |
| --- | --- |
| J256 | Use the per-hash J359 successor registry; retain its subset relationship and qualify only eligible independent source/field/platform evidence |
| J260 | Reuse the shared 28-stream successor registry with J293; execute each unique stream once and preserve original lineage/disposition |
| J268 | Recover original row membership/report and missing digest; if unrecoverable, create a distinctly named successor experiment—J260 is not a substitute |
| J284 | Use J361's three admitted fixtures; resolve the 11 claims' target-debt/integrity blockers and retain ARCALIGNEDTEXT, LARGE_RADIAL payload and MPOLYGON-color exclusions |
| J293 | Share J260's unique-stream work, retain separate claim bindings, and obtain independent semantic/platform evidence rather than duplicate corpus runs |
| J295 | Use J362's authored AC1024 recipe/frozen fields; obtain native receipts for four RTEXT position/rotation claims; nine minJSON successes establish reachability only |

An external process completing, an unavailable test being skipped, or advisory
agreement never changes a support claim automatically.

Share P7/P8/P9 executions only when exact input bytes, route/operation,
executable/modules, configuration and platform match. Store one execution receipt
with separate claim/lineage bindings; an ordinary-read run cannot stand in for an
open/insert, native-platform or independent-kernel run.

## Fast implementation and validation rules

| Stage | Default validation | Escalation trigger |
| --- | --- | --- |
| Plan/docs only | Links, remaining-scope/gate checks and diff review | No C++ rebuild or format suite |
| Edit loop | Build the touched target; run its focused selector and decisive literal/semantic controls | New failure, dependency or grammar boundary |
| Packet ready | Small affected owning cohort; actual CLI/readback for serializer changes; affected ASan once for bounds/lifetime/handle/ownership changes | Shared parser/writer/API or unresolved regression |
| Composed integration / PR | Union of affected cohorts once, compatibility probes and required CI | Broad shared classifier/numeric/allocator changes or release obligations |
| Release / promotion | Required full/native/package/ABI/corpus and claim checks on the exact binding | Explicit qualified support/release deliverable |

For the next packet, identify its owning selector from the exact failing witness
before editing. Build the touched executable once and run that selector during
development. At readiness, choose the smallest affected union from the actual
changed hooks below; do not inherit a prior slice's cohort automatically. Reuse
source-bound peer lanes when their profiles apply, and build `dwg2dxf` for actual
serializer/independent checkpoints. Extend existing projections only for the
missing field/schema and explicit expected output. Keep unaffected projections
cached.

Add tests according to changed hooks, rather than inheriting the prior 11-test
or 14-test checkpoint:

| Changed hook | Additional focused validation |
| --- | --- |
| Original resource publication/pinning | `libdxfrw_symbol_original_publications` |
| Shared core TABLES observer / symbol names | `libdxfrw_dxf_tables_write_inspection`, `libdxfrw_symbol_name_wire_inspection` |
| Writer source-target query / registry | `libdxfrw_dxf_source_table_identities` |
| LTYPE mirror / extension dependency hooks | `libdxfrw_ltype_mirror_authority`, `libdxfrw_linetype_extension_graph` as affected |
| Accounting / name-owner admission | Existing budget and name/owner selectors for the changed allowance |
| Geometry / capture / decoding / validation | Existing owning vector, original-source or wave1 selector for that change |

Rebuild every selected executable affected by adapter/static-library changes;
unchanged test source does not make an old linked executable current. Build
`libdxfrw_wave1_tests` only when its selected controls are affected. Run the final
affected union once in normal/ASan for changed ownership/bounds/lifetime behavior.
Reuse established query/observer proof when those hooks are unchanged; no full
corpus or package rerun is implied by selected provider admission.

1. Keep current configured normal/ASan builds, shared adapter objects and pinned
   comparator environments. Rebuild every selected executable affected by static
   library or adapter changes before running it. Prefer `.cpp` changes and
   existing helpers; batch necessary private state declarations instead of causing
   repeated header-wide recompilation for individual fields.
2. Begin with one discriminating source plus a boundary version/encoding. Expand
   only for a real format distinction. Reuse generated inputs and projections by
   generator/input/source/tool/profile digest; invalidate affected cells only.
3. Run ordinary reads and compare original fields/ownership before any audit or
   repair. Cache neither repaired results as original evidence nor broad claims
   under a narrow receipt. Reuse whole-implementation receipts only under their
   existing digest/reachability validator contract.
4. Run source-inventory checks cheaply. Regenerate/archive/build release artifacts
   at a manifest/package/integration/release checkpoint, not every scalar edit.
   Mark stale full-source qualification explicitly; never rewrite old hashes to
   make a new HEAD appear qualified. Full suites are not per-field/doc work.
5. Keep record/compound rollback, whole-operation destination publication and
   caller-memory restoration distinct. Verify the promised failure/retry scope;
   use small exact/over-bound and sparse reached-allocation controls instead of
   huge payloads or exhaustive offset sweeps on each iteration.
6. Do not add external drawing fixtures to Git. Use existing eligible LibreCAD or
   libdxfrw repository blobs, or original local-from-scratch recipes with the
   fixture admission proof. Keep downloaded/generated research outputs in scratch.
7. At a qualification checkpoint, freeze the affected source/tool/input bindings
   and finish all qualifying processes before further edits to those bindings.
   Normal checks may finish before the independent ASan build. Reuse them after
   verifying unchanged source/tool/input/configuration and all selected normal
   executable digests; qualify ASan against its own rebuilt executables. Preserve
   actual chronology and configuration differences in the receipt. Another build
   finishing later is not itself a reason to rerun a valid normal result.
   Keep code and unrelated plan-only cleanup in separate commits; do not rebuild
   or rerun a passed cohort solely for excluded documentation changes.
8. Use the existing receipts' measured build/runtime costs to choose the smallest
   sufficient checkpoint. Reuse source recipes and independent physical/typed
   projections across relevant slices; add a new oracle or matrix dimension only
   for an unresolved operation or actual grammar/platform distinction. Review
   later receipts and current symbols before acting on an older `next` list.
   A small baseline probe can link source-matching adapter objects and library
   instead of rebuilding the fixture executable; bind its source, compiler
   command, objects/library and inputs in the receipt. This does not replace the
   final owning tests. The small retarget/unused-offset probes took about 3–4
   seconds. Recent MLINE/LTYPE checkpoints took about 10–12 seconds for an
   incremental normal build, 100 seconds for ASan, 9–18 seconds per two-test cohort
   and 4–7 seconds
   per selected peer run on this machine. These are planning observations, not
   portable performance claims: batch necessary header edits, use normal focused
   checks in the edit loop, and pay the affected ASan build once at readiness.

For each ready slice, its existing receipt must identify the failing input or
regression pair, exact operation/fields, reused provider/planner/output hooks,
focused selector, conditional escalation and binary/source binding. Prepare that
small execution brief before editing. If the witness already passes with the
required proof, remove the local implementation action and retain only genuinely
missing evidence. Do not turn every hypothesis into a new parser or framework.

## Self-updating execution contract

After each implementation item, record exact source/fixture/tool/configuration
and validation evidence in the appropriate receipt, remove the completed action
from this file, update its dependent packet/readiness, and select the next ready
local action. Keep only actual residual work; do not append implementation history
or another copy of this queue.

After each slice commit, show the commit hash, resulting behavior, focused
validation and next packet. Resolve routine implementation choices autonomously.
If an oracle/platform is unavailable, record the precise missing condition once
and continue independently ready work; retry it only after relevant state changes.
Do not stop for routine confirmations or weaken an unresolved gate to proceed.

Completion requires the original intended behavior and evidence, including the
2D/3D consumer contracts, full required document closure and independent/native
qualification. Green counts, a narrowed test subset, removal of claims or the end
of a local slice cannot substitute for that completion audit.
