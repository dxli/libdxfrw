# DWG/DXF parity — remaining implementation plan

Updated 2026-11-05. This file contains open work only. Implementation history and
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

### Implementation loop

Take the next smallest implementation slice whose source evidence and local
dependencies are ready; external or platform-only gates do not block independent
local work. After each slice, run its fast targeted checks, update this queue to
remove completed work and expose any newly isolated residual, add or update the
evidence receipt, then commit the code, plan and receipt together. Report the
commit and the next ready slice, and continue while independent implementation
work remains. Reserve full suites for broad integration or release checkpoints;
generated controls should cover the ordinary edit/retry loop. Keep test DWG/DXF
fixtures out of commits unless they already exist in LibreCAD/libdxfrw or were
created locally from scratch for the specific control.

| ID | State | Remaining result | Dependency |
| --- | --- | --- | --- |
| P4 | OPEN — source and graph closure | The tracked AC1021/AC1027 `ordinary_enc` adapter imports now pass through an exact historical bundled-LibreCAD control profile, with role diagnostics retained; strict output remains refused. Obtain a producer-qualified replacement for the tracked AC1027 ENC fixture’s authored dangling references `0x35`–`0x38`, with real color/reactor/extension-dictionary targets; retain refusal meanwhile. Establish a source- and consumer-backed route for AC1018 empty-name STYLE `0xDF`. Broaden producer/source profiles, raw-record coverage, ordering evidence and native linetype semantics beyond one unique strict AC1024 mechanical source: the `mechanical-alt` pathname has the same SHA-256 as the previously passing `mechanical_example-imperial.dwg`, so its current-code conversion and fresh typed readback are repeat evidence, not an independent source witness. A distinct civil sample reads 335 frames with no entity/object parse failures but strict export refuses on incomplete read disposition. See the [mechanical identity and probe receipt](docs/research/p4-ac1024-mechanical-alt-strict-output-2026-11-05.json); independent consumer readback and producer breadth remain open. The two retained AC1024 ODA proxy projections now establish bounded owner/reference target kinds for ACSH frames and the source-witnessed associative and bare persistent-subentity-manager classes; exact typed/raw frame identity, class identity, owner/reactor roles, emitted census/group matching, and wrong-kind atomic refusal are checked, with generated ASCII/Binary class/proxy readback ([role receipt](docs/research/p4-ac1024-acsh-proxy-owner-reference-role-closure-2026-11-05.json), [persistent-manager receipt](docs/research/p4-ac1024-persistent-subentity-manager-proxy-oda-2026-11-05.json)). This remains a bounded retained-output projection; next select another evidenced nested/raw/OBJECTS target or mixed graph and qualify strict whole-document output/readback when independent source gates permit it. Qualify physical/native GEODATA; generated modern DXF output guards coordinate-type, `UnitsValue`, and scale-method domains, and DWG encoder/writer now omit the coordinate-type field from legacy GEODATA v1 and refuse invalid modern coordinate-type, unit, and scale-method values before encoding ([DWG coordinate-type receipt](docs/research/p4-geodata-coordinate-type-dwg-domain-2026-11-05.json), [DXF unit receipt](docs/research/p4-geodata-unit-value-dxf-domain-2026-11-05.json), [DWG unit receipt](docs/research/p4-geodata-unit-value-dwg-domain-2026-11-05.json), [DXF scale receipt](docs/research/p4-geodata-scale-method-dxf-domain-2026-11-05.json), [DWG scale receipt](docs/research/p4-geodata-scale-method-dwg-domain-2026-11-05.json)); physical producer and native geospatial behavior remain unqualified. Broaden RASTERVARIABLES and WIPEOUTVARIABLES profiles, LayerStates beyond the newly admitted AC1024 empty-root profile, and native SUN lighting. The AC1021 architectural input has a three-root LAYER_CONTROL extension, empty ACAD_LAYERSTATES child, and two empty companion dictionaries; this exact graph now passes source-frame and mutation-snapshot admission for output preparation. Strict whole-source output remains atomically refused at its 36 unpreserved MLEADER proxy caches. An exact selected projection of this source’s three empty LayerStates roots now passes ASCII/Binary output and fresh readback, mutation refusal with destination preservation, and same-adapter retry ([output/readback receipt](docs/research/p4-ac1021-layerstates-empty-root-output-readback-2026-11-05.json)). This closes only that empty-root projection; whole-source eligibility and nonempty or other-producer profiles remain open. The AC1021 architectural candidate’s 36 records are MLEADER proxy caches, not MLINE caches. Source resource-order capture and layer-name mapping now allow strict preflight past the typed-proxy-graphics gate; the current whole-file export stops at the unrelated FIELD/FIELDLIST serializer gate. This source’s 66-member FIELDLIST references 66 FIELD records, but the local reader exposes only 29 typed FIELD shells and 66 raw source frames (37 remain raw-only); unknown CadValue flags and incomplete child framing keep the graph refused ([FIELD gate receipt](docs/research/p4-ac1021-architectural-field-output-refusal-2026-11-05.json)). A generated ASCII/Binary dxfRW control now checks source layer remapping, exact cache bytes, group-92 proxy count plus AcDbMLeader style text-color value, atomic refusal on an unresolved source layer, and same-writer retry. The optional physical-source control now remaps and reads back all 36 AC1021 caches byte-for-byte in generated rooted wrappers in ASCII and Binary; it bypasses only document-wide preflight. Original whole-document export, native rendering, and broader opcode/resource profiles remain open ([remap receipt](docs/research/p5-ac1021-mleader-proxy-resource-index-remap-2026-11-05.json)). A selected projection that clears unrelated FIELD/FIELDLIST carriers reaches BLOCK_RECORD planning and fails because its dependency graph is incomplete; do not admit it until a dependency-closed block projection passes atomic refusal/retry and fresh ASCII/Binary readback ([candidate receipt](docs/research/p4-ac1021-layerstates-empty-root-candidate-2026-11-05.json)). A follow-on scratch probe retaining the 43 source BLOCK_RECORD/BLOCK containers advances past that gate, then identifies four VIEWPORTs with zero frozen-layer handles (13 unresolved slots total), dangling LAYOUT viewport links after those entities are omitted, SORTENTSTABLE reciprocity, dynamic-block, NOD/DICTIONARYWDFLT, named-resource and CELLSTYLEMAP/TABLESTYLE gates. A diagnostic variant that removes only the unrelated TABLESTYLE/CELLSTYLEMAP objects advances again but reaches 16 decoded AC1021 VISUALSTYLE records with no raw carriers. The bounded AC1021 typed writer now passes exact 16-style payload comparison against LibreDWG 0.14 and fresh selected-graph ASCII/Binary typed/raw readback. A hand-authored AC1021 body vector also verifies reserved-double decode and byte-exact body/string/handle replay; see the [VISUALSTYLE source/output receipt](docs/research/p4-ac1021-visualstyle-source-output-2026-11-05.json). This closes only that source/output route. Neither scratch variant emits DXF; both preserve the destination sentinel. Keep the broader projection unqualified until retained viewport/layout and remaining source dependencies close and ASCII/Binary readback passes ([downstream-gates receipt](docs/research/p4-ac1021-layerstates-projection-next-gates-2026-11-05.json)). The generated AC1027 direct-NOD and nested-dictionary RASTERVARIABLES selectors now pass ASCII/Binary refusal, correction and same-adapter retry under a selected-graph scope; they deliberately do not require unrelated whole-source reference closure ([updated control receipt](docs/research/p4-rastervariables-dxf-scalar-domain-2026-11-05.json)). Keep the documented named-block SORTENTSTABLE and nonconforming VIEW/SUN routes refused; these are compatibility boundaries, not open owner-mapping tasks. | ODA source layout, exact target kind/owner evidence, and complete typed readback; preserve atomic refusal for unclosed graphs. |
| P5 | OPEN — dynamic-block semantics, rendering and TABLE profiles | Broaden physical producer/version coverage and qualify native constraint-variable/evaluation semantics beyond the now-tested AC1024 mechanical dynamic-constraint DXF projection ([receipt](docs/research/p5-ac1024-dynamic-constraint-dxf-profile-2026-11-05.json)); independent ezdxf recognition/audit is transport evidence only. Establish native evaluation for the AC1024 dynamic-block graph after P4 owner closure. Resolve remaining FIELD flags/child profiles and AcObjProp spans/handle roles before output; qualify physical producer/version coverage and native FIELD evaluation. Qualify physical nonzero TABLEGEOMETRY source spans and target closure; resolve remaining CONTENTFORMAT/CELLSTYLEMAP combinations, overlapping edge semantics and remaining AC1018 TABLE profiles. For VIEWPORT overrides, obtain eligible physical producer evidence for the implemented 335/343/344 fields and qualify native activation/rendering; resolve the 91 type/value conflict and binary encoding before modeling it. | P4 graph closure where required; physical source evidence and native consumer evidence for semantic claims. |
| P6 | OPEN — version, schema and semantic residuals | The typed AC1024 ACSH Loft/Revolve parser still lacks independent semantic field bindings for the reproduced bodies. ODA emits no axis/direction/angle values, so do not modify those parsers from proxy evidence. The former claim that ODA supplied no body-span or following-handle transport evidence is superseded for the two sampled inputs: ODA group-161 spans match each captured AC1024 frame after omitting its verified terminal zero bit, and the ordered typed handle roles/values match ([transport receipt](docs/research/p4-ac1024-acsh-oda-raw-proxy-transport-2026-11-05.json)). The conference-room source has three REVOLVE body failures and the condominium source has two LOFT plus one REVOLVE body failure. Their raw AC1024 source frames now publish under the bounded proxy path while `objectParseFailures` remains three and semantic coverage remains partial-observed; strict AC1027 export passes the read-completeness gate and stops at the independent SUN same-version gate ([raw recovery receipt](docs/research/p4-ac1024-acsh-opaque-reader-recovery-2026-11-05.json)). Keep semantic/ACIS claims closed until independent fields and kernel evidence exist. Explain the AC1027 Cover suffix and AC1032 resave HEADER suffix; keep strict output refused while bytes remain unclaimed. A 2026-11-05 ODA File Converter 27.1.0.0 repeatability check resaved both the suffix-free Line source and Cover: each output has a 739-byte suffix after its validated HEADER trailer. Fresh and prior suffixes differ at every byte position, but their low bits match at every position across repeats of the same source and invert across these two sources. This narrows the observed relation to source-dependent ODA output; the bytes remain unclaimed and strict output remains refused ([repeatability receipt](docs/research/p6-ac1027-oda-header-suffix-repeatability-2026-11-05.json)). The current reader publishes 119/119 frames with zero entity/object parse failures; strict DXF preparation still refuses because source references are not fully qualified. No support claim changes; see the updated [resave receipt](docs/research/p6-ac1027-cover-oda-same-version-resave-2026-11-03.json). The previously noted AC1024 `readDwgClasses` high-bit `strDataSize` failure does not reproduce in the nine unique AC1024 files currently available under `/Users/dli/doc/dwg`; every current trace reaches the class-stage END marker, while later strict-output failures are unrelated. A producer-qualified RTM reproducer is still needed before modifying the footer parser ([corpus rerun](docs/research/p6-ac1024-classes-available-corpus-rerun-2026-11-05.json)). DIMASSOC main-handle vectors now survive parsing, the typed model, bounded type-0 DWG encoding, and ASCII/Binary DXF output; the local AC1021 architectural profile transports 7/7 selected records ([vector receipt](docs/research/p6-dimassoc-main-handle-vector-transport-2026-11-05.json)). A peer scan of 28 unique local DWGs found one two-handle main reference and no multi-handle intersection reference; corpus absence does not set a format maximum ([cardinality scan](docs/research/p6-dimassoc-corpus-cardinality-2026-11-05.json)). LibreDWG 0.14 minJSON independently resolves all 23 AC1018 DIMASSOC objects and all 34 geometry references in Extruder2, and all seven AC1021 objects and ten geometry references in the architectural sample; the latter's five `dimensionobj` targets are `AcDbMLeader`. Autodesk's AcDbDimAssoc API documents dimension-or-leader updates, so the adapter now accepts LEADER/MLEADER targets as well as dimension subtypes ([target-role receipt](docs/research/p6-dimassoc-independent-target-roles-2026-11-05.json)). This establishes bounded target-role transport evidence, not native update semantics, broader producer/version coverage, or whole-document closure. The generated adapter closure now also checks each non-null DIMASSOC common owner, reactor and extension-dictionary handle against one emitted DICTIONARY OBJECTS record, enforces reactor-count/vector agreement and uniqueness, and delays DIMASSOC output until dictionary records have been inventoried. ASCII/Binary controls cover dangling links, destination preservation, correction/retry and typed readback; physical source probes remain low-level transport only ([adapter closure receipt](docs/research/p6-dimassoc-adapter-closure-2026-11-05.json)). LibreDWG 0.14's DXF-to-DWG route warns that DIMASSOC is unsupported and its AC1024 resave drops the object, so that route is excluded as an output oracle (see the same receipt). Qualify TABLEGEOMETRY nonzero source spans, uncovered revision/route profiles, associative semantics, HATCH/MPOLYGON/MESH, annotation/geometry, units/inheritance, and unknown/proxy data. Finish remaining safety/caller-state and CLI/oracle contracts without broad full-suite runs. The corrected ODA mirrored CRC table and remainder order match the original AC1021 tablet page-map/header-page CRCs and its ODA resave; further source/version coverage remains open. For AC1021, qualify graphs without the explicit default-material and plotstyle opt-ins, broader entity profiles and native material semantics; the sampled opt-in LINE passes ODA ([material receipt](docs/research/p6-ac1021-default-material-optin-2026-11-04.json), [VPORT receipt](docs/research/p6-ac1021-synthetic-vport-null-roles-2026-11-04.json)). | Read ODA v5.4.1 before DWG layout changes; trace current source failures; use focused fast controls and retain unknown data. |
| P7 | OPEN — ACIS/ACDS and independent 3D meaning | Complete DWG entity-to-owner/key-to-AcDs segment-to-SAT/SAB associations, versioned framing/index/suffix/prototype assembly, and exact output binding. Qualify placements, units/transforms, topology, trims, periodicity and spline meaning. Obtain independent kernel measurements for each claimed profile; otherwise refuse required 3D data atomically. | P4 graph/output bindings and a genuinely independent semantic/kernel reader for the claimed profile. |
| P8 | OPEN — consumers, platforms and release | Qualify LibreCAD and LibreCAD_3 callback/default behavior on a composed candidate; run native Linux/Windows FreeCAD open/insert checks; qualify intended 3D consumers, ABI, packaging and release contracts. Local generated probes do not close these gates. | Source-stable candidate plus native platform access and recorded consumer/tool versions. |
| P9 | DEFERRED_EXTERNAL — six non-promoting items | Resolve J256, J260, J268, J284, J293 and J295 only through the retained successor registry and eligible independent evidence. J268 has recovered historical path/status rows but still needs original input-byte identities and digest; J284 needs its 11 target-debt/integrity blockers; J295 needs native receipts for four RTEXT position/rotation claims. Preserve each item's lineage and exclusions. | Follow `metadata/external-evidence-resolution-v1.json`; external/advisory results cannot promote support claims by themselves. |

P6–P9 qualification can proceed when its inputs are ready; it does not block
independently ready local code work.

The AC1021 proxy streams contain 15 type-19 `SUBENT_MARKER` chunks. ODA §29
defines these as marker-index changes for subsequently drawn items. The decoder
now reads the RL marker and sends an ordered callback scoped to the source host
handle; DWG block journaling retains that callback order. Generated controls
cover marker transitions, unhandled sinks and malformed lengths. The source
profile still needs native confirmation of marker-to-geometry behavior and
source-cache association, exact DXF output/readback, and consumer qualification
before it advances. Strict output remains refused
([source and peer receipt](docs/research/p6-ac1021-metric-proxy-graphics-strict-refusal-2026-11-05.json)).

Modern GEODATA DXF versions 2 and 3 now admit only documented coordinate types
0–3 on output; tolerant input retains other signed group-70 values. Generated
ASCII/Binary refusal, same-writer correction/readback and tolerant-input
controls pass. Physical whole-document and native geospatial qualification
remain open ([receipt](docs/research/p4-geodata-coordinate-type-dxf-domain-2026-11-05.json)).

The bounded AC1018 direct-NOD frame-0 WIPEOUTVARIABLES graph has selected-source
and generated ASCII/Binary output evidence; physical whole-document output,
other graph shapes and native wipeout behavior remain open
([receipt](docs/research/p4-ac1018-wipeoutvariables-direct-nod-2026-11-04.json)).
The bounded AC1018, AC1024, AC1027 and AC1032 direct-NOD profiles also admit
frame 1 with its witnessed 10-bit field encoding; physical whole-file output
and native rendering remain open ([AC1018 receipt](docs/research/p4-ac1018-wipeoutvariables-frame1-direct-nod-2026-11-04.json),
[AC1024 receipt](docs/research/p4-ac1024-wipeoutvariables-frame1-direct-nod-2026-11-04.json),
[AC1027/AC1032 receipt](docs/research/p4-ac1027-ac1032-wipeoutvariables-frame1-direct-nod-2026-11-04.json)).
The generic DXF codec now reads and retains WIPEOUTVARIABLES group 90, emits
the documented fixed class version 0, and refuses unsupported output versions
before reserving identity. Source-origin graph profiles, physical whole-file
output and native wipeout behavior remain open ([class-version receipt](docs/research/p4-wipeoutvariables-dxf-class-version-2026-11-05.json)).
The DWG writer also refuses WIPEOUTVARIABLES display-frame values outside the
documented 0/1 domain before reserving identity; input remains tolerant. See the
[DWG scalar-domain receipt](docs/research/p4-wipeoutvariables-dwg-scalar-domain-2026-11-05.json).
A dedicated fast selector covers invalid values 2 and 65535 independently of
the broader local DWG roundtrip cohort.
The bounded AC1018 direct-NOD RASTERVARIABLES graph covers the witnessed
0/0/1/3 and 0/1/1/3 profiles in generated ASCII/Binary output; broader graphs,
physical whole-document output and native image behavior remain open
([frame-0 receipt](docs/research/p4-ac1018-rastervariables-direct-nod-2026-11-04.json),
[frame-1 receipt](docs/research/p4-ac1018-rastervariables-frame1-direct-nod-2026-11-04.json)).
The AC1024 and AC1032 direct-NOD profiles additionally cover source-witnessed
frame=1. The physical AC1021 `children-room-decoration.dwg` record now seeds a
locally authored dependency-closed wrapper DWG; its captured fields pass wrapper
re-import, ASCII/Binary DXF output, atomic invalid-value refusal, same-adapter
retry and typed graph readback ([physical writer bridge receipt](docs/research/p4-ac1021-rastervariables-physical-writer-bridge-2026-11-05.json)).
This is not direct or whole-source output from the physical drawing. A whole-source
conversion attempt stops at the separate ModelDocViewStyle source-version gate.
Source-origin admission outside witnessed tuples, broader graphs, physical
whole-document output and native image behavior remain open. The DWG and DXF
writers admit only Autodesk's frame/quality/unit domains (and DXF class version 0); input preserves
unsupported values. This does not
expand the source-origin adapter profile. The DWG object writer now checks the
same domains before common-state preparation and identity reservation; a
same-writer invalid-frame correction, output and readback control is included
with the focused refusal selector. See the [AC1024 frame receipt](docs/research/p4-ac1024-rastervariables-frame1-direct-nod-2026-11-04.json),
[AC1032 frame receipt](docs/research/p4-ac1032-rastervariables-frame1-direct-nod-2026-11-04.json),
the [DXF scalar-domain receipt](docs/research/p4-rastervariables-dxf-scalar-domain-2026-11-05.json),
and [DWG writer receipt](docs/research/p4-rastervariables-dwg-scalar-domain-2026-11-05.json).

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

Separate unresolved gates remain: AC1021 proxy-graphics refusal belongs to P6. A distinct `blocks_and_tables_-_metric.dwg` AC1021 corpus input reads, then strict output initially refused two typed `AcDbTable` proxy-graphics records (`0x2856E` and `0x2A974`) before creating a DXF. The DWG trace places each graph-data length and byte stream inside that host entity's common body (`graphFlag=1`, 32,980 and 37,080 bytes); both records report model-space mode and no block-parent handle. This corrects the earlier INSERT classification and establishes host-entity attachment for these two inputs, not native display or replayability. Both cached streams have complete declared-byte framing. The source inventory isolates 15 opcode-19 chunks; ODA v5.4.1 §29 identifies these as `SUBENT_MARKER` with an RL marker index, and pinned ezdxf 1.4.5 independently decodes both extracted streams to 231 and 259 POLYLINE/TEXT virtual entities. Ordered marker callbacks are retained in the DWG output journal. A bounded same-revision `AcDbTable` raw-cache route now admits only explicitly authorized parsed-DWG bytes with complete chunk framing; `dx_iface` authorizes its retained typed TABLE copy and clears the duplicate cache on the INSERT-shaped 2D projection. Autodesk's [TABLE DXF definition](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-D8CCD2F0-18A3-42BB-A64D-539114A07DA0.htm) assigns group 92 to the proxy byte count and group 310 to its data. On the source AC1021 drawing, strict preflight now passes the proxy-cache gate and stops at the unrelated FIELD/FIELDLIST serializer gate before writing. A generated one-cell TABLE proves exact 84-byte proxy group-310 ASCII/Binary write/readback, wrong-version atomic refusal, and same-adapter retry. The two physical-source caches also pass exact ASCII and Binary byte readback inside generated valid TABLE wrappers; original TABLE semantic output, whole-source conversion, and native marker-to-geometry behavior remain unqualified. Preserve refusal for profiles without the required route and evidence ([receipt](docs/research/p6-ac1021-metric-proxy-graphics-strict-refusal-2026-11-05.json)).
The physical AC1018 ET Border source contains empty-name STYLE `0xDF` with font
`ltypeshp.shx`. The current libdxfrw parse gives flags 68 (xref-reference plus
vertical; shape false); LibreDWG 0.14 decodes the same bytes as shape true,
vertical false, and omits this row from its DXF output. ODA v5.4.1 §20.4.56
orders the AC1018 bits vertical then shape, matching the current parser branch,
while LibreDWG's schema reads shape then vertical. Autodesk's DXF reference
defines bit 1 as a shape record and says only group 3 is meaningful for a
shape-file LOAD request ([STYLE reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-EF68AF7C-13EF-45A1-8175-ED6CE66C8FC9.htm)).
This is an unresolved source-layout disagreement, not permission to omit or
rename the row. Keep strict output refused until the physical bit order and
the source/consumer disposition are resolved. Also qualify the AC1021+ bit order:
ODA lists the bits in a common section without a version reversal, LibreDWG uses
the opposite order across R13+, and the current parser and writer reverse their
order at AC1021. Producer-authored controls now show swapped local flags at
AC1015/AC1018 and matching flags at AC1021/24/27/32; an ODA conversion of ET
Border emits STYLE `0xDF` as a shape in both AC1018 and AC1027 DXF. This conflicts
with the required ODA DWG specification, so obtain an authoritative correction
before changing the bit order. See the [producer differential](docs/research/p4-oda-style-bit-order-ac1015-ac1032-2026-11-03.json)
and [original receipt](docs/research/p4-ac1018-empty-style-dwg-bit-order-differential-2026-11-03.json).

## P4 — original bodies and complete document closure

**Open strict-output defect:** a CRC-resealed AC1027 source with layer 0's
linetype handle changed to a missing target previously exported successfully
with `CONTINUOUS` substituted. Strict and partial output is now conservatively
refused for aggregates that have imported AC1027 DWG, until source closure is
proven ([guard receipt](docs/research/p4-ac1027-strict-graph-interim-guard-2026-11-04.json)).
The tracked `ordinary_enc` AC1021/AC1027 fixtures now import through the
concrete adapter: the reader admits the historical bundled LibreCAD control
frame profile, including its inline AC1021 BL-count form and code-4 LTYPE /
BLOCK_CONTROL members, while preserving role diagnostics. Strict output remains
refused; the AC1027 ENC fixture's references `0x35`–`0x38` have no target
objects, and the AC1021 fixture has not passed the strict-complete gate. This
is read compatibility for the identified historical writer, not producer
interoperability or output qualification ([receipt](docs/research/p4-libreCAD-historical-control-read-profile-2026-11-05.json)).
Keep the selected one-layer graph's output
qualification open for physical source conversion: generated entity and layer
LTYPE references now bind to same-import rows, but no whole-source producer
conversion has yet reached this output gate. Keep dangling and wrong-kind
source cases refused.
Common entity LAYER and explicit LTYPE lookups now report unresolved or
truncated non-null handles to the integrity ledger while preserving tolerant
reads; the original wire role is retained and checked for these three hard
pointer relationships while tolerant callbacks remain unchanged
([receipt](docs/research/p6-dwg-entity-table-reference-diagnostics-2026-11-04.json)).
The deferred table pass now applies the same missing-target and 64-bit alias
diagnostic to LAYER→LTYPE resolution while retaining tolerant fallback names;
the hard-pointer role is also checked. Strict/partial output now binds each
typed LAYER group-6 name to its same-import hard-pointer target and the exact
emitted LTYPE row in the staged TABLES transaction. Null, missing, wide-alias,
wrong-role and mismatched-name references refuse atomically; generated
ASCII/Binary output reads back the layer name and supports same-codec retry.
This is generated output evidence only; see the
[layer output receipt](docs/research/p4-dwg-layer-ltype-output-binding-2026-11-04.json)
and the [source diagnostic receipt](docs/research/p4-dwg-layer-linetype-resolution-diagnostic-2026-11-04.json).
LTYPE_CONTROL member membership now has a source-generation-bound adapter
check: each non-null ordered control slot must match exactly one typed LTYPE
row owned by that control, names must be unique, CONTINUOUS must be in a
counted slot, and named BYBLOCK/BYLAYER rows must occupy the two final phantom
slots in either witnessed order. Source numeric handles are not required to
match this library’s canonical writer handles. Strict and bounded-partial DWG
output refuse missing, duplicate, null, wrong-owner or mismatched membership.
The generated acceptance/rejection matrix passes. A physical AC1024 sample
exposed the incorrect fixed-handle assumption; after correcting it, strict
whole-source output and fresh DXF readback pass for that one drawing, including
all 228 opaque callback qualifications and 54 geometry-dependency records.
See the [membership receipt](docs/research/p4-ltype-control-typed-membership-2026-11-04.json)
and [physical-source receipt](docs/research/p4-ac1024-mechanical-strict-ltype-source-handles-2026-11-04.json).
The generated output gate binds every accepted source row to exactly one
serialized LTYPE row using the writer's source-target handle, exact output name,
LTYPE table handle and owner. It retains the binding through write finalization
and refuses a target-registry mutation after TABLES inspection. Fast controls
cover seven target revisions in ASCII and Binary, wrong/missing/extra/duplicate
rows, an ambiguous alias and a post-inspection alias mutation, with atomic
destination preservation and same-codec retry. This is generated identity
evidence. The existing AC1015 sample still does not pass its whole-read
completeness gate; the AC1024 mechanical sample now reaches and passes the
strict output inspector under its separately witnessed profile. Typed entity
and LAYER name references now bind to those emitted rows. Raw entity group-8/6 and XDATA group-1001/1003 names also
bind under generated AC1027 ASCII/Binary controls. A generated AC1018 ANSI_1252
ASCII/Binary control verifies non-ASCII group-6/8 and XDATA group-1001/1003
names through readback. A separate 64-profile generated control now has its own
fast CTest entry for group-347 MATERIAL and optional group-348 VisualStyle emitted-target
binding/readback, plus common-reference mutation rollback/retry checks. Other
physical whole-source profiles, broader raw-record profiles, producer ordering
beyond witnessed ODA profiles and native linetype semantics remain open (see the [common-field
receipt](docs/research/p4-raw-entity-common-table-name-output-closure-2026-11-04.json),
[XDATA receipt](docs/research/p4-raw-entity-xdata-table-name-output-closure-2026-11-04.json)
and [legacy-codepage receipt](docs/research/p4-raw-entity-cp1252-name-output-readback-2026-11-04.json)).
The generated HEADER `$CMATERIAL` control is now registered as a focused fast
CTest and refreshes its direct-writer mapping vector for current object-handle
remapping: the `2 → EA02 → EA12` chain refuses when the one-hop HEADER target
does not identify the emitted MATERIAL, while source `EA02` and object `EA12`
close at output handle `EA12`. Both encodings pass the 80-profile facade matrix,
68 callback refusals, 248 corrected retries, 52 malformed/target refusals, 16
staged-byte refusals, six direct-writer profiles, eight inspector guards and
128 persistent allocation-failure rollback/readback cycles. This verifies local
identity and transaction behavior, not producer coverage or native material
semantics; see the [HEADER material receipt](docs/research/p4-header-material-one-hop-and-persistent-rollback-fast-2026-11-04.json).
The adapter's LAYER group-347 material graph control is also registered as a
focused fast CTest. Across AC1021/AC1024/AC1027/AC1032 and ASCII/Binary it
checks 32 generated profiles, emitted MATERIAL and owner binding, fresh typed
readback, 82 callback rollbacks, 108 corrected retries and staged-table
mutation refusal. This remains generated graph evidence, not physical producer
or native renderer qualification; see the [LAYER material receipt](docs/research/p4-layer-material-graph-closure-adapter-fast-2026-11-04.json).
Raw H member roles are retained. The first null ordered slot now emits a
source-anchored `TableControlNullMember` Error, and the first non-null role
mismatch emits `TableControlHandleRole`; tolerant callbacks remain unchanged.
Generated controls cover both counted and phantom null slots. This diagnoses
incomplete membership without qualifying or repairing it. See the
[role diagnostic receipt](docs/research/p4-ltype-control-source-member-role-diagnostics-2026-11-04.json)
and [null-member receipt](docs/research/p4-ltype-control-null-member-diagnostic-2026-11-04.json).

Typed entity LTYPE references now close against the source and emitted table
binding before the entity is serialized. Explicit non-null handles must map to
one current-import LTYPE whose source name is exactly the entity's resolved
name, and whose emitted target remains registered. Nested VERTEX/SEQEND and
ATTRIB/SEQEND records receive the same check. Null-handle modes are admitted
separately: ODA v5.4.1 §20.4.1–§20.4.2 permits only BYLAYER for AC1012/AC1014's
`Isbylayerlt` bit, and BYLAYER/BYBLOCK/CONTINUOUS for AC1015+. Generated
ASCII/Binary controls cover seven DXF targets, malformed explicit name/target
references, empty/custom null-handle names, the versioned implicit modes,
atomic refusal, same-codec retry and typed readback. See the
[entity-reference receipt](docs/research/p4-ltype-entity-reference-output-closure-2026-11-04.json).
The staged TABLES inspector also closes each typed DWG LAYER's group-6 name
through its source hard-pointer to the exact emitted LTYPE row; a null source
handle is refused because no implicit layer mode is qualified. Generated
ASCII/Binary output and readback include wrong-role, wide-alias, dangling and
name-mismatch refusal with same-codec retry
([layer output receipt](docs/research/p4-dwg-layer-ltype-output-binding-2026-11-04.json)).
Typed entity group-8 layer names now bind through the retained entity hard-pointer
to the exact emitted LAYER row before serialization; nested VERTEX/SEQEND and
ATTRIB/SEQEND records receive the same check. Generated ASCII/Binary controls
cover seven target revisions, missing/null/wide/wrong-role/wrong-kind handles,
name mismatch, post-inspection output-alias mutation, atomic refusal, same-codec
retry and typed readback ([entity-layer receipt](docs/research/p4-dwg-entity-layer-output-binding-2026-11-04.json)).
The writer's fixed synthetic layer-0 row is never accepted as a source binding.
Common raw entity group-8 LAYER and group-6 LTYPE names, plus XDATA
group-1001 APPID and group-1003 LAYER names, now bind at staged TABLES
inspection to one unique, qualified emitted row. Generated AC1027 ASCII/Binary
controls cover missing-row refusal, atomic destination preservation, same-adapter
correction/retry, ASCII-case-insensitive names and lossless nested application
and XDATA payloads ([common-field receipt](docs/research/p4-raw-entity-common-table-name-output-closure-2026-11-04.json),
[XDATA receipt](docs/research/p4-raw-entity-xdata-table-name-output-closure-2026-11-04.json)).
Application groups and other XDATA fields remain outside the name check; these
name-only references do not prove source-handle identity. Physical whole-source
conversion, producer ordering, native linetype rendering and broader raw entity
profiles remain open. Audit common entity references systematically before
qualifying the one-layer physical output. Nondefault owner, reactor, extension dictionary, color, material, plotstyle,
visual-style and DWG EED APPID/LAYER references require independent closure
proof or refusal. Audit referenced color, text/dimension style, block
record and MLINESTYLE lookup fallbacks as well; class-specific gates do not
automatically establish closure for every entity route.

Earlier selected generated LINE conversions exercised a bounded one-layer
AC1027 default-material/plotstyle graph in ASCII and Binary. Reopen strict
output only after the unresolved references above are closed. The broader
physical producer contains an additional layer; arbitrary materials, extra
layers, other versions and native FreeCAD behavior remain open
([receipt](docs/research/p6-ac1027-default-graph-strict-dxf-2026-11-04.json)).
Strict same-version DXF repeat is bounded to the selected one-layer graph and
its source-bound CLASS and dictionary records; broader source shapes,
cross-version output and generic plotstyle fallback remain unqualified
([repeat receipt](docs/research/p4-ac1027-layer-plotstyle-dxf-repeat-2026-11-04.json)).

An exact AC1027 compact raw-only MATERIAL profile now passes strict CLI replay
for a selected producer-derived default-material graph, with source and graph
mutation refusal. The full source remains blocked by separate SECTIONOBJECT
proxy data; other sparse profiles and general MATERIAL semantics remain open
([receipt](docs/research/p4-ac1027-compact-material-dxf-raw-graph-2026-11-04.json)).

DXF DICTIONARY now accepts its hard-owner flag after completed entries within
the documented subclass, preserving each 350/360 role and rejecting a flag
inside an incomplete pair. Broader graph and native consumer qualification
remain open ([receipt](docs/research/p4-dxf-dictionary-late-hardowner-2026-11-04.json)).

SECTIONOBJECT contour parsing now retains complete vertices when their count
groups appear later and rejects mismatched or ambiguous arrays before record
callbacks. Omitted counts remain accepted for compatibility, though the sampled
ODA reader drops those arrays; native rendering remains unqualified
([receipt](docs/research/p4-dxf-sectionobject-vertex-count-order-2026-11-04.json)).

SECTIONOBJECT AcDbSection group 411 no longer disappears through a typed
projection: default consumers refuse the record, while an explicit raw-only
policy delivers it for caller-managed same-version replay. Its color semantics,
graph closure and automatic CLI admission remain open
([receipt](docs/research/p4-dxf-sectionobject-411-raw-only-2026-11-04.json)).

P4 supplies source/context/role providers and composes them with the existing
local record providers and P5 output. Do not create parallel capture or graph
engines.

Two ODA back-saved AC1018 drawings now read their bounded type-4/unit-0
CELLSTYLEMAP records with correct inline handle boundaries; whole-file output
remains refused by the HEADER suffix and unqualified graphs. Broader producer,
type/unit and rendering profiles remain open ([receipt](docs/research/p4-ac1018-cellstylemap-oda-roundtrip-read-2026-11-03.json)).

**DICTIONARY DWG item-wire role:** ODA v5.4.1 §20.4.44 and LibreDWG's DWG
schema define each item handle as a soft-owner wire reference (code 2),
independent of the `hardOwner` flag. The flag remains relevant to DXF entry
ownership projection. The prior local code and generated code-3/hardOwner=1
vector were incorrect; the parser and writer now use code 2, with a focused
cross-version encoder control and AC1018 physical-source parse proof. This does
not close dictionary graph output or named-block SORTENTSTABLE owner scope; see
the [correction receipt](docs/research/p4-dictionary-item-wire-code-independent-of-hardowner-2026-11-03.json).

**BLOCK_RECORD INSERT references and ENDBLK callbacks:** DWG block-footer
publication now carries the parsed `DRW_Block` through both the journalled
event route and direct fallback, so `dx_iface` receives exact ENDBLK identity
and owner fields. Strict output accepts a missing INSERT owner only when the
source fixed-space context proves ModelSpace or PaperSpace; unknown owners
remain atomic refusals. The BLKREFS path now keeps emitted-record identity
inventory enabled through write setup. Generated AC1015/AC1027 footer controls
and an ASCII/Binary adapter control cover invalid-owner refusal, destination
preservation, same-adapter corrected retry, exact BLKREFS-to-INSERT handle
readback and fixed-space ownership. The physical ET Border conversion now
passes BLOCK_RECORD planning but remains refused in the STYLE table before
reaching its named-block SORTENTSTABLE. See the [BLOCK/ENDBLK receipt](docs/research/p4-dwg-endblk-capture-ownerless-fixedspace-insert-2026-11-03.json).

**DIMENSION and LEADER DIMSTYLE binding:** Generated controls prove the bounded
group-3 projection and fail-closed behavior. Resolved hard pointers map to the
exact DIMSTYLE names; missing non-null targets stay unresolved and strict full
conversion refuses them atomically. LEADER DWG output now writes its modeled
DIMSTYLE reference instead of forcing STANDARD; generated custom-style source
write/read resolves that target across AC1015/18/21/24/27/32. ASCII/Binary strict
DXF output, destination preservation, same-adapter correction retry, and typed
readback pass on AC1024. This adds no independent physical-producer or
native-rendering qualification; see the [DIMENSION receipt](docs/research/p4-dimension-dimstyle-reference-fail-closed-2026-11-03.json)
and [LEADER receipt](docs/research/p4-leader-dimstyle-reference-fail-closed-2026-11-03.json).

1. **Source admission:** open another associative parser/schema packet only when
   a physical source identifies a required nonempty or mismatched body,
   action-dependency, owned-parameter, or versioned-registry profile. Prove its
   class, target role, owner route, fields and bounded span before decoding; reuse
   qualified providers. Keep version-2 nonzero-seed registry layouts raw until
   their framing is proven. Keep investigation DWGs out of committed fixtures;
   bind regressions to eligible repository inputs or locally authored fixtures.
   IMAGE input retains unambiguous incomplete clipping boundaries through
   recompiled callback copies; output refuses them until explicit caller repair.
   Missing group 71 without another contradiction remains tolerated and needs
   producer evidence. Older clients compiled with the former inline copy may
   discard the marker. See the [IMAGE boundary receipt](docs/research/p4-image-clip-boundary-output-guard-2026-11-03.json)
   and [copy-state receipt](docs/research/p4-image-incomplete-dxf-boundary-copy-guard-2026-11-03.json).
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
   Named-block HATCH/MPOLYGON graph admission also checks that each selected
   explicit owner matches the containing BLOCK; broader physical target,
   boundary geometry and native fill semantics remain open. See the [containment
   receipt](docs/research/p4-hatch-named-block-owner-containment-2026-11-03.json).
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
   refuses DBCOLOR output until the physical source, exact `ACAD_COLOR` entry
   semantics, EED/APPID dependencies, and fresh DXF readback are qualified (see the
   [external-evidence gate receipt](docs/research/p4-dbcolor-external-evidence-gate-2026-11-03.json)).
   Research confirms that Autodesk exposes a color dictionary containing
   `AcDbColor` entries and that the root drawing dictionary uses the
   `ACAD_COLOR` key, while the Open Design Specification describes entity
   color references but does not specify the complete DBCOLOR object body or
   dictionary-key serialization. LibreDWG's object schema independently
   models DBCOLOR as common parent plus CMC color; this corroborates the field
   family, not producer interoperability. An existing AC1021 source and ODA
   resave now expose a physical color-book dictionary, owner/reactor edges and
   matching RGB/name; the source provenance and exact field span remain open.
   ODA resave and DXF projection each drop one repeated EED group, so do not
   assume those conversions preserve the original graph. The low-level DXF
   codec now retains an explicitly present ACI fallback alongside RGB; retain
   the original EED and qualify whole-graph output/readback and native color-book
   meaning before admission. See the [physical graph receipt](docs/research/p4-dbcolor-ac1021-physical-oda-graph-and-precedence-2026-11-03.json).
   A bounded optional side channel preserves the source APPID/LAYER handle roles
   before name resolution. The physical witness now passes source-dependency
   checks and selected ASCII/Binary graph readback with all 13 original EED
   tuples; bind every emitted source target in the complete drawing and qualify
   fallback and native behavior before admission. See the [dependency receipt](docs/research/p4-dbcolor-eed-dependencies-selected-graph-2026-11-03.json).
   ODA counterexamples show the apparent match between an EED integer and
   emitted ACI 254 is incidental; do not infer the fallback from that value.
   Simple nearest-RGB calculations using two published palettes also disagree
   with multiple ODA outputs. Determine the actual mapping before synthesizing
   a fallback; see the [EED differential](docs/research/p4-dbcolor-oda-fallback-eed-counterexamples-2026-11-03.json)
   and [palette differential](docs/research/p4-dbcolor-oda-palette-differential-2026-11-03.json).
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
   The adapter now captures bounded SECTIONMANAGER/SECTIONSETTINGS callbacks,
   snapshots their source DXF carriers, and admits a generated AC1021+ graph
   only when manager members resolve to unique SECTIONOBJECT entities,
   SECTIONOBJECT group-360 references resolve to retained settings, nested
   source handles resolve to entities, destination handles resolve to unique
   BLOCK_RECORDs, and owners fit the rooted/reciprocal profile. Duplicate,
   orphaned, opaque-tail, changed-raw, and unmodeled-common-data cases refuse
   atomically. Generated AC1027 ASCII/Binary controls cover nested geometry,
   remapped manager/settings/source identities, fresh readback, and same-adapter
   retry ([graph receipt](docs/research/p4-section-settings-adapter-graph-closure-2026-11-03.json)).
   This closes that generated transport profile only; physical producer field
   spans/owner binding and native section-generation semantics remain open.
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
   `POINTCLOUDEX` direct DXF output now refuses a zero source definition or
   reactor ID, and refuses either reference when resolution/remapping produces
   zero, before staging record bytes. Autodesk's `PointCloudEx` API requires
   the linked definition and reactor. Generated ASCII/Binary controls cover
   each unresolved edge, either missing source edge, destination preservation,
   and corrected output with a fresh writer ([receipt](docs/research/p4-pointcloudex-required-reference-output-guard-2026-11-03.json)).
   This closes only nonzero admission for these required IDs; target kind and
   existence, ownership, recursive graph closure, physical producer breadth,
   point-cloud asset loading, and native-consumer semantics remain open.
   Continue the pointer-role audit only for other supported 3D fields not
   covered by current receipts. Correct output IDs alone do not establish
   target kind, ownership, recursive closure, physical producer breadth, or
   native-consumer semantics.
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

An independent LibreDWG 0.14 `dxf2dwg` attempt on the strict mechanical-alt
output stops before producing a DWG: it treats SORTENTSTABLE body group 5 as a
duplicate record handle and does not implement the emitted
`AcDbAssocValueDependency` subclass. The matching handle is the documented
SORTENTSTABLE sort key, not a second object identity. This run supplies no
readback and does not establish a libdxfrw output defect; consumer qualification
for this profile remains open ([peer rejection receipt](docs/research/p4-mechanical-alt-libredwg-dxf-peer-rejection-2026-11-05.json)).

FreeCAD 1.1.4's native C++ importer does open the same strict AC1024
mechanical-alt output through the exact current `dwg2dxf` executable: 96 entities
were read and 75 FreeCAD objects created. The profile reports two unsupported
VIEWPORT records and two unsupported aligned dimensions; text/dimension and
layout import were disabled. The original audit emitted a LayerStates profile
skip; the bounded AC1024 empty-root profile below removes that skip for this
source only. This is a selected macOS consumer handoff only, with no
source-to-object semantic comparison, and does not close whole-file support or
platform gates
([receipt](docs/research/p4-ac1024-mechanical-alt-freecad-readback-2026-11-05.json)).

**AC1024 empty LayerStates root:** `mechanical_example-imperial-alt.dwg` has a
source-bound LAYER_CONTROL extension dictionary with exactly one
`ACAD_LAYERSTATES` entry and an empty child dictionary. The adapter now admits
this exact one-root profile while retaining unique type-42 source-frame,
owner/reactor, unchanged-snapshot, and emitted-body checks. A focused selector
passes ASCII/Binary output, wrong-owner atomic refusal, same-writer correction
retry, and fresh typed readback; strict CLI conversion and FreeCAD's native C++
importer also pass without the prior skip diagnostic. This does not broaden
the separate three-root/XRECORD profile: its source-projected ASCII/Binary
production output and readback selector passes after the test projection was
updated to retain the required LTYPE_CONTROL membership receipt. Strict
whole-file conversion of its aerial witness remains refused by the SUN owner
graph. Keep other
LayerStates profiles and native restoration/evaluation open ([receipt](docs/research/p4-ac1024-empty-layerstates-root-output-2026-11-05.json)).

After actual prerequisites close, run one composed strict CLI checkpoint. A
successful process exit or record count alone is not closure. For AC1018 opt-in
capture, keep default legacy consumers opted out until codec, source, dependency and
output gates are met.

## P5 — rendering and TABLE

DXF SUN parsing retains a separately supplied RGB group regardless of the
order of group 63, so the existing strict output refusal cannot be bypassed by
reordering. SUN RGB output and native lighting remain unqualified
([receipt](docs/research/p5-dxf-sun-color-order-2026-11-04.json)).

LIGHTLIST resolver callbacks now operate on a validated deep snapshot; tested
member and setup mutations, inconsistent resolutions and recursive writes fail
or leave the emitted snapshot stable. Callback-time raw-section and GROUP
transfer detection observes storage signatures only; swap-out/edit/swap-back,
address reuse and in-place mutation are not detected. Broader graph closure
remains open ([receipt](docs/research/p5-lightlist-resolver-snapshot-2026-11-04.json)).

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
A generated VIEW code-334 adapter route now requires one unique retained
SECTIONOBJECT target, preallocates/remaps its entity identity before VIEW output,
and verifies ASCII/Binary group-334-to-group-5 readback. Its generated AC1021+
profile now also closes SECTIONOBJECT group-360 through a retained
SECTIONSETTINGS record, with manager membership, nested source targets,
destination BLOCK_RECORDs, common-owner checks, remapped IDs, nested geometry,
and fresh ASCII/Binary readback. Missing/wrong-kind/duplicate/unclosed graph
cases and pre-AC1021 targets remain atomic refusals. Physical producer/source-
owner evidence and native live-section behavior remain open; see the [LiveSection
receipt](docs/research/p5-view-live-section-graph-closure-2026-11-03.json) and
[SECTION_SETTINGS graph receipt](docs/research/p4-section-settings-adapter-graph-closure-2026-11-03.json).
The concrete adapter retains typed SUNSTUDY callbacks under aggregate
record, byte, and vector bounds ([capture receipt](docs/research/p5-sunstudy-callback-retention-2026-11-03.json)).
The bounded AC1024, AC1027 and AC1032 DXF source-observed owner graphs have
ASCII/Binary output and readback evidence; other graph shapes, native DWG and
rendering remain open
([version receipt](docs/research/p5-sunstudy-dxf-ac1024-ac1032-2026-11-04.json)).
SUNSTUDY/MOTIONPATH/CURVEPATH/POINTPATH/OBJECTPTR physical
producer coverage and native plotting/scene semantics remain open; generated
record-identity and reference round-trips are transport evidence only (see the [SUNSTUDY receipt](docs/research/p5-sunstudy-record-identity-remapping-2026-11-03.json),
[scene-path receipt](docs/research/p5-scene-path-record-identity-remapping-2026-11-03.json),
and [path-carrier receipt](docs/research/p5-pointpath-objectptr-record-identity-remapping-2026-11-03.json)).
The standalone SUNSTUDY DXF codec now follows Autodesk's group roles: group 91
counts dates, group 73 counts hour flags, and date group 90 values convert
between whole seconds in DXF and milliseconds in the DWG model. Output refuses
subsecond times that DXF cannot preserve. Generated ASCII/Binary controls cover
exact counts, typed readback, atomic lossy-value refusal, and corrected
fresh-writer retry ([wire receipt](docs/research/p5-sunstudy-dxf-date-hour-wire-2026-11-03.json)).
Broader source-span/owner binding, producer breadth, and native rendering remain
unqualified. Incomplete declared DXF arrays now refuse typed
output until explicitly repaired; consumers using older inline copy operators
must rebuild to preserve that guard across copies. See the [array guard receipt](docs/research/p5-sunstudy-incomplete-dxf-array-guard-2026-11-03.json).
The adapter refuses SUNSTUDY output outside the bounded source-observed graph,
including unobserved imports and unqualified references.
The CLI exposes the opt-in as `--observe-whole-dxf-source`. Bounded same-revision
repeat conversion verifies the additional empty `ACAD_GROUP` dictionary and
both root edges for AC1024, AC1027 and AC1032. Other dictionary graphs, source
versions and native rendering remain open
([version receipt](docs/research/p5-sunstudy-dxf-ac1024-ac1032-2026-11-04.json)).
The adapter also retains typed LIGHTLIST callbacks within aggregate record,
item, and byte bounds ([capture receipt](docs/research/p5-lightlist-callback-retention-2026-11-03.json));
An explicit consumer opt-in can retain the exact sparse AC1027 default MATERIAL
profile as incomplete typed data plus its raw record. Typed output and adapter
replay remain refused until the ambiguous field and graph are qualified; older
consumers keep the prior input refusal
([sparse MATERIAL receipt](docs/research/p6-ac1027-sparse-default-material-raw-only-2026-11-04.json)).
DXF-origin LIGHT group 421 true color now has bounded ASCII/Binary transport;
DWG CMC mapping and complete LIGHTLIST graph output remain open
([color receipt](docs/research/p6-light-dxf-421-true-color-2026-11-04.json)).
An explicitly authorized low-level LIGHT preview route retains one observed
AC1027 cache with matching revision and codepage; the caller remains responsible
for cache freshness, and automatic adapter replay remains refused
([preview receipt](docs/research/p5-ac1027-light-preview-explicit-2026-11-04.json)).
The DXF LIGHTLIST parser also distinguishes in-subclass LIGHT references from
the record's own group-5 handle. Physical whole-source conversion and graph
output remain open ([reference receipt](docs/research/p5-lightlist-dxf-reference-self-handle-2026-11-04.json)).
the source-aware DXF parser now separates the first ACAD_REACTORS group-330
ACAD_LIGHT dictionary pointer from actual reactors and exposes its typed role
while retaining the original app-data group ([role receipt](docs/research/p5-lightlist-owner-dictionary-role-2026-11-03.json)).
Physical DWG source binding, LIGHT target resolution, emitted graph closure,
and native lighting semantics remain open. The adapter refuses output for a
retained LIGHTLIST until those gates pass, rather than dropping it silently
(see the [callback-loss refusal receipt](docs/research/p8-unqualified-object-callback-loss-refusal-2026-11-03.json)).
An AC1027 source uses an inline role-0 LIGHT reference; the current writer
uses role 5 and the peer reader does not resolve the source reference. Establish
its role and target independently before enabling adapter output ([receipt](docs/research/p5-ac1027-lightlist-physical-inline-role-conflict-2026-11-04.json)).
The low-level AC1021+ DXF writer now emits the documented LIGHTLIST group order
and remaps the mandatory ACAD_LIGHT dictionary pointer, actual reactors, owner,
and named LIGHT references. Generated ASCII/Binary exact-field and typed
readback controls cover mapped output, unresolved-edge destination preservation,
and late-failure rollback with corrected retry; this transport slice does not
qualify physical DWG source binding, complete dictionary/target closure, or
native lighting semantics ([writer receipt](docs/research/p4-lightlist-dxf-writer-transport-2026-11-03.json)).
The adapter also retains bounded GEOMAPIMAGE callback fields
([capture receipt](docs/research/p5-geomapimage-callback-retention-2026-11-03.json));
source asset binding, exact producer fields, output closure, and native map
rendering remain open. A retained record now triggers strict adapter refusal
until those gates pass (see the [callback-loss refusal receipt](docs/research/p8-unqualified-object-callback-loss-refusal-2026-11-03.json)).
The AC1024 sun-and-sky source's NOD/`ACAD_SUN` → DICTIONARY/`360` → SUN graph
is now admitted only for the exact reciprocal soft-owner profile: both
dictionary item handles use code 2 under `hardOwner=0`, and the SUN common
parent resolves to the owning dictionary. A parser vector rejects item-role
mismatch; generated ASCII/Binary output verifies group 350, parent group 330,
handle remapping, rollback, and typed readback. The physical sample still fails
the global strict-read-completeness gate, so no whole-file output is claimed
([receipt](docs/research/p4-ac1024-acad-sun-dictionary-soft-owner-output-2026-11-03.json)).
The AC1024 aerial sample's SUN `0x24C` is owned by XRECORD `0x246`, so it is
not evidence for another valid source-owner route; keep strict output refused
for that sample ([outlier receipt](docs/research/p5-ac1024-sun-xrecord-owner-outlier-2026-11-01.json)).
The retained AC1027 ODA conversion of the AC1024 condominium has three VIEW
group-361 links to SUN objects whose group-330 owners are XRECORDs beneath each
VIEW's extension dictionary. Autodesk documents `AcDbSun` as hard-owned by an
`AcDbViewportTableRecord` or `AcDbViewport`, so this contradictory peer output
does not qualify a new owner route. The local strict conversion reports the
exact first mismatch (`VIEW 0xe19`, `SUN 0xe23`, parent `0xe1d`); keep the
same-version gate and refuse these chains ([receipt](docs/research/p4-ac1024-condo-view-sun-owner-contract-2026-11-05.json)).
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
native override behavior, dictionary key collation, or direct VIEWPORT entity
fields. Autodesk's VIEWPORT-specific table identifies direct entity codes 335,
343 and 344 as soft pointers to VIEWPORT records; retain that record context
where generic group-code ranges assign other pointer classes. The generic table
calls 90–99 int32 while the VIEWPORT-specific table calls 91 a soft pointer.
The Layer-owned XRECORD profile instead uses 335 as the viewport handle, 343 as
an LTYPE handle, and 91 as a lineweight integer inside that XRECORD. Keep these
contexts separate. See the [code-location
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
strict graph closure and native VIEWPORT rendering remain open. Direct
VIEWPORT codes 335/343/344 now have bounded ordered parser/model transport,
unique-target graph validation, forward-handle preallocation, resolver/remap
output and generated ASCII/Binary readback. The target must be another retained
VIEWPORT entity; invalid target roles fail atomically and corrected same-adapter
retry succeeds. Code 91 remains unmodeled and refused because its specified
pointer role conflicts with the generic int32 code range and no binary witness
resolves its encoding. See the [direct override reference receipt](docs/research/p5-viewport-direct-layer-override-reference-transport-2026-11-05.json).

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
The direct writer also maps nonzero VPORT shade-plot group 333 through the
resolver or remap, with atomic refusal for unresolved IDs; the adapter still
refuses unclosed targets. This is serializer transport only; physical target/
owner closure and native rendering remain open. See the [shade-plot receipt](docs/research/p5-vport-shadeplot-reference-transport-2026-11-03.json).

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
route, and resolution of the VIEWPORT group-91 type/value conflict and binary
encoding. Verify coupled dictionary keys, names, body fields, target roles and
consumers before promoting support. The generated 335/343/344 transport is not
physical producer or native semantic evidence; see the [direct override
reference receipt](docs/research/p5-viewport-direct-layer-override-reference-transport-2026-11-05.json).

**VIEWPORT layer-override reference census:** The nine available local AC1024
DWGs were converted and inspected with LibreDWG 0.14. Its DXF outputs contain
16 VIEWPORT records and none of codes 335/343/344/91. Its direct JSON exposes
12 VIEWPORT records across the parseable files, with no corresponding override
members. This is not evidence that the DWGs lack those fields: the peer may
omit them, one JSON output is malformed, and two parseable inputs expose no
VIEWPORT records. This census provides no physical corroboration for the
generated 335/343/344 transport and promotes no support; obtain an eligible
source with the physical references, target roles, coupled layer/dictionary
records and native behavior. See the [local census
receipt](docs/research/p5-viewport-layer-override-local-corpus-census-2026-11-01.json).

**VIEWPORT group-46 field isolation:** Autodesk documents group 72 as the
circle-zoom field and does not list group 46 for VIEWPORT. The parser now leaves
group 46 on the tolerant unknown-code path, so it cannot overwrite `circleZoom`;
a generated ASCII control verifies that group 72 value 725 survives a following
group 46 value 2500. This is field-isolation evidence only, not physical producer
or native-rendering qualification. See the [field-isolation receipt](docs/research/p5-viewport-undocumented-group46-isolation-2026-11-02.json)
and [Autodesk's VIEWPORT DXF table](https://help.autodesk.com/cloudhelp/2025/ENU/AutoCAD-DXF/files/GUID-2602B0FB-02E4-4B9A-B03C-B1D904753D34.htm).

The typed DXF writers now refuse undocumented VIEWPORT/VPORT/VIEW enum values
for render mode (group 281), default lighting type (282), and VIEWPORT
orthographic UCS type (79), using domains 0..6, 0..1, and 0..6. VIEW group
281 now has a DXF-only 0..6 guard; its emitted group-282 default-lighting
property is also limited to Autodesk's two-value `AbstractViewTableRecord`
enum. Autodesk's VIEW DXF table does not list group 282, so this validates the
current field value domain only and does not establish that mapping's
portability. Generated ASCII and Binary controls verify invalid-value refusal
without changing an existing destination and exact readback at each upper
bound.
DWG parsing/encoding keeps its wire-width checks unchanged; native rendering
and physical producer qualification remain open. See the [VIEWPORT
enum-domain guard receipt](docs/research/p5-viewport-dxf-enum-domains-2026-11-03.json),
[VPORT enum-domain guard receipt](docs/research/p5-vport-dxf-enum-domains-2026-11-03.json),
the [VIEW render-mode guard receipt](docs/research/p5-view-dxf-enum-domains-2026-11-03.json),
and [VIEW default-lighting guard receipt](docs/research/p5-view-default-lighting-domain-2026-11-03.json).

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
   The R2007+ unit-12 DWG value string now follows the ODA field order, while
   a conflicting LibreDWG omission and real-producer boundaries remain
   unresolved. Older libdxfrw-generated FIELD records that omitted the string
   may no longer parse as complete; establish physical compatibility without
   guessing between ambiguous layouts. See the [FIELD framing receipt](docs/research/p5-field-unit12-dwg-value-string-framing-2026-11-03.json).
   Forty-two physical AC1021 AcObjProp FIELDs expose in-body UTF-16 property
   values and two soft-pointer roles matching ODA DXF and ACadSharp, but their
   observed framing conflicts with the authoritative detached-TV specification.
   Resolve that conflict before admitting the profile; the [source-span harness
   and receipt](docs/research/p5-ac1021-acobjprop-inline-string-physical-source-spans-2026-11-03.json)
   allow the comparison to be repeated without bundling a drawing.
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
   A focused follow-up localizes nine oversized child-count failures to the
   `AcSm` type-4 profile (handles `0x151D0`, `0x151D5`, `0x151DA`, `0x15BFB`,
   `0x15C00`, `0x15C05`, `0x15C15`, `0x15C1A`, and `0x15C1F`): each has an
   88-byte value-string length, unit values 20/22/28, and the bounded parser
   reads `0x50069006` at the child-count slot. One separate source FIELD has a
   negative child count. LibreDWG 0.14 JSON also omits child values for these
   `AcSm` objects, but does not expose a trustworthy wire count; ODA §20.4.76
   specifies a BL count. Keep the source records raw and the output profile
   closed until an independent physical span or producer witness resolves the
   layout ([focused receipt](docs/research/p5-ac1021-acsm-child-count-trace-2026-11-05.json)).
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
   TABLE rendering nor cell-geometry semantics are qualified. ODA §20.4.103
   defines the final content value as BD; the DWG parser now reads and retains
   the exact double. Because DXF group 95 is integer-valued, output refuses a
   DWG value it cannot represent exactly. A generated 12.5 BD vector proves
   parser and following-handle alignment, and the DXF writer's atomic refusal
   and same-exporter corrected retry are covered. The AC1027 physical sample
   contains zero, so physical nonzero producer spans and target closure remain
   open. The parser validates cell soft-pointer roles and resolves relative
   handles; physical nonzero target closure remains open. See the [cell-reference
   receipt](docs/research/p5-tablegeometry-cell-soft-pointer-dwg-role-2026-11-03.json).
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
7. **AC1027 TABLECONTENT source alignment:** object `0xC94` in the public
   `example_2013.dwg` remains incomplete under the specification's detached-TV
   layout. ODA's audit-disabled DXF contains seven selected string values,
   unit 0 and no attributes, while the documented-path parse misaligns before
   those values. An in-body interpretation matches those selected values but
   conflicts with ODA §20.4.99, and a generated input shows both grammars can
   overlap. Keep the authoritative detached-TV interpretation and seek an ODA
   correction or a nonconflicting source-backed explanation before changing the
   DWG layout; retain incomplete output disposition. See the [comparison receipt](docs/research/p5-ac1027-tablecontent-native-string-dual-parse-2026-11-03.json)
   and [prior span receipt](docs/research/p5-ac1027-tablecontent-attdef-handle-span-mismatch-2026-11-02.json).

**TABLE CadValue flag scope:** ODA §20.4.99 uses one-based bit names: bit 1
(`0x01`) marks `kGeneral` and suppresses the type-specific payload; setting
only bit 2 (`0x02`) does not suppress it. The AC1021 flags=2/type=2 checksum payload
therefore follows the ordinary present-payload rule; its custom key records
source lineage but does not require a parser exception. The AC1021 flags=4/6,
type=4 native-string carrier has a distinct body encoding. Generated vectors
cover flags 0–3 and following-field alignment. The current flag-numbering
recheck is recorded in the
[flag-scope receipt](docs/research/p5-table-cadvalue-oda-flags-and-ac1021-checksum-scope-2026-11-02.json).
The AC1027 source alignment above remains unresolved; native TABLE semantics
and support claims are not qualified.

**Legacy FIELD DXF flag guard:** AC1018 serialization now refuses nonzero root
or child CadValue format flags before emitting bytes. Autodesk's FIELD DXF
group-code table identifies group 93 as the data-set count and lists no
CadValue format-flags carrier. The focused ASCII/Binary control verifies
atomic refusal and same-writer correction retry, while the AC1021 checksum
control still emits flags=2 with its BD value. This generated representability
guard does not qualify physical legacy FIELD prevalence, source-to-output
conversion, or native evaluation; see the [guard receipt](docs/research/p5-field-legacy-dxf-format-flag-guard-2026-11-02.json).

## P6 — residual semantic and version audit

DWG TEXT now refuses conflicting nonzero alignment elevations for active
alignment before writing; a zero alignment Z retains the existing unspecified
convention. Distinct intentional zero and inactive alignment remain ambiguous,
and broader text/native placement qualification remains open
([receipt](docs/research/p6-dwg-text-active-alignment-elevation-2026-11-04.json)).

The dwg2dxf adapter converts AC1024 DWG common DIMENSION rotation/direction
fields to DXF degrees for seven standard subtypes while preserving the public
radian callbacks. Selected graphs pass ASCII/Binary checks; original full-source
export, group-52 peer interpretation and other versions remain unqualified.
Internal adapter dynamic types/layout change; direct binary users must rebuild
([receipt](docs/research/p6-ac1024-dimension-common-angle-adapter-2026-11-04.json)).

DXF ALIGNED DIMENSION now emits its extension-line rotation in the documented
AcDbAlignedDimension subclass; existing LINEAR placement stays unchanged.
ODA retains both selected values, while ezdxf 1.4.5 reads this subclass field
as zero directly. Wider consumer and rendering qualification remain open
([receipt](docs/research/p6-dxf-aligned-dimension-oblique-2026-11-04.json)).

New AC1027 files now use a source-witnessed maintenance release and matching
AuxHeader copies. Nine selected opt-in generated drawings converted through ODA
and passed independent geometry, graph and checksum checks. The complete
header tuple is a hybrid not yet witnessed in a physical source; broader
producer, native application and whole-file qualification remain open
([receipt](docs/research/p6-ac1027-maintenance-header-coherence-2026-11-04.json)).

The dwg2dxf adapter converts per-record AC1024 DWG PDF underlay rotation to
DXF degrees while retaining the public callback's documented radians and
source identity. DGN/DWF and other revisions remain unqualified; original
physical files still encounter unrelated strict HEADER refusal
([receipt](docs/research/p6-ac1024-pdf-underlay-rotation-adapter-2026-11-04.json)).

DWG VIEWPORT entity snap and twist angles now convert radians to the existing
degree-valued DXF API and back for AC1015 and newer. Consumers compensating
for the former raw-radian callback must remove that workaround. Broader source
coverage, native rendering and generated ODA acceptance remain open
([receipt](docs/research/p6-dwg-viewport-angle-units-2026-11-04.json)).

The bounded synthetic AC1027 active VPORT now writes the three observed typed
null handle roles. The original role audit converted only a diagnostic copy;
the later maintenance correction permits the selected opt-in controls above,
while broader header and whole-file qualification remain open
([receipt](docs/research/p6-ac1027-synthetic-vport-null-roles-2026-11-04.json)).

DWG VIEW table twist angles now convert radians to the existing degree-valued
DXF API and back. Consumers compensating for the former raw-radian callback
must remove that workaround. Broader producer coverage and generated ODA
acceptance for named VIEW records remain open
([receipt](docs/research/p6-dwg-view-angle-units-2026-11-04.json)).

DWG VPORT table snap and twist angles now convert radians to the existing
degree-valued DXF API and back. Consumers compensating for the former
raw-radian callback must remove that workaround. Broader producer coverage and
generated ODA acceptance for authored VPORT records remain open
([receipt](docs/research/p6-dwg-vport-angle-units-2026-11-04.json)).

The explicit default-material and plotstyle opt-ins now cover the bounded
AC1027 synthetic layer-0 graph and exact material frames. Source/peer controls
verify selected fields and reference roles. The earlier diagnostic stopped at
VPORT; later selected opt-in controls pass ODA after the maintenance correction,
but the native material payload remains opaque. Other layers, native material
meaning and broader whole-file support remain open
([receipt](docs/research/p6-ac1027-default-material-graph-optin-2026-11-04.json)).
The existing six-version synthetic null-role control now matches the bounded
AC1027 profile; caller and AC1032 expectations remain unchanged.

DWG HATCH patterned-fill and definition-line angles now convert radians to the
existing degree-valued DXF API and back. Consumers compensating for the old
raw-radian callbacks must remove that workaround. MPOLYGON remains unchanged
because its producer observations conflict; broader producer and native render
qualification remain open
([receipt](docs/research/p6-dwg-hatch-pattern-angle-units-2026-11-04.json)).

DXF VIEWPORT ambient color index, RGB and color-book name now survive any
ordering of their independent groups. Producer-derived selected records and
generated ASCII/Binary controls cover the fields; full-source and native
rendering qualification remain open
([receipt](docs/research/p6-dxf-viewport-ambient-color-order-2026-11-04.json)).

DWG 2D POLYLINE vertex tangent directions now convert radians to the existing
DXF/LibreCAD degree-valued API and back. Consumers that compensated for the
previous raw-radian DWG callback must remove that compensation. The separate
pre-R13 path and broader physical producer coverage remain unqualified
([receipt](docs/research/p6-dwg-vertex-tangent-units-2026-11-04.json)).

DXF LAYER color groups now preserve ACI/off state, true color and color name
independently when group 62 follows groups 420 or 430. Reordered selected
producer-derived records and generated ASCII/Binary controls cover the change;
broader producer and native display qualification remain open
([receipt](docs/research/p6-dxf-layer-color-group-order-2026-11-04.json)).

Audit only a current failing source/profile; do not revive historical defects whose
receipt and code path already agree.

For the exact empty AC1027 BLOCK_CONTROL profile, emit the absent-string and
handle boundary at the observed bit position. The earlier ODA diagnostic no
longer reported that record as improperly read, but rejected later records.
Later maintenance and VPORT changes enable the selected opt-in controls above;
investigate remaining profiles without generalizing those results;
AC1032 and other control profiles remain unqualified
([boundary receipt](docs/research/p6-ac1027-empty-block-control-boundary-2026-11-04.json)).
The bounded empty AC1027 VIEW and UCS controls now use their observed bit
boundary too; the earlier ODA diagnostic no longer reported those two records
as improperly read but then rejected LAYER. Transaction controls cover one-shot
allocation failures; persistent allocation failure during rollback remains a
limit ([VIEW/UCS receipt](docs/research/p6-ac1027-empty-view-ucs-controls-2026-11-04.json)).

Investigate the ODA AC1027 HEADER suffix generator across unrelated producers
and versions. The seven-output cross-source sample, including controls authored separately
from Cover, has 739-byte suffixes with a position-wise matching or inverted
lowest-bit pattern; this is not proof of padding or safe omission. A later
post-unlock same-input resave is recorded separately and remains unclaimed. Keep
strict output refused; see the [cross-source receipt](docs/research/p6-ac1027-oda-header-suffix-cross-source-2026-11-04.json)
and [same-version receipt](docs/research/p6-ac1027-cover-oda-same-version-resave-2026-11-03.json).
A suffix-free AC1027 source acquired the same 739-byte form after an ODA
resave, narrowing its origin to that observed save process without explaining
its contents; see the [controlled receipt](docs/research/p6-ac1027-oda-header-suffix-controlled-producer-2026-11-04.json).
After the user unlocked the macOS session, the direct ODA 27.1 CLI retry still
aborted before conversion with PasteBoard/XPC service errors and produced no
output. Do not repeat this invocation until the host service or supported GUI
path changes; this is not evidence about suffix contents. Keep the bytes
unclaimed and strict output refused ([retry receipt](docs/research/p6-ac1027-oda-unlocked-cli-retry-2026-11-05.json)).
An AC1024 maintenance-55 source now reaches its raster and NOD callbacks under
the same diagnostic policy. Its IMAGEDEF and reactor are retained for diagnosis,
but export refuses the IMAGEDEF's unqualified role-4 owner because ODA's common
and record-specific descriptions conflict. Resolve that role with authoritative
evidence and close the image/proxy graph while retaining the separate HEADER
warnings and unclaimed suffix refusal. Audit other revisions' generic Unsupported
paths separately; the reproduced R13/R14 zero-size IMAGEDEF correlation gap is
closed ([receipt](docs/research/p4-ac1014-imagedef-zero-size-correlation-2026-11-04.json)).
See the [HEADER receipt](docs/research/p6-ac1024-header-unclaimed-suffix-diagnostic-2026-11-04.json)
and [IMAGEDEF receipt](docs/research/p4-ac1024-imagedef-owner-role-diagnostic-2026-11-04.json).
An attributed AC1021 source also uses relative soft-pointer IMAGEDEF owners;
its provenance and the absence of a located erratum do not resolve the conflict
([receipt](docs/research/p4-imagedef-owner-role-ac1021-independent-source-2026-11-04.json)).

For GEODATA, qualify whole-source output and native geospatial meaning beyond
the AC1027 selected graph with a nonempty mesh ([receipt](docs/research/p4-ac1027-geodata-mesh-selected-graph-2026-11-04.json)).
The selected graph now transports ASCII definitions containing both literal
caret-J and line feeds, but this does not qualify CRS semantics or whole-source
output ([receipt](docs/research/p4-geodata-ascii-mixed-caret-newline-output-2026-11-04.json)).
Resolve the AC1024 CIVIL-tail version contradiction with authoritative guidance
and an independently produced AutoCAD 2010 witness before changing the parser
or output gate ([receipt](docs/research/p4-ac1024-geodata-v2-civil-tail-conflict-2026-11-04.json)).

For WIPEOUT, qualify automatic adapter preview handling and native rendering
beyond the explicitly authorized AC1024/AC1027/AC1032 resource-free preview
profile; the caller remains responsible for cache freshness ([AC1024/AC1027](docs/research/p5-wipeout-dwg-preview-explicit-replay-2026-11-04.json), [AC1032](docs/research/p5-ac1032-wipeout-dwg-preview-explicit-replay-2026-11-04.json)).

SOLID, TRACE and 3DFACE input now refuse a fourth Z without fourth X/Y
instead of silently replacing it. Valid triangle and default-Z inputs
remain supported; see the [corner receipt](docs/research/p6-dxf-incomplete-fourth-corner-2026-11-04.json).

For ELLIPSE and HATCH/MPOLYGON ellipse edges, continue source and native
qualification beyond the bounded extreme-axis normalization fix; see the
[normalization receipt](docs/research/p6-dxf-ellipse-extreme-axis-normalization-2026-11-04.json).
For standalone R12 derivation, an explicit adapter opt-in interprets canonical
wrapped ELLIPSE parameters as counterclockwise; default signed-sweep behavior,
HATCH edges, derived types and existing zero/full-turn tolerance behavior remain
unchanged. The opt-in requires a known
input convention and does not qualify mixed authored drawings or native CAD
rendering ([CCW wrap receipt](docs/research/p6-r12-ellipse-ccw-wrap-opt-in-2026-11-04.json)).
The shared DWG HATCH/MPOLYGON edge writer now dispatches ellipse edges before
their LINE base class. Producer-backed HATCH output and generated MPOLYGON body
checks cover the selected fields; broader physical and native behavior remains
open ([dispatch receipt](docs/research/p6-dwg-hatch-ellipse-edge-dispatch-2026-11-04.json)).
The DWG writer now refuses unsupported HATCH/MPOLYGON edge type tags such as
RAY and HELIX before writing instead of silently projecting them to LINE or
SPLINE; callers can correct the edge and retry
([refusal receipt](docs/research/p6-dwg-hatch-unsupported-edge-subtypes-2026-11-04.json)).
Qualify CIRCLE/ARC DWG extrusion writeback beyond the explicit AC1024 helper
profile and its selected geometry checks ([receipt](docs/research/p6-ac1024-dwg-legacy-extrusion-opt-in-2026-11-04.json)).
Qualify SOLID/TRACE DWG writeback and broader producer/normal coverage beyond
the explicit DXF inverse-extrusion helper ([receipt](docs/research/p6-ac1024-trace-solid-legacy-extrusion-opt-in-2026-11-04.json)).
The AC1021 SOLID/TRACE absent-string boundary now passes the sampled tilted and
default-normal ODA cases; broader profiles and native rendering remain open
([boundary receipt](docs/research/p6-ac1021-solid-trace-absent-string-2026-11-04.json)).
DWG SOLID/TRACE now reject nonzero corner Z values inconsistent with their
shared elevation. Zero remains the legacy unspecified 2D-corner convention;
derived codecs and the generic ignored-failure policy retain their prior scope
([elevation receipt](docs/research/p6-dwg-solid-trace-elevation-guard-2026-11-04.json)).
Qualify additional text-derived entities and producer/normal profiles beyond
the explicit TEXT DXF inverse-extrusion helper; current DWG TEXT reads do not
apply that transform ([receipt](docs/research/p6-text-legacy-extrusion-opt-in-2026-11-04.json)).
DWG TEXT now resolves a registered style name when no handle is supplied; the
sampled AC1021 output passes ODA only when the caller supplies the witnessed
STYLE common metadata. Qualify default STYLE metadata and other text-derived
entities separately ([receipt](docs/research/p6-dwg-text-style-name-resolution-2026-11-04.json)).
DWG MTEXT now converts rotation-only inputs to a WCS direction while retaining
the existing DXF precedence and caller fields. Broader producers and native
rendering remain open ([orientation receipt](docs/research/p6-dwg-mtext-rotation-direction-2026-11-04.json)).
Qualify attributed INSERT scale semantics beyond the explicit AC1015
positive-finite chain profile, including external near-unit precision and native
rendering ([receipt](docs/research/p6-ac1015-attributed-insert-positive-scale-2026-11-04.json)).
Qualify additional INSERT extrusion normals and coordinate profiles beyond the
explicit AC1015 default, tilted-Y and negative-Z controls ([receipt](docs/research/p6-ac1015-attributed-insert-tilted-2026-11-04.json)).
Qualify attributed INSERT and SEQEND coordinate recovery, broader producers,
normal profiles and native rendering beyond the explicit un-attributed DXF
inverse-extrusion helper; DWG INSERT delivery currently retains native OCS
([receipt](docs/research/p6-insert-legacy-extrusion-opt-in-2026-11-04.json)).
Resolve the observed AC1024 VIEWPORT ambient CMC index discrepancy against
the ODA CMC wording, including safe handling after caller edits; do not infer
a rendering difference from the index alone ([receipt](docs/research/p6-ac1024-viewport-ambient-cmc-index-differential-2026-11-04.json)).
Resolve HATCH/MPOLYGON's ambiguous legacy rational-spline weight provenance so
non-unit legacy-only weights can be preserved and zero distinguished from an
absent DXF weight. Establish the nested polyline elevation, thickness and
extrusion mapping to its parent plane before changing their output disposition;
see the [boundary receipt](docs/research/p6-hatch-boundary-2d-carrier-loss-guard-2026-11-04.json).

The AC1021 LINE writer now uses the eight-byte signed raw-header size and
the producer-backed compressed-header CRC seed. ODA advances to object parsing
and the seven generic table-control handle boundaries now match their source
bit profiles. The original no-material-opt-in probe still fails at a subsequent
LAYER record; qualify that graph and remaining profiles before claiming complete
interoperability
([receipt](docs/research/p6-ac1021-generic-control-boundaries-2026-11-04.json)).
AC1027 and AC1032 generated LINE and POLYLINE outputs are rejected by ODA even
when matching producer controls convert successfully. A fixed-header byte
factorial and a scratch default-material graph did not resolve the failures;
trace their container and object boundaries without assuming either error names
the failing section ([diagnostic receipt](docs/research/p6-ac1027-1032-oda-writer-rejection-2026-11-04.json)).
The AC1027/AC1032 writer now uses the specified opening HEADER defaults seen
in the producer controls; ODA still rejects the sampled outputs, and arbitrary
source values for these unmodeled fields are not retained
([defaults receipt](docs/research/p6-modern-header-opening-defaults-2026-11-04.json)).
It also writes the specified repeated encrypted file-header data at EOF and
validates its address and CRC before publication. The same ODA failures persist
([repeated-header receipt](docs/research/p6-ac1027-1032-repeated-file-header-2026-11-04.json)).
The AC1027/AC1032 HEADER size now excludes the observed high word while the CRC
covers it; the reader retains its existing compatibility warning and the sampled
strict exports still stop at separate opaque-object qualification
([HEADER size receipt](docs/research/p6-ac1027-1032-header-size-crc-2026-11-04.json)).
The AC1021 LAYER string and handle boundary is now emitted without byte padding;
with a diagnostic typed-null material, ODA no longer reports that LAYER as
improperly read, but broader material graphs and full-file acceptance remain
open ([receipt](docs/research/p6-ac1021-layer-bit-boundary-2026-11-04.json)).
An explicit AC1021 default MATERIAL graph binds the producer-witnessed
defaults, and corrected synthetic VPORT null roles allow one sampled LINE
drawing with material and plotstyle opt-ins to convert through ODA without
warnings. Native AC1021 material semantic decoding, other graphs, native rendering
and complete interoperability remain open
([material receipt](docs/research/p6-ac1021-default-material-optin-2026-11-04.json),
[VPORT receipt](docs/research/p6-ac1021-synthetic-vport-null-roles-2026-11-04.json)).
The corrected AC1021 POINT/CIRCLE footer now passes the sampled ODA and
independent geometry checks, alongside the previously passing LINE and ARC
controls. Broader entities, producer versions, graph defaults and native
rendering remain open
([footer receipt](docs/research/p6-ac1021-point-circle-absent-string-footer-2026-11-04.json)).
Additional opt-in AC1021 LINE, ELLIPSE, LWPOLYLINE and TEXT samples, including a
mixed drawing, passed ODA conversion to AC1027 DXF and independent field checks;
other values, graph profiles and native rendering remain open
([matrix receipt](docs/research/p6-ac1021-varied-entity-matrix-2026-11-04.json)).
An AC1021 LWPOLYLINE absence-bit correction also preserves the sampled variable
widths through ODA; qualify additional fields, producers and native rendering
([footer receipt](docs/research/p6-ac1021-lwpolyline-footer-2026-11-04.json)).
AC1018–AC1032 2D/3D POLYLINE child and SEQEND references use the
producer-witnessed hard-owner role. The bounded AC1021 model-space, paper-space
and ordinary named-block membership and 3D footer fixes preserve the sampled vertices
through ODA and independent readback. The same membership fix now preserves
sampled AC1018 and AC1024 2D/3D vertices in model space, paper space and
ordinary named blocks; their unqualified GROUP child references fail closed.
This narrows prior paper/named behavior, which could admit child targets because
they were also indexed as block-owned entities.
Qualify other ownership profiles, mesh/polyface
and other revisions separately. GROUP references to AC1021
model-space vertices have one bounded reciprocal-reactor route; paper-space
and named-block child references are refused until independently qualified.
SEQEND and other GROUP shapes still need independent target closure
([membership receipt](docs/research/p6-ac1021-polyline-model-block-membership-2026-11-04.json),
[paper and named-block receipt](docs/research/p6-ac1021-polyline-paper-named-membership-2026-11-04.json),
[AC1018/AC1024 receipt](docs/research/p6-polyline-ac1018-ac1024-model-membership-2026-11-04.json),
[AC1018/AC1024 paper and named-block receipt](docs/research/p6-polyline-ac1018-ac1024-paper-named-membership-2026-11-04.json),
[footer receipt](docs/research/p6-ac1021-polyline3d-footer-2026-11-04.json),
[GROUP receipt](docs/research/p6-ac1021-polyline-group-vertex-closure-2026-11-04.json)).

Generated LTYPE_CONTROL output has a consumer-visible built-in linetype
ordering risk: six ODA-produced files conflict with the PDF’s stated order.
Keep the PDF authoritative and seek clarification before changing that order.
AC1015 through AC1032 writer-version count encoding now follows the specified
BL, with tightly checked historical BS fallback; AC1021 also uses the declared
handle boundary with a narrowly validated legacy inline fallback. Physical
control-record evidence does not establish whole-file interoperability. See
the [AC1015 count receipt](docs/research/p6-ac1015-ltype-control-bl-count-2026-11-04.json),
[later-version count receipt](docs/research/p6-multiversion-ltype-control-bl-count-2026-11-04.json),
[AC1021 count receipt](docs/research/p6-ac1021-ltype-control-bl-count-2026-11-04.json),
and [order conflict receipt](docs/research/p6-dwg-ltype-control-order-roles-2026-11-04.json).

The AC1018 empty-name STYLE still fails strict output: ODA-produced bit
observations conflict with the ODA v5.4.1 STYLE field order, which remains
authoritative under repository instructions. A spurious FIELDLIST warning
was also removed; see the [factorial receipt](docs/research/p4-ac1018-style-oda-factorial-and-fieldlist-diagnostic-2026-11-04.json).
A diagnostic-only omission exposes a later HATCH with inactive gradient
stops. ODA emits those stops in a profile conflicting with the DXF reference;
keep strict refusal until a lossless qualified route exists. See the
[ET blocker receipt](docs/research/p4-ac1018-et-style-hatch-later-blocker-2026-11-04.json).

Older DXF output now refuses nonzero LWPOLYLINE vertex identifiers rather
than silently dropping them; the earliest valid group-91 revision remains
unqualified. See the [identifier receipt](docs/research/p6-lwpolyline-older-dxf-vertex-id-loss-2026-11-04.json).
Sparse interleaved LWPOLYLINE optional fields now bind to their actual
vertices; ambiguous grouped sparse arrays and records missing required Y
coordinates refuse rather than guess. Explicit zero Y and manual entities
remain supported. See the [sparse-field receipt](docs/research/p6-dxf-lwpolyline-sparse-vertex-fields-2026-11-04.json)
and [missing-Y receipt](docs/research/p6-dxf-lwpolyline-missing-y-2026-11-04.json).

An opt-in helper exposes full 3D WCS vertex positions from untouched native
OCS LWPOLYLINE data without changing LibreCAD's existing projected callback.
Callers must establish the input frame; bulges, widths and curves remain in
OCS. A physical source confirms the selected callback only: its complete read
still fails. Broader producer and native consumer qualification remain open;
see the [WCS vertex receipt](docs/research/p6-lwpolyline-native-ocs-wcs-vertices-2026-11-04.json).

DWG ELLIPSE dispatch now retains the specified WCS center and major axis in
both extrusion modes. Generated six-version controls and an ODA-produced
AC1015 witness pass; other producer profiles and the separate CIRCLE/ARC
applied-extrusion state problem remain open. See the [ELLIPSE receipt](docs/research/p6-dwg-ellipse-wcs-dispatch-2026-11-04.json).

CIRCLE and ARC applied-extrusion callbacks lose coordinate-frame meaning with
the legacy default writer; an explicit opt-in DXF conversion now reverses
the witnessed single legacy transform without changing authored-field or ABI
behavior. ASCII/Binary tests and independent ezdxf readback pass. Consumers
must track the frame themselves; automatic inference and DWG qualification
remain open. See the [opt-in receipt](docs/research/p6-dxf-legacy-extrusion-opt-in-2026-11-04.json)
and [reproducer](docs/research/p6-circle-arc-applied-extrusion-roundtrip-2026-11-04.json).

The generated SURFACE/VIEWPORT round-trip controls now exercise refusal and
corrected retry for the unmapped DWG loft path and invalid lighting type. Native
rendering, source profiles and graph semantics remain open; see the [receipt](docs/research/p6-roundtrip-stale-surface-viewport-controls-2026-11-03.json).

Typed Binary DXF output now refuses group-1070 values whose positive numeric
meaning would change on signed readback; ASCII and explicit raw-bit transport
remain available. Application-level signedness interpretation remains open;
see the [1070 receipt](docs/research/p6-xdata-1070-binary-exact-numeric-2026-11-03.json).

The witnessed AC1018 maintenance-55 HEADER profile now parses its CRC- and
sentinel-validated envelope and diagnoses its unclaimed suffix. The witnessed
CELLSTYLEMAP records now read; strict output remains refused until the suffix,
STYLE bit order and remaining graphs are qualified. See the [receipt](docs/research/p6-ac1018-oda-header-suffix-diagnostic-admission-2026-11-03.json).

The ODA-resaved AC1021 tablet now reads all observed frames with no entity,
object, page-CRC or class-CRC failures while diagnosing its 739-byte HEADER
suffix; strict output remains refused pending suffix and graph qualification.
See the [AC1021 receipt](docs/research/p6-ac1021-oda-header-suffix-diagnostic-admission-2026-11-03.json).
For the AC1027 Cover original and its ODA resave, replacing each complete
731- or 739-byte suffix with either zeros or a deterministic pattern leaves
that source's ODA DXF output identical apart from update timestamps. An AC1018
ET Border resave shows the same result; replacing 723 of the 739 suffix bytes
in an AC1032 Cover resave also leaves that output unchanged. This is one
converter, not a general discard rule; extend independent and native-consumer evidence before changing
strict admission. See the [mutation receipt](docs/research/p6-ac1027-oda-header-suffix-mutation-2026-11-03.json).

The R2007 writer now emits the required file-header pointer, repeated
Reed–Solomon header blocks, terminated section names, section capacities,
AppInfo encoding and physical locator, and a valid trailing-header offset, but ODA still rejects a
freshly generated single-LINE AC1021 file. Identify the remaining cause before
qualifying this writer profile; see the [second-header receipt](docs/research/p6-ac1021-second-file-header-offset-2026-11-03.json),
the [AppInfo locator receipt](docs/research/p6-ac1021-appinfo-physical-metadata-address-2026-11-03.json)
and earlier [framing receipt](docs/research/p6-ac1021-fixed-metadata-header-pointer-2026-11-03.json).
The R2004+ CLASSES writer now emits its specified true header bit; the ODA
rejection and any class-manifest ordinal issues remain open. See the [CLASSES
bit receipt](docs/research/p6-dwg-classes-true-header-bit-2026-11-03.json).
The R2004-family system-page terminator and synthetic root, table-record and
paper-space reference corrections now permit an explicitly opted-in default
plot-style graph. The graph binds HEADER, NOD, DICTIONARYWDFLT, placeholder
and generated layer 0 with collision checks and transactional retry;
caller registration and no-opt-in behavior remain unchanged. Independent
LibreDWG readback verifies six generated versions; ODA converts the tested
AC1018 LINE/CIRCLE/ARC controls, while AC1015 and AC1021–AC1032 still fail
at separate stages. Implement the remaining version-specific fixes and
qualify broader producers, geometry and consumer behavior before support
claims. See the [opt-in receipt](docs/research/p6-default-plotstyle-optin-2026-11-04.json),
[AC1018 geometry receipt](docs/research/p6-oda-optin-ac1018-generated-geometry-2026-11-04.json),
and [plot-style graph research](docs/research/p6-default-plotstyle-graph-research-2026-11-04.json).
For AC1015 the synthetic layer, text style, default linetypes and active
viewport and ACAD APPID now use witnessed typed-null roles. Synthetic
DIMSTYLE also points to the emitted STANDARD text style, and the opt-in
placeholder now emits the witnessed legacy null role. ODA advances to an
object-type error that still needs diagnosis. See the [placeholder receipt](docs/research/p6-ac1015-synthetic-placeholder-null-role-2026-11-04.json)
and [DIMSTYLE receipt](docs/research/p6-ac1015-synthetic-dimstyle-roles-2026-11-04.json)
and [APPID receipt](docs/research/p6-ac1015-synthetic-appid-null-roles-2026-11-04.json)
and [viewport receipt](docs/research/p6-ac1015-synthetic-vport-null-roles-2026-11-04.json)
and [linetype receipt](docs/research/p6-ac1015-synthetic-ltype-null-roles-2026-11-04.json).
Explicit AC1015 opt-ins now emit predeclared model-space LINE/CIRCLE/ARC and
bounded 2D/3D POLYLINE chains with final parent, child and SEQEND links in
frame receipts. Independent ODA and LibreDWG controls pass for tested
two- and three-vertex profiles. A separate bounded opt-in also handles
one named block, one- or two-attribute INSERTs, and a predeclared block
LINE/CIRCLE/ARC chain, with independent normal, rotated and nonmonotonic
readback. Singleton polylines, multiple or nested blocks, broader INSERT
fields, paper space and raw replay remain refused; extend only with
source-backed graph and rollback evidence. See the [block receipt](docs/research/p6-ac1015-predeclared-block-content-chain-2026-11-04.json),
[INSERT receipt](docs/research/p6-ac1015-predeclared-attributed-insert-chain-2026-11-04.json)
and [POLYLINE receipt](docs/research/p6-ac1015-predeclared-polyline-chain-2026-11-04.json).
An explicit AC1024 opt-in now emits the witnessed NOD→ACAD_MATERIAL→
ByBlock/ByLayer/Global graph, HEADER references and generated layer binding,
with source-profile payloads, collision checks and transactional retry.
LibreDWG reads the complete selected graph. Corrected synthetic VPORT null
roles also allow two generated AC1024 drawings to convert through ODA in
audit-on and audit-off modes with no logged warnings; qualify broader
producer, geometry, consumer and version profiles before support claims.
See the [VPORT receipt](docs/research/p6-ac1024-synthetic-vport-null-roles-2026-11-04.json)
and [material graph receipt](docs/research/p6-ac1024-default-material-optin-2026-11-04.json).
One additional generated AC1024 LINE/CIRCLE/ARC control with tilted normal
matches an ODA-produced source and ODA DXF fields and derived WCS geometry;
other normals, producer profiles and native semantics remain open. See the
[tilted-geometry receipt](docs/research/p6-ac1024-material-plotstyle-tilted-geometry-2026-11-04.json).
Modern retained DWG MATERIAL bodies now refuse replay when detached map
strings cannot be proven, including facade calls whose failure is ignored.
Complete replay still needs immutable source strings, selector binding and
independent output proof; see the [refusal receipt](docs/research/p6-dwg-material-detached-string-replay-refusal-2026-11-04.json).
AC1021 BLOCK_CONTROL now stages handles at its declared boundary; the reader
also supports the witnessed historical inline profile under narrow checks.
LibreDWG and an ODA-produced source confirm model/paper references, but ODA
still rejects generated whole files and general R2007 alignment and other
controls remain open. See the [AC1021 receipt](docs/research/p6-ac1021-block-control-handle-boundary-2026-11-04.json)
and [legacy boundary receipt](docs/research/p6-block-control-handle-boundary-2026-11-04.json).
The AC1021 control reader now bounds and retains common reactor references;
the adapter refuses output until that graph is closed. An ODA-produced
reactor witness imports successfully, but full conversion remains gated.
See the [reactor receipt](docs/research/p6-ac1021-block-control-reactors-2026-11-04.json).
R2004-family object-section prefixes now roll back on failed writes and retry
without duplication ([rollback receipt](docs/research/p6-r2004-object-section-prefix-rollback-2026-11-04.json)).
The reader now rejects truncated declared literal runs while retaining
valid bounded and implicit-end streams; audit other malformed token profiles
separately. See the [system-page receipt](docs/research/p6-r2004-system-page-termination-2026-11-03.json)
and [literal-bounds receipt](docs/research/p6-r2004-decompress-literal-bounds-2026-11-03.json).
The generated HEADER now points to its emitted ACAD_GROUP dictionary; independent
LibreDWG readback confirms the relationship in six writer versions. ODA acceptance
and the remaining dictionary recovery failure still require qualification. See the
[GROUP pointer receipt](docs/research/p6-header-group-dictionary-reference-2026-11-04.json).
HEADER table-control and pointer roles now use their specified handle codes,
including typed hard-pointer nulls. The writer binds current layer, text style,
linetype and dimension style to emitted records, and the reader resolves those
source handles before the HEADER callback. Broader producer coverage and
Unicode name collation remain open. See the [reader reference receipt](docs/research/p6-dwg-header-source-current-reference-resolution-2026-11-03.json)
and [writer reference receipt](docs/research/p6-dwg-header-current-table-reference-binding-2026-11-03.json).

The ELLIPSE-to-polyline work bound now covers derived segment counts and R12
refusal/retry; other geometry and caller-state profiles below remain open. See the
[ellipse receipt](docs/research/p6-ellipse-polyline-work-bound-2026-11-03.json).

**Current AC1024 ACSH parser repro (2026-11-03):** the existing local
`visualization_-_condominium_with_skylight.dwg` and
`visualization_-_conference_room.dwg` each reproduce three OBJECTS failures.
The failing frames are two `ACSH_LOFT_CLASS` and one `ACSH_REVOLVE_CLASS` in
the condominium drawing, and three `ACSH_REVOLVE_CLASS` in the conference-room
drawing. Their shared prefixes parse, but the Loft counts and Revolve 2RD/next
BD fields do not produce a qualified body span. ODA v5.4.1 has no class-specific
layout for these proprietary records; the peer schemas agree with the current
assumptions but do not independently resolve the failure. LibreDWG 0.14 runtime
reproduces the suspect Loft counts and Revolve failure even though its whole-file
exit is zero; the peer marks both classes as debugging. See the [runtime
differential](docs/research/p6-ac1024-acsh-libredwg-runtime-differential-2026-11-03.json). Keep these frames
raw-retained but unvalidated and strict output refused until exact fields,
following-handle alignment, and meaningful values have independent evidence.
The files remain local and were not added to the repository. See the
[current ACSH failure receipt](docs/research/p6-ac1024-acsh-current-parser-failures-2026-11-03.json).

| Boundary | Remaining decision or proof |
| --- | --- |
| Version/route parity | Finish the standalone versus pinned LibreCAD/bundled/LibreCAD_3 route differential. Establish authoritative DXF fields for the derived `AcDbAssocValueDependency` body, validate it with a reader that implements that subclass, and establish native acceptance for the associative profile. See [the P6 differential receipt](docs/research/p6-libre-dwg-associative-dxf-import-differential-2026-10-31.json). |
| DIMASSOC DWG-to-DXF transport | AC1018 inline-text and AC1021+ detached-string readers map bounded main/intersection fields into the existing DXF model, with fail-closed handling for unknown classes and cardinalities the model cannot represent. The opt-in physical-source harness verifies resolved source entity references and ASCII/Binary writer output plus typed readback: 23/23 AC1018 `Extruder2.dwg` records and 7/7 AC1021 architectural-sample records passed after the previously refused record's two main-object handles were retained and remapped. A peer scan of 28 unique local DWGs found no multi-handle intersection witness; this is not a format maximum ([cardinality scan](docs/research/p6-dimassoc-corpus-cardinality-2026-11-05.json)). A generated AC1024 vector independently checks owner/reactor/dimension/main/intersection remapping and all modeled fields. LibreDWG's current `dwg2.spec` declares the same ordered sequence; on AC1018, 23 libdxfrw callbacks/33 refs matched LibreDWG 0.14 minJSON for handles, types, subentity/GS values, points, last-point flags and the intersection edge, with one trace-rounded near-parameter difference. These prove selected-record transport only. AC1018 whole-file output remains refused by 474 entity parse failures; AC1021 whole-file output remains refused by 36 MLINE proxy caches. ODA v5.4.1 does not document this body, and native associative behavior remains unqualified. A bounded AC1024 DWG encoder now emits the generated type 1/6/13 profile, including ordered main/intersection handles and both XREF path fields; the focused framed codec control passes atomic unsupported-detail refusal and corrected same-buffer retry, and a full-file generated read delivers all three records and path strings with zero entity/object parse failures. The target handles are absent, so that physical result does not establish graph closure. LibreDWG 0.14 reads the path-free output fields but its `FIELD_VECTOR_T` reads from the scalar cursor rather than the string cursor for path vectors; do not interpret its path failure as writer incompatibility or as interoperability proof ([generated writer receipt](docs/research/p6-dimassoc-dwg-detailed-writer-generated-control-2026-11-05.json)). A post-unlock ODA retry also exited with PasteBoard/XPC errors, so independent reader validation remains unavailable. A strict `dwg2dxf` probe on the generated file refuses at the opaque-frame gate (`qualified=2/3`), confirming fail-closed behavior rather than silent omission. The DWG-to-DXF adapter now retains bounded typed DIMASSOC callbacks and requires a unique typed dimension target, closed emitted entity references, exact source-frame identity, and a single matching remapped OBJECTS record before qualifying each frame. A locally authored missing-target DWG is refused atomically by focused CTest with no published DXF; stale DIMASSOC rows from earlier import generations are ignored and cannot leak into a later DXF export ([adapter closure receipt](docs/research/p6-dimassoc-adapter-closure-2026-11-05.json)). Generated in-memory adapter controls now prove ASCII/Binary closure refusal, destination rollback, same-adapter target correction/retry, emitted-record identity and typed graph readback; a separate control proves stale import generations do not leak into output. These are locally authored adapter mechanics evidence only. LibreDWG 0.14 independently resolves physical dimension, MLEADER and geometry target classes in the AC1018/AC1021 samples, and Autodesk documents dimension-or-leader updating ([target-role receipt](docs/research/p6-dimassoc-independent-target-roles-2026-11-05.json)); whole-file adapter output for those physical sources, native update semantics, and broader producer profiles remain open. The AC1021 selected-record result is 7/7 in ASCII/Binary DXF ([vector receipt](docs/research/p6-dimassoc-main-handle-vector-transport-2026-11-05.json)); see the [earlier DIMASSOC transport receipt](docs/research/p6-dimassoc-dwg-detail-to-dxf-transport-2026-11-02.json) for the AC1018 23/23 result and prior profile evidence. |
| DWG source profiles | **AC1027 Cover:** resolve the 731-byte suffix after the HEADER end sentinel. ODA v5.4.1 defines the CRC/sentinel boundary; LibreDWG 0.14 parses the known handle stream through byte 815, before the envelope trailer at byte 826. Neither source explains bytes 844–1575. Find producer-backed layout evidence or retain the suffix as unclaimed and strict-complete output refused; do not assume it is padding or corruption. Continue independent per-version reader/container/HEADER/CLASSES and producer qualification only for uncovered profiles, including R2007+ carrier differences and class counts. The exact AC1027 app-maintenance-29 HEADER envelope in the public `example_2013.dwg` witness now passes header parsing; do not reopen this envelope without a new failure. After the bounded TABLEGEOMETRY writer was added, a strict AC1024-target replay passed that object and then refused at ModelDocViewStyle serialization because it requires the exact source DWG version. An AC1027-target replay was refused earlier because the strict read disposition remains incomplete (three object parse failures and one DataStorage record not linked to a parsed modeler/surface entity). This does not close AC1027 producer, SPLINE or whole-output qualification; see the [TABLEGEOMETRY transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json). See the [exact HEADER receipt](docs/research/p6-ac1027-exact-header-size-exclusion-2026-11-01.json). The alleged AC1024 CLASSES high-size read failure is not reproduced in the current local corpus: all nine available AC1024 inputs reach `dwgReader24::readDwgClasses END` and report `operationSucceeded=true`, `legacyError=0`, and no CLASSES CRC mismatch. In that 2026-11-03 corpus run only `mechanical_example-imperial.dwg` exited successfully. A 2026-11-05 current-code rerun converts `mechanical_example-imperial-alt.dwg` successfully and fresh typed libdxfrw re-import passes, but its SHA-256 exactly matches `mechanical_example-imperial.dwg`; this is repeat evidence for one unique source, not a second mechanical input. The distinct `civil_example-imperial.dwg` publishes all 335 observed frames with zero entity/object parse failures, then strict output refuses on incomplete read disposition. Independent consumer readback and a second complete source profile remain unqualified; see the [mechanical identity and civil probe receipt](docs/research/p4-ac1024-mechanical-alt-strict-output-2026-11-05.json). Explicit strict callback-finalization failures are present for the architectural and two title-block samples, and the condominium and conference-room samples report three object parse failures each; these later outcomes are not CLASSES failures. The exact 2026-11-03 command, source/tool fingerprints, inputs, trace hashes, and per-file outcomes are recorded in the [current AC1024 corpus rerun receipt](docs/research/p6-ac1024-classes-current-corpus-rerun-2026-11-03.json); the earlier extended-size and synthetic-footer selector remains in the [prior CLASSES recheck](docs/research/p6-ac1024-classes-extended-size-current-corpus-recheck-2026-11-01.json). Do not reopen this parser path without a new failing sample; the local drawings were not added to the repository. See also the [current Cover receipt](docs/research/p6-ac1027-cover-reader-success-output-refusal-2026-11-01.json). The recorded ODA File Converter 27.1.0 GUI run accepts this source and rewrites it as AC1032, but does not explain the 731-byte AC1027 suffix. The latest current-code AC1032 trace reaches the CLASSES end marker and parses all 115 frames with no entity/object parse failures or DataStorage link/orphan failures: the bounded AC1027-origin alternate schema-index table-role profile is retained in the resave. Strict-complete output still refuses before output preparation because HEADER reports a compatibility warning at file offset 16 for the zero high-size word (declared size 802; payload end 826 versus 822 if the word were included) and an unclaimed-data warning at offset 844 (739 suffix bytes through 1583); no DXF was created. Both suffixes remain unexplained, so keep strict-output refusal. See the [AC1032 DataStorage profile recheck receipt](docs/research/p6-ac1027-cover-ac1032-datastorage-role-profile-2026-11-02.json) and the [ODA resave/retry receipt](docs/research/p6-ac1027-cover-oda-resave-recheck-2026-11-02.json). A prior sandboxed shell retry aborted with PasteBoard/XPC errors. On 2026-11-04, strict signature verification passed outside the sandbox and the requested same-revision retry succeeded. The new AC1027 output again has 739 bytes after its validated HEADER trailer at offset 844; its suffix hash differs from the two earlier same-revision outputs. The current reader publishes all 119 frames with no entity/object parse failures; strict output still refuses because source references are not fully qualified. Keep the suffix unclaimed; see the updated [same-version resave receipt](docs/research/p6-ac1027-cover-oda-same-version-resave-2026-11-03.json). |
| TABLEGEOMETRY DWG field encoding | ODA v5.4.1 contradicts itself: §20.4.98 calls the final shared cell-geometry field BL, while §20.4.103 lists the standalone TABLEGEOMETRY group-95 field as BD. LibreDWG's 2020 schema change and the pinned acadrust writer use BL for the shared geometry payload. A wider local corpus scan covered 184 unique AC1021+ DWGs; nine readable files contained 14 TABLEGEOMETRY objects, all with final value zero. A generated AC1021 direct-parser vector now checks nonzero BL `0x12345`, the geometry fields before it, cell framing and exact following content-handle resolution to `0xA720` (`libdxfrw_tablegeometry_nonzero_bl_tail`). This synthetic vector protects the bounded-BL parser but cannot adjudicate the standalone source encoding. Keep the parser unchanged and retain the standalone source-span gate until a traceable independent producer yields a nonzero value. Do not conflate this DWG-stream question with DXF group-95 int32 output mapping or native TABLE semantics; see the [group-95 discrepancy/census receipt](docs/research/p6-tablegeometry-group95-profile-census-2026-11-02.json), [AC1027 transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json), and [ACadSharp profile receipt](docs/research/p5-acadsharp-tablegeometry-flag-profile-2026-11-01.json). |
| HATCH/MPOLYGON/MESH | Do not claim AC1018 HATCH `0x173` or ET Border HATCH `0x2BA` inactive-gradient stops as supported: both retained-stop profiles remain refused. The physical ET Border profile has a bounded DICTIONARYVAR-only selector that verifies atomic HATCH refusal, explicit local stop removal, and corrected object-graph output/readback; it does not qualify the stops. See [the selector recheck receipt](docs/research/p4-ac1018-dictionaryvar-hatch-resolution-recheck-2026-11-02.json). After explicit stop removal, generated HATCH/MPOLYGON controls and the selected physical HATCH record have bounded ASCII/Binary output/readback coverage for representable companion metadata and boundary fields. This does not establish the stops' native meaning or whole-file output; see the [loss-guard and selected-source transport receipt](docs/research/p6-ac1018-hatch-inactive-gradient-stop-dxf-loss-guard-2026-11-02.json). `DRW_Hatch::boundaryPointToWcs` and `boundaryVectorToWcs` expose the Autodesk OCS/elevation transform for HATCH/MPOLYGON boundaries without changing the stored 2D model or callback behavior; this is coordinate mapping evidence only, not native geometry qualification. See the [HATCH OCS mapping receipt](docs/research/p6-hatch-ocs-wcs-coordinate-mapping-2026-11-02.json). Generated ASCII/Binary HATCH controls enforce base-HATCH group 70/71 as 0/1, group 75/76 as 0..2, and group 77 as 0/1 in the parser; typed writer admission also bounds groups 75/76. MPOLYGON retains its distinct group-70 version and group-71 fill meanings. See the [Boolean-domain receipt](docs/research/p6-hatch-boolean-dxf-domain-2026-11-02.json) and [pattern-domain receipt](docs/research/p6-hatch-pattern-dxf-domains-2026-11-02.json). MPOLYGON group 99 now requires a nonnegative count within the existing 10,000-item parser budget at both reader and typed-writer admission; invalid values preserve the model/destination and corrected same-writer retry plus typed readback pass. Autodesk defines the field as a count of ignored degenerate borders but gives no relation to group 91, so no such relation is inferred and this scalar guard does not qualify native geometry semantics; see the [group-99 domain receipt](docs/research/p6-mpolygon-degenerate-count-domain-2026-11-02.json). MPOLYGON group 63 is now bounded to Autodesk's 0..256 CMC/ACI index domain on parse, typed validation and writer admission, with ASCII/Binary atomic refusal, same-writer retry and typed readback; this scalar guard does not qualify fill rendering (see the [ACI domain receipt](docs/research/p6-mpolygon-aci-dxf-domain-2026-11-02.json)). Generated HATCH gradient parsing now rejects explicit RGB outside 0..0xFFFFFF and ACI outside 0..256 before mutating the current stop; callback validation applies the same scalar bounds, and generated fast controls cover endpoints, preserved state and corrected same-group retry. This validates scalar transport only, not gradient rendering (see the [gradient stop color-domain receipt](docs/research/p6-hatch-gradient-stop-color-domain-2026-11-02.json)). MESH crease values are restricted to finite -1 or nonnegative values; generated parser/writer controls accept a shorter edge-order crease vector. Autodesk identifies group 95 as the level-0 edge-crease count, and ACadSharp's independent DWG reader applies each value to the same-index edge. The count is bounded by level-0 edge count at DXF/DWG read and write admission to prevent nonexistent edge references. A focused generated AC1024 DWG vector now exercises valid one-edge/one-crease parsing, malformed one-edge/two-crease refusal, state clearing, and corrected same-object retry; see the [reader-guard receipt](docs/research/p6-mesh-dwg-crease-count-reader-guard-2026-11-02.json). This establishes bounded positional transport only; physical producer breadth, native crease effect, subdivision, geometry and annotation semantics remain open. See the [MESH crease and edge-count receipt](docs/research/p6-mesh-crease-value-domain-2026-11-02.json). Other witnessed zero-fit/producer layout disagreements, extended CMC meaning and unrepresented overrides remain; qualify boundary geometry and annotation behavior beyond framing. |
| Text/annotation/coordinates | Qualify whole-document conversion and native dimension/text display; generated ASCII/Binary DXF reader controls now cover `applyExtrusion` conversion of TEXT and INSERT-owned ATTRIB insertion/alignment points, INSERT insertion points, and the OCS text points of all seven documented standard DIMENSION subtypes (0–6), including selected clone/arc points for linear and angular records, while preserving their WCS definition points. These controls do not establish native orientation/display or physical-producer breadth. See the [TEXT/ATTRIB receipt](docs/research/p6-dxf-text-attrib-ocs-extrusion-2026-11-02.json), [INSERT OCS receipt](docs/research/p6-dxf-insert-ocs-extrusion-2026-11-02.json), the original [linear/angular DIMENSION receipt](docs/research/p6-dxf-dimension-ocs-extrusion-2026-11-02.json), and the [standard-subtype coverage receipt](docs/research/p6-dxf-dimension-standard-subtypes-ocs-2026-11-02.json). Generated AC1027 ASCII/Binary MLEADER text-content and block-content profiles use the existing adapter serializer after strict owner/style/reference closure. Typed readback verifies selected context/root/line fields, remapped style/text-style/LTYPE/BLOCK_RECORD roles, the block transform, and the referenced content BLOCK_RECORD body. Unrepresentable CMC data remains an atomic refusal with same-adapter correction/retry. This is generated transport evidence only; physical producer profiles and native annotation semantics remain open. See the [text-content adapter receipt](docs/research/p6-mleader-selected-adapter-dxf-transport-2026-11-02.json) and [block-content adapter receipt](docs/research/p6-mleader-block-content-adapter-dxf-transport-2026-11-02.json). A non-mutating typed view now exposes the pre-R2018 ACAD_MTEXT_COLUMN_INFO, ACAD_MTEXT_COLUMNS and ACAD_MTEXT_DEFINED_HEIGHT XDATA brackets while retaining raw XDATA, separate linked MTEXT callbacks and the distinct metadata, height-list, declared-link and actual-handle counts; generated ASCII/Binary round trips also verify remapping. This does not qualify source-producer breadth, linked-column aggregation, or native layout/rendering; see the [typed-view receipt](docs/research/p6-legacy-mtext-column-xdata-typed-view-2026-11-02.json). Continue qualifying those consumer and graph semantics outside the bounded R2018 embedded ATTRIB/ATTDEF context; other ATTRIB/ATTDEF variants; native dimension display, nonstandard DIMARC/LARGE_RADIAL placement, and DSTYLE/picture graphs; SHAPE glyph/font identity; other LEADER/MLEADER profiles and native annotation semantics; and derived-R12 appearance. Do not reopen the selected large-radial picture-block or R2018 embedded-MTEXT parser profiles without a new regression; see the [picture-block graph receipt](docs/research/p6-ac1032-large-radial-picture-block-resolution-2026-11-02.json) and [R2018 embedded-MTEXT receipt](docs/research/p6-ac1032-r2018-embedded-mtext-context-2026-11-02.json). |
| Geometry and clipping | R2013+ DWG fit SPLINEs with non-default square-root-chord or uniform knot parameterization now fail closed on DXF write because DXF has no selector and the choice changes curve shape; the documented chord default remains writable. A separate hand-encoded S6.2 AC1027/AC1024 parser control checks the versioned BL fields and following-sentinel alignment; the read-only local reference scan found AC1021 scenario-1/2 and AC1024 scenario-2 parser-smoke traces, but producer provenance is unverified and strict CLI export published no DXFs. This scan found no additional AC1027/AC1032 SPLINE record; the separate public AC1027 `example_2013.dwg` field differential remains bounded to its recorded fields and does not qualify whole-file support. Keep authentic per-version qualification open (see the [local corpus receipt](docs/research/p6-spline-local-reference-corpus-scan-2026-11-02.json), [AC1027 differential receipt](docs/research/p6-ac1027-fit-spline-field-differential-2026-11-01.json), and [boundary-test receipt](docs/research/p6-spline-r2013-bitlong-boundary-fast-test-2026-11-02.json)). ASCII/Binary refusal and corrected retry are covered. A generated ASCII/Binary reader control now covers classic 2D POLYLINE OCS conversion using the owning elevation while preserving 3D POLYLINE WCS vertices under `applyExtrusion`; this is not physical-producer or native-renderer qualification. See the [classic POLYLINE OCS receipt](docs/research/p6-dxf-classic-polyline-ocs-extrusion-2026-11-02.json). Generated ASCII/Binary controls also verify opt-in conversion of PDF/DGN/DWF UNDERLAY insertion positions from OCS/ECS to WCS while retaining local rotation and 2D clipping coordinates; no physical producer or native consumer claim follows. See the [UNDERLAY OCS receipt](docs/research/p6-dxf-underlay-ocs-extrusion-2026-11-02.json). Autodesk's SubDMesh API crease domain is now enforced at DXF/DWG read and write admission: -1, 0, or any positive finite value; generated controls cover negative-value refusal and the all-version DWG writer boundary. This closes only the scalar domain; crease-to-edge association and subdivision behavior remain unqualified. See the [MESH crease-domain receipt](docs/research/p6-mesh-crease-value-domain-2026-11-02.json). Curve equivalence/native interpretation, other SPLINE/NURBS/ELLIPSE/OCS/polyface/mesh semantics, remaining completed-pool/domain validation, physical SPATIAL_FILTER producer profiles, actual clipping application and native consumer semantics remain open. Generated local DXF writer admission now rejects self-intersecting SPATIAL_FILTER polygons per Autodesk API requirements; this is field-admission evidence only. See the [polygon guard receipt](docs/research/p6-spatial-filter-simple-polygon-dxf-guard-2026-11-02.json). See the [knot-parameterization guard receipt](docs/research/p6-ac1027-spline-fit-knot-parameterization-dxf-guard-2026-11-02.json). |
| Units and inheritance | Generated ASCII/Binary LTYPE controls enforce Autodesk's fixed-A group-72 alignment, signed-16-bit group-75 shape-number domain, and group-70 standard flags (16/32/64, with 32 requiring 16) on typed write admission; parsing preserves unsupported values. See the [group-72 receipt](docs/research/p6-ltype-alignment-dxf-domain-2026-11-02.json), [group-75 receipt](docs/research/p6-ltype-shape-number-dxf-width-2026-11-02.json), [group-70 receipt](docs/research/p6-ltype-standard-flags-dxf-domain-2026-11-02.json), and [group-74 read-tolerance receipt](docs/research/p6-ltype-element-flag-read-tolerance-2026-11-02.json). The group-74 parser retains nonnegative signed-16-bit values outside its modeled writer range; no interpretation or support claim follows. LTYPE description and complex-dash strings already share the retained-byte lease, with replacement, reset, copy/move, over-budget refusal, and corrected retry covered by the [retained-text receipt](docs/research/ltype-retained-text-owner-fast-slice-2026-10-17.json); do not reopen that implementation. Typed LTYPE output now refuses group-9 complex text above 255 encoded bytes after DXF version/code-page conversion, with ASCII/Binary boundaries, Unicode conversion, readback, whole-record rollback and corrected writer retry covered by the [group-9 guard receipt](docs/research/p6-ltype-group9-encoded-length-guard-2026-11-02.json). Autodesk's 2027 ObjectARX table gives group 9 a 255-character maximum (less for Unicode), while its 2024 group-value-type table gives codes 0-9 a 2049-single-byte-character allowance from AutoCAD 2000 onward; the direct-field/group-range references conflict. The 255-byte writer bound is a conservative interim admission rule, not proof of the maximum valid producer value; independent producer/native acceptance and the larger-string profile remain unqualified. DXF HEADER output now refuses explicit zero or non-finite `$LTSCALE` while retaining tolerant input and the absent-value default of 1.0; ASCII/Binary refusal, read retention and same-writer correction retry are covered by the [LTSCALE receipt](docs/research/p6-ltscale-header-zero-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls now also reject malformed explicit `$CELTSCALE` group/type, conflicting aliases, and non-finite values instead of silently replacing a wrong-typed value with the absent default 1.0; finite positive, zero and negative values remain writable because Autodesk documents no sign/zero domain. Tolerant input, ASCII/Binary rollback and corrected retry are covered by the [CELTSCALE receipt](docs/research/p6-celtscale-header-field-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls also enforce the documented 0/1 domains for `$PSLTSCALE` and `$MSLTSCALE`, retain unsupported values on input, keep the absent PSLTSCALE default of 1, leave absent MSLTSCALE omitted, and refuse MSLTSCALE for pre-AC1021 output. Autodesk dates MSLTSCALE to AutoCAD 2008, but AC1021 spans AutoCAD 2007–2009, so the current target enum cannot distinguish that product-version boundary. See the [linetype display-scale receipt](docs/research/p6-linetype-display-scale-dxf-domain-2026-11-02.json). These are field-admission controls only; native linetype appearance remains unqualified. Generated ASCII/Binary HEADER controls now require explicit `$MEASUREMENT` to be the documented integer 0 (imperial) or 1 (metric), retain unsupported values on tolerant input, and preserve absence independently of `$INSUNITS`; the existing AC1015/AC1032 presence and unchanged-geometry selector also passes. See the [MEASUREMENT domain receipt](docs/research/p6-measurement-header-dxf-domain-2026-11-02.json). This establishes scalar admission only; hatch/linetype resource selection remains unqualified. Generated SCALE controls require positive finite paper/drawing units on DXF read/write; group 70 remains preserved on input but nonzero values are refused on typed output because ODA v5.4.1 identifies it as unknown and writes zero. These are field-admission guards only, not native annotation-scale qualification. See the [SCALE units receipt](docs/research/p6-scale-units-dxf-domain-2026-11-02.json) and [SCALE flag receipt](docs/research/p6-scale-unknown-flag-dxf-output-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls now enforce the documented group-2 string representation for `$UCSBASE` and `$PUCSBASE`, retain malformed values tolerantly, and refuse wrong types/groups, aliases, and case-folded keys with atomic rollback and corrected retry; this does not resolve a named UCS table target or qualify orthographic restoration. See the [UCS base field receipt](docs/research/p6-ucs-base-header-fields-guard-2026-11-02.json). Strict output now resolves nonempty ASCII UCSBASE/PUCSBASE values to unique output UCS records, admits blank and built-in WORLD values, and keeps differing non-ASCII case variants refused; see the [UCS base target receipt](docs/research/p6-ucs-base-name-target-closure-2026-11-02.json). Remaining work: native linetype resource selection/rendering across `$MEASUREMENT`, LTSCALE/CELTSCALE, `$PSLTSCALE` and `$MSLTSCALE`, including exact behavior for AutoCAD 2007 versus 2008–2009 inside the shared AC1021 target; native orthographic restoration and non-ASCII UCS case collation; transparency/ByLayer/ByBlock meaning; remaining SCALE version/API boundaries; and Template description/survey policies. ODA §22 byte-count framing remains supported across all six reader versions. A well-bounded non-empty byte-count description is decoded with the recognized source codepage and delivered to tolerant DWG consumers through an optional trailing `DRW_Interface` callback; the following MEASUREMENT is also published. A warning still makes strict-complete DXF output refuse the description because DXF has no matching HEADER field. Modern TU16-shaped descriptions remain readable only with a warning and no guessed MEASUREMENT: LibreDWG 0.13.3 uses TU16 in its R2007+ T16 path, which conflicts with ODA §22. Its local writer/reader round trip preserves a non-empty description, but libdxfrw rejects that generated DWG earlier at the file-header stage, so it is not parser validation. Generated controls cover maximum signed length, exact logical-section bounds, empty descriptions, legacy modern UTF-16-NUL compatibility, codepage decoding/callback delivery, unknown-codepage fail-closed behavior, byte-count measurement publication, TU16-shaped warning-only behavior and malformed framing. Missing mandatory AcDb:Template sections remain tolerant on read but now refuse strict-complete output for AC1018+; absence remains optional for AC1012-AC1015. See the [missing-section strict-output receipt](docs/research/p6-template-missing-section-strict-output-guard-2026-11-02.json). Description text still has no DXF representation; strict DXF preservation, producer breadth, native semantics and broader survey policies remain open. See the [Template frame receipt](docs/research/p6-template-description-frame-2026-11-02.json) and [description strict-output guard receipt](docs/research/p6-template-description-strict-output-guard-2026-11-02.json). |
| INSUNITS scalar domain | Autodesk documents integer group 70 values 0–24. The public `DRW_Header::Units` enum now includes codes 21–24, and the measurement helper classifies all four as English units; DXF output validates the canonical scalar and refuses it for pre-AC1015 targets, while DWG HEADER encoding rejects malformed or out-of-domain values before writing bits. Generated ASCII/Binary controls round-trip 0–24, preserve invalid input tolerantly, and verify failure/retry; direct DWG HEADER controls cover all six writer revisions. This is scalar representation only. Autodesk notes code 21 has release-specific support and codes 22–24 are AutoCAD for Mac only, so insertion scaling and native platform behavior remain unqualified. See the [INSUNITS receipt](docs/research/p6-insunits-domain-guard-2026-11-02.json). |
| Lineweight and display defaults | DXF HEADER output now validates `$CELWEIGHT` against the enumerated values (including special -3/-2/-1 values), `$ENDCAPS`/`$JOINSTYLE` as 0–3, and `$LWDISPLAY`/`$XEDIT` as 0/1. Generated ASCII/Binary tests cover every admitted value, malformed types/groups/aliases, atomic refusal and same-writer retry; tolerant input retains unsupported integer values. This is scalar admission only; rendering, plotting, and native behavior remain unqualified. See the [lineweight HEADER receipt](docs/research/p6-lineweight-header-domain-guard-2026-11-02.json). |
| Orientation and attribute visibility | DXF HEADER output now validates `$ANGDIR` as 0/1 and `$ATTMODE` as 0/1/2. Generated ASCII/Binary controls cover valid readback, malformed type/group/aliases, atomic refusal, corrected retry, and tolerant input retention. Legacy `$ATTDIA` and `$ATTREQ` now enforce the Autodesk R12 DXF group-70 domain 0/1 only below AC1012, matching the existing writer emission branch; AC1012, AC1014, and AC1027 controls verify these omitted values do not block output. This verifies legacy field-domain transport only and does not qualify native INSERT or prompt behavior; see the [legacy attribute-prompt receipt](docs/research/p6-attdia-attreq-legacy-header-domain-guard-2026-11-02.json). Autodesk's system-variable reference enumerates `$MIRRTEXT` as integer 0/1, while the DXF table's “if nonzero” wording describes the Boolean effect; the statements are compatible. HEADER output now enforces the 0/1 domain with ASCII/Binary readback, refusal, retry, and tolerant-input controls; see the [MIRRTEXT resolution receipt](docs/research/p6-mirrtext-header-domain-resolution-2026-11-02.json). These field-domain checks do not qualify native mirroring or attribute display behavior. See the [orientation/attribute HEADER receipt](docs/research/p6-angdir-attmode-header-domain-guard-2026-11-02.json). |
| Unknown/proxy data | A bounded same-revision AC1018+ `ACAD_PROXY_ENTITY` serializer now transports the modeled class ordinal and entity-data payload for generated no-reference profiles with ASCII/Binary readback. Its low-level writer also round-trips resolver-backed object-ID groups in generated controls; the dwg2dxf adapter now binds unique top-level typed entity targets and final emission for pointer handle codes 4/5 and offset soft-pointer codes 6/8/10/12, mapped to groups 330/340. Owner-role/unknown codes, nested/raw/OBJECTS targets, recursive dependencies, physical source profiles, and native semantics remain fail-closed or unqualified. Unknown-data slots, EED/common reference closure, mixed ACDS graphs, and native semantics remain open; unsupported profiles refuse atomically. Additional proxy primitives need independent semantics, not cache-byte agreement. Associative output and replay belong to P4. See the [bounded proxy transport receipt](docs/research/p4-dwg-proxy-entity-bounded-dxf-transport-2026-11-02.json) and [object-ID transport receipt](docs/research/p4-dwg-proxy-entity-object-id-dxf-transport-2026-11-02.json). |
| Safety and caller state | OPEN. Continue auditing remaining setup, assembly and work bounds, especially other `writeObjects` maps/vectors; inject allocation failures across independent builders; broaden rollback/failure injection to other staged families; current controls cover the GROUP handle planner plus core IMAGEDEF and rooted named-dictionary OBJECTS all-allocation-offset sweeps, plus IMAGEDEF callback validation/bad_alloc/retry. The GROUP planner sweeps every measured allocation in ASCII/Binary configurations, confirms no partial plan is retained, and retries the same codec successfully ([GROUP planner sweep](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json)). A reservation allocation failure inside an admitted write now fails that transaction without poisoning its same-codec retry; pre-write reservation failures retain their one-attempt generation guard. The ASCII/Binary IMAGEDEF sweep now checks each calibrated allocation before the consumer OBJECTS callback for atomic rollback, destination and identity preservation, same-writer retry, and typed readback ([IMAGEDEF allocation sweep](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json)). The rooted named-dictionary sweep does the same for every measured allocation, including the handle-reservation failure that previously caused a stale retry rejection; it checks typed DICTIONARY readback and the pre-write reservation guard ([named-dictionary allocation sweep](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json)). The generated NOD/GROUP OBJECTS path now sweeps every calibrated allocation in ASCII and Binary, checks destination and derived-identity rollback before the consumer callback, retries the same writer, and reads back the NOD-to-GROUP-dictionary-to-GROUP handle graph ([generated GROUP OBJECTS allocation sweep](docs/research/p6-dxf-generated-group-dictionary-objects-allocation-retry-2026-11-03.json)). The parallel generated NOD/IMAGE path sweeps every calibrated allocation with a planned IMAGEDEF and source IMAGE dictionary, then checks same-writer retry and typed NOD-to-image-dictionary-to-IMAGEDEF plus IMAGE-reference readback ([generated IMAGE dictionary OBJECTS allocation sweep](docs/research/p6-dxf-generated-image-dictionary-objects-allocation-retry-2026-11-03.json)). Both are local transaction-safety evidence and make no producer or semantic support claim. Generated NOD/GROUP/IMAGE dictionary-source admission also sweeps allocation failures through root-only and rooted child-dictionary profiles, preserving prior owned state and retrying the same codec after each injected failure ([source admission sweep](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json)). The SECTIONMANAGER/SECTIONSETTINGS `writeObjects` path now sweeps every measured allocation offset for generated ASCII/Binary graphs; each failure preserves the destination and the same adapter retries with fresh graph readback in normal and ASan builds ([SECTION OBJECTS allocation receipt](docs/research/p6-dxf-section-objects-allocation-failure-retry-2026-11-03.json)). The generated POINTCLOUDDEFINITIONEX/REACTOREX OBJECTS callback now sweeps every calibrated allocation offset in ASCII and Binary: each failure preserves the destination sentinel, and same-adapter retry plus typed readback verifies NOD → point-cloud dictionary → definition/reactor → entity references ([receipt](docs/research/p6-pointcloud-objects-allocation-failure-retry-2026-11-03.json)). This is local graph/transaction evidence only. The consumer `writeObjects()` callback also has an ASCII/Binary thrown-`std::bad_alloc` control after one XRECORD is staged: the write fails atomically, preserves the prior destination and source-object bindings, skips finalization, then succeeds on same-codec retry with typed readback ([receipt](docs/research/p6-dxf-objects-callback-bad-alloc-rollback-retry-2026-11-03.json)). A generated direct NOD-to-XRECORD OBJECTS profile now sweeps six ASCII and five Binary allocation offsets; each injected failure preserves the destination and caller graph, and same-adapter retry verifies typed owner/payload readback ([receipt](docs/research/p6-dxf-xrecord-objects-allocation-failure-retry-2026-11-03.json)). This closes only that emitter path. A generated AC1027 DXF-source nested dictionary with two DICTIONARYVAR records now sweeps 62 ASCII and 60 Binary OBJECTS-callback allocation offsets; each failure preserves the destination and caller graph, and same-adapter retry verifies both values and their nested owner ([receipt](docs/research/p6-dxf-dictionaryvar-objects-allocation-failure-retry-2026-11-03.json)). This closes only that emitter path. The generated PDF/DGN/DWF underlay-definition OBJECTS graph now sweeps all 60 ASCII and 53 Binary callback allocation offsets, preserving the destination sentinel and succeeding on same-adapter retry with typed definition/entity graph readback ([receipt](docs/research/p6-dxf-underlay-objects-allocation-failure-retry-2026-11-05.json)); this closes only that local transaction path. The generated AC1027 SUNSTUDY OBJECTS callback now sweeps 20 ASCII and 18 Binary allocation offsets with destination rollback, same-adapter retry and typed graph readback in normal and ASan builds ([receipt](docs/research/p6-dxf-sunstudy-objects-allocation-failure-retry-2026-11-05.json)); its source DXF is authored locally for the test and removed after each run. The generated AC1027 direct-NOD RASTERVARIABLES OBJECTS callback now sweeps 11 ASCII and 10 Binary allocation offsets, with destination and caller-record preservation, same-adapter retry and typed readback in normal and ASan builds ([receipt](docs/research/p6-dxf-rastervariables-objects-allocation-failure-retry-2026-11-05.json)); its source DWG is authored locally and removed after each run. The generated AC1027 direct-NOD WIPEOUTVARIABLES OBJECTS callback now sweeps 11 ASCII and 10 Binary allocation offsets, with destination and caller-record preservation, same-adapter retry and typed readback in normal and ASan builds ([receipt](docs/research/p6-dxf-wipeoutvariables-objects-allocation-failure-retry-2026-11-05.json)); its source DWG is authored locally and removed after each run. These controls close only their bounded callback exception paths; allocations inside other OBJECTS builders and broader whole-drawing failure states remain open. A focused four-worker ASCII/Binary codec test now writes and imports separate files concurrently in normal and ASan builds ([scoped codec concurrency receipt](docs/research/p6-scoped-dxf-codec-concurrency-2026-11-03.json)); this qualifies only isolated dxfRW instances and distinct files, not shared-instance concurrency or general thread safety. Inspector callback closures and borrowed-view lifetime are now explicit: a focused normal/ASan control proves the codec owns value captures through invocation and releases them when cleared; reference captures remain caller-owned. Other source-lifetime boundaries and platform numeric/string policies remain open ([receipt](docs/research/p6-dxf-inspector-closure-lifetime-2026-11-03.json)). XDATA APPID preflight now catches allocation failures during group validation and stages additions transactionally, preserving the prior required-APPID set if any allocation fails. A focused ASCII preflight sweep reaches nine injected failures, confirms no partial set survives each, and verifies fresh-codec admission retry; this closes only the APPID set mutation path, not broader OBJECTS builders or shared-instance concurrency ([receipt](docs/research/p6-dxf-xdata-appid-admission-allocation-rollback-2026-11-05.json)). Completed bounded staging families: GROUP, IMAGEDEF plan, named/root dictionaries, CLASS, raw sections, resolved references, TABLESTYLE/BG/SUN registries, source BLOCK_RECORD and canonical LTYPE. The controls use internal logical resource estimates and do not create format maxima or support claims. Evidence: [GROUP count](docs/research/p6-dxf-group-vector-setter-count-guard-2026-11-03.json), [GROUP nested bounds](docs/research/p6-dxf-group-nested-setter-bounds-2026-11-03.json), [GROUP plan allocation retry](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json), [generated dictionary source allocation](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json), [IMAGEDEF count](docs/research/p6-dxf-image-definition-count-and-staging-guard-2026-11-03.json), [IMAGEDEF plan bounds](docs/research/p6-dxf-image-definition-plan-nested-setter-bounds-2026-11-03.json), [IMAGEDEF OBJECTS rollback](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json), [core named-dictionary OBJECTS rollback](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json), [generated GROUP OBJECTS rollback and readback](docs/research/p6-dxf-generated-group-dictionary-objects-allocation-retry-2026-11-03.json), [generated IMAGE dictionary OBJECTS rollback and readback](docs/research/p6-dxf-generated-image-dictionary-objects-allocation-retry-2026-11-03.json), [named-dictionary count](docs/research/p6-dxf-named-dictionary-object-setter-count-guard-2026-11-03.json), [named-dictionary nested bounds](docs/research/p6-dxf-named-dictionary-object-nested-setter-bounds-2026-11-03.json), [root dictionary count](docs/research/p6-dxf-root-dictionary-setter-count-guard-2026-11-03.json), [root dictionary byte bounds](docs/research/p6-dxf-root-dictionary-byte-bound-2026-11-03.json), [CLASS count](docs/research/p6-dxf-class-setter-count-guard-2026-11-03.json), [CLASS byte bounds](docs/research/p6-dxf-class-metadata-setter-byte-bound-2026-11-03.json), [raw sections](docs/research/p6-dxf-raw-section-setter-bounds-2026-11-03.json), [resolved references](docs/research/p6-dxf-resolved-write-reference-setter-bounds-2026-11-03.json), [handle remap](docs/research/p6-dxf-handle-remap-setter-bound-2026-11-03.json), [source table metadata](docs/research/p6-dxf-source-table-metadata-setter-bounds-2026-11-03.json), [scoped DXF codec concurrency](docs/research/p6-scoped-dxf-codec-concurrency-2026-11-03.json). | Keep validation focused; do not reopen a passed guard without a new failure. |
| CLI and oracle contracts | Single-file output/input filesystem-alias refusal is implemented for positional and FreeCAD `-o` routes; the fast CLI selector covers same-path, symlink and hard-link aliases and verifies source preservation (see the [alias guard receipt](docs/research/p6-cli-single-file-input-output-alias-guard-2026-11-02.json)). Generated source-DXF FreeCAD `-o` controls now bind default target selection for all nine supported revision headers in ASCII and Binary, including AC1012→AC1014. Locally authored DWGs now cover every DWG-writer version (AC1015, AC1018, AC1021, AC1024, AC1027 and AC1032). The generated sources succeed at their default targets in ASCII/Binary, with binary readback at the same revision. The bounded AC1018 minimal root/empty-group source also succeeds at explicit AC1024 ([receipt](docs/research/p4-ac1018-minimal-root-group-output-2026-11-04.json)); broader AC1018 graphs remain open. A debug-only preflight projection now exposes source format/revision, selected target revision, implicit-versus-explicit selection, encoding and input-appropriate read-completeness mode before writer setup. Generated AC1012 DXF input verifies its AC1014 default; AC1006 input verifies a fail-closed unavailable default; generated AC1021 and AC1018 default/explicit successes verify the projection before output preflight. An external AC1014 sample also selects AC1014 but is refused because two source objects fail parsing and the frame disposition is incomplete; this is diagnostic evidence only, and the sample is not committed. See the [default target-version receipt](docs/research/p6-cli-freecad-default-target-version-matrix-2026-11-02.json). Remaining contracts: eligible AC1012/AC1014 DWG-source output/readback; other partial-read failure profiles (the six-writer-version generated control covers ASCII and Binary strict refusal, opt-in output and readback; the original ASCII control is recorded in the [all-version receipt](docs/research/p6-cli-partial-dwg-read-all-writer-versions-2026-11-02.json)); and broader CLI projections: broader HEADER structure/repairability profiles beyond generated duplicate `$ACADVER`/`$LTSCALE` variables and repeated group 10 in `$INSBASE`/`$EXTMIN`; the API repair workflow for duplicate variables/repeated group codes is covered by the ASCII/Binary same-adapter replacement/retry control in [the HEADER receipt](docs/research/par27-header-repeated-group-refusal-fast-slice-2026-10-22.json); broader malformed structures remain open; `$REQUIREDVERSIONS` malformed/repeated diagnostic controls now cover duplicate variable, repeated group 160, and wrong group code with AC1032 atomic refusal; feature semantics/native acceptance beyond one generated bit preserved through AC1032 typed DXF output/readback plus AC1024 refusal remain open; decoder issue-mask projection now exercises all 15 defined `DxfTextIssue` flags across 14 locally authored profiles with exact-mask and atomic-refusal checks; see the [CLI issue-mask matrix receipt](docs/research/p6-cli-text-decoder-issue-mask-matrix-2026-11-02.json); and skipped-record projections cover one and three same-class instances plus three distinct AC1027 custom class names, with exact source-name and aggregate counts, strict refusal, and destination preservation; other skip/recovery categories and revisions remain open. See the [multi-class receipt](docs/research/p6-cli-skipped-record-multiple-class-aggregation-2026-11-02.json). The CLI applies no repair; these projections diagnose only. See the [source-disposition projection receipt](docs/research/p6-cli-source-disposition-projection-2026-11-02.json).

**ODA ARM64 package integrity recheck:** Native verification outside the sandbox
passes for the installed 27.1 app and the downloaded 27.9 package and extracted
app; earlier sandboxed signature failures were unreliable. The verified 27.1 CLI completes audit-disabled AC1027-to-AC1027 Cover resaves.
The validated trailer ends at byte 844; the original has 731 unclaimed HEADER
bytes and same-revision ODA outputs have 739. LibreDWG 0.14 independently
decompressed earlier original/resave sections. A further post-unlock retry
produced no bytewise suffix matches against five previously compared same-source
outputs; pairwise low-bit patterns remain structured but unexplained. The new
output reads 119/119 frames with no entity/object parse failures, then strict
DXF preparation refuses on unqualified source references. Keep the suffix
unclaimed and strict output refused. See the [integrity correction](docs/research/p6-ac1027-cover-oda-arm64-package-integrity-2026-11-02.json)
and updated [same-version resave receipt](docs/research/p6-ac1027-cover-oda-same-version-resave-2026-11-03.json).

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
readback. A focused scan of 170 available local DWGs found only one
LOFTEDSURFACE record, in LibreDWG's tracked AC1018 `Surface.dwg`; its decoded
cross-section/guide counts are both zero. The paired DXF therefore provides no
nonzero reference witness. Autodesk's general group-code table defines 310 as
binary data and 320–349 as handle groups, while LibreDWG's local schema maps
the vectors to 310 and prints hexadecimal handle text. Keep that mapping
unqualified until a nonzero producer witness and native consumer readback
resolve the encoding. See the [local corpus and contract scan
receipt](docs/research/p6-lofted-surface-nonzero-reference-witness-scan-2026-11-03.json).
These guards do not qualify the surface's source graph, ACIS shape, or native
semantics.

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
The AC1027 version-3 adapter now closes the witnessed model-space extension
dictionary, GEODATA owner/reactor and separate host pointer, with staged
field/reference checks and independent ASCII/Binary selected-graph readback.
The original whole file remains refused at the VISUALSTYLE gate, and other
producer profiles and native coordinate semantics remain open. See the
[selected-graph receipt](docs/research/p4-geodata-v3-block-extension-graph-2026-11-04.json)
and [physical source receipt](docs/research/p4-geodata-v3-oda-source-selected-output-2026-11-04.json).
A follow-up binds source version, host and common links privately to prevent
a mutated v3 graph entering the older v2 route; modern DXF also refuses
north-direction Z that its wire format cannot represent. One nondefault
ODA AC1027 frame shows two raw north doubles and matching following fields,
while ODA §20.4.78 labels the field 3RD. Preserve the current parser for
that witness and qualify other producers and versions before generalizing.
See the [binding and span audit](docs/research/p4-geodata-graph-binding-audit-2026-11-04.json).
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
  Raw modeler replay now checks the entity/record identity before publication
  and at finalization; physical source breadth and general output association
  remain open ([receipt](docs/research/p7-dwg-raw-modeler-acds-identity-binding-2026-11-03.json)).
  Modeler replay now also checks the source-backed sequential AcDs search
  profile and rejects incomplete or mismatched schema/index/handle joins;
  other producer permutations and whole-DWG/native qualification remain open
  ([receipt](docs/research/p7-acds-search-index-modeler-output-2026-11-03.json)).
  Raw modeler replay also requires the ODA-specified SAB prefix at payload
  offset zero; full SAB framing and semantics remain open ([receipt](docs/research/p7-acds-modeler-sab-prefix-2026-11-03.json)).
  DWG-associated replay additionally requires the four fixed 32-bit header
  words to be present (31 bytes total); their values and the remaining body
  are not validated by this guard. See the [minimum-header receipt](docs/research/p7-dwg-modeler-sab-fixed-header-minimum-2026-11-04.json).
  A physical AC1027 source now independently matches three modeler-to-AcDs
  associations and complete SAB payloads against ODA DXF; model-space
  ownership is established by the source entmode and unique block record.
  Other source/producer profiles, complete output and semantics remain open;
  see the [physical join receipt](docs/research/p7-ac1027-physical-modeler-acds-sab-oda-byte-join-2026-11-03.json).
  DataStorage finalization now distinguishes a dangling record handle from an
  unlinked record whose handle exists in the validated source HANDLE map. The
  latter receives a separate informational diagnostic and per-section count;
  it still has no typed modeler/surface association or qualified output route.
  Generated controls cover both cases and reject a stale source-frame index.
  The strict ACDSDATA thumbnail projection now reconciles both unlinked
  accounting classes against the exact recognized thumbnail count; its
  generated multi-modeler ASCII/Binary control exercises a mixed orphan /
  source-frame-present split and retains atomic refusal/retry/readback coverage.
  This closes only the counter-consumer consistency gap, not the unlinked
  record's association or output route.
  ODA v5.4.1 §24.2.2.3 identifies each stored record's handle as the object
  handle; this accounting change does not establish payload semantics or
  preserve the record. See the [source-frame accounting receipt](docs/research/p7-acds-unlinked-record-source-frame-accounting-2026-11-04.json).
  A separate cq-acis/OCCT attempt measured one solid but left history
  records undecoded and could not convert the two REGIONs. Independent
  placement, measurements and a different kernel remain open; see the
  [advisory observation](docs/research/p7-ac1027-source-sab-cqacis-observation-2026-11-04.json).
  Independent planar arithmetic using the shared parser now agrees on the
  solid and two REGION measurements, subject to strict geometry assumptions;
  history, placement and independent decoding remain open. See the [analytic
  receipt](docs/research/p7-ac1027-source-planar-independent-arithmetic-2026-11-04.json).
  Map the DWG modeler flag and revision UID to the DXF shell before automatic
  replay. A source-backed REGION experiment preserved SAB bytes but produced
  different shell values after ODA reimport; DWG-associated SAB output now
  requires explicitly supplied shell fields. The physical eight-name schema
  remains gated. Matching UID suffixes have multiple plausible field boundaries
  in several physical frames. A forward trace from the current common-entity
  boundary also overlaps the peer-bound revision field in four ODA files;
  resolve the common/body boundary and source profile before decoding them.
  See the [shell-loss receipt](docs/research/p7-dwg-modeler-shell-loss-guard-2026-11-03.json)
  and [UID span receipt](docs/research/p7-ac1027-modeler-revision-uid-physical-spans-2026-11-03.json).
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

The user-selected official CAD Assistant ARM64 build visibly imports all
three source-bound SABs. Its solid topology, area, volume and centroid agree
with cq-acis/OCCT to displayed precision; the two REGION measurements lack
a second oracle. Both routes share OCCT, validation is not reported and
DWG placement/history remain unqualified. Obtain independent-kernel and
source-native evidence before support claims; see the [CAD Assistant
observation](docs/research/p7-ac1027-source-sab-cad-assistant-observation-2026-11-04.json).

A BRL-CAD STEPcode/openNURBS/librt probe independently checks selected
topology and eight rays on the source-associated 2E1 solid. Its STEP bridge
shares the existing SAB parser and OCCT, and source placement and volume remain
unqualified; it cannot promote support claims
([receipt](docs/research/p7-ac1027-sab-brlcad-ray-crosscheck-2026-11-04.json)).

## P8 — consumer, platform, packaging and release gates

The plot-style provider passes the LibreCAD_3 facade compile guard and the
FreeCAD 1.1.4 converter subprocess contract in build-tree and installed CLI
tests. Native application execution, binary ABI and Linux/Windows remain
open; see the [consumer receipt](docs/research/p6-default-plotstyle-consumer-compatibility-2026-11-04.json).

The SAB minimum-header helper is included in the canonical CMake and generated
source-release inventories; the inventory check and refusal self-tests pass.
Native source-release builds and platform qualification remain open.

- Preserve LibreCAD/LibreCAD_3 callback/default behavior; new callbacks remain
  non-pure. The compile-only LibreCAD_3 facade guard and generated ASCII/Binary
  3D-consumer probe pass; see the [local API and 3D probe receipt](docs/research/p8-local-compat-and-generated-3d-probe-2026-11-02.json).
  These checks cover API shape and generated record delivery only, not native
  application behavior. Qualify intended 3D consumers and remaining FreeCAD
  `dwg2dxf` profiles without dropping required geometry.
  A dirty local LibreCAD_3 snapshot's native persistence target passes three
  existing DXF export tests and a planar LINE/CIRCLE open/save/reopen probe
  against a pinned libdxfrw build. Its custom CMake finder needs a downstream
  C++17 propagation fix, its full test target has an unrelated Kaguya C++17
  failure, and a nonplanar LINE triggers its 2D geometry exception. Qualify a
  clean consumer revision and full application separately
  ([receipt](docs/research/p8-librecad3-macos-native-dxf-2026-11-04.json)).
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
  A follow-up requires the exact built-in runtime class as well as the matching
  public tag for MESH and HELIX, so user-derived runtime objects cannot be
  accepted while their additional state is dropped. Generated ASCII/Binary
  controls verify atomic refusal, destination preservation, exact-object retry,
  triangle topology readback and selected HELIX field readback; see the
  [MESH/HELIX subclass-dispatch receipt](docs/research/p8-mesh-helix-runtime-subclass-dispatch-2026-11-03.json).
  The public low-level MESH and HELIX writers now enforce the same exact
  runtime-type/public-tag contract. Direct ASCII/Binary calls also refuse
  derived classes atomically and support same-writer exact-object retry with
  typed topology/payload readback; see the
  [low-level MESH/HELIX receipt](docs/research/p8-mesh-helix-low-level-runtime-dispatch-2026-11-03.json).
  These guards close only subclass-truncation paths; physical producer profiles
  and native semantics remain unqualified.
  The matching capture boundary now refuses derived MESH/SPLINE/HELIX,
  surface-subtype and modeler-body callbacks before copying them into narrower
  built-in records; same-adapter exact callback retry remains available. The
  adapter dispatch and direct low-level SURFACE/modeler writers also require
  exact built-in runtime types. Generated ASCII/Binary controls cover callback
  refusal/retry and direct destination-preserving refusal, exact retry and
  typed readback for plane-surface and modeler records; see the
  [3D subclass-admission receipt](docs/research/p8-3d-runtime-subclass-admission-2026-11-03.json).
  This only prevents runtime-object slicing; it does not qualify payload
  syntax, physical source breadth, topology/kernel meaning or native consumers.
  A follow-up direct-writer matrix now executes all six exact surface subtypes
  in ASCII and Binary DXF. Matching-tag derived subclasses are refused with
  destination preservation, and exact built-in retries write the expected
  record and subtype marker ([receipt](docs/research/p8-direct-surface-subtype-admission-2026-11-03.json)).
  This closes direct guard execution coverage only; payload semantics, producer
  breadth, topology and native consumers remain unqualified.
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
  The adapter also retains POINTCLOUDDEFINITION/EX, their reactor variants,
  NAVISWORKSMODELDEF and POINTCLOUDCOLORMAP callbacks under shared record,
  item and byte budgets. Generated ASCII/Binary controls verify bounded typed
  retention, malformed color-map refusal, and atomic output refusal for an
  unrooted mixed definition/NAVISWORKS/color-map graph
  ([callback-retention receipt](docs/research/p8-pointcloud-object-callback-retention-2026-11-03.json)).
  A bounded authored-only AC1027+ adapter route now closes the exact
  NOD `ACAD_POINTCLOUD_EX_DICT` → DICTIONARY → DefinitionEx path and the
  reciprocal DefinitionEx/ReactorEx/POINTCLOUDEX owner/reference cycle. It
  reserves and remaps object/entity identities, rejects an invalid reactor
  owner without changing the destination, and permits same-adapter correction
  and ASCII/Binary typed readback; see the [authored graph receipt](docs/research/p8-pointcloud-authored-definition-reactor-graph-2026-11-03.json).
  A separate authored-only AC1027+ classic POINTCLOUD graph now preserves the
  supplied NOD root key, closes the dictionary/definition/reactor/entity
  identities and reciprocal owner/reference roles, emits exact classic CLASS
  counts, and supports typed ASCII/Binary readback. The generated root key is
  deliberately local and does not assert a canonical producer key. Wrong
  reactor ownership preserves the destination, and correction succeeds on the
  same adapter; see the [classic authored graph receipt](docs/research/p8-pointcloud-classic-authored-graph-transport-2026-11-03.json).
  Imported DXF point-cloud Definition/DefinitionEx/Reactor/ReactorEx callbacks
  now have a bounded raw-carrier/physical-span binding: the optional source
  facet joins the typed callback, complete raw group sequence, version/encoding
  and physical OBJECTS spans, then keeps private bounded snapshots for later
  unchanged-state checks. Generated ASCII/Binary controls cover nested
  ACAD_REACTORS fields and exact group/span order; see the [DXF definition
  source-span receipt](docs/research/p8-pointcloud-dxf-definition-source-span-binding-2026-11-03.json).
  Imported DXF POINTCLOUDEX entities now also bind ENTITIES and named-BLOCK
  callbacks to their exact physical dispatch spans, retained source models and
  current emitted models, with bounded BLOCK snapshots. Generated ASCII/Binary
  import controls verify one-to-one source/output inventories and keep imported
  output atomically refused at that stage; see the [DXF entity source-binding receipt](docs/research/p8-pointcloud-dxf-entity-source-binding-2026-11-03.json).
  A bounded AC1027+ physical field-profile gate now checks the exact
  `AcDbEntity`/`AcDbPointCloud` subclass sequence, accepted group-code/type and
  cardinality profile, modeled common values, and exact entity/owner/definition/
  reactor handle fields. It refuses app-data, XDATA, proxy payload and unknown
  groups. This caught and fixed a duplicate `AcDbEntity` subclass marker in
  both POINTCLOUD writers. Generated ASCII/Binary controls reject a mutated
  definition reference without touching the destination, then bind the corrected
  source profile on retry; graph admission additionally requires the complete
  DefinitionEx/ReactorEx checks below. See the
  [field-profile receipt](docs/research/p8-pointcloud-dxf-field-profile-2026-11-03.json).
  A follow-on compares every admitted POINTCLOUDEX body group value directly
  against its associated typed model, including crop framing, direction vectors
  and vertices; generated controls also refuse a changed class-version value
  atomically before the corrected source profile is rebound. See the
  [body-value projection receipt](docs/research/p8-pointcloud-dxf-body-value-projection-2026-11-03.json).
  This closes the bounded AC1027+ field-to-model comparison only; it is not
  independent producer/spec/native semantic evidence. Imported DXF output now
  admits the bounded same-revision AC1027+ DefinitionEx/ReactorEx graph after
  source-span/callback binding, definition identity binding, reciprocal owner/
  reactor validation, collision checks, staged serialization and fresh typed
  readback. Generated AC1027 and AC1032 ASCII/Binary controls reject mutated
  entity references and body fields atomically, then re-export through the same
  adapter after correction. The AC1032 control also proves wrong-target
  destination preservation followed by same-revision retry; fresh readback
  verifies the NOD/dictionary/object/entity graph.
  The parser now normalizes DXF ACAD_REACTORS into both the typed link vector
  and its count, and defers physical/source revision equality to completed-read
  preflight because dxfRW has not finalized sourceVersion during OBJECTS
  callbacks. See the [imported graph output receipt](docs/research/p8-pointcloud-imported-graph-output-2026-11-03.json).
  A focused version-boundary receipt records the AC1032 wrong-target refusal
  and same-revision ASCII/Binary graph readback ([receipt](docs/research/p8-pointcloud-imported-graph-ac1032-version-coverage-2026-11-03.json)).
  This is generated transport evidence, not independent producer or native
  semantic evidence; DWG still needs source CLASS/frame/output binding. P4
  source-profile closure, physical producer breadth, asset loading and native
  consumer semantics remain open.
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
  Adapter output now admits bounded AC1021 and AC1027 direct NOD →
  `ACAD_IMAGE_VARS` → `RASTERVARIABLES` owner routes, alongside the existing
  nested dictionary route. Physical `children-room-decoration.dwg` and
  `Cover.dwg` witnesses confirm class version 0, their revision-specific
  image-frame fields, image quality 1, units 1, and one soft-pointer reactor
  back to owner NOD `0x0C`; both also carry the XDictionary-missing flag with no
  XDictionary handle. Autodesk specifies the direct owner entry and class
  version, and ODA §20.1/§20.4.91 defines the common reactor pointer role.
  Generated AC1021/AC1027 ASCII/Binary controls verify exact root/object/reactor
  readback, wrong-reactor, nonzero-class-version and out-of-range-unit
  refusals, destination preservation, same-adapter retry, and source-frame
  binding. See the
  [direct-owner receipt](docs/research/p4-rastervariables-ac1021-ac1027-direct-nod-owner-2026-11-03.json).
  The physical RASTERVARIABLES selectors verify import only. Whole-source output remains
  blocked by the samples' unrelated read/layout-integrity gaps, and native
  image semantics and broader producer/version coverage remain open.
  The hash-bound AC1027 WIPEOUTVARIABLES witness now seeds a local wrapper DWG;
  selected ASCII/Binary output verifies the remapped NOD owner/reactor graph,
  atomic refusal, retry, and typed readback ([physical writer bridge receipt](docs/research/p4-ac1027-wipeoutvariables-physical-writer-bridge-2026-11-05.json)).
  This does not qualify physical whole-source conversion: its two HEADER
  integrity observations and global AC1027 strict-source gate remain unresolved.
  Native wipeout display/plot behavior, preview preservation, and broader
  producer/version/mode coverage remain open.
  A separate AC1024 source-frame control supports the bounded 0/1/3 profile.
  The exact hash-bound physical source now passes selected-object ASCII/Binary
  output and fresh typed readback: its source NOD `0x0A` owner/reactor map to
  the output NOD, and source mutation refusal plus corrected same-adapter retry
  pass. This uses debug/legacy partial-output settings because the source has
  two HEADER integrity observations; strict whole-drawing output remains
  refused. See the [original source-frame receipt](docs/research/p4-ac1024-rastervariables-direct-nod-2026-11-04.json)
  and [selected-output receipt](docs/research/p4-ac1024-rastervariables-physical-selected-output-2026-11-05.json).
  The low-level WIPEOUT writer now translates both IMAGEDEF and IMAGEDEF_REACTOR
  references through the configured resolver or remap fallback; see the
  [WIPEOUT reference receipt](docs/research/p8-wipeout-dxf-reference-remapping-2026-11-03.json).
  The low-level MLINE writer now gives its group-340 MLINESTYLE pointer the
  same resolver precedence and remap fallback; the [MLINE reference receipt](docs/research/p4-mline-dxf-reference-remapping-2026-11-03.json)
  records the focused ASCII/Binary control, including unresolved-target refusal
  and corrected retry. This does not prove target
  existence or close the MLINESTYLE dependency graph.
  The concrete adapter admits authored classic Definition/Reactor and authored
  and imported same-revision AC1027+ DefinitionEx/ReactorEx DXF graphs within
  their bounded profiles above. Imported classic POINTCLOUD graph output now
  admits one AC1027–AC1032 modelspace entity with one classic definition and
  reactor, same-generation physical entity/object spans, origin-bound NOD and
  child DICTIONARY records, unchanged source-projected fields, and reciprocal
  owner/reference closure. It preserves the source-observed NOD key and requires
  exact source-revision output. Generated ASCII/Binary AC1027 controls cover
  changed definition/reactor references and dictionary keys, wrong-revision
  and malformed-graph destination preservation, and same-adapter retry; the
  AC1027 and AC1032 profiles both have fresh typed graph readback. See the
  [classic imported graph receipt](docs/research/p8-pointcloud-classic-imported-graph-transport-2026-11-03.json).
  This is generated transport evidence only. Named-BLOCK and paper-space
  records, DWG source graphs, physical producer breadth, canonical classic-root
  qualification, external asset loading, native 3D semantics, and broader
  BLOCK callback qualification remain open.
  Imported DXF DefinitionEx/ReactorEx output now also requires the ordered raw
  OBJECTS values and exact field cardinalities to project into their source
  typed models, including identity, owner, reactor roles, class version and
  DefinitionEx payload. A locally authored duplicate class-version field is
  refused atomically; the generated imported graph still retries and reads
  back in ASCII and Binary. This is generated source-field transport only;
  physical producer breadth, DWG source binding, assets and native semantics
  remain open. See the [definition field-binding
  receipt](docs/research/p8-pointcloud-definition-field-binding-2026-11-03.json).
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
  record. Generated ASCII/Binary controls
  verify atomic refusal, destination preservation, corrected same-adapter retry
  and typed readback. The focused test passes; no fixture was added. The same
  receipt records this direct-writer boundary.
  This establishes generated transport only, not physical producer breadth or
  native lighting/rendering semantics; DWG CMC-to-DXF mapping and photometric
  fields remain refused pending qualification.
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
- Finish MotionPath/CurvePath/PointPath/ObjectPtr OBJECTS qualification. The
  concrete adapter now retains their callbacks under aggregate record/byte
  limits ([capture receipt](docs/research/p8-action-path-object-callback-retention-2026-11-03.json));
  field/source-span matching, owner and handle-role closure, output binding,
  strict ASCII/Binary readback, and native consumer semantics remain open.
  Strict adapter preflight now refuses any nonempty retained action-path list
  with a specific diagnostic until a source- and graph-qualified output route
  exists; this prevents successful conversion from silently dropping these
  records. The focused capture-limit selector now checks that refusal and
  destination preservation. LIGHTLIST and GEOMAPIMAGE retention uses the same
  fail-closed rule; SUNSTUDY remains refused outside its bounded observed DXF
  graph ([callback-loss refusal receipt](docs/research/p8-unqualified-object-callback-loss-refusal-2026-11-03.json)).
  This guard is not output support and does not reduce the remaining
  qualification work.
  Keep unsupported DWG profiles refused until those gates pass.
- The optional desktop and native Linux/Windows FreeCAD CMake gates now bind to
  the existing tracked `ordinary_enc_AC1027.dwg`; a registration-only configure
  lists the desktop, open, and insert controls. Native execution is still open;
  see the [tracked-sample binding receipt](docs/research/p8-freecad-tracked-sample-binding-2026-11-02.json).
- Complete native Linux/Windows FreeCAD qualification and bind the actual loaded
  library/converter child. Verify Windows UTF-8/Qt paths, filesystem collisions,
  MSVC/DLL/shared/static/relocated consumers and package/toolchain contracts.
- Qualify the Linux x86-64 source-release CI job at integration. The GCC 13
  source-release `distcheck` now passes as an unprivileged user in a Linux
  ARM64 container; this does not qualify native desktop applications or other
  architectures ([receipt](docs/research/p8-gcc13-source-release-overload-warning-policy-2026-11-04.json)).
  Complete ABI/API and native application checks, controlled RSS/performance/
  work-cap measurements and required older-CMake/Conan/PIC/export gates.
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
| J260 | The [shared replay](docs/research/j260-j293-shared-stream-successor-80fd4a96-2026-11-04.json) validates 29 aliases and executes 24 distinct DWG streams once. Resolve each exact refusal or failure and obtain eligible semantics, target and platform evidence; preserve lineage. |
| J268 | The [original 28 pathname/status rows](docs/research/j268-original-execution-transcript-recovery-2026-11-04.json) and six matching pre/post hash observations are recovered; at-sweep bytes and set digest remain unproven. The [three-fixture successor](docs/research/j268-three-admitted-fixture-successor-08b21445-v2-2026-11-04.json) remains advisory and cannot replace that historical set. |
| J284 | Resolve the 11 claims' target and integrity blockers. The [eight-field MPOLYGON peer probe](docs/research/j284-mpolygon-eight-field-peer-probe-6034e221-2026-11-04.json) agrees on one fixture but retains HEADER warnings and does not qualify target equivalence. Keep ARCALIGNEDTEXT, LARGE_RADIAL payload and MPOLYGON-color exclusions. |
| J293 | Share J260's unique-stream work, retain separate claim bindings, and obtain independent semantic/platform evidence rather than duplicate corpus runs |
| J295 | Freeze a current-source successor and resolve peer diagnostic admissibility; the native check now covers all four RTEXT position/rotation fields, but current-digest Linux/Windows/macOS receipts and independent semantic qualification remain open. The [v3 diagnostic](docs/research/p9-j295-rtext-successor-v3-2026-11-04.json) and [reader triage](docs/research/p9-j295-independent-reader-triage-2026-11-04.json) do not promote claims. |

A [selected external LINE peer probe](docs/research/j256-j260-j293-external-line-peer-6034e221-2026-11-04.json)
agrees on one J256/J260/J293 source. The publisher now serves identical
bytes, but the attached file’s license scope and target, preservation and
platform gates remain unresolved; see the [publisher receipt](docs/research/j256-j260-j293-et-drawing-publisher-provenance-2026-11-04.json).

An external process completing, an unavailable test being skipped, or advisory
agreement never changes a support claim automatically.

Share P7/P8/P9 executions only when exact input bytes, route/operation,
executable/modules, configuration and platform match. Store one execution receipt
with separate claim/lineage bindings; an ordinary-read run cannot stand in for an
open/insert, native-platform or independent-kernel run.

## Fast implementation and validation rules

The OBJECTS physical-source test target now uses the policy already inherited
through `dx_iface`, eliminating duplicate-base `-Werror` failures while
retaining its policy overrides. Use its focused selector for graph work.

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
