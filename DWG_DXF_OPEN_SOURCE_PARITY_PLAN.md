# DWG/DXF parity — remaining implementation plan

Updated 2026-11-08 (AC1027 Cover source-reference audit now has authoritative no-body/handle profiles for 40/18 records plus source-matched LibreDWG no-body profiles for 29 more; TABLESTYLE and MLEADERSTYLE now have complete ODA body-handle field profiles, leaving only 3DSOLID partial, with no body reference profile left unknown. The three AC1027 default MATERIAL bodies now have source-matched no-body-handle and semantic profiles; the AC1027 SUN body has a peer no-body-handle profile and ODA-backed VPORT hard-owner edge; DICTIONARYWDFLT has an ODA §20.4.45 default hard-pointer profile. MATERIAL output remains guarded because retained modern body tails are not replayable. See [current audit receipt](docs/research/p6-ac1027-cover-reference-guard-audit-refresh-2026-11-07.json). AC1032 multi-schdat DataStorage plus raw-only TextObjectContextData, PERSUBENTMGR, and TABLECONTENT shell recovery now reaches 842/842 published frames with zero entity/object parse failures; the bounded AC1032 MentalRay recovery separately reaches 182/182 frames with zero entity/object parse failures, while strict output remains refused at unrelated HEADER integrity rows ([receipt](docs/research/p6-ac1032-acad-table-cells-reader-fix-2026-11-07.json)); generated AC1032 TABLECONTENT detached-TV controls publish complete typed values, while the alternative body-byte-count control stays raw-only and verifies rollback, retry, destination preservation and following-frame recovery; the PERSUBENTMGR/TABLECONTENT source bodies remain disputed and raw-only; AutoCAD SAVEAS AC1021/24/27/32 samples now establish the same IMAGEDEF code-0xC parent and group-350 dictionary membership, and the independent wire observer captures that cross-version pattern as explicitly unqualified while strict output remains refused ([receipt](docs/research/p6-imagedef-cross-version-soft-role-evidence-2026-11-07.json)); an unlocked Aqua session was observed, but the Launch Services attempt for ODA 27.1 failed with `kLSNoExecutableErr`; the installed 27.1 app passes native verification outside the sandbox; the downloaded official ODA 27.10 ARM64 bundle still fails local code-signature verification although the 27.10 DMG's internal checksums pass, so 27.10 was not executed and no signature was modified ([package receipt](docs/research/p6-oda-27-10-official-arm64-package-signature-2026-11-07.json)); repeated direct 27.1 attempts, including the latest post-unlock retry, aborted during Pasteboard/XPC initialization before document processing. A materially changed GUI route then produced two same-version AC1027 resaves from scratch copies; both validated trailers end at byte 844 and both 739-byte suffixes remain unclaimed. The hashes differ, with no equal byte positions but matching low three bits at every position ([receipt](docs/research/p6-ac1027-oda-gui-same-version-resave-2026-11-07.json)). The read publishes 119/119 frames with zero entity/object parse failures, while strict DXF output remains blocked by the separate AC1027 source-reference gate. Do not repeat the unchanged direct launch method; continue local work and suffix analysis without promoting support claims; the R13 GROUP trace now binds the DICTIONARY issue to EED app `ACAD`, value `TREAT_AS_HARD`, raw ASCII bytes and per-value codepage `0`; ODA v5.4.1 does not define zero as file-codepage inheritance, so the source remains blocked without a production decoder change; the post-unlock ODA 27.1 GUI conversion emitted the ordered GROUP/dictionary relations but omitted that EED value and a referenced SHAPE; the AC1012/AC1014 DICTIONARY/GROUP common-link profile now passes bounded role/count controls and the source-bound R13 recheck, leaving EED codepage 0 as the sole GROUP source blocker ([receipt](docs/research/p4-populated-group-candidate-census-2026-11-07.json)); current strict `blocks_and_tables` diagnostic reaches 3,586/3,586 frames, and LibreDWG 0.14 repeats the unqualified FIELD child flag values without resolving their semantics, so strict FIELD/FIELDLIST output remains refused; ODA §2's detached-string rule conflicts with ACadSharp-derived TABLE value byte-count parsing; a debug-only alternate body-string parse of both AC1032 TABLECONTENT objects reaches the exact body and detached-string boundaries; the 17-bit interval is the encoded string-size word plus presence bit, and the final handle bit is byte-alignment padding under ODA §20.1; the candidate still conflicts with the TABLECONTENT schema and remains semantically incomplete, so both objects stay raw/refused with no production parser interpretation change; DWG 3DFACE output now rejects invisible-edge masks outside 0–15 and passes generated rollback/retry/readback across all six writer versions; R24 TABLESTYLE generated write/readback now includes AC1032 with same-handle recovery after both invalid-count and invalid-CMC refusal; the focused named-resource import selector now passes after synthetic receipts stopped authorizing strict-complete DWG output, with destination-preserving refusals and no support-claim change; AC1024 mechanical physical selected-output routes now cover 17 associative dependency/body pairs, 399 constraint nodes, and 17 variables across two networks and 13 value-dependency children in ASCII/Binary; the DXF `AcDbAssocEdgePersSubentId` codec slice is complete, and ten AC1024 civil Edge bodies decode to source-matched vertex-index cycles and their bounded selected ASCII/Binary output/readback passes; whole-source conversion and native meaning remain open ([read receipt](docs/research/p4-ac1024-civil-assoc-edge-dwg-read-2026-11-07.json), [selected-route receipt](docs/research/p4-ac1024-civil-assoc-selected-route-readiness-2026-11-07.json)); the viewport DWG debug trace now prints `centerPY` for its Y coordinate (diagnostic only; [receipt](docs/research/p6-viewport-debug-center-y-log-fix-2026-11-07.json)); a LibreCAD_3 persistence probe using raw include/library paths fails because the consumer compiles as C++14 while current upstream libdxfrw requires C++17, and this configuration bypasses libdxfrw's PUBLIC compile-feature propagation ([receipt](docs/research/p8-librecad3-cxx-standard-consumer-compat-2026-11-07.json)); AC1021 tablet now passes strict same-version output and fresh DXF readback after the WCS mesh extrusion default fix; generated WCS mesh round-trip controls pass across all six DWG writer releases ([receipt](docs/research/p4-ac1021-tablet-wcs-mesh-default-2026-11-07.json)); broader source graph closure remains open). This file

**Resume checkpoint (2026-11-07):** the Mac session is unlocked and the
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

### Current execution checkpoint (2026-11-08)

- **Latest local slice:** commit `bff89295` now admits finite, nonnegative
  per-style R24 TABLESTYLE text heights. Generated ASCII/Binary readback
  checks exact raw group-144 order; negative and infinite values refuse
  atomically and corrected values succeed on the same adapter. The focused
  `libdxfrw_tablestyle_r24_typed_writer` CTest passes (1/1, 1.85 seconds); see
  the [text-height receipt](docs/research/p6-r24-tablestyle-text-height-transport-2026-11-08.json).
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
- **P4 inventory decision:** the saved [same-version census](docs/research/p4-local-same-version-strict-output-census-2026-11-06.json)
  reports four already-qualified whole-source passes, 20 strict refusals, and
  zero newly qualified source profiles. The saved [AC1021 source inventory](docs/research/p4-ac1021-opaque-frame-inventory-2026-11-06.json)
  contains only selected graph proofs already represented below; it yields no
  distinct ready physical OBJECTS candidate. Keep the R13 GROUP EED/codepage-0,
  AC1021 TORUS carrier/ACDS owner, and AC1027 Sweep-body gates as recorded in
  the P4 section; do not repeat their probes or weaken strict output. Continue
  with a distinct locally testable P5/P6 implementation gap; do not repeat the
  corpus census or speculate about the remaining Sweep body.
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
instead of repeating successful scans or controls. The generated R24 TABLESTYLE
border LTYPE, value-domain, scalar format, per-style margin and text-height
slices are complete in commits `12698d59`, `93c02f0d`, `9678d0c2`, `513a8adf`,
`37d876cf`, and `bff89295`; the saved P4 inventory decision has no new ready
physical graph. Continue with the following bounded R24 merge-flag packet.

1. **P6 / admit documented R24 TABLESTYLE merge flags.** ODA §20.4.101.4
   defines BL group 92 merge flags and limits them to bits `0x8000` and
   `0x10000`. The writer currently hard-codes the witnessed title/header/data
   combination. Preserve style identity and all other gates, but admit any
   combination within that documented mask per cell style. Generated
   ASCII/Binary controls must verify the exact group-92 positions and typed
   readback, refuse an unknown bit with destination preservation, and retry
   successfully on the same adapter. Keep native merge behavior and physical
   producer qualification open.

2. **P4 / resume physical OBJECTS closure only on a new ready witness.** The
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
   remains refused. Autodesk's DXF reference explicitly classifies group 350
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

No physical P4 OBJECTS subgraph currently passes the source-to-output gate.
The AC1021 public TORUS graph is source-bound and its fields/links match the
narrow writer profile, but only generated output/readback is qualified; the
containing source has 387 opaque callbacks and other ACDS graphs. Next add a
selected projection check for this exact graph in ASCII and Binary while
retaining whole-source refusal. The projection now fails before the modeler
serializer, so no selected physical output route is qualified. Do not promote
a parser/source match to whole-document output or semantic support.

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

**Next implementation slice — resolve the inline-SAB to DXF carrier route:**
1. Hash-pin the manual source probe before import. The wrapper
   `tools/research/run_acsh_torus_source_probe.py` streams SHA-256 and accepts
   only the receipt's exact LibreDWG source digest; the C++ inspector then
   verifies unique graph roles, inline-SAB signature/terminator/span, payload
   length/modeler version and DataStorage absence. Keep the external DWG/DXF
   outside the repository; do not check in a fixture.
2. Trace the current writer gate for this exact source entity in both target
   DXF families. The code admits DWG-associated AC1015 SAT-v1 payloads and
   R2013+ SAB linked through ACDSDATA; the AC1021 inline SAB is neither, so it
   must not be relabeled or emitted by filtering the containing drawing.
3. Resolve the standards-backed conversion route before production changes:
   either preserve this source carrier in a documented DXF representation, or
   create the later ACDSDATA envelope only if its modeler shell fields and
   carrier conversion are supported by an authoritative spec plus eligible
   producer output. LibreDWG's SAB-to-SAT conversion is a useful implementation
   comparison, not independent semantic proof. Do not invent group-290/UID
   values, translate the SAB terminator by guess, or count parser agreement as
   geometry validation. Autodesk's current 3DSOLID DXF reference lists version
   group 70, proprietary SAT strings in groups 1/3 and the history soft-owner
   group 350; the generic 310 binary-chunk code is not specified for 3DSOLID,
   so do not infer that it carries SAB. ODA §20.4.41 distinguishes inline
   version-2 DWG SAB by the `ACIS BinaryFile` header and tagged terminator.
   The existing LibreDWG peer output for this exact source emits handle `452`
   with group 70 = 1 and history `453`, but no groups 1/3/310. A fresh local
   LibreDWG 0.14 conversion reproduced that ten-pair record, while verbose
   conversion of the full drawing did not bind a SAB-to-SAT trace to this
   output entity. Treat this as a negative peer observation, not a positive
   carrier witness. Resolve why the exact peer entity has no carrier and bind
   the bytes to it before using this source to qualify a conversion. The
   installed ODA File Converter still aborts before conversion with
   PasteBoard/XPC errors while the Mac is locked; retry it after the session is
   unlocked. See the [Autodesk 3DSOLID DXF
   reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm),
   [ODA v5.4.1 §20.4.41](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf),
   and the [carrier gate receipt](docs/research/p4-ac1021-inline-sab-torus-dxf-carrier-gate-2026-11-08.json).
4. Close the selected layer-0 extension dictionary `0x1C1` (decimal handle
   449) and its child records; do not confuse decimal diagnostic handles with
   hexadecimal DWG handles. The scratch projection now carries that exact
   typed closure. Its next refusal is `EvaluationGraph owner is unresolved or
   not representable`: the typed ACDS collector is only entered when
   `dataStorageSections` is nonempty, while this AC1021 source has no
   DataStorage and no same-profile raw DXF history receipt. Resolve how the
   DWG-origin history/graph can enter the typed output closure without
   fabricating a DataStorage section; only then can source-bound ASCII/Binary
   controls reach the modeler serializer. Require exact carrier and graph-role
   readback, rollback plus corrected retry for injectable dependency failures,
   and the same complete-source refusal guard. Generated-output checks remain
   separate from physical-source evidence.

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
opt-in `--check-acsh-torus-source-projection` attempts selected ASCII and
Binary output from a scratch projection; both are refused without replacing a
sentinel. The projection includes the exact layer-0 extension DICTIONARY
closure (`0x1C1`, decimal handle 449), then stops at extension-graph
validation: `EvaluationGraph owner is unresolved or not representable`. The
typed ACDS collector is gated on a nonempty DataStorage section, which this
AC1021 source does not have. The probe validates atomic refusal but does not
reach the modeler serializer or qualify the inline-SAB carrier gate. Strict
full-source export also preserves its destination but stops first at the
unrelated unqualified DWG IMAGEDEF owner role, so the later opaque-frame gate
was not reached. A fresh LibreDWG 0.14 peer conversion again emitted only
group 70 and history handle `453` for output handle `452`; its verbose output
did not explain the missing carrier. The ODA converter retry aborts while the
Mac session remains locked (PasteBoard/XPC). The existing generated TORUS/CMC
controls pass (2/2); no fixture was added. These are refusal results, not
successful physical-source output. See the
[inline-SAB carrier gate receipt](docs/research/p4-ac1021-inline-sab-torus-dxf-carrier-gate-2026-11-08.json).

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
   dynamic evaluation and native rendering claims gated.
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
   TABLECONTENT and PERSUBENTMGR remain raw-only. Remaining P6 work is limited
   to source-backed version/layout gaps and independent evidence for the
   IMAGEDEF owner-role conflict and AC1027/AC1032 HEADER suffixes. The successful
   27.1 GUI route supersedes the earlier plan to wait for a changed launch
   environment: two AC1027 outputs are now available, but neither isolates the
   suffix's meaning. Next, avoid another identical resave; if suffix work is
   pursued, make one controlled change to a locally generated or scratch source
   and capture the matching GUI output, then compare only the validated HEADER
   section and independently decoded fields. Keep this lane advisory until a
   repeatable field-to-byte mapping or authoritative wire definition accounts
   for every suffix byte. Do not repeat the unchanged ODA CLI launch: the latest
   post-unlock attempt still aborts during Pasteboard/XPC startup, while the GUI
   route has already reached document processing. Resume suffix work only with
   a controlled source mutation or a newly available converter route.

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
   read profile; AC1027 MATERIAL writing remains refused while the modern raw
   tail cannot be replayed. Keep normative ODA and peer-based dispositions separate: MLINESTYLE and the
   R24 TABLESTYLE handle roles are directly specified by ODA §§20.4.73 and
   20.4.101.2–.4.
   `DRW_MLineStyle::parseDwg` consumes inline AC1027 element linetype indices;
   the hash-bound inventory validates its source frame/common links and NOD
   dictionary edge. The peer PLACEHOLDER evidence is scoped to the same source
   and exact AC1027 graph; it does not replace the ODA authority.

   **AC1027 MATERIAL default parser profile completed; output remains open.**
   The three type-502 MATERIAL frames (handles 68/69/70; keys
   ByLayer/ByBlock/Global) match LibreDWG minJSON from the same Cover source by
   identity, size, owner, reactor, and the decoded default semantic fields.
   The body schema has no H fields before common handles. The parser now
   recognizes AC1027's inherited ambient/diffuse/specular colors, six simple
   maps, scene-sourced refraction, zero translucence/self-illumination/
   reflectivity, channel flags 127, and nonprocedural mode. The focused source
   audit asserts these values and confirms all three semantic-completeness
   flags. The versioned default builder now emits the observed AC1027 profile;
   AC1021/AC1024 exact profiles remain unchanged. This does not enable AC1027
   output: `DRW_Material::encodeDwg` continues to refuse retained modern raw
   tails because body/string replay is not yet complete. The SUN,
   DICTIONARYWDFLT, TABLESTYLE, and MLEADERSTYLE body-reference profiles are
   complete. Only 3DSOLID remains partial; AC1027 MATERIAL output and the
   global strict guard remain open. Do not rescan the corpus.

   **Completed MATERIAL profile/retry slice (2026-11-08):** the AC1027 exact
   frame writer now validates its body and detached strings against the
   versioned default profile builder. This removes stale AC1024 fixed-length
   assumptions that rejected the AC1027 516-bit field body and 118/134-bit
   detached strings. Focused tests now pass for AC1024 defaults, AC1027 opt-in
   graph emission, injected dictionary failure with buffer/checkpoint rollback
   and same-adapter retry, and retained-modern-tail replay refusal. The one
   partial AC1027 report (3DSOLID), MATERIAL output, and aggregate strict guard
   remain open; this slice does not promote support.
   The refreshed source-reference audit independently passes with 88 retained
   identities, 40 normative no-body profiles, 18 normative handle profiles,
   29 peer no-body profiles, 43 dictionary edges, one selected typed report (3DSOLID),
   and zero unknown body-reference dispositions. That audit supports source
   classification; the focused writer tests support this bounded retry change.

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
5. **P7/P8/P9 / qualification gates.** P7 needs independent ACIS/ACDS semantic
   evidence. P8's current-head LibreCAD_3 CONFIG-package build, focused export
   tests, and planar save/open/reopen probe are refreshed; next qualify on a
   clean consumer revision, then native Linux/Windows FreeCAD and LibreCAD/LibreCAD_3
   runs. Keep the dirty-checkout build as integration evidence only. P9 remains
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
| P4 | OPEN — source and graph closure | Continue with the next distinct physically evidenced OBJECTS subgraph; bind source identities, owner/target roles, recursive dependencies, and exact emitted records. Pool_Detail MLEADERSTYLE passes selected ASCII/Binary transport under the exact bounded AC1021 one-style/one-BYBLOCK projection; malformed LTYPE_CONTROL, BLOCK_CONTROL, and HEADER current-layer integrity observations remain excluded, and strict whole-source output remains refused. The AC1021 truetype whole source passes `RequireComplete` ASCII/Binary output and fresh whole-DXF import readback at 173/173 published frames; its group-330 LAYER/group-331 LTYPE references and exact LayerStates DXF-origin group-370 replay now pass under source snapshots and the complete dictionary path, with mutation refusal/retry. Other group-370 XRECORDs remain refused. Do not infer producer-wide support or generalize either source-specific gate. Resolve the tracked ENC dangling-reference profile only with eligible producer evidence. Keep unresolved null members, orphan owners, unsupported frames, and named-block SORTENTSTABLE profiles refused. Broaden beyond individual local sources only with independent producer and native output evidence. **The AC1027 Cover 3DSOLID history field/span and the public-source `0x2e1` → `0x2e4` → `0x2e3` → `0x2e2` identity/edge path are captured and match LibreDWG 0.14 (receipt `p4-ac1027-cover-3dsolid-history-wire-span-2026-11-08`). The production helper has null/non-null/alternate-role and truncation/reset/retry controls. `ACSH_HISTORY_CLASS` direct fields are complete; the graph targets `ACSH_SWEEP_CLASS`, whose EvalExpr/HistoryNode prefixes parse but whose embedded sweep/path entity and SweepOptions action-body tail remains incomplete. The production action-body status now requires exact bounded-body-end consumption before it reports `Complete`; on the pinned AC1027 sample it correctly reports `Partial` after a 174-bit decoded prefix of a 2,571-bit body ([exact-span receipt](docs/research/p4-ac1027-sweep-action-body-exact-span-2026-11-08.json)). This fixes false completeness only; no tail layout is inferred and the profile remains output-refused. The typed BOX-history writer has no Sweep route, and raw-DXF preservation applies only to DXF input; the opaque proxy writer is AC1024-only. Keep output and strict conversion refused. The DWG code-3 to DXF group-350 role mapping remains unqualified; do not alter writer roles from converter output alone.** | Existing Cover inventory, ODA v5.4.1 §§2.13 and 20.4.41, hash-bound source probes, production-helper controls, LibreDWG 0.14 graph view, output-route code audit, and Autodesk DXF group-code authority. |
| P5 | OPEN — dynamic-block semantics, rendering and TABLE profiles | Find a complete FIELD/FIELDLIST source graph with a visible root; qualify physical TABLEGEOMETRY spans and remaining CONTENTFORMAT/CELLSTYLEMAP combinations; resolve VIEWPORT override type/value and native activation/rendering; establish native dynamic constraint evaluation. | P4 closure where needed; source-bound spans/roles plus native semantic evidence. |
| P6 | OPEN — version, schema and semantic residuals | Resolve only evidence-backed source gaps: AC1021/24/27/32 IMAGEDEF parent-role conflict (source capture now retained as unqualified; DXF output still refused); raw-only TABLECONTENT/PERSUBENTMGR/TextObjectContextData; unclaimed AC1027/AC1032 HEADER suffixes; remaining version boundaries, geometry/no-loss, HEADER/CLI, parser disposition and caller-state contracts. The minimal generated AC1021 ODA matrix passes only when both default-layer graph opt-ins are enabled; the AC1021/24/27 fast material control now checks both output layer-0 references against planned targets. This is bounded writer evidence, not a broad support claim. Preserve strict refusal for unsupported or unclaimed bytes. | ODA v5.4.1 plus AutoCAD DWG/DXF source pairs; focused fast controls. Owner-semantic promotion still needs independent native rewrite or normative resolution. Suffix work requires a materially changed converter route. See the [IMAGEDEF role receipt](docs/research/p6-imagedef-cross-version-soft-role-evidence-2026-11-07.json) and [AC1021 writer probe receipt](docs/research/p6-ac1021-minimal-writer-oda-gui-rejection-2026-11-07.json). |
| P7 | OPEN — ACIS/ACDS and independent 3D meaning | Complete entity/owner/key/AcDs/SAT-SAB association, versioned framing/index/suffix/prototype assembly and exact output binding; qualify transforms, units, topology, trims, periodicity and spline meaning with an independent kernel. | P4 graph/output bindings and independent semantic/kernel evidence. |
| P8 | OPEN — consumers, platforms and release | Qualify LibreCAD and LibreCAD_3 callbacks/defaults; adapt LibreCAD's `RS_FilterDXFRW` to the upstream AC1021+ default plotstyle/material graph providers and establish the material opaque-reference predicate for its supported export profiles; run a clean consumer build and tests; run native Linux/Windows FreeCAD open/insert checks; qualify intended 3D consumers, ABI, packaging and release behavior. Existing evidence is source inspection only: LibreCAD currently links its bundled fork and its dirty adapter does not implement these upstream provider interfaces. | Stable candidate and native platform access with tool/version receipts; adapter migration must preserve LibreCAD 2D behavior and refuse unresolved references. |
| P9 | DEFERRED_EXTERNAL — six non-promoting items | Resolve J256, J260, J268, J284, J293 and J295 only through the successor registry and eligible independent evidence. J268 still needs original input identity/digest; J284 has 11 target-debt/integrity blockers; J295 needs native receipts for four RTEXT position/rotation claims. None can promote support claims alone. | `metadata/external-evidence-resolution-v1.json`; retain lineage and exclusions. |

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
   A local AC1021 metric TABLE candidate is screened out: the trace encounters
   unsupported TABLE CadValue data type 48 (payload bits 278726 and 312907),
   which is outside the ODA §20.4.99 documented flag set; the peer JSON exposes
   TABLE entities but no direct TABLEGEOMETRY record. This source cannot establish
   TABLEGEOMETRY source spans or graph closure. Do not broaden the type parser
   from this candidate; reopen only with a complete, source-bound TABLE graph.
   See the [candidate screen receipt](docs/research/p5-ac1021-metric-tablegeometry-candidate-screen-2026-11-06.json).
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
| Safety and caller state | OPEN. Continue auditing remaining setup, assembly and work bounds, especially other `writeObjects` maps/vectors; inject allocation failures across independent builders; broaden rollback/failure injection to other staged families; current controls cover the GROUP handle planner plus core IMAGEDEF and rooted named-dictionary OBJECTS all-allocation-offset sweeps, plus IMAGEDEF callback validation/bad_alloc/retry. The GROUP planner sweeps every measured allocation in ASCII/Binary configurations, confirms no partial plan is retained, and retries the same codec successfully ([GROUP planner sweep](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json)). A reservation allocation failure inside an admitted write now fails that transaction without poisoning its same-codec retry; pre-write reservation failures retain their one-attempt generation guard. The ASCII/Binary IMAGEDEF sweep now checks each calibrated allocation before the consumer OBJECTS callback for atomic rollback, destination and identity preservation, same-writer retry, and typed readback ([IMAGEDEF allocation sweep](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json)). The rooted named-dictionary sweep does the same for every measured allocation, including the handle-reservation failure that previously caused a stale retry rejection; it checks typed DICTIONARY readback and the pre-write reservation guard ([named-dictionary allocation sweep](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json)). The generated NOD/GROUP OBJECTS path now sweeps every calibrated allocation in ASCII and Binary, checks destination and derived-identity rollback before the consumer callback, retries the same writer, and reads back the NOD-to-GROUP-dictionary-to-GROUP handle graph ([generated GROUP OBJECTS allocation sweep](docs/research/p6-dxf-generated-group-dictionary-objects-allocation-retry-2026-11-03.json)). The parallel generated NOD/IMAGE path sweeps every calibrated allocation with a planned IMAGEDEF and source IMAGE dictionary, then checks same-writer retry and typed NOD-to-image-dictionary-to-IMAGEDEF plus IMAGE-reference readback ([generated IMAGE dictionary OBJECTS allocation sweep](docs/research/p6-dxf-generated-image-dictionary-objects-allocation-retry-2026-11-03.json)). Both are local transaction-safety evidence and make no producer or semantic support claim. Generated NOD/GROUP/IMAGE dictionary-source admission also sweeps allocation failures through root-only and rooted child-dictionary profiles, preserving prior owned state and retrying the same codec after each injected failure ([source admission sweep](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json)). The SECTIONMANAGER/SECTIONSETTINGS `writeObjects` path now sweeps every measured allocation offset for generated ASCII/Binary graphs; each failure preserves the destination and the same adapter retries with fresh graph readback in normal and ASan builds ([SECTION OBJECTS allocation receipt](docs/research/p6-dxf-section-objects-allocation-failure-retry-2026-11-03.json)). The generated POINTCLOUDDEFINITIONEX/REACTOREX OBJECTS callback sweeps every calibrated allocation offset in ASCII and Binary: each failure preserves the destination sentinel, and same-adapter retry plus typed readback verifies NOD → point-cloud dictionary → definition/reactor → entity references. The companion classic POINTCLOUDDEFINITION/REACTOR route now has the same ASCII/Binary all-offset rollback/retry/readback sweep; its handle-remapped cloud identity is verified through reciprocal definition/reactor edges rather than assumed numeric identity. Both point-cloud controls use locally authored graphs and make no producer or semantic support claim ([updated receipt](docs/research/p6-pointcloud-objects-allocation-failure-retry-2026-11-03.json)). The generated AC1027 LAYOUT/RapidRT render-preset OBJECTS path now sweeps 57 ASCII and 56 Binary callback allocation offsets, preserving the prior destination and source graph and proving same-adapter retry plus fresh LAYOUT/render-settings graph readback; the source DXF is created under test scratch and removed ([receipt](docs/research/p6-dxf-layout-render-settings-objects-allocation-retry-2026-11-08.json)). This is local graph/transaction evidence only. The consumer `writeObjects()` callback also has an ASCII/Binary thrown-`std::bad_alloc` control after one XRECORD is staged: the write fails atomically, preserves the prior destination and source-object bindings, skips finalization, then succeeds on same-codec retry with typed readback ([receipt](docs/research/p6-dxf-objects-callback-bad-alloc-rollback-retry-2026-11-03.json)). A generated direct NOD-to-XRECORD OBJECTS profile now sweeps six ASCII and five Binary allocation offsets; each injected failure preserves the destination and caller graph, and same-adapter retry verifies typed owner/payload readback ([receipt](docs/research/p6-dxf-xrecord-objects-allocation-failure-retry-2026-11-03.json)). This closes only that emitter path. A generated AC1027 DXF-source nested dictionary with two DICTIONARYVAR records now sweeps 62 ASCII and 60 Binary OBJECTS-callback allocation offsets; each failure preserves the destination and caller graph, and same-adapter retry verifies both values and their nested owner ([receipt](docs/research/p6-dxf-dictionaryvar-objects-allocation-failure-retry-2026-11-03.json)). This closes only that emitter path. The generated PDF/DGN/DWF underlay-definition OBJECTS graph now sweeps all 60 ASCII and 53 Binary callback allocation offsets, preserving the destination sentinel and succeeding on same-adapter retry with typed definition/entity graph readback ([receipt](docs/research/p6-dxf-underlay-objects-allocation-failure-retry-2026-11-05.json)); this closes only that local transaction path. The generated AC1027 SUNSTUDY OBJECTS callback now sweeps 20 ASCII and 18 Binary allocation offsets with destination rollback, same-adapter retry and typed graph readback in normal and ASan builds ([receipt](docs/research/p6-dxf-sunstudy-objects-allocation-failure-retry-2026-11-05.json)); its source DXF is authored locally for the test and removed after each run. The generated AC1027 direct-NOD RASTERVARIABLES OBJECTS callback now sweeps 11 ASCII and 10 Binary allocation offsets, with destination and caller-record preservation, same-adapter retry and typed readback in normal and ASan builds ([receipt](docs/research/p6-dxf-rastervariables-objects-allocation-failure-retry-2026-11-05.json)); its source DWG is authored locally and removed after each run. The generated AC1027 direct-NOD WIPEOUTVARIABLES OBJECTS callback now sweeps 11 ASCII and 10 Binary allocation offsets, with destination and caller-record preservation, same-adapter retry and typed readback in normal and ASan builds ([receipt](docs/research/p6-dxf-wipeoutvariables-objects-allocation-failure-retry-2026-11-05.json)); its source DWG is authored locally and removed after each run. A generated AC1024 ACAD_MATERIAL projection now sweeps all 37 ASCII and 36 Binary OBJECTS callback allocations, preserving the destination and source material after every fault and proving same-adapter retry plus NOD/dictionary/material readback; it adds no DWG/DXF fixture ([receipt](docs/research/p6-dxf-material-objects-allocation-failure-retry-2026-11-08.json)). A generated AC1027 PLOTSETTINGS graph now sweeps 21 ASCII and 20 Binary callback allocation offsets, preserving the destination and caller graph and proving same-adapter retry plus typed NOD/dictionary/PLOTSETTINGS readback; its DXF input is an in-memory string and no fixture file is added ([receipt](docs/research/p6-dxf-plotsettings-objects-allocation-failure-retry-2026-11-08.json)). A generated AC1027 ACAD_PLOTSTYLENAME `DICTIONARYWDFLT`/`PLACEHOLDER` graph now sweeps 25 ASCII and 24 Binary callback allocation offsets, preserving the destination, caller records and HEADER plot-style binding, then proving same-adapter retry and typed readback. Its temporary DXF input is created locally and removed after a successful run; no fixture is committed ([receipt](docs/research/p6-dxf-dictionary-default-objects-allocation-failure-retry-2026-11-08.json)). These controls close only their bounded callback exception paths; allocations inside other OBJECTS builders and broader whole-drawing failure states remain open. A focused four-worker ASCII/Binary codec test now writes and imports separate files concurrently in normal and ASan builds ([scoped codec concurrency receipt](docs/research/p6-scoped-dxf-codec-concurrency-2026-11-03.json)); this qualifies only isolated dxfRW instances and distinct files, not shared-instance concurrency or general thread safety. Inspector callback closures and borrowed-view lifetime are now explicit: a focused normal/ASan control proves the codec owns value captures through invocation and releases them when cleared; reference captures remain caller-owned. Other source-lifetime boundaries and platform numeric/string policies remain open ([receipt](docs/research/p6-dxf-inspector-closure-lifetime-2026-11-03.json)). XDATA APPID preflight now catches allocation failures during group validation and stages additions transactionally, preserving the prior required-APPID set if any allocation fails. A focused ASCII preflight sweep reaches nine injected failures, confirms no partial set survives each, and verifies fresh-codec admission retry; this closes only the APPID set mutation path, not broader OBJECTS builders or shared-instance concurrency ([receipt](docs/research/p6-dxf-xdata-appid-admission-allocation-rollback-2026-11-05.json)). Completed bounded staging families: GROUP, IMAGEDEF plan, named/root dictionaries, CLASS, raw sections, resolved references, TABLESTYLE/BG/SUN registries, source BLOCK_RECORD and canonical LTYPE. The controls use internal logical resource estimates and do not create format maxima or support claims. Evidence: [GROUP count](docs/research/p6-dxf-group-vector-setter-count-guard-2026-11-03.json), [GROUP nested bounds](docs/research/p6-dxf-group-nested-setter-bounds-2026-11-03.json), [GROUP plan allocation retry](docs/research/p6-dxf-group-handle-plan-allocation-failure-retry-2026-11-03.json), [generated dictionary source allocation](docs/research/p6-dxf-generated-dictionary-source-allocation-failure-retry-2026-11-03.json), [IMAGEDEF count](docs/research/p6-dxf-image-definition-count-and-staging-guard-2026-11-03.json), [IMAGEDEF plan bounds](docs/research/p6-dxf-image-definition-plan-nested-setter-bounds-2026-11-03.json), [IMAGEDEF OBJECTS rollback](docs/research/p6-dxf-image-definition-whole-object-rollback-retry-2026-11-03.json), [core named-dictionary OBJECTS rollback](docs/research/p6-dxf-named-dictionary-core-objects-allocation-retry-2026-11-03.json), [generated GROUP OBJECTS rollback and readback](docs/research/p6-dxf-generated-group-dictionary-objects-allocation-retry-2026-11-03.json), [generated IMAGE dictionary OBJECTS rollback and readback](docs/research/p6-dxf-generated-image-dictionary-objects-allocation-retry-2026-11-03.json), [named-dictionary count](docs/research/p6-dxf-named-dictionary-object-setter-count-guard-2026-11-03.json), [named-dictionary nested bounds](docs/research/p6-dxf-named-dictionary-object-nested-setter-bounds-2026-11-03.json), [root dictionary count](docs/research/p6-dxf-root-dictionary-setter-count-guard-2026-11-03.json), [root dictionary byte bounds](docs/research/p6-dxf-root-dictionary-byte-bound-2026-11-03.json), [CLASS count](docs/research/p6-dxf-class-setter-count-guard-2026-11-03.json), [CLASS byte bounds](docs/research/p6-dxf-class-metadata-setter-byte-bound-2026-11-03.json), [raw sections](docs/research/p6-dxf-raw-section-setter-bounds-2026-11-03.json), [resolved references](docs/research/p6-dxf-resolved-write-reference-setter-bounds-2026-11-03.json), [handle remap](docs/research/p6-dxf-handle-remap-setter-bound-2026-11-03.json), [source table metadata](docs/research/p6-dxf-source-table-metadata-setter-bounds-2026-11-03.json), [scoped DXF codec concurrency](docs/research/p6-scoped-dxf-codec-concurrency-2026-11-03.json). | Keep validation focused; do not reopen a passed guard without a new failure. |
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
