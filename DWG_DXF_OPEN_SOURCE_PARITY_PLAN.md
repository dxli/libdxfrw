# DWG/DXF parity — remaining implementation plan

**P5 AC1018 TABLE direct-encoder transaction recheck (2026-11-10):** the
focused generated allocation control now checks exact body/handle bytes,
non-byte-aligned cursors, handle-occurrence ledgers, and common-write flags.
It injects 17 allocation failures, verifies rollback, then retries the same
TABLE into the same seeded buffers and compares with a clean encoding; the
terminal success offset also matches. `libdxfrw_dwg_table_cell_overrides`
passes 1/1. This completes direct-codec rollback proof for that generated
profile. It does not test allocation failure during whole-file graph assembly
or qualify physical TABLE cells, native rendering, or broad TABLE support. See
the [updated TABLE readiness receipt](docs/research/p5-dwg-table-encoder-readiness-2026-11-08.json).

**P5 FIELD evidence refresh (2026-11-10, AC1021 metric sample):** a post-fix diagnostic run against the hash-bound `blocks_and_tables_-_metric.dwg` publishes 4,408/4,408 frames and reaches the FIELD refusal. The physical sample contains 162 FIELD objects in LibreDWG JSON; 14 nested values carry format flags `0x10801FC`. LibreDWG's projection for all 14 reports `data_type=0`, `data_long=256`, and `unit_type=1660972800`, so it does not provide a semantic or field-boundary oracle for this profile. ODA v5.4.1 §20.4.99 documents only bit `0x01` as the kGeneral selector and leaves these upper flags unspecified. Keep those fields raw-only and FIELD output refused; do not infer that the shared peer values are meaningful. This is a blocker refresh, not a support promotion. Receipt: [AC1021 FIELD unknown-flags screen](docs/research/p5-ac1021-field-unknown-format-flags-screen-2026-11-10.json).

**Latest implementation slice (2026-11-10, parsed TABLE fallback proxy authorization):** `dx_iface::addTable` retained an authorized TABLE snapshot, but its incomplete-table INSERT compatibility projection copied the parser's `replayAuthorized=false` cache. Authorization was also attempted against `dx_data::sourceFormat/sourceVersion` during callbacks, before `fileImport` populates those values. The adapter now authorizes only the unchanged projection copy when its proxy provenance is `ParsedDwg`; later preflight still requires the output revision to equal the captured source revision and validates the exact proxy chunk framing. The generated ASCII/Binary proxy round-trip test now begins with source metadata unset, verifies the retained TABLE and INSERT fallback copy carry separate authorization, and passes 1/1. Rechecking the hash-bound AC1021 `blocks_and_tables_-_metric.dwg` now gets beyond the prior `typed-proxy-graphics-not-preserved` gate: the reader publishes 4,408/4,408 frames with zero parse failures, then strict export refuses at `FIELD/FIELDLIST carriers have no qualified DXF serializer`. No output file was published. That source-bound FIELD gate remains closed pending complete source graph and field semantics evidence; do not weaken it. See [the TABLE fallback authorization receipt](docs/research/p8-ac1021-table-incomplete-fallback-proxy-authorization-2026-11-10.json).

**Latest implementation slice (2026-11-10, FIELD/FIELDLIST DWG encoder rollback):** `DRW_Field::encodeDwg` and `DRW_FieldList::encodeDwg` now stage body/string/handle destinations and publish only after a complete valid encode; `std::bad_alloc` and late refusal leave caller buffers untouched. Generated AC1021 sweeps inject all 25 FIELD and 11 FIELDLIST allocation failures, verify destination bytes/cursors and handle occurrence state, then retry the same object into the same seeded buffers and compare with clean output. Focused FIELD allocation, text-framing, CadValue, and oversized-count tests pass 4/4. This closes only local transaction behavior for the tested generated profiles; FIELD/FIELDLIST graph semantics, physical-source coverage, native acceptance, and support claims remain open. See [the FIELD rollback receipt](docs/research/p5-field-fieldlist-dwg-encoder-allocation-rollback-2026-11-10.json).

**Latest implementation slice (2026-11-10, TABLESTYLE DWG encoder rollback):** both legacy AC1021 and R24 AC1032 `DRW_TableStyle::encodeDwg` paths now stage body/string/handle streams before publication and return `false` on `std::bad_alloc`; distinct buffers and intentional aliases are preserved. Generated parsed legacy and source-initialized R24 profiles inject all 38 and 45 reached allocation failures respectively, verify seeded destination bytes/cursors and handle-occurrence state remain unchanged, then retry the same object into the same buffers and compare exact output with the clean encoding. All-three-stream alias routing is also checked for both branches. The focused three-test TABLESTYLE/CELLSTYLEMAP cohort passes. This closes only local writer transaction behavior; it adds no new field profile, physical-source qualification, native acceptance, or support claim. See [the TABLESTYLE rollback receipt](docs/research/p5-tablestyle-dwg-encoder-allocation-rollback-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1021 CELLSTYLEMAP writer rollback):** the generated rich style-map encoder previously wrote directly into caller buffers and leaked partial body/string/handle bytes when allocation failed at the first writer allocation. `DRW_CellStyleMap::encodeDwg` now stages each distinct destination while retaining intentional alias routing, publishes with no-throw swaps only after all streams are good, and converts `std::bad_alloc` to a clean `false`. The generated control sweeps 39 reached allocation failures, checks all destination bytes/cursors and modeled style data remain unchanged, retries the same object into the same buffers, and compares exact output to a clean encoding; it also checks all-three-buffer alias compatibility. Focused CTest passes 1/1. This is failure/retry evidence only; it adds no CELLSTYLEMAP field profile, producer coverage, or support claim. See [the allocation rollback receipt](docs/research/p5-ac1021-cellstylemap-encoder-allocation-rollback-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE fill-state polarity):** corrected the ACAD_TABLE cell override bit named “Background fill none” in ODA v5.4.1 §20.4.96.1 to the public model’s DXF group-283 “background color enabled” meaning. DWG parse and encode now invert the bit; DXF parse/write continue to use enabled polarity, and the existing table-level row mapping already performs the inverse. A generated paired-state wire test proves enabled→DWG 0 and disabled→DWG 1, then checks parser readback and the following body sentinel. The focused DWG cell-override and DXF appearance tests pass 1/1 each. No fixtures were added; no physical-source, native rendering, whole-table, or support claim is promoted. See [the polarity receipt](docs/research/p5-ac1018-table-fill-none-polarity-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1027 MATERIAL class-metadata parity):** an independent LibreDWG readback caught the typed MATERIAL registrar dropping class capability bit `0x80` and emitting class version/maintenance fields `0/0`. The typed row now emits the source-matched AC1027 class metadata `proxyflag=0x481`, version `23`, maintenance `14`; ODA §5.8 defines `0x80` as “cloning allowed.” The minJSON verifier now checks CLASSES metadata as well as graph and body fields, passes on both source Cover and source-fed output, and rejects flag/version mutations. The focused writer/reader cohort still passes, including all 219 same-writer rollback/retry offsets. This restores the selected AC1027 class contract; it does not qualify clone behavior, other producer profiles, native acceptance, or general MATERIAL support. No fixture or support claim was added. See the [source replay receipt](docs/research/p6-ac1027-material-source-fed-dwg-replay-2026-11-10.json).

**Completed implementation slice (2026-11-10, physical-source AC1027 MATERIAL graph replay):** the three exact-profile `ByBlock`, `ByLayer`, and `Global` records parsed from physical `Cover.dwg` now pass together through one selected AC1027 DWG write, preserving their retained bodies in the single `ACAD_MATERIAL` dictionary graph. Fresh libdxfrw readback verifies all three keys, remapped handles, common owner/reactor edges, complete typed fields, detached strings, and exact raw body bits. Dictionary admission is keyed by entry name and does not assume source ordering. The focused CTest and physical selector pass; this is source-fed graph transport, not a full-source rewrite or producer/native acceptance. No drawing fixture or support claim was added. See the [source replay receipt](docs/research/p6-ac1027-material-source-fed-dwg-replay-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1027 MATERIAL default-body replay):** the DWG writer now replays the exact 516-bit retained body and the five detached empty map-name strings for the parsed `Global`/`ByBlock`/`ByLayer` default profile only. A strict predicate checks all decoded body selectors and raw-frame version/size before output. Generated tests verify identical body bits, bounded string order, seeded-buffer preservation on mutation refusal, corrected same-object retry, and full generated AC1027 dictionary→MATERIAL file readback. The existing physical `Cover.dwg` audit confirms its three default materials meet the replay predicate; that source was not written back to DWG. Focused CTest passes 1/1, with no fixture or support claim. Other modern MATERIAL profiles remain refused. See the [replay receipt](docs/research/p6-ac1027-material-default-body-replay-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1024 associative-network action/handle-role binding):** the qualified AC1024 class-version-1 parser now requires each `isOwned` action flag to agree with its DWG handle role (hard ownership for owned actions, soft pointer otherwise). Generated mutations for both contradictory roles are withheld, and the same reader accepts the corrected frame on retry. The focused network/output/readback cohort passes 8/8; a rebuilt inventory reader accepts the existing hash-bound AC1024 architectural sample and parses all 28 action references across five networks. AC1032 handling is unchanged and remains a candidate-only parse with output refused. No drawing fixture or support claim was added. See the [role-binding receipt](docs/research/p4-ac1024-assocnetwork-action-handle-role-binding-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1032 associative-network multi-action parser control):** added a locally generated class-version-2 dispatch test with three logical actions, mixed ownership flags, and a distinct owned-action handle list. It verifies the exact candidate fields and complete bounded prefix while requiring `m_networkFieldsParsed=false`. The targeted reader-matrix build and both associative-network CTests pass (2/2). The values are guided by LibreDWG's projection of an unqualified ACadSharp AEC sample that libdxfrw cannot reach before OBJECTS; they remain candidates only. No production parser or writer behavior changed, no drawing fixture was added, and no support claim is promoted. AC1032 network schema, source spans/roles, graph closure, and output remain refused. See the [generated control receipt](docs/research/p4-ac1032-assocnetwork-multi-action-generated-2026-11-10.json).

**Latest implementation slice (2026-11-10, HATCH source-handle restoration regression):** fixed strict AC1021 conversion refusal caused by an adapter check that expected entity writers to leave a borrowed record mutated to its emitted handle. The writers now restore caller-owned handles, so the adapter proves output through the source→emitted map and write ledger; finalization still checks the emitted record identity, owner, and section. The four focused HATCH/MPOLYGON ASCII/Binary/runtime tests pass. The hash-bound local AC1021 tablet profile now converts strictly and fresh libdxfrw readback re-exports it successfully at the same version. No drawing fixture was added and no support claim is promoted. See the [regression receipt](docs/research/p8-ac1021-hatch-source-handle-restoration-regression-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE selected false title-suppression value):** added a focused generated encoder/parser vector for the admitted state where override bit `0x01` is set and its Boolean payload is false. Readback checks the mask, false value, exact body boundary, and following sentinel. The focused TABLE cell-override CTest passes 1/1. This closes the untested false branch for that scalar value only; physical-producer semantics and native TABLE behavior remain open. See the [scalar-override writer receipt](docs/research/p5-ac1018-table-scalar-override-dwg-writer-2026-11-08.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE→DBCOLOR whole-file readback):** the generated AC1018 TABLE writer route now emits its AcDbColor target as a DBCOLOR object in the same whole-file write. Fresh libdxfrw readback verifies the TABLE's AcDbColor hard-pointer handle and captures the DBCOLOR public callback with exact handle, RGB, method, and name. The focused CTest passes 1/1. The first test draft incorrectly asserted dx_iface's bounded importer-retention map without establishing the read-outcome contract; tracing showed the object parsed and its callback fired, so the harness now checks the callback directly. This closes generated transport/readback for the positive graph edge only; it does not establish source-producer prevalence or native color/rendering semantics. The generated DWG remains in `/tmp`, not in the repository. See the [whole-file receipt](docs/research/p5-ac1018-table-dbcolor-whole-file-readback-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE layer/linetype identity closure):** the compound TABLE gate now resolves the layer and any explicit linetype handle against the writer's normalized name→handle maps, and requires each matching table record to be emitted once deferred records are complete. Generated graph controls reject mismatched layer handles and unresolved named linetypes; the focused TABLE selector passes. This closes only reference identity/record-existence checks for the bounded TABLE route; it does not qualify physical-source semantics or native rendering. No fixture was added. See the [layer/linetype closure receipt](docs/research/p5-ac1018-table-layer-linetype-graph-closure-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE AcDbColor graph closure):** the compound TABLE gate now requires a flagged AcDbColor target to have an emitted object frame and class registration matching `AcDbColor`/`DBCOLOR`; an inconsistent flag/handle pair also refuses. The generated graph vector mutates the TABLE to a dangling AcDbColor handle and confirms refusal, while the focused TABLE selector passes. This retains the existing typed DBCOLOR writer route but does not yet prove successful TABLE→DBCOLOR full-file readback or native color semantics. No fixture was added. See the [AcDbColor graph receipt](docs/research/p5-ac1018-table-dbcolor-graph-closure-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE plot-style graph closure):** the direct TABLE codec continues to serialize an explicit plot-style hard pointer. The compound TABLE graph gate now accepts a non-default handle only when it is exactly the placeholder from the staged default plot-style graph and that placeholder frame has been emitted; a generated dangling-handle mutation is refused. The focused TABLE selector passes. This connects TABLE output to the existing provider graph without promoting physical-source or native semantics. No drawing fixture was added. See the [plot-style graph-closure receipt](docs/research/p5-ac1018-table-plotstyle-graph-closure-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE extension-dictionary preflight):** the bounded direct TABLE encoder now refuses a nonzero `xDictHandle`, because its common entity header emits that handle while this TABLE profile does not close or emit the extension-dictionary graph. The focused vector verifies seeded body/handle buffers survive refusal and that clearing the unsupported reference permits same-object retry. The focused TABLE selector passes; no drawing fixture was added. This closes only the direct-codec omission, not extension-dictionary output or physical/native TABLE qualification. See the [extension-dictionary preflight receipt](docs/research/p5-ac1018-table-xdictionary-preflight-2026-11-10.json).

**Latest verification slice (2026-11-10, AC1018 whole-file TABLE proxy retention):** the corrected generated AC1018 TABLE whole-file retry now uses both canonical parsed BLOCK_RECORD metadata and an explicitly authorized parsed-DWG proxy cache. Fresh file readback verifies the cache bytes/count alongside TABLE fields, graph closure, and all 15 `*T1` children. The focused whole-file CTest passes. This extends local generated no-loss evidence only; it does not qualify the physical source's compound graph, native cache semantics, or whole-drawing conversion. See the [parsed-handle graph receipt](docs/research/p5-ac1018-table-parsed-blockrecord-graph-retry-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 parsed TABLE handle through graph closure):** `hasClosedAc1018TableBlockGraph` now admits the exact parsed BLOCK_RECORD hard-pointer shape already validated by the direct TABLE encoder. The existing generated whole-file control still refuses a wrong TABLESTYLE NOD key with destination/caller rollback, then retries the same facade using parsed hard-pointer metadata; fresh readback verifies the role/ref/ref64 and TABLESTYLE/NOD/`*T1` graph plus all 15 cached children. The focused vector and both TABLE CTests pass. This extends generated graph transport only; it is not physical whole-source output or native TABLE evidence. No fixture was added. See the [parsed-handle graph receipt](docs/research/p5-ac1018-table-parsed-blockrecord-graph-retry-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 physical TABLE handle and proxy cache):** the direct AC1018 TABLE encoder now accepts the canonical parsed BLOCK_RECORD hard-pointer metadata and source proxy graphics only after explicit same-version parsed-DWG replay authorization. The generated vector verifies byte-identical body/handle output for normalized versus parsed handle metadata, refuses unauthorized proxy bytes without changing either destination buffer, then encodes after authorization and reads back the exact proxy cache, block-record handle, cell model, and following-body sentinel. The scratch ODA-converted physical peer (`acad_table_simple.dwg`, SHA-256 `aa88b0bd…918951ab`) now encodes and reads back its 3×3 TABLE, 0x9d BLOCK_RECORD handle, and 2,104-byte cache. This is a bounded selected object-body codec result: the source has an unrelated HEADER integrity warning, its `*T1` graph was not rewritten by this direct-body probe, and no native TABLE acceptance/rendering or broader producer claim follows. No fixture was added. See the [handle/proxy receipt](docs/research/p5-ac1018-table-blockrecord-proxy-writer-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE value flags):** ODA v5.4.1 §20.4.96.1 identifies the legacy TABLE value-flag profile (`0x06` base, `0x10` direction, `0x20` title-suppressed). The parser now marks other bits semantically incomplete; the bounded AC1018 body writer preserves documented flag combinations. A generated `0x16` write/readback and unknown-`0x40` refusal/retry control pass. Inspection of the physical ODA-converted peer confirms its `0x16` flag and exact 8 retained spans, and the later bounded direct-body slice now encodes that selected object after BLOCK_RECORD-role validation and explicit same-version proxy-cache authorization; whole-source output remains refused. This adds no fixture and does not qualify physical-source output or native table meaning. See [the flag-profile receipt](docs/research/p5-ac1018-table-value-flag-profile-2026-11-10.json).

**Latest implementation slice (2026-11-10, AC1018 TABLE retained no-op spans):**
the direct TABLE body encoder now admits only the complete ordered AC1018
zero-mask span signature observed in the physical peer: a 10-bit cell override
record per populated text cell and a 4-bit absent TABLE/border-mask tail. The
generated vector re-encodes parser-produced spans, checks typed readback and a
following-body sentinel, then verifies 9-bit truncation refusal, destination
preservation, nonzero raw-handle-code refusal and same-object corrected retry.
The focused vector and the
TABLE cell-override plus whole-file TABLE CTests pass. This adds no drawing
fixture and does not qualify physical-source conversion, native TABLE
semantics, rendering, or broader producer support. See the
[retained-range receipt](docs/research/p5-ac1018-table-retained-zero-override-range-writer-2026-11-10.json).

**Latest evidence checkpoint (2026-11-10, generated AC1018 TABLE whole-file writer):**
the exact bounded plain-text TABLE profile now writes through DWG finalization.
The writer requires `AcDbTable` class registration before manifest freeze,
stages the TABLE entity and `*T1` cached geometry, then checks the complete
TABLE → BLOCK_RECORD / TABLESTYLE → NOD dictionary graph after OBJECTS
emission and before publishing output. A deliberately wrong NOD key proves
final-closure refusal, destination preservation, restoration of caller TABLE
class/handle state, and corrected retry on the same facade. Fresh libdxfrw
readback verifies cell values, merge roles and all 15 cached children.
Independent LibreDWG 0.14 resolves the TABLE class, block/style handles, all
eight LINE endpoints, and seven MTEXT strings/placements/extents/attachments;
it leaves the TABLE cell body opaque, so this is graph/display-geometry
evidence only. No fixture was added. Native AutoCAD/ODA acceptance/rendering,
producer breadth and independent cell-semantic decoding remain open. See the
[whole-file TABLE receipt](docs/research/p5-ac1018-table-whole-file-cached-geometry-2026-11-10.json).

**Latest evidence checkpoint (2026-11-09, TABLEGEOMETRY AC1027/AC1032 source spans):**
the pinned ACadSharp samples each contain two standalone TABLEGEOMETRY objects.
For all four objects, a new independent Python bit walk and a separately
sequenced C++ cursor walk agree on exact body/string boundaries, cell/content
field intervals, and handle spans. Counts, all typed cell/content values,
owner targets, and null cell references match LibreDWG 0.14 and the typed
libdxfrw callbacks. The source-object bytes after the type prefix also match
LibreDWG's raw `unknown_bits` byte-for-byte. This establishes only the observed
profile: all 41 cell references are null soft pointers and all 37 standalone
unknown BD fields are zero. Keep non-null cell targets, nonzero unknown values,
native rendering, and whole-source output gated; no support claim is promoted.
A hash-deduplicated scan of 177 local DWGs (397 paths) found 18
TABLEGEOMETRY objects across 11 distinct files; none has a non-null cell
reference or nonzero standalone unknown BD. This exhausts the current local
corpus search, not the producer space. External DWGs and raw captures remain
outside the repository. See the [physical-span receipt](docs/research/p5-acadsharp-tablegeometry-ac1027-ac1032-physical-spans-2026-11-09.json),
[local corpus receipt](docs/research/p5-tablegeometry-local-nonnull-reference-corpus-scan-2026-11-09.json),
and reusable scanners
([C++ span walk](tools/research/tablegeometry_dwg_span_scan.cpp),
[independent Python verifier](tools/research/tablegeometry_physical_span_verify.py)).

**Resume checkpoint (2026-11-09, generated OBJECTS writer allocation safety):**
the AC1027 callback sweep covers FIELD, FIELDLIST, MOTIONPATH, CURVEPATH,
POINTPATH, OBJECTPTR, and PARTIALVIEWINGINDEX with LINE, POINT, and a two-vertex
POLYLINE target: 44 allocation points in ASCII and 42 in Binary. Every injected failure preserves
the whole-file destination and modeled caller records; retry reproduces
calibration bytes and typed readback verifies field values, FIELDLIST
membership, path fields, MOTIONPATH references to CURVEPATH/POINTPATH, the
CURVEPATH-to-LINE edge, OBJECTPTR identity, POINT fields, and the LINE/PVI
graph. Ordinary entities and POLYLINE/VERTEX preserve source handles on success
and failure; the compound INSERT/ATTRIB path retains minted handles when child
owner links need the emitted parent/child identities. `RecordStateScope::commit()`
restores entity handles by default, with only that INSERT/ATTRIB path opting
into identity retention. `writePolyline()` now restores each caller vertex
handle after emitting it, while retaining source-to-emitted mappings for the
DXF graph. The focused allocation sweep verifies LINE, POINT and POLYLINE/
VERTEX source state through calibration, every injected failure, and retry;
typed DXF readback checks the minted parent, vertex and owner identities.
Entity-reference remapping, INSERT rollback, vertex XDATA and polyline role
controls pass. Other entity types still require their owning controls. No
fixture was added. The successful-write/whole-file-failure lifecycle also has
an independently selectable fast CTest now: its ASCII/Binary retry profiles
assert that source POLYLINE/VERTEX handles remain intact, emitted identities
are obtained from the source-to-emitted map, and failed enclosing attempts
discard derived maps. The old assertions that expected successful caller-handle
mutation were corrected; the focused attempt-failure control passes.
See the [allocation receipt](docs/research/p6-dxf-partial-viewing-index-objects-allocation-failure-retry-2026-11-09.json).

**Resume checkpoint (2026-11-09, physical AC1032 MLINESTYLE dependency profile):**
The already-present LibreDWG `test/test-data/example_2018.dwg` at pinned
revision `6d6a33987a1a97095b069c799c3d8a793320f812` contains one AC1032
MLINESTYLE, handle `0x18`, with two element linetype refs to `0x15`. The
current parser reports ordered 16-bit hard-pointer spans at bits 627 and 643.
LibreDWG 0.14 independently reports both refs with handle role code 5 and the
same targets; its JSON does not independently expose those absolute span
offsets. A focused source selector also closes the typed NOD
`ACAD_MLINESTYLE` -> dictionary `0x17` -> style `0x18` path and both style
elements' dependency on the unique LTYPE `0x15` (`ByLayer`). The adapter now
admits this exact AC1032 source profile into its MLINESTYLE output-source
snapshot and rejects/restores mutations to either the element target handle or
the target LTYPE name. This closes selected-source admission only. A selected
physical export attempt stops earlier in general OBJECTS graph validation:
DICTIONARY `0x3A8` contains `ACAD_DIMASSOC` -> DIMASSOC `0x3B4` (type 526),
which has no admitted output closure. Add that DIMASSOC dependency to the
tracked OBJECTS graph and writer/readback route before retrying this physical
output profile; do not weaken graph validation or claim MLINESTYLE output from
source admission. The source read also has one unrelated
ACDBASSOCOSNAPPOINTREFACTIONPARAM object failure, so strict whole-source output
remains refused. No fixture was copied or added. See the
[source-profile receipt](docs/research/p6-ac1032-mlinestyle-libredwg-source-profile-2026-11-09.json)
and the [generated parser receipt](docs/research/p6-ac1032-mlinestyle-linetype-handle-roles-2026-11-09.json).

The AC1024 online sample search also re-confirmed public candidates: Autodesk's
catalog describes its samples as AutoCAD 2010 and later, while
[`nextgis/dwg_samples`](https://github.com/nextgis/dwg_samples) contains
AC1024 `*_2010.dwg` files but identifies AutoCAD 2016 M.49.0.0 as their
producer. These are useful AC1024-format samples, not verified AutoCAD 2010
RTM witnesses. Existing screening of 28 online AC1024 files found no CLASSES
high-bit failure; keep that issue evidence-gated and do not change the parser
from this negative screen. No downloaded sample is added to the repository.

**Resume checkpoint (2026-11-09, six-version external POINT read matrix):**
the hash-pinned AutoCAD 2016 `nextgis/dwg_samples` POINT now matches between
libdxfrw's current-source semantic adapter and LibreDWG 0.14 for handle `0x7c`,
WCS `(50, 50, 50)`, and extrusion `(0, 0, 1)` across AC1014/AC1015/AC1018/
AC1021/AC1024/AC1027; all five available paired source DXFs match as well. This
matrix exposed and fixed malformed JSON in the test adapter's named-summary
rows, which had blocked AC1015 and AC1018 comparisons. The controlled build,
adapter self-test and differential-runner self-test pass. This remains one
bounded POINT witness per pinned version and does not qualify other entities,
general version support, DWG writing or native geometry. The historical
Autodesk sample collection and LibreDWG's public `test/test-data/2010` corpus
were downloaded to scratch storage and screened: all 28 AC1024 DWGs reach
`readDwgClasses END` without a CLASSES CRC mismatch. Their per-file producer
builds are not established, so the exact AutoCAD 2010 RTM input or hash remains
needed; keep the issue open and do not change the parser from this negative
screen. No downloaded DWG/DXF was added as a fixture. See the
[external AC1024 sample screen](docs/research/p6-ac1024-external-sample-classes-screen-2026-11-09.json),
[six-version POINT matrix](docs/research/p6-autocad-point3d-six-version-read-matrix-2026-11-09.json),
[AC1024 sample RTM recheck](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json),
[AC1024 sample downloads](docs/research/p6-ac1024-online-sample-download-check-2026-11-09.json),
and [online sample source search](docs/research/p6-ac1024-online-sample-source-search-2026-11-09.json).

**Resume checkpoint (2026-11-09, AC1015 PERSUBENTMGR partial profile):** the
hash-bound AutoCAD 2000 D63 sample now publishes the exact first eight values
`[2,0,2,4,0,2,3,4]` as a `Partial` prefix at the R2000 RL pre-handles bound
(bit 150). The DWG/DXF pair agrees on those values; the candidate trailing
count crosses the bound and remains undecoded. The common-handle raw frame is
retained unvalidated. A generated control verifies a short-vector rollback,
same-parser corrected-input retry, and following-object publication. The
physical read still has 3 entity and 6 object parse failures elsewhere and
strict export remains refused by the unrelated IMAGEDEF/reactor capture gate.
See the [cross-version PERSUBENTMGR receipt](docs/research/p6-persubentmgr-acadsharp-cross-version-dxf-wire-conflict-2026-11-09.json).

**Resume checkpoint (2026-11-09, AC1018 TABLE encoder allocation rollback):**
the direct generated TABLE body codec now has an allocation-fault sweep over 18
failure points using a one-row/one-column plain-text cell with alignment and
text-style overrides. Every failure preserves the exact seeded body and handle
buffers; the same TABLE object succeeds after each failure. The focused
`--table-cell-overrides` selector passes. This hardens only direct codec
transaction behavior. **Historical status, superseded 2026-11-10:** that direct
codec checkpoint preceded the bounded generated AC1018 whole-file TABLE →
`*T1` writer, caller rollback, and independent graph/cached-geometry readback.
Physical source profiles, native rendering, and independent cell-semantic
decoding remain open. See the [TABLE readiness receipt](docs/research/p5-dwg-table-encoder-readiness-2026-11-08.json)
and [whole-file TABLE receipt](docs/research/p5-ac1018-table-whole-file-cached-geometry-2026-11-10.json).

**Completed P5 graph-identity guard (2026-11-10, AC1018 TABLESTYLE/NOD key):**
the generated AC1018 TABLE graph ledger now retains the emitted TABLESTYLE
handle/name as transaction-scoped state and requires the registered
`ACAD_TABLESTYLE` dictionary entry's key to match that emitted name
with exact byte length and ASCII-only case folding, and requires the emitted
TABLESTYLE parent handle to point back to that dictionary. This also refuses
embedded-NUL names whose prefix alone matches a dictionary key, and duplicate
dictionary entries that alias the same style handle. Closure requires exactly one captured
NOD-registered dictionary and TABLESTYLE record for the selected handles. Two
outer rollback controls deliberately stage a mismatched key and a mismatched
style parent; closure refuses both. The same writer then retries with matching
key and parent; dictionary/style state and the pending NOD registration are
rolled back before retry. The focused
`libdxfrw_dwg_table_cell_overrides` CTest passes 1/1. This closes only generated
graph-identity bookkeeping: whole-file NOD finalization, independent readback,
style-value semantics, cell-to-`*T` geometry correspondence, and native
rendering remain open. `dwgRW::writeTable` remains fail-closed. See the
[TABLESTYLE key-closure receipt](docs/research/p5-ac1018-table-nod-tablestyle-closure-2026-11-10.json).

**Completed P5 whole-file TABLESTYLE graph readback (2026-11-10):** the
generated AC1018 NOD → named dictionary → TABLESTYLE graph now runs through
`dwgRW::write()` and full file finalization. A fresh libdxfrw reader verifies
the root key, dictionary/style owners and three legacy row-style values.
Independent GNU LibreDWG `dwgread 0.14` parses the finalized bytes and confirms
the exact NOD/dictionary/style handle path, hard-owner flag, style name, row
text heights `[1,2,3]` and alignments `[5,5,5]`. The focused
`libdxfrw_tablestyle_nod_whole_file_roundtrip` CTest passes. The generated DWG
and peer JSON stay in `/tmp`; no fixture was added. This is a TABLESTYLE graph
interoperability result only: it emits no TABLE entity, does not qualify cell
semantics or `*T` geometry/rendering, and leaves `dwgRW::writeTable`
fail-closed. See the [whole-file receipt](docs/research/p5-ac1018-table-nod-tablestyle-whole-file-2026-11-10.json).

**AC1018 TABLE implementation ordering and status (updated 2026-11-10):**
`dwgRW::write()` freezes classes before TABLES, then calls `writeBlocks()`,
`writeEntities()`, and `writeObjects()`. The exact generated plain-text profile
now registers `AcDbTable` before freeze, emits the `*T1` BLOCK_RECORD and its
8 LINE/7 MTEXT cache, writes the TABLE entity, and validates the TABLESTYLE/NOD
path after OBJECTS before publishing. Late graph rejection preserves the old
destination and restores TABLE caller class/handle state through a
no-allocation snapshot swap; a corrected same-facade retry succeeds. Fresh
libdxfrw semantic readback and independent LibreDWG object/geometry checks
pass; the latter leaves the TABLE cell body
opaque. The compound graph closure check now accepts canonical parsed
BLOCK_RECORD hard-pointer metadata, and its existing whole-file graph control
passes destination-preserving refusal and corrected same-facade retry. This is
still generated graph transport, not native acceptance. Continue with the next
distinct TABLE readiness item: obtain native
AutoCAD/ODA acceptance and rendering for this profile or an independently
decoded cell-semantic peer, then qualify additional physical profiles only
when their body and reference closure are exact. The selected physical AC1018
body now re-encodes and reads back only after explicit same-version authorization
of its parsed proxy cache; the direct-body probe does not rewrite the compound
`*T1` graph. The ODA-converted peer has an unrelated HEADER warning, so it does
not establish whole-source or native acceptance. Keep FIELD/BLOCK content,
unknown/unmatched source subrecords, R24 TABLECONTENT, producer breadth and
native rendering gated. The direct-codec preflight rejects nonzero
extension-dictionary references until that graph is explicitly closed; the
compound TABLE gate admits only a non-default plot-style handle bound to the
emitted provider placeholder graph, resolves AcDbColor references only to an
emitted DBCOLOR-class frame, and matches layer/explicit-linetype references to
normalized names and emitted table records. A generated positive
TABLE→DBCOLOR full-file write/readback now confirms the exact reference and
typed callback payload; this does not establish native meaning or a physical
producer profile.
Do not infer cell semantics from cached geometry or class/graph closure alone. See the
[whole-file TABLE
receipt](docs/research/p5-ac1018-table-whole-file-cached-geometry-2026-11-10.json).

**Completed P5 guard slice (2026-11-09, exact cached TABLE string match):**
the AC1018 direct codec previously compared the cached `DRW_Variant` string
through `c_str()`. A public value containing an embedded NUL followed by extra
bytes could therefore compare equal to the cell text's prefix and be silently
truncated when the encoder emitted the separate cell text. `DRW_Variant` now
provides an exact length-and-byte `stringEquals` check, and the TABLE preflight
uses it. Generated controls verify atomic refusal with destination preservation
and corrected same-object retry. The focused
`libdxfrw_dwg_table_cell_overrides` CTest passes; no drawing fixture was added.
This closes one direct-codec model-consistency gap only. Keep the compound
writer refused until the physical body profile, semantic `*T` graph, outer
transaction, and independent readback gates pass. See the [exact string guard
receipt](docs/research/p5-ac1018-table-cached-string-exact-match-2026-11-09.json).

**Completed P5 rollback slice (2026-11-09, TABLE common-header caller state):**
the generated TABLE allocation sweep exposed a late failure that preserved
body/handle buffers but left `DRW_Table::plotFlags` recomputed by
`encodeDwgCommon`. The direct AC1018 encoder now restores all common-header
flag caches it can mutate unless both staged output buffers commit. The
25-offset rectangular-grid allocation sweep checks caller flag state on every
failure and confirms the successful same-object retry retains the emitted
derived flags. Its focused allocation CTest passes. This closes only direct
codec caller-state rollback; it does not qualify physical TABLE bytes, the
`*T` graph, public writer admission, or native behavior. See the [common-header
rollback receipt](docs/research/p5-ac1018-table-common-header-state-rollback-2026-11-09.json).

**Completed P5 model-domain guard (2026-11-09, TABLE linetype scale):** the
AC1018 direct TABLE codec now refuses zero and negative `ltypeScale`, matching
the existing shared DXF entity validation rule. Generated controls verify
atomic refusal for both values; the focused TABLE cell-override CTest passes.
This closes a model-domain inconsistency only and does not qualify native
linetype rendering or broaden the direct TABLE profile. See the [scale guard
receipt](docs/research/p5-ac1018-table-linetype-scale-domain-2026-11-09.json).

**Resume checkpoint (2026-11-09, AC1024 PERSUBENTMGR D63 partial profile):**
the hash-bound AutoCAD SAVEAS sample `sample_AC1024.dwg` at handle `0xD63`
now enters a source-matched partial-prefix path for values
`[2,0,2,4,0,2,3,4]`. The reader consumes only bits 47–111, exactly to the
validated body bound, records `ParseStatus::Partial`, and does not consume the
following candidate `num_subents`. The paired ASCII DXF has the same eight
group-90 values; LibreDWG independently reads the same prefix but overruns
when it guesses the next count. The raw source remains unvalidated and strict
output remains refused. The physical AC1024 sample now publishes 923/923
frames with zero entity/object parse failures; export stops at standalone
OBJECTCONTEXTDATA's unqualified replay path. The matching AC1021 source profile
also reaches its 151-bit body bound after the same eight values and publishes
925/925 frames with zero entity/object parse failures; export stops at the
separate IMAGEDEF owner-role gate. The generated focused selector passes,
including rollback/retry controls. See the
[cross-version conflict receipt](docs/research/p6-persubentmgr-acadsharp-cross-version-dxf-wire-conflict-2026-11-09.json).

**Resume checkpoint (2026-11-09, PERSUBENTMGR legacy raw-validation fix):**
the object dispatcher now marks the PERSUBENTMGR raw companion
`typedPayloadValidated=false` whenever its body prefix status is not
`Complete`, including `Missing` for the AC1018-and-earlier path. It retains the
existing associative callback and exact raw frame. A generated AC1018 control
checks the missing status, raw disposition, and following-object recovery.
`cmake --build build-gcc --target libdxfrw_dwg_reader_matrix_tests -j4 &&
build-gcc/libdxfrw_dwg_reader_matrix_tests
--persistent-manager-prefix-dispatch` passes. This fixes the validation
bookkeeping only; it does not decode the legacy body or change format/support
claims. See the [cross-version PERSUBENTMGR receipt](docs/research/p6-persubentmgr-acadsharp-cross-version-dxf-wire-conflict-2026-11-09.json).

**Resume checkpoint (2026-11-09, PERSUBENTMGR source-corpus expansion):** a
LibreDWG 0.14 minJSON scan of 178 already-local DWGs found 41
PERSUBENTMGR records across 26 source files. This broadens the evidence beyond
Platte's all-zero six-scalar suffix: the existing ACadSharp AC1014/AC1015
DWGs at handle `0xD63` report `numassocsteps=4` and steps `[3,4]`. LibreDWG
also reports subentities `[12783812,0,0,256]` for these versions, but its
AC1015 trace overruns the object while reading that candidate tail, so those
values are untrusted. Corresponding AC1021/AC1024 samples report the same
associated-step count and steps but no subentities. The paired
AC1024 AutoCAD ASCII DXF at handle `0xD63` has group-90 sequence
`[2,0,2,4,0,2,3,4]` and ends without a trailing subentity count. These are
useful nonzero/cross-version observations, but the peer JSON interpretation
and a DXF projection do not by themselves prove the DWG body grammar or its
semantics. The local AC1024 visualization fixture also produces an extreme
peer count (`numassocsteps=1060`, `numassocsubents=213`) and must be treated as
a separate anomaly until its raw frame and exact bounds are traced. No
production parser interpretation or support claim changes in this checkpoint.

The follow-up hash-bound scan confirms that paired AC1015/AC1021/AC1024 DXFs
at `0xD63` each end the group-90 sequence after `[2,0,2,4,0,2,3,4]`, while
AC1027/AC1032 end after `[2,0,2,0,0,0]`. LibreDWG's AC1015 trace parses the
common 68-bit shell and the same first eight candidate values, then reads a
candidate final count of `4` and overruns the 24-byte object. Its
`[12783812,0,0,256]` subentity vector is therefore not evidence of valid
fields. This weakens any argument that an absent DXF group-90 value proves an
absent DWG scalar; the body/handle/padding bit spans must be isolated first.
The local AC1015 libdxfrw CLI run publishes 1041/1047 frames with six object
parse failures and refuses export at the unrelated IMAGEDEF/reactor capture
gate. See the [wire/projection conflict receipt](docs/research/p6-persubentmgr-acadsharp-cross-version-dxf-wire-conflict-2026-11-09.json).

**PERSUBENTMGR disposition and next evidence:** the AC1015/AC1021/AC1024
prefix conflict is narrowed to a bounded partial profile: the paired DXFs
show eight group-90 values, while the candidate trailing count in peer
readers crosses the DWG body/handle bound. The physical D63 reads establish
the first eight values only; they do not establish the complete DWG grammar
or field meanings. Keep those records `Partial` with unvalidated raw
carriers. Do not spend another implementation slice retrying the same
overrunning count. AC1014 still needs a paired semantic projection before
typed-prefix admission; the AC1027 DataStorage blocker is resolved for the
exact multi-schdat reversed-role profile below, but that does not interpret
AC1027 PERSUBENTMGR; and AC1032's six-scalar body ends at bit 76 after
`[2,0,2,0,0,0]` with disputed meanings. Keep
AC1014/AC1027/AC1032 raw-only until those exact profiles are independently
reconciled. Any later grammar control must retain truncation rollback,
corrected-input retry, following-frame recovery, and byte-exact raw fallback.
The hash-bound cross-version observations remain in the
[conflict receipt](docs/research/p6-persubentmgr-acadsharp-cross-version-dxf-wire-conflict-2026-11-09.json).

**Resume checkpoint (2026-11-09, AC1027 multi-schdat DataStorage profile):**
the parser now admits the exact AC1027 reversed schema-index table-role
profile when its six schema roots and eight unknown-property headers are
partitioned across two `schdat` segments. The hash-bound physical
ACadSharp `sample_AC1027.dwg` proves the bounded references, four contiguous
8-byte header offsets per segment, root start at local offset 32, and both
system-data alignment limits. The generated bounds test now admits the
well-formed AC1027 vector and still rejects malformed references in both
AC1027 and AC1032. The physical read advances from the DataStorage error to
865/865 published frames with zero entity/object parse failures and zero
DataStorage link/orphan failures. Strict DXF export remains refused by the
independent AC1027 source-reference qualification gate; this does not promote
ACDS semantics, PERSUBENTMGR body support, or whole-file output. No fixture
was added. See the [AC1027 multi-schdat receipt](docs/research/p6-ac1027-multischdat-reversed-role-profile-2026-11-09.json).

**Resume checkpoint (2026-11-09, AC1027 Cover 3DSOLID source-audit join):**
the repeatable source-reference inventory now includes the exact History ID
code, target, bit span, and parse-complete status for 3DSOLID records. For the
hash-bound Cover handle `0x6f`, the verifier requires the ODA §20.4.41 code-3
History ID at bits 209–217 with null target and a unique layer-name `0` lookup
to LAYER `0x10`; it classifies this one record as a source-bound
History-ID profile. The refreshed audit covers all 88 retained identities,
129 selected typed-reference rows, zero selected-report-only rows, and zero
rows labeled not-exhaustively-decoded. This is an exact source profile, not
proof that all entity/table references, owner roles, or omitted dependencies
are closed. The separate HEADER suffix and aggregate output disposition still
block strict export. See the [3DSOLID source-audit receipt](docs/research/p6-ac1027-cover-3dsolid-source-audit-closure-2026-11-09.json).

**Verified bounded common-handle role slice:** AC1018+ entity and OBJECTS
common owner/reactor/extension-dictionary roles now follow ODA v5.4.1
§§2.13, 20.1 and 20.4.2. Entity and object generated controls reject each
wrong role, cover nulls and relative soft-pointer owner forms, preserve staged
outputs/cursors, and retry corrected input. Every direct
`readObjectCommonHandles()` call is version-aware. The OBJECTS vector builder's
common-parent role mistakes were corrected; actual owned/body references keep
their hard-owner roles. The AC1024 abbreviated PERSUBENTMGR source profile is
explicitly tested as partial, while AC1027/AC1032 variants remain refused.
Focused entity/object selectors and the full object-vector cohort pass; all
25 tests in the AC1018/AC1024 labeled cohort pass. The existing ACadSharp
AC1027 sample publishes 865/865 frames with zero entity/object parse failures.
No fixture was added. Strict output remains refused by the independent
source-reference gate. See the [entity receipt](docs/research/p6-ac1018plus-common-entity-handle-role-validation-2026-11-09.json)
and [OBJECTS receipt](docs/research/p6-ac1018plus-common-object-handle-role-validation-2026-11-09.json).

**Completed P6 local implementation packet — DIMSTYLE body handle roles and
spans:** ODA v5.4.1 §20.4.68 is implemented for the AC1015+ version gates.
`DRW_Dimstyle::parseDwg` now transactionally validates the common DIMTXSTY
pointer, R2000+ leader/arrow-block pointers, and R2007+ linetype pointers;
only canonical null code/target pairs are accepted. It records ordered bit
spans, exposes the external-reference-block handle, and resets cleanly on
truncation before corrected same-object retry. Generated vectors cover
AC1015/1018/1021/1024/1027 boundaries, wrong roles, nulls, span sentinels,
copy/assignment/reset, and failure/retry. DXF and DWG strict output refuse a
nonzero unresolved external-reference-block target. The hash-bound AC1027
Cover record is DIMSTYLE `0x27` (`ISO-25`): nine spans from bit 1290 through
1370; DIMTXSTY resolves to STYLE `0x11`. LibreDWG 0.14 independently matches
all nine field roles and targets. The affected fast CTest cohort passes 8/8;
no drawing fixture was added. This is one source profile plus generated codec
evidence, not broad producer or native-app qualification. Receipt:
[`p6-ac1015plus-dimstyle-body-handle-roles-spans-2026-11-09.json`](docs/research/p6-ac1015plus-dimstyle-body-handle-roles-spans-2026-11-09.json).

**Completed P6 local implementation packet — VPORT body handle roles and
spans:** ODA v5.4.1 §20.4.64 is implemented across the R13 through R2013
reader versions. The parser validates the common control and reactor soft
pointers, optional xdictionary hard owner, xref-block hard pointer, R2000+
named/base UCS hard pointers, and R2007+ background, visual-style, and SUN
roles. It preserves raw role codes and exact ordered bit spans; canonical
code-0/target-0 nulls, wrong-role rollback, final-field truncation, and
same-object retry have generated controls. The version matrix covers AC1012,
AC1014, AC1015, AC1018, AC1021, AC1024, and AC1027. The hash-bound AC1027
Cover VPORT `0x29` (`*Active`) has seven captured fields; LibreDWG 0.14 matches
all seven role codes and targets, including VisualStyle `0x2f` and SUN `0x6d`.
The five-test focused VPORT/object-vector cohort passes. No fixture was added.
This is one physical profile plus generated parser evidence; strict graph
closure, visual semantics, broad producer support, and native-app
qualification remain open. Receipt:
[`p6-ac1012plus-vport-body-handle-role-spans-2026-11-09.json`](docs/research/p6-ac1012plus-vport-body-handle-role-spans-2026-11-09.json).

**Completed P6 local implementation packet — UCS table-record handle roles
and spans:** ODA v5.4.1 §20.4.62 (pp. 176–177) now drives `DRW_UCS::parseDwg`
role checks and ordered raw-role/bit-span capture for the control soft pointer,
reactors, optional xdictionary hard owner, xref-block hard pointer, and R2000+
base/named UCS hard pointers. Generated AC1012/14, AC1015/18, and
AC1021/24/27 controls cover each wrong role with atomic same-object retry,
canonical nulls, exact span/role/target comparison against an independent wire
walk, a following sentinel, copy/assignment/reset, final-field truncation, and
corrected retry. The focused UCS, VPORT, and object-vector cohort passes 3/3;
no fixture was added. The LibreDWG 0.14 census of five valid local minJSON
outputs found no UCS table records; one malformed JSON output was excluded.
This closes the local generated-codec action only. Physical UCS field-role
evidence and target-graph closure remain open, and no support claim is
promoted. Receipt:
[`p6-ac1012plus-ucs-body-handle-role-spans-2026-11-09.json`](docs/research/p6-ac1012plus-ucs-body-handle-role-spans-2026-11-09.json).
Code, focused controls, and UCS evidence receipt were committed as `f050ad28`.

**Completed P6 local implementation packet — VIEW body-handle roles and
spans:** ODA v5.4.1 §20.4.60 (pp. 173–175) confirms R2000+ Base UCS and Named
UCS handles are present even when the body `HasUCS` bit is false, before the
R2007+ Live Section handle. `DRW_View::parseDwg` and the writer now preserve
that order. The parser validates bounded control/reactor, xdictionary,
xref-block, background, visual-style, SUN, base/named UCS, and live-section
roles, and records ordered bit spans. Generated AC1012/14 and AC1015/18/21/24/27
vectors cover legacy common-handle parsing and, from AC1015 onward, `HasUCS`
both ways, exact field spans and roles against an independent wire walk,
canonical nulls, each wrong-role rollback, final-field truncation, copy/reset,
and same-object retry; the focused CTest passes 1/1. The local
AC1027 Cover source publishes 111/111 frames with no entity/object parse
failures or CRC mismatches and no VIEW role mismatch, but strict output remains
refused by the separate source-reference gate. This closes only the local
VIEW codec slice; graph closure, full producer coverage, native behavior, and
strict whole-source output remain open. The focused AddressSanitizer selector
also passes without diagnostics. No fixture was added. Receipt:
[`p6-ac1012plus-view-body-handle-roles-spans-2026-11-09.json`](docs/research/p6-ac1012plus-view-body-handle-roles-spans-2026-11-09.json).

**Completed local investigation — PERSUBENTMGR D63 boundary traces:** fresh
LibreDWG 0.14 `-v5` traces for hash-bound AC1015/AC1021/AC1024 sources confirm
the eight candidate prefix values and record the respective 150-, 152-, and
112-bit object-data limits and handle-stream starts. The peer's candidate
trailing count overruns AC1015, is absent at the AC1021 bound, and is
out-of-bounds for AC1024. Its per-step cursor labels overflow, so no exact
subfield span or class schema is accepted from these logs. The paired DXFs
still carry the same eight group-90 values; this is insufficient to infer
complete DWG grammar or semantics. Receipt:
[`p6-persubentmgr-libreDWG-v5-boundary-traces-2026-11-09.json`](docs/research/p6-persubentmgr-libreDWG-v5-boundary-traces-2026-11-09.json).

**Next local work — continue implementation while leaving PERSUBENTMGR
interpretation unchanged:** AC1014 has no paired DXF in the available corpus;
AC1027/AC1032 semantic field meanings remain disputed. The ACadRust source
screen lists a candidate sequence with separate count-prefixed `steps[]` and
`subentities[]` arrays, but does not bind that parser revision to the physical
source files. Its candidate conflicts with the paired DXF value projections
and does not resolve the previously observed DWG body-boundary overruns; see
the [ACadRust schema screen](docs/research/p6-persubentmgr-acadrust-schema-screen-2026-11-10.json).
Keep existing raw-only/Partial profiles. Do not add the peer candidate to the
production parser or retry the same unstable tail decode. Revisit only with a
pinned parser run on the exact hash-bound inputs or a native producer/consumer
capture that resolves body boundaries and array semantics. Any eventual parser
change must retain truncation rollback, corrected-input retry,
following-frame recovery, and byte-exact raw fallback. Add no drawing fixture
and promote no support claim from transport-only agreement.

**Resume checkpoint (2026-11-09, latest desktop inventory):** after the
user's unlock-in-progress reply, the native-app inventory still reports “The
Mac is locked and automatic unlock could not unlock it” and lists no native
apps. No ODA or FreeCAD process was launched. This is an access-state condition
only; it supplies no format evidence and changes no support claim. Keep native
ODA/FreeCAD qualification queued and do not retry the unchanged ODA File
Converter 27.1 CLI route that aborts before processing. Continue with other
independently actionable local work. Retry native qualification only after a fresh
inventory exposes an unlocked session or a materially different launch path is
available.

**Resume checkpoint (2026-11-09, AC1032 control-reactor slice):** the
VPORT_CONTROL reader now consumes declared common reactor references before
the XDictionary and table-entry handles. The generated AC1032 regression
proves reactor `0x3d126` stays separate from VPORT row `0x57`, and checks
truncation reset/retry and the zero-reactor case. The focused target passes.
All five scratch-only public DWGs from LibreDWG issue #1021 now pass VPORT and
reach OBJECTS. Platte and Stahl publish every observed frame with zero
entity/object parse failures; Teile, test-trichter and xx17 retain 2, 1 and 3
unrelated frame failures respectively. Strict DXF output remains refused due
to those failures and the corpus's large custom/unsupported object sets.
Platte's PERSUBENTMGR object `0x20c50` is now reachable: its bounded 84-bit
body ends after scalar values `2, 0, 2, 0, 0, 0`. The AutoCAD 2019 and 2027
paired DXFs contain the same six group-90 values. A generated exact-profile
regression now verifies raw-only byte retention, common-link validation,
following-frame publication and failure/retry behavior. Keep it raw-only:
the zero-valued suffix cannot distinguish competing field meanings. See the
[PERSUBENTMGR profile receipt](docs/research/p6-ac1032-persubentmgr-six-scalar-raw-only-profile-2026-11-09.json).
The
source archive, hashes, per-file outcomes, test command and limitations are in
the [control-reactor receipt](docs/research/p6-ac1032-control-common-reactor-fix-2026-11-09.json);
the [source-pair receipt](docs/research/p6-ac1032-vport-public-pair-read-blocker-2026-11-09.json)
records the pre-fix baseline and provenance.

**Resume checkpoint (2026-11-09 08:57 UTC):** after the user's latest
“I’ll unlock it now” reply, a fresh desktop inventory still reports the
session locked and exposes no native app surface. Native qualification could
not be resumed in this pass. Treat this as a desktop-access blocker only; it
is not file-format evidence. Do not retry the unchanged ODA File Converter
27.1 CLI path, which previously aborted during Pasteboard/XPC initialization
before processing input. Resume native qualification when a fresh inventory
exposes the unlocked session or a materially different launch route is
available. Continue independently actionable local work. The selected AC1021 FreeCAD
torus B-rep check is recorded in
[`p7-ac1021-freecad-native-torus-brep-2026-11-08.json`](docs/research/p7-ac1021-freecad-native-torus-brep-2026-11-08.json);
its one-body/one-face/one-solid result is a bounded selected-profile check,
not independent-kernel, whole-source, platform, or support-claim evidence.

This checkpoint is an access-state refresh only: it does not alter the earlier
successful GUI conversions or their hash-bound receipts, does not qualify any
new file-format profile, and does not change support claims. No ODA or FreeCAD
process was launched during this recheck.

**Desktop access refresh (2026-11-09, after the latest unlock-in-progress
reply):** a new native-app inventory again returned “The Mac is locked and
automatic unlock could not unlock it.” No app was launched. Leave the ODA and
native FreeCAD qualification items queued; continue with local source-bound
work and retry only after a later inventory reports an unlocked session.

**Latest desktop check (2026-11-09, after the user's unlock message):** the
fresh inventory still reports “The Mac is locked and automatic unlock could
not unlock it” and lists no native apps. No ODA or FreeCAD process was
launched. This is an access-state blocker only; it is not format evidence. Do
not retry the unchanged ODA CLI route. Continue independently actionable local
work and inspect desktop state again before the next native qualification
attempt.

**Earlier access recheck (2026-11-09 07:51 UTC):** following the user's
unlock-in-progress reply, direct CUA inventory still returned “The Mac is
locked and automatic unlock could not unlock it”; no native apps were listed.
No ODA or FreeCAD process was launched, and this observation does not qualify
or disqualify any drawing profile. Retry native qualification only after a new
inventory reports an unlocked session; until then, proceed with local slices
whose evidence does not depend on the desktop.

**Latest implementation checkpoint (2026-11-09, commit `8a0ef446`):** the
AC1027 and AC1032 PERSUBENTMGR generated controls now cover the complete
candidate scalar/vector prefix and the one-bit-short common-handle failure.
They verify exact values, bounded Complete prefix status, an unvalidated raw
carrier, following-frame recovery and corrected-input recovery. The focused
`libdxfrw_dwg_reader_matrix_tests --persistent-manager-prefix-dispatch` selector
passes; no fixture or production parser change was added and no support claim
was promoted. See the
[PERSUBENTMGR receipt](docs/research/p6-persubentmgr-common-handle-truncation-retry-2026-11-09.json).
The preceding AC1024 CLASSES generated readback and corpus check remains
recorded: its focused selector reads three 2048-character strings from a
from-scratch file, while all nine local AC1024 sources pass the current reader.
The online Autodesk and LibreDWG candidate sweep adds 28 hashed AC1024 inputs;
all 28 reach the CLASSES end marker without CRC mismatch. Exact AutoCAD 2010
RTM producer provenance and the missing tenth source/hash remain unknown, so
these results do not resolve the six-of-ten report. Do not alter the high-size
path from these passing candidates; keep an exact failing input as the evidence
needed to revisit it. See the [external sample screen](docs/research/p6-ac1024-external-sample-classes-screen-2026-11-09.json)
and the
[AC1024 CLASSES receipt](docs/research/p6-ac1024-classes-repository-corpus-recheck-2026-11-09.json).
The online search found Autodesk's sample collection labeled "AutoCAD 2010
and later" and the NextGIS AC1024 corpus, whose README identifies AutoCAD 2016
M.49.0.0 as the producer. A direct download of the contemporaneously listed
Autodesk `visualization_-_aerial.dwg` is AC1024 and reaches `readDwgClasses END`,
but its hash is byte-identical to the existing repository fixture, and the
specific producing AutoCAD build is not independently established. Four other
direct Autodesk collection downloads checked here are AC1021. None supplies a
verified AutoCAD 2010 RTM witness for the reported CLASSES failure. The failure
stays unresolved; do not infer an RTM defect from these later or ambiguously
sourced samples. See the [AC1024 online sample receipt](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json).
The follow-up public-web search reconfirmed those same source classes: Autodesk's
current sample catalog says "AutoCAD 2010 and later" without per-file authoring
builds, and the NextGIS README explicitly names AutoCAD 2016 M.49.0.0. Search
results describing files merely as AC1024 or "AutoCAD 2010 format" do not add
producer provenance and are not sufficient to qualify the reported RTM-only
failure. Do not spend another broad download pass on such candidates. Resume this
investigation only with the original six failing inputs or their hashes/traces,
the missing tenth file, or a source independently tied to an AutoCAD 2010 RTM
build; keep any acquired external DWG/DXF under scratch storage. This avoids
duplicating the already screened 28-file AC1024 corpus without relaxing the
evidence gate.
The pinned NextGIS repository tree also exposes seven paired AC1024 DWG/DXF
profiles (`arc`, `circle`, `ellipse`, `line`, `point2d`, `point3d`, and
`polyline2d_line`). `arc_2010`, `circle_2010`, `line_2010`, `point3d_2010`,
and `polyline2d_line_2010` have been downloaded and checked; the POINT matrix
also covers `point3d_2010` across six versions. Their documented AutoCAD 2016
producer does not qualify RTM. The remaining two profiles are not needed to
settle that provenance question; see the [online sample receipt](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json).
The preceding P5 TABLE unknown-prefix profile remains recorded in
[`p5-ac1024-table-unknown-prefix-default-gate-2026-11-09.json`](docs/research/p5-ac1024-table-unknown-prefix-default-gate-2026-11-09.json).

**P8 LibreCAD_3 CI follow-up (2026-11-09, live status checked 06:45 UTC):** the previous PR #456 head
`321f2cad` passed build and required regressions on macOS arm64, then failed
while installing the app because the runner's cached Homebrew formula/API
snapshot produced runtime-dependency metadata inconsistent with the bottles
downloaded later in that job. The existing PR branch now includes commit
`7e81275b`, which explicitly refreshes Homebrew metadata before dependency
resolution; focused installer tests verify call ordering and fail-closed
behavior. Current PR head is
[`7e81275b71038d4a42fa580842e661ddcee7adbf`](https://github.com/LibreCAD/LibreCAD_3/pull/456/commits/7e81275b71038d4a42fa580842e661ddcee7adbf).
The live PR head remains `7e81275b71038d4a42fa580842e661ddcee7adbf`, open and
mergeable. On that exact head, Plan Sync and both Python/macOS bundle jobs are
queued; CodeQL Python has passed while CodeQL C++ is queued; Windows, Linux
x64, Linux ARM64, AppImage, and DWG sanitizer jobs are in progress; Windows
persistence has passed. Keep packaging/platform qualification open until the
remaining hosted checks complete, and retain the previous CI failure as
historical evidence. See the
[Homebrew metadata refresh receipt](docs/research/p8-librecad3-homebrew-api-snapshot-fix-2026-11-09.json).

**P8 current-head CI recheck (2026-11-09 07:58 UTC):** PR #456 remains open
and mergeable at `7e81275b71038d4a42fa580842e661ddcee7adbf`. The latest hosted
run (`42573888375`, `42573888467`, `42573888513`) passes Linux x64/ARM64,
Windows, Windows persistence, AppImage, DWG sanitizers, CodeQL C++/Python, and
Plan Sync. Both macOS bundle jobs fail on this head for distinct packaging
environment reasons: Intel Homebrew has no bottles for the requested formulas
and the bottle-only dependency installer exits; Apple Silicon builds and tests
but Python-runtime staging refuses because no cached `zstd 1.5.7_1` bottle
manifest matches the installed `lz4 1.11.0` dependency receipt. The arm64 log
then reaches its evidence-artifact upload step, but the package stage itself
failed. Preserve the
exact Homebrew origin checks while resolving these failures; do not turn either
red CI result into a format or consumer-support claim. The evidence and
precise next investigation are in the
[current CI receipt](docs/research/p8-librecad3-pr456-current-ci-2026-11-09.json).

**P8 arm64 provenance diagnosis and local repair (2026-11-09):** inspection of
the actual zstd/lz4 Homebrew receipts and the staging code shows the rejected
`runtime_dependencies` comparison treated Homebrew's package-level dependency
table as the runtime Mach-O load graph. Homebrew can regenerate that table
after a pour, and the selected versions may change while the original keg and
its bottle remain valid. `RuntimeStager` already traverses each actual
`LC_LOAD_DYLIB` edge and independently captures the exact source path, bottle
identity, architecture, and original-byte digest for every library copied.
The local LibreCAD_3 candidate commit `1a012ff6` schema-validates both retained
and installed dependency tables without using either as a keg-resolution
graph; exact source/bottle checks remain mandatory for each actual selected
load. Focused provenance
tests pass 42/42, including updated-table/replaced-dependency-table controls,
malformed table rejection, independent rejection of a source-built loaded
dependency, and a direct `RuntimeStager._stage_external` control proving source
capture precedes byte-identical staging and rejected source-built loads publish
no destination. The regression and its plan update are committed locally as
`8412c5e0`; the 12-gate worktree audit passes with no unmapped paths or stale
evidence. This is local fixture evidence only: the revised code has not yet
passed hosted arm64 CI or been published to PR #456. Automatic review rejected
the GitHub content upload because it would publish a large internal
implementation plan; it explicitly disallowed retrying through another route.
Follow-up plan synchronization in the isolated checkout passed `--full`, all
31 plan-checker regression tests, the worktree coverage audit, and
`git diff --check`. The detailed commands are in the receipt. The local
implementation commit is preserved, and PR publication now needs the user's
explicit approval for that disclosure. Intel remains blocked because the
runner cannot obtain the required bottles under `--force-bottle`; determine a
supported binary provider/runner policy without permitting silent source
builds. See the
[arm64 provenance repair receipt](docs/research/p8-librecad3-homebrew-load-graph-repair-2026-11-09.json).

**P8 Intel dependency-provider policy research (2026-11-09 08:27 UTC):**
Homebrew's current support policy classifies Intel x86_64 as Tier 3, says it
has stopped building new Intel bottles, and expects Homebrew support to be
removed in or after September 2027. The same policy recommends MacPorts but
explicitly says its Intel Tahoe binary set is incomplete. MacPorts' port index
does show x86_64 binary availability for individual required components such
as Python 3.14 and Qt 6, but the observed Boost shim points to Boost 1.76 and
the complete LibreCAD_3 dependency closure has not been proven. Therefore an
Intel Homebrew `--force-bottle` lane is not a sustainable fix target, while
MacPorts is only a candidate provider, not yet a qualified replacement. Decide
whether Intel remains a release target; if it does, inventory the complete
binary closure and provenance/licence records under a supported provider before
implementing the installer. Do not use silent source-build fallback or mark
the current x86_64 job advisory. Sources: [Homebrew support tiers](https://docs.brew.sh/Support-Tiers),
[MacPorts Python 3.14](https://ports.macports.org/port/python314/details/),
[MacPorts Qt 6 base](https://ports.macports.org/port/qt6-qtbase/details/), and
[MacPorts Boost](https://ports.macports.org/port/boost/details/). See the
[provider-policy receipt](docs/research/p8-intel-macos-binary-provider-policy-2026-11-09.json).

**LibreCAD_3 P8 compatibility review:** the isolated consumer branch is based
on `5cf435f4` and contains code commits `36082e71`, `93354846`, and `762a0f1e`,
plus same-change plan-sync receipt commit `8269dd7b`. The upstream
`DRW_Header::write()` itself gates `$INSUNITS` to targets newer than AC1014;
the consumer patch now applies the same version gate to preserved and default
header values, reports explicit unit-metadata loss for non-unitless R12/R14
exports, and checks that a LINE remains in the output. Existing raw-object
fixtures were edited in place to remove the optional 330 owner from the first
dictionary in the low-handle and preservation cases; no fixture was added.
The focused current-HEAD rebuild succeeded;
`DxfRoundTripTest.DrawingUnitsSurviveASave` passed for R12 and R14 across inch,
millimetre, and metre inputs plus a unitless control, and the current-head
`DxfExportTest.*:DxfRoundTripTest.*` cohort passed 64/64. The earlier saved
64-test log predates the R14 test extension, but this rerun includes it. The
unitless case confirms R12/R14 exports omit `$INSUNITS` without reporting a
loss, while each nonzero unit reports exactly one loss note and retains its
LINE. The reviewed unit-loss slice has no identified correctness blocker. PR
[#456](https://github.com/LibreCAD/LibreCAD_3/pull/456) is open against
`LibreCAD/LibreCAD_3:master`; GitHub reports no merge conflict. Code head
`762a0f1e` has a plan-only follow-up at `8269dd7b`; its focused behavior result
remains the 64/64 run on the unchanged code. PR #456's prior Plan Sync failure
was repaired. Commit `fd5af0b8` raises the Windows package-layout test's
temporary CMake timeout from 30 to 120 seconds
after hosted `BuildWindows` timed out at 30 seconds; the Windows package smoke
suite passes 19/19 locally on macOS. Plan commit `321f2cad` records that result.
The consumer-plan `--full`, explicit-path, actual-base (`origin/master`), and
31-test checks pass locally. On current PR head `321f2cad`, Plan Sync passed;
`BuildWindows`, Linux x64/ARM64, AppImage, CodeQL C++, and both macOS jobs are
in progress; Plan Sync, Windows persistence, DWG sanitizers, and CodeQL Python
passed. The aggregate CodeQL check is neutral because the base branch lacks
its C++ configuration. Keep native Windows/Linux and
application qualification open. This consumer review does not change DWG/DXF support claims.

**Earlier checkpoint (2026-11-08, before the latest ODA launch attempt):** after the user said
they would unlock the Mac, the desktop inventory still reported the session
locked and exposed no native app surface. No ODA process was retried in this
check because the unchanged CLI route has already failed during Pasteboard/XPC
initialization before argument or document processing. No new native conversion
result was observed. Keep native qualification pending; do not repeat the
unchanged ODA CLI route until the desktop reports an unlocked session or the
launch path materially changes. The latest local TABLE work extends commit
`410f6c51` with
order-independent group-141/142 dimension retention and fail-closed handling
for nonzero group-94 border-color masks. Focused ASCII/Binary controls pass
([dimension receipt](docs/research/p5-dxf-table-physical-dimension-cardinality-2026-11-08.json),
[border-color receipt](docs/research/p5-table-border-color-mask-incomplete-2026-11-08.json)).
The earlier `prepareTableEntryEed()` guard rejects pending APPID/LAYER
references and unmodeled group-102 appData before a record frame is opened,
while allowing canonical `ACAD_REACTORS` and `ACAD_XDICTIONARY` groups only when
their handles match the modeled fields. Its focused generated control verifies
opaque/mismatched refusal, both pending-reference families, canonical reactor
and xDictionary acceptance, destination preservation, and same-writer retry
([receipt](docs/research/p5-table-entry-common-appdata-preflight-2026-11-08.json)).
This guard prevents silent loss; it does not qualify additional table-record
fields or promote output support claims.

The generated-only AC1018 TABLE codec now admits exactly ODA
v5.4.1 §20.4.96.1 scalar override mask `0x1f` (title suppression,
implicit-true header suppression, flow direction, horizontal margin, and
vertical margin). The direct body writer emits the selected values in the
documented order; generated parsing checks exact values, body cursor, and a
following sentinel. Unsupported bits, mismatched flag/value state, invalid
flow values, and invalid or unflagged margins remain atomic refusals. The
focused `libdxfrw_dwg_table_cell_overrides` CTest also round-trips all 32
subsets of the five admitted mask bits, checking typed values, exact body
cursor, and a per-mask sentinel. This is codec self-consistency only: no
producer-backed physical witness, semantic graph
closure, public `dwgRW::writeTable` enablement, or support-claim promotion.
See [the scalar override receipt](docs/research/p5-ac1018-table-scalar-override-dwg-writer-2026-11-08.json).

The direct AC1018 writer admits text-cell alignment (`0x01`), the group-283
background-enabled model value with inverse DWG fill-none polarity (`0x02`), indexed background/content colors (`0x04`/`0x08`),
the cell text-style hard pointer (`0x10`), text height (`0x20`), and the four
physical edge color, lineweight, and visibility families (`0x0003ffc0`) for
plain-text cells. It emits selected
fields in ODA §20.4.96.1 order. Generated readback checks those fields,
edge-mask mapping, exact body boundary, and following sentinels; a combined
`0x3ffff` vector exercises all currently admitted bits and the interleaved
hard-pointer slot together. The reader
decodes alignment's raw 16-bit value and marks out-of-domain values
semantically incomplete; the text-style pointer is role-checked as a hard
pointer and emitted in the cell-override handle slot. Color admission is
limited to canonical indexed CMC; named and true-color forms remain
incomplete/refused. Unsupported cell
masks, malformed edge/value correspondence, invalid selected
alignment/height/lineweights, and unflagged value residue remain atomic writer
refusals. The focused TABLE CTest passes. This profile does not interpret the
physical source's `0x40000`/`0x40020` masks and does not promote
physical span, public `writeTable`, graph-closure, or interoperability claims.
See [the text-height receipt](docs/research/p5-ac1018-table-text-height-cell-override-dwg-writer-2026-11-08.json).
See also [the four-edge receipt](docs/research/p5-ac1018-table-cell-border-overrides-dwg-writer-2026-11-08.json).
See [the cell-alignment receipt](docs/research/p5-ac1018-table-cell-alignment-override-dwg-codec-2026-11-08.json).
See [the cell-appearance receipt](docs/research/p5-ac1018-table-cell-appearance-overrides-dwg-writer-2026-11-08.json).
See [the cell text-style receipt](docs/research/p5-ac1018-table-cell-text-style-override-dwg-writer-2026-11-08.json).

**Completed AC1018 TABLE DXF cell-appearance slice:** selected cell fill-state
(`0x02`), indexed background/content colors (`0x04`/`0x08`), and uniquely
resolved text-style hard pointers or name-only overrides (`0x10`) now map to
groups 283, 63, 64, and 7.
The generated ASCII/Binary control checks explicit false and true group-283
values, indexed endpoints, handle/name-only STYLE resolution, field order,
refusal, destination preservation, and same-writer retry. See the [DXF mapping receipt](docs/research/p5-ac1018-table-cell-appearance-dxf-write-readback-2026-11-08.json).
This is codec self-consistency only; it does not prove physical producer
ordering, native fill semantics, or TABLE support. Public `dwgRW::writeTable`
remains disabled until the physical TABLE profile, `*T` block/object graph
transaction, and independent readback gates below are satisfied.

Updated 2026-11-08 (AC1027 Cover source-reference audit now has authoritative no-body/handle profiles for 40/18 records plus source-matched LibreDWG no-body profiles for 29 more; TABLESTYLE and MLEADERSTYLE now have complete ODA body-handle field profiles, leaving only 3DSOLID partial, with no body reference profile left unknown. The three AC1027 default MATERIAL bodies now have source-matched no-body-handle and semantic profiles; the AC1027 SUN body has a peer no-body-handle profile and ODA-backed VPORT hard-owner edge; DICTIONARYWDFLT has an ODA §20.4.45 default hard-pointer profile. AC1027 DWG MATERIAL encoding remains guarded because retained modern body tails are not replayable; a bounded selected physical Cover MATERIAL-to-DXF ASCII/Binary route now passes local readback, without qualifying whole-document output or external semantics. See [current audit receipt](docs/research/p6-ac1027-cover-reference-guard-audit-refresh-2026-11-07.json) and [selected physical MATERIAL receipt](docs/research/p6-ac1027-material-physical-output-probe-2026-11-08.json). AC1032 multi-schdat DataStorage plus raw-only TextObjectContextData, PERSUBENTMGR, and TABLECONTENT shell recovery now reaches 842/842 published frames with zero entity/object parse failures; the bounded AC1032 MentalRay recovery separately reaches 182/182 frames with zero entity/object parse failures, while strict output remains refused at unrelated HEADER integrity rows ([receipt](docs/research/p6-ac1032-acad-table-cells-reader-fix-2026-11-07.json)); generated AC1032 TABLECONTENT detached-TV controls publish complete typed values, while the alternative body-byte-count control stays raw-only and verifies rollback, retry, destination preservation and following-frame recovery; AC1027/AC1032 TABLECONTENT and PERSUBENTMGR typed failures, plus one exact AC1027 osnap action-parameter failure, now retain exact frames raw-only after independent common-shell validation. The hash-matched `example_2013.dwg` now publishes 481/481 frames with zero entity/object parse failures, while strict output remains blocked at the AC1027 source-reference gate and no class-body/output claim is promoted ([TABLECONTENT](docs/research/p6-ac1027-tablecontent-raw-only-shell-recovery-2026-11-08.json), [PERSUBENTMGR](docs/research/p6-ac1027-persubentmgr-raw-only-shell-recovery-2026-11-08.json), [osnap action parameter](docs/research/p6-ac1027-osnap-action-param-raw-only-shell-recovery-2026-11-08.json)); the PERSUBENTMGR/TABLECONTENT source bodies remain disputed and raw-only; AutoCAD SAVEAS AC1021/24/27/32 samples now establish the same IMAGEDEF code-0xC parent and group-350 dictionary membership, and the independent wire observer captures that cross-version pattern as explicitly unqualified while strict output remains refused ([receipt](docs/research/p6-imagedef-cross-version-soft-role-evidence-2026-11-07.json)); an unlocked Aqua session was observed, but the Launch Services attempt for ODA 27.1 failed with `kLSNoExecutableErr`; the installed 27.1 app passes native verification outside the sandbox; the downloaded official ODA 27.10 ARM64 bundle still fails local code-signature verification although the 27.10 DMG's internal checksums pass, so 27.10 was not executed and no signature was modified ([package receipt](docs/research/p6-oda-27-10-official-arm64-package-signature-2026-11-07.json)); repeated direct 27.1 attempts, including the latest post-unlock retry, aborted during Pasteboard/XPC initialization before document processing. A materially changed GUI route then produced two same-version AC1027 resaves from scratch copies; both validated trailers end at byte 844 and both 739-byte suffixes remain unclaimed. The hashes differ, with no equal byte positions but matching low three bits at every position ([receipt](docs/research/p6-ac1027-oda-gui-same-version-resave-2026-11-07.json)). The read publishes 119/119 frames with zero entity/object parse failures, while strict DXF output remains blocked by the separate AC1027 source-reference gate. Do not repeat the unchanged direct launch method; continue local work and suffix analysis without promoting support claims; the R13 GROUP trace now binds the DICTIONARY issue to EED app `ACAD`, value `TREAT_AS_HARD`, raw ASCII bytes and per-value codepage `0`; ODA v5.4.1 does not define zero as file-codepage inheritance, so the source remains blocked without a production decoder change; the post-unlock ODA 27.1 GUI conversion emitted the ordered GROUP/dictionary relations but omitted that EED value and a referenced SHAPE; the AC1012/AC1014 DICTIONARY/GROUP common-link profile now passes bounded role/count controls and the source-bound R13 recheck, leaving EED codepage 0 as the sole GROUP source blocker ([receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json)); current strict `blocks_and_tables` diagnostic reaches 3,586/3,586 frames, and LibreDWG 0.14 repeats the unqualified FIELD child flag values without resolving their semantics, so strict FIELD/FIELDLIST output remains refused; ODA §2's detached-string rule conflicts with ACadSharp-derived TABLE value byte-count parsing; a debug-only alternate body-string parse of both AC1032 TABLECONTENT objects reaches the exact body and detached-string boundaries; the 17-bit interval is the encoded string-size word plus presence bit, and the final handle bit is byte-alignment padding under ODA §20.1; the candidate still conflicts with the TABLECONTENT schema and remains semantically incomplete, so both objects stay raw/refused with no production parser interpretation change; DWG 3DFACE output now rejects invisible-edge masks outside 0–15 and passes generated rollback/retry/readback across all six writer versions; R24 TABLESTYLE generated write/readback now includes AC1032 with same-handle recovery after both invalid-count and invalid-CMC refusal; the focused named-resource import selector now passes after synthetic receipts stopped authorizing strict-complete DWG output, with destination-preserving refusals and no support-claim change; AC1024 mechanical physical selected-output routes now cover 17 associative dependency/body pairs, 399 constraint nodes, and 17 variables across two networks and 13 value-dependency children in ASCII/Binary; the DXF `AcDbAssocEdgePersSubentId` codec slice is complete, and ten AC1024 civil Edge bodies decode to source-matched vertex-index cycles and their bounded selected ASCII/Binary output/readback passes; whole-source conversion and native meaning remain open ([read receipt](docs/research/p4-ac1024-civil-assoc-edge-dwg-read-2026-11-07.json), [selected-route receipt](docs/research/p4-ac1024-civil-assoc-selected-route-readiness-2026-11-07.json)); the viewport DWG debug trace now prints `centerPY` for its Y coordinate (diagnostic only; [receipt](docs/research/p6-viewport-debug-center-y-log-fix-2026-11-07.json)); a LibreCAD_3 persistence probe using raw include/library paths fails because the consumer compiles as C++14 while current upstream libdxfrw requires C++17, and this configuration bypasses libdxfrw's PUBLIC compile-feature propagation ([receipt](docs/research/p8-librecad3-cxx-standard-consumer-compat-2026-11-07.json)); AC1021 tablet now passes strict same-version output and fresh DXF readback after the WCS mesh extrusion default fix; generated WCS mesh round-trip controls pass across all six DWG writer releases ([receipt](docs/research/p4-ac1021-tablet-wcs-mesh-default-2026-11-07.json)); broader source graph closure remains open; saved-evidence TABLECONTENT inventory screening found no eligible physical witness and the overlapping edge masks stay refused ([receipt](docs/research/p5-tablecontent-existing-source-evidence-screen-2026-11-08.json))). This file

**Resume checkpoint (2026-11-08, latest desktop check):** after the user said
they would unlock the Mac, the desktop inventory still reported that the Mac
was locked. No native app surface was available, so this created no new ODA,
FreeCAD, or format evidence. Do not repeat the unchanged ODA CLI launch or
claim the external qualification gate passed. Continue local implementation.

**Latest TABLE implementation checkpoint (2026-11-09):** commits through
`ac561329` add generated AC1018 plain-text cell and table-scalar DWG codec
coverage: scalar mask `0x1f`, cell alignment/fill-state/indexed colors/text
style pointer/text height, and four physical edge color/lineweight/visibility
families. Readback checks typed fields, body boundary, following sentinel,
handle role/order, refusal, and rollback/retry. The focused
`libdxfrw_dwg_table_cell_overrides` CTest passes; no new DWG/DXF fixture was
added. This does not enable `dwgRW::writeTable`. The physical AC1018 TABLE
sources still stop at undocumented cell masks `0x40000`/`0x40020`. The matching
generated AC1018 DXF output mapping is now implemented for cell fill-state,
indexed colors, and uniquely resolved text-style handles/names; focused TABLE grid,
cell appearance, cell-edge, and legacy unknown-mask controls pass 4/4. See the [DXF mapping
receipt](docs/research/p5-ac1018-table-cell-appearance-dxf-write-readback-2026-11-08.json).
The private direct DWG codec now emits rectangular plain-text grids per ODA
v5.4.1 §20.4.96.1 repeat counts. A generated 2x2 vector checks row-major cell
order, widths, heights, values, per-cell text-style handle ordering, exact
body boundary, and following-sentinel alignment; parser readback publishes
the table row/column counts in the inherited fields. The focused CTest passes
in normal and ASan builds; no fixture was added. See the [multi-row
receipt](docs/research/p5-ac1018-table-multirow-plain-text-codec-2026-11-09.json)
and [rectangular-grid receipt](docs/research/p5-ac1018-table-multicolumn-plain-text-codec-2026-11-09.json).
The generated 2x2 grid's allocation sweep injects failure at all 25 measured
allocation points, verifies byte and bit-count rollback for both output buffers,
then retries the same object and buffers and compares both output streams exactly
with calibration. Normal and ASan focused CTests pass; see the [grid allocation
rollback receipt](docs/research/p5-ac1018-table-grid-allocation-rollback-2026-11-09.json).
The `*T` block/object graph transaction and independent source/native
qualification remain open. Keep public TABLE output refused. A fresh desktop
inventory after the user's unlock message still reports a locked Mac and no
native app surface. Do not repeat the unchanged ODA CLI launch. No synthetic
codec result promotes physical, native, or support claims.
The latest focused guard slice adds atomic refusals for each TABLE grid budget:
1,001 columns, 10,001 rows, and 201,000 cells with each dimension individually
within its limit. The fast TABLE CTest passes in normal and ASan builds; see
the [grid-budget receipt](docs/research/p5-ac1018-table-grid-budget-refusals-2026-11-09.json).

The P6 audit added generated AC1027 NOD → named DICTIONARY → typed SCALE and
NOD → named DICTIONARY → DATALINK graph allocation sweeps. SCALE passed 12
ASCII/12 Binary offsets; DATALINK with two custom references passed 28
ASCII/25 Binary offsets plus the focused ASan selector. Each preserves
destination and caller state, retries the same writer, and passes typed
readback ([SCALE receipt](docs/research/p6-dxf-scale-objects-allocation-failure-retry-2026-11-08.json),
[DATALINK receipt](docs/research/p6-dxf-datalink-objects-allocation-failure-retry-2026-11-08.json)).
The slices are committed as `6e8db601` and `c22619fe`. These close only the
selected typed writer profiles. A local generated FIELD/FIELDLIST allocation
prototype was rejected before commit: its DXF output reached the first FIELD
record, but `fileImport` refused during raw-carrier capture at the FIELD code-5
handle, before typed parsing or graph readback. The failure is not yet attributed
to a production defect; preserve the existing strict FIELD/FIELDLIST support
gate and do not use this synthetic graph to make claims. The post-unlock desktop
check still reports the Mac locked, so no new ODA/FreeCAD peer evidence is
available. A minimal generated AC1027 TABLECONTENT writer slice then swept 12
ASCII and 10 Binary callback allocation points. Every injected failure preserved
the destination and caller model; same-writer retries passed typed grid/value
readback. The receipt is [here](docs/research/p6-dxf-tablecontent-objects-allocation-failure-retry-2026-11-08.json).
The output's `m_semanticContentComplete` remains false for this minimal profile,
so this is transaction/typed-field evidence only and does not qualify strict
output or TABLECONTENT semantics. A first TABLEGEOMETRY allocation-probe
prototype was also rejected before commit because the generated minimal object
was refused during warm-up; it did not reach allocation injection or readback.
Do not infer a TABLEGEOMETRY writer defect from this incomplete setup. A broad
global-allocation sweep around the selected associative adapter export was also
rejected: it terminated on an uncaught `std::bad_alloc` during adapter preflight,
before a safe writer-callback boundary; the prototype was removed and no
production files changed. The narrowed low-level AC1024 OBJECTS callback has since been expanded to a
rooted graph with one NETWORK, two VARIABLE action targets, and one
VALUEDEPENDENCY pointing to the second variable. Its allocation sweep now
covers 40 ASCII and 37 Binary callback offsets with destination/source rollback,
same-writer retry, and typed readback of the root, owners, action targets,
reactor backlink, dependency target and cached int32 value ([receipt](docs/research/p6-dxf-associative-network-objects-allocation-failure-retry-2026-11-08.json)).
The generated variable records still read back with
`m_associativeVariableWireComplete == false`; these controls do not qualify a
complete variable source profile or support claim. Geometry dependencies,
constraint groups, parameter/body records, and broader associative semantics
remain open. Keep FIELD/FIELDLIST gated. Do not reopen swept families or promote
support claims. Each slice must update its receipt and checkpoint, commit
implementation with evidence, then report the commit and next ready slice.

**Implementation slice (2026-11-08):** a generated AC1024
`ACDBASSOCDEPENDENCY` OBJECTS callback now has allocation rollback coverage.
The fast CTest sweeps all 6 ASCII and 6 Binary callback allocation offsets;
each preserves the destination sentinel and caller object, then retries on the
same codec and confirms the dependency's typed fields by importing the DXF.
No DWG/DXF fixture was added and no production code or support claim changed
([receipt](docs/research/p6-dxf-associative-dependency-objects-allocation-failure-retry-2026-11-08.json)).
This is one base-dependency profile only; geometry/value dependency semantics,
graph closure, and source-producer prevalence remain open.

**Implementation follow-up (2026-11-08):** the same fast target now also
sweeps the AC1024 `ACDBASSOCVALUEDEPENDENCY` profile: 8/8 ASCII and 7/7 Binary
callback allocations, with destination/source rollback, same-codec retry, and
typed value-name/real-value readback. The base dependency remains covered at
6/6 offsets per encoding. The combined [receipt](docs/research/p6-dxf-associative-dependency-objects-allocation-failure-retry-2026-11-08.json)
limits this evidence to those generated record profiles; integer and geometry
dependencies, graph closure, source prevalence, and support claims remain
open.

**Implementation follow-up (2026-11-08):** the dependency allocation target
now also covers generated AC1024 `ACDBASSOCGEOMDEPENDENCY` with the default
`AcDbAssocSingleEdgePersSubentId` profile. It sweeps 8/8 ASCII and 7/7 Binary
callback allocations and proves destination/source rollback, same-codec retry,
and typed field readback. Its dependency references and subentity indices are
null/default, so this is not a source-backed graph or geometry semantic claim.
The [combined receipt](docs/research/p6-dxf-associative-dependency-objects-allocation-failure-retry-2026-11-08.json)
records the base, real-valued, and default-index geometry profiles. Next, use
the admitted AC1024 source profile to add non-null dependency reference roles
inside a rooted graph and retain strict graph-closure gates.

**Implementation follow-up (2026-11-08):** that graph step is complete for
`ACDBASSOCVALUEDEPENDENCY`: the existing allocation target now verifies
NOD → DICTIONARY → NETWORK → two VARIABLE action targets, plus the dependency's
owner, non-null target, and reciprocal target reactor. It passes 40/40 ASCII
and 37/37 Binary allocation failures with same-writer retry/readback. This
still leaves the two locally-authored variables' wire-completeness flag false
and is not independent source or semantic evidence.

**Implementation follow-up (2026-11-08):** a second focused target now sweeps
a rooted AC1024 NETWORK → constraint GROUP → geometry dependency → LINE graph.
It verifies the group's action and inline-node references, the dependency's
owner/LINE target and `AcDbAssocEdgePersSubentId` indices, and the reciprocal
LINE reactor. All 52 ASCII and 50 Binary callback allocation offsets preserve
the destination and source graph, then pass same-adapter retry and typed
readback. This is generated graph/transaction evidence, not native constraint
or geometry qualification ([receipt](docs/research/p6-dxf-associative-geometry-graph-allocation-failure-retry-2026-11-08.json)).
**Implementation follow-up (2026-11-08):** the standalone paired-record slice
now emits an `ACDBASSOCDEPENDENCY` with a non-null body handle together with its
`ASSOCDIMDEPENDENCYBODY`. The fast allocation selector passes 12/12 ASCII and
12/12 Binary callback offsets for the pair; base dependency-only fields,
version-1 body fields, the body-to-dependency owner link, and
dependency-to-body handle all pass typed readback. Failures preserve the output
sentinel and both caller records, then same-codec retry succeeds. Value and
geometry profiles remain covered at 8/7 offsets in ASCII/Binary. See the
[combined receipt](docs/research/p6-dxf-associative-dependency-objects-allocation-failure-retry-2026-11-08.json).
This proves generated transport and transaction behavior only. Do not attach a
dimension body to `ACDBASSOCGEOMDEPENDENCY`: its admitted geometry profile
requires a null body handle. A separate rooted dimension-constraint graph
still needs an admitted node profile, actual dimension entity,
value-dependency edge, group membership, reactor closure, and body ownership
verified together. Keep the generated pair separate from source prevalence,
native associative meaning, and support claims.

**Implementation follow-up (2026-11-08):** the same focused target now covers
the admitted AC1024 signed-int32 `ACDBASSOCVALUEDEPENDENCY` variant (group 90,
value 17), in addition to the real-valued group-40 profile. Integer-value
allocation rollback passes 8/8 ASCII and 7/7 Binary callback offsets with
destination/source preservation, same-codec retry, and typed readback. The
[combined receipt](docs/research/p6-dxf-associative-dependency-objects-allocation-failure-retry-2026-11-08.json)
records this generated record profile only; it does not add a value target or
rooted dimension graph.

**Implementation checkpoint (2026-11-08):** an initial attempt to place the
dimension body under the geometry dependency was rejected by the existing
profile validator because geometry dependencies require a null body handle.
The invalid prototype was removed; no production change or evidence was
committed. The user said they were unlocking the Mac, but the latest desktop
inventory still reports the Mac locked, so ODA/FreeCAD qualification remains
pending. Verify a fresh desktop inventory before retrying the native workflow;
do not treat the prior PasteBoard/XPC abort as format evidence. Continue local
implementation meanwhile. After the user's latest message that the session
would be unlocked, a fresh desktop inventory again found the Mac locked and
no native app surface available; this is an access blocker only, not DWG/DXF
evidence. Retry ODA/FreeCAD qualification only after a later inventory exposes
an unlocked session. A separate locally authored full-file dimension
graph was also refused by strict adapter preflight because it lacks the
source-bound AC1024 group-owned dimension profile. Its temporary prototype was
removed; neither output policy nor source gates were changed.

**P6 rooted dimension-constraint generated profile — local transaction gate complete; broader qualification remains open.** The hash-bound AC1024 mechanical profile closes the 17 dimension-dependency/body pairs, 399 constraint nodes, distinct value-dependency and associative-variable targets, ASCII/Binary readback, late-mutation refusal, destination rollback, and same-adapter retry. The focused receipt also covers a post-OBJECTS-callback allocation failure. This is selected graph transport evidence only; it does not establish whole-file output or native associative meaning. The earlier locally authored graph refusal was correct and remains separate; do not relax source preflight. The saved P4 census has no additional ready physical object graph. Continue with a distinct evidence-backed P5/P6 action only when its source/schema boundary is actionable. See the [node dependency closure receipt](docs/research/p6-ac1024-mechanical-dimension-node-dependency-closure-2026-11-08.json).

**Resume checkpoint (2026-11-07):** the Mac session was unlocked and the
audit-enabled ODA 27.1 GUI route has completed the AC1024 civil residual
attempt, the four-variant AC1021 writer matrix, and an AC1012 R13 GROUP
conversion to 2010 ASCII DXF. The matrix result, paired
layer-reference assertion, limits, and receipts are recorded below; no more
authorization or identical ODA launch/retry is required. The AC1021 writer
probe is complete and must not be repeated. Continue with the first P4
OBJECTS profile already present in the saved census/inventory that has closed
source identity, ownership, dependencies, and byte/frame disposition. If the
first candidate fails any evidence gate, record that precise gate once and
advance to the next inventory row; do not restart a corpus scan or weaken
strict output. The implementation slice, focused control, plan/receipt update,
commit, and concise progress report form one uninterrupted loop.

**Resume checkpoint (2026-11-08):** the one planned ODA 27.1 CLI retry after
the user reported unlocking the Mac again aborted in 0.288 seconds during
Pasteboard/XPC initialization. It produced no stdout or output file; the source
and converter hashes match the prior receipts. The direct CLI invocation is
exhausted unless a concrete executable or launch-environment change occurs.
Do not repeat it. See the [retry receipt](docs/research/p6-ac1027-oda-cli-after-user-unlock-2026-11-08.json).
This result does not explain suffix contents: keep the suffix unclaimed and
strict DXF output refused at the separate source-reference gate. Continue
independent local implementation.

**Latest implementation slice (2026-11-08):** the typed primitive DXF writer
now admits a closed TORUS profile alongside the existing BOX profile. TORUS
requires an exact one-node/no-edge graph, a null material reference, complete
prefixes, two positive finite radii, and a spec-conforming CMC/index profile;
the same-profile DXF re-import closure validates the 34-group TORUS body and
null group-347 link. Generated ASCII/Binary readback, invalid-material refusal
with destination preservation and same-adapter retry, ODA-shaped ACI CMC
controls, and the existing BOX CMC loss guard pass. The current parser also
qualifies the hash-bound public AC1021 path
`0x452 → 0x453 → 0x454 → 0x455`, including radii, ACI 2, CMC raw index 0 and
null material; LibreDWG 0.14 emits the matching TORUS DXF record. This is a
selected graph/profile result only: the containing source's unrelated ACDS and
opaque frames still block whole-source output, and independent ACIS/kernel
meaning remains open. See the [TORUS receipt](docs/research/p4-ac1021-acsh-torus-typed-dxf-projection-2026-11-08.json).
**ODA converter retry result (2026-11-07):** after the user unlocked the
macOS session, ODA File Converter 27.1.0 completed an audit-enabled native GUI
conversion of a scratch copy of the AC1024 civil source to 2010 ASCII DXF. Its
handle `0xC55` is emitted as `ACAD_PROXY_OBJECT`, with no typed persistent-
subentity fields; this route does not explain the 24 residual source bits or
qualify associative semantics. Do not repeat the same conversion without a
discriminating source change. The AC1027 suffix remains unclaimed and strict
output remains refused. This external observation does not block the local P4
implementation queue; see the [current residual receipt](docs/research/p4-ac1024-civil-assoc-current-one-unresolved-2026-11-07.json).

**AC1021 writer qualification probe (2026-11-07):** four generated variants
were read by LibreDWG 0.14 and processed in one audit-enabled ODA 27.1 GUI
pass. The base, plotstyle-only, and material-only variants all fail at layer
record `0x12`; only the variant with both explicit default graph opt-ins
produces a 2010 ASCII DXF without an error report. The combined source layer
references plotstyle `0x31` and material `0x35`. This isolates the paired-graph
condition for this exact locally authored profile, not general DWG validity.
The existing fast material and plotstyle opt-in tests pass (2/2), and the
material test now reads back both layer-0 references against their planned
targets for AC1021/24/27; its focused rerun passes. ODA v5.4.1 §20.4.54
documents the plotstyle hard pointer and adds the material
handle for R2007+; no upstream production default changed. Source inspection
found LibreCAD's writer invokes `m_dwgW->write(this, dwgVer, false)` and its
current adapter does not implement either new upstream opt-in provider. The
consumer is still linked to its bundled fork, so this is a migration task, not
a current-output failure finding. P8 must adapt the writer caller and establish
the opaque-reference predicate before claiming the upstream migration. Scratch
DWG/DXF/JSON files remain outside the repository. See the [probe receipt](docs/research/p6-ac1021-minimal-writer-oda-gui-rejection-2026-11-07.json).

**Current P8 verification (2026-11-07):** the current `c33edaa4` libdxfrw
revision was installed into `/private/tmp/libdxfrw-install-current`, then a
fresh out-of-tree LibreCAD_3 configure selected its exported CONFIG package.
The persistence target built and its compile flags contain `-std=gnu++17`.
The same persistence library passed all three existing `DxfExportTest.*`
cases (raw file inspection for entity export, table/header fidelity, and Z/text
attributes) and a scratch LINE/CIRCLE/ARC R2000 DXF save/open/reopen probe with
field readback. The exact archive, consumer library and runner hashes,
commands, and limits are in
the [current-head build receipt](docs/research/p8-librecad3-current-head-package-build-2026-11-07.json).
This refreshes selected package/export/2D persistence integration evidence
only: the LibreCAD_3 checkout is dirty, and no full application, ABI, Linux,
or Windows qualification was run in this pass. The umbrella
`lcunittest` target still fails while compiling Kaguya member-function
`noexcept` signatures with AppleClang 21; its failure is outside this library.
Do not repeat this unchanged package build or focused export cohort.

contains open work only. Implementation history and completed-item evidence belong in Git and
`docs/research/`, not in this queue.

**Latest implementation slice (2026-11-07):** the exact AC1015/AC1018
`CELLSTYLEMAP` class/version profiles now preserve a failed typed record as
raw-only after common-shell validation. The local AC1015 `PolyLine3D_2.dwg`
witness advances from 228/229 to 229/229 published frames; strict output still
refuses because the out-of-version object is unsupported and has no qualified
serializer. The focused version/class guard passes. No body semantics, output
eligibility or support claim changed. See the [recovery receipt](docs/research/p4-ac1015-cellstylemap-raw-only-recovery-2026-11-07.json).

The local AC1018 `HatchG.dwg`
CellStyleMap typed parse failure now falls back to exact raw-only preservation
when a forked `RawObjectShell` validates its common header and handle data. The
read publishes all 229 observed frames with zero failed frames. The profile is
restricted to the exact AC1018 `CELLSTYLEMAP` / `AcDbCellStyleMap` class pair;
no typed values, output support, or parity claim are inferred. Strict DXF
preflight still refuses on the independent IMAGEDEF/reactor source-correlation
gate. The generated CellStyleMap encoder/readback selector passes. See the
[AC1018 raw-only receipt](docs/research/p6-ac1018-cellstylemap-raw-only-recovery-2026-11-07.json).

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

### Current execution checkpoint (2026-11-10)

- **DIMASSOC DWG encoder transaction safety:** `DRW_DimensionAssociation::encodeDwg`
  now stages its body, detached strings and handle-occurrence ledger, preserving
  stream aliasing and publishing only after all staged writes succeed. A
  generated AC1024 `AcDbOsnapPointRef` control injects a one-shot allocation
  failure at all 85 reached offsets across separate streams, shared
  string/handle streams and all-streams-aliased mode. Every failure preserves
  caller bytes, cursor and handle ledger; retrying the same association and
  buffers matches clean output. The two focused DIMASSOC CTests pass. This is
  codec-level transaction evidence only; other record encoders, OBJECTS
  builders and whole-drawing failure paths remain open. No DWG/DXF fixture was
  added. See the [allocation rollback receipt](docs/research/p6-dimassoc-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **EvaluationGraph DWG encoder transaction safety:**
  `DRW_EvaluationGraph::encodeDwg` now stages its body and handle ledger and
  publishes them together only on success. Generated AC1021 controls sweep all
  14 reached allocation offsets with separate body/handle streams and all 10
  offsets when those streams alias; every injected failure preserves caller
  state and same-buffer retry matches clean bytes and handle occurrences. The
  combined fast allocation test and focused object-vector/readback tests pass.
  This closes only this record encoder's local transaction path. Other DWG
  encoders, OBJECTS builders and whole-drawing failure paths remain open; no
  DWG/DXF fixture was added. See the [allocation rollback receipt](docs/research/p6-evaluation-graph-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **BlockRepresentationData DWG encoder transaction safety:**
  `DRW_BlockRepresentationData::encodeDwg` now stages the body and effective
  handle stream, retaining its version-dependent inline-handle behavior and
  existing hard-owner role. Generated AC1024 controls sweep eight separate
  stream and seven body-aliased allocation offsets; failure preserves all
  caller streams and same-record retry matches clean bytes and handle roles.
  The focused allocation and object-vector tests pass. This does not resolve
  the record's opaque binary payload or qualify its producer semantics. No
  DWG/DXF fixture was added. See the [allocation rollback receipt](docs/research/p6-block-representation-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **IMAGEDEF_REACTOR DWG encoder transaction safety:**
  `DRW_ImageDefinitionReactor::encodeDwg` now stages its body and effective
  handle stream while retaining the version-specific inline/detached handle
  layout. Generated controls sweep seven AC1012 inline-handle offsets, nine
  AC1024 separate-stream offsets and seven AC1024 body-aliased offsets; every
  injected failure preserves caller state and same-record retry matches clean
  output. The focused allocation, object-vector and DIMASSOC readback tests
  pass. This does not resolve the IMAGEDEF parent-role conflict or qualify
  output. No DWG/DXF fixture was added. See the [allocation rollback
  receipt](docs/research/p6-imagedef-reactor-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **IMAGEDEF DWG compound-writer transaction safety:**
  `DRW_ImageDef::encodeDwg` now stages its effective body, detached string and
  handle destinations before publishing, retaining the AC1012 inline layout,
  AC1018 detached-handle layout and AC1021 stream alias combinations. The
  generated control sweeps 127 reached allocation offsets across seven
  version/alias cases; failures preserve caller streams and corrected same-
  object retries match clean output. The image compound transaction, object
  vector, allocation retry and DIMASSOC readback tests pass 4/4. This does not
  resolve the IMAGEDEF parent-role conflict or authorize image output. No
  DWG/DXF fixture was added. See the [allocation rollback receipt](docs/research/p6-imagedef-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **PLOTSETTINGS DWG encoder transaction safety:**
  `DRW_PlotSettings::encodeDwg` now stages the body, detached text and handles
  while preserving the existing revision-specific field layout and plot-view/
  shade handle tail. Generated AC1012, AC1018 and AC1021 controls cover seven
  stream arrangements and sweep 217 allocation offsets with rollback and
  same-object retry. The focused PlotSettings DWG decode/readback, DXF OBJECTS
  allocation, object-vector and combined DWG encoder tests pass 4/4. This is
  local codec/transaction evidence, not native plotting qualification. No
  fixture was added. See the [allocation rollback receipt](docs/research/p6-plotsettings-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **SCALE DWG encoder transaction safety:** `DRW_Scale::encodeDwg` now stages
  its body and effective text destination while preserving AC1012 inline and
  AC1021 detached-string behavior. Generated controls sweep 30 allocation
  offsets across inline, detached and explicit body-alias cases, with caller
  rollback and same-object retry. The focused SCALE/PlotSettings graph,
  object-vector and combined allocation tests pass 3/3. This does not close
  general SCALE graph or producer semantics. No fixture was added. See the
  [allocation rollback receipt](docs/research/p6-scale-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **GROUP DWG encoder transaction safety:** `DRW_Group::encodeDwg` now stages
  its body, description and handles while retaining the existing versioned
  stream boundaries and member order. Generated AC1012/AC1018/AC1021 controls
  sweep 121 allocation offsets over seven alias/layout cases, checking rollback
  and same-object retry. The focused Group encoder vector and allocation tests
  pass. The GROUP dictionary and metadata CTests now pass after correcting
  their AC1027 expectation: explicit partial mode must still preserve atomic
  refusal at the unqualified source-reference gate. This verifies the gate
  without bypassing it; AC1027 GROUP output qualification remains open, as do
  the broader P4 source and graph closures. No fixture was added. See the
  [transaction and gate receipt](docs/research/p6-group-dwg-encoder-allocation-rollback-retry-2026-11-10.json).

- **AC1024 AutoCAD 2016 MATERIAL replay:** the writer admits only the
  exact 1284-bit default profile from the public NextGIS AC1024 sample and
  remaps its three MATERIALs, three extension dictionaries and 18 tile
  XRECORDs. Fresh libdxfrw readback and LibreDWG 0.14 peer comparison verify
  semantic fields and graph references. The same source exposed and fixed
  detached-string bit-padding in the shared R2007+ stream writer. The focused
  MATERIAL CTest passes 1/1. An injected first-allocation failure preserves
  the existing destination, and the same writer retries successfully with
  fresh readback. The sample/output stay in `/private/tmp`; this is
  not whole-source or broad AC1024 qualification. See the [profile receipt](docs/research/p6-ac1024-autocad2016-material-replay-2026-11-10.json).

- **Clean LibreCAD_3 master compatibility patch:** a fresh archive of
  `origin/master` (`5cf435f4a5a57d8dba1599c720917b902e74b25b`) was populated
  with its pinned submodules, then its bundled libdxfrw submodule was replaced
  in `/private/tmp` by this repository's `045a3892` source. The clean
  persistence target configured, built, and linked. The focused export and
  round-trip cohort initially passed 48/64 tests: LibreCAD_3's `writeHeader`
  supplied `$INSUNITS` even for R12/R14, which the library correctly refuses
  because those revisions cannot represent it. A version-aware adapter change
  (emit `$INSUNITS` only after AC1014, and report explicit insertion-unit loss
  for legacy export) raised the cohort to 61/64. The remaining three tests
  shared a malformed fixture: its first OBJECTS `DICTIONARY` had a non-null
  owner, violating the root-dictionary rule. Correcting that owner in the two
  existing LibreCAD_3 fixtures brought the cohort to 64/64. The consumer
  patch is committed on `/private/tmp/librecad3-p8`, branch
  `codex/libdxfrw-upstream-compat`, with code commits `36082e71`, `93354846`,
  and `762a0f1e`, plus plan-sync follow-up `8269dd7b`; it is proposed in
  [LibreCAD_3 PR #456](https://github.com/LibreCAD/LibreCAD_3/pull/456).
  The build command `cmake --build /private/tmp/librecad3-p8/build --target
  persistence lcunittest -j4` passed; the focused
  `lcunittest --gtest_filter='DxfExportTest.*:DxfRoundTripTest.*'` run passes
  64/64 on the latest head. No new DWG/DXF fixture was created; only two
  existing LibreCAD_3 DXF fixtures were corrected. The unit regression covers
  unitless and three nonzero profiles for both R12 and R14. Preserve libdxfrw's
  unsupported version refusal and root-dictionary validation. Receipt:
  [clean master compatibility probe](docs/research/p8-librecad3-origin-master-clean-consumer-2026-11-08.json).
  Follow-up commit `93354846` extended the drawing-unit regression to both R12
  and R14. Commit `762a0f1e` adds unitless input, requiring no loss note, while
  retaining the exact one-note rule for each nonzero unit profile. The focused
  build and targeted test pass 1/1, verifying unit-loss reporting,
  `$INSUNITS` omission, and LINE preservation in both legacy outputs. No
  fixture was added. See the [R14
  regression receipt](docs/research/p8-librecad3-legacy-units-r14-regression-2026-11-08.json).
  The full focused consumer export/round-trip cohort was rerun at current HEAD
  after adding unitless input coverage and passed 64/64. The targeted unit case
  now checks zero loss notes for unitless input and exactly one insertion-unit
  loss note for each nonzero profile, while both legacy outputs retain LINE
  and omit `$INSUNITS`. The patch is pushed and proposed as
  [LibreCAD_3 PR #456](https://github.com/LibreCAD/LibreCAD_3/pull/456), based
  on the exact `origin/master` commit above. The code head remains `762a0f1e`;
  the PR branch also contains `fd5af0b8` (Windows smoke timeout fix plus plan
  receipt) and `321f2cad` (19/19 local package-smoke receipt). On current head
  `321f2cad`, `BuildWindows` and
`BuildWindowsPersistence` are in progress and the other required hosted jobs
are queued; the preceding `fd5af0b8` Plan Sync passed. The consumer-plan
`--full`, explicit candidate paths, actual-base check and 31 checker tests pass
locally. No hosted Windows result is yet available. Continue reviewing current-
head CI, then qualify native Linux/Windows FreeCAD and LibreCAD/LibreCAD_3
behavior independently. Local macOS checks do not satisfy those platform gates.
- **Current LibreCAD_3 PR #456 hosted rerun:** the remote head remains
  `7e81275b71038d4a42fa580842e661ddcee7adbf`, based on the recorded exact
  `origin/master` commit. Builder, CodeQL, and Python Binding Plan Sync pass.
  The Python-binding macOS workflow fails on both architectures. Arm64 fails
  installation because Homebrew's regenerated `runtime_dependencies` receipts
  disagree with the selected `zstd/1.5.7_1` and `lz4/1.11.0` kegs; x86_64 has
  no required CMake bottle under `--force-bottle`, and source-build fallback is
  unqualified. In the isolated `/private/tmp/librecad3-p8` checkout, commits
  `8412c5e0`, `1a012ff6`, `7e81275b`, and `83f36931` remove dependency-origin
  inference from mutable package receipts, resolve actual Mach-O load paths,
  and retain per-library architecture/original-digest validation. Local checks
  pass: 42 native-origin tests, 8 dependency-install tests, full plan
  validation, 31 plan-checker tests, actual `origin/master` base validation,
  worktree gate audit, and `git diff --check`. These are local results; the
  candidate has no hosted rerun and is not the remote PR head. Automatic review
  rejected uploading the candidate plan file through the alternate route, so
  do not publish this candidate through another route. Keep both Mac gates
  open; decide the x86_64 provider explicitly and do not accept source-build
  fallback. See the [hosted CI recheck receipt](docs/research/p8-librecad3-pr456-hosted-ci-recheck-2026-11-09.json).
- **Desktop qualification recheck:** after the latest user reply that they
  would unlock the Mac, a fresh desktop inventory still reports the native
  session locked. The ODA File Converter launch therefore remains unexecuted
  against any drawing; its earlier PasteBoard/XPC abort is not format evidence.
  Do not retry the same CLI launch or mark FreeCAD/ODA qualification complete
  until a fresh desktop inventory exposes an unlocked native app surface.
  Continue independently actionable repository work and keep platform
  qualification as a separate external gate. No application qualification was
  run in this recheck.
- **P5 VIEWPORT DXF domain guard:** strict typed output now admits the
  Autodesk-documented group-68 status values (`-1`, `0`, positive) and group-170
  shade-plot values (`0`–`3`). The generated focused test passes in ASCII and
  Binary, including tolerant field parsing, destination-preserving refusal,
  same-writer correction/retry, and readback for valid statuses and shade-plot
  modes. No DWG/DXF fixture was added. The DWG wire-width checks are unchanged;
  viewport group 91 remains unresolved and output-refused pending a qualified
  native witness. See the [domain receipt](docs/research/p5-viewport-status-dxf-domain-2026-11-09.json).
- **Latest ACIS writer hardening:** commit `e3111a82` preserves binary64
  precision in SAT v1 output using `max_digits10`. Follow-up commit
  `a3d58f7f` adds nonfinite header-scalar refusal, `dce17422` adds a
  nonfinite torus-vector control, and `fc8929f0` verifies that an extra
  torus-surface Boolean is refused. All failure paths preserve the preexisting
  SAT output buffer. Commit `44781903` catches SAT assembly allocation failures;
  its sweep covers all 11 observed allocation points and retries the same
  output buffer after every injected failure. The generated raw SAB decoder
  control now sweeps 26 allocation points on a nonempty v21800 point→vertex
  graph with heap-backed header strings. It sweeps allocation failures across
  raw SAB parsing (14 points), graph building (9), wireframe decoding (26), and
  header-returning decoding (26), verifies each API's cleared output and retry,
  and checks vertex coordinates/bounds plus parsed tokens/header fields (test
  commit `1eb10dcc`). A heap-backed header copy fault exposed partial caller-
  header publication; commit `304f6fc7` changes final publication to a
  statically verified nonthrowing move. The expanded control passes for each
  stage (test/receipt commit `03bb9717`). The
  pinned independent parser
  matches the physical source SAB
  fields exactly. Native B-rep and independent-kernel gates remain open; see
  the [P7 parser receipt](docs/research/p7-ac1021-inventorloader-sat-parser-crosscheck-2026-11-08.json).
- **Latest local slice:** the AC1027 `AcDbAssocOsnapPointRefActionParam`
  frame at `0x393` now publishes raw-only after independent common-shell and
  handle-stream validation. A hash-matched physical read publishes 481/481
  frames with zero entity/object parse failures; strict output remains refused
  at the source-reference gate. The generated invalid-body/shell-valid control
  and prior recovery controls pass `libdxfrw_persistent_manager_prefix_dispatch`
  (1/1). See the [action-parameter receipt](docs/research/p6-ac1027-osnap-action-param-raw-only-shell-recovery-2026-11-08.json).
- **AC1018 TABLE writer checkpoint:** commit `1a590aed` adds a generated
  low-level control that emits a real TABLE entity frame with writer-local
  `AcDbTable` class identity/count bookkeeping, then rolls the frame and class
  ledger back through an outer compound checkpoint. The placeholder `*T`
  block is empty; this proves only frame/class transaction mechanics. The
  commit `c0cb07f3` extends the focused control to route class registration
  and frame emission through an explicit `dwgRW` transaction: caller class
  state, frame bytes, handle/class ledgers and success count roll back after a
  failure injected following an earlier `*T` owned geometry frame; one retry
  then commits on the same facade. The generated LINEs are transaction probes
  only, not TABLE rendering evidence. Next build the smallest semantically complete
  `*T` geometry/reference graph from a bounded, source-supported profile. Keep
  `dwgRW::writeTable` fail-closed until that graph and its
  physical-source, independent-reader/native qualification gates pass. See the
  [TABLE readiness receipt](docs/research/p5-dwg-table-encoder-readiness-2026-11-08.json).
- **AC1018 TABLE source graph screen:** the local AutoCAD 2004 sample is
  independently decoded by LibreDWG 0.14: TABLE handle `0x4F2` points to
  BLOCK_RECORD `0x4F3` named `*T`, whose ordered owned-handle list resolves to
  39 entities (32 LINE, 7 MTEXT); each decoded child points back to `0x4F3`.
  This establishes one physical graph shape only. Both libdxfrw and LibreDWG
  retain/stop at unknown TABLE body data (libdxfrw flags `0x40000`; LibreDWG
  retains 12,284 unknown bits and no decoded row/column widths), so these
  entities cannot be matched to typed cell semantics or used to derive the
  generated one-cell profile. The next graph implementation needs a complete
  bounded source/producer profile; keep public TABLE output refused meanwhile.
  ACadSharp's current local writer explicitly rejects TABLE before R2010;
  LibreDWG can emit AC1018 from DXF, but its secondary export of this source
  already loses a reliable cell-to-cache mapping, so that round trip is only a
  mechanical probe. Neither writer supplies the missing legacy semantic
  witness. A separate from-scratch one-cell ACAD_TABLE DXF was passed through
  LibreDWG 0.14 `dwgwrite --as r2004`; although the command printed `SUCCESS`,
  its diagnostics say `Object TABLE unsupported` and `No class for TABLE type
  721 found`. `dwgread` confirms the result contains zero TABLE frames/entities
  and zero custom classes. Exclude this route as a physical profile source;
  see the [LibreDWG writer screen](docs/research/p5-ac1018-libredwg-table-writer-screen-2026-11-08.json).
  The full source/output fingerprints and limits are in the [TABLE readiness
  receipt](docs/research/p5-dwg-table-encoder-readiness-2026-11-08.json).
- **AC1018 ODA TABLE peer profile (2026-11-09):** ODA File Converter 27.1
  converted the pinned ezdxf `acad_table_simple.dxf` (AC1027) into an AC1018
  DWG in scratch storage. libdxfrw reads its TABLE `0x9c` with complete
  3-by-3 text semantics, three 152-unit columns, row heights 11/9/9, and the
  values `Title`, `1`–`6`. ACadSharp independently agrees on those values,
  dimensions, the `*T1` block name, and its 15 children (7 MTEXT, 8 LINE),
  though it reports the cells' content type as `Unknown`. The physical cell
  profile has a merged title span and eight retained bounded override ranges.
  The direct codec handles the merge-role shape and now emits the observed
  zero-mask override record for populated text cells, but still refuses parsed
  source subrecord ranges. The TABLE's 15 block children point back to
  BLOCK_RECORD `0x9d`,
  and libdxfrw's entry graph resolves through `*T1`. This one peer-converted
  drawing also has an unrelated HEADER integrity warning, so it cannot qualify
  strict whole-source output. **Superseded for the bounded generated AC1018
  profile:** `dwgRW::writeTable` now emits the exact generated plain-text cell
  and `*T1` cache graph through whole-file finalization, with fresh libdxfrw
  readback and independent LibreDWG graph/cached-geometry checks. The peer
  leaves the TABLE cell body opaque, so native acceptance/rendering and
  independent cell-semantic decoding remain open. Broader source profiles
  remain gated. The parser records seven complete 10-bit cell override spans (zero override and
  virtual-edge flags) and one complete 4-bit TABLE override span (zero enabled
  masks). Instrumented ACadSharp cursor positions independently reproduce all
  eight spans under a constant 35,888-bit cursor-origin bias; these are bits in
  decompressed object bodies, not DWG file offsets. Next complete the compound
  `*T` graph transaction and fresh independent readback. This one ODA-converted
  drawing still does not establish native AutoCAD acceptance or geometry
  generation. No drawing fixture was added. Full hashes, field values,
  provenance, and limitations are in the
  [ODA TABLE peer receipt](docs/research/p5-ac1018-oda-table-physical-peer-profile-2026-11-09.json).
- **AC1018 merged-cell codec slice completed (2026-11-09):** the direct body
  encoder now writes the cell merge flag and width/height fields for the
  bounded legacy representation: a non-merged anchor carries its span, and
  covered cells carry `merged=true` with unit spans. A shared bounded layout
  validator rejects orphaned, overlapping, and out-of-range roles in the
  encoder and marks malformed parsed AC1018 tables semantically incomplete
  while retaining their frame. Generated vectors cover a 2x2 merge,
  exact readback and following sentinel, atomic malformed-profile refusal, and
  same-object retry; the focused `--table-cell-overrides` selector passes.
  The allocation/retry control still passes all 26 offsets. Re-reading the ODA
  peer sample against the rebuilt library retains its complete 3x1 title merge.
  This does not admit its eight parsed subrecord ranges to the direct encoder,
  generate the `*T` block, or enable `dwgRW::writeTable`. Keep source-output and
  compound graph gates open; see the [peer/implementation receipt](docs/research/p5-ac1018-oda-table-physical-peer-profile-2026-11-09.json).
- **AC1018 zero-mask override slice completed (2026-11-09):** instrumented
  ACadSharp independently reproduces the seven cell-override spans and the
  four TABLE mask bits from the ODA-converted peer. All cell override masks and
  virtual-edge flags are zero; the four TABLE mask-presence bits are false.
  The direct generated codec now emits the 10-bit zero-mask override record for
  populated text cells and the 4-bit all-absent TABLE mask tail. Generated
  merge/readback controls assert both spans, exact cursor boundary and sentinel;
  the focused object-vector and 26-point allocation/retry controls pass. This
  still proves only the bounded body profile; retained source ranges remain
  refused, and compound `*T` graph output is not enabled. See the peer receipt.
- **Previous local slice:** AC1027/AC1032 PERSUBENTMGR typed failures now
  publish raw-only after independent common-shell and handle-stream validation.
  The hash-matched AC1027 physical source publishes handle `0x2E0` and reduces
  object parse failures from two to one; strict output remains refused at the
  source-reference gate. See the
  [PERSUBENTMGR receipt](docs/research/p6-ac1027-persubentmgr-raw-only-shell-recovery-2026-11-08.json).
- **Previous local slice:** commit `4f615637` adds AC1027 TABLECONTENT raw-only
  recovery, gated to AC1027/AC1032 `TABLECONTENT` / `AcDbTableContent` records
  whose typed parse fails but whose independent common-shell and handle-stream
  parse succeeds. The generated control preserves exact frame bytes and common
  links, publishes no typed TABLECONTENT, and recovers the following frame.
  The hash-matched physical source publishes object `0xC94` raw-only; one object
  parse failure remained after that slice. See the
  [TABLECONTENT recovery receipt](docs/research/p6-ac1027-tablecontent-raw-only-shell-recovery-2026-11-08.json).
  Plan-only commit `21bb2ebc` records the saved source-evidence screen and its
  limits; it does not infer that uninspected local samples contain no such
  objects.
- **Earlier local slice:** commit `05720365` adds bounded R24 TABLESTYLE CMC
  truecolor/color-book DXF group-62/420/430 output and parser recovery for
  background, content and border colors. Generated ASCII/Binary controls
  verify raw fields, typed CMC readback, invalid-RGB refusal, destination
  preservation and corrected same-adapter retry. This is codec consistency
  only: independent producer behavior, native color-book meaning and broader
  document closure remain unqualified. See the [CMC receipt](docs/research/p6-r24-tablestyle-cmc-truecolor-book-2026-11-08.json).
  Follow-up commits `360e8470` and `d12fdb5c` added loss guards to both
  writers. Commit `eeb864b5` refines them against the DXF wire: [Autodesk
  defines groups 420/430 as truecolor/name fields](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-3610039E-27D1-4E23-B6D3-7E60B22BB5BD.htm)
  and DXF has no CMC method byte, so representable method-zero RGB/book colors
  now emit and normalize to
  C2 on typed readback. Book/name without RGB remains refused. ASCII/Binary
  controls verify normalization, destination-preserving refusal and
  same-adapter retry; the [TABLESTYLE](docs/research/p6-r24-tablestyle-cmc-truecolor-book-2026-11-08.json)
  and [CELLSTYLEMAP](docs/research/p6-cellstylemap-cmc-loss-guard-2026-11-08.json)
  receipts record the corrected behavior. Color-name allocation failure stays
  contained at the CELLSTYLEMAP record boundary.
  Commit `e19a6170` admits per-style R24 TABLESTYLE
  merge flags within ODA's `0x18000` mask. Generated ASCII/Binary controls
  verify raw group-92 order and typed readback; an undocumented bit refuses
  atomically before a same-adapter retry. The focused
  `libdxfrw_tablestyle_r24_typed_writer` CTest passes (1/1, 1.64 seconds); see
  the [merge-flag receipt](docs/research/p6-r24-tablestyle-merge-flags-transport-2026-11-08.json).
  Commit `bff89295` admits finite, nonnegative per-style text heights with
  exact group-144 readback/refusal controls ([receipt](docs/research/p6-r24-tablestyle-text-height-transport-2026-11-08.json)).
  Commit `37d876cf` admits per-style explicit/inherited margin profiles with
  exact readback and refusal controls ([receipt](docs/research/p6-r24-tablestyle-style-margins-2026-11-08.json)).
  Commit `513a8adf` covers non-default safe group-300 format strings and finite
  group-40/140 scalars ([receipt](docs/research/p6-r24-tablestyle-format-scalar-transport-2026-11-08.json)).
  Commits `9678d0c2`, `12698d59`, and `93c02f0d` cover ODA value/unit code
  domains and qualified border-LTYPE mapping with separate [domain](docs/research/p6-r24-tablestyle-value-format-domains-2026-11-08.json)
  and [reference](docs/research/p6-r24-tablestyle-border-ltype-dxf-transport-2026-11-08.json)
  receipts. These are bounded generated writer/readback results; physical
  source, native rendering and whole-source conversion remain open. The
  preceding ACSH exact-span guard remains in commit `6106bfd4`: the pinned
  AC1027 Sweep body remains `Partial` (174/2,571 bits); do not infer its
  undocumented tail or output route. See its [receipt](docs/research/p4-ac1027-sweep-action-body-exact-span-2026-11-08.json).
- **AC1032 TABLESTYLE control correction:** the generated R24 frame helper
  encoded the TABLESTYLE “Table” cell-style owner as a hard pointer, although
  ODA v5.4.1 §20.4.101.2 specifies a hard owner. The helper now emits the
  documented role. The focused data-flag, cell-style-type, and margin-flag
  selectors and the aggregate `libdxfrw_dwg_object_vectors` CTest pass. This
  corrects invalid test input only; no parser behavior or support claim changed.
  See the [control receipt](docs/research/p6-r24-tablestyle-control-owner-role-2026-11-08.json).
- **P4 inventory decision:** the saved [same-version census](docs/research/p4-local-same-version-strict-output-census-2026-11-06.json)
  reports four already-qualified whole-source passes, 20 strict refusals, and
  zero newly qualified source profiles. The saved [AC1021 source inventory](docs/research/p4-ac1021-opaque-frame-inventory-2026-11-06.json)
  contains only selected graph proofs already represented below; it yields no
  distinct ready physical OBJECTS candidate. Keep the R13 GROUP EED/codepage-0,
  AC1021 TORUS carrier/ACDS owner, and AC1027 Sweep-body gates as recorded in
  the P4 section; do not repeat their probes or weaken strict output. Continue
  with a distinct locally testable P5/P6 implementation gap; do not repeat the
  corpus census or speculate about the remaining Sweep body.
- **P5 pre-R24 TABLE scalar overrides:** the AC1018/AC1021 TABLE reader now
  retains ODA §20.4.96.1 table override bits `0x01`–`0x10` for title/header
  suppression, flow direction and margins. DXF output preserves selected
  default values; unmodeled table bits and all border override masks refuse.
  Generated AC1021 boundary/alignment controls and ASCII/Binary DXF
  readback/refusal/retry controls pass. This is bounded schema/codec evidence,
  not producer or whole-drawing qualification. A current hash-bound trace of
  the AC1021 metric and imperial TABLEs found alignment override mask `0x01`.
  ODA raw-RS decoding yields 42049, outside the defined domain; a bounded
  BitShort candidate yields a valid alignment and lets all 171 CadValues in
  each source parse. The reader keeps these TABLE views semantically partial
  because this candidate conflicts with ODA's RS declaration. Type 16 remains
  supported. The imperial census has no TABLECONTENT evidence. See the
  [current type-48 recheck](docs/research/p5-ac1021-metric-table-current-type48-cursor-recheck-2026-11-09.json)
  and [scalar override receipt](docs/research/p5-ac1018-ac1021-table-scalar-overrides-2026-11-08.json),
  plus the [bounded compatibility-profile receipt](docs/research/p5-ac1021-metric-table-bitshort-fallback-2026-11-09.json).
  The earlier type-48 candidates were caused by the disputed alignment width,
  not valid CadValue type 48 data. The fallback is read-only and partial; it
  does not authorize strict output or a format-support claim. LibreDWG still
  retains these bodies as unknown bits, and ACadSharp's successful decode is
  not independent semantic proof because it uses BitShort for the RS field.
  Resolve the producer-specific alignment encoding and independently verify
  the visible cell meanings before promoting this profile. See the
  [first-failure cell trace receipt](docs/research/p5-ac1021-metric-table-first-failure-cell-trace-2026-11-09.json)
  and [bounded compatibility-profile receipt](docs/research/p5-ac1021-metric-table-bitshort-fallback-2026-11-09.json).
  The current AC1021 metric read publishes all 4,408 frames with zero entity
  or object parse failures. TABLE handles `0x2856e` and `0x2a974` now retain
  their complete raw entity bodies alongside the existing typed, semantically
  incomplete TABLE view; the raw peers are marked unvalidated. This preserves
  2D consumer behavior and source bytes without admitting the disputed cell
  tail. Strict export still stops at FIELD/FIELDLIST qualification. The
  focused DWG reader build and physical read pass; no drawing fixture was
  added. See the [raw-peer retention receipt](docs/research/p5-ac1021-incomplete-table-raw-peer-retention-2026-11-09.json).
  The three row-height values (`0x100000`, `0x200000`, `0x400000`), row
  alignments (`0x4000`, `0x8000`, `0x10000`), and text-style handles
  (`0x20000`, `0x40000`, `0x80000`) now have typed retention. ODA classifies
  the selected text-style handles as hard pointers; the parser publishes only
  matching roles as typed, while retaining a mismatched-role record as
  semantically incomplete. A generated AC1021 vector checks field/handle order,
  values, body boundary, a following sentinel, and wrong-role refusal. The
  table-level row alignments now also map to repeated entity-level DXF group
  170 values in title/header/data order, while cell-level group 170 remains
  distinct. Generated ASCII/Binary parser controls cover mixed cell/table
  values, mask/value cardinality, invalid enums, and exact order; the output
  control covers group order, fresh readback, destination-preserving refusal
  for other unmapped row metadata, and corrected retry. ODA specifies the
  title/header/data DWG field order and Autodesk documents one DXF group-170
  entry per cell type, but the Autodesk page does not state the repeated-entry
  order; this is bounded codec mapping, not a physical DXF producer witness or
  native precedence/rendering qualification. Row text-style handles now map
  through repeated entity-level group 7 names only when the staged STYLE
  registry proves exactly one emitted target. ODA §20.4.96.1 identifies the
  three table row heights as repeated group 140 values; group 141 remains the
  separate physical per-row height array. Both mappings have generated
  ASCII/Binary parseback, cardinality, strict refusal and retry controls.
  Autodesk does not specify repeated-value order, so producer order and native
  precedence remain unqualified. See the [row text-style/height mapping
  receipt](docs/research/p5-ac1021-table-row-text-style-height-dxf-map-2026-11-08.json),
  the [alignment mapping receipt](docs/research/p5-ac1021-table-row-alignment-dxf-map-2026-11-08.json),
  the
  [row-height receipt](docs/research/p5-ac1021-table-row-height-overrides-2026-11-08.json),
  [alignment receipt](docs/research/p5-ac1021-table-row-alignments-2026-11-08.json),
  and [text-style receipt](docs/research/p5-ac1021-table-row-text-style-overrides-2026-11-08.json).
  The three row text colors, fill-none flags and fill colors (`0x20`–`0x2000`)
  now map through TABLE groups 64, 283 and 63. The DXF fill-enabled boolean is
  inverted from DWG fill-none. Only exact index methods (ByLayer, ByBlock and
  indexed ACI 1–255) serialize; true-color CMC, color names/books, unsupported
  flags, invalid indices, and orphaned values refuse output. Generated
  ASCII/Binary controls cover the three slot values, cell-level fields alongside
  entity-level rows, parseback, missing/extra/invalid input, true-color/book
  companions, destination preservation, and same-writer retry. This is bounded
  codec mapping; no physical producer ordering or native style precedence is
  qualified. See the [row color/fill mapping receipt](docs/research/p5-ac1021-table-row-color-fill-dxf-map-2026-11-08.json)
  and the prior [DWG retention receipt](docs/research/p5-ac1021-table-row-color-fill-overrides-2026-11-08.json).
  The three 18-bit border color, lineweight and visibility masks retain
  selected values and explicit presence (including selected zero values).
  Lineweight and visibility now have bounded DXF mappings: each mask selects
  title/header/data values in ODA's six-category order; groups 274–279 and
  284–289 carry those repeated values. DWG visibility is inverted at the DXF
  boundary. Lineweights are admitted only when the signed DXF value is in
  Autodesk's exact 27-value `AcDb::LineWeight` enum; the DWG's unsigned 16-bit
  representation is reinterpreted only for valid enum sentinels such as
  ByBlock. Cell-level and row-level lineweights use the same domain. ASCII/Binary
  parseback, exact group order, missing/invalid-value
  incompleteness, orphan/mask refusal, destination preservation and corrected
  retry are covered. Per-row border CMC remains refused because DXF groups
  65/66/68/69 are single entity-level values: they cannot preserve arbitrary
  distinct title/header/data values for four of the six ODA border categories.
  Groups 63/64 repeat by cell type but cover only two categories. The general
  lossless-mapping question is resolved; a narrower equality-constrained
  projection would need its own explicit profile. Codec tests do not qualify
  physical producer ordering, native precedence or rendering. See the [border
  lineweight/visibility mapping receipt](docs/research/p5-ac1021-table-border-lineweight-visibility-dxf-map-2026-11-08.json)
  and [lineweight enum guard correction](docs/research/p5-ac1021-table-border-lineweight-enum-guard-2026-11-08.json)
  and [CMC representability audit](docs/research/p5-ac1021-table-border-cmc-lossless-dxf-audit-2026-11-08.json),
  plus the earlier [DWG retention receipt](docs/research/p5-ac1021-table-border-overrides-2026-11-08.json).
- **Remaining P5 TABLE border work (bounded, fast):**
  keep per-row border CMC output-refused; the general representability audit
  is complete and does not need to block other P5 work. Revisit only for a
  separately specified equality-constrained profile. Autodesk's
  [TABLE DXF group-code definition](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-D8CCD2F0-18A3-42BB-A64D-539114A07DA0.htm)
  establishes that entity-level groups 7/140/170/63/64/283 may repeat once per
  cell type; 274–279 and 284–289 may likewise repeat per cell type; 65/66/68/69
  represent shared border categories. It does not prove DWG mask-bit ordering,
  CMC-to-DXF conversion for every color method, or output precedence. Execute
  this packet in order:
  1. Audit ODA §20.4.96.1 and existing `DRW_Table::parseCode` /
     `dxfRW::writeTable` sequencing. Use the ODA title/header/data slot order;
     Autodesk documents repeated groups per cell type but omits their order,
     so keep generated-only results bounded pending a physical DXF witness.
     Reject missing/excess values and invalid enums as semantically incomplete;
     keep cell-level and entity-level values distinct. **Done for groups
     7/140/170/63/64/283:** STYLE hard-pointer-to-name resolution, title/header/
     data row heights, alignment, bounded indexed CMC, and fill-enabled mapping
     have ASCII/Binary parseback. Missing, excess, invalid, ambiguous, wrong-
     type, or un-emitted STYLE targets fail closed; non-finite heights refuse
     output, while selected zero is preserved. No public virtuals were added.
  2. Keep the mapped six-category lineweight and visibility arrays, strict
     18-bit mask/value presence checks, the exact 27-value lineweight enum for
     both row and cell overrides, and DWG/DXF visibility inversion.
     Per-row border CMC is resolved as general-profile output-refused: four
     edge categories have only one DXF entity-level value for three independent
     DWG rows. Do not block other P5 work on this non-representable profile.
  3. Keep group 141 physical row heights separate from the three group 140
     title/header/data style heights. Keep groups 94–96 count/mask relationships
     consistent; selected-zero values must not disappear when a mask is cleared.
  4. Generated controls (no new fixture files): for each admitted field, verify
     exact DWG field order/value and body sentinel; ASCII and Binary output;
     parse-back of group-code order, count, and value; unsupported-CMC and
     duplicate/ambiguous text-style target refusals; output
     destination preservation and corrected same-adapter retry. Cover both
     table-level values and an explicit conflicting cell override. Commit each
     small slice with this plan and a receipt; run only the focused vector,
     relevant single CTest, and `git diff --check`. Keep full suites to the
     integration checkpoint. This establishes codec mapping only—not producer
     coverage, native style precedence, rendering, or whole-drawing support.
  Groups 7/140/170/63/64/283 and border groups 274–279/284–289 are implemented
  and covered by
  `libdxfrw_table_row_alignment_override_parse` and
  `libdxfrw_table_scalar_override_output`; details are in the row
  text-style/height and border lineweight/visibility receipts. Next sub-slice:
  keep the general per-row border CMC profile output-refused per the
  representability audit; proceed with unrelated P5 work. Do not add a narrow
  equality-constrained projection unless it has concrete consumer value and
  its own complete field, readback, and refusal controls.
  Keep group 141 physical row heights distinct from group 140 row-style heights.
  Do not block
  independent P4/P6/P7/P8 work on unavailable samples.
- **ODA suffix lane:** the user unlocked the Mac and the planned direct ODA
  27.1 retry was made; it again aborted during Pasteboard/XPC initialization
  before document processing, produced no output, and matched the prior source
  and converter hashes. The unchanged CLI route is exhausted. The two successful
  GUI resaves still have validated trailers at byte 844 and unexplained,
  differing 739-byte suffixes; no byte meaning is established. Keep suffix
  bytes unclaimed and strict output refused. Resume only after a concrete
  launch-route change or a controlled source mutation that can isolate a field.
  See the [post-unlock retry receipt](docs/research/p6-ac1027-oda-cli-after-user-unlock-2026-11-08.json)
  and [GUI comparison receipt](docs/research/p6-ac1027-oda-gui-same-version-resave-2026-11-07.json).
- **AC1021 writer lane:** the four-way scratch matrix is complete; only the
  combined default plotstyle+material graph passes ODA audit for that generated
  profile. Its focused controls verify layer-0 target references for AC1021/24/27.
  Do not repeat the matrix or change global defaults from this bounded result.
  LibreCAD still links its bundled fork and its adapter lacks the upstream
  provider opt-ins; P8 must qualify migration without weakening unresolved
  reference refusal. Keep generated files in scratch storage.
- **External and consumer gates:** P7 independent ACIS/ACDS meaning, P8 clean
  consumer/native Linux and Windows qualification, and P9's six
  `DEFERRED_EXTERNAL` rows remain open. Local parser/codec evidence cannot
  promote those claims. Existing selected LibreCAD_3 persistence checks do not
  replace a clean consumer or native application qualification.

### Next work packets

The queue below contains only unfinished actions. Completed source profiles,
codecs, tests, and controls have been removed from the queue; their commits and
receipts remain in Git and `docs/research/`. Resume from the linked blocker
instead of repeating successful scans or controls.

**Immediate P4 packet — AC1032 associative-network profile and owner path.**
The generated DIMASSOC graph node is indexed and guarded. The AC1032 parser now
preserves separate base/network action-index candidates for class-version-2
networks while leaving `m_networkFieldsParsed=false`. The existing physical
LibreDWG sample has root `0x390` (NOD key `ACAD_ASSOCNETWORK`, owner dictionary
`0x38F`) and child `0x38E` (owner dictionary `0x38D`, owning network `0x390`);
both have a complete bounded parser prefix and the independent LibreDWG 0.14
JSON agrees on indexes and action roles. The supplied ODA v5.4.1 PDF has no
class-specific ACDBASSOCNETWORK schema section, so these candidates do not
qualify output. Autodesk ObjectARX distinguishes logical action membership from
physical hard ownership; its documented NOD root path uses a soft-owning
subdictionary, while block-record subnetworks may be logically parented by the
root and physically owned through a block-record extension dictionary. Keep the
NOD/dictionary path, network action list, action owning-network edge, physical
hard owner, and dependency targets as separate graph roles ([ownership
semantics receipt](docs/research/p4-ac1032-assocnetwork-autodesk-ownership-semantics-2026-11-09.json)).
These API semantics do not resolve the AC1032 class-version-2 byte layout.
Current graph/writer admission remains AC1024-only and physical output stays
refused. Next identify authoritative producer/schema evidence for class version
2, then close the exact owner/action dependencies before enabling any AC1032
writer route. Do not prune `ACAD_ASSOCNETWORK` or DIMASSOC edges to
claim MLINESTYLE output. Once qualified, run exact physical
DIMASSOC→MLINESTYLE output, staged identity inspection, and fresh
ASCII/Binary readback. The generated DIMASSOC graph admission/refusal/retry
control is `dwg2dxf_generated_dimassoc_graph_node`; AC1032 candidate fields are
in the [network receipt](docs/research/p4-ac1032-assocnetwork-candidate-fields-2026-11-09.json)
and physical MLINESTYLE limits are in the [source receipt](docs/research/p6-ac1032-mlinestyle-libredwg-source-profile-2026-11-09.json).

**Additional AC1032 candidate screened (2026-11-10):** the ACadSharp
`samples/aec_objects/AecObjects.dwg` CLASSES table declares four
`ACDBASSOCNETWORK` instances, but its DWG producer/integrity provenance is not
established. The current reader refuses earlier at `BAD_READ_HANDLES`: it
consumes 5,295 of the declared 5,557 handle-map bytes and publishes no object
frames; it also reports a separate HEADER envelope length discrepancy. The
class count therefore supplies no object-field evidence. Do not relax either
integrity check or infer associative fields from the declaration. A reusable
read-only `--assocnetwork-source-inventory` selector now prints candidate
fields and bounded spans for any source the current reader successfully
imports. The selector builds and passes against the existing hash-bound
LibreDWG example, but that remains the previously recorded same-peer profile.
Running it on the ACadSharp AEC candidate returns before inventory because the
reader refuses its handle map; the selector does not bypass that gate.
See the [ACadSharp candidate screen receipt](docs/research/p4-ac1032-assocnetwork-acadsharp-aec-sample-screen-2026-11-10.json).

**Generated regression slice completed (2026-11-09):** a from-scratch AC1032
class-version-2 dispatch control now checks separate base/network action-index
candidates, action ownership flags, and complete bounded prefix statuses while
requiring `m_networkFieldsParsed=false`. Its malformed handle-boundary variant
is rejected without typed publication, then the same reader accepts the
corrected frame. The focused CTest pair passes 2/2, and the existing physical
LibreDWG selector still matches both candidate records and their owner/action
relations; that whole source remains blocked by its unrelated object and HEADER
integrity failures. This is parser regression coverage only: it does not
resolve the missing normative/source-producer schema, qualify graph closure,
or enable AC1032 output. No DWG/DXF fixture was added. See the [generated
control receipt](docs/research/p4-ac1032-assocnetwork-generated-candidate-control-2026-11-09.json).

**P4 AC1032 associative-network output-mapping screen (2026-11-10):** the
LibreDWG `dwg2.spec` marks `ASSOCNETWORK` stable and defines the network
subclass sequence and owned-dependent DXF handle roles. A successful LibreDWG
0.14 DXF projection of the existing hash-bound `example_2018.dwg` makes root
`0x390` action `0x38E` a group-330 soft pointer and child `0x38E` action
`0x391` a group-360 hard owner; it also emits their distinct base/network
action indexes. The schema and projection come from the same peer
implementation, the DWG producer is not established, and native acceptance
was not obtained. This clarifies a candidate mapping only: retain
`m_networkFieldsParsed=false`, keep the AC1032 writer refused, and require
eligible producer/native acceptance plus full graph/dependency closure before
promoting output. The unchanged ODA CLI attempt aborted at Pasteboard/XPC
initialization before reading the file; do not repeat it until desktop session
state changes. See the [LibreDWG DXF projection receipt](docs/research/p4-ac1032-assocnetwork-libredwg-dxf-projection-2026-11-10.json).

The generated R24 TABLESTYLE
border LTYPE, value-domain, scalar format, margin, text-height, merge-flag,
content-layout, alignment, property-flag, border-override, indexed-color,
border-detail, border-lineweight and bounded TABLESTYLE/CELLSTYLEMAP CMC
truecolor/book-color codec slices are complete in commits
`12698d59`, `93c02f0d`,
`9678d0c2`, `513a8adf`, `37d876cf`, `bff89295`, `e19a6170`, `9b064748`,
`7781bcfc`, `9d9de967`, `c4bb35ee`, `4432792f`, `55ce3c7c`, `6463f3b7`, and
`05720365`, `360e8470`, `d12fdb5c`, and `eeb864b5`. The latest
bounded additions follow ODA §20.4.101.3–.4: group 93 admits content-layout
values 1/2/4; group 94 admits cell alignments 1–9; group 90/91 admit
documented property-override masks and AutoScale only when its override is
selected; and border group 90 admits its six defined property bits. Indexed
group-62 colors and the bounded truecolor/book-color CMC profiles (groups
62/420/430) are supported in both R24 TABLESTYLE and CELLSTYLEMAP generated
codec paths. Method-zero RGB states normalize to C2; book/name states without
an RGB base refuse atomically. Border types
1/2, invisibility 0/1, finite nonnegative double-line spacing, and the
existing 27-value lineweight domain are also admitted. Generated controls
verify group-92 ordering and typed values; see the
[lineweight receipt](docs/research/p6-r24-tablestyle-border-lineweights-2026-11-08.json)
and [CMC receipt](docs/research/p6-r24-tablestyle-cmc-truecolor-book-2026-11-08.json).
Generated ASCII/Binary controls verify exact field positions, raw and typed
readback, invalid-value/flag refusal, destination preservation and corrected
same-adapter retries. Each focused build and
`libdxfrw_tablestyle_r24_typed_writer` run passed 1/1; see the
[content-layout](docs/research/p6-r24-tablestyle-content-layout-2026-11-08.json),
[alignment](docs/research/p6-r24-tablestyle-cell-alignment-2026-11-08.json),
[property-flag](docs/research/p6-r24-tablestyle-property-flags-2026-11-08.json),
the [border-override](docs/research/p6-r24-tablestyle-border-override-flags-2026-11-08.json),
the [indexed-color](docs/research/p6-r24-tablestyle-indexed-colors-2026-11-08.json),
[border-detail](docs/research/p6-r24-tablestyle-border-details-2026-11-08.json),
and [lineweight](docs/research/p6-r24-tablestyle-border-lineweights-2026-11-08.json)
receipts. These generated controls add no native rendering or
physical-producer qualification. The saved P4 inventory decision has no new
ready physical graph. Continue with the remaining source-backed P4/P6 queue;
do not repeat this TABLESTYLE codec work.

1. **P4 / resume physical OBJECTS closure only on a new ready witness.** The
   saved census and inventory have been reviewed: every current selected-output
   pass is already recorded, and no distinct physical graph has a closed
   source-to-output route. Keep the exact R13 GROUP, AC1021 TORUS/ACDS, and
   AC1027 Sweep blockers above. Resume this branch only when new eligible
   producer evidence closes source identity, frame disposition, owner/target
   roles and recursive dependencies; do not rescan
   colorwh, plot-screening, lineweights,
   children-room, civil Edge, Pool_Detail VISUALSTYLE, Pool_Detail MATERIAL, or
   the tablet strict-output profile.
   The AC1027 Cover 3DSOLID source-binding slice now retains and reports the
   raw history H handle and exact parser handle-stream span. In the hash-bound
   local Cover source, frame handle `0x6f` is at file offset 2696; its layer
   name `0` resolves uniquely to LAYER `0x10`; its same-handle DataStorage
   record `0x6f` matches the complete 22,983-byte payload; and the null
   History ID is raw code 3, size 0, reference 0 at handle-stream bits 209–217.
   A second already-receipted AC1027 source has 3DSOLID `0x2e1`, code 3, size 2,
   reference `0x2e4` at bits 446–470. See the [history field receipt](docs/research/p4-ac1027-cover-3dsolid-history-wire-span-2026-11-08.json).
   These source-bound observations establish two encoded profiles, not a
   universal omission rule, semantic target graph or output route. ODA
   v5.4.1 §20.4.41 (pp. 144–146) identifies the R2007+ `H` History ID but
   limits the documented ACIS body; §2.13 maps code 3 to hard ownership. The
   prior independent ODA DXF projection emits the same non-null value as group
   350; that cross-format mapping is not a role-equivalence proof. Keep the
   raw role and bit span diagnostic, preserve strict refusal, and do not reject
   tolerant reads or promote support from the observed profile. The production
   bounded field helper now has from-scratch null/non-null/alternate-role,
   partial-bound refusal, cursor-preservation, corrected-retry, and exact-empty
   controls in the focused hardening selector; the external-source comparator
   separately refuses a wrong role/span for its pinned profile. For the pinned
   public AC1027 source, the local parser and LibreDWG 0.14 independently match
   a selected target path: 3DSOLID `0x2e1` H code 3 references
   `ACSH_HISTORY_CLASS 0x2e4`; its source owner resolves to `0x2e1`, its typed
   `m_ownerHandle` is evaluation graph `0x2e3`, graph `0x2e3` is owned by
   `0x2e4` and its one node references `ACSH_SWEEP_CLASS 0x2e2`, which is owned
   by `0x2e3`. The exact handles, classes and graph node match in both views.
   This is a selected identity/edge match, not full semantic or output closure:
   the local `ACSH_HISTORY_CLASS` direct fields parse completely, but the
   referenced `ACSH_SWEEP_CLASS` action-body tail is best-effort; the whole
   source has three unrelated object parse failures, and strict output
   remains refused. A current LibreDWG master source screen finds
   `ACSH_SWEEP_CLASS` classified as `DEBUGGING` but no complete serialized body
   schema in its class map or release notes ([screen receipt](docs/research/p4-ac1027-sweep-open-source-schema-screen-2026-11-08.json)).
   A concrete independent implementation lead is now available in ACadRust,
   pinned at `eababc03c625eaf9cfc5169953d7ee88f2015338`: its current reader puts
   the Sweep scalars/options, DWG-only 3BD and flag 296 before two 16-BD
   transforms, followed by profile/path entities. Its corrective commit says
   this order was checked against reference DWG/DXF output; a later commit
   separately handles modeler wire-body entities and corrects flag meanings.
   The earlier cached docs.rs sequence was stale. This is a peer
   implementation lead, not ODA authority or independent semantic proof (see
   the [ACadRust peer-schema receipt](docs/research/p4-ac1027-sweep-acadrust-peer-schema-2026-11-08.json)).
   **Bounded structural and whole-span capture implemented; one tail bit remains unresolved:**
   the production decoder follows the peer candidate order with bounded
   BL/BS/B/BD/3BD reads and transactionally retains both transforms, options,
   type codes, embedded bit strings, and a separate byte-packed exact copy of
   every bit from action-body start through the object-data bound. The raw copy
   includes modeled fields and unknown suffix bits, without declaring any
   suffix padding or serializer-ready. `AcShActionBody` remains Partial unless
   all fields are recognized and the exact bound is reached. Special type-15/16
   modeler-wire profiles stay opaque pending their separate codec. Focused
   generated controls compare the whole retained span against source bits and
   cover exact-end parsing, suffix retention, special-profile opacity,
   truncation, oversized counts and retry; see the [bounded-capture receipt](docs/research/p4-ac1027-sweep-bounded-capture-2026-11-08.json)
   and [whole-body retention receipt](docs/research/p4-ac1027-sweep-whole-body-retention-2026-11-08.json).
   **Physical trace:** using the
   existing bounded DWG readers on an independent cursor, the pinned AC1027
   sample follows ACadRust's candidate order from bit 228 through both finite
   16-value transforms and two type-77 embedded entities (744 and 480 bits).
   ACadRust's pinned common type table reserves special modeler wire-body
   handling for type codes 15/16, so these witnessed type-77 records use the
   ordinary type-plus-bit-length path.
   Their declared payload spans end at bit 2570 of the object's bounded
   2,571-bit body; the last bit is zero but has no established meaning. The
   second local AC1027 file, `Cover.dwg`, contains no Sweep record. See the
   [field-span trace receipt](docs/research/p4-ac1027-sweep-acadrust-field-span-trace-2026-11-08.json).
   Production readback retains the full 2,343-bit action-body span, including
   both type-77 bodies (744/480 bits), and preserves the zero bit at 2570 as a
   one-bit opaque suffix while reporting Partial. A complete candidate-field
   differential against the hash-pinned same-handle `ACSH_SWEEP_CLASS` DXF
   record matches operation version, direction, six scalars, group-70
   alignment option, documented groups 290/292–296, and matrices 46/47 within
   1.82e-12 maximum absolute delta. Captured embedded payloads equal the
   declared 744/480-bit prefixes of group 310; each DXF string has one extra
   `0xCC` byte, which remains unexplained. The captured second short option
   equals group 71, but [Autodesk's swept-surface table](https://help.autodesk.com/cloudhelp/2019/ENU/AutoCAD-DXF/files/GUID-55DCEC23-9286-4A32-AAFA-E1F945B10A19.htm)
   does not document group 71, so it remains semantically unnamed. The separate DWG-only 3BD
   `(1,1,1)` does not match paired DXF reference-vector groups 11/21/31
   `(0,0,0)`; do not conflate them. This co-located LibreDWG pair is not
   authenticated AutoCAD/ODA output and cannot prove a producer-wide DWG
   layout or embedded-entity semantics. A peer cross-check further lowers
   confidence: LibreDWG 0.14 marks this class `DEBUGGING`; its `dwg2.spec`
   order conflicts with ACadRust, and its exact-input JSON/DXF outputs contain
   2,568 unknown object bits and malformed values. A body-bounded competing-
   order diagnostic tries that `dwg2.spec` sequence directly: after
   major/minor/direction it reads profile/path candidates 0/0 at bit 34 (the
   paired record says 77/744 and 77/480), then stops at a reserved BD code at
   bit 186. This rules out that sequence for this witness, not every profile.
   With the candidate order held fixed, replacing its two BS option fields
   with LibreDWG's RC widths fails at the following vector at bit 63; BS yields
   2/2 and the 2342/2343-bit candidate endpoint. This supports those widths
   only for the pinned sample. The paired DWG and DXF
   entered LibreDWG's corpus in one 2018 conversion/test commit without naming
   an AutoCAD/ODA producer, so the DXF does not establish independent DWG
   semantics. Do not consume the suffix bit or extra `0xCC` bytes as padding,
   and do not reorder fields from this conflict. Next pursue a native or
   independently authored producer witness. Keep Sweep output refused and support claims unchanged
   until graph dependencies, roles, output/readback and independent consumer
   semantics close. See the [candidate-field differential receipt](docs/research/p4-ac1027-sweep-dxf-candidate-field-differential-2026-11-08.json),
   [peer-conflict receipt](docs/research/p4-ac1027-sweep-libredwg-peer-conflict-2026-11-08.json),
   [competing-order diagnostic](docs/research/p4-ac1027-sweep-competing-order-diagnostic-2026-11-08.json),
   and [option-width diagnostic](docs/research/p4-ac1027-sweep-option-width-diagnostic-2026-11-08.json).
   Autodesk's DXF reference explicitly classifies group 350
   as soft-owner, while ODA §2.13 classifies observed DWG H code 3 as hard
   ownership; ODA File Converter emits this same target value as group 350.
   That is an observed format projection, not a normative role-conversion rule.

   **Source-to-output route audit complete; profile remains refused:** the
   reader's `makeRawObject` retains `DwgObjectFrame::body()` bytes and source
   metadata for successfully parsed custom objects; `dx_iface` retains the
   bounded ACSH/evaluation-graph carriers. This is a complete decompressed DWG
   object body, not a DXF payload. For AC1027, the selected OBJECTS writer
   chooses the typed BOX-history collector, whose admitted profile requires
   `ACSH_BOX_CLASS`; the pinned graph instead targets `ACSH_SWEEP_CLASS`. The
   Sweep EvalExpr and HistoryNode prefixes parse, but its class-specific action
   body (embedded sweep/path entities and SweepOptions) remains incomplete.
   The direct `ACSH_HISTORY_CLASS` fields are fully decoded and emitted by the
   collector, but there is no AC1027 Sweep writer. The existing raw-DXF history route preserves original DXF
   group sequences only when the input itself was DXF. The opaque proxy route
   is separately limited to AC1024 and does not convert AC1027 DWG body bytes.
   Therefore no existing lossless route is available, and synthesizing missing
   fields or emitting the DWG body as a DXF proxy would be unsafe. Keep this
   profile output-ineligible and do not change the writer's group-350 mapping.
   The local and LibreDWG 0.14 selected graph rows match exactly (receipt below);
   that corroborates identity/edges, not class-body semantics. Resume only if a
   new authoritative layout or independently decoded complete prefix establishes
   the required DXF fields and role mapping. Do not repeat graph discovery,
   helper controls, or unchanged ODA launches; advance to another ready local
   P4/P6 implementation item.
   The Pool_Detail AC1021 `ACAD_VISUALSTYLE` graph (NOD `0xC` to dictionary `0x33CC`, 16
   styles) now passes selected ASCII/Binary output and typed/raw readback,
   late-mutation refusal, destination preservation and same-adapter retry.
   Pool_Detail `ACAD_MATERIAL` also passes selected ASCII/Binary output,
   typed readback, refusal, destination preservation and same-adapter retry.
   Its exact reachable extension-XRECORD set is empty; children-room's existing
   seven-record profile and the AC1024 mechanical profile still pass. The
   selector now derives the expected census from the selected dependency graph
   instead of assuming every source has seven XRECORDs. AC1021 `tablet.dwg`
   now passes strict whole-source output and fresh same-version DXF readback
   after fixing the absent WCS MESH extrusion default. The locally authored
   WCS MESH control round-trips through all six DWG writer releases; the local
   source remains unauthenticated and makes no support claim. See the [tablet
   receipt](docs/research/p4-ac1021-tablet-wcs-mesh-default-2026-11-07.json)
   and the [Pool_Detail
   MATERIAL receipt](docs/research/p4-ac1021-pool-detail-material-selected-output-2026-11-07.json).
   A screen of the remaining 24 local document DWGs found no populated GROUP,
   IMAGE or IMAGEDEF candidate among 18 valid LibreDWG 0.14 minJSON outputs;
   five CLI failures and one malformed JSON output are excluded as evidence of
   absence. A broader read-only scan of 173 unique DWGs in existing open-source
   worktree corpora found a hash-bound LibreDWG R13 example with one GROUP and
   seven LINE members, plus an R2007 Leader GROUP with five IMAGE members. The
   R13 source publishes 5,765/5,785 frames and strict output stops at the
   exact-source-version ModelDocViewStyle gate. A fresh libdxfrw source-bound
   adapter audit confirms NOD `0xC` → dictionary `0xD` → GROUP `0x257` and its
   ordered LINE members `0x256` through `0x250`; the typed baselines and payload
   comparisons match. The R13–R14 DICTIONARY/GROUP common-link gate is now
   source-qualified for these exact frames: bounded handles and roles pass
   generated AC1012/AC1014 controls and the external R13 import. The
   DICTIONARY still has `UnknownCodePage` text issue mask `0x8`, so selected
   output remains ineligible. ODA v5.4.1 §§20.4.1–20.4.2, §20.4.44 and
   §20.4.72 define the checked inline fields and roles. The exact dictionary EED value is now known (`ACAD` / `TREAT_AS_HARD`,
   codepage `0`); ODA v5.4.1 does not define its codepage semantics. Do not scan
   the whole EED stream again or change the decoder. Reopen this candidate only
   if an authoritative definition or independent producer behavior specifies
   codepage `0`; otherwise leave it ineligible and take another ready P4 item.
   Do not reopen the now-qualified common-link profile. Preserve the original
   1 entity/19 object failures and whole-source refusal. The exact recheck is in the [GROUP candidate
   receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json).
   Do not serialize this source until the remaining text gate and the normal
   completeness/version gates pass. The Leader source publishes
   245/245 frames but whole-source output stops at standalone OBJECTCONTEXTDATA,
   and its IMAGE dependencies retain the unresolved owner-role conflict. An
   ACadSharp AC1021 sample has two six-LINE groups but contains the same image
   role blocker and its producer provenance is not established. These inputs
   remain external and uncommitted. The R13 member identities and their
   dictionary/owner relations are already bound; no selected output is eligible
   while EED codepage zero remains unresolved. Do not
   repeat that import or member census. Keep whole-source output refused. The exact
   scan and current blockers are recorded in the [GROUP candidate receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json);
   the earlier 24-file screen remains recorded in the [local candidate receipt](docs/research/p4-local-dwg-group-image-screen-2026-11-07.json).
   The locally authored GROUP codec control now exercises six ordered unique
   member handles through ASCII/Binary output, typed readback, a second
   round-trip and corrected retry; this is generated transport evidence only.
   Keep the unrelated `SORTENTSTABLE` null-target graph refused. The R13 GROUP
   candidate remains blocked on its source-evidence gates; do not add a general
   selected-output escape hatch or weaken whole-source refusal. The P6
   IMAGEDEF cross-version source capture is complete: its parent code `0xC`
   and dictionary group-350 edge match in AutoCAD AC1021/24/27/32 DWG/DXF
   pairs. Do not repeat that capture. Keep output refused because ODA §20.4.81
   still calls the parent a hard owner; the next change requires independent
   owner-semantics or native rewrite evidence. See the [cross-version role
   receipt](docs/research/p6-imagedef-cross-version-soft-role-evidence-2026-11-07.json).
   The AC1021 `Pool_Detail.dwg` MLEADERSTYLE projection now passes in ASCII and
   Binary through a narrowly scoped dependency proof: NOD `0x7784C` → style
   `0x7784D` → hard-pointer LTYPE `0x12` (`BYBLOCK`), plus its required APPID and
   TEXTSTYLE rows. The source LTYPE_CONTROL omits that row and contains null
   member slots; BLOCK_CONTROL is also malformed, and HEADER current-layer
   evidence is invalid. The selected output binds the exact emitted BYBLOCK
   row and the writer's two canonical default linetypes, with fresh typed
   readback, mutation refusal, destination preservation and same-adapter retry.
   Only the exact three retained source-integrity observations are excluded by
   this private AC1021 one-style/one-linetype profile. This does not prove
   control membership, clear or reinterpret those integrity rows, or admit the
   source to strict whole-drawing output. Other source-error identities,
   versions, styles, or dependency shapes remain refused. The source is an
   unauthenticated local drawing and remains uncommitted. See the [bounded
   MLEADERSTYLE projection receipt](docs/research/p4-ac1021-pool-detail-mleaderstyle-subset-physical-output-2026-11-07.json).
   The AC1021 `truetype.dwg` source now passes `RequireComplete` ASCII and
   Binary output and fresh whole-DXF import readback: all 173 source frames
   publish, with zero entity/object parse failures and zero retained integrity
   rows. Its rooted `ACAD_VIEWS_default` XRECORD survives with four group-330
   LAYER refs, four group-331 LTYPE refs, and four group-370 values. The reader
   had rejected the emitted group-330/331 targets on re-import because the
   extension graph admitted only the DWG-specific LayerStates path; it now
   validates those exact group-code/typed-table target roles. An unresolved
   target still refuses atomically, then corrected same-adapter retry passes.
   This is one unauthenticated local source profile and adds no support claim.
   Exact same-version DXF-origin re-emission of this XRECORD now passes through
   its own parse-time snapshot gate, complete ACAD_LAYERSTATES dictionary path,
   four LAYER/four LTYPE targets, and four `-3` values; changing one value is
   refused atomically and corrected retry/readback succeeds. This closes only
   the observed truetype replay gap. [Autodesk's XRECORD reference](https://help.autodesk.com/cloudhelp/2023/ENU/AutoCAD-DXF/files/GUID-24668FAF-AE03-41AE-AFA4-276C3692827F.htm)
   limits the general application code range to 1–369; the [numeric group-code
   reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-3F0380A5-1C15-464D-BC66-2C5F094BCFB9.htm)
   explains 370's non-entity exception, so keep 370 outside ordinary XRECORD
   output and do not generalize this source-specific LayerStates route. See the
   [strict output/readback receipt](docs/research/p4-ac1021-truetype-strict-dxf-output-readback-2026-11-07.json)
   and [bounded DXF replay receipt](docs/research/p4-ac1021-truetype-layerstate-dxf-group370-replay-2026-11-07.json).
   The earlier civil readiness count of 11 incomplete geometry dependencies is
   stale against the current parser. A fresh source-bound probe finds 28/29
   wire-complete; the sole residual is handle `0xC55`, whose recognized fields
   end at bit 128 while its object-data boundary is bit 152. LibreDWG reports
   the dependency name `AcDbAssocEdgePersSubentId` but an empty persistent-
   subentity classname, and does not explain the remaining 24 bits. Keep this
   record raw and output-ineligible; do not infer an Edge tail or padding.
   Reopen only with authoritative class layout or an independent producer trace
   that accounts for every bit. The other 28 records and the ten-record civil
   Edge selected route are not broadened by this finding. See the [current
   residual receipt](docs/research/p4-ac1024-civil-assoc-current-one-unresolved-2026-11-07.json).
   For any selected subgraph, freeze source
   identity and expected record/handle roles; implement only the missing
   serializer or planner binding; add ASCII/Binary output, fresh typed
   readback, destination-preserving refusal, correction and same-adapter retry.
   If a candidate lacks source proof, record the exact blocker and move directly
   to the next eligible row. Whole-source/native claims remain closed unless
   their own gates pass. Start with the [same-version census](docs/research/p4-local-same-version-strict-output-census-2026-11-06.json)
   and [current source inventory](docs/research/p4-ac1021-opaque-frame-inventory-2026-11-06.json).

### P4-GROUP-01 — qualify the R13 source profile before projection

**Status: BLOCKED on the DICTIONARY EED codepage-zero gate; selected output was not attempted.**
The external LibreDWG R13 witness remains read-only and uncommitted. Its exact
source hash, independent provenance, libdxfrw import identities and ordered
member list are in the [GROUP candidate receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json).
The libdxfrw import confirms NOD `0xC` → DICTIONARY `0xD` → GROUP `0x257` →
seven LINE handles `0x256` through `0x250`, with typed baselines and payload
comparisons matched. It also retains 1 entity and 19 object parse failures.

The selected projection remains ineligible. The DICTIONARY still has text issue
mask `0x8` (`UnknownCodePage`), despite the recognized file-level codepage.
ODA v5.4.1 §§20.4.1–20.4.2, §20.4.44 and §20.4.72 specify the R13–R14 inline
object sizing, common parent/reactor/extension-dictionary roles, DICTIONARY
item references, and GROUP member list.

The common-link issue is now resolved narrowly for AC1012/AC1014 DICTIONARY and
GROUP objects. Their parsers publish `hasDwgCommonLinkTail()` only after the
typed body/count succeeds and each common handle has the expected soft-pointer
or hard-ownership role; GROUP members must be hard pointers. The dispatcher
copies those typed links into the frame publication only when its shared
reactor-count and extension-dictionary header match. Other object families and
legacy versions remain `Unknown`. Generated AC1012/AC1014 controls cover valid
links, truncation, reactor-count mismatch, wrong common-parent/member roles,
wrong reactor/extension-dictionary roles, and a following-handle sentinel; the
focused CTest passes. A fresh import of
the same external R13 witness qualifies DICTIONARY `0xD` (parent `0xC`, one
reactor) and GROUP `0x257` (parent `0xD`, one reactor) with the same seven LINE
members. See the [legacy common-link receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json).

The trace now binds the DICTIONARY issue to one EED value: app handle `0x12`
(`ACAD`), item code 0, codepage 0, and the 13 raw ASCII bytes for
`TREAT_AS_HARD`. The 297 diagnostic occurrences across the full trace were not
deduplicated and are not a count of unique EED values. LibreDWG 0.14 independently
decodes the same ASCII value but does not establish why codepage 0 is valid.
ODA v5.4.1 chapter 28 specifies the codepage field without defining zero as
inheritance from the file header. Keep the decoder and issue mask unchanged;
this remains the sole source-evidence blocker for the bounded GROUP projection.

The post-unlock ODA 27.1 GUI audit converted a scratch copy of this R13 source
to 2010 ASCII DXF without an error report. Its output preserves GROUP `0x257`,
the ordered seven LINE references, dictionary entry `*A1` → `0x257`, and the
dictionary parent relation to `0xC`. It omits the source EED value
`TREAT_AS_HARD` and the referenced SHAPE object, so this is partial independent
projection evidence only: it does not explain EED codepage zero and is not a
lossless conversion. The output was scratch-only; libdxfrw selected output
remains unattempted. Do not repeat this unchanged conversion or promote a
support claim. See the [ODA GUI conversion
receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json).

Implementation sequence:

1. The exact EED app/value/span and the ODA GUI omission are recorded in the
   receipt. No more whole-file scanning or identical conversion is needed.
   Reopen only for authoritative semantics or an independent producer mapping
   for codepage 0; otherwise leave this witness ineligible and move to another
   ready P4 implementation packet. Do not infer that zero means file codepage
   from the ASCII-only value, LibreDWG's decoded text, or ODA's partial DXF.
2. **Complete:** the narrow legacy common-link classifier and generated
   boundary controls are implemented. They are limited to AC1012/AC1014
   DICTIONARY/GROUP typed routes; do not broaden them to other types or versions.
3. **Complete:** the physical source profile recheck confirms both frames now
   publish validated common links. The selected ASCII/Binary projection remains
   unattempted because the EED codepage-zero issue is unresolved.
4. Keep the original import outcome unchanged and require the normal
   whole-source route to remain refused at its existing completeness/version
   gates. Do not commit the external witness or any derived DWG/DXF fixture.

**Fast checks:** `libdxfrw_legacy_dictionary_group_links` passed (1/1); the
reader and `dwg2dxf` targets build. The source-bound R13 profile recheck used
the external witness read-only and retained its 5,785 source frames, 5,765
published frames, and 1 entity/19 object parse failures. No full suite or ASan
run is needed unless production bounds, ownership or lifetime code changes.

Saved-inventory candidate check (no corpus rescan):

| Candidate | Current evidence | Gate / disposition |
| --- | --- | --- |
| LibreDWG AC1012 `example_r13.dwg` | 5,765/5,785 frames publish; NOD → DICTIONARY → GROUP with 7 ordered LINE members; common links now validated | EED codepage `0` remains `UnknownCodePage`; no selected output |
| LibreDWG AC1021 `Leader.dwg` | 245/245 frames publish; GROUP has 5 IMAGE members | IMAGEDEF owner-role conflict and standalone OBJECTCONTEXTDATA block output |
| ACadSharp AC1021 GROUP sample | Two 6-LINE groups | Producer provenance is unestablished; same drawing has the IMAGEDEF owner-role conflict |
| AC1021 `children-room-decoration.dwg` | 112/112 standard objects and 74/74 opaque carriers have selected-output proof | Already-complete exact-source profile; not a distinct candidate |

One bounded physical P4 graph now passes selected source-to-output projection: the AC1021 inline-SAB TORUS graph is emitted to AC1024 ASCII/Binary and read back with exact SAT carrier and remapped graph roles. It does not close the containing source, which still has 387 opaque callbacks and other ACDS graphs; full-source output refuses at the unrelated IMAGEDEF owner-role gate. The saved candidate table below contains no additional ready physical graph. Do not promote this selected route to whole-document output or semantic support.

**Correction from source inspection (2026-11-08):** the proposed next slice
incorrectly assumed that the AC1021 TORUS source had a linked DataStorage
record. The pinned file is AC1021 and its unique solid `0x452` has no
`hasDataStorageRecord`, no DataStorage handle/key, and the drawing has no
DataStorage sections. The solid instead carries an exact 1,059-byte inline
`m_dwgAcisPayload` with modeler version 2 and source version AC1021; its
`m_objectSize` is 1,799 bytes. Keep that carrier identity separate from the
source-bound history path `0x452 → 0x453 → 0x454 → 0x455`. Never synthesize a
physical source DataStorage record or call this graph an ACDSDATA record
selection.

**Completed implementation slice — AC1021 inline-SAB selected route:** the
hash-pinned probe verifies the exact input before import; the AC1021 source
profile now converts inline SAB v2 to the schema-backed SAT v1 text carrier,
closes the exact history/EvaluationGraph/torus and layer extension roles
without DataStorage, and emits the selection to AC1024 ASCII and Binary. Both
outputs pass exact SAT carrier and graph-role readback. An injected source-span
qualification failure preserves the destination; correcting it and retrying
with the same adapter succeeds. Strict whole-source output still refuses at
the unrelated IMAGEDEF owner role. The corrected LibreDWG bytes are a
differential target only; independent native/semantic validation remains open.
See the [inline-SAB carrier gate receipt](docs/research/p4-ac1021-inline-sab-torus-dxf-carrier-gate-2026-11-08.json).

**Completed selected-profile check:** the exact corrected SAT payload was run
through the pinned InventorLoader and native FreeCAD Part/OpenCASCADE; it built
one valid closed shell and solid, with bounds and analytic area/volume matching
the source-derived torus. The ODA-produced DXF carrier was also decoded by
ACadRust and matched the selected source-derived torus fields. These receipts
close only that selected torus semantic check, not an independent-kernel,
source-native, full ACDS/history, or whole-source output gate. See the [native
B-rep receipt](docs/research/p7-ac1021-freecad-native-torus-brep-2026-11-08.json)
and [semantic cross-check receipt](docs/research/p7-oda-ac1021-torus-semantic-crosscheck-2026-11-09.json).
A bounded independent-kernel reconstruction of the selected torus scalar
profile now passes topology, volume, centroid and analytic-bound checks, but the
exact SAT-to-B-rep bridge remains incomplete and its bounds differ slightly
from the prior FreeCAD/OCCT result. Next qualify a source carrier with complete
boundary topology through that independent route, resolve the bound difference,
and obtain source-native acceptance. Broader consumer/platform qualification
remains separate. Do not promote support from parser fields,
the local reader/writer loop, or LibreDWG comparison. Keep full-source
IMAGEDEF and opaque-frame output gates separate.

Use the pinned existing local source only as an opt-in/manual probe. During the
edit loop, rebuild `libdxfrw_dwg_local_roundtrip` once and run the hash-pinned
wrapper plus `dxf_acsh_torus_typed_roundtrip` and `dxf_acsh_cmc_loss_guard`;
run the owning ACDS cohort only if its production hook changes. No full-suite
run is warranted for this isolated slice. Record the exact source/writer gate
result here and in its receipt; do not promote support claims from a refusal
control.

**Current checkpoint:** the source wrapper verifies the receipt's exact SHA-256
before invoking the C++ inspector; a one-byte-mutated scratch copy is refused
before import. The inspector requires unique graph roles,
the exact inline SAB signature and tagged terminator, the 1,059-byte payload,
the AC1021/modeler-version-2 identity, and absence of a DataStorage link. The
opt-in `--check-acsh-torus-source-projection` now emits selected AC1024 DXF in
ASCII and Binary and verifies readback of the exact converted SAT bytes plus
the solid→history→EvaluationGraph→torus handle roles. Both encodings pass a
same-adapter retry after an injected source-span qualification failure, with
the prior destination preserved on failure. The selection includes the exact
layer-0 extension DICTIONARY closure (`0x1C1`, decimal handle 449). No physical
source DataStorage record or ACDSDATA envelope is created. Strict full-source
export still preserves its destination and stops first at the unrelated
unqualified DWG IMAGEDEF owner role, so the opaque-frame gate remains open.
The user said they were unlocking the session, but the next desktop inventory still reported the Mac locked. ODA 27.1 again aborted during PasteBoard/XPC startup before reading input; this produced no drawing-level evidence. A
fresh LibreDWG 0.14 peer conversion again emitted only group 70 and history
handle `453` for output handle `452`. The later verbose LibreDWG trace shows
that its SAB-to-SAT converter runs for handle `0x452`, but the source writer
sets the emitted chunk count to the last chunk index and consequently omits
the final chunk; the exact output entity has only groups 70 and 350. This is a
peer serializer defect, not a positive carrier witness. A disposable archive
of that exact LibreDWG revision was rebuilt with the final block count corrected
from `i` to `i + 1`; its conversion emits 17 group-1 SAT chunks for handle
`0x452`. The output hash and scratch patch/build details are in the receipt.
This gives a peer-derived byte target for implementing our own SAB-to-SAT
route, but does not independently validate the DXF carrier or geometry. The
post-unlock ODA startup retry still aborts before conversion during
PasteBoard/XPC initialization. The generated TORUS/CMC controls and physical
selected-output probe pass; no fixture was added. This qualifies the local
carrier and graph transport for the selected profile, not independent CAD
semantics or whole-document support. See the [inline-SAB carrier gate
receipt](docs/research/p4-ac1021-inline-sab-torus-dxf-carrier-gate-2026-11-08.json).

**Implementation progress (2026-11-08):** the bounded AC1021 inline SAB v2
converter parses the observed version-21200 token profile and emits SAT v1
text. The first independent parser run exposed invalid torus range markers: all
five SAB booleans had been rendered as `F`. The converter now preserves the
`torus-surface` boolean ordinal: the first bit emits `forward_v`/`reverse_v`,
and the four range bits emit `I` when false or `F` when true, with following
numeric bounds retained. A second exact-value comparison found that the
formatter still rounded binary64 numbers to six significant digits; it now
uses `max_digits10` and the classic locale. Generated controls check nontrivial
coordinate/radius lexemes, forward/reverse sense, full ranges, and finite
U-start output. The current physical SAT text is 1,057 bytes (SHA-256
`4917d971df23651987e01f18cf8635d19dab7ec2c306c910734ba0093641da08`). The
pinned independent InventorLoader parser reads 14 records; its typed torus
center, axis, radii, and U origin exactly equal the source SAB-decoded
binary64 values, and its sense/full ranges decode. Generated failure controls also prove NaN in the SAB header tolerance or torus location vector is refused without replacing the prior SAT output buffer. This isolated harness stubs
FreeCAD/Part and does not construct a B-rep or prove native application
acceptance. The initial byte match with the corrected LibreDWG peer is not a
valid target: that output failed typed parsing. The `writeModelerGeometry`
route admits only the source-bound AC1021 E3DSOLID profile and emits only for
AC1021/AC1024 targets. The exact source-bound
`0x452 → 0x453 → 0x454 → 0x455` selected graph passes AC1024 ASCII/Binary
readback, rollback/retry, and strict full-source refusal at the unrelated
IMAGEDEF owner role. Focused token/TORUS/CMC controls pass 3/3, including nonfinite header/vector refusal and unknown extra torus-Boolean refusal with the existing output buffer preserved; the hash-pinned physical probe passes; no fixture was added. This is selected carrier, graph
transport, and independent parser-field evidence, not B-rep semantics, native
application acceptance, or whole-document support. Keep support claims
unchanged. See the [parser field cross-check receipt](docs/research/p7-ac1021-inventorloader-sat-parser-crosscheck-2026-11-08.json).

The local P6 AC1027
source-reference inventory is complete, with
3DSOLID the only partial body report; its selected history target graph is now
identified but the typed body/output route is incomplete. Keep this profile
refused and advance to the next locally implementable P4/P6 packet. Do not
repeat the corpus census or alter the aggregate strict guard from this audit.

3. **P5 / FIELD and TABLE residuals.** No eligible AC1021 FIELD graph currently
   supplies complete FIELDLIST membership plus a consumer-visible root; keep
   FIELD output refused and do not retry the screened samples. Resume only with
   a source profile that closes that graph. Continue other P5 work only where
   exact source spans and target roles are available; keep TABLEGEOMETRY,
   dynamic evaluation and native rendering claims gated. The generated
   pre-R24 TABLE scalar override profile for ODA §20.4.96.1 bits `0x01`–`0x10`
   is implemented; selected title/header suppression, flow direction, and
   margins have bounded DXF output/readback. Row heights, alignments, text
   styles, text colors, fill colors, and fill-enabled flags also have generated
   DXF mappings with refusal/retry controls. Border lineweight and visibility
   map through groups 274–279/284–289 with exact enum checks; general per-row
   border CMC remains output-refused because the DXF schema cannot represent
   four categories of distinct row values. These are generated codec mappings,
   not producer ordering, native precedence, or rendering qualification.
   AC1018 cell-level fill-state, indexed background/content colors, and
   uniquely resolved text-style handles/names now map through groups 283/63/64/7
   with generated ASCII/Binary readback and refusal/retry controls. ODA's DWG
   fill-none bit is inverted to Autodesk DXF's group-283 background-enabled
   value; the generated DWG wire-polarity vector now proves both Boolean states.
   See the [cell-appearance receipt](docs/research/p5-ac1018-table-cell-appearance-dxf-write-readback-2026-11-08.json)
   and [polarity receipt](docs/research/p5-ac1018-table-fill-none-polarity-2026-11-10.json).
   This closes the selected codec slice only; physical ordering, native
   precedence, and rendering remain unqualified.
   The metric and imperial AC1021 TABLE traces now identify the cursor issue:
   both sample cells select override mask `0x01`; raw-RS reads `42049`, while
   the bounded BitShort compatibility candidate produces 171/171 CadValue
   reads per source. The reader retains the valid alignment but keeps each
   TABLE semantically partial because the candidate conflicts with ODA's raw-RS
   field definition. A from-scratch vector covers both the normative raw-RS
   path and the partial BitShort fallback, including the following CadValue
   and table sentinel boundaries. Next obtain authoritative producer/schema
   confirmation and independent semantic evidence for this cell profile; keep
   strict output refused. See the [bounded compatibility-profile receipt](docs/research/p5-ac1021-metric-table-bitshort-fallback-2026-11-09.json)
   and [generated alignment boundary control](docs/research/p5-ac1021-table-alignment-raw-rs-boundary-2026-11-09.json).
4. **P6 / source-backed parser and no-loss gaps.** The
   raster wire observer now recognizes the witnessed IMAGEDEF code-`0xC`
   parent in AC1021/24/27/32 as `UnqualifiedOwnerRole`; the generated control
   verifies offset arithmetic and later reactor spans for all four releases.
   On the physical AC1021 sample, source capture now completes with zero
   entity/object parse failures and strict output stops at the explicit
   unqualified-role gate. Other versions still encounter earlier independent
   strict-output gates. This is source retention only: ODA §2.13's soft-pointer
   offset rule and §20.4.81's hard-owner schema still conflict, despite the
   AutoCAD DXF group-350 correspondence. Do not admit output until independent
   owner-semantics/native rewrite evidence resolves the contradiction. See the
   [role evidence receipt](docs/research/p6-imagedef-cross-version-soft-role-evidence-2026-11-07.json).
   The bounded AC1032 `MENTALRAYRENDERSETTINGS` recovery for frame `0x7B` is
   complete (commit `9e7f393f0`): transactional candidates with and without
   `hasPredefined` accept only a unique body/string-bounded parse. A clean
   current-source diagnostic publishes 182/182 frames with zero entity/object
   parse failures; strict output still refuses at the independent HEADER
   integrity/unclaimed-byte gate. Generated AC1027/AC1032 controls cover both
   layouts, reset/readback and later records. Do not reopen this parser slice
   without a producer witness that contradicts the current bounds. See the
   [parser recovery receipt](docs/research/p6-ac1032-acad-table-cells-reader-fix-2026-11-07.json).
   AC1018 TABLECONTENT and TABLEGEOMETRY are documented as AutoCAD 2008
   additions, so their AC1018 appearances are out-of-version compatibility
   frames. They now remain raw-only after independent R2004 common-shell and
   inline-handle validation. The two AC1018 DATATABLE variable bodies also
   fail typed decoding but pass independent shell validation, so they remain
   raw-only with no DataTable callback. The ACadSharp sample now publishes
   1014/1014 frames with zero entity/object parse failures; strict
   IMAGEDEF/reactor output refusal remains. Generated controls check exact
   payloads, no typed callback and following-frame publication under normal
   and ASan builds. The DATATABLE control also verifies that a one-bit-short
   common-body bound remains a parse failure rather than raw-only publication,
   that the next valid frame still publishes, and that corrected input retries
   successfully ([TABLECONTENT/TABLEGEOMETRY receipt](docs/research/p6-ac1018-tablecontent-tablegeometry-raw-only-shell-recovery-2026-11-09.json),
   [DATATABLE receipt](docs/research/p6-ac1018-datatable-raw-only-shell-recovery-2026-11-09.json)).
   AC1021/AC1024/AC1027/AC1032 TABLECONTENT typed failures likewise fall back
   to raw-only when an independent common-shell/handle parse succeeds; generated
   controls preserve exact bytes and the following frame. The added AC1021 and
   AC1024 controls publish a subsequent typed object after raw recovery. These
   TABLECONTENT controls only improve bounded frame retention; typed semantics
   and output remain unsupported. See the
   [cross-version recovery receipt](docs/research/p6-tablecontent-cross-version-raw-only-shell-recovery-2026-11-09.json).
   The generated AC1027 and AC1032 PERSUBENTMGR controls also shorten the common
   handle span by one bit in each profile: the malformed manager is withheld,
   its following object publishes, and restoring the original span permits
   raw-only recovery. Complete generated vectors for both versions also decode
   exact scalar counts, ordered step/subentity arrays and the bounded Complete
   prefix status, while explicitly retaining `m_typedPayloadValidated=false`
   on the raw carrier. This tests the candidate field sequence only; it does not
   qualify physical bodies, graph closure or output. See the
   [PERSUBENTMGR receipt](docs/research/p6-persubentmgr-common-handle-truncation-retry-2026-11-09.json).
   The companion complete-prefix control checks exact class version, reserved
   scalars, counts and ordered vectors for both versions, requires a bounded
   Complete body status and following-frame publication, and asserts the raw
   carrier remains unvalidated (receipt:
   [complete-prefix control](docs/research/p6-persubentmgr-generated-complete-prefix-2026-11-09.json)).
   A separate LibreDWG issue #1398 recheck confirms the physical body order is
   still disputed: its Platte sample reports `2, 0, 2, 0, 0, 0` before an
   implausible final count; the author lists competing layouts, and the merged
   change was clarified as targeting the distinct ASSOCPERSSUBENTMANAGER class.
   Treat the generated Complete vectors as candidate-parser coverage only; do
   not infer a physical schema or output route from the peer code. See
   [LibreDWG issue #1398](https://github.com/LibreDWG/libredwg/issues/1398).
   This is generated parser coverage only; it does not close physical layout,
   graph, DXF mapping or output eligibility.
   The initial AC1032 corpus run exposed and then resolved a VPORT_CONTROL
   common-reactor ordering defect. The parser now keeps Platte's reactor
   `0x3d126` separate from its VPORT entry `0x57`; the generated control test
   covers distinct handles, truncation/retry and zero reactors. All five
   scratch DWGs now reach OBJECTS. Platte's PERSUBENTMGR object `0x20c50` has
   an 84-bit body ending after candidate scalars `2,0,2,0,0,0`; the longer
   generated arrays candidate remains unadmitted. The later bounded-prefix
   slice publishes only `[2,0,2,0,0]` as Partial for the exact AC1032 profile
   and retains the final two-bit BL zero as opaque; it does not decode the sixth
   scalar or establish its role. See the
   [control-reactor receipt](docs/research/p6-ac1032-control-common-reactor-fix-2026-11-09.json)
   and the [source/provenance receipt](docs/research/p6-ac1032-vport-public-pair-read-blocker-2026-11-09.json).
   A hash-matched physical recheck of
   `example_2013.dwg` publishes object `0xC94` raw-only; its remaining object
   parse failure was subsequently recovered. Bare AC1027/AC1032 PERSUBENTMGR
   profiles outside the exact bounded-prefix admission retain raw frames after
   the same independent shell check; physical object `0x2E0` reduced the
   failure count from two to one.
   The AC1027 `AcDbAssocOsnapPointRefActionParam` at `0x393` now likewise
   publishes raw-only, bringing the same source to 481/481 frames and zero
   entity/object parse failures. ODA Specification 5.4.1 has no class-specific
   entry for this object, and LibreDWG labels its route `UNSTABLE`; neither
   provides a safe typed layout. Keep it raw-only pending a later authoritative
   schema or producer-backed field-span proof. These recoveries do not establish
   body semantics or qualify output: the strict source-reference gate still
   refuses export. See the [TABLECONTENT recovery receipt](docs/research/p6-ac1027-tablecontent-raw-only-shell-recovery-2026-11-08.json),
   [PERSUBENTMGR recovery receipt](docs/research/p6-ac1027-persubentmgr-raw-only-shell-recovery-2026-11-08.json),
   and [action-parameter recovery receipt](docs/research/p6-ac1027-osnap-action-param-raw-only-shell-recovery-2026-11-08.json).
   TextObjectContextData remains raw-only.
   Remaining P6 work is limited to source-backed version/layout gaps, including
   PERSUBENTMGR tail semantics/graph/output, and independent evidence for the
   IMAGEDEF owner-role conflict and AC1027/AC1032 HEADER suffixes. The prior
   same-version ODA resaves established repeatable suffix presence but did not
   isolate its meaning. On 2026-11-09, two scratch DXFs derived from the
   existing Cover ODA projection and differing only in `$LTSCALE` (1.0/2.0)
   were converted to AC1027 by ODA File Converter 27.1.0.0. LibreDWG 0.14
   independently reads the intended values; libdxfrw publishes 119/119 frames
   with zero entity/object failures for each, then correctly refuses DXF output
   at the separate source-reference gate. Both decompressed HEADER sections
   pass the sentinel and CRC16 audit. Their size words differ by eight, and the
   unclaimed suffix lengths are 28,852 and 28,844 bytes with different hashes.
   This controlled correlation does not identify a field boundary, suffix
   encoding, or causal relationship to `$LTSCALE`; keep every suffix byte
   unclaimed. Two earlier locally authored minimal AC1027 DWGs were rejected
   by ODA as needing recovery and are excluded from this comparison. See the
   [controlled ODA suffix receipt](docs/research/p6-ac1027-header-controlled-oda-suffix-2026-11-09.json)
   and [earlier local-control receipt](docs/research/p6-ac1027-header-variable-controlled-conversion-lock-2026-11-09.json).
   Do not repeat the unchanged resave or suffix-only mutation. Further suffix
   interpretation needs source-level field evidence; continue independent
   implementation meanwhile.

   **AC1027 source-reference audit update (the guard change remains
   unauthorized):** marker lifecycle is audited:
   `fileImport` sets the aggregate marker before DWG parsing, retains it for
   AC1027, and only restores the prior marker for a non-AC1027 read that did not
   change generation; strict/owner-resolved export checks it before planning.
   The new bounded audit runner hashes the source before and after, validates
   every retained object identity, and records per-frame common links, typed
   references, and dictionary edges. On the existing Cover source, 111/111
   frames publish (75 opaque, 13 standard); the inventory records 86 owner
   edges and 85 reactor edges. ODA-backed body classification covers 40
   no-body-handle records (33 SCALE, five DICTIONARYVAR, one RASTERVARIABLES,
   one pre-R2018 MLINESTYLE) and 18 handle-bearing profiles (13 DICTIONARY, two LAYOUT, one
   DICTIONARYWDFLT, one TABLESTYLE, and one MLEADERSTYLE). A separate LibreDWG source profile
   covers 29 no-body frames: one ACDBPLACEHOLDER, 24 VISUALSTYLE, three
   MATERIAL, and one SUN. The independent minJSON read matches all 32
   MLINESTYLE/PLACEHOLDER/VISUALSTYLE/MATERIAL/TABLESTYLE/MLEADERSTYLE/SUN source frames by
   type, frame size, owner, and reactor. For TABLESTYLE, ODA §20.4.101.2–.4
   plus source-bound spans classify the R24 hard-owner cell-style slot, four
   text-style slots, and 24 border-linetype slots (29 total); the same-source
   LibreDWG minJSON confirms the common identity and hard-owner token. There are
   43 dictionary-membership edges, one selected typed report (3DSOLID), and no body
   with an unknown reference disposition. 3DSOLID remains partial; the TABLESTYLE
   and MLEADERSTYLE body-reference profiles are complete. None of these profiles
   qualifies whole-object output or native semantics. Cover MLINESTYLE frame `0x18`
   (type 73) belongs to the `ACAD_MLINESTYLE` dictionary at `0x17` under key
   `Standard`; ODA §20.4.73 specifies inline linetype indices before R2018,
   with linetype handles only from R2018 onward. These classifications do not
   qualify output or native semantics. See the [current audit
   receipt](docs/research/p6-ac1027-cover-reference-guard-audit-refresh-2026-11-07.json)
   and runner `tools/research/audit_ac1027_source_references.py`. The prior
   interim-guard receipt's LAYER/LTYPE role and target mutations remain required
   negative controls.

   **AC1027 TABLESTYLE body-reference profile completed (2026-11-08):** for
   Cover handle 106 (type 504, class `AcDbTableStyle`, owner/reactor 105), the
   parser now checks that the R24 `cellstyle` handle uses ODA's hard-owner code
   and records its bounded handle-stream span. The focused source inventory
   proves 29 handle slots: one R24 cell-style hard-owner slot, four nested
   text-style slots, and 24 border-linetype slots. All spans are complete; the
   Cover hard-owner target is null, and the 21 non-null text-style/linetype
   references remain individually reported. LibreDWG minJSON matches the same
   type, frame size, owner and reactor, and its schema corroborates the
   hard-owner token. The root dictionary path is NOD key `ACAD_TABLESTYLE` to
   dictionary 105 to style 106. ODA sections §§20.4.101.2–.4 define the roles.
   The focused `libdxfrw_tablestyle_r24_typed_writer` selector and refreshed
   source-bound audit pass. This closes TABLESTYLE's body-reference disposition
   only; whole-object output, full typed semantics and native consumer behavior
   remain unqualified. One selected typed report remains: 3DSOLID.

   **AC1027 MLEADERSTYLE body-reference profile completed (2026-11-08):**
   Cover handle 108 (custom type 505, frame size 122, owner/reactor 107) now
   has all four §20.4.87 body H fields decoded with bounded spans and validated
   hard-pointer roles: line type → 20, arrow-head block → null, text style → 17,
   and block → null. The exact source path is NOD key `ACAD_MLEADERSTYLE` →
   dictionary 107 → `Standard` → style 108. LibreDWG minJSON from the same
   source matches type, size, owner and reactor; its `dwg2.spec` MLEADERSTYLE
   schema independently declares the same four hard-pointer fields. The
   focused MLEADERSTYLE cohort passes 5/5. A synthetic DWG receipt is explicitly
   refused for strict output with destination preservation; generated opt-in
   writer checks remain mechanical codec evidence only. This closes the
   MLEADERSTYLE body-reference disposition, not whole-object output or native
   semantics. The source audit now has 18 normative handle profiles, 32 peer
   matches, and one remaining partial body report: 3DSOLID.

   **Completed body-reference profiles:** Type 73 MLINESTYLE is classified for
   pre-R2018 under ODA §20.4.73. Type 80 ACDBPLACEHOLDER, the 24 type-501
   VISUALSTYLE frames, and the type-506 SUN frame have source-matched LibreDWG peer profiles from
   `dwg2.spec`. The same Cover DWG minJSON run matches all 32 peer-profiled records to
   libdxfrw's captured identities, frame sizes, owner handles, and reactors.
   Type 73 belongs to the rooted `ACAD_MLINESTYLE` dictionary and has no body
   H fields; type 80 is dictionary member `Normal` and the default target of
   its owner dictionary; VISUALSTYLE bodies have no H fields before the common
   handle stream. SUN handle 109 has owner 41, no reactors or extension
   dictionary; the independent VPORT report links handle 41 to SUN 109, and
   ODA §20.4.81 defines that VPORT SUN field as a hard owner. The type-506
   peer schema contains no body H fields before the common handle stream.
   DICTIONARYWDFLT handle 14 is rooted at NOD key `ACAD_PLOTSTYLENAME`; its
   one dictionary entry and ODA §20.4.45 default hard pointer both target
   handle 15. These are source-role profiles, not output qualification.
   The MATERIAL parser now recognizes the source-matched AC1027
   default values (inherited colors, scene-sourced refraction, channel flags
   127) and marks those three records semantically complete. This is a bounded
   read profile; the exact AC1027 default body and detached strings now replay
   for the three parsed default records only. Other modern MATERIAL bodies and
   values remain refused. Keep normative ODA and peer-based dispositions separate: MLINESTYLE and the
   R24 TABLESTYLE handle roles are directly specified by ODA §§20.4.73 and
   20.4.101.2–.4.
   `DRW_MLineStyle::parseDwg` consumes inline AC1027 element linetype indices;
   the hash-bound inventory validates its source frame/common links and NOD
   dictionary edge. The peer PLACEHOLDER evidence is scoped to the same source
   and exact AC1027 graph; it does not replace the ODA authority.

   **Completed generated AC1032 MLINESTYLE handle-role parser slice
   (2026-11-09):** ODA §20.4.73 changes per-element linetype fields from
   inline BS indices to H hard pointers in R2018+. `DRW_MLineStyle::parseDwg`
   now validates those roles, permits only canonical null, and retains ordered
   exact handle-stream spans transactionally. Generated controls independently
   walk the stream through a sentinel, reject each wrong role, and verify
   truncation reset plus same-object corrected retry; a pre-R2018 control
   confirms inline indices add no handle spans. The focused build and both
   object-vector CTests pass. A follow-up generated writer round trip confirms
   zero linetype targets serialize as canonical null (code 0), preserve the
   following hard-pointer target and sentinel alignment, and parse back with
   both targets intact; the focused selector passes 1/1 after this control.
   This is generated codec evidence only: there is
   no physical AC1032 producer witness, target-graph closure, output admission,
   or native-consumer qualification. No fixture was added. See the
   [AC1032 MLINESTYLE receipt](docs/research/p6-ac1032-mlinestyle-linetype-handle-roles-2026-11-09.json).

   **AC1027 MATERIAL default parser and exact-profile body replay completed.**
   The three type-502 MATERIAL frames (handles 68/69/70; keys
   ByLayer/ByBlock/Global) match LibreDWG minJSON from the same Cover source by
   identity, size, owner, reactor, and the decoded default semantic fields.
   The body schema has no H fields before common handles. The parser now
   recognizes AC1027's inherited ambient/diffuse/specular colors, six simple
   maps, scene-sourced refraction, zero translucence/self-illumination/
   reflectivity, channel flags 127, and nonprocedural mode. The focused source
   audit asserts these values and confirms all three semantic-completeness
   flags. The versioned default builder now emits the observed AC1027 profile;
   AC1021 remains unchanged, and AC1024 remains bounded to its separately
   witnessed AutoCAD 2016 profile below. This does not enable AC1027
   output: `DRW_Material::encodeDwg` continues to refuse retained modern raw
   tails outside the admitted exact profile. The writer replays the retained
   516-bit AC1027 default body and its five explicit empty detached map strings
   only for parsed `Global`/`ByBlock`/`ByLayer` records whose full decoded
   selectors still match the source profile. A generated full-file
   dictionary/MATERIAL graph finalizes and fresh libdxfrw readback confirms
   the exact body and strings. A source-fed selector takes all three parsed
   records from the physical Cover source through one selected DWG write with
   a shared ACAD_MATERIAL dictionary, then checks exact raw bodies, typed
   fields, names, detached strings and remapped graph roles on fresh readback.
   This is not whole-source rewrite or producer/native acceptance. AC1021 and
   other AC1024 raw bodies, other AC1027 bodies and newly authored modern MATERIAL values
   remain refused. The SUN, DICTIONARYWDFLT, TABLESTYLE, and MLEADERSTYLE
   body-reference profiles are complete. Only 3DSOLID remains partial in the
   physical body-reference inventory; independent producer acceptance, broader
   graph closure and the global strict guard remain open.
   Do not rescan the corpus. See the [body replay receipt](docs/research/p6-ac1027-material-default-body-replay-2026-11-10.json).

   **Completed source-bounded AC1024 AutoCAD 2016 MATERIAL replay (2026-11-10).**
   The public `nextgis/dwg_samples/arc_2010.dwg` source (AC1024; README
   producer claim AutoCAD 2016 M.49.0.0) supplies `ByBlock`, `ByLayer`, and
   `Global` MATERIALs with 1284-bit bodies, six empty source-0 maps using
   projection 2/tiling 1/auto-transform 1, scale matrix diag(1/48, 1/48, 1, 1),
   and channel flags 127. The exact profile predicate rejects any other
   semantic/body shape. A selected writer replay remaps the three materials,
   their three six-entry extension dictionaries, and all 18 TILE XRECORDs;
   fresh libdxfrw readback verifies material bodies/fields, xDictionary links,
   dictionary keys/owners and all XRECORD owners/reactors/payloads. LibreDWG
   0.14 independently reads the generated AC1024 output and confirms those
   semantics, graph targets, the 23/14 AcDbMaterial capability metadata, and
   all 18 `{270:1,271:1}` tile records. Detached-string replay also exposed a
   writer bug: the R2007+ stream assembler copied byte padding as logical string
   bits. It now appends only the string buffer's logical bits and pads the
   combined stream at its trailer boundary. The generated 51-object output
   peer-parses successfully. The focused MATERIAL CTest passes 1/1. An
   injected first-allocation failure preserved pre-existing destination bytes;
   the same writer then retried successfully and passed the same graph
   readback. The input
   and output remain in `/private/tmp`; no downloaded drawing is committed.
   This is a selected source-fed writer/readback profile, not whole-source
   conversion, AutoCAD acceptance, or broad AC1024 support. Other AC1024
   MATERIAL bodies and unsourced output graphs remain refused. See the
   [AutoCAD 2016 MATERIAL receipt](docs/research/p6-ac1024-autocad2016-material-replay-2026-11-10.json).

   **Completed MATERIAL profile/retry slice (2026-11-08):** the AC1027 exact
   frame writer now validates its body and detached strings against the
   versioned default profile builder. This removes stale AC1024 fixed-length
   assumptions that rejected the AC1027 516-bit field body and 118/134-bit
   detached strings. Focused tests now pass for AC1024 defaults, AC1027 opt-in
   graph emission, injected dictionary failure with buffer/checkpoint rollback
   and same-adapter retry, and retained-modern-tail replay refusal. That
   2026-11-08 slice predates the bounded 2026-11-10 AC1027 default-body replay
   above. The one partial AC1027 report (3DSOLID), broader AC1027 MATERIAL
   encoding, other AC1021/AC1024 MATERIAL output, whole-source DXF output, and
   aggregate strict guard remain open; neither slice promotes broad support.
   The refreshed source-reference audit independently passes with 88 retained
   identities, 40 normative no-body profiles, 18 normative handle profiles,
   29 peer no-body profiles, 43 dictionary edges, one selected typed report (3DSOLID),
   and zero unknown body-reference dispositions. That audit supports source
   classification; the focused writer tests support this bounded retry change.

   **Bounded physical AC1027 MATERIAL-to-DXF slice (2026-11-08):** the first
   selected Cover probe exposed two wrong assumptions in the exact dictionary
   validator: it required zero reactors/BL reactor count and imposed one fixed
   key order. ODA v5.4.1 §20.4.44 places the reactor count after EED; the
   source has one NOD-owner reactor repeated in the common handle stream. The
   validator now checks that owner role and the same-source ordered name/soft-
   owner-handle pairs, while accepting only the unique `ByLayer`, `ByBlock`,
   and `Global` member set. A selected physical Cover graph then passed ASCII
   and Binary output, fresh libdxfrw readback, destination-preserving mutation
   refusal, and same-adapter retry. The projection also includes its rooted
   RASTERVARIABLES record because that independent NOD edge is required by
   preflight. The fixture remains local and is not committed. This qualifies
   only the selected same-version graph and local transport/readback; it does
   not qualify DWG MATERIAL encoding, whole-file conversion, other dictionary
   profiles, or native/independent semantic behavior. Keep the aggregate strict
   guard and those broader claims open. A generated 520-bit frame regression
   now covers both observed member orders, exact name/handle sequence pairing,
   duplicate-key refusal, typed reordering, and reactor-role refusal; it is
   registered as a fast CTest and included in source-release manifests. The
   focused MATERIAL cohort passes 3/3. See the [physical output receipt](docs/research/p6-ac1027-material-physical-output-probe-2026-11-08.json).

   Each frame must end in
   `no body handles`, ordered typed handle fields with spans/roles, or
   `unknown/incomplete` with a fail-closed reason. Bind fields to immutable
   import-generation/frame identities, resolve targets to exact identities and
   roles, and record their selected-DXF disposition, including references from
   omitted frames. Add the smallest generated role/target mutation and
   rollback/retry/readback control for each admitted class. Keep the aggregate
   AC1027 strict guard until every frame in a candidate profile has complete
   body disposition, all selected edges close, omitted-frame dependencies are
   qualified, failed capture remains fail-closed, and ASCII/Binary output plus
   fresh typed readback pass. The Cover HEADER suffix is an independent blocker;
   these inventories do not qualify output or support claims.
**P6 PERSUBENTMGR implementation slice completed (2026-11-10):** the pinned ACadRust 0.6.8 runtime route was executed against five hash-bound ACadSharp DWGs. For the AC1027/AC1032 D63 objects, cursor instrumentation observes the five-BL prefix `[2,0,2,0,0]`, followed by a two-bit BL zero, with the independently reported handle-stream start one bit later. ACadRust consumes the zero as part of its flat integer tail but does not assign a field role. The reader now exposes only the five-field source-bound prefix as `Partial`, probes the opaque tail non-destructively to require a complete BL zero, and preserves the raw carrier unvalidated. Generated controls cover both versions, a one-bit near miss, a truncated nonzero two-bit tail, following-object recovery, AC1032 handle truncation, and corrected-input retry. The focused selector passes. Fresh `dwg2dxf -d` reads of both hash-bound AC1027/AC1032 sources reach the typed associative-shell callback, publish 865/865 and 842/842 frames respectively, and report zero entity/object parse failures; output is later refused at unrelated source-completeness gates. This does not establish tail meaning, array semantics, graph closure, output, or a support claim; keep those gates open. See the [ACadRust schema/span receipt](docs/research/p6-persubentmgr-acadrust-schema-screen-2026-11-10.json).

5. **P7/P8/P9 / qualification gates.** P7 needs independent ACIS/ACDS semantic
   evidence. P8 now has a clean `origin/master` compile and focused
   64-test export/round-trip pass only after the scratch consumer's
   version-aware `$INSUNITS` callback and malformed root-DICTIONARY fixtures
   were corrected. Those changes are applied in clean-master branch
   `codex/libdxfrw-upstream-compat` (`36082e71`); follow-up `93354846` covers
   both R12 and R14 unit-loss behavior with a focused regression. Next review
   and propose that consumer patch, then qualify native Linux/Windows FreeCAD
   and LibreCAD/LibreCAD_3 runs. Keep the dirty-checkout build as integration
   evidence only. P9 remains
   `DEFERRED_EXTERNAL` for J256, J260, J268,
   J284, J293 and J295. These do not block independent local slices and cannot
   be promoted by generated controls, advisory reports, or transport-only
   evidence.

After every slice: run its focused fast checks; update this queue and the
relevant receipt; commit code, plan and receipt together; report the commit and
next eligible slice, then continue. Keep fixtures out of commits unless already
in LibreCAD/libdxfrw or authored locally from scratch. Reserve full suites for
integration/release checkpoints.

| ID | State | Remaining work | Dependency |
| --- | --- | --- | --- |
| P4 | OPEN — source and graph closure | Continue with the next distinct physically evidenced OBJECTS subgraph; bind source identities, owner/target roles, recursive dependencies, and exact emitted records. The selected AC1024 class-version-1 associative-network parser now rejects a logical `isOwned` action flag whose DWG handle code disagrees (owned → hard ownership; non-owned → soft pointer); both mismatch directions and corrected same-reader retry pass. The existing AC1024 architectural input parses all 28 action references across five networks with the guard enabled. This remains a bounded input-role check; broader source/producer and network graph closure remain open. Pool_Detail MLEADERSTYLE passes selected ASCII/Binary transport under the exact bounded AC1021 one-style/one-BYBLOCK projection; malformed LTYPE_CONTROL, BLOCK_CONTROL, and HEADER current-layer integrity observations remain excluded, and strict whole-source output remains refused. The AC1021 truetype whole source passes `RequireComplete` ASCII/Binary output and fresh whole-DXF import readback at 173/173 published frames; its group-330 LAYER/group-331 LTYPE references and exact LayerStates DXF-origin group-370 replay now pass under source snapshots and the complete dictionary path, with mutation refusal/retry. Other group-370 XRECORDs remain refused. Do not infer producer-wide support or generalize either source-specific gate. Resolve the tracked ENC dangling-reference profile only with eligible producer evidence. Keep unresolved null members, orphan owners, unsupported frames, and non-model/paper SORTENTSTABLE owner profiles (named or anonymous) refused; the bounded counted BLOCK_CONTROL tombstone warning documented in the AC1024 source profile below does not qualify a broader null-member profile. Broaden beyond individual local sources only with independent producer and native output evidence. **AC1032 MLINESTYLE `0x18` remains a bounded source-only profile:** its snapshot binds both element handles to unique LTYPE `0x15` (`ByLayer`) and rejects changed handles/names. This implementation slice adds a typed DIMASSOC graph node, NOD/contained-dictionary target indexing, exact source-frame/owner checks, refusal when modeled body details are unrepresentable, and a generated same-adapter retry control. The AC1032 parser now preserves separate class-version-2 base/network action-index candidates for the existing sample's root `0x390` and child `0x38E`, with complete bounded prefixes matching LibreDWG 0.14 values; `m_networkFieldsParsed` remains false. A generated dispatch control now covers three logical actions with mixed ownership flags and a separate owned-action handle vector; it asserts the full bounded prefix while retaining `m_networkFieldsParsed=false` ([receipt](docs/research/p4-ac1032-assocnetwork-multi-action-generated-2026-11-10.json)). This tests parser handling of candidate values only: the ACadSharp source is unqualified and fails before OBJECTS, so it supplies neither physical span nor schema evidence. The supplied ODA v5.4.1 PDF has no class-specific ACDBASSOCNETWORK schema section, so the candidate is not output-eligible. Physical export remains blocked by the AC1024-only associative graph/writer policy. The independent DIMASSOC leaf is physically confirmed at dictionary `0x3A8`, key `ACAD_DIMASSOC`, target `0x3B4`; it is not reached until its owning route is qualified. Next obtain authoritative class-version-2 schema/producer evidence and close all owner/action dependencies before considering output; then rerun exact DIMASSOC output, staged-record inspection, and fresh readback. See the [AC1032 network candidate receipt](docs/research/p4-ac1032-assocnetwork-candidate-fields-2026-11-09.json). Keep strict whole-source refusal. See the [AC1032 MLINESTYLE source receipt](docs/research/p6-ac1032-mlinestyle-libredwg-source-profile-2026-11-09.json).** **The AC1027 Cover 3DSOLID history field/span and the public-source `0x2e1` → `0x2e4` → `0x2e3` → `0x2e2` identity/edge path are captured and match LibreDWG 0.14 (receipt `p4-ac1027-cover-3dsolid-history-wire-span-2026-11-08`). The production helper has null/non-null/alternate-role and truncation/reset/retry controls. `ACSH_HISTORY_CLASS` direct fields are complete; the graph targets `ACSH_SWEEP_CLASS`, whose EvalExpr/HistoryNode prefixes parse. The production parser now uses the pinned ACadRust candidate order and transactionally captures the hash-bound 2,571-bit AC1027 action body through the exact object bound. The local probe retains type-77 profile/path bit strings (744/480 bits) and the final zero bit as an opaque one-bit suffix; the body status correctly remains `Partial` ([field-span receipt](docs/research/p4-ac1027-sweep-acadrust-field-span-trace-2026-11-08.json), [bounded-capture receipt](docs/research/p4-ac1027-sweep-bounded-capture-2026-11-08.json)). Generated focused controls cover exact-end capture, suffix retention, opaque special profiles, truncation, oversized counts and retry. The suffix identity and embedded geometry semantics remain open; do not treat the bit as padding or map unqualified fields to DXF. Next resolve the suffix and decode supported embedded-entity profiles transactionally. ACadRust's claimed reference-output checks are an implementation lead, not normative/semantic support evidence. The typed BOX-history writer has no Sweep route, and raw-DXF preservation applies only to DXF input; the opaque proxy writer is AC1024-only. Keep output and strict conversion refused. The DWG code-3 to DXF group-350 role mapping remains unqualified; do not alter writer roles from converter output alone.** | Existing Cover inventory, ODA v5.4.1 §§2.13 and 20.4.41, hash-bound source probes, production-helper controls, LibreDWG 0.14 graph view, pinned ACadRust source and hash-bound field trace, output-route code audit, and Autodesk DXF group-code authority. |
| P5 | OPEN — dynamic-block semantics, rendering and TABLE profiles | Find a complete FIELD/FIELDLIST source graph with a visible root; for TABLEGEOMETRY, the 177-file local corpus scan is now exhausted and found no non-null cell soft pointers or nonzero standalone unknown BD. A new traceable producer sample is required for external semantic evidence; a generated-from-scratch control may cover code paths but cannot promote producer support claims. The four pinned AC1027/AC1032 objects now have exact independent body/string/handle spans; Python and C++ walks agree, and all decoded fields match typed libdxfrw plus LibreDWG 0.14. They contain only null cell refs and zero unknown BD values, so they do not authorize a broader output profile. Current-code AC1027 and AC1032 reads publish both standalone records with zero entity/object failures, while whole-source DXF exports remain refused by unrelated source-reference / OBJECTCONTEXTDATA gates. The bounded AC1021+ planar/null-cell-reference DXF writer already exists; its focused generated projection control passes 1/1. See the [physical-span receipt](docs/research/p5-acadsharp-tablegeometry-ac1027-ac1032-physical-spans-2026-11-09.json), [local corpus receipt](docs/research/p5-tablegeometry-local-nonnull-reference-corpus-scan-2026-11-09.json), and [current reader recheck](docs/research/p5-acadsharp-tablegeometry-current-reader-recheck-2026-11-09.json). **Generated AC1018 TABLESTYLE and the bounded plain-text TABLE plus `*T1` cache now pass whole-file finalization and fresh libdxfrw readback; LibreDWG 0.14 independently confirms the TABLE graph and cached geometry but leaves the TABLE cell body opaque.** The pre-R24 value-flag profile is now constrained to ODA-defined bits; generated AC1018 `0x16` read/write and unknown-bit refusal/retry pass, while the selected physical source TABLE body now encodes and reads back only after explicit same-version proxy-cache authorization; the generated compound graph also passes parsed-role closure/readback, while physical whole-source output remains refused. Remaining TABLE gates: native AutoCAD/ODA acceptance/rendering or independently decoded cell semantics; additional physical producer/profile qualification beyond this one selected body; R24 TABLECONTENT; unknown/unmatched retained source subrecords (the bounded zero-mask signature is admitted by the direct codec only); FIELD/BLOCK contents; and remaining CONTENTFORMAT/CELLSTYLEMAP combinations. See the [TABLESTYLE receipt](docs/research/p5-ac1018-table-nod-tablestyle-whole-file-2026-11-10.json) and [TABLE receipt](docs/research/p5-ac1018-table-whole-file-cached-geometry-2026-11-10.json). Also qualify remaining CONTENTFORMAT/CELLSTYLEMAP combinations; obtain a native ASCII/Binary VIEWPORT group-91 witness to resolve its conflict. The 2026-11-09 screen found Autodesk's 2008/2020/2021 entity-specific references consistently call group 91 a soft pointer, while ezdxf says its override storage is undocumented. A from-scratch ezdxf 1.4.4 R2010 color-override drawing stores the value in a LAYER extension-dictionary XRECORD (`102 {ADSK_LYR_COLOR_OVERRIDE`, `335` viewport handle, `420` color) and emits no group 91 in the VIEWPORT; this peer-specific result still is not a native witness. Keep parser refusal and the generic Int32 conflict until native ASCII/Binary evidence is available ([source screen receipt](docs/research/p5-viewport-group91-source-screen-2026-11-09.json)). Also qualify native layer-override activation/rendering and establish native dynamic constraint evaluation. For pre-R24 TABLE, groups 7/140/170/63/64/283 and border lineweight/visibility groups 274–279/284–289 have bounded parser/writer/readback mappings using ODA title/header/data order and the exact 27-value AcDb::LineWeight domain; Autodesk documents repeats but not their order, so retain physical DXF order evidence for broad producer/native claims. Keep group 141 as physical row height. General per-row border CMC is output-refused because four border categories have one DXF entity-level value across three independent DWG row values; see the representability audit. | P4 closure where needed; ODA §§20.4.96.1, 20.4.103, and 2.13; source-bound spans/roles plus native semantic evidence for promotion. |
| P6 | OPEN — version, schema and semantic residuals | Resolve only evidence-backed source gaps: AC1021/24/27/32 IMAGEDEF parent-role conflict (source capture now retained as unqualified; DXF output still refused); raw-only TABLECONTENT recovery covers AC1018 and AC1021/24/27/32; AC1018 TABLEGEOMETRY and DATATABLE out-of-version/unknown-body occurrences retain raw-only; PERSUBENTMGR: AC1024 remains raw-only/Partial. AC1027 and AC1032 now admit only the exact source-bound class-version-2 prefix [2,0,2,0,0] when exactly two body bits remain; publish those five scalar fields as Partial, leave the two-bit BL-zero tail unconsumed and opaque, and retain the raw payload as unvalidated. This does not close manager semantics, graph closure, or output. AC1032 TextObjectContextData is no longer queued as an implementation task: current-source readback confirms its bounded raw fallback contributes to 842/842 published frames with zero entity/object parse failures; typed text semantics and strict replay remain open ([queue recheck](docs/research/p6-ac1032-text-context-data-queue-recheck-2026-11-09.json)). The ACadSharp AC1018 sample now publishes 1014/1014 frames with zero entity/object parse failures after six TABLECONTENT/TABLEGEOMETRY/DATATABLE occurrences were retained raw-only; focused controls also keep a one-bit-short common shell failed, recover the next valid frame, and pass corrected-input retry. Strict output remains refused by the independent IMAGEDEF/reactor source-correlation gate. See receipts [`p6-ac1018-tablecontent-tablegeometry-raw-only-shell-recovery-2026-11-09`](docs/research/p6-ac1018-tablecontent-tablegeometry-raw-only-shell-recovery-2026-11-09.json) and [`p6-ac1018-datatable-raw-only-shell-recovery-2026-11-09`](docs/research/p6-ac1018-datatable-raw-only-shell-recovery-2026-11-09.json). AC1024 PERSUBENTMGR handle 0xD63 also recovers raw-only after independent shell validation (receipt [`p6-ac1024-persubentmgr-raw-only-shell-recovery-2026-11-09`](docs/research/p6-ac1024-persubentmgr-raw-only-shell-recovery-2026-11-09.json)); its source reaches 923/923 frames with zero entity/object parse failures, while strict output remains refused by standalone OBJECTCONTEXTDATA. Keep manager body semantics/graph/output unresolved. `DRW_Block_Record::parseDwg` validates and records the ODA §20.4.52 BLOCK_HEADER body-handle roles and exact ordered spans for AC1015 first/last entity, AC1018 owned-entity, and the shared block/insert/layout references; generated wrong-role and truncation controls prove reset and retry. A hash-bound source scan now compares all 208 body-handle fields in selected AC1015 and AC1018 ACadSharp samples against LibreDWG 0.14: legacy first/last, a 163-entry owned list, two-entry paper space, and ten-INSERT lists match exact bit positions, widths, role codes, and references. A narrow AC1018 `*Paper_Space` graph (the physical-span-selected handle `0x58`) is closed across BLOCK_CONTROL membership, BLOCK/ENDBLK ownership, both owned-entity backlinks, and LAYOUT reciprocity; this does not close `my_block`, whose ten-INSERT list contains six unresolved peer handles (`0xD17`, `0xD18`, `0xD40`, `0xD41`, `0xD95`, `0xD96`). Keep that row physical-span-only. A source-backed XREF writer gap is now closed for the generated AC1015+ profile: bounded unique `insertHandles` are checked against emitted INSERT callbacks before deferred BLOCK_HEADER publication; ODA §20.4.52 places the counted INSERT list in R2000+ records alongside AC1015 legacy first/last entity references. Generated loaded/unloaded reference and overlay cases verify exact readback, mismatch refusal with destination preservation, and same-facade corrected retry across AC1015/18/21/24/27/32. LibreDWG 0.14 independently reads generated AC1015 and AC1018 witnesses; its AC1015 JSON reports `GENERATED_XREF` inserts `[0xA123]` and an INSERT with handle `0xA123` targeting BLOCK_HEADER `0x30`. These remain generated interoperability checks, not native source-producer qualification. This does not qualify native/imported source XREF graphs or general XREF support; no DWG/DXF fixtures were committed ([receipt](docs/research/p6-dwg-block-record-xref-insert-membership-2026-11-09.json)). Producer release provenance is uncertified, the peer/production name mismatch (`*Paper_Space` vs `*Paper_Space0`) remains unresolved, and strict output remains open ([generated receipt](docs/research/p6-ac1015plus-block-header-handle-roles-spans-2026-11-09.json), [physical peer receipt](docs/research/p6-ac1015plus-block-header-physical-peer-spans-2026-11-09.json), [selected graph receipt](docs/research/p6-ac1018-paper-space-block-header-graph-closure-2026-11-09.json)). Other gaps: unclaimed AC1027/AC1032 HEADER suffixes; remaining version boundaries, geometry/no-loss, HEADER/CLI, parser disposition and caller-state contracts. The minimal generated AC1021 ODA matrix passes only when both default-layer graph opt-ins are enabled; the AC1021/24/27 fast material control now checks both output layer-0 references against planned targets. This is bounded writer evidence, not a broad support claim. Preserve strict refusal for unsupported or unclaimed bytes. | ODA v5.4.1 plus AutoCAD DWG/DXF source pairs; focused fast controls. Owner-semantic promotion still needs independent native rewrite or normative resolution. Suffix work requires a materially changed converter route. See the [IMAGEDEF role receipt](docs/research/p6-imagedef-cross-version-soft-role-evidence-2026-11-07.json) and [AC1021 writer probe receipt](docs/research/p6-ac1021-minimal-writer-oda-gui-rejection-2026-11-07.json). |
| P7 | OPEN — ACIS/ACDS and independent 3D meaning | Complete entity/owner/key/AcDs/SAT-SAB association, versioned framing/index/suffix/prototype assembly and exact output binding; qualify transforms, units, topology, trims, periodicity and spline meaning with an independent kernel. The pinned ACadRust 0.6.8 screen of the original SAT 21500 payload still reports an unresolved pointer and its accessor uses the wrong torus token offset for that modified profile; do not use that result as semantic evidence. A distinct ODA-produced ACAD2007/2010 route for the same source 3DSOLID `0x452` yields recoverable SAT 21200: pinned ACadRust validates 13 records with zero errors and its torus center, axis, radii and U direction exactly match the source-derived SAT. Native FreeCAD/OpenCASCADE reconstructs the same valid solid and matches bounds, area, volume and centroid to the source-derived route and analytic torus formulas. This resolves a bounded external semantic cross-check for the selected torus only. ACadRust is not a B-rep kernel, and InventorLoader and FreeCAD Part share OpenCASCADE. The pinned OpenCADStudio `ocs_acis` route now reads the exact source-derived inline SAB and its ODA SAT projection with ACadRust and lifts both into the non-OCCT OpenCADKernel model. Both produce one torus face with no loops, edges or vertices; mesh-based mass properties return volume 2919.3521093 and a matching centroid, within 2.76e-7 relative error of the analytic torus volume. The SAB header declares two bodies while the lift returns one. A separate OpenCADKernel constructor from the same source-bound fields validates a complete one-face/two-edge/one-vertex placed torus and its analytic bounds. These are bounded scalar/mesh and kernel-constructor checks, not complete source-carrier topology or full B-rep conversion; the FreeCAD/OpenCADKernel bounds discrepancy remains unresolved. Keep body-count and boundary-topology closure, native modeler acceptance, broader ACDS graph, other profiles and whole-source gates open (see the [independent-kernel receipt](docs/research/p7-opencadkernel-independent-torus-2026-11-10.json)). The 2026-11-09 screen found no eligible independent route at that time; the 2026-11-10 OpenCADKernel probe is a new bounded result, not a general qualification. The prior FreeCAD sandbox startup failure is superseded for the CLI route: the exact selected torus was reconstructed successfully with native FreeCAD 1.1.4 Part/OpenCASCADE outside the sandbox (see the native B-rep receipt below). The separate OpenCADKernel check supplies bounded independent geometry evidence, but it does not qualify direct SAT-to-B-rep topology, native source acceptance, GUI import, Linux/Windows execution, or support promotion. Keep those gates open; the next kernel action is to exercise a source carrier with complete boundary topology and resolve the recorded bounds discrepancy. The earlier candidate screen remains at [its receipt](docs/research/p7-independent-sat-brep-kernel-open-source-screen-2026-11-09.json). See the [ACadRust screen](docs/research/p7-acadrust-pinned-exact-sat-torus-screen-2026-11-09.json), [ODA conversion](docs/research/p7-oda-ac1021-dwg-to-dxf-opaque-solid-2026-11-09.json), and [semantic cross-check receipt](docs/research/p7-oda-ac1021-torus-semantic-crosscheck-2026-11-09.json). The reusable `tools/research/decode_dxf_sat1.py` now extracts one exact-handle SAT v1 carrier from ASCII DXF, joins group 1/3 chunks, reverses the evidenced SAT1 transform and caret escape, and rejects malformed or ambiguous selection. In-memory and CLI checks cover transform, chunking, exact header/end marker, malformed continuation and trailing-garbage refusal, and overwrite refusal; no drawing fixtures were added. It is a research aid only and closes no P7 graph, geometry, or support gate ([receipt](docs/research/p7-dxf-sat1-recovery-tool-2026-11-09.json)). | P4 graph/output bindings and independent semantic/kernel evidence. |
| P8 | OPEN — consumers, platforms and release | The isolated LibreCAD `origin/master` adapter bridge and its generated import/re-export checks remain implemented. A focused runtime contract now confirms `DRW_Interface::addTable` still reaches an insert-only consumer through `addInsert`, including for a semantically incomplete TABLE prefix, and that its proxy-graphics bytes remain attached as a preserved carrier; this callback check does not qualify native rendering or broaden support ([receipt](docs/research/p8-insert-only-table-callback-compat-2026-11-09.json)). LibreCAD_3 PR #456 is open on `codex/libdxfrw-upstream-compat`, based on `5cf435f4`; code head `762a0f1e` passes the focused 64/64 export/round-trip cohort. Follow-up `fd5af0b8` raises the temporary Windows package-layout CMake timeout after a hosted timeout; its smoke suite passes 19/19 locally on macOS. PR head `7e81275b71038d4a42fa580842e661ddcee7adbf` adds the Homebrew metadata refresh fix; isolated local candidate `83f36931` adds the Mach-O-based provenance repair and passes focused local checks, but has no hosted CI result and is not published. Automatic review rejected uploading the candidate plan file through the alternate route; do not publish this candidate through another route. Homebrew now officially classifies Intel x86_64 as Tier 3 and has stopped building new bottles; MacPorts is suggested but its Intel Tahoe binary set is explicitly incomplete. Do not treat the current Intel force-bottle job as a sustainable provider. Keep hosted qualification open. Remaining: authorize and publish the arm64 provenance repair, pass hosted arm64 CI, decide whether Intel remains a release target and either qualify a complete maintained binary provider or retire that job/target explicitly, clean-consumer reproducibility/ABI, LibreCAD and LibreCAD_3 2D behavior across supported builds, native Linux/Windows FreeCAD open/insert checks, and 3D consumer/package/release qualification. These checks do not change DWG/DXF support claims. | Stable candidate plus native platform access and reproducible consumer build receipts; preserve existing 2D behavior and refuse unresolved references. |
| P9 | DEFERRED_EXTERNAL — six non-promoting items | Resolve J256, J260, J268, J284, J293 and J295 only through the successor registry and eligible independent evidence. J268 still needs original input identity/digest; J284 has 11 target-debt/integrity blockers; J295 needs native receipts for four RTEXT position/rotation claims. None can promote support claims alone. | `metadata/external-evidence-resolution-v1.json`; retain lineage and exclusions. |

**AC1024 CLASSES high-bit string-size discrepancy:** the 2026-11-09 current-
binary recheck confirms `operationSucceeded=true`, `legacyError=0` and no
CLASSES CRC mismatch for all nine AC1024 DWGs currently under
`/Users/dli/doc/dwg/`; all ten repository fixture paths also pass, with the
`mechanical_example-imperial-alt.dwg` path duplicating an existing source hash.
Some files still fail later strict-output gates, which are not CLASSES errors.
This conflicts with the repo guidance's six-of-ten AutoCAD 2010 RTM report;
the referenced directory currently contains only nine AC1024 paths. Keep the
reported failure unresolved until the missing tenth input or exact failing
hashes are identified and `BAD_READ_CLASSES` is reproduced. Do not change the
parser or commit fixtures from this evidence. The bounded high-bit footer
decoder and extended-prefix gate remain covered by the generated
`libdxfrw_dwg_reader_matrix_tests --r2007-class-string-footer` control. That
selector now also generates a complete AC1024 file in memory with three
2048-character class strings, verifies the footer exceeds `0x7fff` bits, and
reads all strings back through `readDwgClasses()`. No fixture was added. See the
[2026-11-09 repository-corpus receipt](docs/research/p6-ac1024-classes-repository-corpus-recheck-2026-11-09.json)
and [earlier recheck](docs/research/p6-ac1024-classes-high-bit-rtm-recheck-2026-11-08.json).
A further screen of the public LibreDWG `test/test-data/2010` directory found
18 AC1024 DWGs: all reach the CLASSES end marker without a CRC mismatch, and 17
complete the full reader operation. One fails later at TABLES; the successful
reads then fail only at unrelated strict export gates. This directory has no
per-file AutoCAD 2010 RTM attribution, so it adds AC1024 inputs but does not
resolve the reported producer-specific failure. See the [LibreDWG 2010 corpus
screen](docs/research/p6-ac1024-libredwg-2010-corpus-screen-2026-11-09.json);
no drawings were copied into the repository.

**Online AC1024 sample checked:** `nextgis/dwg_samples/arc_2010.dwg` was
retrieved to `/private/tmp`, verified as AC1024 (51,549 bytes; SHA-256
`fd87187b435062cb15e0f6a88c2b23546768162f54ffae83a3f51788503ec262`), and
read through `dwgReader24::readDwgClasses END` with `operationSucceeded=true`,
`legacyError=0`, and no CLASSES CRC mismatch. The repository identifies the
sample set as produced with trial AutoCAD 2016 M.49.0.0, so this is an
independent AC1024-format input but not evidence for AutoCAD 2010 RTM saves.
ODA File Converter successfully generated a 195,074-byte 2010 ASCII DXF pair
(SHA-256 `88b094c08ba6361b3d4caf93223f7d14672a001a78366295d453ba42119b44e0`).
The current reader publishes 181/181 frames with zero entity/object parse
failures. Its validated HEADER compatibility profile has a four-byte
canonical-size discrepancy (830 expected, 834 observed) and is warning-only;
it is not the strict-output blocker. Strict libdxfrw output is correctly
refused for null members in LTYPE_CONTROL `0x5` and BLOCK_CONTROL `0x1`. A
2026-11-09 public-source search found Autodesk's sample collection labeled
for AutoCAD 2010 and later, but it gives no per-file originating release; the
`nextgis/dwg_samples` corpus explicitly says AutoCAD 2016 M.49.0.0. Neither is
an AutoCAD 2010 RTM provenance witness. The receipts cover the online Autodesk
and LibreDWG AC1024 corpus screens; do not repeat broad downloads or screens
without a new source or producer lead. A retrieval through the contemporaneous
2009 Autodesk sample URL was screened on 2026-11-09: its metric DWG is AC1021
and its SHA-256 exactly duplicates the already screened local sample, so it is
not an AC1024/RTM witness ([receipt](docs/research/p6-ac1024-2009-autodesk-mirror-sample-screen-2026-11-09.json)).
The missing tenth input, an exact failing hash, or an independently verified
AutoCAD 2010 RTM witness is still required before changing the parser. Do not
promote support from version code alone. See the
[online sample receipt](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json). The
DWG and paired DXF remain in `/private/tmp` and are not committed. Keep the RTM
discrepancy open and preserve strict refusal; no parser change is justified by
this source. See the
[online-sample receipt](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json)
and the public [`nextgis/dwg_samples` source](https://github.com/nextgis/dwg_samples).

**Expanded external AC1024 sample screen (2026-11-09):** downloaded 18 DWGs
from LibreDWG's public `test/test-data/2010` directory and all 18 DWGs in
Autodesk's historically indexed sample collection to `/private/tmp`. The
LibreDWG directory yielded 18 AC1024 files; the Autodesk collection yielded
10 AC1024 files and 8 AC1021 files. All 28 AC1024 files reach
`dwgReader24::readDwgClasses END` with no CLASSES CRC mismatch under the
current `build-gcc/dwg2dxf/dwg2dxf` binary. Nine Autodesk conversions and all
18 LibreDWG-corpus conversions stop later at output or completeness gates;
those outcomes are not CLASSES failures. The contemporaneous Autodesk listing
calls its files AutoCAD 2010 samples, but selected embedded metadata identifies
AutoCAD 2009/Gator Beta 2 (`D.40.0.200`, registry `18.0`); the other files
lack per-file producer-build evidence. The LibreDWG directory label also does
not establish an RTM producer. This screen neither reproduces nor disproves
the report about six of ten RTM saves. Keep the issue open pending the exact
failing hash or verifiable RTM provenance; do not change the parser or commit
external fixtures. See the [external sample screen receipt](docs/research/p6-ac1024-external-sample-classes-screen-2026-11-09.json).

**AC1015 LTYPE_CONTROL outlier:** the unauthenticated local `simple.dwg` still
fails at the bounded count/boundary check on current code. LibreDWG 0.14 parses
25 entries, but its decoded profile includes BYBLOCK/BYLAYER in the counted
entries, duplicates them in separate special-handle fields, and reports soft
pointers where ODA v5.4.1 §20.4.57 specifies soft-owner counted entries and
hard-owner BYLAYER/BYBLOCK references. The reported 520-bit size also equals
the 65-byte frame although ODA says R2000 object size excludes end handles.
Keep strict refusal; peer tolerance does not establish a valid wire profile or
justify parser relaxation. Reconsider only with producer provenance or an
independent spec-conforming witness ([updated receipt](docs/research/p6-ac1015-simple-ltype-control-outlier-2026-11-06.json)).

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
controls pass. The physical AC1024 version-2 no-tail profile now parses only
when its bounded body ends exactly after the mesh; the existing present-tail
profile remains accepted. The selected source has 157/157 frames published
with zero entity/object parse failures, but strict output still refuses on its
unclaimed 739-byte HEADER suffix. This does not choose the general CIVIL-tail
rule, change writer output, or qualify geospatial meaning; see the [reader
follow-up](docs/research/p4-ac1024-geodata-v2-civil-tail-conflict-2026-11-04.json).
Physical whole-document and native geospatial qualification remain open
([coordinate-type receipt](docs/research/p4-geodata-coordinate-type-dxf-domain-2026-11-05.json)).

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
This is not direct or whole-source output from the physical drawing. The earlier
AC1021-to-AC1024 whole-source attempt stopped at the deliberate ModelDocViewStyle
exact-version guard; a same-version AC1021 recheck passes that guard and reaches
the distinct DBCOLOR source/output evidence gate. The 1,673 observed frames have
zero entity/object parse failures, and no DXF is created. The 186 observer frames
are 74 opaque carriers plus 112 standard-object frames; all 112 standard payloads
match typed callbacks. Bounded selected-graph routes now qualify all 74 opaque
carriers and all 112 standard-object frames for this source, with physical
ASCII/Binary readback and applicable atomic refusal/retry. The final three
standard-frame proofs close the physical AC1021 TABLESTYLE/Recompose Data graph:
DICTIONARY `0x7E`, extension DICTIONARY `0x162`, and XRECORD `0x3C3F8`. The
XRECORD source frame (offset 7794, size 34) verifies scalar group 90 and the
ordered group-330 handle to `TABLESTYLE:Standard`; the DICTIONARY proofs close
the NOD root, TABLESTYLE owner and CELLSTYLEMAP extension edge. The final census
is 112/112 ([receipt](docs/research/p4-ac1021-recompose-data-physical-selected-output-2026-11-06.json),
[inventory](docs/research/p4-ac1021-opaque-frame-inventory-2026-11-06.json)).
Selected ASCII/Binary readback verifies remapped references and layer
extension-dictionary ownership; late reactor mutation preserves the destination,
and the same adapter succeeds after restoration.
The physical AC1021 `AcDsDecomposeData` graph now also passes selected ASCII/Binary
output and fresh readback across its NOD root, two DICTIONARY levels and five
XRECORDs. It checks all four remapped group-341 references, exact ordered record
payloads, reactor-group representation, mutation refusal, destination
preservation and same-adapter retry ([receipt](docs/research/p4-ac1021-acds-decompose-data-physical-selected-output-2026-11-06.json)).
The generic root route is limited to a single NOD edge targeting one unique,
complete, NOD-owned DICTIONARY and uses the existing graph closure validation.
These remain local selected-source profiles and do not promote DWG support
claims. The two-frame AC1021 `Siberia` NOD graph (`0x38F4B` → `Styles` → empty
dictionary `0x38F4C`) also passes selected ASCII/Binary readback, owner/reactor
and 350/360 role checks, late mutation refusal, destination preservation and
same-adapter retry ([receipt](docs/research/p4-ac1021-siberia-dictionary-physical-selected-output-2026-11-06.json)).
The one-root empty `ACAD_LAYERSTATES` graph on `LAYER_CONTROL` (dictionary
`0x1FF` → `0x200`) now passes source admission, selected ASCII/Binary output,
fresh typed readback, late mutation refusal, destination preservation and
same-adapter retry ([receipt](docs/research/p4-ac1021-layerstates-one-root-physical-selected-output-2026-11-06.json)).
This test-only projection removes unrelated object families so it can reach the
selected writer; that pruning is not evidence for whole-source conversion.
The neighboring AC1024 one-root empty-LayerStates compatibility control now
passes again. Its last unqualified callback was DICTIONARY `0x104`, owned by
TABLESTYLE `0x87`; that table style receives a minted output identity without a
generic object binding. The standard-object verifier now accepts this owner
mapping only after TABLESTYLE source/output inspection, unchanged-source proof,
and unique staged TABLESTYLE identity. ASCII/Binary refusal, sentinel
preservation, same-adapter retry and fresh readback pass
([receipt](docs/research/p4-ac1024-table-style-owned-dictionary-identity-bridge-2026-11-06.json)).
This closes only the exact current AC1024 witness/control, not broader
TABLESTYLE ownership profiles.
The finalized selected-output census confirms all 112/112 standard-object
frames and all 74/74 opaque carrier records for the AC1021 children-room
source, including its LAYER extension dictionaries, VIEWPORT annotation-scale
XRECORDs, TABLESTYLE/Recompose Data graph, and LAYOUT extension records. No
standard-frame or opaque-carrier residual remains for this source. The
same-version whole-source recheck still stops at independent DBCOLOR and other
document-wide gates. See the current [frame inventory](docs/research/p4-ac1021-opaque-frame-inventory-2026-11-06.json),
[same-version gate recheck](docs/research/p4-ac1021-children-room-same-version-output-gate-2026-11-05.json),
[VisualStyle graph receipt](docs/research/p4-ac1021-visualstyle-extension-dictionary-xrecord-output-2026-11-06.json),
[Pool_Detail VISUALSTYLE receipt](docs/research/p4-ac1021-pool-detail-visualstyle-selected-output-2026-11-07.json),
[Pool_Detail MATERIAL receipt](docs/research/p4-ac1021-pool-detail-material-selected-output-2026-11-07.json),
[SCALE graph receipt](docs/research/p4-ac1021-scale-physical-selected-output-2026-11-06.json), and
[DICTIONARYVAR graph receipt](docs/research/p4-ac1021-dictionaryvar-physical-selected-output-2026-11-06.json).
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
The 2026-11-07 recheck found six named-block SORTENTSTABLE callbacks in the
AC1024 architectural sample. A temporary output-qualification relaxation
passed libdxfrw's own ASCII/Binary round trip but was reverted after the
Autodesk group-330 rule was rechecked; current strict conversion remains refused
at 484/490 with no DXF published. Treat that result as evidence that internal
codec consistency is insufficient, not as an output profile
([recheck receipt](docs/research/p4-ac1024-architectural-named-block-sortents-spec-boundary-2026-11-07.json)).

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

The AC1021 children-room source's bounded selected-output census is complete:
all 112/112 standard-object frames and 74/74 opaque carrier records have
selected-output proof for that exact local source. The six previously listed
NOD/DICTIONARY/XRECORD slices are therefore removed from the open queue. This
does not close populated GROUP membership, whole-source conversion, native
semantics, producer-wide coverage, or named-block SORTENTSTABLE ownership; see
the current [inventory receipt](docs/research/p4-ac1021-opaque-frame-inventory-2026-11-06.json).

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
The current AC1021 fixture recheck confirms why this is not a one-condition
output fix: all 26 HANDLE-map frames publish with zero parse failures, but the
integrity ledger has seven Error rows—four null current HEADER references and
role errors for LTYPE_CONTROL, the LAYER-to-LTYPE pointer, and BLOCK_CONTROL—plus
nine checksum warnings. A temporary local probe showed that matching the exact
legacy BYBLOCK/BYLAYER/CONTINUOUS LTYPE order satisfies the membership predicate
alone; strict conversion still refuses on the independent source errors, and
that probe was reverted. Keep output refused until each source error has a
format-valid, lossless projection; do not whitelist the LTYPE member order by
itself. See the [strict-refusal recheck](docs/research/p4-libreCAD-ac1021-strict-refusal-recheck-2026-11-07.json).
Keep the selected one-layer graph's output
qualification open for physical source conversion: generated entity and layer
LTYPE references now bind to same-import rows, but no whole-source producer
conversion has yet reached this output gate. Keep dangling and wrong-kind
source cases refused.

**AC1015 `entities-3d.dwg` STYLE blocker:** TABLES diagnostics now identify the
exact failed table. This candidate reads 75/75 frames, but its STYLE table has
an empty-name shape STYLE at handle `0x38` (`es.shx`) referenced by SHAPE `0x39`;
it cannot be dropped as unreferenced. LibreDWG 0.14 emits this SHAPE with empty
style and shape-name groups while omitting the STYLE row. Autodesk's STYLE and
SHAPE references document the relevant fields, but do not settle this empty-name
profile's validity or rendering semantics. Keep strict output refused; do not
invent a name, discard a referenced STYLE, or treat peer output as semantic
proof. Resolve with a producer-backed source and an independent native consumer;
only then bind the exact source identities, output references and readback and
add a scoped serializer regression. The post-unlock ODA 27.1 GUI audit pass
produced an AC1024 DXF without an `.err` report and retained the empty-name
STYLE row, but omitted SHAPE `0x39`; this does not establish lossless native
acceptance or the pair's semantics. Keep this candidate blocked and do not
repeat the unchanged conversion. See the [ODA receipt](docs/research/p4-ac1015-entities3d-style-empty-name-oda-gui-2026-11-07.json)
and [focused blocker receipt](docs/research/p4-ac1015-entities3d-style-empty-name-output-blocker-2026-11-07.json).
Source provenance is unverified, so neither the DWG nor peer/ODA DXF is a
fixture.
The existing generated TABLES inspection selector now also exercises STYLE
stage diagnostics in ASCII/Binary, destination preservation, and same-codec
retry; this verifies the diagnostic path without making the physical profile
eligible.

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
   The AC1024 inline constraint-node dependency audit is already implemented;
   do not reopen that parser/audit task from the earlier 2026-10-30 receipt.
   Across the two identified physical sources, its local audit verifies all
   29/29 and 31/31 non-null geometry-dependency references, dependency object
   kinds, same-group ownership/action membership, dependent-on entity targets,
   owner routes, and reactor backlinks. The AC1024 civil source additionally
   verifies 18 non-null readdep/node links, including 16 same-group reciprocal
   dependency edges and two entity targets. LibreDWG 0.14 independently
   confirms the common dependency objects and links, but does not decode the
   inline node bodies. The original receipt closes only source-link accounting.
   The AC1024 mechanical witness now also has a separate source-bound
   selected-output proof for 17 dependency/body pairs and all 399 constraint
   nodes, including ASCII/Binary typed readback, refusal, and same-adapter retry;
   this is not native constraint behavior or a whole-document claim ([receipt](docs/research/p4-ac1024-assoc-dependency-physical-selected-output-2026-11-07.json)).
   The civil witness is not ready for the same 17 dimension-dependency route: it
   has 17 generic dependencies, 29 geometry dependencies, 13 dimension-body
   records, and three groups. The AC1024 Edge source profile now closes ten
   geometry-dependency bodies and their two target polyline vertex cycles. One
   distinct geometry dependency (handle `0xC55`) remains wire-incomplete; strict
   whole-source conversion also remains blocked by the separate null
   LTYPE_CONTROL member. Keep both refusals. A diagnostic profile comparison
   confirms the mechanical source has
   54 parsed and wire-complete geometry-dependency bodies named
   `AcDbAssocSingleEdgePersSubentId`; this does not prove their exact physical
   spans independently, does not extend its selected route beyond the 17
   generic dependency/body pairs, and does not establish a general
   `AcDbAssocEdgePersSubentId` profile. Add output only for the ten source-bound
   civil records after their graph membership and identity checks pass; keep
   handle `0xC55` excluded. Do not broaden mechanical output or support claims.
   See the [readiness
   receipt](docs/research/p4-ac1024-civil-assoc-output-readiness-2026-11-07.json)
   and [mechanical/civil wire-profile comparison](docs/research/p4-ac1024-mechanical-civil-assoc-wire-profile-comparison-2026-11-07.json).
   Neither source profile establishes native constraint behavior. See also the
   [dependency-link receipt](docs/research/p4-ac1024-assoc-node-dependency-source-links-2026-10-30.json)
   and [generated graph output control](tests/CMakeLists.txt).
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
   `ACAD_COLOR` root. It is not independent source evidence, and the generated AC1027 route remains refused. The existing AC1021 color-book witness now passes a bounded production-adapter selected graph: one NOD `ACAD_COLOR` edge, its one-entry owner dictionary, one DBCOLOR record keyed by `book$name`, the bound ACAD APPID row, and all 13 original EED tuples survive same-version ASCII/Binary output and fresh readback. Late color/dictionary mutation and a wrong dictionary key refuse atomically; the same adapter succeeds after restoration. This does not qualify whole-source output or other producer/version profiles ([selected-output receipt](docs/research/p4-ac1021-dbcolor-physical-selected-output-2026-11-06.json)).
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
   assume those conversions preserve the original graph. The low-level DXF codec retains an explicitly present ACI fallback alongside RGB; this selected source has no explicit group-62 fallback, so the route preserves the typed RGB/book/name and EED without deriving ACI from CMC or an EED integer. DWG-only CMC metadata normalizes in DXF. Native color-book meaning, fallback derivation for other inputs, producer prevalence, and complete-document closure remain open. See the [physical graph receipt](docs/research/p4-dbcolor-ac1021-physical-oda-graph-and-precedence-2026-11-03.json) and [selected-output receipt](docs/research/p4-ac1021-dbcolor-physical-selected-output-2026-11-06.json).
   A bounded optional side channel preserves the source APPID/LAYER handle roles
   before name resolution. The physical witness passes source-dependency checks and selected ASCII/Binary adapter readback with all 13 original EED tuples; whole-drawing target closure, fallback rules for other profiles, and native behavior remain open. See the [dependency receipt](docs/research/p4-dbcolor-eed-dependencies-selected-graph-2026-11-03.json) and [selected-output receipt](docs/research/p4-ac1021-dbcolor-physical-selected-output-2026-11-06.json).
   ODA counterexamples show the apparent match between an EED integer and
   emitted ACI 254 is incidental; do not infer the fallback from that value.
   Simple nearest-RGB calculations using two published palettes also disagree
   with multiple ODA outputs. Determine the actual mapping before synthesizing
   a fallback; see the [EED differential](docs/research/p4-dbcolor-oda-fallback-eed-counterexamples-2026-11-03.json)
   and [palette differential](docs/research/p4-dbcolor-oda-palette-differential-2026-11-03.json).
   The low-level DBCOLOR writer now rejects a present group-62 ACI above 256
   before output, while retaining tolerant input and same-object retry. ASCII/
   Binary checks cover 0, 254, 256, 257 and 65535; this closes the modeled
   output-domain gap only and does not infer the DWG CMC fallback
   ([receipt](docs/research/p4-dbcolor-dxf-aci-domain-guard-2026-11-05.json)).
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
   The generated positive adapter route is now verified in ASCII and Binary:
   one named target BLOCK_RECORD, its BLOCK/ENDBLK identities, and its BLKREFS
   INSERT survive fresh readback with exact remapped identity; wrong-owner
   output preserves the destination, and same-adapter correction/retry passes
   ([generated output receipt](docs/research/p4-blockrecord-blkrefs-generated-adapter-readback-2026-11-06.json)).
   This still does not prove selected physical-source output, source-bound complete
   membership, producer breadth or native meaning. Autodesk's BLOCK_RECORD field
   table does not enumerate BLKREFS, and generic group-331 classification is not
   evidence of required membership semantics. Keep whole-document output
   unqualified until those gates close.
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

A historical FreeCAD 1.1.4 native C++ importer run opened the AC1024
mechanical-alt DXF produced by converter SHA-256
`9b757361e17985fac4c3f4d0755b1163c9f6c3bf2c5a23d130fe0872e4c92c0d`: 96 entities
were read and 75 FreeCAD objects created. Two VIEWPORT records and two aligned
dimensions were unsupported; text/dimension and layout import were disabled.
That run used an older converter hash. The current build now completes strict
conversion after the bounded C278 profile correction, but the FreeCAD handoff
has not been repeated against that current output. The source-bound AC1024
empty-root LayerStates projection below is independently covered by its focused
ASCII/Binary output/readback selector; it does not establish native LayerStates
behavior. No current FreeCAD source-to-object comparison or broad platform gate
is closed ([historical FreeCAD receipt](docs/research/p4-ac1024-mechanical-alt-freecad-readback-2026-11-05.json),
[current recheck](docs/research/p4-ac1024-current-peer-recheck-and-ac1027-enc-candidate-screen-2026-11-05.json)).

**AC1024 empty LayerStates root:** `mechanical_example-imperial-alt.dwg` has a
source-bound LAYER_CONTROL extension dictionary with exactly one
`ACAD_LAYERSTATES` entry and an empty child dictionary. The adapter now admits
this exact one-root profile while retaining unique type-42 source-frame,
owner/reactor, unchanged-snapshot, and emitted-body checks. A focused selector
passes ASCII/Binary output, wrong-owner atomic refusal, same-writer correction
retry, and fresh typed readback. The current strict CLI now succeeds after the
bounded C278 profile correction, but the old FreeCAD import used an earlier
converter build and has not been repeated. This does not broaden the separate
three-root/XRECORD profile: its source-projected ASCII/Binary production output and readback
selector passes after the test projection was updated to retain the required
LTYPE_CONTROL membership receipt. Strict whole-file conversion of its aerial
witness remains refused by the SUN owner graph. Keep other LayerStates profiles
and native restoration/evaluation open ([receipt](docs/research/p4-ac1024-empty-layerstates-root-output-2026-11-05.json)).

After actual prerequisites close, run one composed strict CLI checkpoint. A
successful process exit or record count alone is not closure. For AC1018 opt-in
capture, keep default legacy consumers opted out until codec, source, dependency and
output gates are met.

## P5 — rendering and TABLE

### AC1018 TABLESTYLE CMC method `0xC8` replay (2026-11-10)

An ODA-converted AC1018 TABLE sample has `0xC8` CMC fill colors in all three
legacy TABLESTYLE rows. The bounded TABLESTYLE parser now retains the complete
packed 32-bit color word for this method, and the DWG writer replays it only
when the retained method byte, scalar fallback, reserved index, flags, and
color-book strings are consistent. The low 24 bits reuse the existing
`DRW_TableStyleCmcColor::m_rgb` storage only while `m_method == 0xC8`; no public
struct field was added, preserving its layout. The value remains opaque:
ODA v5.4.1 §2.11 specifies the CMC slots but does not assign `0xC8` semantics.
DXF output continues to use the existing no-color sentinel and omits group 420
for this method, so the retained opaque payload is not mis-emitted as RGB.
Generated AC1018 TABLESTYLE whole-file readback exercises method/payload
retention and refuses absent or mismatched opaque payloads; the paired focused
TABLESTYLE/TABLE tests pass. This closes only the bounded CMC carrier gap. It
does not qualify the physical source's reactor/extension-dictionary graph,
native rendering, other producer profiles, or TABLE support claims. See the
[CMC replay receipt](docs/research/p5-ac1018-tablestyle-cmc-none-opaque-replay-2026-11-10.json).

**R2010+ TABLE unknown-prefix admission:** `DRW_Table::parseDwg` now keeps
the entity frame readable but withholds semantic completeness when the
ODA v5.4.1 §20.4.96.2 unknown preamble differs from its documented defaults:
the RC/BL fields are nonzero, the unknown soft pointer is non-null, the R2010
Boolean is false, or the R2013+ BL is nonzero. The non-default pointer is no longer used as a
TABLESTYLE identity; only the TABLECONTENT style reference populates that
field. Generated AC1024 controls cover the R2010 fields; AC1027 and AC1032
controls exercise the non-default R2013+ BL refusal and documented-default
typed-content path. The H field also uses the existing bounded soft-pointer
decoder; a generated hard-pointer-role mutation fails closed. This is conservative parser
disposition, not evidence for non-default field meaning, independent raw-frame
replay, or native TABLE semantics. See the [focused receipt](docs/research/p5-ac1024-table-unknown-prefix-default-gate-2026-11-09.json).

The generated nested TABLESTYLE raw-net control now models ordinary and covered
cells with explicit unit spans and uses the output's remapped raw-object handles
for identity and order assertions. Its focused ASCII/Binary selector passes;
this fixes a malformed test control and stale source-handle expectations, not a
production codec defect. The adjacent AC1024 identity and AC1032 unit-domain
selectors now pass: the generated repeated-style index matches its style ID,
and DWG decoding accepts only the ODA §20.4.99 wire values 0/1/2/4/8. Autodesk's
broader UnitType API values remain supported in the DXF/API value domain, but
are not admitted as DWG wire values without a producer witness. The existing
AC1024 producer-observed class 0 remains opaque. After the legacy-row reader
adopted the same wire-domain validator, the existing AC1021 `colorwh.dwg`
source still completes strict conversion and a fresh same-version DXF read.
That is regression evidence for one local profile, not field-semantic equality
or producer qualification. Physical producer breadth and the rest of P5 remain
open ([domain receipt](docs/research/p5-tablestyle-dwg-unit-and-identity-domains-2026-11-07.json), [raw-net receipt](docs/research/p5-tablestyle-nested-raw-net-remapped-identity-fast-slice-2026-11-07.json)).

**Physical TABLE dimensions (group 141/142):** Autodesk defines one repeated
group-141 row height per row and one repeated group-142 column width per
column. The parser retains these values in the existing bounded row/column
records even when the group codes precede row/column counts, and marks missing,
excess/duplicate, contradictory-count and nonpositive values incomplete; the
DXF reader rejects non-finite spellings. Group-141 physical row heights remain
separate from group-140 title/header/data style-height overrides. Generated
ASCII/Binary controls pass through the focused legacy TABLE height selector.
The current positive-finite value rule is a conservative fail-closed
implementation constraint, not a confirmed DXF numeric-domain requirement:
Autodesk's [TABLE DXF definition](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-D8CCD2F0-18A3-42BB-A64D-539114A07DA0.htm)
documents the repetition counts but gives no numeric range, and the
[AcDbTable::setRowHeight API entry](https://help.autodesk.com/cloudhelp/2027/ENU/OARX-RefGuide/files/OARX-RefGuide-AcDbTable__setRowHeight_int_double.html)
describes the argument without specifying its accepted range. Keep the
conservative refusal until an eligible producer/native probe or authoritative
API contract establishes the boundary; do not describe zero/negative values as
spec-invalid. This remains parser-integrity/order evidence, not native producer,
rendering or broader TABLE qualification; see the [physical dimension cardinality
receipt](docs/research/p5-dxf-table-physical-dimension-cardinality-2026-11-08.json).

**TABLE border-color mask (group 94):** ODA v5.4.1 §20.4.96.1 defines 18
selected border CMC fields across title/header/data rows. Since the general DXF
profile cannot preserve arbitrary per-row border colors, the parser now retains
a nonzero group-94 mask but marks the TABLE semantically incomplete, including
unsupported high bits. Generated ASCII/Binary controls check the zero-mask
baseline and both selected/unknown-bit refusals. Group-95 lineweight and
96 visibility paths are unchanged; see the [border-color mask receipt](docs/research/p5-table-border-color-mask-incomplete-2026-11-08.json)
and the existing [CMC representability audit](docs/research/p5-ac1021-table-border-cmc-lossless-dxf-audit-2026-11-08.json).

**VIEWPORT layer-override reference (group 91):** Autodesk's VIEWPORT-specific
DXF table calls this a soft pointer, but Autodesk's generic DXF type table
classifies 90–99 as 32-bit integers, and its Binary DXF documentation applies
the same group-code typing rules to binary files. The current reader therefore
keeps group 91 rejected; do not globally reclassify it as a handle or emit it
from the existing 335/343/344 path. Resume only with an authenticated native
ASCII/Binary producer pair that establishes its physical carrier and handle
remapping behavior. Native paper-space override activation remains a separate
gate. See the [group-91 conflict receipt](docs/research/p5-viewport-group91-wire-type-conflict-2026-11-08.json).

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
The AC1032 group-94 channel mask value 63 now has a separate explicit raw-only
replay route for sparse/partially modeled MATERIAL records, including trailing
XDATA. Generated ASCII/Binary controls verify exact group retention, legacy
consumer refusal without partial callbacks, same-adapter opt-in retry,
typed-output refusal, destination preservation, opt-in raw replay and fresh readback. A local
physical AC1032 DXF now parses beyond these records but whole-file output remains
refused at two unpreserved proxy graphics (first handle 1284). This does not
qualify channel-mask semantics, typed material support, rendering, or whole-file
output; broaden producer evidence and semantic/output qualification separately
([receipt](docs/research/p5-ac1032-sparse-material-channel-mask-raw-replay-2026-11-05.json)).
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

**DWG `ACAD_TABLE` writer gap — bounded generated profile enabled 2026-11-10:**
`DRW_Table::encodeDwg` and `dwgRW::writeTable` now emit one generated AC1018
plain-text grid profile through full file finalization. The route requires
pre-freeze `AcDbTable` class registration, stages the TABLE frame and `*T1`
cached 8 LINE/7 MTEXT graph, then checks TABLE → BLOCK_RECORD and TABLESTYLE →
NOD closure before publishing. Late closure failure preserves the destination,
restores caller TABLE state, and allows a corrected same-facade retry. Fresh
libdxfrw readback validates cell semantics and cached children; LibreDWG 0.14
validates graph and cached geometry but does not decode the cell body. This is
not broad TABLE support. FIELD/BLOCK cell payloads, owned ATTRIB chains,
retained source subrecords, R24 TABLECONTENT, producer breadth, and native
rendering remain gated. ODA v5.4.1
§20.4.96.1 documents the pre-R24 body, but marks the AutoCAD 2008 cell changes
as TODO; §20.4.96.2 says R24 moves data into TABLECONTENT and the specification
does not document the round-trip connection. The local authoritative PDF is
not installed; references here use the official [ODA v5.4.1
PDF](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf),
§20.4.96 (pp. 229–232). Do not attempt one cross-version encoder or infer the
missing R24 graph from the current parser.
The direct generated AC1018 body codec now also sweeps 18 allocation-failure
positions for a one-row/one-column text cell with alignment and text-style
handle overrides. Each failure preserves seeded body and handle buffers, and
the same object retries successfully; the focused `--table-cell-overrides`
selector passes. This is a direct-codec transaction check narrower than the whole-file writer.
The generated whole-file profile is recorded in the [AC1018 TABLE receipt](docs/research/p5-ac1018-table-whole-file-cached-geometry-2026-11-10.json);
native acceptance/rendering and independent cell-body semantics remain open.
The inspected code paths, exact authority section, candidate exclusions, and
compound-output acceptance gates are recorded in the
[encoder-readiness receipt](docs/research/p5-dwg-table-encoder-readiness-2026-11-08.json).

The existing AC1021
`tests/samples/AC1021/blocks_and_tables_-_imperial.dwg` captures two
`AcDbTable` entities with encoded/resolved class number 506. The bounded
BitShort alignment candidate now reads all 171 cell CadValues across the
metric and imperial sources, but deliberately leaves the TABLE views partial
because the field conflicts with ODA's RS declaration; strict output stops at
the unrelated FIELD/FIELDLIST gate. This is not a complete writer witness and
does not provide an AC1018 class number. Before registering a TABLE encoder,
capture its class number from a real DWG of the exact target version; never
copy 506 across versions or use a peer's numeric class value.

An AC1018 source screen is now available, but neither candidate is a complete
TABLE witness. `/Users/dli/doc/dwg6/example_2004.dwg` resolves `AcDbTable` to
class 528, while `/Users/dli/doc/dwg6/sample_AC1018.dwg` resolves it to class
523. Both have TABLE bodies with undocumented cell override bits (`0x40000`
and `0x40020`) and stop typed semantic parsing there; the second source also
has unrelated IMAGEDEF/reactor and other object parse failures. These are
source-specific observations, not stable AC1018 class IDs. Do not hardcode
either value or skip the unknown override payload. Keep these physical source
profiles refused: the writer admits only the separate generated AC1018
plain-text profile, and no source-to-output proof exists for these parsed
overrides. The speed-first
unknown-override slice preserves the consumed flag word and virtual-edge byte
as an exact, explicitly incomplete range; generated `0x40020` control confirms
bounded cursor position, incomplete semantics, and same-object recovery on a
corrected retry. A second bounded milestone now encodes a generated AC1018
plain-text cell directly, checks strict one-cell/default-field preconditions,
and verifies typed parser readback, BLOCK_RECORD and TABLESTYLE handle roles,
exact body cursor, and a following-body sentinel. Focused preflight controls
also refuse unsupported version, TABLE/cell overrides, extra columns,
incomplete semantic content, and detached value handles without changing
pre-seeded body/handle buffers; the same object succeeds after its rejected
override is corrected. Generated parse controls also refuse a body whose
declared span is one bit short and a frame missing its trailing handle stream.
The focused `--table-cell-overrides` selector passes. The latest guard-matrix
slice adds atomic refusal checks for a missing entity handle, a noncustom
class number, detached string-stream use, a null body buffer, lineweight
override markers, unsupported content-override kinds, retained geometry
coordinates, and typed value variants. This is encoder/parser
self-consistency evidence only: covered refusal paths are still not a proof
that every public-model field or every low-level emission failure is guarded.
A subsequent model-to-encoder audit found eleven states the generated codec had
silently ignored: column name, cell base style ID, cell text-style override
handle, retained SEQEND metadata, and attached `DRW_Insert::attlist` ATTRIB
models (the existing check covered only parsed `attribHandles`), inherited
`DRW_Point::thickness` and `xAxisAngle` values not encoded by this TABLE frame,
and noncanonical `seqendH` code, size, raw role, and 64-bit handle metadata
when the 32-bit footer reference is absent. The direct AC1018 codec now rejects
each before writing, and the focused matrix confirms buffer preservation.
This closes those specific omission paths; continue auditing the public model
and private retained DXF state before widening the profile.
The follow-up common-header audit found more ignored state in this AC1018-only
profile: proxy/reactor counts that disagree with their empty vectors; material,
shadow, and R2010+ visual-style references that the AC1018 common writer omits;
and the AC1027+ DataStorage flag, linkage, payload, indices, and marker fields.
The direct TABLE preflight now refuses those states. Focused tests exercise the
count mismatches, newer-version common fields, and each DataStorage state
family while checking output-buffer preservation. A reference audit also found
that a null layer target, or a named linetype without its required handle,
could be serialized as a null reference; these cases now fail preflight too.
Malformed common state is also refused: non-finite linetype scale, lineweight
codes outside the DWG domain, negative plot-style sentinels, and inconsistent
AcDbColor-presence/handle pairs. The preflight also rejects out-of-domain ACI/
RGB values and color names or inline RGB data that the selected AcDbColor
encoding would otherwise discard, plus invalid linetype-flag and entity-space
enum values that the common writer would normalize. The frame-path audit then
found the writer resolving the special names BYLAYER/BYBLOCK/CONTINUOUS to
ordinary LTYPE handles before encoding. `dwgWriter15` now keeps those states
implicit, and the TABLE preflight rejects contradictory caller-supplied
linetype handles or mismatched raw flag/name pairs; the focused frame test
checks the BYLAYER state survives class registration and encoding. The latest
public-model audit also found DXF `appData` groups have no general DWG serializer.
The shared common encoder now admits only canonical `ACAD_REACTORS` and
`ACAD_XDICTIONARY` wrappers whose handle values and group-code roles exactly
match the separately encoded common references; arbitrary, malformed, or
mutated groups fail before common output is written. Generated controls cover
matching groups, opaque group refusal, handle mismatch, and wrong XDictionary
group-code role. The narrower TABLE profile continues to refuse all nonempty
`appData` because its selected model covers no common application groups.
This preserves known common references without claiming a mapping for other
group-102 application data.
The next direct-codec model audit found that the inherited
`DRW_Insert::name` was not emitted by the AC1018 TABLE entity frame and was not
validated by its direct encoder; arbitrary display names could therefore pass
the frame preflight even though the compound output requires a matching
anonymous `*T…` BLOCK_RECORD. The direct AC1018 encoder now requires the
bounded `*T` name profile before staging output. A generated negative vector
checks destination preservation and same-object correction retry; valid frame
encoding still passes. Compound graph validation remains responsible for
matching the TABLE name to the actual BLOCK_RECORD name. See the [TABLE block
name preflight receipt](docs/research/p5-ac1018-table-block-name-preflight-2026-11-10.json).
The retained-state audit found one more edge case: an unsupported group 97/98/4
can mark DXF parsing incomplete before the TABLE grid exists, so the public
`m_hasSemanticContent` flag is still false. The encoder now checks its private
unsupported/open-value/invalid-merge parser state independently of that public
flag. A generated `AcDbTable` subclass + group-97 control verifies pre-grid
refusal with both output streams unchanged; no fixture was added.
The EED encoder also returns immediately for an empty `extData` vector, so
stale pending APPID/LAYER resolution entries could otherwise be ignored. The
shared `encodeDwgEed` now refuses writer-side APPID/LAYER resolution vectors
without payload, while `encodeDwgCommon` rejects parser-side pending refs and
writer-side resolution residue with an empty payload before writing common
fields. Generated controls verify both families preserve a preseeded
destination; the TABLE preflight retains its narrower early guard as well.
The inherited INSERT audit also found `blockRecH`'s parsed wire code, size,
raw role, and 64-bit reference were ignored while the TABLE encoder rebuilt a
canonical hard pointer from the 32-bit `ref`. This generated-only codec now
accepts caller-created handle references with no retained wire metadata and
refuses each retained source-metadata field until its role/width mapping is
qualified; four atomic refusal vectors cover those fields.
The public `eType` tag can also be mutated independently of the dynamic
`DRW_Table` type. Since the writer registers and serializes the custom TABLE
class based on the dynamic type, the direct encoder now rejects a non-TABLE
tag rather than creating inconsistent in-memory/output identity; the focused
matrix verifies atomic refusal.
The common EED guards apply to all entity writers. The remaining model and
parser-state checks apply only to the generated AC1018 TABLE codec.
The direct encoder builds both streams in private copies, catches emission
exceptions, and swaps the staged buffers into the caller only when both are
good. The focused control injects 17 allocation failures across 18 attempted
offsets, verifies exact seeded bytes, bit counts/positions, handle-occurrence
ledgers and caller common-flag rollback, then retries the same TABLE into the
same buffers and compares exact output with a clean encoding. The terminal
success offset also matches. The focused CTest passes in 0.39 seconds; no
external drawing fixture is used. This closes direct-codec allocation rollback
for the bounded generated profile. The generated whole-file route now also
covers class registration, `*T` geometry, graph closure, late refusal rollback,
same-facade retry and fresh readback (see the [whole-file TABLE
receipt](docs/research/p5-ac1018-table-whole-file-cached-geometry-2026-11-10.json)).
Physical source-to-output proof, native/independent cell semantics and rendering,
and whole-file allocation-failure injection remain open. See the [TABLE
allocation receipt](docs/research/p5-ac1018-table-codec-allocation-rollback-2026-11-08.json)
and the [updated encoder-readiness receipt](docs/research/p5-dwg-table-encoder-readiness-2026-11-08.json).

The two AC1018 CLASS records agree on the identity fields despite their
different ordinals: app `ObjectDBX Classes`, class `AcDbTable`, record
`ACAD_TABLE`, proxy capabilities 1025, DWG class version 25 and maintenance
version 20; group 280 marks them as entity classes. LibreDWG independently
maps `TABLE` to `ACAD_TABLE` but classifies its implementation as
`UNSTABLE`, which reinforces a version/profile-specific gate rather than a
general output claim ([LibreDWG objects.in](https://github.com/LibreDWG/libredwg/blob/master/src/objects.in#L1210-L1213)).
The writer now has an AC1018-only registration helper that allocates a
writer-local ordinal and resolves repeats by class identity; generated controls
confirm identity reuse and refusal on AC1027. A generated AC1018 control now
registers one handle-bearing instance, confirms the declared count is invalid
until its frame-commit marker, serializes the registered CLASS entry, and
independently decodes its class number, full identity, entity flag, version
fields and instance count. The focused `--table-cell-overrides` selector
passes. A bounded generated decoder now also walks the complete emitted AC1018
CLASSES section, checks its row ordinals, TABLE metadata and instance count,
payload padding, CRC, zero tail and ending sentinel. This is a generated
writer/decoder control, not an independently implemented DWG reader or physical
interoperability witness. A second generated check stages the TABLE class and a
pending instance, then applies the writer's existing compound checkpoint
rollback and verifies the class definition, handle-to-class link and
pending/emitted count state are gone. A further generated `dwgRW` transaction
control now registers a placeholder `*T` BLOCK_RECORD, stages and writes an
actual AC1018 TABLE entity frame, adds block-owned geometry, injects a late
geometry failure, and rolls back the outer transaction. It verifies the frame,
class definition/instance ledger, writer counters and caller state are
restored; the same adapter then retries and commits. This closes the
class/frame and late-failure transaction proof, but its hand-authored geometry
is not a semantically complete TABLE `*T` graph. A focused CTest is registered
as `libdxfrw_dwg_table_cell_overrides`; see the [transaction
receipt](docs/research/p5-table-dwgrw-class-frame-transaction-2026-11-08.json).
**Superseded by the 2026-11-10 generated whole-file slice:** public
`writeTable` now admits exactly the generated plain-text AC1018 profile after
pre-freeze class registration and final TABLE/BLOCK_RECORD/TABLESTYLE/NOD
closure. The physical ODA-converted candidate still carries an unrelated
HEADER warning, and independent LibreDWG leaves its cell body opaque. Next
work is native acceptance/rendering or an independent cell-semantic decoder;
then qualify additional physical profiles without broadening the guarded
writer from cached geometry alone.

**Independent writer caution:** ACadSharp merged TABLE DWG write support in
PR #1040, but a later ACadSharp 3.8.0 report reproduced a TABLE-containing
AC1032 read/write round trip that AutoCAD 2020 rejects (`ErrorStatus=53`) and
that loses one of two tables. The reporter traced a likely cause to FIELD and
BLOCK cell-content handle references being written as null; this is a reported
root-cause hypothesis, not an independently confirmed fix. Do not treat the
presence of an upstream writer or successful self-readback as support proof.
Our candidate excludes FIELD/BLOCK cell content, and its compound-writer gate
must assert exact content-handle roles, preserve every table, and qualify with
an independent reader/producer or native platform before promoting support.
See [ACadSharp PR #1040](https://github.com/DomCR/ACadSharp/pull/1040) and
[ACadSharp issue #1321](https://github.com/DomCR/ACadSharp/issues/1321).

Speed-oriented implementation sequence:

1. The direct **AC1018 plain-text-cell** codec is implemented for generated
   rectangular plain-text grids; inline strings avoid both the R2007+
   detached string stream and the undocumented R24 TABLECONTENT relationship.
   Focused refusal controls now cover unsupported version, table/cell
   overrides, grid shape/spacing, owner/required handles, incomplete semantic
   content, entity attachments, styles/colors, row options, retained
   subrecords, links/attributes, cell kinds/value state, cache/geometry and
   invalid numeric values. They check pre-seeded output buffers and corrected
   same-object retry. Parser controls now refuse a declared body shortened by
   one bit and a frame without trailing handles. The direct codec stages both
   streams and publishes only on success; a successful append preserves seeded
   prefixes. Generated codec-level allocation failure injection now covers all
   25 measured allocations for a 2x2 grid, with destination byte/bit rollback
   and exact same-object retry (normal and ASan); this verifies the direct codec
   boundary only, not the outer `dwgRW` compound graph transaction. See the
   [grid allocation rollback receipt](docs/research/p5-ac1018-table-grid-allocation-rollback-2026-11-09.json).
   A generated 2x2 vector now reads back both column widths, both row heights,
   all four row-major text values, the bottom-right cell's hard-pointer
   text-style override, exact body boundary, and following sentinel;
   `rowcount`/`colcount` are published from the parsed TABLE grid. The owning
   CTest passes in normal and ASan builds ([multi-row
   receipt](docs/research/p5-ac1018-table-multirow-plain-text-codec-2026-11-09.json),
   [rectangular-grid receipt](docs/research/p5-ac1018-table-multicolumn-plain-text-codec-2026-11-09.json)).
   The latest fast slice extends refusal coverage to missing entity identity,
   noncustom class ordinals, detached string stream/null body, lineweight and
   content override markers, retained geometry coordinates, and typed value
   variants. Grid resource preflight now has atomic refusal controls for
   over-limit columns, rows and total cells; the last case holds both individual
   dimensions within limits. Normal and ASan focused selectors pass. See the
   [grid-budget receipt](docs/research/p5-ac1018-table-grid-budget-refusals-2026-11-09.json).
   Continue auditing remaining guard branches only where a distinct public
   model invariant lacks an observable refusal/readback control. The cached
   string-versus-cell-text check now compares exact length and bytes, including
   embedded NULs; its generated refusal, destination-preservation, and corrected
   retry control is in the [exact string guard receipt](docs/research/p5-ac1018-table-cached-string-exact-match-2026-11-09.json).
   Allocation-failure coverage now also proves common-header flag caches roll
   back with staged body/handle buffers: the sweep exercises both the BYBLOCK
   `ltFlags` derivation and `plotFlags`, then retries the same object
   ([receipt](docs/research/p5-ac1018-table-common-header-state-rollback-2026-11-09.json)).
   The production
   compound writer still needs its own graph-level staging and atomic publication. A
   generated ownership slice now verifies that the AC1018 TABLE frame has one
   hard-pointer edge to its matching anonymous `*T` BLOCK_RECORD, every
   `*T` child is indexed under that record, and the BLOCK_RECORD's reverse
   INSERT list and writer index point back to the TABLE exactly once. Mismatched
   names and non-TABLESTYLE targets are refused. The TABLE and TABLECONTENT
   style handles must agree and resolve to an emitted `AcDbTableStyle` class
   instance. The `ACAD_TABLESTYLE` registration must select one emitted
   dictionary whose parent is NOD and whose member list points to that style.
   The emitted-dictionary ledger participates in outer rollback; a generated
   table graph control confirms the pending NOD registration, dictionary
   frame/index and TABLESTYLE class instance are removed by outer rollback and
   all three are restored by same-writer retry. This closes only
   generated graph-bookkeeping and retry evidence: the control uses one
   synthetic LINE, does not establish that geometry matches cells or that the
   dictionary key matches the TABLESTYLE name, and does not serialize/freshly
   read back the final NOD frame. See the
   [graph-closure receipt](docs/research/p5-ac1018-table-block-graph-closure-2026-11-09.json).
   Public `dwgRW::writeTable` remains fail-closed. The
   existing AC1018 traces disagree on class number (528 versus
   523), while their CLASS identity metadata agrees. An AC1018-only writer
   registration helper now allocates a local ordinal and resolves duplicates
   by identity; its focused control passes. Keep it disconnected from
   `writeTable` until remaining profile and graph gates pass. Do not infer
   class IDs from the DWG version.
   Require bounded matching row/column counts, exactly the declared number
   of cells in each row, and one value-content item of type 1 per cell. Allow
   only the already validated table-scalar/per-cell override subset and the
   bounded AC1018 merge-role layouts accepted by the direct codec. Keep data
   links, fields, block content, attributes, unknown retained raw subrecords,
   and unexplained geometry/cache state refused. The one admitted retained
   subrecord signature is AC1018-only: complete ordered 10-bit zero-mask cell
   override spans for populated text cells followed by the complete 4-bit
   zero-mask TABLE override tail. Validate every range and the matching model
   fields; reject anything outside this profile before emitting bytes. See the
   [retained-range codec receipt](docs/research/p5-ac1018-table-retained-zero-override-range-writer-2026-11-10.json).
2. **Physical profile evidence remains partial.** The peer-converted AC1018
   TABLE body is independently readable and agrees on the 3-by-3 text values,
   dimensions, anonymous-block name, and 15-entity geometry census. The direct
   generated codec now emits and reads back validated AC1018 merged-cell
   flags/spans and the seven zero-mask cell records plus four absent TABLE mask
   bits. Instrumented ACadSharp independently reproduces their source-bound
   cursor spans under a constant 35,888-bit origin bias. The peer is ODA
   converted from AC1027 DXF, has a HEADER integrity warning, and does not
   qualify native acceptance or text/line geometry generation. The direct
   encoder now accepts only the bounded retained zero-mask range signature
   described above; the focused control re-encodes a generated model carrying
   parser-produced spans, rejects a shortened span atomically, and retries the
   corrected object. It does not establish source-document conversion. The
   generated whole-file AC1018 route already finalizes TABLE, `*T1` geometry,
   TABLESTYLE, and NOD closure, preserves destination/caller state on failure,
   retries, and passes fresh libdxfrw readback; LibreDWG independently confirms
   the graph and cached geometry while leaving TABLE cell semantics opaque.
   Remaining work is native AutoCAD/ODA acceptance/rendering or independent
   cell-semantic decoding, additional physical producer profiles, broader
   content/field/block cases, and R24 TABLECONTENT. Do not repeat the completed
   generated graph bookkeeping or promote support from the single
   ODA-converted source.
3. Keep AC1021+ and AC1024+ refused until their source-bound stream/handle
   profiles and the R24 TABLECONTENT/TABLEGEOMETRY graph are established. Add
   one version at a time with separate body, dependency, rollback/retry, and
   readback evidence. Native table rendering and broad support claims remain
   separate gates.

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
   A local AC1021 metric TABLE candidate has no direct TABLEGEOMETRY evidence:
   its two ACAD_TABLE entities now parse their 171 CadValues under a bounded,
   semantically partial alignment-width candidate, while the peer JSON exposes
   no TABLEGEOMETRY record. This source cannot establish TABLEGEOMETRY source
   spans or graph closure. Keep that gate separate from the TABLE alignment
   candidate and reopen it only with a complete, source-bound TABLEGEOMETRY graph.
   See the [candidate screen receipt](docs/research/p5-ac1021-metric-tablegeometry-candidate-screen-2026-11-06.json).
4. Use the existing `p5-ac1027-*` receipts as the exclusion inventory. The
   generated R24 TABLESTYLE and CELLSTYLEMAP group-62/420/430 CMC profile is
   implemented with method/base consistency and refusal/retry controls; see
   [TABLESTYLE CMC](docs/research/p6-r24-tablestyle-cmc-truecolor-book-2026-11-08.json)
   and [CELLSTYLEMAP CMC](docs/research/p6-cellstylemap-cmc-loss-guard-2026-11-08.json).
   Continue only with distinct uncovered source- or consumer-backed
   CONTENTFORMAT/CELLSTYLEMAP/linetype combinations and source-backed
   precedence for overlapping TABLECONTENT edge declarations.
   A post-unlock AC1021 `blocks_and_tables_-_imperial.dwg` census read and
   published 3,586/3,586 frames without parse failures, but contained no
   TABLECONTENT trace records; strict output stopped at the unrelated
   FIELD/FIELDLIST serializer gate before producing DXF. This candidate is
   excluded from TABLECONTENT evidence. See the
   [AC1021 edge-census screen](docs/research/p5-ac1021-blocks-and-tables-edge-census-screen-2026-11-08.json).
   **Saved-evidence inventory screen complete:** the AC1021 imperial candidate
   has no TABLECONTENT frames; the AC1021 metric TABLE remains semantically
   partial under the alignment-width compatibility candidate; AC1027 object `0xC94` has
   unresolved body/string/handle alignment and no typed/raw callback; and
   AC1032 objects `0xE5F`/`0x747` remain raw-only with semantic completeness
   false. The alternate parses and peer observations do not establish an
   independent wire layout. No existing TABLECONTENT-specific receipt provides
   a source-bound witness ready for selected-object extraction. Do not repeat
   these whole-file reads or treat uninspected local samples as negative
   evidence. Keep overlapping edge masks refused; resume only with a new
   source-bound witness or authoritative resolution, otherwise take a distinct
   locally implementable P5/P6 slice. See the
   [saved-evidence TABLECONTENT screen](docs/research/p5-tablecontent-existing-source-evidence-screen-2026-11-08.json).
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
After repeated user unlocks, five bounded retries of the same verified ODA
27.1 direct CLI invocation still aborted during Pasteboard/XPC initialization
and produced no output; a Launch Services attempt also returned
`kLSNoExecutableErr`. The latest direct retry after another user-reported unlock rechecked source and converter hashes and again aborted before document processing with Pasteboard/XPC errors. Do not repeat this unchanged launch method unless the launch environment or available converter session changes. This is not evidence about suffix contents:
keep the bytes unclaimed and strict output refused ([latest retry receipt](docs/research/p6-ac1027-oda-header-suffix-after-latest-unlock-2026-11-07.json), [earlier retry receipt](docs/research/p6-ac1027-oda-unlocked-cli-retry-2026-11-05.json)).
Two GUI conversions through ODA File Converter 27.1 used scratch copies of the
same input and ACAD2013/AC1027 DWG output, with recursion and audit disabled.
Both outputs have 739 unclaimed suffix bytes after the validated byte-844
trailer; their hashes differ and no byte positions match, while every position
has matching low three bits. The reader publishes 119/119 observed frames with
zero entity/object parse failures, but strict DXF output still refuses at the
independent AC1027 source-reference gate. This confirms that a materially
changed converter route works; it does not explain the suffix or qualify
output. See the [GUI resave receipt](docs/research/p6-ac1027-oda-gui-same-version-resave-2026-11-07.json).
An AC1024 maintenance-55 source now reaches its raster and NOD callbacks under
the same diagnostic policy. Its IMAGEDEF and reactor are retained for diagnosis,
but export refuses the IMAGEDEF's unqualified role-4 owner because ODA's common
and record-specific descriptions conflict. Resolve that role with authoritative DWG wire evidence and close the image/proxy graph while retaining the separate HEADER warnings and unclaimed suffix refusal. Autodesk confirms the ACAD_IMAGE_DICT relationship and soft-pointer role in DXF, but that does not resolve the DWG common-parent conflict; the LibreDWG image constructor is explicitly incomplete and is not a role oracle ([role boundary receipt](docs/research/p4-imagedef-autodesk-dxf-owner-role-boundary-2026-11-05.json)). Audit other revisions' generic Unsupported
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
The AC1024 v2 no-tail source profile is now admitted at read time by exact
body-end evidence; the present-tail vector remains covered, while v1 keeps its
existing tail requirement. This is a bounded reader accommodation, not a
resolution of §20.4.78's release/AutoCAD-2010 contradiction. The writer still
emits the current canonical target-version tail and strict whole-document
output for this source remains refused at its HEADER suffix. Obtain an
authoritative erratum and native AutoCAD 2010 acceptance before changing the
writer gate or claiming general AC1024 GEODATA support
([receipt](docs/research/p4-ac1024-geodata-v2-civil-tail-conflict-2026-11-04.json)).

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
warnings. The fixed nonprocedural AC1021 MATERIAL body now decodes for the
three-record children-room source: a focused physical probe matches the
LibreDWG-observed scalar/map profile and owner/reactor roles, and the generated
default-material control passes for AC1021/AC1024/AC1027 ([parser receipt](docs/research/p4-ac1021-material-fixed-body-parser-2026-11-06.json)).
This is parser evidence only; selected physical graph output/readback,
procedural or unknown tails, broader source profiles, native rendering and
complete interoperability remain open
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
The AC1024 sample-reader matrix now distinguishes this bounded raw-only
disposition from a zero-failure typed parse: only these two named samples may
report exactly three object failures, and the probe requires each to correspond
to a validated untyped LOFT/REVOLVE frame with complete source-frame coverage.
It also pins the observed class census per sample: two LOFT plus one REVOLVE in
the condominium source, and three REVOLVE in the conference-room source.
All other sample profiles still require zero entity/object failures and zero
raw-only AcSh exceptions. The focused 13-test AC1024 reader/retention cohort
passes; this corrects the test expectation without claiming typed LOFT/REVOLVE
semantics or strict output
([receipt](docs/research/p4-ac1024-acsh-sample-read-disposition-test-2026-11-07.json)).
The 2026-11-08 source screen found no ready field-layout oracle: ODA v5.4.1 has
no class-specific LOFT/REVOLVE body entries; LibreDWG marks both types
`DEBUGGING` and its same-input runtime reports the same suspect fields; the
additional local `dwg-writer` parser follows the same `dwg-parser` project
lineage and candidate field sequence; the local ACadSharp source has no parser
for either class. Keep the frames raw-only and do not port the candidate schema
or change parser boundaries without new authoritative or provenance-qualified
independent evidence ([source screen](docs/research/p6-ac1024-acsh-independent-layout-source-screen-2026-11-08.json)).
This source screen is complete for the currently available local authorities
and peers; do not repeat it unchanged. Resume only when an authoritative class
layout or provenance-qualified independent body/span observation becomes
available. Until then, keep these AC1024 frames raw-only and strict output
refused, and advance to a different implementation-ready packet.

| Boundary | Remaining decision or proof |
| --- | --- |
| Version/route parity | Finish the standalone versus pinned LibreCAD/bundled/LibreCAD_3 route differential. Establish authoritative DXF fields for the derived `AcDbAssocValueDependency` body, validate it with a reader that implements that subclass, and establish native acceptance for the associative profile. See [the P6 differential receipt](docs/research/p6-libre-dwg-associative-dxf-import-differential-2026-10-31.json). |
| DIMASSOC DWG-to-DXF transport | AC1018 inline-text and AC1021+ detached-string readers map bounded main/intersection fields into the existing DXF model, with fail-closed handling for unknown classes and cardinalities the model cannot represent. The opt-in physical-source harness verifies resolved source entity references and ASCII/Binary writer output plus typed readback: 23/23 AC1018 `Extruder2.dwg` records and 7/7 AC1021 architectural-sample records passed after the previously refused record's two main-object handles were retained and remapped. A peer scan of 28 unique local DWGs found no multi-handle intersection witness; this is not a format maximum ([cardinality scan](docs/research/p6-dimassoc-corpus-cardinality-2026-11-05.json)). A generated AC1024 vector independently checks owner/reactor/dimension/main/intersection remapping and all modeled fields. LibreDWG's current `dwg2.spec` declares the same ordered sequence; on AC1018, 23 libdxfrw callbacks/33 refs matched LibreDWG 0.14 minJSON for handles, types, subentity/GS values, points, last-point flags and the intersection edge, with one trace-rounded near-parameter difference. These prove selected-record transport only. AC1018 whole-file output remains refused by 474 entity parse failures; AC1021 whole-file output remains refused by 36 MLINE proxy caches. ODA v5.4.1 does not document this body, and native associative behavior remains unqualified. A bounded AC1024 DWG encoder now emits the generated type 1/6/13 profile, including ordered main/intersection handles and both XREF path fields; the focused framed codec control passes atomic unsupported-detail refusal and corrected same-buffer retry, and a full-file generated read delivers all three records and path strings with zero entity/object parse failures. The target handles are absent, so that physical result does not establish graph closure. LibreDWG 0.14 reads the path-free output fields but its `FIELD_VECTOR_T` reads from the scalar cursor rather than the string cursor for path vectors; do not interpret its path failure as writer incompatibility or as interoperability proof ([generated writer receipt](docs/research/p6-dimassoc-dwg-detailed-writer-generated-control-2026-11-05.json)). A post-unlock ODA retry also exited with PasteBoard/XPC errors, so independent reader validation remains unavailable. A strict `dwg2dxf` probe on the generated file refuses at the opaque-frame gate (`qualified=2/3`), confirming fail-closed behavior rather than silent omission. The DWG-to-DXF adapter now retains bounded typed DIMASSOC callbacks and requires a unique typed dimension target, closed emitted entity references, exact source-frame identity, and a single matching remapped OBJECTS record before qualifying each frame. A locally authored missing-target DWG is refused atomically by focused CTest with no published DXF; stale DIMASSOC rows from earlier import generations are ignored and cannot leak into a later DXF export ([adapter closure receipt](docs/research/p6-dimassoc-adapter-closure-2026-11-05.json)). Generated in-memory adapter controls now prove ASCII/Binary closure refusal, destination rollback, same-adapter target correction/retry, emitted-record identity and typed graph readback; a separate control proves stale import generations do not leak into output. These are locally authored adapter mechanics evidence only. LibreDWG 0.14 independently resolves physical dimension, MLEADER and geometry target classes in the AC1018/AC1021 samples, and Autodesk documents dimension-or-leader updating ([target-role receipt](docs/research/p6-dimassoc-independent-target-roles-2026-11-05.json)); whole-file adapter output for those physical sources, native update semantics, and broader producer profiles remain open. The AC1021 selected-record result is 7/7 in ASCII/Binary DXF ([vector receipt](docs/research/p6-dimassoc-main-handle-vector-transport-2026-11-05.json)); see the [earlier DIMASSOC transport receipt](docs/research/p6-dimassoc-dwg-detail-to-dxf-transport-2026-11-02.json) for the AC1018 23/23 result and prior profile evidence. |
| DWG source profiles | **AC1027 Cover:** resolve the 731-byte suffix after the HEADER end sentinel. ODA v5.4.1 defines the CRC/sentinel boundary; LibreDWG 0.14 parses the known handle stream through byte 815, before the envelope trailer at byte 826. Neither source explains bytes 844–1575. Find producer-backed layout evidence or retain the suffix as unclaimed and strict-complete output refused; do not assume it is padding or corruption. Continue independent per-version reader/container/HEADER/CLASSES and producer qualification only for uncovered profiles, including R2007+ carrier differences and class counts. The exact AC1027 app-maintenance-29 HEADER envelope in the public `example_2013.dwg` witness now passes header parsing; do not reopen this envelope without a new failure. After the bounded TABLEGEOMETRY writer was added, a strict AC1024-target replay passed that object and then refused at ModelDocViewStyle serialization because it requires the exact source DWG version. An AC1027-target replay was refused earlier because the strict read disposition remains incomplete (three object parse failures and one DataStorage record not linked to a parsed modeler/surface entity). This does not close AC1027 producer, SPLINE or whole-output qualification; see the [TABLEGEOMETRY transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json). See the [exact HEADER receipt](docs/research/p6-ac1027-exact-header-size-exclusion-2026-11-01.json). The alleged AC1024 CLASSES high-size read failure is not reproduced in the current local corpus: all nine available AC1024 inputs reach `dwgReader24::readDwgClasses END` and report `operationSucceeded=true`, `legacyError=0`, and no CLASSES CRC mismatch. In that 2026-11-03 corpus run only `mechanical_example-imperial.dwg` exited successfully. A 2026-11-05 current-code rerun converts `mechanical_example-imperial-alt.dwg` successfully and fresh typed libdxfrw re-import passes, but its SHA-256 exactly matches `mechanical_example-imperial.dwg`; this is repeat evidence for one unique source, not a second mechanical input. The distinct `civil_example-imperial.dwg` publishes all 335 observed frames with zero entity/object parse failures, then strict output refuses on incomplete read disposition. Independent consumer readback and a second complete source profile remain unqualified. Autodesk's `title_block-ansi.dwg` sample is a distinct AC1024 source (SHA-256 `907398897552a090182f5446f18ad1ef82b91176764c8ccf08b5c5799546eeba`) and exposed a BLOCK_HEADER false rejection: its `*Model_Space` blockControl is a valid soft pointer encoded as relative handle code 0x8, which resolves object handle 0x2 to BLOCK_CONTROL handle 0x1. ODA v5.4.1 §2.13 and LibreDWG 0.14 independently confirm this. `DRW_Block_Record::parseDwg` now accepts the documented relative soft-pointer codes 0x6/0x8/0xA/0xC for fields whose semantic role is soft pointer; the focused generated control passes all four encodings plus its wrong-role and truncation/retry cases. Current libdxfrw publishes the sample's 498/498 observed frames with zero entity/object parse failures. Its three null counted BLOCK_CONTROL entries now produce a recoverable warning, while null model/paper-space phantom members remain strict-fatal; ODA v5.4.1 §20.4.51 and LibreDWG 0.14 distinguish the counted entries from those two required members. Strict DXF output now advances through read disposition and refuses at 106/109 opaque callbacks because three SORTENTSTABLE records (handles 0x3398, 0x3668, 0x3924) remain unqualified; LibreDWG 0.14 maps them to two anonymous `*U` blocks and the named `Title_Block-ANSI` block, all outside the Autodesk group-330 owner domain. Other Autodesk downloads were hash duplicates of local samples or AC1021; do not treat their repeated names as new AC1024 witnesses. See the [candidate and fix receipt](docs/research/p6-ac1024-online-sample-candidates-2026-11-09.json). See the [mechanical identity and civil probe receipt](docs/research/p4-ac1024-mechanical-alt-strict-output-2026-11-05.json). Explicit strict callback-finalization failures are present for the architectural and two title-block samples, and the condominium and conference-room samples report three object parse failures each; these later outcomes are not CLASSES failures. The exact 2026-11-03 command, source/tool fingerprints, inputs, trace hashes, and per-file outcomes are recorded in the [current AC1024 corpus rerun receipt](docs/research/p6-ac1024-classes-current-corpus-rerun-2026-11-03.json); the earlier extended-size and synthetic-footer selector remains in the [prior CLASSES recheck](docs/research/p6-ac1024-classes-extended-size-current-corpus-recheck-2026-11-01.json). Do not reopen this parser path without a new failing sample. A newly acquired online `nextgis/dwg_samples/line_2010.dwg` is hash-bound as AC1024 and reaches `readDwgClasses END` with zero legacy error, but its repository provenance is trial AutoCAD 2016, so it does not qualify the reported AutoCAD 2010 RTM failure; Autodesk’s public 2010-and-later sample collection still lacks per-file producer release metadata. See the [online AC1024 sample download check](docs/research/p6-ac1024-online-sample-download-check-2026-11-09.json). the local drawings were not added to the repository. See also the [current Cover receipt](docs/research/p6-ac1027-cover-reader-success-output-refusal-2026-11-01.json). The recorded ODA File Converter 27.1.0 GUI run accepts this source and rewrites it as AC1032, but does not explain the 731-byte AC1027 suffix. The latest current-code AC1032 trace reaches the CLASSES end marker and parses all 115 frames with no entity/object parse failures or DataStorage link/orphan failures: the bounded AC1027-origin alternate schema-index table-role profile is retained in the resave. Strict-complete output still refuses before output preparation because HEADER reports a compatibility warning at file offset 16 for the zero high-size word (declared size 802; payload end 826 versus 822 if the word were included) and an unclaimed-data warning at offset 844 (739 suffix bytes through 1583); no DXF was created. Both suffixes remain unexplained, so keep strict-output refusal. See the [AC1032 DataStorage profile recheck receipt](docs/research/p6-ac1027-cover-ac1032-datastorage-role-profile-2026-11-02.json) and the [ODA resave/retry receipt](docs/research/p6-ac1027-cover-oda-resave-recheck-2026-11-02.json). A prior sandboxed shell retry aborted with PasteBoard/XPC errors. On 2026-11-04, strict signature verification passed outside the sandbox and the requested same-revision retry succeeded. The new AC1027 output again has 739 bytes after its validated HEADER trailer at offset 844; its suffix hash differs from the two earlier same-revision outputs. The current reader publishes all 119 frames with no entity/object parse failures; strict output still refuses because source references are not fully qualified. Keep the suffix unclaimed; see the updated [same-version resave receipt](docs/research/p6-ac1027-cover-oda-same-version-resave-2026-11-03.json). |
| TABLEGEOMETRY DWG field encoding | ODA v5.4.1 contradicts itself: §20.4.98 calls the final shared cell-geometry field BL, while §20.4.103 lists the standalone TABLEGEOMETRY group-95 field as BD. LibreDWG's 2020 schema change and the pinned acadrust writer use BL for the shared geometry payload. A wider local corpus scan covered 184 unique AC1021+ DWGs; nine readable files contained 14 TABLEGEOMETRY objects, all with final value zero. A generated AC1021 direct-parser vector now checks nonzero BL `0x12345`, the geometry fields before it, cell framing and exact following content-handle resolution to `0xA720` (`libdxfrw_tablegeometry_nonzero_bl_tail`). This synthetic vector protects the bounded-BL parser but cannot adjudicate the standalone source encoding. Keep the parser unchanged and retain the standalone source-span gate until a traceable independent producer yields a nonzero value. Do not conflate this DWG-stream question with DXF group-95 int32 output mapping or native TABLE semantics; see the [group-95 discrepancy/census receipt](docs/research/p6-tablegeometry-group95-profile-census-2026-11-02.json), [AC1027 transport receipt](docs/research/p5-ac1027-tablegeometry-null-cell-cache-transport-2026-11-01.json), and [ACadSharp profile receipt](docs/research/p5-acadsharp-tablegeometry-flag-profile-2026-11-01.json). |
| HATCH/MPOLYGON/MESH | Do not claim AC1018 HATCH `0x173` or ET Border HATCH `0x2BA` inactive-gradient stops as supported: both retained-stop profiles remain refused. The physical ET Border profile has a bounded DICTIONARYVAR-only selector that verifies atomic HATCH refusal, explicit local stop removal, and corrected object-graph output/readback; it does not qualify the stops. See [the selector recheck receipt](docs/research/p4-ac1018-dictionaryvar-hatch-resolution-recheck-2026-11-02.json). After explicit stop removal, generated HATCH/MPOLYGON controls and the selected physical HATCH record have bounded ASCII/Binary output/readback coverage for representable companion metadata and boundary fields. This does not establish the stops' native meaning or whole-file output; see the [loss-guard and selected-source transport receipt](docs/research/p6-ac1018-hatch-inactive-gradient-stop-dxf-loss-guard-2026-11-02.json). `DRW_Hatch::boundaryPointToWcs` and `boundaryVectorToWcs` expose the Autodesk OCS/elevation transform for HATCH/MPOLYGON boundaries without changing the stored 2D model or callback behavior; this is coordinate mapping evidence only, not native geometry qualification. See the [HATCH OCS mapping receipt](docs/research/p6-hatch-ocs-wcs-coordinate-mapping-2026-11-02.json). Generated ASCII/Binary HATCH controls enforce base-HATCH group 70/71 as 0/1, group 75/76 as 0..2, and group 77 as 0/1 in the parser; typed writer admission also bounds groups 75/76. MPOLYGON retains its distinct group-70 version and group-71 fill meanings. See the [Boolean-domain receipt](docs/research/p6-hatch-boolean-dxf-domain-2026-11-02.json) and [pattern-domain receipt](docs/research/p6-hatch-pattern-dxf-domains-2026-11-02.json). MPOLYGON group 99 now requires a nonnegative count within the existing 10,000-item parser budget at both reader and typed-writer admission; invalid values preserve the model/destination and corrected same-writer retry plus typed readback pass. Autodesk defines the field as a count of ignored degenerate borders but gives no relation to group 91, so no such relation is inferred and this scalar guard does not qualify native geometry semantics; see the [group-99 domain receipt](docs/research/p6-mpolygon-degenerate-count-domain-2026-11-02.json). MPOLYGON group 63 is now bounded to Autodesk's 0..256 CMC/ACI index domain on parse, typed validation and writer admission, with ASCII/Binary atomic refusal, same-writer retry and typed readback; this scalar guard does not qualify fill rendering (see the [ACI domain receipt](docs/research/p6-mpolygon-aci-dxf-domain-2026-11-02.json)). Generated HATCH gradient parsing now rejects explicit RGB outside 0..0xFFFFFF and ACI outside 0..256 before mutating the current stop; callback validation applies the same scalar bounds, and generated fast controls cover endpoints, preserved state and corrected same-group retry. This validates scalar transport only, not gradient rendering (see the [gradient stop color-domain receipt](docs/research/p6-hatch-gradient-stop-color-domain-2026-11-02.json)). MESH crease values are restricted to finite -1 or nonnegative values; generated parser/writer controls accept a shorter edge-order crease vector. Autodesk identifies group 95 as the level-0 edge-crease count, and ACadSharp's independent DWG reader applies each value to the same-index edge. The count is bounded by level-0 edge count at DXF/DWG read and write admission to prevent nonexistent edge references. A focused generated AC1024 DWG vector now exercises valid one-edge/one-crease parsing, malformed one-edge/two-crease refusal, state clearing, and corrected same-object retry; see the [reader-guard receipt](docs/research/p6-mesh-dwg-crease-count-reader-guard-2026-11-02.json). This establishes bounded positional transport only; physical producer breadth, native crease effect, subdivision, geometry and annotation semantics remain open. See the [MESH crease and edge-count receipt](docs/research/p6-mesh-crease-value-domain-2026-11-02.json). Other witnessed zero-fit/producer layout disagreements, extended CMC meaning and unrepresented overrides remain; qualify boundary geometry and annotation behavior beyond framing. |
| Text/annotation/coordinates | Qualify whole-document conversion and native dimension/text display; generated ASCII/Binary DXF reader controls now cover `applyExtrusion` conversion of TEXT and INSERT-owned ATTRIB insertion/alignment points, INSERT insertion points, and the OCS text points of all seven documented standard DIMENSION subtypes (0–6), including selected clone/arc points for linear and angular records, while preserving their WCS definition points. These controls do not establish native orientation/display or physical-producer breadth. See the [TEXT/ATTRIB receipt](docs/research/p6-dxf-text-attrib-ocs-extrusion-2026-11-02.json), [INSERT OCS receipt](docs/research/p6-dxf-insert-ocs-extrusion-2026-11-02.json), the original [linear/angular DIMENSION receipt](docs/research/p6-dxf-dimension-ocs-extrusion-2026-11-02.json), and the [standard-subtype coverage receipt](docs/research/p6-dxf-dimension-standard-subtypes-ocs-2026-11-02.json). Generated AC1027 ASCII/Binary MLEADER text-content and block-content profiles use the existing adapter serializer after strict owner/style/reference closure. Typed readback verifies selected context/root/line fields, remapped style/text-style/LTYPE/BLOCK_RECORD roles, the block transform, and the referenced content BLOCK_RECORD body. Unrepresentable CMC data remains an atomic refusal with same-adapter correction/retry. This is generated transport evidence only; physical producer profiles and native annotation semantics remain open. See the [text-content adapter receipt](docs/research/p6-mleader-selected-adapter-dxf-transport-2026-11-02.json) and [block-content adapter receipt](docs/research/p6-mleader-block-content-adapter-dxf-transport-2026-11-02.json). A non-mutating typed view now exposes the pre-R2018 ACAD_MTEXT_COLUMN_INFO, ACAD_MTEXT_COLUMNS and ACAD_MTEXT_DEFINED_HEIGHT XDATA brackets while retaining raw XDATA, separate linked MTEXT callbacks and the distinct metadata, height-list, declared-link and actual-handle counts; generated ASCII/Binary round trips also verify remapping. This does not qualify source-producer breadth, linked-column aggregation, or native layout/rendering; see the [typed-view receipt](docs/research/p6-legacy-mtext-column-xdata-typed-view-2026-11-02.json). Continue qualifying those consumer and graph semantics outside the bounded R2018 embedded ATTRIB/ATTDEF context; other ATTRIB/ATTDEF variants; native dimension display, nonstandard DIMARC/LARGE_RADIAL placement, and DSTYLE/picture graphs; SHAPE glyph/font identity; other LEADER/MLEADER profiles and native annotation semantics; and derived-R12 appearance. Do not reopen the selected large-radial picture-block or R2018 embedded-MTEXT parser profiles without a new regression; see the [picture-block graph receipt](docs/research/p6-ac1032-large-radial-picture-block-resolution-2026-11-02.json) and [R2018 embedded-MTEXT receipt](docs/research/p6-ac1032-r2018-embedded-mtext-context-2026-11-02.json). |
| Geometry and clipping | R2013+ DWG fit SPLINEs with non-default square-root-chord or uniform knot parameterization now fail closed on DXF write because DXF has no selector and the choice changes curve shape; the documented chord default remains writable. A separate hand-encoded S6.2 AC1027/AC1024 parser control checks the versioned BL fields and following-sentinel alignment; the read-only local reference scan found AC1021 scenario-1/2 and AC1024 scenario-2 parser-smoke traces, but producer provenance is unverified and strict CLI export published no DXFs. This scan found no additional AC1027/AC1032 SPLINE record; the separate public AC1027 `example_2013.dwg` field differential remains bounded to its recorded fields and does not qualify whole-file support. Keep authentic per-version qualification open (see the [local corpus receipt](docs/research/p6-spline-local-reference-corpus-scan-2026-11-02.json), [AC1027 differential receipt](docs/research/p6-ac1027-fit-spline-field-differential-2026-11-01.json), and [boundary-test receipt](docs/research/p6-spline-r2013-bitlong-boundary-fast-test-2026-11-02.json)). ASCII/Binary refusal and corrected retry are covered. A generated ASCII/Binary reader control now covers classic 2D POLYLINE OCS conversion using the owning elevation while preserving 3D POLYLINE WCS vertices under `applyExtrusion`; this is not physical-producer or native-renderer qualification. See the [classic POLYLINE OCS receipt](docs/research/p6-dxf-classic-polyline-ocs-extrusion-2026-11-02.json). Generated ASCII/Binary controls also verify opt-in conversion of PDF/DGN/DWF UNDERLAY insertion positions from OCS/ECS to WCS while retaining local rotation and 2D clipping coordinates; no physical producer or native consumer claim follows. See the [UNDERLAY OCS receipt](docs/research/p6-dxf-underlay-ocs-extrusion-2026-11-02.json). Autodesk's SubDMesh API crease domain is now enforced at DXF/DWG read and write admission: -1, 0, or any positive finite value; generated controls cover negative-value refusal and the all-version DWG writer boundary. This closes only the scalar domain; crease-to-edge association and subdivision behavior remain unqualified. See the [MESH crease-domain receipt](docs/research/p6-mesh-crease-value-domain-2026-11-02.json). Curve equivalence/native interpretation, other SPLINE/NURBS/ELLIPSE/OCS/polyface/mesh semantics, remaining completed-pool/domain validation, physical SPATIAL_FILTER producer profiles, actual clipping application and native consumer semantics remain open. Generated local DXF writer admission now rejects self-intersecting SPATIAL_FILTER polygons per Autodesk API requirements; this is field-admission evidence only. See the [polygon guard receipt](docs/research/p6-spatial-filter-simple-polygon-dxf-guard-2026-11-02.json). See the [knot-parameterization guard receipt](docs/research/p6-ac1027-spline-fit-knot-parameterization-dxf-guard-2026-11-02.json). |
| Units and inheritance | Generated ASCII/Binary LTYPE controls enforce Autodesk's fixed-A group-72 alignment, signed-16-bit group-75 shape-number domain, and group-70 standard flags (16/32/64, with 32 requiring 16) on typed write admission; parsing preserves unsupported values. See the [group-72 receipt](docs/research/p6-ltype-alignment-dxf-domain-2026-11-02.json), [group-75 receipt](docs/research/p6-ltype-shape-number-dxf-width-2026-11-02.json), [group-70 receipt](docs/research/p6-ltype-standard-flags-dxf-domain-2026-11-02.json), and [group-74 read-tolerance receipt](docs/research/p6-ltype-element-flag-read-tolerance-2026-11-02.json). The group-74 parser retains nonnegative signed-16-bit values outside its modeled writer range; no interpretation or support claim follows. LTYPE description and complex-dash strings already share the retained-byte lease, with replacement, reset, copy/move, over-budget refusal, and corrected retry covered by the [retained-text receipt](docs/research/ltype-retained-text-owner-fast-slice-2026-10-17.json); do not reopen that implementation. Typed LTYPE output now refuses group-9 complex text above 255 encoded bytes after DXF version/code-page conversion, with ASCII/Binary boundaries, Unicode conversion, readback, whole-record rollback and corrected writer retry covered by the [group-9 guard receipt](docs/research/p6-ltype-group9-encoded-length-guard-2026-11-02.json). Autodesk's 2027 ObjectARX table gives group 9 a 255-character maximum (less for Unicode), while its 2024 group-value-type table gives codes 0-9 a 2049-single-byte-character allowance from AutoCAD 2000 onward; the direct-field/group-range references conflict. The 255-byte writer bound is a conservative interim admission rule, not proof of the maximum valid producer value; independent producer/native acceptance and the larger-string profile remain unqualified. DXF HEADER output now refuses explicit zero or non-finite `$LTSCALE` while retaining tolerant input and the absent-value default of 1.0; ASCII/Binary refusal, read retention and same-writer correction retry are covered by the [LTSCALE receipt](docs/research/p6-ltscale-header-zero-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls now also reject malformed explicit `$CELTSCALE` group/type, conflicting aliases, and non-finite values instead of silently replacing a wrong-typed value with the absent default 1.0; finite positive, zero and negative values remain writable because Autodesk documents no sign/zero domain. Tolerant input, ASCII/Binary rollback and corrected retry are covered by the [CELTSCALE receipt](docs/research/p6-celtscale-header-field-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls also enforce the documented 0/1 domains for `$PSLTSCALE` and `$MSLTSCALE`, retain unsupported values on input, keep the absent PSLTSCALE default of 1, leave absent MSLTSCALE omitted, and refuse MSLTSCALE for pre-AC1021 output. Autodesk dates MSLTSCALE to AutoCAD 2008, but AC1021 spans AutoCAD 2007–2009, so the current target enum cannot distinguish that product-version boundary. See the [linetype display-scale receipt](docs/research/p6-linetype-display-scale-dxf-domain-2026-11-02.json). These are field-admission controls only; native linetype appearance remains unqualified. Generated ASCII/Binary HEADER controls now require explicit `$MEASUREMENT` to be the documented integer 0 (imperial) or 1 (metric), retain unsupported values on tolerant input, and preserve absence independently of `$INSUNITS`; the existing AC1015/AC1032 presence and unchanged-geometry selector also passes. See the [MEASUREMENT domain receipt](docs/research/p6-measurement-header-dxf-domain-2026-11-02.json). This establishes scalar admission only; hatch/linetype resource selection remains unqualified. Generated SCALE controls require positive finite paper/drawing units on DXF read/write; group 70 remains preserved on input but nonzero values are refused on typed output because ODA v5.4.1 identifies it as unknown and writes zero. These are field-admission guards only, not native annotation-scale qualification. See the [SCALE units receipt](docs/research/p6-scale-units-dxf-domain-2026-11-02.json) and [SCALE flag receipt](docs/research/p6-scale-unknown-flag-dxf-output-guard-2026-11-02.json). Generated ASCII/Binary HEADER controls now enforce the documented group-2 string representation for `$UCSBASE` and `$PUCSBASE`, retain malformed values tolerantly, and refuse wrong types/groups, aliases, and case-folded keys with atomic rollback and corrected retry; this does not resolve a named UCS table target or qualify orthographic restoration. See the [UCS base field receipt](docs/research/p6-ucs-base-header-fields-guard-2026-11-02.json). Strict output now resolves nonempty ASCII UCSBASE/PUCSBASE values to unique output UCS records, admits blank and built-in WORLD values, and keeps differing non-ASCII case variants refused; see the [UCS base target receipt](docs/research/p6-ucs-base-name-target-closure-2026-11-02.json). Remaining work: native linetype resource selection/rendering across `$MEASUREMENT`, LTSCALE/CELTSCALE, `$PSLTSCALE` and `$MSLTSCALE`, including exact behavior for AutoCAD 2007 versus 2008–2009 inside the shared AC1021 target; native orthographic restoration and non-ASCII UCS case collation; transparency/ByLayer/ByBlock meaning; remaining SCALE version/API boundaries; and Template description/survey policies. ODA §22 byte-count framing remains supported across all six reader versions. A well-bounded non-empty byte-count description is decoded with the recognized source codepage and delivered to tolerant DWG consumers through an optional trailing `DRW_Interface` callback; the following MEASUREMENT is also published. A warning still makes strict-complete DXF output refuse the description because DXF has no matching HEADER field. Modern TU16-shaped descriptions remain readable only with a warning and no guessed MEASUREMENT: LibreDWG 0.13.3 uses TU16 in its R2007+ T16 path, which conflicts with ODA §22. Its local writer/reader round trip preserves a non-empty description, but libdxfrw rejects that generated DWG earlier at the file-header stage, so it is not parser validation. Generated controls cover maximum signed length, exact logical-section bounds, empty descriptions, legacy modern UTF-16-NUL compatibility, codepage decoding/callback delivery, unknown-codepage fail-closed behavior, byte-count measurement publication, TU16-shaped warning-only behavior and malformed framing. Missing mandatory AcDb:Template sections remain tolerant on read but now refuse strict-complete output for AC1018+; absence remains optional for AC1012-AC1015. See the [missing-section strict-output receipt](docs/research/p6-template-missing-section-strict-output-guard-2026-11-02.json). Description text still has no DXF representation; strict DXF preservation, producer breadth, native semantics and broader survey policies remain open. See the [Template frame receipt](docs/research/p6-template-description-frame-2026-11-02.json) and [description strict-output guard receipt](docs/research/p6-template-description-strict-output-guard-2026-11-02.json). |
| INSUNITS scalar domain | Autodesk documents integer group 70 values 0–24. The public `DRW_Header::Units` enum now includes codes 21–24, and the measurement helper classifies all four as English units; DXF output validates the canonical scalar and refuses it for pre-AC1015 targets, while DWG HEADER encoding rejects malformed or out-of-domain values before writing bits. Generated ASCII/Binary controls round-trip 0–24, preserve invalid input tolerantly, and verify failure/retry; direct DWG HEADER controls cover all six writer revisions. This is scalar representation only. Autodesk notes code 21 has release-specific support and codes 22–24 are AutoCAD for Mac only, so insertion scaling and native platform behavior remain unqualified. See the [INSUNITS receipt](docs/research/p6-insunits-domain-guard-2026-11-02.json). |
| Lineweight and display defaults | DXF HEADER output now validates `$CELWEIGHT` against the enumerated values (including special -3/-2/-1 values), `$ENDCAPS`/`$JOINSTYLE` as 0–3, and `$LWDISPLAY`/`$XEDIT` as 0/1. Generated ASCII/Binary tests cover every admitted value, malformed types/groups/aliases, atomic refusal and same-writer retry; tolerant input retains unsupported integer values. This is scalar admission only; rendering, plotting, and native behavior remain unqualified. See the [lineweight HEADER receipt](docs/research/p6-lineweight-header-domain-guard-2026-11-02.json). |
| Orientation and attribute visibility | DXF HEADER output now validates `$ANGDIR` as 0/1 and `$ATTMODE` as 0/1/2. Generated ASCII/Binary controls cover valid readback, malformed type/group/aliases, atomic refusal, corrected retry, and tolerant input retention. Legacy `$ATTDIA` and `$ATTREQ` now enforce the Autodesk R12 DXF group-70 domain 0/1 only below AC1012, matching the existing writer emission branch; AC1012, AC1014, and AC1027 controls verify these omitted values do not block output. This verifies legacy field-domain transport only and does not qualify native INSERT or prompt behavior; see the [legacy attribute-prompt receipt](docs/research/p6-attdia-attreq-legacy-header-domain-guard-2026-11-02.json). Autodesk's system-variable reference enumerates `$MIRRTEXT` as integer 0/1, while the DXF table's “if nonzero” wording describes the Boolean effect; the statements are compatible. HEADER output now enforces the 0/1 domain with ASCII/Binary readback, refusal, retry, and tolerant-input controls; see the [MIRRTEXT resolution receipt](docs/research/p6-mirrtext-header-domain-resolution-2026-11-02.json). These field-domain checks do not qualify native mirroring or attribute display behavior. See the [orientation/attribute HEADER receipt](docs/research/p6-angdir-attmode-header-domain-guard-2026-11-02.json). |
| Unknown/proxy data | A bounded same-revision AC1018+ `ACAD_PROXY_ENTITY` serializer now transports the modeled class ordinal and entity-data payload for generated no-reference profiles with ASCII/Binary readback. Its low-level writer also round-trips resolver-backed object-ID groups in generated controls; the dwg2dxf adapter now binds unique top-level typed entity targets and final emission for pointer handle codes 4/5 and offset soft-pointer codes 6/8/10/12, mapped to groups 330/340. Owner-role/unknown codes, nested/raw/OBJECTS targets, recursive dependencies, physical source profiles, and native semantics remain fail-closed or unqualified. Unknown-data slots, EED/common reference closure, mixed ACDS graphs, and native semantics remain open; unsupported profiles refuse atomically. Additional proxy primitives need independent semantics, not cache-byte agreement. Associative output and replay belong to P4. See the [bounded proxy transport receipt](docs/research/p4-dwg-proxy-entity-bounded-dxf-transport-2026-11-02.json) and [object-ID transport receipt](docs/research/p4-dwg-proxy-entity-object-id-dxf-transport-2026-11-02.json). |
| Safety and caller state | OPEN. Continue auditing remaining setup, assembly and work bounds, especially other `writeObjects` maps/vectors; inject allocation failures across independent builders; broaden rollback/failure injection to other staged families; current controls cover the GROUP handle planner plus core IMAGEDEF and rooted named-dictionary OBJECTS all-allocation-offset sweeps, plus IMAGEDEF callback validation/bad_alloc/retry. The GROUP planner sweeps every measured allocation in ASCII/Binary configurations, confirms no partial plan is retained, and retries the same codec successfully ([GROUP planner sweep](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json)). A reservation allocation failure inside an admitted write now fails that transaction without poisoning its same-codec retry; pre-write reservation failures retain their one-attempt generation guard. The ASCII/Binary IMAGEDEF sweep now checks each calibrated allocation before the consumer OBJECTS callback for atomic rollback, destination and identity preservation, same-writer retry, and typed readback ([IMAGEDEF allocation sweep](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json)). The rooted named-dictionary sweep does the same for every measured allocation, including the handle-reservation failure that previously caused a stale retry rejection; it checks typed DICTIONARY readback and the pre-write reservation guard ([named-dictionary allocation sweep](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json)). The generated NOD/GROUP OBJECTS path now sweeps every calibrated allocation in ASCII and Binary, checks destination and derived-identity rollback before the consumer callback, retries the same writer, and reads back the NOD-to-GROUP-dictionary-to-GROUP handle graph ([generated GROUP OBJECTS allocation sweep](docs/research/p6-dxf-generated-group-dictionary-objects-allocation-retry-2026-11-03.json)). The parallel generated NOD/IMAGE path sweeps every calibrated allocation with a planned IMAGEDEF and source IMAGE dictionary, then checks same-writer retry and typed NOD-to-image-dictionary-to-IMAGEDEF plus IMAGE-reference readback ([generated IMAGE dictionary OBJECTS allocation sweep](docs/research/p6-dxf-generated-image-dictionary-objects-allocation-retry-2026-11-03.json)). Both are local transaction-safety evidence and make no producer or semantic support claim. Generated NOD/GROUP/IMAGE dictionary-source admission also sweeps allocation failures through root-only and rooted child-dictionary profiles, preserving prior owned state and retrying the same codec after each injected failure ([source admission sweep](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json)). The SECTIONMANAGER/SECTIONSETTINGS `writeObjects` path now sweeps every measured allocation offset for generated ASCII/Binary graphs; each failure preserves the destination and the same adapter retries with fresh graph readback in normal and ASan builds ([SECTION OBJECTS allocation receipt](docs/research/p6-dxf-section-objects-allocation-failure-retry-2026-11-03.json)). The generated POINTCLOUDDEFINITIONEX/REACTOREX OBJECTS callback sweeps every calibrated allocation offset in ASCII and Binary: each failure preserves the destination sentinel, and same-adapter retry plus typed readback verifies NOD → point-cloud dictionary → definition/reactor → entity references. The companion classic POINTCLOUDDEFINITION/REACTOR route now has the same ASCII/Binary all-offset rollback/retry/readback sweep; its handle-remapped cloud identity is verified through reciprocal definition/reactor edges rather than assumed numeric identity. Both point-cloud controls use locally authored graphs and make no producer or semantic support claim ([updated receipt](docs/research/p6-pointcloud-objects-allocation-failure-retry-2026-11-03.json)). The generated AC1027 LAYOUT/RapidRT render-preset OBJECTS path now sweeps 57 ASCII and 56 Binary callback allocation offsets, preserving the prior destination and source graph and proving same-adapter retry plus fresh LAYOUT/render-settings graph readback; the source DXF is created under test scratch and removed ([receipt](docs/research/p6-dxf-layout-render-settings-objects-allocation-retry-2026-11-08.json)). This is local graph/transaction evidence only. The consumer `writeObjects()` callback also has an ASCII/Binary thrown-`std::bad_alloc` control after one XRECORD is staged: the write fails atomically, preserves the prior destination and source-object bindings, skips finalization, then succeeds on same-codec retry with typed readback ([receipt](docs/research/p6-dxf-objects-callback-bad-alloc-rollback-retry-2026-11-03.json)). A generated direct NOD-to-XRECORD OBJECTS profile now sweeps six ASCII and five Binary allocation offsets; each injected failure preserves the destination and caller graph, and same-adapter retry verifies typed owner/payload readback ([receipt](docs/research/p6-dxf-xrecord-objects-allocation-failure-retry-2026-11-03.json)). This closes only that emitter path. A generated AC1027 DXF-source nested dictionary with two DICTIONARYVAR records now sweeps 62 ASCII and 60 Binary OBJECTS-callback allocation offsets; each failure preserves the destination and caller graph, and same-adapter retry verifies both values and their nested owner ([receipt](docs/research/p6-dxf-dictionaryvar-objects-allocation-failure-retry-2026-11-03.json)). This closes only that emitter path. The generated PDF/DGN/DWF underlay-definition OBJECTS graph now sweeps all 60 ASCII and 53 Binary callback allocation offsets, preserving the destination sentinel and succeeding on same-adapter retry with typed definition/entity graph readback ([receipt](docs/research/p6-dxf-underlay-objects-allocation-failure-retry-2026-11-05.json)); this closes only that local transaction path. The generated AC1027 SUNSTUDY OBJECTS callback now sweeps 20 ASCII and 18 Binary allocation offsets with destination rollback, same-adapter retry and typed graph readback in normal and ASan builds ([receipt](docs/research/p6-dxf-sunstudy-objects-allocation-failure-retry-2026-11-05.json)); its source DXF is authored locally for the test and removed after each run. The generated AC1027 direct-NOD RASTERVARIABLES OBJECTS callback now sweeps 11 ASCII and 10 Binary allocation offsets, with destination and caller-record preservation, same-adapter retry and typed readback in normal and ASan builds ([receipt](docs/research/p6-dxf-rastervariables-objects-allocation-failure-retry-2026-11-05.json)); its source DWG is authored locally and removed after each run. The generated AC1027 direct-NOD WIPEOUTVARIABLES OBJECTS callback now sweeps 11 ASCII and 10 Binary allocation offsets, with destination and caller-record preservation, same-adapter retry and typed readback in normal and ASan builds ([receipt](docs/research/p6-dxf-wipeoutvariables-objects-allocation-failure-retry-2026-11-05.json)); its source DWG is authored locally and removed after each run. A generated AC1024 ACAD_MATERIAL projection now sweeps all 37 ASCII and 36 Binary OBJECTS callback allocations, preserving the destination and source material after every fault and proving same-adapter retry plus NOD/dictionary/material readback; it adds no DWG/DXF fixture ([receipt](docs/research/p6-dxf-material-objects-allocation-failure-retry-2026-11-08.json)). A generated AC1027 PLOTSETTINGS graph now sweeps 21 ASCII and 20 Binary callback allocation offsets, preserving the destination and caller graph and proving same-adapter retry plus typed NOD/dictionary/PLOTSETTINGS readback; its DXF input is an in-memory string and no fixture file is added ([receipt](docs/research/p6-dxf-plotsettings-objects-allocation-failure-retry-2026-11-08.json)). A generated AC1027 ACAD_PLOTSTYLENAME `DICTIONARYWDFLT`/`PLACEHOLDER` graph now sweeps 25 ASCII and 24 Binary callback allocation offsets, preserving the destination, caller records and HEADER plot-style binding, then proving same-adapter retry and typed readback. Its temporary DXF input is created locally and removed after a successful run; no fixture is committed ([receipt](docs/research/p6-dxf-dictionary-default-objects-allocation-failure-retry-2026-11-08.json)). A generated AC1027 NOD-to-named-DICTIONARY-to-typed-SCALE OBJECTS callback now sweeps 12 ASCII and 12 Binary allocation offsets, checks destination and source graph preservation, and retries the same writer with typed readback; this closes only the typed SCALE writer profile, not DXF-origin raw SCALE replay or native/producer semantics ([receipt](docs/research/p6-dxf-scale-objects-allocation-failure-retry-2026-11-08.json)). A generated AC1027 NOD-to-named-DICTIONARY-to-DATALINK OBJECTS graph with two custom-data references and two target DATALINK records now sweeps 28 ASCII and 25 Binary callback allocation offsets, preserving destination and caller state before same-writer retry and typed graph readback; this closes only that generated typed writer path ([receipt](docs/research/p6-dxf-datalink-objects-allocation-failure-retry-2026-11-08.json])). A locally authored AC1027 direct GEODATA OBJECTS callback now sweeps all 11 measured ASCII and 11 Binary allocation offsets, with one-shot allocation faults, destination and caller-record preservation, same-codec retry, and typed readback; this is generated callback transaction evidence only ([receipt](docs/research/p6-dxf-geodata-objects-allocation-failure-retry-2026-11-09.json)). A generated AC1027 DBCOLOR OBJECTS callback now sweeps eight ASCII and eight Binary allocation offsets, preserves destination/caller state, and requires same-codec retry to reproduce the clean calibration bytes and typed readback. This exposed and fixed attempt-local handle reservations leaking into retry HANDSEED; caller reservations made before a write remain explicit. The control is generated transaction evidence only ([receipt](docs/research/p6-dxf-dbcolor-objects-allocation-failure-retry-2026-11-09.json)). A generated AC1027 IMAGEBACKGROUND OBJECTS callback also sweeps nine ASCII and nine Binary allocation offsets; each failure preserves destination and caller state, and same-codec retry reproduces clean bytes plus typed image-background readback. It establishes local serializer transaction behavior only, without an ACAD_BACKGROUND graph or image rendering claim ([receipt](docs/research/p6-dxf-background-objects-allocation-failure-retry-2026-11-09.json)). A direct generated AC1027 SPATIAL_FILTER writer sweep covers nine ASCII and nine Binary allocation offsets, preserving the existing destination and caller record before same-codec exact-byte retry and typed field readback. Extension-dictionary ownership closure and native clipping remain outside this local transaction proof ([receipt](docs/research/p6-dxf-spatial-filter-objects-allocation-failure-retry-2026-11-09.json)). A standalone generated AC1027 scalar TABLECONTENT OBJECTS callback now sweeps 12 ASCII and 10 Binary allocation offsets with destination/caller preservation, same-writer retry and typed grid/value readback; its readback is not semantically complete and does not authorize strict output or feature claims ([receipt](docs/research/p6-dxf-tablecontent-objects-allocation-failure-retry-2026-11-08.json])). A generated AC1024 NOD-to-DICTIONARY-to-ACDBASSOCNETWORK-to-ACDBASSOCVARIABLE OBJECTS callback sweeps 26 ASCII and 24 Binary allocation offsets with destination/caller preservation, same-writer retry and typed graph readback; the variable `m_associativeVariableWireComplete` flag remains false on readback, so its full wire/support profile stays gated ([receipt](docs/research/p6-dxf-associative-network-objects-allocation-failure-retry-2026-11-08.json])). These controls close only their bounded callback exception paths; allocations inside other OBJECTS builders and broader whole-drawing failure states remain open. A focused four-worker ASCII/Binary codec test now writes and imports separate files concurrently in normal and ASan builds ([scoped codec concurrency receipt](docs/research/p6-scoped-dxf-codec-concurrency-2026-11-03.json)); this qualifies only isolated dxfRW instances and distinct files, not shared-instance concurrency or general thread safety. Inspector callback closures and borrowed-view lifetime are now explicit: a focused normal/ASan control proves the codec owns value captures through invocation and releases them when cleared; reference captures remain caller-owned. Other source-lifetime boundaries and platform numeric/string policies remain open ([receipt](docs/research/p6-dxf-inspector-closure-lifetime-2026-11-03.json)). XDATA APPID preflight now catches allocation failures during group validation and stages additions transactionally, preserving the prior required-APPID set if any allocation fails. A focused ASCII preflight sweep reaches nine injected failures, confirms no partial set survives each, and verifies fresh-codec admission retry; this closes only the APPID set mutation path, not broader OBJECTS builders or shared-instance concurrency ([receipt](docs/research/p6-dxf-xdata-appid-admission-allocation-rollback-2026-11-05.json)). Completed bounded staging families: GROUP, IMAGEDEF plan, named/root dictionaries, CLASS, raw sections, resolved references, TABLESTYLE/BG/SUN registries, source BLOCK_RECORD and canonical LTYPE. The controls use internal logical resource estimates and do not create format maxima or support claims. Evidence: [GROUP count](docs/research/p6-dxf-group-vector-setter-count-guard-2026-11-03.json), [GROUP nested bounds](docs/research/p6-dxf-group-nested-setter-bounds-2026-11-03.json), [GROUP plan allocation retry](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json), [generated dictionary source allocation](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json), [IMAGEDEF count](docs/research/p6-dxf-image-definition-count-and-staging-guard-2026-11-03.json), [IMAGEDEF plan bounds](docs/research/p6-dxf-image-definition-plan-nested-setter-bounds-2026-11-03.json), [IMAGEDEF OBJECTS rollback](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json), [core named-dictionary OBJECTS rollback](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json), [generated GROUP OBJECTS rollback and readback](docs/research/p6-dxf-generated-group-dictionary-objects-allocation-retry-2026-11-03.json), [generated IMAGE dictionary OBJECTS rollback and readback](docs/research/p6-dxf-generated-image-dictionary-objects-allocation-retry-2026-11-03.json), [named-dictionary count](docs/research/p6-dxf-named-dictionary-object-setter-count-guard-2026-11-03.json), [named-dictionary nested bounds](docs/research/p6-dxf-named-dictionary-object-nested-setter-bounds-2026-11-03.json), [root dictionary count](docs/research/p6-dxf-root-dictionary-setter-count-guard-2026-11-03.json), [root dictionary byte bounds](docs/research/p6-dxf-root-dictionary-byte-bound-2026-11-03.json), [CLASS count](docs/research/p6-dxf-class-setter-count-guard-2026-11-03.json), [CLASS byte bounds](docs/research/p6-dxf-class-metadata-setter-byte-bound-2026-11-03.json), [raw sections](docs/research/p6-dxf-raw-section-setter-bounds-2026-11-03.json), [resolved references](docs/research/p6-dxf-resolved-write-reference-setter-bounds-2026-11-03.json), [handle remap](docs/research/p6-dxf-handle-remap-setter-bound-2026-11-03.json), [source table metadata](docs/research/p6-dxf-source-table-metadata-setter-bounds-2026-11-03.json), [scoped DXF codec concurrency](docs/research/p6-scoped-dxf-codec-concurrency-2026-11-03.json). | Keep validation focused; do not reopen a passed guard without a new failure. |
| CLI and oracle contracts | Single-file output/input filesystem-alias refusal is implemented for positional and FreeCAD `-o` routes; the fast CLI selector covers same-path, symlink and hard-link aliases and verifies source preservation (see the [alias guard receipt](docs/research/p6-cli-single-file-input-output-alias-guard-2026-11-02.json)). Generated source-DXF FreeCAD `-o` controls now bind default target selection for all nine supported revision headers in ASCII and Binary, including AC1012→AC1014. Locally authored DWGs now cover every DWG-writer version (AC1015, AC1018, AC1021, AC1024, AC1027 and AC1032). The generated sources succeed at their default targets in ASCII/Binary, with binary readback at the same revision. The bounded AC1018 minimal root/empty-group source also succeeds at explicit AC1024 ([receipt](docs/research/p4-ac1018-minimal-root-group-output-2026-11-04.json)); broader AC1018 graphs remain open. A debug-only preflight projection now exposes source format/revision, selected target revision, implicit-versus-explicit selection, encoding and input-appropriate read-completeness mode before writer setup. Generated AC1012 DXF input verifies its AC1014 default; AC1006 input verifies a fail-closed unavailable default; generated AC1021 and AC1018 default/explicit successes verify the projection before output preflight. An external AC1014 sample also selects AC1014 but is refused because two source objects fail parsing and the frame disposition is incomplete; this is diagnostic evidence only, and the sample is not committed. See the [default target-version receipt](docs/research/p6-cli-freecad-default-target-version-matrix-2026-11-02.json). Remaining contracts: eligible AC1012/AC1014 DWG-source output/readback; other partial-read failure profiles (the six-writer-version generated control covers ASCII and Binary strict refusal, opt-in output and readback; the original ASCII control is recorded in the [all-version receipt](docs/research/p6-cli-partial-dwg-read-all-writer-versions-2026-11-02.json)); and broader CLI projections: broader HEADER structure/repairability profiles beyond generated duplicate `$ACADVER`/`$LTSCALE` variables and repeated group 10 in `$INSBASE`/`$EXTMIN`; the API repair workflow for duplicate variables/repeated group codes is covered by the ASCII/Binary same-adapter replacement/retry control in [the HEADER receipt](docs/research/par27-header-repeated-group-refusal-fast-slice-2026-10-22.json); broader malformed structures remain open; `$REQUIREDVERSIONS` malformed/repeated diagnostic controls now cover duplicate variable, repeated group 160, and wrong group code with AC1032 atomic refusal; feature semantics/native acceptance beyond one generated bit preserved through AC1032 typed DXF output/readback plus AC1024 refusal remain open; decoder issue-mask projection now exercises all 15 defined `DxfTextIssue` flags across 14 locally authored profiles with exact-mask and atomic-refusal checks; see the [CLI issue-mask matrix receipt](docs/research/p6-cli-text-decoder-issue-mask-matrix-2026-11-02.json); skipped-record projections cover one and three same-class instances plus three distinct AC1027 custom class names; and the source-disposition projection now also exposes DWG entity/object parse-failure counts and all seven HANDLE-frame dispositions, checked against generated partial-read sources across AC1015–AC1032. See the [frame-projection receipt](docs/research/p6-cli-source-disposition-frame-projection-2026-11-07.json). Named skipped-record totals remain separate from frame/parse counters and are not combined into semantic-loss counts. Other skip/recovery categories, revisions, broader HEADER structures, and native/feature semantics remain open. The CLI applies no repair; these projections diagnose only. See the [source-disposition projection receipt](docs/research/p6-cli-source-disposition-projection-2026-11-02.json).

**ODA ARM64 package integrity recheck:** Native verification outside the sandbox
passes for the installed 27.1 app and the downloaded 27.9 package and extracted
app; earlier sandboxed signature failures were unreliable. The installed 27.1
package produced the currently qualified AC1027-to-AC1027 Cover resaves through
the GUI route; direct CLI retries continued to abort during startup.
The validated trailer ends at byte 844; the original has 731 unclaimed HEADER
bytes and same-revision ODA outputs have 739. LibreDWG 0.14 independently
decompressed earlier original/resave sections. A tenth bounded direct retry
after the user again unlocked the Mac aborted during PasteBoard/XPC
initialization (SIGABRT after 0.5 seconds) and produced no output. The changed
session state therefore does not unblock this CLI launch path; do not repeat
the identical invocation. No new suffix comparison was possible. The prior
successful resave had no bytewise suffix matches against five previously
compared same-source outputs; pairwise low-bit patterns remain structured but
unexplained. That output reads 119/119 frames with no entity/object parse
failures, then strict DXF preparation refuses on unqualified source references.
Keep the suffix unclaimed and strict output refused. The functioning GUI route
has already reached document processing, so do not repeat the identical export.
Continue only with a controlled source mutation that can discriminate candidate
suffix dependencies, or a new independently decoded/spec-defined field mapping.
See the [latest retry
receipt](docs/research/p6-ac1027-oda-header-suffix-retry-2026-11-07b.json),
[integrity correction](docs/research/p6-ac1027-cover-oda-arm64-package-integrity-2026-11-02.json)
and [same-version resave receipt](docs/research/p6-ac1027-cover-oda-same-version-resave-2026-11-03.json).

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
ACadSharp issue [#1261](https://github.com/DomCR/ACadSharp/issues/1261)
reports two private drawings where a R2009 GEODATA class-version-1 object
needs a trailing Civil 3D block after its mesh. The report does not identify
the target DWG revision. This implementation already writes and parses the
literal tail for AC1021 class version 1 and keeps the AC1024 exception
separate from object version; the fast matrix verifies AC1021/AC1024 v1 and
v2 bodies, exact tail ordering, and that AC1027/AC1032 omit the tail. The
focused selector passed on 2026-11-05. Treat #1261 as corroboration for the
R2009 object profile, not evidence to extend the tail to later target releases
or to close physical/native qualification; see the [cross-check receipt](docs/research/p4-geodata-acadsharp-civil-tail-crosscheck-2026-11-05.json).
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

**Classic POLYLINE closed-N topology guard:** DXF output now refuses parent
flag bit 32 unless polygon-mesh bit 16 is also present. Without the mesh bit,
the prior writer emitted an `AcDb2dPolyline` parent while tagging its vertices
as polygon-mesh vertices. The ASCII/Binary fast control checks atomic
destination preservation, same-adapter correction/retry, typed readback, and
the valid 16|32 closed-N mesh profile. The neighboring WCS extrusion-loss
guard still passes. This is output-field consistency evidence only; it does
not qualify mesh geometry or native behavior ([receipt](docs/research/p6-dxf-polyline-closed-n-mesh-flag-guard-2026-11-07.json)).

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
- **Completed bounded AC1021 inline-SAB selected projection:** the exact physical E3DSOLID source at handle `0x452` now writes its version-2 SAB as SAT v1 to AC1024 ASCII and Binary DXF. The selected 0x452→0x453→0x454→0x455 history/EvaluationGraph/torus graph is closed, output handles are remapped and read back, and atomic refusal plus same-adapter retry pass. This route is independent of ACDSDATA and does not qualify whole-source output, geometry semantics, native CAD acceptance, or other source revisions; full-source export still stops at the unrelated IMAGEDEF owner-role gate. No fixture was added. See the [selected carrier receipt](docs/research/p4-ac1021-inline-sab-torus-dxf-carrier-gate-2026-11-08.json).
- **SAT serialization allocation safety:** `drw_convertSabToSatV1` now catches allocation failures during text assembly and returns failure before swapping the staged output. The new generated allocation sweep injects failure at all 11 observed allocation positions; every failure returns `false`, preserves the caller buffer, and succeeds on retry with the same buffer. The focused fast ACIS/DXF cohort passes 4/4. This is transaction/failure evidence only; the native B-rep and independent-kernel gates remain open. See the [P7 parser receipt](docs/research/p7-ac1021-inventorloader-sat-parser-crosscheck-2026-11-08.json).
- **AC1018 SAT-v1 extension candidate checked:** two existing local AC1018 samples were traced with a temporary diagnostic that was removed immediately afterward. All six observed REGION/3DSOLID modeler headers declare version 2; neither sample supplies a physical SAT-v1 block frame for broadening AC1015 extraction. The exact sources, trace hashes, body offsets, and limitations are recorded in the [AC1018 SAT-v1 profile receipt](docs/research/p7-ac1018-sat-v1-profile-candidate-screen-2026-11-09.json). Keep AC1018 v1 carriers opaque unless a qualifying physical source or authoritative versioned rule supports the extension; the generated AC1018 opaque-control remains in place. This negative screen does not qualify v2 semantics or overall AC1018 modeler support.
- **Raw SAB pipeline allocation rollback:** the generated in-memory v21800 point→vertex profile with heap-backed header strings now exercises the public parser, graph builder, wireframe decoder, and `WithHeader` decoder independently. Exhaustive reached-allocation sweeps cover 14 parser, 9 graph-builder, and 26 wireframe/header failure points. Each API returns false, clears its documented output, records allocation failure, and succeeds on same-object retry; parser graph tokens, resolved vertex-to-point identity, the final vertex coordinates/bounds, and all header values are compared. A heap-backed header copy fault had exposed partial caller-header publication; commit `304f6fc7` changes final publication to a statically verified nonthrowing move. Decoder `complete` retains stage semantics and can already be true if projection allocation fails after framing. The focused four-test ACIS/ACSH cohort passes. These are local failure/retry controls only, not producer, B-rep, kernel, or support evidence. No fixture file was added; native B-rep and independent-kernel gates remain open. See the [P7 parser receipt](docs/research/p7-ac1021-inventorloader-sat-parser-crosscheck-2026-11-08.json).
- **Native FreeCAD B-rep check completed for the exact selected torus SAT payload.** The 1,057-byte SAT output (SHA-256 `4917d971df23651987e01f18cf8635d19dab7ec2c306c910734ba0093641da08`) was parsed by InventorLoader's pinned `Acis.py` (`e94bdf5e29052a0dc7ce6fdf755e956ae507caec`, parser SHA-256 `ec84f9528355f0df2e28bd94eecf7d9e4be89c5b9f7a77eef9b9cccbc21366bb`) and reconstructed through native FreeCAD 1.1.4 Part/OpenCASCADE on Apple M2 Max arm64. Running `freecadcmd` outside the Codex sandbox allowed Qt's `hw.optional.neon` probe; the prior sandbox-only CPU-feature abort is resolved for this CLI route. The pinned `importerSAT` face-build sequence produced one valid closed shell; `Part.makeSolid` produced one valid solid with one face, two edges and one vertex. Its center/bounds agree with the selected torus fields; analytic torus area and volume differ by 4.13e-15 and 4.67e-15 relative error. Exact command/runtime, hashes and metrics are in the [native B-rep receipt](docs/research/p7-ac1021-freecad-native-torus-brep-2026-11-08.json). This is one selected geometry profile only: InventorLoader and the native Part module share OpenCASCADE, so an independent-kernel comparison, source-native acceptance, GUI import, broader version coverage and whole-source output remain open.
- **Independent parser screen for the selected AC1021 torus SAT:** the locally available ezdxf source parser at revision `dd7f6cc34983fdbf0b2b910f2497e0720d06dd16` rejects the exact 21500-version payload in its normal loader because the modified record shape does not match its >=700 entity-ID assumption. A forced low-level token parse identifies 13 records, including one `torus-surface`, but pointer resolution stops on `eye_refinement` token `$3000`; this does not validate roles, topology, placement or geometry. ezdxf also has no ACIS kernel. This candidate is now screened for this exact input; do not count it as independent semantic or kernel evidence. Keep that gate open and try a materially different reader/profile only ([receipt](docs/research/p7-ac1021-ezdxf-sat-parser-boundary-2026-11-09.json)).
- **ODA desktop conversion is now available after unlocking the macOS session.** An audited ODA File Converter GUI run converted the online AC1024 `nextgis/dwg_samples/arc_2010.dwg` to 2010 ASCII DXF; both input and output hashes and the source's AutoCAD 2016 producer claim are recorded in the [AC1024 online sample receipt](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json). This confirms the desktop converter route works, but this source is not an AutoCAD 2010 RTM witness and says nothing about ACIS geometry parsing. The earlier CLI startup attempt while the session was locked still failed with PasteBoard/XPC errors and is not file-format evidence. The FreeCAD CLI geometry check succeeds through an unsandboxed invocation; the regular Codex sandbox still blocks Qt hardware-feature probing. No independent ACIS reader is installed in the user add-on directory, so the check used the hash-pinned scratch copy of InventorLoader Acis.py. The independent-kernel and native-source gates remain open.
- **Additional online AC1024 samples checked:** downloaded `line_2010`, `circle_2010`, `point3d_2010`, and `polyline2d_line_2010` DWG/DXF pairs from the public `nextgis/dwg_samples` repository into `/tmp`. All four DWGs reached `dwgReader24::readDwgClasses END`, had zero entity/object parse failures and no CLASSES CRC mismatch; strict DWG-to-DXF output still stopped later at completeness or extension-graph preflight. The repository identifies AutoCAD 2016 M.49.0.0 as producer, so these do not reproduce or resolve the distinct AutoCAD 2010 RTM high-bit class-size report. The broader source search and screening are complete for the online Autodesk and LibreDWG AC1024 collections: all 28 recorded candidates reached the CLASSES end marker without a CRC mismatch, and none is a verified AutoCAD 2010 RTM witness. Do not repeat those screens. The remaining blocker is the missing tenth input from the reported local corpus, an exact failing hash, or an independently verified RTM-produced sample. Do not touch `strDataSize` before reproducing `BAD_READ_CLASSES`; keep external samples uncommitted. Search sources and limitations are in the [online sample source-search receipt](docs/research/p6-ac1024-online-sample-source-search-2026-11-09.json). The tested-pair hashes, provenance and outcomes remain in the [AC1024 online sample receipt](docs/research/p6-ac1024-online-sample-rtm-nonreproduction-2026-11-09.json); no external DWG/DXF was added to the repository.
- **ODA CLI retry after the new unlock succeeded:** ODA File Converter 27.1.0.0 initialized with exit status 0 and converted the existing committed AC1021 `dwg-writer/tests/fixtures/committed/libredwg/atmos_dc22s_2007.dwg` to ACAD2010 and ACAD2007 ASCII DXF. Both outputs retain 3DSOLID handle `0x452`, its group-350 history handle `0x453`, and 18 proprietary modeler-data chunks (17 group 1 and one group 3; 1,058 concatenated characters). The initial inspection saw an encoded prefix (`mnmoo nl m mi`); the follow-up receipt below records recovery of the selected 0x452 SAT 21200 payload and its bounded semantic cross-check. The desktop-access/CLI startup gate is resolved, while independent-kernel, native modeler, whole-source output and broader support gates stay open. Hashes and conversion details are in the [ODA AC1021 peer-conversion receipt](docs/research/p7-oda-ac1021-dwg-to-dxf-opaque-solid-2026-11-09.json).
- **Selected AC1021 torus cross-producer semantics:** the ODA group 1/3 SAT1 transform, including its `^ ` DXF escape, is reversed under a temporary research probe without changing production code. ACadRust 0.6.8 independently parses the resulting SAT 21200 payload with zero validation errors; the torus center, axis, radii, U direction and sense match the libdxfrw source-derived SAT fields exactly. Pinned InventorLoader plus native FreeCAD Part/OpenCASCADE builds a valid closed shell and solid whose bounds, area, volume and centroid exactly match the source-derived check; analytic area/volume errors are below 5e-15 relative. This qualifies one selected torus semantic profile across the libdxfrw source carrier, ODA-produced DXF carrier and independent SAT parser. InventorLoader and FreeCAD Part share OpenCASCADE, and ACadRust has no B-rep kernel, so this is not an independent-kernel result. Keep native modeler acceptance, full ACDS/history closure, other profile/version semantics, whole-source strict output and platform qualification open. See the [semantic cross-check receipt](docs/research/p7-oda-ac1021-torus-semantic-crosscheck-2026-11-09.json).
- **CADMPEG 0.6.1 exact-version screen (2026-11-10):** the official arm64 release asset checksum matches its published SHA-256, and the extracted executable reports version 0.6.1. On both the source-derived AC1021 SAB and the ODA-produced SAT for entity `0x452`, the declared save format is 21200; CADMPEG documents verified record decoding only for ACIS 217/218. Its default SAB check reports geometry transfer but also reports that it used the 217 grammar as an unverified fallback; `--no-salvage` refuses with `source.kernel-dialect-unverified`. The SAT strict check refuses for the same version mismatch. Exclude this run from semantic qualification and from the independent-kernel gate; it does not change production behavior or support claims. Revisit this reader only with a 217/218 source or a build that explicitly verifies 212.00. Exact hashes and outputs: [CADMPEG 0.6.1 receipt](docs/research/p7-cadmpeg-061-acis212-dialect-screen-2026-11-10.json). This candidate is a parser/neutral-model route, not a B-rep kernel.
- **OpenCADKernel selected-torus check (2026-11-10):** pinned OpenCADStudio
  `36981c6a` uses `ocs_acis` with `opencadkernel` `802f751`; the kernel manifest
  has no OCCT/OpenCASCADE dependency. The exact source-derived inline SAB and ODA ACAD2010 SAT were both probed.
  `ocs_acis::lift` returns one face with no loops, coedges, edges or vertices;
  mesh-based mass properties nevertheless produce the recorded approximate
  volume/centroid. The SAB header declares two bodies while only one lifts, so
  body coverage remains incomplete. Separately, the independent kernel
  constructor validates a one-face/two-edge/one-vertex placed torus from the
  same source-bound fields. The constructor volume differs from the analytic
  formula by 2.76e-7 relative; centroid and analytic bounds match. Those bounds
  differ from the prior FreeCAD result by up to 0.006589 units; retain that
  discrepancy for resolution. This advances independent scalar/mesh geometry
  evidence for one torus profile only. It does not validate complete source
  topology, surface area, native application acceptance or broader ACIS/ACDS
  support. No fixture was added; see the [receipt](docs/research/
  p7-opencadkernel-independent-torus-2026-11-10.json).
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

### Upstream LibreCAD adapter bridge (bounded fast slice)

The isolated LibreCAD `origin/master` checkout at `9889c13cdc2ef18b774abd9ff00a652594f07485`
now has a focused adapter bridge at `e216b44e763a87374556bad13e16628ff95e574c`.
It makes `DRW_Interface` public for provider cross-casts, implements opt-in
default plot-style/material providers with fail-closed opaque-reference and
existing-graph checks, binds a synthesized layer 0 to the planned Global
material, and converts hatch angles at the API boundary when building against
upstream's degree-valued fields while preserving the bundled fork's
radian-valued API. The generated default-graph adapter test passes (80
assertions in 2 cases) against the upstream libdxfrw overlay. It verifies
write/readback and LibreCAD import/re-export/readback for AC1021/AC1024/AC1027,
including suppression of both synthesized graphs when group-1005 XDATA carries
an opaque handle. The three focused hatch-unit cases pass (19 assertions).
This is bounded compile/transport/readback evidence, not native consumer
semantics, ABI qualification, or platform qualification.

The matching libdxfrw writer change permits a supplied AC1027 layer 0 only when
its material reference is the exact staged Global handle; arbitrary supplied
material references remain refused. The focused
`libdxfrw_dwg_default_material_optin_tests` passes, including its generated
AC1021/AC1024/AC1027 readback controls. The adapter and writer receipts are
recorded in [the P8 fast adapter bridge receipt](docs/research/p8-upstream-librecad-default-graph-adapter-bridge-2026-11-08.json).
The writer, focused regression, plan update, and receipt are committed in
`ff0f8dd4` (based on `6466df12dfb272e1595bcc010681d318c330a04b`). The isolated
consumer slice is separately committed in LibreCAD at
`e216b44e763a87374556bad13e16628ff95e574c`. Do not treat either test as closing
native Linux/Windows FreeCAD, clean-consumer ABI, or broader DWG graph/output
qualification.

**P8 default-graph same-version replay:** the failed import/re-export probe
identified dictionary children emitted as raw carriers but validated as generic
objects before their frames were written. The adapter now reserves eligible
raw identities during CLASSES admission, resolves DICTIONARY entries with the
actual RawCarrier kind, and avoids treating reservation-only typed identities
as selected outputs. Generated AC1021/AC1024/AC1027 import → LibreCAD export →
readback now passes with exactly one plot-style dictionary/placeholder and
three MATERIAL records, plus the LINE. This closes only the generated
same-version graph round-trip. Review of the linked-upstream hatch tests found
that the adapter reserved `dxfrw 2.0.0` for raw-radian legacy files even though
current upstream also emitted that marker for degree-valued output. The
upstream project and new DXF writer comment are now versioned 2.0.1 so new
degree-valued files are distinguishable; the root writer-primitives check
passes and the built archive contains the new marker. Existing 2.0.0 files
remain ambiguous and are not newly qualified. The consumer hatch microtarget
was 7/9 before this fix: the other failure was its missing
`support/library/misc/a3.dxf` asset. A post-fix focused consumer link rebuilt
the hatch test translation unit against the byte-identical 2.0.1 header,
replaced only `libdxfrw.cpp.o` in a scratch copy of the existing consumer
archive, and relinked the test target; the degree-writer, legacy/foreign-header,
and write/readback cases pass (45 assertions in 3 cases). This avoids the full
LibreCAD library rebuild but is not a clean/full consumer rebuild or native,
ABI, or platform qualification. Keep the missing `a3.dxf` asset separate and
retain the historical 2.0.0 ambiguity gate. See the [P8 adapter bridge
receipt](docs/research/p8-upstream-librecad-default-graph-adapter-bridge-2026-11-08.json)
and [version-comment compatibility receipt](docs/research/p8-upstream-hatch-version-comment-2026-11-09.json).

The plot-style provider passes the LibreCAD_3 facade compile guard and the
FreeCAD 1.1.4 converter subprocess contract in build-tree and installed CLI
tests. Native application execution, binary ABI and Linux/Windows remain
open; see the [consumer receipt](docs/research/p6-default-plotstyle-consumer-compatibility-2026-11-04.json).

**Native FreeCAD consumer observation (2026-11-10):** FreeCAD 1.1.4 on
macOS arm64 ran its Draft DWG import route against the existing hash-bound
AC1021 tablet sample, invoking this build's `dwg2dxf` and then FreeCAD's C++
DXF importer. Conversion returned success; FreeCAD created 4,875 document
objects, reported 4,867 valid non-null shapes and no invalid shapes, and
completed the C++ import in 3.67 seconds. Its importer explicitly reports
38 3DFACE, 22 HATCH, 69 SOLID, one OLE2FRAME and two VIEWPORT records
unsupported. Treat this as execution-route/consumer-boundary evidence only:
it does not qualify those entity types, semantic preservation, producer-wide
support, native DWG opening, or Linux/Windows behavior. The source was not
added to the repository. See the [FreeCAD import observation receipt](docs/research/p8-freecad-ac1021-tablet-import-observation-2026-11-10.json).

The SAB minimum-header helper is included in the canonical CMake and generated
source-release inventories; the inventory check and refusal self-tests pass.
Native source-release builds and platform qualification remain open.

- Preserve LibreCAD/LibreCAD_3 callback/default behavior; new callbacks remain
  non-pure. The compile-only LibreCAD_3 facade guard and generated ASCII/Binary
  3D-consumer probe pass; see the [local API and 3D probe receipt](docs/research/p8-local-compat-and-generated-3d-probe-2026-11-02.json).
  These checks cover API shape and generated record delivery only, not native
  application behavior. Qualify intended 3D consumers and remaining FreeCAD
  `dwg2dxf` profiles without dropping required geometry.

### Incomplete TABLE proxy fallback transport (generated fast slice)

The concrete LibreCAD adapter previously cleared proxy graphics from every
TABLE-to-INSERT projection, although an incomplete TABLE is emitted only by
that legacy INSERT path. It now clears the duplicate projection cache only
when the typed TABLE is semantically complete. For an incomplete TABLE, the
INSERT fallback retains parsed-DWG proxy bytes and can replay them only at the
source revision after exact chunk-framing validation in both the writer and
the `FailIfNotPreserved` adapter preflight. Generated ASCII and Binary controls
prove one readable INSERT, no emitted TABLE, exact proxy-byte readback, refusal
of a different output revision without replacing a sentinel destination, and
same-adapter retry/readback. These are opaque-carrier and 2D callback
compatibility checks, not display semantics, full-document output, or native
LibreCAD/LibreCAD_3/FreeCAD qualification. The authored control remains in
memory and no DWG/DXF fixture was added. See the [fallback proxy receipt](docs/research/p8-incomplete-table-insert-proxy-fallback-2026-11-09.json).
  A dirty local LibreCAD_3 snapshot's native persistence target passes three
  existing DXF export tests and a planar LINE/CIRCLE open/save/reopen probe
  against a pinned libdxfrw build. Its raw include/library finder route loses
  the C++17 requirement: a fresh persistence probe compiles as C++14 and fails
  in upstream public headers on `std::optional`. Installing the current
  libdxfrw build and configuring LibreCAD_3 with
  `CMAKE_FIND_PACKAGE_PREFER_CONFIG=ON` plus the installed `libdxfrw_DIR`
  selects the exported package target, propagates C++17, and successfully
  builds the persistence target. This is the fastest known integration route;
  the existing focused DXF export tests pass 3/3 and the planar LINE/CIRCLE
  open/save/reopen control passes 1/1 against that exact installed package
  archive; `DYLD_PRINT_LIBRARIES` confirms the new persistence/kernel dylibs
  were loaded. An in-tree compile-only consumer guard now requests C++14 while
  linking `dxfrw` and requires the public `<optional>` API to compile; its
  effective command is `-std=c++17` and the focused target builds. Outputs
  remain scratch-only. Keep the default module-mode/raw-path
  failure recorded. This closes only this dirty macOS ARM64 persistence
  integration probe; preserve the pinned libdxfrw revision and qualify a clean
  consumer revision, full application, ABI, and Linux/Windows separately.
  Its full test target has an unrelated Kaguya C++17
  failure, and a nonplanar LINE triggers its 2D geometry exception. Qualify a
  clean consumer revision and full application separately
  ([native probe](docs/research/p8-librecad3-macos-native-dxf-2026-11-04.json),
  [C++ standard reproduction](docs/research/p8-librecad3-cxx-standard-consumer-compat-2026-11-07.json)).
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
  Hash-bound physical AC1021 and AC1027 RASTERVARIABLES witnesses now seed
  locally authored wrapper DWGs; each wrapper passes source-value re-import,
  selected ASCII/Binary output, atomic invalid-value refusal, same-adapter retry,
  and typed readback ([AC1021 receipt](docs/research/p4-ac1021-rastervariables-physical-writer-bridge-2026-11-05.json),
  [AC1027 receipt](docs/research/p4-ac1027-rastervariables-physical-writer-bridge-2026-11-05.json)).
  A hash-bound physical AC1032 frame=1 witness now passes exact 14-byte raw
  frame and 73-bit scalar/handle-boundary proof, then seeds a locally authored
  wrapper DWG. Its source NOD `0x0A` owner/reactor roles are remapped to the
  wrapper NOD `0x0C`; selected ASCII/Binary output, invalid-field/reference
  refusal, same-adapter retry and typed readback pass. A separate hash-bound
  frame=0 AC1032 witness now passes the same selected wrapper route under
  RequireComplete owner-resolved output ([frame=0 receipt](docs/research/p4-ac1032-rastervariables-frame0-physical-writer-bridge-2026-11-05.json);
  [frame=1 receipt](docs/research/p4-ac1032-rastervariables-frame1-physical-writer-bridge-2026-11-05.json)).
  A hash-bound physical AC1024 frame=1 witness now passes the same bounded
  selected-wrapper route. Its source span verifies class, canonical frame
  scalar, quality/units, zero string marker and role-4 owner/reactor to source
  NOD `0x0A`; wrapper output remaps those roles to NOD `0x0C`, with ASCII/Binary
  refusal, retry and typed readback ([receipt](docs/research/p4-ac1024-rastervariables-frame1-physical-writer-bridge-2026-11-05.json)).
  The physical AC1024 frame=0 source now also passes exact scalar/handle-span
  checks and a locally authored wrapper under RequireComplete owner-resolved
  ASCII/Binary output; the wrapper remaps physical NOD `0x0A` to `0x0C`
  ([receipt](docs/research/p4-ac1024-rastervariables-frame0-physical-writer-bridge-2026-11-05.json)).
  A hash-bound physical AC1018 frame=1 witness now passes the same bridge with
  the R2004-specific 18-byte body, 10-bit frame span, RL boundary and owner/
  reactor handle spans verified before wrapping; source NOD `0x0A` maps to the
  wrapper NOD `0x0C`. Selected ASCII/Binary output, refusal, same-adapter retry
  and typed readback pass ([receipt](docs/research/p4-ac1018-rastervariables-frame1-physical-writer-bridge-2026-11-05.json)).
  A separate physical AC1018 frame=0 witness also passes the same
  RequireComplete owner-resolved wrapper route; its 17-byte body, RL boundary,
  scalar fields and source owner/reactor spans are checked before NOD remapping
  ([receipt](docs/research/p4-ac1018-rastervariables-frame0-physical-writer-bridge-2026-11-05.json)).
  These controls do not convert the physical drawings as a whole. Their
  unrelated source/read/layout-integrity gates and native image semantics remain
  open, as do broader producer/version profiles.
  Hash-bound AC1018, AC1024, AC1027 and AC1032 WIPEOUTVARIABLES witnesses now
  seed local wrapper DWGs. Their version-specific frame proofs, selected
  ASCII/Binary owner/reactor output, refusal, retry and typed readback pass
  ([AC1018](docs/research/p4-ac1018-wipeoutvariables-physical-writer-bridge-2026-11-05.json),
  [AC1024](docs/research/p4-ac1024-wipeoutvariables-physical-writer-bridge-2026-11-05.json),
  [AC1027](docs/research/p4-ac1027-wipeoutvariables-physical-writer-bridge-2026-11-05.json),
  [AC1032](docs/research/p4-ac1032-wipeoutvariables-physical-writer-bridge-2026-11-05.json)).
  None converts its physical drawing as a whole; AC1018/AC1024 retain unclaimed
  HEADER suffixes and unrelated whole-source gates, while AC1027 retains HEADER
  integrity observations and the global strict-source gate. Native wipeout
  display/plot behavior, preview preservation, generic common-link expansion,
  and broader producer/mode coverage remain open.
  Physical frame=1 witnesses for AC1018, AC1024, AC1027 and AC1032 now also
  pass exact version-specific frame/class/owner/reactor checks and seed local
  wrappers. Each wrapper passes selected ASCII/Binary output, refusal,
  same-adapter retry and typed readback; physical source NOD `0x0A` roles are
  remapped into the wrapper graph ([frame=1 bridge receipt](docs/research/p4-wipeoutvariables-frame1-physical-writer-bridges-2026-11-05.json)).
  This closes only those four selected-object source-to-writer profiles; no
  physical whole-drawing or native display claim follows.
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
- The earlier positive-test failure is resolved at the harness/source-selection
  layer. Its trace identified an error-level `HeaderCurrentDimensionStyleReference`
  row: the LibreDWG-generated source had an empty `$DIMSTYLE`, so its header
  contained a null value for the common DIMSTYLE hard pointer. ODA v5.4.1's
  header-variable layout lists DIMSTYLE as a hard pointer; strict refusal was
  correct, and neither parser leniency nor weaker output preflight is warranted.
  The tracked AC1027 and LibreDWG-generated AC1015 inputs remain unsuitable for
  this positive integration gate ([failure analysis](docs/research/p8-freecad-desktop-strict-source-blocker-2026-11-07.json)).
- **Implemented positive native source gate:** the optional desktop CTest and
  manually dispatched Linux/Windows FreeCAD tests now generate an AC1021
  3D LINE DWG in a unique temporary directory using the existing
  `libdxfrw_dwg_local_roundtrip --generate-cli-batch-source` path. They copy it
  into the Unicode input path, run strict preflight with the exact installed
  converter before launching FreeCAD, then assert the authored 3D LINE
  endpoints and bounds during `open()` and `insert()`. No DWG/DXF fixture is
  added. On this macOS ARM64 host, the optional GUI CTest passed both desktop
  operations with FreeCAD 1.1.4; the matching installed `FreeCADCmd` open and
  insert controls also passed. The current local result qualifies this
  generated writer-to-converter-to-FreeCAD path only; hosted Linux 1.1.3 and
  Windows 1.1.3 workflow runs remain pending
  ([implementation receipt](docs/research/p8-freecad-desktop-generated-ac1021-open-insert-2026-11-07.json)).
- The old tracked-sample binding receipt remains historical registration
  evidence only ([receipt](docs/research/p8-freecad-tracked-sample-binding-2026-11-02.json)).
  Complete the manually dispatched native Linux/Windows jobs on the updated
  generated-source path; keep those platform, package/loader, Windows UTF-8/Qt,
  and native application claims open until their hosted receipts pass.
- `dwg2dxf` now uses `std::filesystem::u8path` for input/output preflight and
  the Windows `wmain` entry point converts Unicode arguments strictly to UTF-8
  before handing them to the shared CLI parser; MinGW builds select Unicode
  console startup with `-municode`. The focused FreeCAD CLI
  compatibility control now stages its generated source at an ASCII path and
  copies it into Unicode paths, so it exercises Unicode input and output paths
  on every host; build and focused CTest pass on the current host. Native
  Windows compilation/runtime, Qt path delivery, and native FreeCAD qualification
  remain open ([Unicode CLI slice receipt](docs/research/p8-dwg2dxf-windows-utf8-argv-fast-slice-2026-11-07.json)).
- The manually dispatched native FreeCAD Linux/Windows workflow now builds the
  local DWG source generator and runs both build-tree and installed CLI
  compatibility tests alongside its tracked-file `open()`/`insert()` handoff
  tests. Those tests copy the tracked DWG to a Unicode path
  (`INPUT_PATH_UNICODE=ON`) before the actual FreeCAD converter subprocess.
  The focused build-tree and installed tests pass locally; hosted
  MSVC/FreeCAD execution remains pending ([workflow](.github/workflows/freecad-platform-qualification.yml),
  [Unicode CLI slice receipt](docs/research/p8-dwg2dxf-windows-utf8-argv-fast-slice-2026-11-07.json)).
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
