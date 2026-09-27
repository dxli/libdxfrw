# DWG/DXF 3D Support Review and Fix Plan

Status: implementation in progress; DWG reader edits remain evidence-gated.
Review date: 2026-09-26.
Implementation baseline: `423cf99fd26bc9938dc259907e2889666d672d1f`
(`origin/master`, `codex/pr100-clean` after rebase). The local commit was
patch-equivalent to this origin commit and was skipped by rebase. Existing
untracked workspace files, including this plan and `tests/samples/AC1021/` and
`tests/samples/AC1024/`, were preserved and not staged.

**FreeCAD consumer status:** the installable `dwg2dxf` command is a first-class
deliverable for FreeCAD Draft's external DWG-import workflow. Its exact
`dwg2dxf <input> -o <output>` invocation, failure-safe output publication,
installed-artifact discovery, and pinned FreeCAD 1.1.3/macOS arm64 `open`,
`insert`, and registered desktop-dispatch paths are implemented and verified
for the bounded cases recorded under S8.9 and S8.15. A new local, nontracked
AutoCAD-sample probe found one AC1014/R14 `dwg2dxf` failure after successful DWG
reading: DXF generation aborts in the LTYPE table, while the five probed
AC1015/AC1018/AC1021/AC1024/AC1027 files converted. S8.9.11 now fixes the
AC1014 writer rejection by retaining the reachable mandatory-LTYPE →
DICTIONARY → XRECORD graph, and qualifies one installed-artifact FreeCAD
`open()` route against the existing local AutoCAD POINT sample. Treat this as
one concrete converter/consumer tuple, not a blanket version claim; the
external drawing remains untracked and is not added to the repository. This
qualifies neither native Linux/Windows deployment nor every entity's FreeCAD
geometry: keep those as separate platform and per-entity gates. For each
change, run the fast CLI/DXF-readback checks; run opt-in FreeCAD process checks
only when the integration behavior or claimed profile changes. Preserve
correct DXF output when FreeCAD's importer lacks support, and report converter
integration separately from imported-geometry support.

**FreeCAD deployment qualification update (2026-09-24):** the S8.9.8 runtime
record originally relied on FreeCAD's process environment to resolve the
installed converter's dependencies. A clean-environment check already passed
for the macOS `BUILD_SHARED_LIBS=OFF` install artifact and its actual FreeCAD
child process. It exposed a separate shared-build packaging defect: installed
`dwg2dxf` had `@rpath/libdxfrw.2.dylib` but no install RPATH. The converter
target now receives a relocatable path from its install `bin` directory to
`lib` for shared macOS/Linux builds. The new fast installed-artifact test
installs to an isolated prefix and invokes the exact FreeCAD CLI contract with
loader overrides removed; both static and shared macOS runs pass, and `otool`
confirms `@loader_path/../lib` with only the installed libdxfrw and macOS
system libraries linked. S8.9.14 additionally qualifies the freshly installed
shared artifact in FreeCAD 1.1.3/macOS arm64 `open()` and `insert()` processes
for the generated AC1015 3D LINE control. This closes the shared-install
loader/handoff gap for that pinned tuple, not a general shared-binary geometry
claim or native Linux/Windows qualification; those remain S8.9.5. Do not copy
FreeCAD libraries or bundle system runtimes speculatively.

**FreeCAD converter contract audit (2026-09-25):** both the pinned FreeCAD
1.1.3 runtime revision and current upstream `Draft/importDWG.py` resolve
LibreDWG from the shared converter-path preference before searching the
FreeCAD process's `PATH`, use `dwg2dxf.exe` on Windows and `dwg2dxf` on
Linux/macOS, and invoke
`[converter, input.dwg, "-o", output.dxf]`. Its conversion path waits for the
child but currently treats output-file existence—not the child return code—as
success before handing the path to `importDXF.open()` or `importDXF.insert()`.
Therefore the converter's transactional publication rule is an integration
requirement: a failed conversion must not create a new partial final DXF (and
must preserve an existing complete destination). In Automatic mode,
LibreDWG is tried before ODA/QCAD, and a fallback can hide a failed
`dwg2dxf`; any result attributed to this project must identify the actual child
process and exact importer input. The shared preference can also derive the
sibling `dxf2dwg` path, which this project does not provide. Keep setup guidance
and qualification import-only, use an isolated FreeCAD profile, and pin the
FreeCAD revision for every source/runtime claim. These findings refine the
existing S8.9 gates; they do not add a plugin, modify FreeCAD, or promote any
DWG/entity/platform support claim. Runtime contract source: [pinned FreeCAD
1.1.3 `importDWG.py`](https://github.com/FreeCAD/FreeCAD/blob/145529fe741292ff0b3977a01195bf0247425794/src/Mod/Draft/importDWG.py);
drift check: [current FreeCAD `importDWG.py`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py).
Preference documentation: [FreeCAD Import/Export
Preferences](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Import_Export_Preferences.md).

**FreeCAD converter-path setup boundary (2026-09-25):** qualify and document
this as an import-only `dwg2dxf` integration. FreeCAD's `TeighaFileConverter`
preference is shared by DWG import and export. When it is set to a path whose
filename is `dwg2dxf` or `dxf2dwg`, the current resolver substitutes the
requested sibling executable; if that sibling is absent, it does not continue
to PATH discovery. Therefore the recommended import-only setup is to add the
installed `bin` directory to FreeCAD's launch `PATH` and leave any existing
shared converter preference unchanged. A user may configure the exact
`dwg2dxf` path instead, but instructions and results must warn that doing so
can redirect FreeCAD's export lookup to an unavailable sibling. Never install,
symlink, or advertise a fake `dxf2dwg` to make the shared preference appear
complete. FreeCAD's documented platform names are `dwg2dxf` on Linux/macOS and
`dwg2dxf.exe` on Windows; there are no standard LibreDWG install paths, so
the tested PATH or explicit-path route must be stated for each qualification
tuple. In Automatic mode, FreeCAD may continue to ODA/QCAD after a failed
LibreDWG attempt; accept attribution only when the actual child path/hash,
arguments, exit status, and importer input are captured. The CLI's
failure-atomic publication remains mandatory because FreeCAD's wrapper treats
output-file existence as success rather than checking the child return code.
These rules refine existing S8.9.5/platform and FreeCAD handoff criteria; they
do not add a FreeCAD plugin, change FreeCAD behavior, qualify DWG export, or
promote entity support. Source: [pinned converter resolver and handoff
(FreeCAD 1.1.3)](https://github.com/FreeCAD/FreeCAD/blob/145529fe741292ff0b3977a01195bf0247425794/src/Mod/Draft/importDWG.py),
[current resolver and handoff](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py),
and [FreeCAD converter preference documentation](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Import_Export_Preferences.md).

**FreeCAD preference-mode qualification (2026-09-24):** S8.9.12 now verifies
the installed converter under FreeCAD 1.1.3/macOS 27 arm64 PATH discovery in
both LibreDWG-only (`DWGConversion=1`) and Automatic (`DWGConversion=0`) modes,
through separate `open()` and `insert()` runs. The successful Automatic-mode
control launches `dwg2dxf` first and no ODA/QCAD fallback. Automatic mode may
still try those alternatives after a failed conversion; S8.15.5's fallback
failure attribution remains a separate observation. This closes only the
preference-selection gap for that pinned profile, not native platform,
entity-family, version, or general FreeCAD geometry support.

**FreeCAD release decision and next work:** the support target is the real
Draft workflow—FreeCAD discovers an installed `dwg2dxf`, launches it as a
separate process, and imports the complete DXF it produced—not merely that the
library or a build-tree CLI can parse a DWG. Keep three gates explicit:
(1) installed converter/CLI contract, (2) attributed FreeCAD process handoff,
and (3) per-entity geometry construction. Gates 1–2 can pass while FreeCAD
reports an entity unsupported; that is converter integration, not geometry
support. The bounded macOS profile and entity results below are implemented;
native Linux/Windows qualification remains S8.9.5 and is not a prerequisite
for independent code slices. For AC1027 modeler entities, the immediate
FreeCAD-related implementation dependency is the evidence gate S2.3.2, then
the minimal carrier and ODA-acceptance slices S2.3.4–S2.3.6, and only then
the separate `open()`/`insert()` handoff check S2.3.7/S8.15.19. Until those
gates pass, keep those conversions fail-closed while continuing unrelated
FreeCAD-usable entity work. Per-slice validation stays fast and FreeCAD-free;
run the slower process checks only when the launch/handoff behavior changes or
when qualifying a new profile, operation, or entity result.

**Implementation target: `dwg2dxf` as FreeCAD's DWG-import converter
(2026-09-24):** prioritize the installable standalone program FreeCAD Draft
actually launches. Do not create a FreeCAD plugin or link the library into
FreeCAD. The converter must continue to accept FreeCAD's exact
`[dwg2dxf, input.dwg, "-o", output.dxf]` argument vector (while retaining the
existing positional CLI), run noninteractively, emit a complete version-correct
ASCII DXF, and publish it failure-safely. Those command, installation, and
pinned macOS `open`/`insert`/dispatcher checks are already implemented; reuse
them rather than building a second integration harness. Keep two separate
remaining lanes:

1. **Converter implementation:** continue small entity/version slices whose
   DWG interpretation and DXF serialization are independently specified.
   S2.3.2 now narrowly qualifies one ODA-produced AC1027 standalone
   `3DSOLID`/`ASM_Data` carrier. S2.3.4 implements its bounded DWG→DXF
   projection, and generated ASCII/binary fast vectors pass. The emitted file
   has also been opened by Open CAD Studio with its history proxy chain and
   exact SAB bytes intact;
   S2.3.6 ODA acceptance and S2.3.7 FreeCAD handoff are still pending. ODA
   File Converter 27.1.0.0's official macOS GUI accepted the exact output
   DXF and produced an AC1032 DWG; a subsequent R14 ASCII DXF downgrade kept
   a `3DSOLID` but replaced the ACDSDATA/SAB representation with inline SAT,
   so it is only partial reader acceptance, not the required semantic
   round-trip. Retry an ACAD2013 DWG→ACAD2013 DXF round-trip after the desktop
   is unlocked. Keep AC1021 inline SAB and all other producers, entities,
   versions, and lifecycle operations fail-closed until separately evidenced.
   Do not promote this implementation/readback result to general support or
   FreeCAD geometry.
2. **Consumer qualification:** native Linux and Windows installation,
   dependency resolution, executable discovery, and FreeCAD process handoff
   remain S8.9.5 external-platform work. Run that qualification on each native
   platform only when available; it is not a prerequisite for independent
   format slices and must not be inferred from macOS or cross-compilation.

For implementation speed, each relevant slice gets the `dwg2dxf` build, exact
FreeCAD-argv fast conversion, and public DXF readback assertions for only the
changed fields. Run an independent ODA acceptance check once when a complete
new carrier is ready, then the opt-in FreeCAD `open()`/`insert()` checks once
for that handoff; do not repeat full suites or GUI qualification for ordinary
entity edits. A converter integration pass requires correct DXF semantics and
the real process handoff. A FreeCAD geometry-support claim additionally needs
the pinned importer to construct independently expected geometry. Do not
claim DWG export: FreeCAD's separate `dxf2dwg` program is not provided here.

**FreeCAD-first feature priority (2026-09-24):** the intended user workflow is
FreeCAD Draft opening or inserting a DWG through the installed `dwg2dxf`
process and then FreeCAD's selected DXF importer. Prioritize 3D feature rows
that can complete this real end-to-end path, using the already recorded
positive and negative S8.15 entity results to select the next slice. A row is
**FreeCAD geometry support** only when the pinned importer creates geometry
with independently expected coordinates/topology; a correctly typed,
field-preserving DXF for an entity FreeCAD currently ignores is still a
successful **converter integration** result, but not a geometry pass. Keep the
converter's standards-correct output even when that importer limitation is
known. FreeCAD's [DXF preferences documentation](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Import_Export_Preferences.md)
distinguishes C++ and legacy Python DXF importer modes and notes their
different feature coverage, so qualify each mode as a distinct profile and
keep the pinned C++ mode as the current baseline; do not transfer results
between them. This feature-priority rule does not defer parser/writer
correctness for other in-scope entities or make FreeCAD a required build/test
dependency.

**Current SAB-to-FreeCAD boundary (2026-09-24):** AC1021/R2007 inline SAB v2
is now extracted as a bounded opaque payload by S3.2.3, but the DXF writer
still rejects this unqualified SAB-to-DXF route. Consequently the exact
FreeCAD command can parse the DWG and then fail before a complete DXF is
published; FreeCAD receives no importer handoff. This is intentional
fail-closed behavior, not FreeCAD support. Do not change it to “succeed” by
dropping the `3DSOLID`, relabeling SAB as SAT, substituting wireframe/2D
geometry, or publishing a partial file. First qualify a legal DXF carrier and
independent carrier semantics; then use the existing fast CLI/readback gate.
Only after a complete DXF is produced should the opt-in FreeCAD `open()` and
`insert()` checks be run. A legal carrier that FreeCAD's pinned importer
reports unsupported qualifies converter integration only, never FreeCAD
geometry support.

**Explicit unblock conditions and evidence handoff (2026-09-25):** the two
remaining external gates are independent; closing one does not close or delay
the other. Use these packets to turn an external blocker into a bounded,
implementation-ready slice rather than a general support claim.

| Gate | Required external condition / handoff | Acceptance and state transition |
| --- | --- | --- |
| S8.9.5 Linux | A native Linux host or native Linux CI runner with the intended architecture, a usable compiler/CMake toolchain, and an installed, version-pinned FreeCAD release including `freecadcmd`. Cross-compilation from macOS is not a substitute. The handoff records host/OS/architecture, FreeCAD release and source revision, exact install prefix/build mode, and whether shell, desktop-entry, or configured-path launch is in scope. | Build/install the current revision outside the checkout; run the existing installed CLI/readback/failure-publication tests in a clean environment; then, with isolated FreeCAD preferences and fallbacks disabled, prove installed `dwg2dxf` path/hash, exact argv, status, complete output, and identical output path for separate `open()` and `insert()` runs. Verify desktop-entry environment only if it will be claimed; otherwise narrow docs to the passing shell/configured-path route. Mark only the tested Linux profile qualified after its evidence packet is saved. |
| S8.9.5 Windows | A native Windows host or native Windows CI runner with the intended architecture, MSVC/runtime toolchain, and installed, version-pinned FreeCAD release. The handoff records OS/build, architecture, FreeCAD release/revision, compiler/runtime, install layout, and launch route. | Build/install `dwg2dxf.exe`; verify its required DLLs resolve from the documented package/system locations, not FreeCAD-private directories; then run the same clean-profile `open()`/`insert()` attribution and output-handoff checks as Linux. Include paths with spaces and a Unicode/non-ASCII input/output path before claiming those cases. Qualify the desktop launcher only if tested natively; otherwise document only the passing shell/configured-path route. Mark only the tested Windows profile qualified. |
| S2.3.2 ACIS/ACDS semantic contract | A full normative contract remains unavailable. A hash-pinned ODA-produced AC1027 pair from a tracked LibreDWG source, plus independent Open CAD Studio readback, now qualifies one standalone `3DSOLID` / `AcDb3DSolid_ASM_Data` tuple only. Static audits and ODA retention of ODA-authored input alone remain insufficient. | `READY_NARROW` only for this tuple: one `ASM_Data` owner key equals the entity handle; six observed schema definitions; group 94 equals concatenated ordered group-310 bytes; opaque group-2 GUID; existing group-350 history handle; exact SAB payload hash and equal nonempty DWG/DXF semantic mesh results. Preserve bytes and observed order; fail closed on unsupported or ambiguous references. Clone/INSERT/XREF lifecycle, other schemas/entities/producers/versions, DWG modeler writing, and FreeCAD geometry remain blocked. Implement S2.3.4 only within this boundary, then run fast generated vectors, one ODA acceptance on libdxfrw output, independent semantic readback, and only then the separately gated FreeCAD converter handoff. |

For both gates, keep external binaries and converted outputs at their supplied
paths or in unique temporary directories; record hashes and tool logs without
adding external DWG/DXF fixtures to the repository. A partially supplied
packet is useful for narrowing questions, but does not change the blocked
state or promote a support claim. The platform lanes may close one OS at a
time; S2.3.2 may unblock a narrow AC1027 carrier without establishing AC1021
inline SAB, other ACIS versions/entities, FreeCAD B-rep support, or general
INSERT/XREF behavior.

**Unblock execution packets (2026-09-25):** the acceptance rows above now have
the following concrete provisioning and evidence handoffs. These are setup
conditions, not claims that either external gate has passed. Run the two lanes
independently; a platform pass does not satisfy S2.3.2, and an ACDS witness
does not qualify a native FreeCAD installation.

1. **Provision one native FreeCAD runner per platform.** Reserve a native
   Linux host/runner and a native Windows host/runner for the intended
   architectures; record the exact OS image/build and architecture. Install a
   pinned FreeCAD release that includes `freecadcmd`, and record its release,
   source revision/build identifier, executable path, and package or installer
   hash. Also provision a pinned native `dwgadd` from LibreDWG for the existing
   locally authored AC1015 LINE recipe; record its version/build. Do not use a
   macOS cross-build, a FreeCAD bundle's private converter, or a moving
   `latest` download as the qualification environment. If either dependency
   cannot be provisioned, leave that OS `BLOCKED_EXTERNAL_PLATFORM` and keep
   the missing prerequisite explicit.
2. **Run the existing installed-artifact harness, not a new consumer shim.**
   Build the static/default CLI profile and install it to a fresh prefix
   outside the checkout. Reconfigure the test build after installation so the
   installed `dwg2dxf` path exists, then enable
   `LIBDXFRW_ENABLE_FREECAD_INSTALLED_3D_LINE_CONTROL` and point
   `LIBDXFRW_FREECADCMD_EXECUTABLE`, `LIBDXFRW_FREECAD_DWG2DXF_EXECUTABLE`,
   and `LIBDXFRW_DWGADD_EXECUTABLE` at the pinned native tools. Run the two
   existing `dwg2dxf_freecad_installed_3d_line_open` and
   `dwg2dxf_freecad_installed_3d_line_insert` CTests. Also run the fast
   installed-CLI/readback and failure-publication controls on that build. Keep
   loader overrides and build-tree paths out of the process environment; on
   Linux explicitly test without `LD_LIBRARY_PATH`, and on Windows inspect the
   installed `.exe` dependency closure and run from outside the checkout.
   Use separate isolated FreeCAD profiles with fallbacks disabled. The LINE is
   only a launcher/deployment smoke, not ACIS or general 3D geometry evidence.
3. **Save a compact, reproducible platform evidence packet.** Attach the
   source revision, configure arguments/build mode, install prefix layout,
   installed executable SHA-256, FreeCAD and `dwgadd` identities, native
   dependency/load-path report, exact converter argv/status, output DXF hash,
   `open()`/`insert()` handoff paths and explicit macro PASS markers, and the
   CTest log/XML. Redact user-specific secrets, but retain enough environment
   detail to reproduce discovery. Keep generated DWG/DXF and profiles in the
   test's unique temporary/build locations; do not add them as fixtures. A
   passing Linux packet closes only the tested Linux profile; likewise for
   Windows. Qualify desktop-entry/Start-menu discovery and Unicode paths only
   if the evidence packet explicitly exercises those routes; otherwise narrow
   setup guidance to the tested shell/configured-path and path-encoding scope.
   Do not add FreeCAD installation or runtime tests to ordinary fast CI; use a
   provisioned native runner or an explicitly opt-in/manual qualification job.
4. **Request a source-provenance AC1027 pair for the first ACDS slice.** The
   next external handoff is not another source-code search: obtain the actual
   BricsCAD `mm.dxf` cited by the existing public report, if its author can
   supply it, or have an independent CAD producer create one minimal analytic
   `3DSOLID`, save the native DWG, and export the paired AC1027 DXF. Ask for the
   producer name/build, exact create/save/export operations, original files,
   file hashes, DWG/DXF versions, and a read-only transfer/source URL. Keep
   artifacts external and do not treat a screenshot, pasted excerpt, sample
   recipe, or unpinned report as a substitute. Start with one standalone
   AC1027 `3DSOLID`; a `REGION`, other version, or lifecycle operation is a
   separate tuple unless the same packet directly covers it.
5. **Require independent semantic observations, not just byte retention.**
   From the untouched pair, record the entity type/handle, associated
   `ACDSRECORD`, schema and record names/IDs, all relevant group-320 keys,
   group-94 declared length, ordered group-310 chunk lengths and reconstructed
   payload hash, plus the modeler entity's group-2/GUID and group-350/history
   values. Have an implementation independent of both the producer and this
   serializer read the DXF and report the association and payload validity;
   have an independent ACIS-capable application/kernel report semantic solid
   invariants (valid B-rep, extents, volume, and available face/edge counts)
   against the producer's known analytic dimensions. Pin tool versions/builds,
   commands, logs, input/output hashes, and numeric tolerances. ODA-only
   DXF→DWG→DXF retention of ODA-authored bytes, LibreDWG-only self-generation,
   ezdxf/dxfgrabber same-author lineage, static source inspection, or matching
   opaque payload hashes alone remain advisory and do not meet this gate.
6. **Resolve identity policy before setting S2.3.2 `READY`.** Show whether the
   group-320 association survives an independent save/rewrite that changes the
   modeler entity handle, or provide authoritative documentation of that
   behavior. Because libdxfrw assigns output handles, the witness must justify
   whether the key is preserved, remapped, or causes fail-closed output. If
   this operation cannot be evidenced, limit the proposed slice to an
   identity-preserving case only if the current writer can actually preserve
   that identity; otherwise keep the serializer blocked. INSERT/XREF/copy
   behavior may be excluded from the first narrow claim, but must remain
   explicitly unsupported/opaque and cannot be inferred from standalone
   evidence. Once the contract and evidence are complete, mark only that
   entity/version tuple `READY`, implement the narrow carrier, run fast
   generated vectors, then perform one ODA acceptance and one separate
   FreeCAD handoff. Geometry support still requires the pinned FreeCAD importer
   to construct geometry matching the independent semantic oracle.

**Current unblock state:** the repository already contains the installed
FreeCAD open/insert harness and the ACDSDATA opaque passthrough/identity tests;
what is missing is the native Linux/Windows toolchain/runtime evidence and the
external source-provenance semantic packet above. No workflow or support claim
should be treated as unblocked merely because these handoff conditions are now
specified.

**Unblock decision checklist and handoff request (2026-09-25):** treat these as
two separately owned external handoffs, not code changes that can be simulated
on this macOS checkout. A partial packet is useful for diagnosis but leaves its
gate blocked. Keep external DWG/DXF inputs, generated outputs, and FreeCAD
profiles outside version control; check in only harness/code/plan changes and
sanitized evidence summaries, never received or generated drawing fixtures.

* **Native Linux and Windows FreeCAD qualification (S8.9.5).** The unblocker
  must provide one native runner for each OS/architecture being claimed, with
  network/package access or pre-provisioned pinned compiler, CMake, FreeCAD
  `freecadcmd`, and LibreDWG `dwgadd`. Before testing, capture a manifest with
  OS image/build, architecture, tool versions/build IDs and executable paths,
  FreeCAD source revision, and package/installer hashes. Run the existing
  installed-artifact harness from a fresh prefix outside the checkout, in
  isolated FreeCAD profiles and with fallback converters disabled. A platform
  packet is complete only when the installed CLI/readback/failure-publication
  controls and both installed FreeCAD `open()`/`insert()` controls pass; logs
  identify the exact installed `dwg2dxf` path and hash, invocation argv and
  exit status, complete output hash, importer handoff path, and explicit PASS
  marker. Also capture the clean runtime dependency/load-path check (no Linux
  `LD_LIBRARY_PATH`; no Windows dependency resolved from FreeCAD-private
  directories). The authored AC1015 nonzero-Z LINE is only a deployment smoke.
  Test desktop launch, shared-library packaging, Unicode paths, or other
  architectures/releases only when those specific routes are claimed; otherwise
  state their exclusion. Mark Linux and Windows independently qualified only
  from their own native passing packets; S8.9.5 is complete only when every
  required target OS has passed. A macOS result, cross-build, or CI job running
  in a different OS container cannot substitute for a native result. If a
  runner or pinned tool is unavailable, the next action is to provision/request
  that resource and retain `BLOCKED_EXTERNAL_PLATFORM`, not to add another local
  simulation or weaken the criterion.
* **Independent ACIS/ACDS semantic evidence (S2.3.2).** The unblocker must
  supply either a primary normative document that resolves the bounded contract
  below, or an untouched source-provenance DWG/DXF pair produced by a CAD tool
  independent of libdxfrw. The packet must include a stable read-only source or
  transfer reference, producer/tool build and exact create/save/export steps,
  source and derived-file SHA-256 hashes, format versions, and a minimal
  standalone analytic `3DSOLID` tuple. A different implementation must
  independently parse the DXF graph and report the entity-to-`ACDSRECORD`
  association and payload reconstruction; an ACIS-capable independent reader
  must report a valid solid and compare its known extents, volume, and available
  topology counts. Pin commands, builds, logs, tolerances, and all output hashes.
  The evidence table must resolve required entity shell fields, schema names and
  ID namespaces, record names/IDs, group-320 association, GUID/history policy,
  group-94 length versus ordered group-310 chunks, payload timestamp mutation,
  and whether a handle-changing rewrite preserves/remaps/breaks the association.
  Do not mark `READY` if a field or lifecycle rule is inferred only from byte
  equality, a self-round-trip, source inspection, an author report, or an ODA
  round-trip of ODA-authored input. If identity-changing behavior is untested,
  the first implementation must be demonstrably identity-preserving; if this
  library necessarily changes the handle and the key rule remains unknown, the
  serializer stays blocked. Once the complete evidence packet is reviewed,
  mark only its proven producer/version/entity tuple `READY`; implement that
  narrow carrier, run its fast generated-vector tests, then one ODA acceptance
  check. FreeCAD `open()`/`insert()` is a later, separate handoff; successful
  conversion alone never asserts FreeCAD geometry support. If no qualifying
  source pair or normative contract can be supplied, request it from the CAD
  producer/maintainer or spec owner and keep the gate blocked rather than
  repeating broad web searches.

**Gate transition rule:** external evidence changes a blocker to eligible work
only after its complete packet is available for review. Eligibility is not a
support claim: each implementation slice still needs the plan's fast local
checks, and any external acceptance/runtime qualification is reported only for
the exact tested OS/tool/version/entity tuple. Contradictory evidence returns
the affected slice to `BLOCKED` until its scope or contract is corrected.

**Additional AC1027 FreeCAD-route witness (2026-09-24; external and
advisory):** the ACadSharp-maintained `sample_AC1027.dwg` / ASCII-DXF pair
provides a second, structurally richer sample chain than the one-solid ODA
`Cone`: its DXF contains 3DSOLID handles `D65` and `D6A` plus REGION `D69`,
and its `ACDSDATA` section carries matching group-320 keys and chunked group-310
payload records. The exact pair hashes, schema/record counts, and byte-length
checks are recorded under S2.3.1. The adjacent repository's `SAVEAS 2013`
recipe and the payload's AutoCAD producer stamp are provenance clues, not an
independent audit that the checked-in DWG itself was created by AutoCAD. The
local `dwg2dxf` exact-FreeCAD-argv probe parsed all three modeler entities and
reached `writeModelerGeometry()`, then exited 1 without publishing a DXF;
therefore FreeCAD received no importer handoff. This is a concrete AC1027
converter failure case, not evidence that the sample's section is normative,
accepted by ODA, or renderable by FreeCAD. Keep both sample files outside this
repository; use them as external opt-in inputs only after the format/acceptance
gate in S2.3 permits output.

**R2013+ FreeCAD carrier lead (2026-09-24; advisory, not yet a support
claim):** an ODA File Converter 27.1.0.0 AC1027/R2013 `Cone.dxf` has an
`ACDSDATA` section with schema `AcDb3DSolid_ASM_Data`, record `ASM_Data`,
group 320 handle `20B` (the corresponding `3DSOLID` handle), declared byte
count 1980, and chunked group-310 bytes. The matching ODA-resaved AC1027 DWG
exposes `has_ds_data` for that solid; LibreDWG 0.14 minJSON reports one SAB-v2
payload of the same size. Reconstructed payloads differ only at 1-based byte
positions 100 and 103; both are characters in the embedded ACIS timestamp
string (`Thu Sep 24 17:43:57 2026` vs `Thu Sep 24 17:44:56 2026`). Their
exact semantic/update rule is not established. The ODA File Converter 27.1.0.0
DXF→AC1027 DWG→DXF round trip retained all six schemas and the one solid
record; only four embedded timestamp characters changed on that second pass.
This establishes an ODA-reader/writer acceptance route for ODA-produced input,
not independent authorship, normative DXF syntax, AutoCAD interoperability,
or acceptance of libdxfrw-produced output. LibreDWG 0.14 does not
preserve/interpret `ACDSDATA` in its DXF route. The narrow source-key→output-key
mapping, witnessed entity envelope, schema emission, and history/material
closure now have a bounded implementation; Open CAD Studio reads the emitted
DXF and confirms the exact SAB bytes. Keep every other tuple fail-closed and
keep support claims blocked until ODA accepts actual libdxfrw output. The
material object's advanced appearance fields are not yet preserved. The
current FreeCAD C++ source audit has no `3DSOLID` construction route, and the
pinned AC1015 SAT-v1 runtime check reports the record unsupported. Carrying
this section can preserve data for a capable downstream reader, but cannot
create FreeCAD solid geometry without a separately verified importer path.

**Locally authored ODA/LibreDWG conversion probe (2026-09-25; scratch-only,
negative evidence):** LibreDWG 0.14 `dwgadd` generated an analytic box from a
local recipe (`version 2004; box (1 2 3) (0 0 1) 4 3 2`) as AC1018 DWG
`5cd281c399312666b9e74f1a436da8c01aadc867900a565f921ee56df60b79de`; its
LibreDWG-produced AC1018 DXF is
`9854ac8cf810712aa858e80ee41566c342081f9e848e3d888c35b5cbe16205ac` and
contains a SAT-text `3DSOLID` body. Reproduce the source pair with:

```sh
dwgadd --as r2004 -o build/oda-libredwg-pairs/libredwg_box_ac1018.dwg \
  build/oda-libredwg-pairs/acis_box.dwgadd
dwg2dxf --as r2004 -y \
  -o build/oda-libredwg-pairs/libredwg_box_ac1018.dxf \
  build/oda-libredwg-pairs/libredwg_box_ac1018.dwg
```

ODA File Converter / SDK 27.1 was run with the seed DXF as input (`*.DXF`,
recursion and audit off) and `2018 DWG` output. It converted that DXF to an
AC1032 DWG at
`build/oda-libredwg-pairs/oda-from-dxf-ac1032/acis_box.dwg` (SHA-256
`1d49fa53ceb761e6de4a9227c41567a45c85438c821888f5208ee84a2a037910`).
LibreDWG 0.14 `dwgread -O minJSON` returns `SUCCESS` and exposes the AC1032
DataStorage directory, but its readback reports `_data_`/`schdat` segments
unhandled and zero linked SAB payloads. `dwg2dxf --as r2013` still writes a
typed AC1027 `3DSOLID` shell (`group 70=168`, zero GUID, `group 350=0`), but
the output DXF (`af67c700a6dd56a1c033d33460609c37ffbc1221c2db71e1889ac06b5b3aed68`)
contains no SAT body or `ACDSDATA`; LibreDWG also reports a `revision_bytes`
read error and empty history/GUID. The current libdxfrw `dwg2dxf` run on the
same ODA DWG exits 1 with `Conversion failed` and publishes no DXF.
LibreDWG wrote its derived AC1027 DXF with
`dwg2dxf --as r2013 -y -o <readback.dxf> <ac1032.dwg>`.

This is a useful, hash-pinned cross-tool failure reproducer—not the S2.3.2
unblock packet. The geometry recipe and seed DXF were authored through
LibreDWG, so this is not an independent CAD-authored witness; the independent
LibreDWG readback cannot validate the ACDS record semantics or solid geometry,
and the ODA conversion changes the drawing version. In particular, do not
infer that ODA rejected the solid or that the DXF carrier is invalid from
LibreDWG's unsupported DataStorage path. All files remain under ignored
`build/oda-libredwg-pairs/`; do not move them into tracked fixtures. Continue
to require the source-provenance independent CAD pair and independent semantic
reader described in S2.3.2 before enabling an ACDSDATA serializer.

**Independent R2013+ DXF witness and generated-control route (bounded):** the
ezdxf maintainer's analysis of a BricsCAD-authored AC1027 DXF describes an
`ACDSRECORD` whose group 320 refers back to the `3DSOLID` handle, with SAB
bytes in group-310 chunks; the shown entity has `AcDbModelerGeometry`, group
290=0, an all-zero GUID, `AcDb3dSolid`, and group 350=0. This independently
supports handle-based association and the broad section/record shape, but it
is a public analysis rather than a hash-pinned source artifact or a normative
specification. Its 290/GUID/history values differ from the ODA witness (290=1,
nonzero GUID, nonzero history handle), so none of those fields may be copied or
interpreted by analogy. The account also describes an ID/name-based record
layout, but does not establish equivalence between its identifiers and the
ODA witness's schema/object identifiers. See the
[BricsCAD DXF analysis](https://stackoverflow.com/questions/68259913/how-to-represent-read-a-cube-in-dxf-file).

**Pinned ezdxf implementation audit (2026-09-24; candidate behavior only):**
the local ezdxf v1.4.4 source at commit
`dd7f6cc34983fdbf0b2b910f2497e0720d06dd16` provides a concrete candidate
reader/writer shape in `src/ezdxf/sections/acdsdata.py`: it finds an ACIS
record by matching its `AcDbDs::ID` group 320 to the modeler entity handle,
and writes an `ASM_Data` record with group 280, declared group-94 byte count,
and ordered group-310 chunks. Its `Body` exporter routes AC1027+ SAB into
`ACDSDATA`. This is useful implementation evidence and narrows the candidate
association algorithm; it is not a normative DXF contract, independent
semantic oracle, or proof that arbitrary handle changes are safe. ezdxf itself
labels the section implementation work in progress, its local tests exercise
synthetic self-round-trips, and the inspected local DXF section example carries
thumbnail data rather than a solid. Static source inspection only was possible
in this environment: Python dependencies are missing, so no ezdxf test or
generated-file acceptance result is claimed. Do not install dependencies or
modify the sibling ezdxf checkout as part of this plan.

The audit makes the handle-lifecycle risk concrete in libdxfrw: entity writing
mints output handles and records source-to-output mappings, while raw DXF
section replay currently leaves group 320 unchanged. Therefore opaque replay
can produce a stale ACDSDATA-to-solid link whenever the entity handle changes.
A typed association/remap is a candidate implementation, but must be validated
against a version-pinned independent reader before it is enabled. This remains
converter/carrier work for FreeCAD's `dwg2dxf` route; it cannot make FreeCAD's
current DXF importer create a 3DSOLID shape.

For a reproducible second-author control, ezdxf documents generating a simple
polyhedron ACIS body from a mesh and exporting it in R2013+ SAB form; its
documented interoperability is limited to simple polygonal bodies and named
CAD readers, not arbitrary ACIS data. Its documentation also notes that the
DXF reference does not describe ACDSDATA, so treat this solely as a second
writer/control, not a normative specification. Use a pinned ezdxf release
only in an opt-in scratch generator, then have ODA File Converter turn that
locally created DXF into an AC1027 DWG for the `dwg2dxf` path. Keep all generated
DWG/DXF files in a unique temporary directory and never check them in. This
provides an independent authoring route for a small end-to-end regression; it
does not by itself prove Autodesk acceptance, arbitrary SAB preservation, or
FreeCAD solid construction. See the
[ezdxf ACIS export limits and examples](https://ezdxf.readthedocs.io/en/stable/acis.html)
and LibreDWG's
[minimal R2018 data-loss reproducer](https://github.com/LibreDWG/libredwg/issues/1411).

**ACDS evidence-gate recheck (2026-09-25; no state promotion):** current
upstream [ezdxf ACIS DXF API](https://github.com/mozman/ezdxf/blob/master/src/ezdxf/acis/dxf.py)
confirms that `export_dxf()` routes AC1027+ bodies through SAB into ACDSDATA;
the same project's [DXF structure notes](https://github.com/mozman/ezdxf/blob/master/docs/source/dxfinternals/filestructure.rst)
still describe ACDSDATA as undocumented by the DXF reference. The historical
[dxfgrabber ACDSDATA reader](https://projects.blender.org/blender/blender-addons/commit/82a00ee2a0d8f2001917ddc4e34de4104200eca3)
groups `ACDSRECORD` subsections on group 2, looks up `AcDbDs::ID` and
`ASM_Data`, treats the third ID-section tag (group 320) as the modeler-entity
handle, and concatenates group-310 bytes. This corroborates one candidate
reader algorithm, but dxfgrabber is the same author's predecessor to ezdxf,
is parser-only here, and does not independently qualify record lengths,
GUID/history behavior, or semantic geometry. The public
[LibreDWG #1411 reproduction](https://github.com/LibreDWG/libredwg/issues/1411)
uses ezdxf-generated data and demonstrates loss through LibreDWG, not
independent acceptance. The reported BricsCAD-authored `mm.dxf` remains
unavailable with no downloadable artifact/hash; the new RealDWG-Web changelog
also mentions attaching ACDSDATA to solids, but the implementation is in a
private package and cannot be audited or reproduced. `ezdxf` is not installed
in this environment, so no generated-file or runtime result is claimed.

The shortest unblock is now explicit: obtain the actual BricsCAD `mm.dxf` (or
an equivalent nonconfidential, source-provenance AC1027 DWG/DXF pair) with
producer build/operation and hashes; run at least one independently implemented
reader against the untouched DXF and compare the linked SAB with independently
expected simple-solid semantics; verify group-94 length against ordered
group-310 chunks and record the GUID/history fields without assigning
unproven meaning. Keep the artifact external and hash-pinned. If only one
entity/schema/producer/version tuple is established, mark only that narrow
S2.3.2 contract `READY`; do not infer clone, INSERT/XREF, other entity, or
FreeCAD B-rep support. Until this packet exists, keep the production
ACDSDATA serializer fail-closed and continue unrelated ready converter work.

The current converter failure is localized: `dwg2dxf` reads the AC1027
`3DSOLID` at handle `20B`, resolves its linked DataStorage record, and reaches
the DXF modeler writer; `dxfRW::writeModelerGeometry()` then fails closed
because no qualified carrier is available and publishes no output file. The
`dx_iface` CLI adapter currently has no `addDataStorage()` override, while
`dwgR` delivers that callback after entity/object traversal. The implementation
must therefore buffer explicit entity/data identities and join them after
reading completes, then serialize transactionally; callback order is not an
association. This trace confirms the work is a DXF writer/adapter gap, not a
reason to relax DWG parsing or silently omit the solid.

The observed ODA AC1027 DXF entity envelope is not a license to emit the
current writer's DWG SAB version as DXF group 70. That sample's entity has
group 290=1, group 2 GUID, and group 350 history handle, but no group 70 or
inline groups 1/3. Autodesk's published `3DSOLID` DXF reference describes
group 70 as modeler format version 1 with groups 1/3 for text, and does not
document the ODA sample's group 290/GUID envelope. Treat this as a gated
format discrepancy: S2.3 must establish the target-version entity shell and
ACDSDATA projection before the production writer changes. In particular,
DWG SAB encoding version 2 and DXF modeler format group 70 are distinct
fields, not interchangeable version numbers.

**DXF modeler-carrier research (2026-09-24):** Autodesk's published
[`3DSOLID` DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm)
defines group 70 as modeler format version (currently 1), group 1 as
proprietary text, and group 3 as continuation text. Autodesk's [common entity
codes](https://help.autodesk.com/cloudhelp/2023/ENU/AutoCAD-DXF/files/GUID-3610039E-27D1-4E23-B6D3-7E60B22BB5BD.htm)
define group 310 as proxy-entity graphics, not as a `3DSOLID` SAB carrier.
Thus the AC1021 DWG's version-2 SAB cannot be emitted by merely copying its
bytes into group 310 or by labeling it as the version-1 text carrier. A future
conversion must establish a real ACIS-aware SAB-to-SAT/text transformation (or
another explicitly documented, reader-accepted DXF route), with an independent
semantic/carrier oracle. Until then, retain the fail-closed writer result; do
not infer validity from FreeCAD opening the file or from a generic binary-group
round trip.

The same-input converter investigation also ran LibreDWG 0.14 on the available
ODA-resaved AC1024/R2010 `Cone.dwg`: its SAB-v2-to-SAT1 path likewise reports
unknown SAB tag 18 and writes no group-1/3 carrier despite a successful process
exit. This is one adjacent-version diagnostic, not a claim about all AC1024
drawings; it rules out treating this LibreDWG build as a working conversion
oracle for the two tested sample/version pairs.

**Normal FreeCAD launch remains a separate deployment check:** the existing
macOS desktop-dispatch test starts FreeCAD's app executable under a controlled
test environment; it does not prove Finder/LaunchServices, a Linux desktop
entry, or the Windows Start menu inherits a shell's modified `PATH`. Every
end-user setup recipe must name the launch route it supports and prove that
route resolves the installed converter. If only a controlled shell launch or
an explicitly configured executable path is qualified, keep README/man claims
at that boundary; do not imply that changing a terminal's `PATH` affects an
already-running desktop session.

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

The named application-integration deliverable is also the standalone,
installable `dwg2dxf` executable used by FreeCAD Draft for DWG import. Treat
that external-process workflow as a first-class consumer alongside library
adapters: FreeCAD must be able to find and launch the installed program, and
the generated DXF must preserve the converter's supported entities/fields.
This does not add a FreeCAD plugin, link libdxfrw into FreeCAD, provide FreeCAD's
separate `dxf2dwg` export program, or imply that FreeCAD's own DXF importer
supports every legal DXF entity.

### FreeCAD `dwg2dxf` completion gate

FreeCAD is an explicit external consumer target for the installable
`dwg2dxf` executable, not an incidental CLI smoke test: FreeCAD Draft launches
the converter as a separate process and then imports its DXF. For each claimed
FreeCAD profile, completion requires all of the following in the same pinned
profile: (1) the installed, platform-named executable and its declared runtime
dependency closure work outside the build tree and without borrowing private
runtime libraries from the FreeCAD installation; (2) FreeCAD invokes it with
its actual argument vector
`[dwg2dxf, input.dwg, "-o", output.dxf]` and the converter exits successfully
while publishing a complete ASCII DXF; and (3) the selected FreeCAD DXF
importer receives that exact output path. Keep two non-interchangeable result
labels: **FreeCAD converter integration** requires the first three process and
handoff checks plus correct DXF semantics; **FreeCAD geometry support** also
requires imported geometry matching independent expected values. A file
existing or an importer accepting the DXF is not a geometry pass. `open()` and
`insert()` are separate claims; desktop-app use additionally requires the
desktop dispatcher gate. Disable fallback converters for positive
attribution, and preserve the converter's nonzero-Z/entity semantics rather
than changing output to hide limitations in FreeCAD's importer. A failed
conversion must not leave a partial final DXF that FreeCAD's file-existence
check could mistake for success.

This is qualified by a specific tuple—FreeCAD revision, OS/architecture,
launch/discovery route, importer mode, operation, DWG version, and tested entity
semantics. Evidence does not transfer across tuple dimensions. Keep FreeCAD
optional and out of default CI: fast installed-CLI/readback tests are the
per-change gate, while native FreeCAD runtime checks run only for changed
integration behavior or a new qualification tuple. The current narrow runtime
baseline is FreeCAD 1.1.3 on macOS arm64; native Linux/Windows remain
unqualified. S8.9.1–S8.9.4 and S8.9.6–S8.9.12 are implemented/committed for
their bounded macOS profiles; S8.9.5 retains the native-platform qualification
gap and S8.15 gates entity semantics. The current result therefore supports only
the explicitly recorded macOS FreeCAD 1.1.3 profiles and entity rows; do not
write an unqualified “FreeCAD supported” claim or infer Linux/Windows support.
Broader platform claims require native install/dependency/discovery and
FreeCAD-process evidence under S8.9.5. Continue those existing work items
rather than creating a duplicate FreeCAD harness or treating a CLI-only pass
as application integration.

**FreeCAD support ledger and implementation priority:** treat usable FreeCAD
integration as a short, separately releasable converter lane—not as the end of
the broader 3D-support plan. The current status and remaining gate are:

| Gate | Evidence-backed status | Remaining work |
| --- | --- | --- |
| Standalone/installable command | `dwg2dxf` accepts FreeCAD's exact `input -o output` form; installation, default ASCII, source-version policy, no-prompt behavior, and failure-safe output publication have fast coverage. The fast CLI test also verifies that DXF input/readback does not silently downgrade a declared AC1027 `$ACADVER`. S8.9.11 now converts the local AC1014/R14 sample and verifies the retained LTYPE extension graph; the external sample is not committed. | Repeat install/dependency/argv checks on each platform before making that platform claim; Windows Unicode argv remains unqualified. |
| FreeCAD process discovery and handoff | On the pinned FreeCAD 1.1.3/macOS 27 arm64 profile, isolated `freecadcmd` checks cover PATH and configured-path discovery, `open()` and `insert()`, exact process identity/argv, and the same output path passed to the DXF importer. S8.9.8 verifies LibreDWG-only mode (`DWGConversion=1`); S8.9.12 additionally verifies Automatic mode (`DWGConversion=0`) selects the installed converter first with no fallback for successful LINE `open()`/`insert()`. S8.9.11 additionally installs this build under a temp prefix and verifies AC1014 POINT `open()` through the actual C++ DXF importer, including the expected `(50,50,50)` vertex and no unsupported features. | S8.9.5: qualify native Linux/Windows FreeCAD process environments and Windows `dwg2dxf.exe`, using the same launch route named in setup guidance; do not infer them from macOS or cross-builds. |
| FreeCAD desktop dispatch | S8.9.6 covers the registered open/insert dispatcher in the pinned macOS desktop process for its bounded LINE control. Its controlled app launch does not establish Finder/LaunchServices PATH inheritance. | Repeat for a newly claimed platform or changed dispatch/deployment behavior. Qualify the documented normal launch route or explicitly bound the setup claim to a tested shell/configured-path route; menu/file-dialog automation and viewport rendering remain outside this gate. |
| Imported feature semantics | S8.14/S8.15 contain entity-specific positive and negative findings; unsupported entities remain correctly typed in converter output. S8.15.18 additionally verifies one tilted AC1015 3DFACE survives the installed converter route, while pinned FreeCAD 1.1.3 C++ reports it unsupported and creates no shape. | Extend only per S8.15 with an independent semantic oracle. Never delay the installable converter or rewrite legal DXF entities to hide a FreeCAD importer limitation. |
| AC1027 modeler conversion | The external ACadSharp-maintained AC1027 sample has two solids plus a region and three keyed ACDSDATA records; exact FreeCAD CLI invocation currently fails closed at modeler serialization with no importer handoff. | Resolve S2.3's entity/section/identity contract, qualify complete output with ODA first, then run the opt-in FreeCAD `open()` and `insert()` handoff separately. No FreeCAD solid-geometry claim is implied. |

**FreeCAD entity-readiness matrix (pinned C++ importer, macOS arm64):** use
the existing results to choose small end-to-end slices instead of treating all
3D entities as one FreeCAD capability. This is a consumer matrix, not a
format-wide support declaration.

| Result in the pinned profile | Bounded evidence | Implementation consequence |
| --- | --- | --- |
| Geometry created with checked semantics | LINE through DWG→installed `dwg2dxf`→DXF importer (`open()` and `insert()`); POINT including one AutoCAD-authored AC1027 sample; AC1015 3D POLYLINE; AC1015 planar LWPOLYLINE; full XY ELLIPSE. | Keep these as positive smoke controls for their exact entity/version/profile rows. When modifying shared CLI, installation, or importer handoff, reuse LINE/POINT controls rather than replaying the feature matrix. |
| Valid converter output, importer rejects or loses semantics | Tilted 3DFACE, SOLID, and SAT-v1 3DSOLID are reported unsupported; HELIX is unhandled by the audited C++ importer; MESH/PFACE and MINSERT can be silently misconstructed; oblique ARC/CIRCLE, tilted/partial ELLIPSE, and elevated/oblique 2D POLYLINE lose geometry semantics. | Preserve correct typed DXF and field readback. Record downstream limitation; do not flatten, decompose, drop, or substitute records to make the current importer appear to succeed. Geometry support requires a separately pinned importer path/build with independent geometric assertions. |
| No qualified legal converter handoff yet | AC1021/R2007 SAB-v2 `3DSOLID` currently fails closed before DXF publication; the R2013+ ACDSDATA carrier remains specification/acceptance-gated under S2.3. | Keep this row explicitly converter-blocked as well as FreeCAD-geometry-unqualified. Do not send FreeCAD partial output or use its file-existence behavior as the only success test. |

**FreeCAD implementation order:** preserve the completed installable-CLI and
macOS handoff baseline; keep entity changes gated by fast exact-argv
conversion plus public DXF readback; use the already-positive LINE/POINT
profiles as shared-route smoke controls. S8.9.12 closes successful
Automatic-mode selection for the pinned macOS profile without feature-matrix
replay. The remaining short-term consumer qualification is native
Windows/Linux executable installation and discovery under S8.9.5, not broad
parser work.
Continue useful parser/writer slices while those external hosts are
unavailable. For a new geometry claim, first confirm
the selected FreeCAD importer has a construction path, then add one bounded
entity/version case and exercise `open()`/`insert()` separately as claimed.
Importer gaps are not a reason to delay field-preserving converter support or
to claim that the converter alone can supply FreeCAD's missing geometry code.

For implementation speed, execute in that order: keep the dependency-free
installed-CLI/readback checks fast and per-change; run FreeCAD headless checks
only when its handoff/discovery contract changes or a new platform/profile is
being qualified; run desktop dispatch only for a new platform or changed
registration path; and add importer geometry checks only for a specific
entity/version row with independent expected geometry. A passing earlier gate
is useful deliverable progress but cannot promote a later gate. A plan item
blocked by unavailable FreeCAD/platform infrastructure must be recorded as an
external qualification gap while converter/parser slices continue.

**Fast implementation gate for the FreeCAD `dwg2dxf` path:** whenever a
reader, coordinate conversion, entity writer, source-version policy, or CLI
change can affect FreeCAD's import route, build the `dwg2dxf` target and run
`dwg2dxf_version_policy` plus `dwg2dxf_freecad_cli_compat`, then run only the
affected fast entity/readback test. Exercise the actual argv as separate
arguments—`<installed-or-test-binary> <input.dwg> -o <output.dxf>`—and verify
the output through the public DXF reader, including `$ACADVER` and the changed
entity fields against independently specified expectations. Keep paths with
spaces in the base CLI case; qualify Unicode paths natively per platform.
Generated input/output belongs in the build or unique temporary directory,
never in the source tree by accident. Do not require a FreeCAD installation
for this per-slice gate.

Run the slower FreeCAD process gate only when changing executable discovery,
installation/runtime dependencies, argument/output behavior, FreeCAD-facing
documentation, or when adding a new OS/revision/importer/operation claim. That
gate must use an installed binary outside the build tree, isolated preferences,
FreeCAD's LibreDWG-only setting (`DWGConversion=1`) to prevent a successful
ODA/QCAD fallback from being misattributed, and recorded converter path/hash,
argv/status, output path, and importer handoff. Test `open()` and `insert()`
separately when claiming both; reserve the desktop dispatcher for explicit
desktop-route claims. If FreeCAD is unavailable, retain the fast converter
evidence and leave only the corresponding external consumer qualification
open—do not block independent implementation slices.

**S8.9.12 — FreeCAD Automatic-mode success attribution (`COMMITTED`).**
Dependencies: S8.9.1–S8.9.3 installation/CLI contract, S8.9.2
configured-path/PATH harness, S8.9.7 failure-publication check, S8.9.8
installed-binary `open()`/`insert()` controls, and S8.15.5 fallback
attribution. Extended `tests/freecad_dwg2dxf_import_check.FCMacro` and its
existing CMake runner to accept `DWGConversion=0` while retaining FreeCAD's
real ODA/QCAD resolvers in Automatic mode; captured child processes must show
exactly one `dwg2dxf` invocation or attribution fails. Added the opt-in
`LIBDXFRW_ENABLE_FREECAD_AUTOMATIC_3D_LINE_CONTROL`, with distinct `open()` and
`insert()` CTests using the existing locally authored AC1015 nonzero-Z LINE
recipe. It is OFF by default, creates DWGs only under unique temporary roots,
and adds no drawing fixture. README and man-page guidance now distinguishes
the tested Automatic preference from its still-available failure fallback.

Validation: `cmake --build build --target dwg2dxf --parallel 2` passed;
`dwg2dxf_version_policy` and `dwg2dxf_freecad_cli_compat` pass 2/2; the four
focused installed-artifact FreeCAD tests pass 4/4 (existing LibreDWG-only and
new Automatic modes × `open()`/`insert()`). The two Automatic CTests alone pass
2/2 in 1.81 seconds. On FreeCAD 1.1.3 revision
`145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode
2, isolated preferences and PATH discovery, `DWGConversion=0` and an empty
`TeighaFileConverter` preference select
`/private/tmp/libdxfrw-freecad-auto.m85IDP/bin/dwg2dxf` (SHA-256
`54c9a6fedd8a4dd8ef59e3490b1a730f68e0defa70e665fb322dbc8c2757d00f`). Each
operation starts exactly that installed binary once with FreeCAD's exact
`[binary, input, "-o", output]` argv, status 0, and hands the identical
complete AC1015 DXF to its corresponding importer; both create one LINE with
bounds `(1,2,3)-(4,6,9)` and report no unsupported features. The process-count
assertion confirms that ODA/QCAD fallback did not run. These tests initially
abort before application code inside the sandbox because Qt cannot see the
host `neon` feature; they pass outside the sandbox. The existing no-partial
failure gate remains S8.9.7 and was not redundantly replayed. This is one
pinned profile and generated control only; it does not qualify native
Linux/Windows, desktop-launch environment, other importer modes, DWG families,
or general FreeCAD 3D geometry support.

**Resolved `dwg2dxf` source-version gap (S8.9.11):** before the change, a
local, user-owned AutoCAD sample `point3d_R14.dwg` (AC1014) reached the end of
DWG reading but failed during DXF TABLES/LTYPE emission; explicit `-v2000` did
not avoid the failure. The five sampled AC1015/AC1018/AC1021/AC1024/AC1027
conversions passed. The immediate rejection was the three mandatory records
`BYBLOCK`, `BYLAYER`, and `CONTINUOUS` carry extension dictionaries at handles
`0x2AB`, `0x2AD`, and `0x2AF`. Each dictionary owns one `ACAD_XREC_ROUNDTRIP`
XRECORD; each XRECORD preserves original linetype spelling through codes 102,
1, and 2 (`EXTNAMES`, uppercase key, display-case name), plus its reactor link.
The prior writer rejected those canonical rows because it would drop their
extension-dictionary handles. The R14 sample is locally supplied and
nontracked; its SHA-256 is
`7e6b1378b053f20de2cf9d153da3485e6475d0cd5845d5a8144b09cfe17cdc46`. The
independent LibreDWG DXF export retains the same table links and
dictionary/XRECORD closure. S8.9.11 now preserves that closed graph in the
converter output and FreeCAD handoff; keep the sample outside the repository
and do not commit it. The normal support boundary remains version-, entity-,
operation-, importer-, and platform-specific.

**S8.9.11 — AC1014 FreeCAD converter regression (COMMITTED).** Dependencies:
S1's checked table-entry contracts and committed S8.9/S8.9.10 CLI/version
policy. Implementation and completion evidence:

1. Reproduced the AC1014 failure with both the exact FreeCAD argv
   (`dwg2dxf input.dwg -o output.dxf`) and the legacy CLI form. DWG parsing
   completed; conversion rejected the three mandatory LTYPE rows because each
   carried an extension dictionary and the converter would otherwise discard
   it. The three source links target DICTIONARY handles `0x2AB`, `0x2AD`, and
   `0x2AF`; each owns an `ACAD_XREC_ROUNDTRIP` XRECORD (`0x2AC`, `0x2AE`,
   `0x2B0`) preserving codes 102/1/2 and a reactor link. The user-supplied
   sample and independent LibreDWG export stayed local and outside the repo.
2. Added typed capture and DXF emission for the reachable DICTIONARY/XRECORD
   closure without adding a pure virtual to `DRW_Interface`. Canonical
   `BYBLOCK`/`BYLAYER`/`CONTINUOUS` rows now retain their appData, extData,
   reactors, and xDict links before `ENDTAB`; source LTYPE handles map to
   canonical output handles `0x14`/`0x15`/`0x16`. AC1014 table/OBJECTS owner
   fields emit group 330. XRECORD code/value order is retained, including
   group-102 payloads. Autodesk's [symbol-table codes](https://help.autodesk.com/cloudhelp/2016/ENU/AutoCAD-DXF/files/GUID-5926A569-3E40-4ED2-AE06-6ACCE0EFC813.htm),
   [DICTIONARY](https://help.autodesk.com/cloudhelp/2017/ENU/AutoCAD-DXF/files/GUID-40B92C63-26F0-485B-A9C2-B349099B26D0.htm),
   and [XRECORD](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-24668FAF-AE03-41AE-AFA4-276C3692827F.htm)
   references anchor the output fields; no DWG byte-layout/parser change was
   needed.
3. The adapter emits only the closed object graph reachable from mandatory
   LTYPE xDict links and its owner/reactor references. It fails closed on
   duplicate handles, missing or null child objects, dangling XRECORD handle
   references (ordered and legacy fallback forms), cycles, unsupported/raw
   payloads, and unsafe handle collisions. It preserves stable handles unless
   reserved output handles require consistent remapping. From-scratch AC1014
   positive tests verify `$ACADVER`, canonical LTYPE links, DICTIONARY owners
   and entries, XRECORD reactors and ordered 102/1/2 values, public DXF-reader
   readback, and remapping collisions with fixed handles `0x14`/`0x15`. It
   also verifies preservation of both dictionary `hardOwner` values and
   remapping of XDATA group 1005 handles when their source handle collides.
   Negative tests verify missing child, duplicate source handle, unresolved
   XRECORD reference, null dictionary child, and no published partial file.
   A separate input test asserts XRECORD group/value order. No external DWG or
   DXF was copied into the repository.
4. Installed the changed executable outside the build tree at
   `/private/tmp/libdxfrw-r14-install-final-verified/bin/dwg2dxf` and ran
   `tests/freecad_dwg2dxf_3d_point_check.FCMacro` via FreeCADCmd with isolated
   preferences, PATH discovery, `DWGConversion=1`, and ODA/QCAD fallbacks
   disabled. FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer
   mode 2 invokes exact argv `[installed-dwg2dxf, input, -o, output]`, returns
   zero, and imports the identical AC1014 DXF. It creates one valid
   vertex-only `Part::Feature` at `(50,50,50)` and reports no unsupported
   features. Installed executable SHA-256:
   `6a60077d4389437e1e9dd0e2be761062d2f0b792caac432a23fde4f96c1289e4`;
   external input SHA-256:
   `7e6b1378b053f20de2cf9d153da3485e6475d0cd5845d5a8144b09cfe17cdc46`;
   converter DXF SHA-256:
   `2a7910281628144cf806a9733f076ac30f3a9248cad8de174acd1955e39ffd8a`.
   This qualifies only this installed macOS FreeCAD `open()` route and this
   AC1014 POINT drawing; no `insert()`, GUI dispatch, other FreeCAD revision,
   platform, or entity/version tuple is inferred.
5. Build targets `libdxfrw_dxf_fixture_tests`,
   `libdxfrw_dwg_object_vectors_tests`, and `dwg2dxf` succeed. Focused tests
   `libdxfrw_fast_focus`, `libdxfrw_graph_preservation`,
   `libdxfrw_writer_primitives`, `libdxfrw_writer_version_matrix`,
   `libdxfrw_dxf_fixtures`, `libdxfrw_dwg_fixtures`,
   `dwg2dxf_version_policy`, and `dwg2dxf_freecad_cli_compat` pass 8/8;
   `git diff --check` passes. Fast-focus exposed one stale vector assertion
   contradicted by already-committed S0.7's valid null DWG dictionary-reference
   behavior; the assertion now checks that established contract. The full
   suite was not run because the focused suites cover the touched writer
   boundaries. All generated sample/output/config artifacts stayed outside
   the repo. Continue native Linux/Windows qualification and unrelated
   entity/version evidence under S8.9.5/S8.15; these do not block other slices.

### FreeCAD end-user support target

The concrete user outcome is: a user builds/installs this project's
`dwg2dxf`, configures FreeCAD Draft to select the LibreDWG converter lane, and
opens or inserts a DWG through FreeCAD's own registered handler. FreeCAD must
launch the installed executable as a separate process; no FreeCAD plugin,
in-process library link, or automatic FreeCAD packaging is implied. The
supported integration is **DWG import through DXF**, not DWG export.

**FreeCAD failure-detection constraint (source-audited 2026-09-24):** the
current [`Draft/importDWG.py`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py)
`convertToDxf()` waits for the LibreDWG subprocess but decides whether
conversion succeeded by checking `os.path.exists(result)`; it does not check
`proc.returncode` before handing the path to the DXF importer. FreeCAD
currently allocates a fresh temporary output directory for
each conversion, so stale prior output is not the normal path, but a partial
DXF left by a failed `dwg2dxf` run would still be treated as a successful
conversion. Therefore `dwg2dxf` must publish the requested output only after
a complete successful write, and on failure leave no new output at that path
(or preserve an existing complete file without truncating it). FreeCAD
integration tests must assert both the process status and output validity;
the converter's return code alone is not enough to establish what FreeCAD
will import. Keep this requirement tied to the audited implementation and
recheck it if FreeCAD changes its subprocess wrapper.

**FreeCAD launch/discovery contract:** the current Draft source's
`get_libredwg_converter("dwg2dxf")` checks the `TeighaFileConverter`
preference first (deriving the sibling `dwg2dxf` name when the configured path
names its companion executable), then searches the FreeCAD process `PATH`;
`DWGConversion=1` selects the LibreDWG lane without ODA/QCAD fallback. Verify
these against each pinned FreeCAD revision. FreeCAD resolves the converter
from its own process environment, not from a terminal or build-tree setting
considered in isolation. Each tested setup must identify both (a) how the
installed `dwg2dxf` is made discoverable—`TeighaFileConverter` preference or
PATH—and (b) how that FreeCAD process was launched. If documenting desktop-launched use,
verify the actual supported launcher environment (Finder/LaunchServices on
macOS, the selected desktop entry on Linux, or Start menu/app launcher on
Windows); a harness that injects PATH directly into FreeCAD qualifies only
that controlled route. If an OS launcher does not inherit the documented PATH,
either qualify an explicit-path setup in an isolated profile or narrow the
user-facing claim. The shared `TeighaFileConverter` preference can also affect
FreeCAD's separate export route, so any explicit-path recipe must retain the
import-only/export caveat under S8.9.4. Do not modify a user's real FreeCAD
profile, silently edit system PATH, or install into FreeCAD's private
directories.

**Converter data-mode invariant (source-checked 2026-09-24):**
[`dwg2dxf/dx_iface.cpp`](dwg2dxf/dx_iface.cpp)'s `dx_iface::fileImport()`
currently calls both `dxfRW::read(this, false)` and `dwgR::read(this, false)`.
Keep this unprojected/full-data mode for the FreeCAD converter path; do not
switch the adapter to `ext == true` as a shortcut for preserving the existing
2D LibreCAD behavior. LibreCAD's 2D adapter and the standalone converter are
separate consumers with separate contracts. For every changed 3D reader,
coordinate-frame, or DXF-writer path used by `dwg2dxf`, extend the fast
FreeCAD-argv CLI/readback control with independently expected entity fields
and verify them both in emitted DXF and public DXF readback. A FreeCAD runtime
check is additionally required only to claim that FreeCAD constructs the
expected geometry; if its importer drops or rejects a legal entity, retain
the converter output and record that downstream limitation instead of
projecting coordinates or rewriting the entity to a different DXF type.

Use this implementation-ready checklist for every profile we claim:

1. Install `dwg2dxf` outside the build tree and verify its runtime dependencies
   resolve there. FreeCAD must discover the platform artifact (`dwg2dxf` on
   Linux/macOS; `dwg2dxf.exe` on Windows) from the same process environment it
   will use in normal operation.
2. In an isolated FreeCAD profile, select its dedicated LibreDWG mode
   (`DWGConversion=1`) to prevent Automatic mode from silently succeeding via
   ODA/QCAD. Prefer PATH discovery when that PATH is actually inherited by the
   FreeCAD process. If a full-path preference is used, separately preserve or
   configure DWG export: FreeCAD shares `TeighaFileConverter` between import
   and export and may derive the sibling `dxf2dwg` path. This project does not
   provide that sibling.
3. Verify FreeCAD launches exactly
   `[installed-dwg2dxf, input.dwg, "-o", output.dxf]`; the CLI must be
   noninteractive, emit ASCII by default, preserve the supported source DXF
   version when no override is given, return nonzero on failure, and never
   leave a partial final output. The old positional CLI remains compatible.
4. Exercise FreeCAD's actual DWG `open` and `insert` entry points separately.
   Capture the executable path/hash, argv/status, fallback selection, DXF
   output path, and exact path handed to the selected DXF importer. First use
   a small independently bounded LINE control to qualify deployment and
   geometry handoff; then add only entity/version cases with their own
   independent semantic oracle under S8.15.
5. Keep these outcomes distinct: (a) installed executable discovery, (b)
   converter output/readback correctness, (c) FreeCAD construction of
   independently expected geometry, and (d) normal desktop dispatcher
   integration. Passing a prior gate does not imply a later one. Pin the
   FreeCAD revision, OS/architecture, launch route, importer mode, preference
   values, operation, source DWG version, and tested entity semantics.

**Current completion and remaining gap:** the exact `-o` CLI contract,
install-prefix check, and no-prompt/failure-safe publication are implemented.
On macOS arm64, FreeCAD 1.1.3 has pinned headless `open`/`insert` checks and a
desktop-process dispatcher check for bounded LINE controls. S8.9.8 also
proves both headless operations using a byte-identical installed executable
and the generated nonzero-Z LINE control; selected entity-specific routes are
recorded under S8.15, including explicit downstream-unsupported outcomes.
This is a bounded macOS FreeCAD
import-side integration result; it is not a promise that the library is linked
into FreeCAD, that all FreeCAD features are supported, or that DWG export is
provided. Native Linux and Windows installation, dependency resolution,
discovery, and FreeCAD-process checks remain platform qualification work.
Finder/Start-menu environment inheritance, FreeCAD viewport rendering,
bidirectional export, and blanket DWG/3D support are not established. Platform
gaps must not block independent parser/converter work, and unavailable FreeCAD
runtimes stay external gates rather than inferred passes.

**Native-platform implementation-ready sub-plan (remaining S8.9.5):** keep the
already-qualified macOS results closed and qualify Linux and Windows only on
native hosts with the actual FreeCAD release and importer named for each
claim. Execute the same short sequence on each platform; do not repeat the
entity matrix or full repository test suite:

1. Build and install `dwg2dxf` into a fresh prefix outside the build tree,
   including a prefix and source path with spaces. Test the static artifact;
   test shared mode only if that mode is shipped or advertised. Require the
   installed platform name (`dwg2dxf` or `dwg2dxf.exe`), not a build-tree
   executable.
2. Prove runtime closure in a clean process environment. On Linux inspect ELF
   dependencies and run without `LD_LIBRARY_PATH`; on Windows inspect and
   provide the build's required runtime DLLs in the normal install layout,
   without borrowing DLLs from FreeCAD's private directories. On either
   platform, report the resolved dependency paths and executable hash. Do not
   add bundled runtimes or FreeCAD-private deployment without a separately
   justified packaging change.
3. Run the existing fast installed-CLI contract and public DXF readback first
   against a tracked repository input or a locally authored-from-scratch
   control. Include exact FreeCAD argv, spaces in paths, source-version and
   entity-field assertions, and failure-with-no-partial-output. Keep generated
   DWG/DXF products in build/temp; do not add external samples as fixtures.
4. In a fresh isolated FreeCAD profile, disable fallback for attribution and
   qualify the supported discovery route (PATH and configured executable path
   only where each is intended to be documented). Run `open()` and `insert()`
   separately, asserting actual child path/hash, exact argv, exit status,
   output validity, importer implementation/settings, and exact output-to-
   importer handoff. Use the bounded nonzero-Z LINE control as the deployment
   smoke test; entity-specific geometry requires its own S8.15 oracle. Do not
   use Automatic-mode success to hide a failed `dwg2dxf` route.
5. Qualify one normal desktop launch route per claim (the chosen Linux
   desktop entry or Windows app launcher/Start-menu route) only after the
   headless checks pass. If that route cannot provide PATH reliably, test and
   document the isolated configured-path route instead. A controlled shell
   launch proves only that shell environment; it does not qualify desktop
   environment inheritance. Windows additionally needs native `.exe` lookup,
   paths containing spaces, and a Unicode/non-ASCII path case before any such
   path claim is made.

The fast CLI/readback gate is the per-code-change check. The `open()` and
`insert()` FreeCAD process runs are one-time platform qualification or are
rerun only when installation, discovery, subprocess, or importer-handoff
behavior changes. Desktop dispatcher work is likewise limited to a new
platform/launch route. If a native host is unavailable, leave its row
`BLOCKED_EXTERNAL_PLATFORM`, state the missing host/evidence, and continue all
independent ready format work.

For `3DSOLID`/`REGION`/`BODY`, distinguish converter support from FreeCAD
geometry support. A positive converter result means the DWG reader selected a
bounded, format-qualified payload and `dwg2dxf` emitted the legal DXF modeler
carrier without substituting a proxy or 2D primitive; it does not mean FreeCAD
constructed a B-rep. The AC1015 SAT-v1 slice (S8.15.16) is deliberately
narrow: decode only its bounded version-1 SAT blocks, write the DXF text
carrier in the representation expected by independent converters, and run
FreeCAD's exact `dwg2dxf <input> -o <output>` command form. Preserve the
unknown DWG bit as unknown and do not claim it round-trips through DXF; do not
infer wireframe, solid topology, or ACIS semantics from the SAT text. The
pinned FreeCAD 1.1.3 C++ importer already has a negative `SOLID` result in
S8.15.12 (record recognized, reported unsupported, no shape), so this SAT
converter result must remain converter-level evidence until a named FreeCAD
revision/importer constructs independently expected geometry.

**Modeler payload fast path (S8.15.16):** keep the ordinary fast gate in
`libdxfrw_dwg_local_roundtrip` and `dwg2dxf_freecad_cli_compat`; use an
in-memory generated AC1015 object-frame control to verify parsing,
group-1/3 serialization, and public DXF readback without a FreeCAD runtime or
downloaded drawing. The opt-in external test may take a caller-supplied
known-good DWG and independent DXF reference, run the exact FreeCAD argv, and
compare only the `3DSOLID` SAT group-1/3 values. Do not vendor those external
inputs or make the test a default dependency. A full-DWG witness authored
locally from scratch is useful only if an existing independent writer accepts
the locally authored SAT carrier without building a new ACIS/DWG encoder; it
is not a prerequisite and must not delay FreeCAD integration. S8.15.17 below
reuses the existing isolated FreeCAD harness for the real process-to-importer
handoff. Fast format/readback checks stay per-change; full repository suites
are reserved for broader milestones, and FreeCAD/independent-converter checks
are opt-in.

**FreeCAD-directed modeler-carrier sub-plan (S8.15.17):** make the already
implemented SAT-v1 conversion usable and correctly attributed when launched
by FreeCAD Draft, without implying that libdxfrw implements FreeCAD's ACIS
geometry kernel or changes FreeCAD's importer.

1. Reuse S8.9.2/.3's isolated FreeCAD profile and the existing converter
instrumentation; do not add a second runtime framework. Run a caller-supplied
AC1015 SAT-v1 DWG/reference-DXF pair through the installed `dwg2dxf`, with
FreeCAD's LibreDWG-only selection (`DWGConversion=1`) and the pinned C++ DXF
importer. Keep both source files outside the repository and use only
build/temp copies whose paths include spaces. The test is disabled by
default and requires no fixture download or external tool in normal CI.
2. For `Draft.importDWG.open()` and `Draft.importDWG.insert()` as separate
cases, assert the resolved installed executable path/hash, the single exact
argv `[dwg2dxf, input, "-o", output]`, zero process status, complete ASCII
output, retained source `$ACADVER`, and identical output path handed to
`importDXF.open()`/`importDXF.insert()`. Reuse the existing 30-code/value
SAT-carrier reference comparator; also require libdxfrw public DXF readback
to retain the same complete group-1/3 payload. The FreeCAD macro must emit an
explicit result marker because a FreeCAD command wrapper may exit zero after
a script exception.
3. Record the importer outcome independently: entity count/type, unsupported
messages/exceptions, created object/shape counts, and any geometry properties
available. A correct converter handoff can pass even if the pinned importer
does not construct a shape; that result remains **converter integration
only** and is explicitly downstream-unsupported. Do not infer 3DSOLID
behavior from S8.15.12's distinct `SOLID` result. Any future geometry-support
promotion requires an independently specified ACIS/B-rep oracle and matching
valid solid semantics for the named FreeCAD revision and importer mode.
4. First run the cheaper headless `open()`/`insert()` checks. Reuse S8.9.6's
registered desktop dispatcher only when claiming the FreeCAD desktop route;
do not rerun GUI integration for each parser/entity change. A runtime or
platform unavailable on this host is `BLOCKED_EXTERNAL_RUNTIME` or
`BLOCKED_EXTERNAL_PLATFORM`, not a reason to invent an importer result or
block independent code slices. Never rewrite a valid DXF carrier as a proxy,
mesh, or collection of primitives to mask a downstream importer gap.

**S8.15.17 completion:** the pinned FreeCAD process selects this installed
converter and hands the verified DXF to both Draft entry points, with
converter and importer results reported separately. This can close the
FreeCAD converter-integration row even when geometry remains unsupported; it
cannot promote ACIS/B-rep support without the independent semantic gate above.

To reproduce the optional external CLI check when both evidence files are
available locally, configure with
`-DLIBDXFRW_ENABLE_EXTERNAL_SAT_V1_CLI_CONTROL=ON`,
`-DLIBDXFRW_SAT_V1_DWG=<AC1015-DWG>`, and
`-DLIBDXFRW_SAT_V1_REFERENCE_DXF=<independent-DXF>`, build
`dwg2dxf libdxfrw_modeler_sat_dxf_compare`, then run
`ctest --test-dir build -R '^dwg2dxf_freecad_external_sat_v1_cli$' --output-on-failure`.
The default remains OFF, and only generated copies under the build tree are
used by the test.

To run the pinned FreeCAD consumer check as well, keep the external SAT-v1
paths and CLI option above, then additionally configure
`-DLIBDXFRW_ENABLE_FREECAD_SAT_V1_RUNTIME_CONTROL=ON`,
`-DLIBDXFRW_FREECADCMD_EXECUTABLE=<FreeCAD-freecadcmd>`, and
`-DLIBDXFRW_FREECAD_DWG2DXF_EXECUTABLE=<installed-libdxfrw-dwg2dxf>`.
Run
`ctest --test-dir build -R '^dwg2dxf_freecad_sat_v1_(open|insert)$' --output-on-failure`.
These tests run FreeCAD's C++ importer on the host, remain opt-in, and isolate
profiles and generated files beneath a unique system-temp directory.

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
3. **FreeCAD's DWG conversion/import paths:** support the concrete
   `Draft.importDWG.open()` and `Draft.importDWG.insert()` workflows as
   separate consumer entry points and three separate contracts:
   install/discovery and `dwg2dxf` argv/output publication, correct DXF entity
   and field serialization by the converter, and the selected FreeCAD DXF
   importer's construction of expected geometry. Verify both FreeCAD's
   configured executable-path route and PATH discovery against this installed
   `dwg2dxf`, following the actual LibreDWG converter-selection code in the
   tested FreeCAD revision and accounting for platform executable names and
   same-named tools. Current FreeCAD `main` has a dedicated LibreDWG lane:
   its configured converter preference or PATH resolves `dwg2dxf` on
   Linux/macOS and `dwg2dxf.exe` on Windows. Use its LibreDWG-only preference
   for positive attribution; automatic mode may continue to ODA/QCAD.
   The converter must preserve legal DXF entity types and 3D coordinates; it
   must not rewrite a rejected entity to a different 2D type merely to
   suppress FreeCAD's unsupported-feature report.
   Distinguish a converter merely found on PATH/preferences from one actually
   invoked; record every attempted argv, exit status, candidate output and the
   exact DXF path handed to the corresponding `importDXF.open()` or
   `importDXF.insert()` call. FreeCAD 1.1.3 currently treats
   an output file's existence as conversion success, so a nonzero converter
   exit and a later ODA/QCAD fallback must be visible rather than attributed
   to `dwg2dxf`.
   Require at least one nonzero-Z geometry witness to pass each claimed
   `open()` or `insert()` DWG→`dwg2dxf`→DXF→FreeCAD route before claiming 3D
   consumer coverage for that entry point; an `open()` result does not qualify
   `insert()`.
   Pin FreeCAD release/revision, OS/architecture, importer implementation and
   settings for each runtime result. This targets FreeCAD's existing external
   converter workflow for opening and inserting a DWG, not a direct libdxfrw
   link, GUI feature, or replacement converter integration. It does not
   qualify FreeCAD's separate DXF-to-DWG export path or imply that FreeCAD is
   bundled with libdxfrw. The default lane must remain dependency-free; fast
   CLI/readback controls run without FreeCAD, while the pinned runtime macro is
   an explicit integration check. This does not claim that libdxfrw changes
   FreeCAD's own importer. This repository supplies the import-side
   `dwg2dxf` program only, not the paired `dxf2dwg` program used by FreeCAD's
   reverse export lane; FreeCAD's shared converter preference may derive the
   sibling name when the opposite direction is requested.

   Current pinned-runtime baseline: `open()` has a locally generated AC1015
   nonzero-Z LINE witness (S8.14), and `insert()` has an independent
   locally-generated AC1015 nonzero-Z LINE witness (S8.15.15). Both exercise
   FreeCAD 1.1.3's C++ importer on macOS arm64 through PATH-resolved
   `dwg2dxf`; the `insert()` run additionally asserts the exact converter
   binary hash, input/output paths containing spaces, same-output handoff,
   target-document identity, one resulting edge, and XYZ bounds. These are
   narrow headless consumer results, not desktop-GUI, target-authored DWG,
   other-platform, or general 3D-family claims.
   Separately, S8.9.6 qualifies the registered `open`/`insert` dispatcher in
   the actual FreeCAD desktop process for tracked planar AC1027 LINEs via the
   configured converter path. It does not qualify menu/file-dialog interaction,
   viewport display, or 3D entity semantics in that desktop profile.

   **FreeCAD deployment boundary:** FreeCAD runs `dwg2dxf` as a separate
   process; this plan does not link libdxfrw into or bundle it with FreeCAD.
   The installed executable must be discoverable from FreeCAD's own process
   environment, not merely from a developer shell. Prefer PATH discovery when
   that PATH is inherited by FreeCAD, or use its configured converter path
   with the shared-preference/export caveat above. The platform names are
   `dwg2dxf` on Linux/macOS and `dwg2dxf.exe` on Windows. Record the selected
   DXF importer and settings: FreeCAD's C++ and legacy Python importers do not
   promise identical feature coverage. Converter field preservation is not by
   itself evidence of imported geometry. The existing `freecadcmd` checks do
   not prove that the installed desktop application's process environment,
   normal DWG-open registration, or GUI `insert()` route works. Any claim that
   users can use this converter from the FreeCAD desktop application must also
   pass S8.9.6 on each claimed platform. That check asserts document geometry
   and converter attribution only; it does not qualify viewport rendering or
   the GUI's visual display.

   **FreeCAD release-readiness rule (reviewed 2026-09-24):** the deliverable
   for this consumer is the installable `dwg2dxf` command-line program selected
   by FreeCAD Draft, not a FreeCAD plugin or an in-process libdxfrw link. The
   current `Draft/importDWG.py` resolver checks the shared converter-path
   preference before PATH, derives the opposite-direction sibling name when
   that preference names one LibreDWG program, and on Windows looks for the
   `.exe` filename. Its DWG import call supplies only
   `[dwg2dxf, input.dwg, "-o", output.dxf]`; it does not request overwrite or
   binary DXF. FreeCAD's Automatic mode can continue to ODA/QCAD, so tests and
   any attribution claim must select the LibreDWG-only mode and capture the
   executable actually invoked. In the source-reviewed resolver, mode
   `DWGConversion=1` selects LibreDWG only; Automatic mode (`0`) can continue
   through ODA and QCAD after a failed or missing earlier converter. The
   current `convertToDxf()` calls `Popen()` with an argument vector, waits for
   completion, ignores the process return code, and treats output-path
   existence as success before passing that path to `importDXF.open()` or
   `importDXF.insert()`. Therefore `dwg2dxf` must publish complete output
   transactionally, leave no partial final DXF on failure, and have tests
   assert process status, selected executable, and exact output-to-importer
   handoff. These observations are source-reviewed against [FreeCAD's DWG importer](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py)
   and [Import/Export preferences](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Import_Export_Preferences.md);
   `main` and the documentation are mutable, so runtime claims remain pinned
   to the exact FreeCAD revision and profile.

   Track four non-substitutable FreeCAD readiness results for every claimed
   platform/profile: (a) the installed executable is discoverable and starts
   outside the build tree; (b) the exact FreeCAD argv converts to a valid DXF
   with the required fields preserved; (c) FreeCAD's selected DXF importer
   constructs the independently expected geometry for the specific entity
   and version; and (d) the actual desktop dispatcher works if desktop use is
   claimed. A pass at (a) or (b) must never be described as FreeCAD geometry
   support. Record open and insert separately, and pin OS/architecture, FreeCAD
   revision, C++ versus legacy Python importer, preferences, converter path
   and hash, and expected geometry. Current evidence establishes a narrow
   FreeCAD 1.1.3/macOS arm64 command and desktop route plus selected entity
   rows; it does not establish Windows/Linux packaging or blanket 3D support.
   Continue using S8.9.1–S8.9.6 for implementation and S8.15 for entity-level
   qualification; those existing work items are the executable backlog, not
   new duplicate tasks.

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
- For a positive FreeCAD converter/import assertion, require one successful
  `dwg2dxf <input> -o <output>` process and prove that the same output path is
  passed to the selected DXF importer entry point (`open` or `insert`) used by
  the test. Also prove FreeCAD resolved the
  intended installed executable (by absolute path and, where useful, hash),
  with the converter preference configured to the LibreDWG-compatible lane;
  FreeCAD's UI label is not proof of the executable's implementation. Disable
  fallback converters in that assertion so a successful ODA/QCAD import cannot
  qualify libdxfrw output; keep a separate fallback-enabled audit for real
  configured-workflow diagnosis and report its actual producer.

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
| `3DFACE` | DXF library read/write routes exist. S5.1 found the in-tree `dx_iface` did not dispatch `E3DFACE` on output; it now does. The reader requires XY components for the first three corners, accepts the fourth corner as optional and copies corner 3 when absent. The DWG parser bounds its invisible-edge flags to `0x0f`; the DXF writer now rejects group-70 values outside `0..15` before the 16-bit write. | Generated ASCII/binary DXF vectors check WCS corners and group-70 invisible-edge bits. S8.15.18 additionally runs a locally authored tilted control through ODA-generated AC1015 DWG, exact FreeCAD-form conversion, and public DXF readback; all four WCS corners and the invisible-edge flag survive. FreeCAD 1.1.3 C++ counts one entity but reports it unsupported and creates no shape. Autodesk defines the four corner points as WCS and group 70's optional per-edge invisibility flags in its [3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm). This is one generated AC1015 converter witness only; it does not qualify target-authored DWG layout/version support or FreeCAD geometry support. |
| `3DLINE` | Typed DXF/DWG codec routes exist; `dx_iface` now stores the modern `DRW_3DLine` subtype and dispatches its typed DXF writer (S8.13.4). | Locally generated ASCII and binary DXF exact-CLI/two-pass readback vectors verify adapter subtype and field preservation; S8.13.4.1 verifies pre-AC1015 output fails closed. Autodesk's current DXF ENTITIES index lists `LINE` but not `3DLINE`; no authoritative Autodesk definition or target-produced DWG witness was found in the 2026-09-26 check. LibreDWG's manual documents start/end/extrusion/thickness fields, but this is an implementation cross-check only. Keep the DXF spelling as a portability-sensitive extension, do not substitute it for standard WCS `LINE`, and keep pre-R13 legacy 3DLINE on its separate evidence lane. |
| 3D point/line families | `POINT`, `LINE`, `RAY`, and `XLINE` carry WCS 3D data; legacy pre-R13 and modern custom-class 3DLINE remain distinct. | S8.4a.8 matches one AutoCAD-authored POINT `(50,50,50)` against LibreDWG for each exact AC1014/1015/1018/1021/1024/1027 sample; five also match their hash-pinned paired source DXF. Only the AC1027 and local AC1015 POINT controls have separate pinned-FreeCAD geometry checks (S8.15.3/.3.1). S8.13.4 and S8.13.4.2 verify generated ASCII and binary DXF adapter retention for the modern `3DLINE` extension only. Do not equate a 3D `LINE` with implementation-specific `3DLINE`, or promote DWG/version/target-CAD portability from a codec self-round-trip. |
| Planar entities placed in 3D | ARC/CIRCLE, SOLID/TRACE, 2D POLYLINE/LWPOLYLINE, HATCH/MPOLYGON, and INSERT use distinct coordinate and topology rules. DXF ELLIPSE center and major-axis vector are WCS, with extrusion providing its plane normal. INSERT adds scale, rotation, array spacing, and block-base transforms. | INSERT's nested placement matrix is covered in S5.2. S5.1 covers SOLID/TRACE corner fields and TRACE projection; S5.4 corrects the DXF ELLIPSE `ext=true` double-transform and checks WCS invariance in both modes. S5.7 validates the HATCH/MPOLYGON zero-XY elevation header, OCS retention, and nonzero extrusion; S5.8 rejects partial X/Y pairs for polyline, line, arc, ellipse, and spline boundary data plus seed points; S5.9 preserves HATCH elliptic-edge direction; S5.10 disambiguates MPOLYGON's top-level group-73 annotation flag from boundary-path code 73 and retains its group-11/21 vector. This does not establish fill/render behavior, all optional tuples, DWG semantics, or FreeCAD MPOLYGON geometry support. Broader ARC/CIRCLE OCS, thickness, and DWG ELLIPSE qualification remain open. Continue to use Autodesk's arbitrary-axis and per-entity rules, not a transform helper round-trip alone. |
| Classic `POLYLINE` 3D forms | The model stores 3D polylines, polygon meshes, and polyfaces in the `POLYLINE`/`VERTEX`/`SEQEND` family. DWG dispatch has separate vertex and face types, owned-child handling, and subtype checks. S5.1 corrected DXF polyface output to include groups 71/72 for declared vertex/face counts, use the polyface/face-record subclass markers, omit vertex group 91 from polyface records, and emit the legal SEQEND subclass set. S5.5 closes the DWG→DXF PFACE face gap: DWG face subtype (whose layout has no flags field) now supplies DXF face-record bit 128 and signed index serialization. | Generated ASCII/binary vectors verify WCS 3D-polyline points and signed polyface indices; LibreDWG 0.14 independently reads both. A locally generated AC1015 control exercises this conversion path only; it is not target interoperability evidence. Keep versioned DWG ownership/count qualification separate. DXF readers must remain tolerant of legal child ordering; writers emit coordinate vertices before faces. |
| `MESH` / `AcDbSubDMesh` | Typed vertex/face/edge/crease/property-override data, topology validation, DXF and DWG encode/decode paths, and generated local round-trip tests exist. S5.1 added the missing in-tree `dx_iface` MESH read callback and write dispatch. | Generated ASCII/binary DXF vectors compare typed vertices/faces/edges/creases; LibreDWG 0.14 independently reads both. Self-round-trips remain consistency checks, not DWG-layout evidence. Autodesk's DXF table is useful for DXF group codes; the searchable ODA v5.4.1 text reviewed here has no named `AcDbSubDMesh` DWG layout. Keep DWG MESH layout/version claims unqualified until primary DWG evidence or a target-produced, independently checked witness exists. |
| Six analytic/NURBS surface classes | Typed DXF paths exist. The DWG surface parser retains a bounded raw ACIS body and links DataStorage; DWG surface encoding rejects versions before AC1021. Class registration and modern DWG read/write paths exist. | The searchable ODA v5.4.1 text reviewed here has no named modern `AcDb*Surface` layouts. Keep DWG surface layout/version claims unqualified until feature-specific primary evidence or target-produced, independently checked witnesses exist. Keep the AC1021+ writer gate meanwhile; check each typed field, handle, transform, and ACIS carrier separately. Do not imply surface evaluation. |
| `3DSOLID` / `REGION` / `BODY` and ACIS | DXF R2000–R2010 stores SAT groups 1/3 on the entity; R2013+ may place SAB in `ACDSDATA`. S2.2 routes DXF text carriers through groups 1/3 for AC1015/1018/1021/1024 and rejects binary, mixed, DWG-frame, and unassociated AC1027+ inline payloads rather than guessing. DWG parsing has a deliberately narrow AC1015 version-1 SAT block extractor for `3DSOLID`; each declared block is bounded to the object body, extraction requires the zero-size terminator, and the ODA character transform is reversed before exposing SAT text. The opt-in S8.15.16/S8.15.17 checks verify one external AC1015 carrier against an independent DXF reference and FreeCAD's installed-converter handoff through both `open()` and `insert()`. Generic raw-DXF-section preservation remains independent. S3.2.2 proves one DWG entity-to-DataStorage association. A fresh ODA 27.1.0.0 AC1027 pair plus Open CAD Studio v2026.38 establishes one bounded standalone `3DSOLID`→`ASM_Data` SAB carrier and equal nonempty source DWG/DXF mesh results. The S2.3.4 serializer now emits this narrow carrier and its referenced proxy/material closure; Open CAD Studio reads one output solid, its linked history, and the exact SAB bytes. ODA's official macOS GUI accepted the exact libdxfrw DXF and produced an AC1032 DWG, but the only completed return conversion was R14 ASCII DXF: it retained a `3DSOLID` while replacing ACDSDATA/SAB with inline SAT, so the required version-matched semantic acceptance remains pending (S2.3.6). CAD Assistant v1.6.0 ARM64 returned generic transfer errors for the DXF and extracted SAB and supplied no semantic result. This does not qualify material appearance/property fidelity: current `DRW_Material` DXF writing emits only name/description. The dispatcher keeps complete DWG frame bytes separate from qualified ACIS payload; AC1027+ DataStorage linking is separate. `decodeWireframe()` does not reinterpret SAT text as topology. `DRW_ModelerGeometry` has no DWG encoder, and `dwgRW` has no typed modeler-geometry write route. | The narrow AC1015 `3DSOLID` SAT-v1 converter integration is verified for one external source/reference and pinned FreeCAD 1.1.3 macOS arm64 C++ importer profile. FreeCAD reports the entity unsupported and creates no shape; this is not B-rep/geometry support. Do not generalize to other AC1015 modeler entities, AutoCAD-authored inputs, AC1024 SAB-to-DXF, other AC1027+ entities/producers, other versions/platforms/importers, material property preservation, or DWG modeler writing. Keep frame bytes, decoded inline ACIS text, ACDSDATA/AcDsPrototype bytes, proxy bytes, and material semantics distinct. |
| NURBS/spline curve path | Local ODA v5.4.1 §20.4.40 specifies R2013+ `Spline flags 1` as BL and the subsequent `Rational`, `Closed`, and `Periodic` values as individual B fields in scenario 1. The current `parseDwgSplineBody()` reads BL for `splFlag1`/`knotParam` only when `version > AC1024`, matching that stated version boundary; the one-bit fields are read separately. The initial suspected width defect is not supported by the cited ODA text. Scenarios 1 and 2 do not store DXF planarity metadata; S8.15.7 replaces the prior unconditional planar bit with conservative geometry-derived metadata and verifies a generated AC1015 control/fit pair through `dwg2dxf` and FreeCAD, including nonplanarity smaller than the declared spline tolerance. No authentic AC1027/AC1032 spline witness has been identified. | Do not change the DWG flag width based on the stale issue note. S8.15.7 qualifies only its exact generated AC1015 converter/importer route and four cubic shapes; keep other DWG spline versions/forms unqualified until authentic target samples/field traces verify the parse. Do not infer AC1032 from the AC1027 pass-through reader. |
| Semantic comparison | S1.1 added field-level serializers for MESH, modeler geometry, and surfaces; binary values and raw carriers remain explicitly separate digest/opaque fields. | Field-level serializer output improves diagnostics and mutation sensitivity, but still compares adapter observations rather than independently proving format semantics. S7/S8 witnesses remain necessary for support claims. |
| Unknown DXF sections | `dxfRW::processRawDxfSection()` captures unrecognized sections as `DRW_RawDxfSection`; the writer can re-emit supplied raw sections. | This is a useful opaque-preservation route for `ACDSDATA`, but the 3D plan must test the complete read/callback/consumer/write chain and must not call it a typed ACIS link. |
| DXF ACIS version routing | S2.2 writes SAT text chunks as groups 1/3 through AC1024, records/validates DXF chunk identity, and refuses unassociated AC1027+ inline bytes. S2.3 implements only the witnessed AC1027 `AcDb3DSolid_ASM_Data` / `ASM_Data` ACDSDATA shape, exact owner-key remap, and evidenced history/material closure. Open CAD Studio v2026.38 independently reads the emitted output and the exact SAB payload; ODA 27.1.0.0 acceptance of libdxfrw output remains untested because its GUI/CLI cannot be run while this macOS session is locked. The surface writer applies the same SAT gate while keeping subtype-specific binary group-310 data distinct. | Autodesk's AutoCAD 2010/current DXF references list proprietary payload groups 1/3; ezdxf documents SAT inline through R2010 and SAB in ACDSDATA from R2013+. LibreDWG 0.14 `dxf2dwg` accepted locally generated AC1015/1018/1021/1024 SAT vectors, but its DXF implementation cannot qualify ACDSDATA. Keep all other AC1027+ SAB-to-entity relationships opaque. |
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
| WCS primitives | POINT, LINE with nonzero Z, RAY, XLINE, 3DFACE | WCS endpoints/corners, optional values, edge flags, finite values | One AutoCAD-authored POINT `(50,50,50)` per exact AC1014/1015/1018/1021/1024/1027 sample matches LibreDWG field-by-field; five paired DXFs also match. Only AC1015 generated and AC1027 target-authored POINTs have pinned-FreeCAD geometry checks. LINE has its own generated AC1015 tuple. S8.15.18 preserves one tilted 3DFACE field set through ODA-generated AC1015 DWG conversion/readback, but FreeCAD 1.1.3 C++ reports that record unsupported and creates no shape. Other entities, versions, platforms, and family-wide support remain unqualified. |
| 3DLINE variants | Pre-R13 legacy 3DLINE; modern custom DWG class and DXF `3DLINE` spelling | Keep legacy type 21 separate from modern class identity; determine DXF portability and supported versions from witnesses | Typed paths exist; extension/version support unqualified |
| Planar geometry in 3D | ARC, CIRCLE, ELLIPSE, SOLID, TRACE, 2D POLYLINE, LWPOLYLINE, HATCH, MPOLYGON | OCS arbitrary-axis frame, elevation, extrusion, thickness, angle direction, vertex order, MPOLYGON annotation and offset-vector fields | HATCH/MPOLYGON have bounded header elevation/OCS retention (S5.7), paired coordinate completeness (S5.8), and distinct path/top-level group-73 handling plus MPOLYGON vector retention (S5.10). Broader edge geometry, rendering, omitted optional tuples, DWG semantics, and FreeCAD MPOLYGON geometry remain unqualified. |
| Placed block geometry | INSERT/MINSERT and block contents | OCS insertion point, block base point, nested transform composition, nonuniform/mirrored scales, rows/columns/spacings, attributes/ownership | One INSERT transform is positive in the pinned FreeCAD route. S8.15.11 verifies MINSERT array-field conversion/readback; FreeCAD 1.1.3 C++ imports only its first cell, so array geometry remains downstream-unsupported. Other transform/ownership combinations remain unqualified. |
| Classic 3D topology | 3D POLYLINE, polygon mesh, polyface, VERTEX, SEQEND | WCS vertices, flags, closure, M/N order, signed one-based face indices, edge visibility, child ordering, counts and handles | Typed routes exist; independent qualification incomplete |
| Subdivision topology | MESH / AcDbSubDMesh | Base-cage vertices, flat face-list counts, n-gons, edges, crease values, property overrides, version gate | Typed routes and generated tests exist; oracle/version checks incomplete |
| Curve/surface geometry | SPLINE, HELIX, plane/extruded/revolved/swept/lofted/NURBS surfaces | Degrees, knots, weights, control point order, closure/periodicity where represented, sweep/profile handles, matrices, flags, version gates | Partial typed routes; one generated AC1015 weighted 3D HELIX converter path now guards rational/planar flags and metadata. Current FreeCAD `main` C++ `ReadEntity()` dispatch lacks HELIX and routes it to `ReadUnknownEntity()`; this is downstream-unsupported, not grounds to alter converter output. Target-authored DWG and other HELIX semantics remain unqualified. |
| ACIS modeler geometry | 3DSOLID, REGION, BODY; SAT and SAB carriers | Entity-inline payload vs DXF ACDSDATA vs DWG DataStorage; frame vs payload separation; empty/history state; preservation versus semantic decode | AC1015 SAT-v1 has one bounded converter route; AC1021/AC1024 SAB-to-DXF remains blocked; one AC1027 standalone 3DSOLID/AcdsData tuple now emits with a witnessed history-proxy/material closure and exact SAB bytes, independently read by Open CAD Studio. ODA acceptance of libdxfrw output, material appearance fidelity, FreeCAD handoff, other tuples, and general support remain unqualified (S2.3). No ACIS semantic decode or DWG modeler writer. |
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
S2.2.1 adds a runtime-generated, in-memory regression for the existing
generic `ACDSDATA` raw-section callback and write/read replay boundary. The
deliberately orphaned ACDS-shaped tags stay opaque and produce no typed
modeler entity; this is not validation of a legal ACDSDATA schema, SAB
payload, entity association, FreeCAD import, or DWG-DataStorage-to-DXF
serializer. S3.2.2 proves one bounded DWG
entity-to-DataStorage association; S2.3 is the next planned bridge from that
typed callback to an R2013+ DXF `ACDSDATA` section. No general association or
DXF writer support claim is implied, and this test does not unblock S2.3.
S2.2.2 separately preserves observed AC1027 modeler-envelope fields through
DXF ASCII/binary round trips and remaps the known soft-owner history handle;
it is entity-shell preservation only and does not qualify DWG-to-DXF SAB or
ACDSDATA emission for FreeCAD. S2.2.3 now carries unknown DXF sections across
the concrete `dwg2dxf` adapter by retaining the generic section callback in
`dx_data` and supplying it to the export writer. This closes a DXF-input to
DXF-output data-loss gap only: the DWG reader's `addRawDwgSection` and typed
DataStorage paths are distinct and remain unchanged. A new external ACadSharp
AC1027 DXF control exposed a separate standalone-writer identity defect: the
CLI succeeds and retains the section, but mints new handles for its
`3DSOLID`/`REGION` entities while raw ACDS group-320 keys remain at their
source values. S2.2.4 narrowly remaps only the `ASM_Data` owner key when it
resolves uniquely to a modeler entity; unrelated ACDS records remain opaque.
This repairs an observed DXF→DXF association loss, not the blocked DWG
DataStorage-to-DXF serializer or FreeCAD geometry import.

Dependencies: S0, S1. This slice may edit public entity fields and DXF paths,
but must not edit `src/intern/dwgreader*` until the DWG spec gate is met.

Files: `src/drw_entities.h/.cpp`; `src/libdxfrw.{h,cpp}`;
`dwg2dxf/dx_data.h`; `dwg2dxf/dx_iface.{h,cpp}`;
`tests/dwg_local_roundtrip_tests.cpp`; `tests/CMakeLists.txt`;
`tests/run_dwg2dxf_raw_dxf_section_test.cmake`;
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
6. Carry parser-validated unknown DXF sections through the concrete CLI
   adapter without interpreting their schema or remapping arbitrary section
   keys. Generate the regression input at test runtime, run the actual
   `dwg2dxf <input> -o <output>` command twice, and assert that both the opaque
   section payload and an adjacent ordinary entity survive. Keep this DXF to
   DXF adapter gate separate from DWG `ACDSDATA`/DataStorage serialization and
   from FreeCAD's DWG-import geometry gate.
7. **S2.2.4 — Preserve `ASM_Data` identity across output-handle allocation.**
   The external ACadSharp AC1027 DXF sample proves that the `ACDSRECORD`
   `AcDbDs::ID` group-320 key equals its `3DSOLID`/`REGION` entity handle; its
   independent ACadSharp reader uses that key to attach SAB payloads. In a
   local exact-argv CLI round trip, libdxfrw changed the entity handles
   D65/D69/D6A to 14D3/14D4/14D5 while leaving those three section keys
   unchanged. For a parsed `ACDSRECORD` that contains both `AcDbDs::ID`, one
   group 320, and `ASM_Data`, resolve that source key against the uniquely
   written modeler-entity handle map and serialize the final emitted handle.
   Do not globally remap code 320, mutate raw sections in the callback, rewrite
   unrelated/unknown records, or guess on duplicate, malformed, ambiguous, or
   orphan associations; fail the output transaction when a recognized
   `ASM_Data` record cannot be joined safely. Runtime-generated `dx_iface`
   ASCII and binary controls compare the public-reader entity handle with the
   parsed ACDS key; the ASCII route is checked through a second conversion.
   The exact FreeCAD-form CLI test runs a generated AC1032 control twice and
   checks the same handle relation in both outputs, while an unrelated code-320
   record remains unchanged. Missing, duplicate, malformed, ambiguous-source,
   and orphan associations must fail without publishing the output. The
   attempted binary second pass currently fails while reading TABLES, before
   ACDSDATA is reached; it is recorded as a separate binary DXF reader/writer
   issue and is not a gate for this repair. No sample file is committed. This
   narrow identity repair is supported by independent reader evidence and the
   concrete stale-link reproduction; it does not qualify the ACDSDATA schema,
   fresh serialization, INSERT/XREF behavior, SAB semantics, ODA acceptance,
   or FreeCAD modeler geometry. Keep S2.3's carrier-emission gate separate.

Positive gate: each version's DXF carrier is accepted by an independent reader;
same-version SAT/text and raw ACDSDATA retain their bytes and section/entity
identity through the documented path. Cross-version
or cross-container output is compared semantically only where an independent
ACIS decoder can establish equivalence; otherwise report opaque preservation
and the representation change. Negative gate: a DWG frame, proxy-only,
truncated, ambiguous, or orphaned section is never emitted as entity ACIS.

### S2.3 — Bridge verified AC1027 DataStorage into an R2013+ DXF section for FreeCAD's `dwg2dxf`

State: S2.3.1 (observed-file inventory) and S2.3.3 (ODA acceptance-route
research) are `VERIFIED` as bounded results. S2.3.2 is now `READY_NARROW` for
one hash-pinned ODA-produced AC1027 standalone `3DSOLID`/`ASM_Data` carrier:
an independent reader reconstructed the exact SAB record and returned
identical nonempty mesh metrics for the paired DWG and DXF. S2.3.4 is
`IN_PROGRESS_NARROW`; no public support claim is promoted. Autodesk's
published `3DSOLID` reference and other producer shells still disagree, and
neither the DXF reference nor the ODA DWG specification defines a universal
ACDSDATA projection. The current primary
source check adds an unresolved handle semantic: Autodesk's general DXF rules
classify group 320–329 as arbitrary IDs that are not translated during INSERT
or XREF, while group 350–359 soft-owner IDs are translated. The community
BricsCAD witness calls group 320 a `3DSOLID` back-reference, but does not say
how that key is maintained when the drawing is rewritten. Therefore matching
group 320 to a changing output entity handle is not yet a justified remap
rule. A focused reread of the ODA specification's §24.2.2.7.1 clarifies a
separate DWG-container identity domain: the DataStorage schema-search table
associates data-segment object handles with sorted indexes and says AutoCAD
ignores the entity when that search data is absent. This does not define the
DXF `ACDSDATA` section or its group-320 key. Keep the DWG data-record handle /
index mapping distinct from the DXF ACDSRECORD-to-entity key; do not copy a
DWG DataStorage object handle or sorted index into the DXF group-320 field.
LibreDWG's own documentation marks ACDSRECORD/ACDSSCHEMA as unhandled
(without fields/DXF), and its AcDs reader notes describe incomplete,
heuristic extraction; this remains an independent implementation gap, not a
serializer oracle. The narrow ODA tuple can now proceed, but unresolved
required-vs-optional shell fields and key lifecycle continue to block every
broader claim. S2.3.6 additionally requires a complete libdxfrw-produced DXF
accepted and semantically retained by ODA. This slice is independent of
additional DWG parser edits; do not wait for generic ACIS decoding or a DWG
writer.

Dependencies: S0.6, S1.1, S2.1, S2.2, and S3.2.2. Research now covers the
exact observed sample facts, installed ODA acceptance route, the
ACadSharp-maintained multi-entity AC1027 sample, Autodesk's published
`3DSOLID` and group-code boundaries, and LibreDWG's published ACDS limitations.
The second sample improves the converter-failure vector but is not
target-provenance or normative evidence. Do not repeat broad searches. The
remaining high-value research is narrowly defined: acquire/hash-pin the
BricsCAD source DXF (if available) or obtain a primary-source answer for
group-320 lifecycle and universal entity/section fields. These are not
prerequisites for the already bounded ODA tuple, but remain prerequisites for
broader implementation claims. The external ODA round trip and FreeCAD runtime
are one-time checkpoints after a serializer slice is complete.

Purpose: make FreeCAD's installed `dwg2dxf` command able to finish a supported
AC1027/R2013 conversion containing the evidenced `3DSOLID`, without dropping
the solid, aborting publication, or corrupting neighboring entities. Match
FreeCAD's invocation contract exactly: `[binary, input, "-o", output]`, with
the complete output at the requested path and a zero exit status before its
normal DXF importer is called. The output must be complete parseable ASCII DXF
at the declared target version. Preserve the solid's SAB as opaque carrier
data only if the version-correct DXF section/entity mapping is established.
This is converter integration and carrier preservation, not solid geometry
support: the pinned FreeCAD importer has no verified `3DSOLID` construction
route. Do not advertise FreeCAD solid import from a successful conversion.
Keep AC1021/R2007 inline SAB blocked; this ACDSDATA route does not apply, and
no SAB-v2-to-SAT transformation is authorized by these observations.

Files likely involved: `dwg2dxf/dx_iface.{h,cpp}` to stage entity identity,
collect the late `addDataStorage()` callback delivered after entity/object
traversal, and join records by explicit handle/key rather than callback order;
`src/drw_datastorage.{h,cpp}` only if a narrowly typed DXF projection belongs
in the shared API; `src/libdxfrw.{h,cpp}` or its raw-section writer to emit the
DXF section; `tests/dwg_local_roundtrip_tests.cpp` or a focused generated
vector/CLI test, plus the opt-in FreeCAD CTest/macro. Prefer a CLI-local
serializer if shared API changes are unnecessary. Do not edit DWG parsers in
this slice.

Implementation sequence:

1. **S2.3.1 — Freeze observed mapping and evidence (`VERIFIED`).** The local
   ODA File Converter 27.1.0.0 ACAD2013 `Cone` witness has a 3DSOLID at handle
   `20B`, history handle `20E`, no inline groups 1/3, and one `ACDSDATA`
   `ACDSRECORD` keyed to `20B`: schema ID 1 / `AcDb3DSolid_ASM_Data`, record
   `ASM_Data`, 1980 bytes in 16 ordered group-310 chunks (15×127 bytes plus
   75 bytes). The section declares six schemas, IDs 0–5: `AcDb_Thumbnail_Schema`,
   `AcDb3DSolid_ASM_Data`, `TreatedAsObjectDataSchema`, `LegacySchema`,
   `IndexedPropertySchema`, and `HandleAttributeSchema`. There is no thumbnail
   data record in this witness. The record's ACDS object ID and record-name
   fields are also present and must not be confused with schema ID or handle.
   Reconstructed DWG DataStorage and DXF record payloads differ only at
   1-based positions 100 and 103, both in an embedded ACIS timestamp string;
   its exact update rule is unknown. This supports the candidate association,
   not a general byte-transform rule. ODA §24 is authority for the DWG
   DataStorage container only, not DXF ACDSDATA syntax.

   Evidence is hash-pinned and external-only. Original resaved AC1027 DWG
   SHA-256 `421b1db343254af497a67e15fb86e4a4aaf3e1ec0c5513ecc143bdc1084158fb`,
   ODA DXF SHA-256
   `aede3c38a0edbe6fafbb211c8056995e68243fbcccf2a547548695f842bd2a4c`, and
   LibreDWG 0.14 minJSON SHA-256
   `7149954b8a1740a3e114269d9056ff09567dd2878735dd58a30659b6a68adc31`.
   The ODA-produced DXF→AC1027 DWG→DXF acceptance control retained all six
   schemas and the one handle-`20B` record; its round-tripped DWG SHA-256 is
   `bfb27a8bb2a5941563d9176fdd19305426f2ef482bf2cf22d934fdf13e8def08`, and
   output DXF SHA-256 is
   `f2aecd617f7752446b8ad3deca2b4e566e88abdedd2ee52006d0aa920ac0e66a`.
   Four payload characters at 1-based positions 97, 100, 102, and 103 changed
   during the ODA rewrite; all are in that timestamp string. All files remain
   in temporary storage and are not repository fixtures. Provenance is one
   ODA-origin chain, not AutoCAD-authored interoperability evidence.

   Cross-sample evidence (external only): ACadSharp repository revision
   `3a52a52047e84670759c1f9ad3916ec3ce85ff56` contains
   `samples/sample_AC1027.dwg` (SHA-256
   `4c63508dd794d5cdfb56c60aded47928999b02619ea8c2ef435a4b74e60d24d6`)
   and `samples/sample_AC1027_ascii.dxf` (SHA-256
   `4f26752008643642c37f7bab410b23cfd8b6dd9ee3aaa19746b612ea956b61e6`).
   Its `samples/sample_base/save_samples.lsp` has an AutoCAD `SAVEAS 2013`
   generation step, and the DWG's SAB stamp names `Autodesk AutoCAD
   230.7.0.65535 NT`; record this conservatively as a repository-maintained
   sample with an AutoCAD recipe/stamp, not an independently audited
   AutoCAD-origin file. A pinned ezdxf 1.4.4 parse counts six schema
   definitions (IDs 0–5), four ACDSRECORDs total, and three `ASM_Data`
   records keyed to `3DSOLID` handles `D65` and `D6A` and `REGION` handle
   `D69`; the fourth record is `Thumbnail_Data`, keyed to `22`. The three
   modeler records declare byte lengths 7779, 7489, and 2876; their 62, 59,
   and 23 ordered group-310 chunks sum exactly to those lengths. All three
   modeler entities have group 290=1 and nonzero group-2 identifiers. An ODA
   File Converter 27.1.0.0 ACAD2013 DXF→DWG→DXF run on this non-ODA-origin
   sample pair is now available: input DXF SHA-256
   `4f26752008643642c37f7bab410b23cfd8b6dd9ee3aaa19746b612ea956b61e6`,
   output AC1027 DWG SHA-256
   `3babd08f2cbc78cb3d06da47f0f7b620668f3378f5805ce7f56db744bc6c62d0`, and
   output AC1027 ASCII DXF SHA-256
   `50ed64d7742296dbcd50349110f1ea1bdb6e9df0c5b31f924567fe9a1d248386`.
   ODA retained six schemas, all three owner-key/payload associations, every
   declared length and ordered chunk, and byte-identical SAB payloads. The
   owner handles remained D65/D69/D6A; the entity group-2 identifiers changed
   on ODA rewrite while the group-350 history handles remained D66/D6B. The
   LibreDWG 0.14 minJSON independently recognizes all three DWG entities with
   `has_ds_data=1` but exposes empty `acis_data`; this corroborates entity
   classification only. ezdxf 1.4.4 maps all three DXF records back to the
   corresponding entity handles but its SAB parser rejects each payload at
   the same `(0x14, (1.0, 0.0, 0.0))` token, so no geometry invariant or
   semantic-kernel result is established. The thumbnail record and shared
   record/schema identifiers show why association cannot be guessed from
   callback order or numeric ID alone. The ODA result is independent
   structural acceptance of this repository-maintained input—not an
   independently audited AutoCAD-origin pair, a handle-changing rewrite, a
   complete DXF contract, or acceptance of libdxfrw output. Both source and
   generated files remain outside this repository.
   Exact FreeCAD-form `dwg2dxf input -o output` on the ODA-generated DWG still
   exits 1 without publishing a DXF; FreeCAD therefore receives no importer
   handoff. This remains a converter-failure control, not a normative
   ACDSDATA contract or support claim.
2. **S2.3.2 — Resolve the R2013 DXF entity/section contract (`READY_NARROW`
   for one standalone ODA AC1027 `3DSOLID`/`ASM_Data` carrier; all broader
   contracts remain blocked).**
   Compare the bounded descriptions:
   Autodesk's published `3DSOLID` reference (group 70 modeler version 1 and
   groups 1/3 text); the ODA AC1027 entity shell (290=1, nonzero GUID, nonzero
   group-350 history handle, no 70 or 1/3); and the BricsCAD-authored shell
   published by the ezdxf maintainer (290=0, zero GUID, group 350=0, no 70 or
   1/3) plus ACDSRECORD group 320 back-reference; and the ACadSharp-maintained
   AC1027 pair (290=1, nonzero group-2 identifier, and keyed `ASM_Data`
   records for two solids and one region). Treat the cross-sample matches as
   corroboration only, not proof that these fields are normative or remain
   valid after insert/XREF/handle reassignment. Do not infer that group 290,
   GUID, or history has the same requiredness/value in all producers. Autodesk's
   `3DSOLID` reference documents only inline modeler version 1 and groups
   1/3; it does not define ACDSDATA. Autodesk's general group-code reference
   further distinguishes group 320–329 arbitrary IDs (not translated during
   INSERT/XREF) from group 350–359 soft-owner IDs (translated). Record this
   as a real unresolved compatibility constraint, not as proof that the
   witnessed group-320 relationship is false. The local ezdxf v1.4.4 source
   audit is complete but static-only; do not treat its self-tests as
   interoperability evidence. A source audit on 2026-09-24 found a useful
   independent-reader cross-check in ACadSharp PR [#1139](https://github.com/DomCR/ACadSharp/pull/1139),
   merged 2026-07-09: its R2013+ DXF reader associates `ACDSRECORD` data by
   the `AcDbDs::ID` subsection's group-320 owner handle and reads group-310
   chunks from `ASM_Data`; the PR reports 13 R2018 REGION payloads whose
   declared sizes match and describes a larger 306-file read corpus. This
   independently corroborates that bounded reader mapping, but it does not
   establish all required schema records, the `3DSOLID`/`REGION` entity-shell
   fields, identity behavior after cloning/remapping, or writer acceptance.
   ACadSharp PR [#1150](https://github.com/DomCR/ACadSharp/pull/1150) proposes
   the corresponding writer and remains open as of this review. Its author
   reports testing DXF outputs with DWG TrueView and an ODA-based reader, and
   identifies a version boundary for the `AcDb3dSolid` subclass/history field;
   this is a valuable implementation lead, not independently reproducible
   evidence until the exact patch/output and tool versions are available and
   rerun. The local ACadSharp checkout is pinned at
   `3a52a52047e84670759c1f9ad3916ec3ce85ff56`, predates those PRs, and its
   `DxfWriter.writeACDSData()` is empty; do not mistake that checkout or its
   maintained AC1027 sample for the proposed writer implementation.
   The exact #1150 head was separately fetched into an isolated temporary
   clone at `098d7a3a7ce84101e2391ce965fc4138d1b35e54` and source-audited. Its
   DXF writer emits six schemas (IDs 0–5), a type-1 `ACDSRECORD`,
   `AcDbDs::ID` group 320, `ASM_Data`, declared group-94 length, and chunked
   group-310 bytes, after OBJECTS and only for AC1027+. Its entity writer emits
   the R2013+ `AcDbModelerGeometry` shell as group 290=1 plus a GUID (generating
   one when absent), and emits the `AcDb3dSolid` subclass/group-350 zero
   history field from AC1021+. This is an exact candidate mapping, not a
   normative contract. Its DXF reader test hand-authors a partial section and
   tests one synthetic REGION attachment; the DWG AcDs tests exercise the
   separate DWG container. No test writes a complete DXF section with the PR's
   own writer and validates that output independently. The PR comment reports
   TrueView and ODA-reader checks, but provides no hash-pinned output, complete
   invocation log, or independent rerun result. The local host has no .NET SDK,
   so the isolated branch could not be built here. Keep those reports advisory.
   In particular, the PR's unconditional group-350 zero differs from the ODA
   witness's nonzero history handle and still needs independent field policy.
   **Current-state recheck (2026-09-26):** the upstream PR is still open at
   head `098d7a3a7ce84101e2391ce965fc4138d1b35e54`. Its September 4 update
   reports additional DWG TrueView checks for R2000/R2004/R2007/R2010/R2013,
   an SAT group-1/3 token-boundary fix, and version-gating the
   `AcDb3dSolid` subclass/history field; the discussion also repeats the
   author's ODA-reader check. The PR page still shows five commits and no
   maintainer review or published hash-pinned output pair; no later test
   artifact or reproducible tool-build/command manifest is available. The
   maintainer's August 1 comment explicitly defers writer review until the
   Prototype1b reader is stable and calls for manual testing in different
   software. The source is a valuable implementation candidate, but it does
   not discharge S2.3.2; PR state, source code, or an unpinned test report
   alone is not contract evidence.
   **Independent semantic readback completed (2026-09-26; narrow evidence, no
   public support promotion):** used the official
   [Open CAD Studio `v2026.38` release](https://github.com/HakanSeven12/OpenCADStudio/releases/tag/v2026.38)
   macOS arm64 DMG, SHA-256
   `469d8f4546872fa2cee615b22513d069235cf322d559ef117fa16306ac879c46`.
   `OpenCADStudio-App --serve` reported version `2026.38`; the release source
   revision is `0d023d267bc5b7afeca3b54e98875b0efd4f3926`, with release notes
   pinning `cadcodec` `5b682ed` (upstream acadrust sync) and `cadkernel`
   `6f046af`. This is a distinct reader/kernel from libdxfrw and LibreDWG, but
   not independent of acadrust; retain that dependency caveat. The official
   app was run locally against public/temp data only; no user drawing was
   opened or uploaded.

   **Reproducible source-provenance pair:** source is the tracked public
   LibreDWG file `test/test-data/2000/Cone.dwg` at repository commit
   `6d6a33987a1a97095b069c799c3d8a793320f812`, SHA-256
   `a444b0148dd58bb4269bf80dbda3f81dfc0a09980fb9713e89de7ff6b1c396f4`.
   ODA File Converter `27.1.0.0` generated an AC1027/R2013 DWG and then its
   paired AC1027 DXF using these exact argument sequences (unique folders
   under `/private/tmp`; recursion and audit disabled):

   ```text
   ODAFileConverter <input-folder> <dwg-output-folder> ACAD2013 DWG 0 0 *.DWG
   ODAFileConverter <dwg-output-folder> <dxf-output-folder> ACAD2013 DXF 0 0 *.DWG
   ```

   The resulting `Cone.dwg` SHA-256 is
   `01eb475b13e0cf7c4585e81130a55fe7613618b11899336df4a4e96658cf9df8`; the
   paired `Cone.dxf` SHA-256 is
   `8cad81489ae0ce314bbbae4530b23e1ed0a0c811ef0d4f158f5962ad8fd62292`.
   The DXF contains one `3DSOLID` handle `20B`, owner `1F`, shell
   `AcDbModelerGeometry`/group 290=`1`/GUID
   `{8689176e-18f0-1c46-8176-73b0acf4d1bc}`, then `AcDb3dSolid`/group
   350=`20E`. The ACDSDATA section has six observed schemas (IDs 0–5); the
   relevant schema ID 1 is `AcDb3DSolid_ASM_Data`. Its `ASM_Data` record
   stores group-320 key `20B`, declared group-94 length 1,980, and 16 ordered
   group-310 chunks (15×127 bytes plus 75 bytes). Reassembling those chunks
   yields exactly 1,980 bytes and SHA-256
   `a5ab9c449ddc100afa636d59a69d883211bd4adbebf3e8e9654918a2602b028e`.
   The record key equals the modeler entity handle. Open CAD Studio independently
   parsed that ACDSDATA record from the DXF to the same 1,980-byte `sab_data`
   (exact hash match); it also read the paired DWG to one `3DSOLID` with the
   same handle, SAB version 2, and payload hash. The entity's group-350 handle
   `20E` is retained by Open CAD Studio as a `ProxyObject` history object.

   The reader's kernel produced identical DWG- and DXF-side mesh results:
   volume `392.5413946193438`, surface area `326.84057228347973`, 254
   triangles/762 vertices, centroid approximately `(10,10,3.75)`. This is
   semantic geometry evidence for this exact ODA-produced cone pair, not a
   libdxfrw-output or FreeCAD acceptance result. One earlier ODA-resaved pair
   differed at SAB bytes 100 and 103, both embedded timestamp characters,
   while Open CAD Studio reported the same mesh metrics; this new, explicitly
   reproduced pair is byte-identical between DWG and DXF. Neither observation
   defines a general timestamp-update permission. The implementation policy
   for this slice is therefore: preserve the SAB bytes and timestamp exactly;
   preserve the observed group-2 GUID as opaque text; retarget group 320 to the
   final emitted entity handle; remap group 350 only through the normal object
   handle map, failing closed if its history object is absent/ambiguous. Preserve
   the whole observed schema/record table; do not infer missing schemas.

   This completes S2.3.2 only for the observed standalone ODA 27.1.0.0 / AC1027
   `3DSOLID` / `AcDb3DSolid_ASM_Data` tuple and authorizes starting S2.3.4 for
   that tuple. It does not qualify clones, INSERT/XREF, other entities,
   producers, schema variants, versions, DWG modeler writing, FreeCAD solid
   geometry, or any support claim. S2.3.6 must still run ODA acceptance on the
   actual libdxfrw-produced DXF, followed by the separately gated FreeCAD
   converter handoff. Keep every source, downloaded app, and output under
   `/private/tmp`; do not add fixture files.
   ACadSharp issue [#1195](https://github.com/DomCR/ACadSharp/issues/1195)
   (opened 2026-08-19) is a concrete but unfulfilled evidence lead: its author
   reports two customer DWGs containing 252 and 51 `3DSOLID` entities plus
   three `REGION`s, confirms counts with LibreDWG, and offers to provide a
   trimmed reproducer on request. The post does not identify the source CAD
   producer/build, versions, file hashes, or a corresponding DXF; the drawings
   are not attached. A qualifying request would need to ask for a sanitized
   source-provenance DWG/DXF pair and its creation/export manifest, not just a
   trimmed DWG. No issue comment was posted: contacting the author is external
   coordination and requires user authorization. Treat this as a possible
   next unblock route, not available evidence.
   A focused Autodesk-primary-source lookup on 2026-09-24 for
   `ACDSDATA`, `ACDSRECORD`, `AcDbDs::ID`, and `ASM_Data` found no published
   DXF envelope or identity contract. The official [ObjectARX `AcDb3dSolid`
   reference](https://help.autodesk.com/cloudhelp/2027/ENU/OARX-RefGuide/files/OARX-RefGuide-AcDb3dSolid.html)
   describes the entity as a container/interface for ShapeManager geometry
   and points geometry traversal to BREP APIs, but does not specify DXF
   serialization. Autodesk's [XRECORD DXF reference](https://help.autodesk.com/cloudhelp/2023/ENU/AutoCAD-DXF/files/GUID-24668FAF-AE03-41AE-AFA4-276C3692827F.htm)
   permits application-defined groups inside XRECORD objects only; it is not
   authority for the distinct ACDSDATA section. This is bounded negative
   evidence, not a reason to invent an XRECORD substitute or promote support.
   Autodesk's generic group-code rule that 320-series handles are not
   translated during INSERT/XREF is still an unresolved lifecycle constraint,
   even though independent readers and multiple samples use group 320 as the
   application-level entity key. A newly reproduced libdxfrw DXF→DXF failure
   shows why “preserve source handles unchanged” is not a viable assumption:
   source `3DSOLID`/`REGION` handles D65/D69/D6A are emitted as 14D3/14D4/14D5,
   but the opaque ACDS section still points to D65/D69/D6A. The section bytes
   otherwise remain identical. S2.2.4 therefore repairs only the proven
   `ASM_Data` owner key in the standalone writer by mapping it to the final
   emitted entity handle. This is not a general rule for every group-320 field
   or for INSERT/XREF, and it does not prove the required fields for a newly
   generated section.

   A bounded follow-up on 2026-09-24 found no
   downloadable `mm.dxf` in the public BricsCAD/ezdxf analysis. The existing
   sibling ezdxf v1.4.4 checkout contains a tracked AC1032
   `examples_dxf/uncommon.dxf` (SHA-256
   `5efdd671e3b6d8c45bd0688b1d6479d523f2290f0b390f1584348bf46e34c965`),
   now used only as an opaque section-preservation control under S2.2.3; its
   authoring provenance is not established and it does not settle contract
   semantics. The system and bundled Python environments lack
   `typing_extensions`/`pyparsing`; on 2026-09-26 the pinned ezdxf 1.4.4 and
   its dependencies were provisioned only under `/private/tmp` to reproduce
   issue #1411. The resulting generated-control evidence and ceiling are
   recorded below; do not install into the sibling checkout or treat this
   environment-specific repro as CAD-producer evidence.
   **ODA acceptance rerun (2026-09-26; structural, not semantic):** the
   ACadSharp-maintained AC1027 ASCII DXF was copied only into a fresh
   `/private/tmp` input folder. ODA File Converter 27.1.0.0 converted it with
   `ACAD2013 DWG`, recursion/audit disabled, then converted that DWG back with
   `ACAD2013 DXF`; both runs completed successfully. Exact source/output hashes
   and the field-level result are recorded under S2.3.1 above. An initial
   sandboxed CLI launch still aborted with macOS pasteboard `-4960`; after
   that boundary was isolated, the same narrowly scoped local conversion was
   run with host authorization. No project or sibling-checkout files were
   changed. ODA accepted and retained the externally supplied section and
   exact payload bytes, while changing the entity GUID-like group-2 values.
   LibreDWG 0.14 recognized three linked entities in the generated DWG but
   returned empty payload strings, and ezdxf 1.4.4 could not decode the SAB
   token stream. Thus this adds a reproducible independent writer/readback
   control for the ACadSharp-maintained sample, but not an ACIS geometry
   oracle or libdxfrw-output acceptance. Do not install dependencies into the
   sibling checkout or infer the contract from static code. Do not repeat
   broad searches. The exact #1150 patch is locally source-audited, and the
   external AC1027 DXF pair exposed a concrete stale-link failure. S2.2.4 now
   fixes that narrow source-key→output-key association with generated vectors
   and the exact CLI regression; it is an independent DXF-to-DXF correctness
   repair, not permission for a generic DWG DataStorage serializer.

   S2.3.2 is `READY_NARROW` only for the exact hash-pinned ODA 27.1.0.0 /
   AC1027 / one `3DSOLID` / `AcDb3DSolid_ASM_Data` standalone tuple above. For
   this slice, preserve the observed six-schema section and SAB byte sequence;
   retarget the `ASM_Data` group-320 owner key to the final emitted entity
   handle; preserve the group-2 GUID verbatim; remap group-350 through the
   existing object handle map and fail closed if the referenced history object
   is missing or ambiguous. The evidence packet distinguishes the observed
   schema indexes, record key, entity handle, and history-object handle but
   does not define their lifecycle for new solids, clone/INSERT/XREF, or other
   producers. The two ODA timestamp observations disagree on whether a
   particular converter pass rewrites timestamp characters; preserve bytes and
   do not normalize timestamps. The 16 `310` chunks reconstruct exactly the
   declared length in their observed order; emit that same order and do not
   infer a different chunking rule. These policies authorize only S2.3.4's
   narrow implementation, not a support claim.

   Implement this one-tuple writer path in small fail-closed slices, add fast
   runtime-generated vectors, and run the existing ODA 27.1.0.0 acceptance
   route plus Open CAD Studio semantic readback against the actual
   libdxfrw-produced DXF. Only
   after that may the exact FreeCAD `open()`/`insert()` handoff be attempted;
   FreeCAD solid geometry remains separately unqualified. ODA §24 establishes
   DWG DataStorage only. LibreDWG's unhandled ACDS object classification and
   heuristic DWG extraction cannot generalize this DXF evidence. Never set
   DXF group 70 to 2 merely because the DWG payload uses SAB version 2; they
   are different fields. Keep INSERT/XREF, arbitrary remapping, additional
   entities/schemas/versions/producers, and DWG modeler writing separately
   blocked.
   ODA §24
   establishes DWG DataStorage only. LibreDWG's
   unhandled ACDS object classification and heuristic DWG extraction cannot
   settle the DXF writer contract. Never set DXF group 70 to 2 merely because
   the DWG payload uses SAB version 2; they are different fields. If the
   field/key contract remains unresolved after these bounded checks, keep
   production emission blocked and continue other slices rather than infer
   a contract from one producer.

   Primary references checked 2026-09-24: Autodesk [3DSOLID DXF reference
   (2020)](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm),
   [general group-code rules (2027)](https://help.autodesk.com/cloudhelp/2027/ENU/OARX-RefGuide/files/OARX-RefGuide-DXF_Group_Codes.html),
   and [ODA DWG v5.4.1 §24](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf).
   Cross-checks: LibreDWG [object coverage](https://github.com/LibreDWG/libredwg/blob/master/doc/LibreDWG.texi)
   marks ACDSRECORD/ACDSSCHEMA unhandled; its [release notes](https://github.com/LibreDWG/libredwg/blob/master/NEWS)
   describe current DWG extraction as incomplete/heuristic, while its [issue
   #1411](https://github.com/LibreDWG/libredwg/issues/1411) is an ezdxf-authored
   R2018 reproducer, not evidence of AutoCAD acceptance. ACadSharp's merged
   [ACIS DXF/DWG reader](https://github.com/DomCR/ACadSharp/pull/1139) and
   proposed [ACIS writer](https://github.com/DomCR/ACadSharp/pull/1150) provide
   independent implementation evidence with the limits stated above; neither
   is a normative DXF specification. No available source yet proves the
   complete required DXF entity/section contract or handle lifecycle.
   **2026-09-26 pinned local-control reproduction (not a target witness):**
   provisioned ezdxf 1.4.4 and dependencies only under
   `/private/tmp/libdxfrw-bricscad-mm.pBvt93/python`, then reproduced issue
   #1411's AC1032 `REGION` recipe from scratch. The ezdxf-authored source DXF
   (SHA-256
   `6d4704e6d5730203249857d6c786242f704596a2ac0a35d479bfe185b37ab0ff`)
   contains one `REGION` handle `2F`, six schemas, and one `ASM_Data` record
   whose group-320 key is `2F`; its group-94 length and 44 group-310 chunks
   both reconstruct to 5,489 bytes. ODA File Converter 27.1.0.0 converted
   that DXF to AC1032 DWG (SHA-256
   `4090b886ef97c2a3af8e91197172df7fa2b0c629fb8b813103a6c6e05da3c645`)
   and exported it back to AC1032 DXF (SHA-256
   `e9ec62ed28ec5add175340ea154f900cc0cc4d32e07babceaafb72ac41fc76cf`).
   The ODA DXF again has one `REGION` handle `2F`, six schemas, and one
   group-320-keyed `ASM_Data` record; the 44 chunks and declared length agree
   at 5,497 bytes. ezdxf 1.4.4 parses both payloads as one body with eight
   vertices, six flat faces, and identical bounds `[-0.5,0.5]^3`; the raw
   payloads are not equal (ODA changed/re-encoded multiple byte ranges), so
   this does not establish a general timestamp or byte-transform rule.
   LibreDWG 0.14 `dwgread -O minJSON` reads the ODA DWG and identifies the
   `REGION` at handle `2F` with `has_ds_data=1` and `acis_empty=0`, but exposes
   `acis_data` only as an empty string. Its successful `dwg2dxf` output
   (SHA-256
   `5b1856b2bfb5102886e2c6ec08a0c0f296933af697f73b1a2e387106fc3945b5`)
   retains the `REGION` shell but has zero SAB bytes and no `ACDSDATA`; its
   log reports an invalid REGION material reference and empty history, so
   exit status zero is not a semantic pass. This checkout's exact FreeCAD
   `dwg2dxf input -o output` form exits 1 on the same ODA DWG and publishes
   no DXF. All generated files remain in `/private/tmp`; none is a repository
   fixture. The control makes the issue reproducible and supplies a bounded
   cross-tool negative vector, but the original author is ezdxf rather than
   an independent CAD application, ODA's output is a conversion/resave, and
   no INSERT/XREF or producer identity lifecycle was exercised. It does not
   satisfy S2.3.2 or authorize support claims.

   The separate public BricsCAD `mm.dxf` attachment was checked again on
   2026-09-26: `download.escain.org` now returns NXDOMAIN; the live parent
   server does not present a matching TLS certificate, and the legacy HTTP
   attachment path returns 404. No bytes were obtained, so the Stack Overflow
   report remains unpinned and advisory. Do not repeat this URL probe unless
   its owner supplies a current location.
   **Fast regression baseline recheck (2026-09-26):**
   `libdxfrw_dwg_local_roundtrip`, `dwg2dxf_version_policy`,
   `dwg2dxf_freecad_cli_compat`, and
   `dwg2dxf_raw_dxf_section_passthrough` pass 4/4. This confirms the existing
   typed-modeler fail-closed, CLI contract, and opaque DXF-section behavior;
   these tests do not consume the generated ODA DWG or qualify ACDSDATA
   extraction/emission. No full suite was run because there was no code edit.
3. **S2.3.3 — Record the available acceptance route (`VERIFIED`, bounded).**
   ODA File Converter 27.1.0.0 successfully read the ODA-origin AC1027 control
   and the ACadSharp-maintained AC1027 ASCII DXF, wrote AC1027 DWG, and
   exported AC1027 ASCII DXF. In the ACadSharp-maintained case it retained six
   schemas, one Thumbnail_Data record, all three modeler owner keys and their
   exact SAB payload bytes; group-2 identifiers changed, while entity handles
   and group-350 history handles did not. This is external reader/writer
   acceptance for these two inputs and the pinned ODA version, not a normative
   DXF contract, a semantic ACIS-kernel result, or acceptance of libdxfrw
   output. LibreDWG's empty payload extraction and ezdxf's SAB token parse
   failure keep the independent geometry oracle open. Use the official [ODA File Converter
   CLI](https://www.opendesign.com/guestfiles/oda_file_Converter) for the
   reproducible external check after implementation, scoped strictly to ODA
   27.1.0.0 read/round-trip compatibility. LibreDWG 0.14 is not an ACDSDATA
   oracle: its DXF path ignores this section.
4. **S2.3.4 — Implement the smallest handle-keyed projection (`IMPLEMENTED`,
   bounded to .2 evidence).** The adapter retains typed DataStorage, modeler,
   proxy-object, material, and dictionary records. The writer accepts only one
   AC1027 standalone 3DSOLID, the exact witnessed six-schema/property-name,
   index, type, and value fingerprint, one handle-matched `ASM_Data` SAB
   record, and the exact witnessed three-proxy graph:
   `AcDbShHistory`→`AcDbEvalGraph`→`AcDbShCone`, with 340, 340, and 360
   references respectively. It preserves the observed proxy body bits and
   typed handle edges; the ODA specification
   §20.4.90 defines proxy DWG type 0x1F3, class ID (DXF group 91), drawing
   format (95), source format (70), databits, and handle references. The
   implicit root named-object dictionary is not published by this DWG reader,
   so `ACAD_MATERIAL` is reconstructed only when exactly one complete
   root-owned dictionary has entries that each resolve uniquely to parsed
   MATERIALs and include every history reference. Ambiguous/missing proxy or
   material closure fails before output publication. The generated output
   keeps the group-320 key aligned to the minted entity handle and emits the
   group-350 history link plus the referenced proxy chain. The output
   reconstructs the six observed DXF `ACDSSCHEMA` headers; it does not treat
   the DWG schema directory as those DXF records. Exact SAB bytes and
   timestamp, group-2 GUID when present, source record/schema order, and the
   six observed schemas remain separate from DWG frame/proxy bytes. Other
   schemas, versions, entities, ambiguous graphs, and missing closure remain
   fail-closed. `DRW_Material` currently writes only name/description; do not
   claim its advanced appearance properties are preserved.
5. **S2.3.5 — Fast generated-vector and compatibility gate (`FAST_PASS`,
   narrow).** Runtime-generated ASCII and binary vectors now include the
   modeler, ACDSDATA record, history proxy chain, material dictionary, and
   referenced MATERIAL objects; readback checks the remapped owner key,
   history handle, proxy class IDs/owners/references, and exact payload bytes.
   Negative vectors verify duplicate history proxies, ambiguous matching
   material dictionaries, and seven malformed closure edges (missing proxy,
   wrong class/owner/reference/subclass, missing material, or inconsistent
   proxy-body bit size) fail without publishing. Three more vectors mutate the
   parsed schema-property name table, a property type, or an index mapping and
   must fail without publishing. Readback checks all six
   exact DXF schema headers/names, the three proxy body bit lengths/bytes,
   owners, class IDs, and references. Existing vectors
   continue to cover malformed/missing/duplicate/orphaned ACDS owner keys,
   unsupported inline SAB, and output failure. The fast executable
   `build/libdxfrw_dwg_local_roundtrip` passed on 2026-09-26 and
   `lc3_compat_check` builds. The production
   FreeCAD-compatible CLI also converted the ODA pair with its source-revision
   default; its output SHA-256 exactly matches the file independently opened
   by Open CAD Studio. Forcing AC1024/`-v2010` is correctly rejected by the
   AC1027-only gate. No full suite was run. Generated ASCII/binary vectors
   reverse proxy, material, and dictionary callback collection order and pass
   the same semantic readback. Both entity orders (neighboring LINE
   before/after the 3DSOLID) preserve the line's nonzero-Z endpoints and ACDS
   association. Representative malformed-edge vectors cover closure, schema,
   proxy-body, and owner failures; exhaustive combinatorial cross-products are
   intentionally omitted to keep the default fast slice bounded. The
   locally-authored ezdxf→ODA control and external ACadSharp negative baseline
   remain optional advisory checks; neither substitutes for the required ODA
   acceptance in S2.3.6. Keep generated artifacts temporary; no fixture files
   are added to the repository.
6. **S2.3.6 — Accept actual libdxfrw output externally (`PENDING_ODA`).** The
   exact output from LibreDWG `Cone.dwg` converted by libdxfrw was opened by
   Open CAD Studio v2026.38: one 3D solid, output entity handle `275`, history
   handle `526`, and proxy owners `20E→275`, `20D→20E`, `20C→20D`; the SAB is
   1,980 bytes and hashes to
   `a5ab9c449ddc100afa636d59a69d883211bd4adbebf3e8e9654918a2602b028e`,
   identical to the paired source. The libdxfrw output SHA-256 is
   `6289fd99756dbb68d05ce2f7a4c3d468c8f8948bb3554ad34e26fec328c9337c`.
   ODA File Converter 27.1.0.0's official macOS GUI accepted that exact DXF
   and wrote an AC1032 DWG (`6718aab1b70066f524e2e64722ef5eff50c72fd8c2b2e921c8d752834a4109b9`).
   It then converted that DWG to R14 ASCII DXF
   (`79aaf203b379fc21abf029d5d5cabdd650e5c4cf2a563adefca2fa02f48e3565`),
   retaining a `3DSOLID` but converting ACDSDATA/SAB into inline SAT and
   dropping the schema/history relationships; this downgrade is not the
   required semantic acceptance. The attempt shows the GUI route works, but
   the format selector must be set to ACAD2013 DWG and ACAD2013 DXF to retest
   the observed AC1027 contract. The Mac locked again before that retry. The
   official CLI retry used source/target folders, `ACAD2013`, `DWG`, recurse
   `0`, audit `0`, and `*.dxf` filter, following ODA's documented input
   contract and the invocation shape used by LibreDWG. It still exits 134 with
   pasteboard `-4960` and XPC errors before writing a DWG. The app bundle has
   only the Cocoa Qt platform plugin, so do not inject a mismatched plugin or
   alter the installation to force headless mode. See [ODA File Converter
   CLI](https://www.opendesign.com/guestfiles/oda_file_converter?language=en)
   and [LibreDWG's ODA invocation](https://github.com/LibreDWG/libredwg/blob/master/oda).
   CAD Assistant v1.6.0 official ARM64 release was also tried against the DXF and extracted SAB;
   both returned generic file-transfer errors, so it yields no geometry or
   rejection evidence. Inspection of its signed macOS ARM64 1.6.0.0 bundle
   found only the `CADAssistant` GUI executable in `Contents/MacOS`; there is
   no bundled headless ACIS-reader helper. After unlock, compare schema IDs/names, record
   identity, handle, history/GUID relationship, marker, declared/reassembled
   bytes, and supported neighboring-entity semantics. Any payload timestamp
   mutation remains disallowed absent a rule from S2.3.2. Keep this opt-in and
   out of default CI; do not promote partial acceptance or the R14 downgrade
   to a passing gate.
7. **S2.3.7 — Verify FreeCAD's real converter handoff (one-time, opt-in).**
   Reuse S8.15.19 only after .4–.6 pass, using both the pinned ODA-origin
   witness, the external ACadSharp-maintained AC1027 sample pair above, and,
   if the generated control passes ODA, that locally-authored control. Keep
   external samples at their existing paths; do not copy them into the
   repository. Install the exact `dwg2dxf` build
   outside the build tree and run FreeCAD 1.1.3 `Draft.importDWG.open()` and
   `insert()` independently with `DWGConversion=1` (the LibreDWG-only lane),
   isolated preferences, fallbacks disabled, and FreeCAD's exact
   `[binary, input, "-o", output]`
   argv. Assert zero converter exit, a complete AC1027 ASCII DXF at the exact
   requested path, and that the identical path reaches the corresponding
   normal DXF importer. Inspect importer warnings and the actual `3DSOLID`
   result; independently verify neighboring supported entities. Prior AC1015
   FreeCAD runtime/source evidence says `3DSOLID` is unsupported, but the
   AC1027 ACDSDATA outcome must be measured. A complete converter handoff with
   unsupported/no solid shape is integration-only. If FreeCAD rejects the
   whole DXF, investigate parser/section interaction without dropping the
   carrier or downgrading the entity. Record only the pinned FreeCAD revision,
   OS/architecture, operation, and observed result; keep this test out of
   default CI.

Readiness/self-unblock rule: .1 and .3 are complete research; do not rerun
them absent new evidence. The next blocking item is .2. Resolve it through
version-specific primary DXF documentation, a hash-pinned target-produced
AC1027 DXF with audited provenance, or focused parser/source inspection of the
installed ODA/target tooling. The exact ACadSharp #1150 source audit and
completed S2.2.4 vectors narrow the implementation question but do not satisfy
that provenance gate. The new acadrust/Open CAD Studio route is a concrete
candidate for the missing independent semantic readback; test it only against
the pinned source-provenance pair and do not substitute its self-round-trip
suite. If neither primary/target evidence nor a reproducible external semantic
readback can be obtained, record the exact missing contract, keep only this
serializer gated, and continue independently ready FreeCAD-supported DWG→DXF entity slices. After each
implementation item is
committed, refresh this section's states/dependencies/next unblock step, then
publish the existing progress update before advancing; do not batch progress
updates until the whole ACIS lane finishes.

Positive gate: a recognized AC1027 source yields a complete version-correct
DXF via FreeCAD's exact converter contract; handle/schema associations survive
the libdxfrw reader and ODA checkpoint; supported adjacent entities remain
semantically unchanged; FreeCAD receives the produced DXF at the correct
importer handoff. Negative gate: no SAB is relabeled SAT, placed in an
unrelated entity group, guessed positionally, silently dropped, or emitted
under an unproven entity envelope/handle rule. A failed conversion publishes
no partial final output. Even a positive gate proves converter/carrier
integration only—not ACIS decoding, FreeCAD solid construction, AutoCAD
interoperability, or general AC1027+ ACDSDATA support.

### S3 — Recover ACIS carriers from DWG entities and DataStorage

State: ODA v5.4.1 authority is resolved; S3.1 marker validation and S3.2.1's
AC1024/R2010 inline SAB carrier are committed and independently byte-matched
on 129 solids across four local samples. S3.2.2 also unblocks one AC1027/R2013
3DSOLID with `has_ds_data`: the entity callback receives the uniquely
handle-linked AcDs SAB record, and its exact bytes match LibreDWG 0.14. S3.2.3
adds exact bounded inline SAB-v2 extraction for AC1021/R2007 using the
version-specific object-size/handle framing; one ODA-resaved external DWG
corroborates signature, terminator, and payload length, but not byte identity
or semantic equivalence. These are opaque carrier identity/extraction results
only. In particular, AC1021 `dwg2dxf` still fails closed at the SAB writer and
does not hand a DXF to FreeCAD. All other DWG parser/carrier changes remain
`BLOCKED_PER_VERSION` until the relevant section, successful target trace, and
independent witness are tied to the exact version/field; the AC1024
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
   **Completed sub-slice S3.2.3:** AC1021/R2007 version-2 inline SAB is
   extracted only after the exact `ACIS BinaryFile` signature and one complete
   tagged `End-of-ACIS-data` marker are found inside the bounded entity data
   body. The R2007 object-size field and embedded handle framing are kept
   distinct from the R2010 body/split-handle framing; local runtime-generated
   frames exercise both version paths, tail exclusion, missing/duplicate
   marker rejection, source-version recording, and no fallback from a raw DWG
   frame. One ODA File Converter 27.1.0.0-resaved AC1021 `Cone.dwg`, derived
   from the external LibreDWG `Cone.dwg` witness, has LibreDWG minJSON
   `.acis_data` with the SAB signature plus 1,614 body bytes; libdxfrw extracts
   the expected 1,629-byte signature-plus-body payload. This corroborates
   envelope boundary and length only: the converted output is not a target-
   authored DWG, and there is no full independent payload-byte comparison.
   An earlier comparison of ODA's DXF with the LibreDWG reference DXF had
   different source versions (AC1021 versus AC1015), so that count/value
   difference is not a semantic finding. A same-input check now converts this
   AC1021 DWG with LibreDWG 0.14 `dwgread -O DXF`: its `3DSOLID` has no
   group-1/3 SAT values, while ODA's DXF has 31 pairs. LibreDWG's verbose trace
   reaches its SAB-v2-to-SAT1 converter but fails on unknown SAB tag 18; the
   process nevertheless exits successfully and writes a DXF missing that
   carrier. This demonstrates why output existence/exit status is insufficient
   and why this LibreDWG build is not an independent SAT oracle. Neither output
   proves semantic equivalence. The exact
   FreeCAD form
   `dwg2dxf input.dwg -o output.dxf` reaches the DXF writer, which rejects the
   unqualified SAB representation and publishes no output; this is a passing
   fail-closed converter check, not a successful FreeCAD handoff. No solid
   geometry, AC1021 family-wide support, SAT-v2 support, or DXF writer support
   follows.
   **AC1018/R2004 self-unblock probe (negative only):** the external local
   `ACAD2004 DWG/Cone.dwg` (SHA-256
   `c90a6b9ad0f366b894e67b440d6601d9f5a1a724dcdbb01ba6675d14e017310b`,
   signature AC1018) reaches `writeModelerGeometry()` with a non-empty
   modeler-version-2 object frame (2,445 retained raw-frame bytes) but no
   separately extracted ACIS payload or source-version identity. The
   converter fails closed and publishes no DXF. LibreDWG 0.14 `dwgread -O
   DXF` exits zero but emits only a `3DSOLID` shell with group 70=1 and no
   group-1/3 carrier, `AcDb3dSolid` subclass, or ACDSDATA section; its JSON
   output did not expose the entity as a usable field-level oracle. This
   sample therefore does not unlock AC1018 extraction or conversion and must
   not be used to normalize its opaque frame. It identifies the next evidence
   need: an AC1018 sample whose modeler payload can be independently
   associated and checked, plus its version-specific ODA trace.
3. **S3.3 — pre-R13 separation.** Inventory the R1.4/R11 readers separately.
   Do not apply R13+ layouts to their records; use an appropriate legacy
   specification and authentic version-specific sample trace before changing
   them. If no such source exists, leave code unchanged and keep the row
   unqualified.
4. **S3.4 — bounded extraction.** Implement bounded extraction and validation
   with transactional publication; malformed entity-level payloads must not
   consume adjacent frame/handle data or publish partial geometry. The
   AC1021/AC1024 sub-slices scan only the version-bounded entity body, require
   the exact SAB signature and one complete compound marker, and publish only
   after common handle parsing succeeds. Their object-size and handle-stream
   boundaries are version-specific; do not share offsets or frame assumptions
   across R2007/R2010. Missing/duplicate markers remain opaque; the whole
   frame stays separate in `m_rawBytes`.
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
5. **FreeCAD `dwg2dxf` handoff sub-plan.** For each enabled DWG source version,
   first prove the corresponding legal DXF modeler carrier with primary
   format guidance and an independent reader/writer witness; for Autodesk's
   documented `3DSOLID` route, preserve group-1/3 text and the documented
   modeler version, and never use group 310 proxy graphics as a substitute.
   Any SAB-to-SAT transformation must be ACIS-aware and independently
   qualified; keep SAT text, SAB binary, the DWG object frame, and `ACDSDATA`
   distinct. Exercise the
   installed command with FreeCAD's exact argv and verify the output with the
   public DXF reader, including source `$ACADVER`, modeler entity identity,
   payload bytes/ranges, and unchanged neighboring entities. A carrier that
   cannot be encoded must fail transactionally with no new final DXF; never
   silently omit the solid or substitute a proxy. Once converter output is
   valid, use the existing optional pinned FreeCAD runtime gate to verify the
   same output path reaches `open()`/`insert()`. If that FreeCAD importer
   rejects the legal entity, record converter integration only and leave
   geometry support blocked. Keep platform/importer qualification separate
   from this format/writer slice.

Positive gate: writer output is accepted by an independent reader for every
enabled version and has matching envelope-field/carrier evidence; compare an
opaque payload by its exact digest, not by claiming decoded ACIS semantics.
Negative gate:
unregistered class, absent/unbound payload, bad handle, unsupported version,
or failed DataStorage emission rolls back the entire entity and does not leave
an orphan record.

### S5 — DXF topology, coordinates, and finite-value semantics

State: planned DXF topology, placement, flags, and conversion-boundary fixes
are implemented in S5.1-S5.12. Their evidence remains vector/family-specific;
S7/S8 independently gate broader semantic claims and blocked DWG lanes.

Dependencies: S0, S1. Keep DWG-specific parser changes in S3.

Files: `src/drw_entities.{h,cpp}`; `src/libdxfrw.cpp`;
`tests/dwg_local_roundtrip_tests.cpp`; field serializers.

Steps:

1. Audit both ASCII and binary DXF records against Autodesk group codes for
   `3DFACE`, `POLYLINE`, `VERTEX`, `MESH`, `SOLID`, `TRACE`, `INSERT`, `HATCH`,
   `HELIX`, `ELLIPSE`, and surface entities. Preserve distinct WCS/OCS conventions,
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
9. **S5.7 — Lock HATCH/MPOLYGON elevation and OCS boundaries.** Autodesk's
   HATCH reference requires the elevation point's group-10 X and group-20 Y
   to be zero; group-30 carries elevation. Boundary vertices/edges and seed
   points are 2D OCS values, and the extrusion vector defines their plane.
   Validate the parent elevation/normal before the callback and before output:
   reject nonzero parent X/Y, non-finite elevation/normal, or a zero normal
   rather than silently discarding coordinates while writing canonical zero
   values. Keep boundary/seed values in OCS under both `applyExt` modes. Add
   an in-memory oblique-plane HATCH vector with a separately calculated WCS
   point, malformed X/Y negatives, and writer-preflight negatives; verify the
   shared MPOLYGON validation path too. Do not infer hatch fill tessellation,
   render/display behavior, arbitrary edge-family correctness, or DWG layout
   support from this bounded field-contract slice.
10. **S5.8 — Reject incomplete HATCH/MPOLYGON coordinate pairs.** Autodesk's
    boundary-path table defines OCS point pairs for polyline vertices, line
    endpoints, arc/ellipse centers and axis endpoints, spline control/fit
    points, and spline tangents. Track pending X groups 10–13 while parsing
    and require their matching Y group 20–23 before accepting another field
    or publishing the HATCH/MPOLYGON callback. Preserve the current field
    values and OCS policy; do not transform coordinates. Add runtime-only
    positive pairs and malformed dangling-X/Y cases across the supported
    boundary forms and seed points. Do not infer that wholly omitted optional
    point tuples, edge closure/continuity, arc/ellipse evaluation, fill
    tessellation, rendering, or DWG layouts are qualified by pair validation.
11. **S5.9 — Preserve HATCH elliptic-edge direction.** Autodesk's boundary
    path table assigns group 73 to the elliptic edge's counterclockwise flag;
    ezdxf's `EllipseEdge.ccw` maps the same semantic field. The parser already
    retained group 73 for circular edges and polyline closure, but omitted it
    for elliptic edges, leaving the default `isccw=1` even when the source was
    clockwise. Route the value to `DRW_Ellipse::isccw`; add a hand-authored
    runtime ASCII vector plus ASCII/binary writer-reader round trips asserting
    direction and the center, major-axis endpoint, ratio, and angle fields.
    This is field preservation only; do not infer loop closure, curve
    evaluation, hatch rendering, or FreeCAD support from this slice.
12. **S5.10 — Disambiguate MPOLYGON trailer codes from boundary-path codes.**
    Autodesk's HATCH table assigns group 73 to the MPOLYGON annotated-boundary
    flag, while its boundary-path table assigns group 73 to polyline closure
    and ARC/ELLIPSE direction. Parse the entity-level flag only after the
    top-level group-76 pattern-type field; leave earlier nested path values to
    their existing parsers and reject annotation values outside 0/1. Autodesk
    calls MPOLYGON group 11 an offset vector; ezdxf maps it as a 2D point and
    exports the group-11/21 pair after pattern data. Preserve the pair in the
    existing `xDirX`/`xDirY` API fields without applying an unproven transform,
    reject an incomplete pair, and emit annotation only when non-default.
    Verify hand-authored runtime ASCII inputs for annotation 0/1, unchanged
    boundary closure, malformed annotation and dangling vector; verify ASCII
    and binary write/read round trips for annotation, vector, and ordered
    closed-boundary vertices. This qualifies DXF field preservation only—not
    DWG MPOLYGON layout, fill rendering, FreeCAD's downstream MPOLYGON import,
    or general hatch semantics. Sources: [Autodesk HATCH group codes](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-C6C71CED-CE0F-4184-82A5-07AD6241F15B.htm), [Autodesk boundary-path codes](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-DC5215D6-E73F-4DFF-8BE9-01CA9610FAEE.htm), and [ezdxf MPOLYGON implementation](https://github.com/mozman/ezdxf/blob/master/src/ezdxf/entities/mpolygon.py).
13. **S5.11 — Keep meaningless extrusion vectors out of FreeCAD's WCS polyline input.**
    Classic 3D POLYLINE, polygon-mesh, and polyface vertices are WCS data;
    their parser-side `extPoint` may be zero-initialized and is not a valid
    DXF extrusion vector. In `dxfRW::writePolyline`, omit groups 210/220/230
    for those WCS forms, including an all-zero vector, while preserving the
    existing non-default extrusion serialization for planar OCS POLYLINE.
    Test ASCII and binary DXF with WCS flags 8/16/64, a nonzero-Z vertex, and
    a zero-initialized extrusion field; assert the parent record has no
    extrusion tuple and public readback defaults to +Z without setting
    `haveExtrusion`. Keep an oblique classic planar OCS POLYLINE control to
    prove its non-default vector is still written and read back. Extend the
    exact FreeCAD `dwg2dxf input -o output` CLI control to assert there are no
    210/220/230 tuples on the WCS parent,
    and use the pinned FreeCAD 1.1.3/macOS arm64 `open()` probe to verify the
    same WCS vertex geometry without a zero-length-extrusion diagnostic.
    This is a writer/consumer-compatibility correction only; it does not
    qualify other POLYLINE semantics, DWG interoperability, other FreeCAD
    profiles, or blanket 3D import support. Sources: [Autodesk OCS rules](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm)
    and [POLYLINE group codes](https://help.autodesk.com/cloudhelp/2016/ENU/AutoCAD-DXF/files/GUID-ABF6B778-BE20-4B49-9B58-A94E64CEFFF3.htm).
14. **S5.12 — Reject undefined ARC/CIRCLE extrusion directions without
    altering valid OCS data.** Autodesk's ARC and CIRCLE references define
    their centers in OCS and group 210/220/230 as an optional extrusion
    direction, defaulting to `(0,0,1)`. Reject an explicitly all-zero or
    non-finite extrusion direction before publishing an ARC/CIRCLE callback
    and before writing either entity; do not silently run the arbitrary-axis
    transform on an undefined normal or emit an invalid OCS entity. Accept the
    omitted/default and finite nonzero directions, including an oblique normal,
    without normalizing, projecting, or rewriting source fields. Add runtime
    ASCII parse controls for ARC and CIRCLE under both `ext` settings, and
    transactional ASCII/binary writer negatives proving zero-normal entities
    produce no record. Keep the exact FreeCAD `dwg2dxf input -o output`
    positive field/readback gate; a malformed DWG entity must fail the outer
    output transaction rather than leave a partial DXF that FreeCAD's current
    file-existence check could accept. Do not “fix” FreeCAD's oblique-ARC
    importer behavior by changing `dx_iface` to `ext=true`: this converter
    passes OCS fields through with `ext=false`, and standards-correct output is
    required even where the downstream importer is deficient. This slice
    rejects only an undefined direction; it does not qualify arbitrary OCS
    geometry, FreeCAD ARC/CIRCLE construction, thickness extrusion, or DWG
    interoperability. Sources: [Autodesk ARC](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-0B14D8F1-0EBA-44BF-9108-57D8CE614BC8.htm),
    [Autodesk CIRCLE](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-8663262B-222C-414D-B133-4A8506A27C18.htm),
    and [Autodesk OCS rules](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm).

Positive gate: round trips preserve point order, flags, topology, frame, and
finite values. FreeCAD's exact converter argv and output handoff stay covered;
WCS-only polylines do not carry invalid zero extrusion tuples while planar OCS
polylines retain meaningful extrusion. ARC/CIRCLE require a finite nonzero
extrusion direction while valid omitted and oblique directions retain their
established OCS field behavior. Negative gate: invalid vertex indices,
impossible counts, zero/non-finite ARC/CIRCLE normals, non-finite coordinates,
half-present points, or overflowed count arithmetic are rejected before
callback publication or output commit.

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
   primary evidence.
4. **S6.2 — Guard the R2013+ SPLINE BL field boundary locally.** In
   `tests/dwg_local_roundtrip_tests.cpp`, expose only the protected spline-body
   parser through a test probe and hand-encode both AC1027 scenario 2 and an
   AC1024 scenario-1 control with the low-level bitstream primitives, not
   `DRW_Spline::encodeDwgSplineBody()`. The AC1027 vector uses nontrivial
   `splFlag1` and `knotParam` BL values; the AC1024 vector confirms those fields
   are absent and that `Rational`, `Closed`, and `Periodic` remain separate B
   flags. Set each body's exclusive bit boundary before appending a distinct
   BL sentinel; assert the parsed fields and that the next read returns the
   sentinel exactly. This catches width/version-gate/cursor drift in the
   production reader without needing a full DWG or claiming independent
   interoperability. No parser-width change is part of this item.
5. Verify with authentic target splines separately for AC1024, AC1027, and
   AC1032; do not let an AC1024 trace qualify AC1027 or the AC1032 pass-through
   stub. Until each version has a witness, leave that row unqualified.
6. Make unsupported NURBS/surface operations explicit. Retaining control
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
   **S8.3.1 — Typed ELLIPSE consumer fields.** Serialize center, WCS
   major-axis vector, ratio, start/end parameters, and extrusion normal in the
   neutral semantic record. The fast probe writes default-plane full and
   oblique-plane partial ellipses in ASCII and binary DXF, then verifies the
   same fields with both `ext=false` and legacy `ext=true`. Keep coordinates
   and curve parameters distinct from the extrusion normal; this proves
   callback data delivery and the established 2D-mode invariant only, not
   FreeCAD shape construction or DWG-version support.
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
   **S8.4a.6 — Sample-backed DWG ELLIPSE field comparison (AC1021).** Extend
   the optional comparator to the exact existing local `tablet.dwg` content
   (SHA-256
   `7f203649dc8434ef7cf7a46f7f6def2a0192a1163ba34ebcf88ebfe69635ccd4`),
   comparing all 24 ELLIPSE records by handle with LibreDWG 0.14 using the
   adapter's default `ext=false` mode (omit `--apply-extrusion`). Compare
   center and semi-major-axis vectors as WCS tuples, extrusion normal, axis
   ratio, and start/end eccentric anomaly in radians. ODA v5.4.1 §20.4.39 is
   the normative field-layout authority; the current local sample contains
   two negative-Z extrusion normals and 14 non-full parameter intervals,
   while all center/major-axis Z values are zero. The recorded file hash makes
   this exact local evidence reproducible, but its original producer/date are
   unverified: describe it only as a user-owned local sample and never as
   target-authored or AutoCAD-authored. Run all current comparator profiles
   after the change, including existing AC1021 3DFACE/LINE and AC1024
   INSERT/SPLINE/LINE cases. Update the evidence docs without changing the
   ELLIPSE family support disposition. This is one-sample DWG read-field
   corroboration only—not writer, FreeCAD shape, other-version, or general
   ELLIPSE support. Never stage or commit the DWG.
   **S8.4a.7 — Sample-backed DWG ARC/CIRCLE field comparison (AC1021).**
   Reuse the exact-hash, provenance-unverified `tablet.dwg` sample from
   S8.4a.6 and compare all 243 ARC and 168 CIRCLE records by handle with
   LibreDWG 0.14. For ARC compare center tuple, radius, thickness, extrusion,
   and start/end angle radians; for CIRCLE compare center tuple, radius,
   thickness, and extrusion. Use the adapter's default `ext=false` mode and
   ODA v5.4.1 §§20.4.18/.20 as the field-layout authority. In this sample all
   compared centers have Z=0, all extrusion vectors are default +Z, and all
   thickness values are zero; therefore this is baseline field mapping only,
   not non-default OCS/WCS, tilted-plane, extrusion, or 3D-placement coverage.
   Preserve the unverified sample-provenance boundary from S8.4a.6, update both
   evidence docs, and keep ARC/CIRCLE general support unqualified. Run all
   current comparator profiles. Positive gate: exact entity handle sets/counts
   and every listed finite field match. Negative gate: missing/extra handle,
   non-finite or differing field, or sample hash mismatch fails the profile;
   a pass never promotes writing, FreeCAD shape, other-version, or general
   entity support. Never stage or commit the DWG.
   **S8.4a.8 — AutoCAD-authored DWG POINT read matrix (six exact revisions).**
   Extend the optional handle-keyed comparator for the six external
   `nextgis/dwg_samples` files pinned by basename, AC signature, and SHA-256:
   `point3d_R14.dwg`/AC1014,
   `point3d_2000.dwg`/AC1015,
   `point3d_2004.dwg`/AC1018,
   `point3d_2007.dwg`/AC1021,
   `point3d_2010.dwg`/AC1024, and
   `point3d_2013.dwg`/AC1027. The corpus README at pinned commit
   `ff05d5a89ce123da511ed93c5fe5e6df85e02ab8` records AutoCAD 2016 M.49.0.0,
   blank metric drawings, and `_POINT 50,50,50`; ODA v5.4.1 §20.4.31 is the DWG
   POINT layout authority. For every version, require exactly one matching
   POINT at expected handle `0x7c`, WCS `(50,50,50)`, and default extrusion
   `(0,0,1)` in libdxfrw and LibreDWG 0.14. For AC1015/1018/1021/1024/1027,
   additionally require the same point/handle/extrusion from the hash-pinned
   paired target-authored DXF; AC1014 has no paired DXF in this corpus and
   must be reported as such rather than synthesized. Hash/signature mismatch,
   missing/extra POINT, wrong handle/fields, or missing/mismatched paired DXF
   fails closed. Use only caller-supplied files; never download, modify, stage,
   or commit these DWG/DXF samples. Passing qualifies only this POINT read-field
   subset in these six files; it does not qualify DWG writing, `dwg2dxf`
   conversion output, FreeCAD import for the five older profiles, or family-
   wide/version-wide support. Keep the existing AC1027 FreeCAD result under
   S8.15.3.1 as a separate converter/importer claim.
   **S8.4a.9 — Sample-backed AC1021 legacy polygon-mesh field comparison.**
   Extend the existing optional comparator for the exact-hash, user-owned
   `tablet.dwg` sample from S8.4a.6 (`7f203649dc8434ef7cf7a46f7f6def2a0192a1163ba34ebcf88ebfe69635ccd4`).
   Compare both `POLYLINE_MESH` parents, handles `0x154e` and `0x22d8`, by
   handle against LibreDWG 0.14 in the existing `ext=false` route. For each
   parent require DWG type 30 / `AcDbPolygonMesh`, flag 16, matching curve
   type and M/N dimensions (2×5), density, SEQEND, exactly ten ordered owned
   mesh vertices, expected child handles/owners, vertex DWG subtype/flags,
   and all ten WCS point tuples. ODA v5.4.1 §§20.4.13/.34 are the layout
   authority. The two records contain the same ten expected tuples and every
   Z value is zero; original producer/date are unverified. This is one-sample
   flat legacy-mesh read-field corroboration only—not non-planar mesh or face
   topology, AutoCAD-authored interoperability, writing, FreeCAD geometry,
   another version, or general MESH support. Run every existing comparator
   every comparator profile with a locally available pinned input after the
   extension; record absent external corpus inputs as not-run and do not
   download replacements. Retain the local sample outside the repository;
   never stage or commit it. A missing/extra parent or child, hash mismatch,
   changed link/order/type/flag, or non-finite/different coordinate fails the
   profile. Do not promote any format-support claim from this result.
11. **S8.4b — Remaining DWG consumer rows (blocked per family/version).** Add
   comparisons only where the parser layout is verified against ODA, a
   suitable authentic target sample is available, and an independent semantic
   witness returns comparable fields. The AutoCAD-authored AC1015 planar
   3D-POLYLINE sample in S8.4a.4 does not qualify nonzero-Z 3D POLYLINE,
   PFACE, or MESH; S8.4a.5 supplies only a locally generated parser control,
   not target interoperability evidence. It also does not qualify
   ARC/CIRCLE OCS, ACIS/modeler, PLANESURFACE, modern surface, or other-version
   behavior. S8.4a.6/.7 compare ELLIPSE, ARC, and CIRCLE field subsets in one
   provenance-unknown AC1021 local sample only; these do not establish
   target-authored interoperability or general family support. S8.4a.9
   additionally compares two legacy POLYLINE_MESH parents from the same
   provenance-unknown sample, but all twenty observed vertex Z values are
   zero; this is field corroboration, not non-planar mesh/face-topology or
   target-authored interoperability evidence. The sample's LibreDWG and
   libdxfrw PLANESURFACE fields
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
   header, entity-class, or vertex inspection. It is AC1032, while this repo's
   AC1032 reader inherits most behavior from reader27. ODA v5.4.1 includes
   Chapter 8's R2018 organization summary and §9's R2018+ header-size rule;
   those generic sections do not provide the missing class-specific modern
   surface/MESH layout, and Chapter 8 lists only a bounded set of R2018
   deltas. If the artifact becomes available, verify provenance, signature,
   actual class, and non-planar vertices, then trace it against the applicable
   documented container/header rules before using it as a family witness.
   Resolve a reader change only where the real trace demonstrates a
   divergence; inheritance alone proves neither correctness nor defect. It does not address
   nonzero-Z 3D POLYLINE or modern `AcDbSubDMesh`; keep those and PFACE
   interoperability claims blocked until their evidence gates are met.
12. **S8.5 — Consumer-facing contract and release claims.** After S8.1-S8.4a.9,
   document how a 3D-aware client consumes typed geometry and separates opaque
   modeler payloads from decoded fields, and how a 2D client can retain its
   existing projection policy. Update `docs/3D_SUPPORT_STATUS.md` only with
   evidence-backed distinctions among library data delivery, 2D consumer
   mapping, 3D consumer field access, semantic format/version qualification,
   and actual display/edit behavior. Keep all DWG rows not explicitly narrowed
   by S8.4a, S8.4a.1-S8.4a.9, or corresponding S8.4b evidence explicitly
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

16a. **S8.9.1 — Qualify the installable CLI and document setup.** `dwg2dxf`
   already has a CMake install rule to `${CMAKE_INSTALL_BINDIR}`; qualify the
   installed artifact, not only `$<TARGET_FILE:dwg2dxf>` in the build tree.
   Install to a temporary prefix, verify the platform-correct executable name
   (`dwg2dxf` or `dwg2dxf.exe`) is runnable there, and run the existing fast
   CLI contract against that installed binary. Document the minimal setup in
   README/man text: add the installed bindir to FreeCAD's process `PATH`, or
   configure the FreeCAD converter-path preference with the full executable
   path, and choose its LibreDWG-compatible converter lane when attributing an
   `open` or `insert` operation to this tool. Explain that multiple same-named
   executables make PATH order material and that FreeCAD's automatic mode may
   fall back to ODA/QCAD.
   Keep outputs under a temporary prefix/build tree and do not add FreeCAD to
   default CI. This qualifies installation and the documented command only;
   actual FreeCAD discovery is S8.9.2, and no entity/version/shape claim is
   extended here.

16a.i. **S8.9.4 — Make the FreeCAD integration explicitly import-only.**
   FreeCAD's LibreDWG integration uses the same `TeighaFileConverter`
   preference for `dwg2dxf` import and `dxf2dwg` export; when a configured
   value names one tool, current `Draft/importDWG.py` derives the sibling
   executable for the other direction. If that named sibling is missing, the
   resolver does not fall back to PATH. This repository builds `dwg2dxf`, not
   the paired `dxf2dwg`, so pointing that shared preference at this executable
   can make DWG import work while leaving LibreDWG DWG export unavailable.
   Update README/man setup guidance to say this is an import-side converter
   only, and do not imply a complete/bidirectional LibreDWG toolchain. Explain
   the preference/sibling consequence and recommend PATH discovery with the
   dedicated LibreDWG import mode, without a `TeighaFileConverter` preference
   naming either LibreDWG executable, when users want this executable for
   import without displacing a separately configured export tool. If users
   explicitly configure this path, tell them to keep export routed to a
   converter that actually supplies `dxf2dwg`. Confirm the advice against the
   current [FreeCAD resolver source](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py#L129-L165)
   and record the review date; pin the exact source revision for runtime
   qualification under S8.9.2.
   do not install a fake sibling, add a symlink, or expand this task into
   implementing DXF-to-DWG conversion. Validation: fast documentation/link
   review plus the existing installed-CLI compatibility CTest. This is a
   documentation/expectation boundary only; it does not add FreeCAD to
   default CI or change support claims.

16a.ii. **S8.9.5 — Qualify the installed converter in FreeCAD's own process
environment, including the native Windows artifact.** Close the gap between
the existing installed-binary `freecadcmd` checks and end-user discovery from
FreeCAD installations. Treat FreeCAD as an external process using the
installed `dwg2dxf`, not as a library consumer of libdxfrw. The source-reviewed
FreeCAD resolver checks its `TeighaFileConverter` preference and then PATH;
on Windows it searches for `dwg2dxf.exe`, while Linux/macOS search for
`dwg2dxf`. Its import route launches the exact argument vector
`[converter, input.dwg, "-o", output.dxf]` and hands that output path to the
DXF importer. Current FreeCAD documentation also says DWG inherits DXF import
settings and distinguishes its faster C++ importer from the legacy Python
importer. Keep those importer profiles separate.

   Sub-plan / acceptance gates:

   1. On each available native platform, install into an isolated prefix and
      use the installed artifact—not `$<TARGET_FILE:dwg2dxf>`—from a clean
      FreeCAD profile. Verify it starts without relying on the build tree or
      developer-only library paths. First record the CMake build mode and the
      executable's direct/transitive runtime dependencies using native loader
      inspection (`otool`/loader tracing, `readelf`/loader tracing, or Windows
      PE dependency inspection). Run the installed executable from a working
      directory outside the checkout with build-tree and developer loader
      paths removed from the test environment; prove non-system dependencies
      resolve from the installed package or an explicitly documented system
      dependency, not accidentally from FreeCAD's private libraries. On
      Windows assert the installed filename is `dwg2dxf.exe`; on Linux/macOS
      assert `dwg2dxf`.
      **Build-mode/dependency gate:** keep packaging work minimal and
      evidence-led. The CI/default `BUILD_SHARED_LIBS=OFF` install profile is
      the baseline. If shared `dxfrw` builds are advertised for FreeCAD use,
      add one separate install smoke for that mode and require its runtime
      library to resolve from the documented install/system location;
      otherwise state the supported CLI package/build mode. If a dependency
      fails, choose the smallest existing project-compatible fix (static-link
      the CLI where supported, install the required library with correct
      loader metadata, or document a real runtime package dependency). Do not
      indiscriminately bundle host/system libraries or rely on FreeCAD's
      inherited loader environment.
   2. Exercise both supported discovery forms where that FreeCAD revision
      permits them: PATH with the LibreDWG-only converter choice, and a
      configured full path in an isolated profile. Also test a conflicting
      earlier PATH executable so the log/hash proves which binary won. Do not
      change the user's real FreeCAD preferences or create a fake `dxf2dwg`
      sibling; the shared preference's export effect is S8.9.4. For each
      end-user recipe, exercise the same FreeCAD launch route that the recipe
      names. A direct test-harness PATH override qualifies only a controlled
      launch; if the OS desktop launcher does not inherit it, qualify a safe
      explicit-path profile or document the recipe as shell-launched. Do not
      make shell PATH claims for Finder/LaunchServices, Linux desktop entries,
      or Windows Start-menu launch without native evidence.
   3. Use an already tracked ordinary-encoding DWG copied to a temporary path
      containing spaces; execute actual `Draft.importDWG.open()` and
      `Draft.importDWG.insert()` as separate cases. Require exactly one
      successful `dwg2dxf <input> -o <output>` invocation, capture executable
      absolute path/hash/argv/return code, and prove the identical output path
      reaches `importDXF.open()` or `importDXF.insert()`. Disable ODA/QCAD
      fallbacks for positive attribution. A simple LINE is sufficient for
      this deployment item; nonzero-Z shape assertions remain S8.14 and
      entity-specific semantics remain S8.15. Exercise non-ASCII paths on
      platforms where path support is claimed; until the native Windows argv
      encoding check passes, leave Windows Unicode paths explicitly
      unqualified.
   4. In the actual FreeCAD child-process run, capture loaded-module/dependency
      origins where the platform permits and reject accidental resolution of
      libdxfrw or another project-private dependency from FreeCAD's own bundle.
      It is acceptable for FreeCAD's own modules to load from its installation;
      the converter must not need those private modules to satisfy its own
      declared dependency graph. Keep the fast installed-CLI contract test
      independent of FreeCAD and default CI. Run the native FreeCAD smoke only
      where FreeCAD is installed;
      record release/revision, OS/architecture, exact launch route, effective
      PATH or configured converter preference, importer mode/settings, and
      executable identity. Update README/man in the same slice if the tested
      setup recipe changes. A missing native platform is
      `BLOCKED_EXTERNAL_PLATFORM`, not inferred from cross-compilation. A
      profile without evidence for its documented desktop launch must be
      described only as shell-launched or explicitly configured, whichever
      route actually passed.
   5. Negative gate: a missing/unlaunchable executable or failed conversion
      must not be attributed to libdxfrw and must not leave a partial final
      DXF that FreeCAD's file-existence check could accept. The audited
      `Draft.importDWG.convertToDxf()` currently waits for the child but checks
      only whether its fresh temporary output path exists; it does not gate
      import on the child return code. Reuse S8.12's fast transactional
      failure tests to prove that every failed write leaves no newly
      published output (and preserves any pre-existing complete output).
      S8.9.7 implements this as an opt-in FreeCAD failure-path check: use a
      temporary malformed input with fallbacks disabled and assert nonzero
      converter status, no output path, no DXF-importer handoff, and no
      success marker. Do not add a malformed DWG binary fixture to the
      repository. Keep fallback-enabled diagnosis separate and name the
      actual producer.

   Already established: installation under a temporary prefix and actual
   FreeCAD 1.1.3 `freecadcmd` PATH/configured-path discovery on macOS arm64
   (S8.9.1/.2), with `open()` and `insert()` handoffs (S8.9.2/.3); S8.9.6
   additionally covers registered dispatch inside a controlled macOS GUI
   process. Requalified the current installed macOS `BUILD_SHARED_LIBS=OFF`
   artifact on 2026-09-24: `/private/tmp/libdxfrw-freecad-deploy-check.3AjOVh/bin/dwg2dxf`,
   SHA-256 `2ce17276b418bc5d3e7417e2d7642bcc0eea7b0535c8d0a8bf027ea822035b1f`.
   `otool -L` lists only `/usr/lib/libc++.1.dylib` and
   `/usr/lib/libSystem.B.dylib`. Exact FreeCAD argv converted the tracked
   AC1027 input under `env -i PATH=/usr/bin:/bin` from a temp working directory;
   input/output paths with spaces succeeded. Then the existing
   `run_freecad_installed_3d_line_test.cmake` ran with all `DYLD_*` and
   `LD_LIBRARY_PATH` variables unset and a minimal PATH: FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794` on macOS 27 arm64 selected that
   exact installed binary, invoked `[binary,input,-o,output]`, and handed the
   same DXF to its C++ importer. Separate `open()` and `insert()` runs both
   created the expected LINE bounds `(1,2,3)-(4,6,9)`; focused
   `dwg2dxf_freecad_cli_compat` passed 1/1. The sandboxed FreeCAD attempt
   aborted before app code because Qt could not see host `neon`; both runs
   passed outside the sandbox. This closes standalone dependency provenance
   and process handoff only for this macOS binary/build mode. Remaining: native
   Windows and, where available, Linux install/discovery/runtime checks;
   qualify shared builds only if advertised. The normal OS
   desktop-launch environment (Finder/LaunchServices, Linux desktop entry, or
   Windows Start menu) is not established merely by the current controlled GUI
   launch; test the actual launcher named in setup guidance or keep the
   documented support route narrower. These gaps do not block independent
   ready implementation slices. A passing headless test is not a substitute
   for desktop-process qualification.

16a.iii. **S8.9.6 — Qualify `dwg2dxf` from FreeCAD's desktop application.**
   The user-facing target is FreeCAD's normal desktop workflow, not only
   `freecadcmd`. Reuse the installed-artifact and isolated-profile discipline
   from S8.9.1-S8.9.3; do not add a FreeCAD dependency to default CI or create a
   second fixture corpus. Before writing the smoke, inspect the exact tested
   FreeCAD revision's file-open/import registration and GUI `Draft.importDWG`
   routes so the test invokes the same handler used by ordinary DWG open and
   insertion rather than calling a lower-level DXF parser directly.

   Sub-plan / acceptance gates:

   1. Launch the actual FreeCAD desktop executable with a fresh, isolated
      user-config/data/temp root. Use the C++ DXF importer profile already
      qualified by the headless route and set the dedicated LibreDWG
      conversion mode. For unattended automation, set `dxfShowDialog=false`
      only in the isolated profile so the C++ importer's modal options dialog
      cannot stall the run; assert the effective value in the result. This
      qualifies importer dispatch, not dialog interaction or user choice.
      Exercise the configured full-path route to the
      installed `dwg2dxf`; additionally exercise PATH lookup only when the
      tested desktop launch environment can set and verify PATH deterministically.
      Do not mutate the user's real preferences or infer Finder/Start-menu
      environment behavior from a terminal-launched app. If PATH inheritance
      is not qualified, document the configured-path procedure as the supported
      desktop setup. A configured `TeighaFileConverter` preference may affect
      the sibling `dxf2dwg` export lookup; preserve S8.9.4's import-only warning
      and do not fabricate that sibling.
   2. Reuse the tracked ordinary AC1027 LINE input (copied only under the
      temporary root), and the existing locally authored nonzero-Z LINE
      control only if the desktop launcher can consume it without a new
      generated fixture. Test the normal DWG open action into a fresh document
      and `Draft.importDWG.insert()` into a named target document as separate
      cases. Keep the simple tracked LINE as the deployment oracle; S8.14 and
      S8.15 retain the family-specific 3D semantic gates.
   3. Instrument the GUI process to capture FreeCAD release/revision,
      OS/architecture, DXF importer and relevant preferences, installed
      executable absolute path/hash, every converter argv/status, candidate
      output path, and the exact DXF path passed to the registered open or
      insert handler. Require a successful converter status and one invocation
      per operation; disable ODA/QCAD fallback for the positive attribution.
      Assert independently expected LINE count and XYZ bounds in the opened
      document and insertion target. A macro PASS marker or equivalent result
      file must be required because GUI wrapper/launcher exit status alone is
      not an adequate assertion.
   4. Keep all generated/config/cache outputs beneath a unique temporary root
      and remove only that test-owned root after the run. No fixture is needed;
      never commit generated DWG/DXF output. Fast CLI/readback CTests remain the
      per-change gate. Run this bounded desktop smoke only when FreeCAD-facing
      resolver/packaging/docs change, or when opening a new platform/release
      qualification row—not for every entity-family slice or full-suite pass.
   5. Negative gate: the wrong registered importer, wrong converter/path,
      nonzero process status, fallback-produced DXF, mismatched output handoff,
      unchanged insertion target, wrong LINE bounds, or writes outside the
      isolated root fails the desktop claim even if FreeCAD displays a document.
      Report converter invocation, importer construction, and viewport display
      as distinct outcomes.

   Status: `COMMITTED` for one pinned macOS desktop profile. FreeCAD 1.1.3,
   revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, passed
   the opt-in `dwg2dxf_freecad_desktop_open_insert` CTest (1/1, 20.53 seconds)
   using the installed converter via the isolated configured-path preference.
   The test proves the GUI process selects registered `importDWG`, invokes
   this `dwg2dxf` once with successful exact argv per operation, hands the
   same output to the C++ DXF importer, and creates the expected three LINE
   shapes in open and insert documents with no unsupported features. The
   first attempt exposed the default modal DXF-options dialog; disabling it
   only in the test profile resolved the timeout. Native Windows/Linux
   desktop results stay attached to S8.9.5, and Finder/Start-menu launch,
   dialog interaction, viewport rendering, entities beyond LINE, and export
   remain unqualified.

16a.iv. **S8.9.7 — Verify failed conversion is not mistaken for success by
FreeCAD.** The fast S8.12 CLI test proves transactional output publication,
but FreeCAD's external wrapper must also be exercised because its current
`convertToDxf()` accepts a result path based on existence rather than child
status.

   Sub-plan / acceptance gates:

   1. Add a default-OFF `LIBDXFRW_ENABLE_FREECAD_FAILURE_CONTROL`; require an
      explicit `freecadcmd` and installed `dwg2dxf`, and reject a build-tree
      executable. Reuse `tests/freecad_dwg2dxf_import_check.FCMacro` with a
      failure-only mode and a small CMake runtime harness, not a second
      FreeCAD framework.
   2. Generate the malformed input from scratch under a unique system-temp
      root; seed an isolated `user.cfg`, select `DWGConversion=1`, use
      FreeCAD's exact PATH-resolved installed binary, and suppress ODA/QCAD
      fallback. Exercise `Draft.importDWG.open()` with its actual
      `[dwg2dxf, input.dwg, "-o", output.dxf]` argv. Do not write or stage a
      malformed binary fixture.
   3. Require exactly one process with nonzero status, no candidate output
      path (`lexists` false), a `None`/failed DWG-open result, no call to
      `importDXF.open()` (and therefore no importer statistics), and no
      FreeCAD “Conversion successful” output. Require a dedicated explicit
      macro marker and record FreeCAD revision/platform plus converter
      absolute path/hash/argv/status; wrapper exit status alone is not
      sufficient.
   4. Keep the runtime test OFF by default and run it only when converter
      transaction behavior or the FreeCAD wrapper contract changes. S8.12's
      dependency-free fast failure test remains the per-change gate. On any
      test failure retain only the uniquely named temporary evidence root;
      remove that exact root after a successful run.

   Negative gate: any output file or DXF-importer handoff after nonzero
   conversion, fallback launch, missing result marker, or mislabeled process
   identity fails. This is failure signaling/publication evidence only; it
   does not qualify invalid-input recovery semantics for other DWG versions.

   **Status: `COMMITTED` for one pinned macOS headless profile.** On FreeCAD
   1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64,
   the opt-in `dwg2dxf_freecad_failed_conversion` CTest invoked installed
   `/private/tmp/libdxfrw-freecad-install.0XOVoo/bin/dwg2dxf` (SHA-256
   `5cf1832e7c5d5c2d82f1b05f42aa82dc972069edbc79046043c4a81da1aaa44d`) with
   the exact FreeCAD argv on a generated malformed input. It exited 1, left
   no DXF at the candidate output path, FreeCAD returned no document and did
   not call the DXF importer, and no conversion-success message appeared.
   The dedicated `FREECAD_DWG_IMPORT_FAILURE_ASSERTIONS_PASS=` marker and
   explicit converter identity were verified. The FreeCAD test passes 1/1
   outside the sandbox; its sandboxed attempt reproduces the known Qt
   `neon`-feature startup abort before application code. Existing positive
   `dwg2dxf_freecad_sat_v1_open` also passes 1/1 outside the sandbox after the
   shared macro change; fast version-policy and CLI compatibility tests pass
   2/2. Python AST parsing and `git diff --check` pass. Runtime input/profile
   were generated under system temp and removed on success; no fixture was
   added. This does not close S8.9.5's native Linux/Windows platform gates.

16a.v. **S8.9.8 — Prove installed-artifact 3D geometry handoff through
FreeCAD.** S8.14/S8.15's fast converter controls and generated LINE runtime
checks establish 3D fields and a FreeCAD consumer result, while S8.9.2 and
S8.9.7 exercise the installed executable for ordinary success/failure
behavior. Add one small bridge so the same positive nonzero-Z geometry result
is verified using the actual installed artifact, not a build-tree executable.

   Sub-plan / acceptance gates:

   1. Add a default-OFF opt-in CTest with separate `open` and `insert` cases.
      Require native `freecadcmd`, `dwgadd`, and
      `LIBDXFRW_FREECAD_DWG2DXF_EXECUTABLE`; verify the configured executable
      resolves to a file distinct from `$<TARGET_FILE:dwg2dxf>`. Do not install
      or mutate the user's FreeCAD profile from the test.
   2. Reuse the locally authored
      `tests/fixtures/dwg/ac1015_3d_line_control.dwgadd` recipe and
      `tests/freecad_dwg2dxf_import_check.FCMacro`. Generate a fresh AC1015 DWG
      beneath a unique system-temporary root with paths containing spaces;
      do not commit a generated DWG/DXF. Reuse the existing isolated-profile
      preference seeding and pass the installed executable's bin directory
      first on PATH, with `DWGConversion=1`, the converter-path preference
      empty, and ODA/QCAD fallbacks disabled.
   3. For each operation independently, require the explicit macro PASS
      marker and assert the resolved absolute executable path and SHA-256,
      exact argv `[dwg2dxf, input, "-o", output]`, zero exit, same output path
      handed to the matching `importDXF.open()` or `importDXF.insert()`, and
      one valid imported LINE edge with independently authored endpoint
      bounds `(1,2,3)`–`(4,6,9)`. Require the requested target document for
      insertion and no unsupported LINE report. Successful child execution
      from FreeCAD also proves that the installed artifact's runtime
      dependencies resolve in this profile.
   4. Keep the check optional and run it only for a new installed-artifact
      profile or changes to packaging, CLI, discovery, or converter/importer
      handoff. The dependency-free exact-argv/readback CTest remains the fast
      per-change gate. Record FreeCAD revision, OS/architecture, importer
      mode, operation, installed binary hash, and result. A pass qualifies
      only that tuple and one generated AC1015 LINE; it does not close
      S8.9.5's Linux/Windows gates or promote other DWG families/versions.

   Negative gate: build-tree binary, same-named competing PATH binary,
   fallback conversion, nonzero status, missing/mismatched handoff, wrong
   geometry/target document, missing macro PASS marker, or generated files
   outside the isolated temporary root invalidates the result. Retain the
   uniquely named evidence root on failure and remove only that exact root
   after a successful run.

16a.vi. **S8.9.9 — Publish the measured FreeCAD profile boundary.** The README
and man page explain installation and converter selection, but do not state
which end-to-end FreeCAD profile has actually been qualified. Keep setup
guidance useful while preventing readers from inferring platform-wide or
entity-wide support from the macOS controls.

   Sub-plan / acceptance gates:

   1. In README and `dwg2dxf(1)`, state the pinned profile established by
      S8.9.2/S8.9.3/S8.9.6/S8.9.8: FreeCAD 1.1.3 revision
      `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ DXF
      importer, installed `dwg2dxf`, with `open()` and `insert()` controls.
      Name the generated AC1015 nonzero-Z LINE evidence and its limited scope.
   2. Explicitly leave native Linux/Windows profiles (including Windows
      executable lookup) unqualified, and say that geometry support is
      entity/version-specific. Do not weaken the import-only/export boundary,
      converter-selection/fallback guidance, or suggest changing legal DXF
      output to accommodate importer gaps.
   3. Verify the man page with `mandoc -Tlint`; check Markdown links and
      `git diff --check`. This docs-only slice needs no FreeCAD runtime rerun.

   Negative gate: omit the profile pin, present the macOS result as universal,
   call the import route bidirectional, or promote untested feature support.

16b. **S8.9.2 — Prove FreeCAD discovers and invokes the installed converter.**
   Add a bounded optional runtime smoke using an isolated FreeCAD user config
   and an already-tracked ordinary DWG copied beneath a path containing spaces.
   Prefer the installed `freecadcmd` console executable when available: its
   `Draft.importDWG.open()` route selects FreeCAD's C++ `Import.readDXF` backend
   without opening a GUI. On macOS, invoke the bundle's actual
   `Contents/Resources/bin/freecadcmd`, not the `Contents/MacOS/FreeCAD`
   launcher wrapper, which may detach into the GUI and ignore CLI arguments.
   Pass an explicit `--user-cfg` inside a fresh system-temp root and redirect
   FreeCAD user-data/temp paths there. The optional macro preference-seeding
   mode must verify `App.ConfigGet("UserParameter")` equals that exact config
   and that it is beneath the temp root before it writes any preference.
   The [FreeCAD startup documentation](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Start_up_and_Configuration.md)
   describes `--user-cfg`, console mode, and command-line script startup; use
   the installed executable's observed behavior as the final authority.
   Cover both PATH discovery and the configured executable-path preference.
   For the current FreeCAD `Draft/importDWG.py` contract, test the dedicated
   LibreDWG mode (`DWGConversion=1`); automatic mode (`0`) tries LibreDWG,
   then may fall through to ODA/QCAD, so it is a separate diagnostic only.
   `get_libredwg_converter("dwg2dxf")` uses a configured
   `TeighaFileConverter` value naming either `dwg2dxf` or `dxf2dwg` (deriving
   the sibling executable when needed), otherwise it searches PATH for
   `dwg2dxf.exe` on Windows or `dwg2dxf` on Linux/macOS. Exercise both
   configured-name forms if supported by the tested FreeCAD revision; assert
   the resolved absolute path/hash so a same-named competitor cannot pass.
   For the PATH lane, clear the isolated `TeighaFileConverter` preference so
   the resolver actually performs PATH lookup. Treat the configured-path lane
   as import-only: source-audit or record that a missing sibling `dxf2dwg` is
   not supplied by this project, and never describe a successful `dwg2dxf`
   import as FreeCAD export support.
   Assert the actual subprocess uses exact argv
   `[resolved-dwg2dxf, input, -o, output]`, exits zero, and the identical
   output path is handed to `importDXF.open()` with the existing independent
   LINE coordinate assertions. FreeCAD currently regards output-path
   existence as conversion success without checking the subprocess return
   code; retain the S8.12 failure/no-partial-output guard and assert process
   status in this positive smoke. Record executable path and hash, FreeCAD
   revision/import mode and `DWGConversion`, argv/status, and imported path.
   A FreeCAD console macro exception may be reported while `freecadcmd` still
   exits zero; count a runtime run positive only if its output contains
   `FREECAD_DWG_IMPORT_ASSERTIONS_PASS=` as well as satisfying the in-macro
   converter/import assertions. Do not use process exit status alone.
   Run one smoke per supported OS family when practical; specifically verify
   Windows `.exe` discovery/path behavior. Keep this a compact integration
   check, not a replay of every entity-family test. Never commit generated
   DWG/DXF outputs or downloaded samples. This verifies installed-executable
   attribution only; it does not extend entity/version/shape claims, modify
   FreeCAD, or cover its separate DXF-to-DWG `dxf2dwg` export path.

16c. **S8.9.3 — Qualify FreeCAD's DWG insert entry point independently.**
   Current `Draft.importDWG.insert(filename, docname)` calls the same
   `convertToDxf()` helper as `open()`, but passes the resulting file to
   `importDXF.insert()` for insertion into an existing document. Depends on
   S8.9 and S8.9.1; extend the existing isolated FreeCAD runtime macro using
   the installed converter, tracked ordinary LINE DWG, path-with-spaces setup,
   and `DWGConversion=1` attribution. Reuse S8.9.2's runtime harness/setup if
   available, but do not block this item on configured-path discovery for
   `open()`. Extend
   `tests/freecad_dwg2dxf_import_check.FCMacro` with a bounded
   `LIBDXFRW_FREECAD_OPERATION=insert` mode rather than creating a second test
   framework. Create a target document, call `Draft.importDWG.insert()`, and
   when the installed build supports headless imports, use the same isolated
   `freecadcmd --user-cfg` invocation and require the explicit PASS marker;
   `freecadcmd` may exit zero after a script exception.
   wrap `importDXF.insert()` to capture the exact handoff path and target
   document name. Assert the expected installed executable absolute
   path/hash, argv
   `[resolved-dwg2dxf, input, -o, output]`, zero exit, identical output path
   passed to `importDXF.insert()`, and that insertion modifies the requested
   document with the expected LINE count and independently known XYZ extents.
   Check document identity/content rather than relying on the importer return
   value (the current FreeCAD source itself flags that return behavior as
   uncertain). Exercise the converter through PATH for this entry point and
   assert the executable actually invoked. S8.9.2 separately tests configured
   path versus PATH selection on the shared resolver. Negative gate:
   wrong/missing handoff path, another converter,
   nonzero status, unchanged target document, or wrong extents fails even if a
   DXF file happens to exist. Clean only this smoke's isolated temp/config
   outputs. Do not rerun the entity-family matrix. This closes the `insert()`
   workflow contract and adds one narrow nonzero-Z LINE witness; `open()`
   results remain separately scoped. FreeCAD GUI display is not qualified,
   and neither route establishes
   general DWG/3D support. Update README/man wording to name both FreeCAD DWG
   import entry points while preserving the converter/importer support
   boundary. Keep FreeCAD and generated output out of default CI and the
   source tree.

17. **S8.10 — Qualify actual FreeCAD DWG open/import (external/runtime gate).**
   Execute the real `Draft.importDWG.open()` path with this `dwg2dxf`, then assert
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
   field subset—not `Draft.importDWG.insert()`, GUI display, or general
   FreeCAD/DWG/3D support. The distinct `insert()` route is S8.9.3 and must not
   inherit this `open()` result.

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
18a. **S8.9.10 — Preserve a DXF's declared source revision in FreeCAD's
   no-version command.** FreeCAD's exact `[dwg2dxf, input, -o, output]` form
   has no explicit output-version argument, so reading an AC1027 DXF and
   writing it again must preserve `$ACADVER=AC1027`, not use the text codec's
   normalized AC1021 effective version. Keep `dxfRW::getVersion()` and its
   existing compatibility semantics unchanged; in the CLI adapter, recover
   the declared revision from the header callback data and map it through the
   existing version table, falling back to the reader value only when the
   header is absent or unknown. Extend the fast tracked-fixture CTest with an
   exact FreeCAD-form DXF readback assertion for AC1027. Gate on a successful
   second conversion and exact declared header; preserve all older positional
   and explicit-override behavior. This is CLI output-policy evidence, not a
   new file-format or FreeCAD geometry claim; no fixture is added.
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

   **S8.13.4 — Preserve the modern custom-class 3DLINE through the concrete
   adapter.** `DRW_Interface::add3DLine()` intentionally has a compatibility
   default that projects into `addLine()`, but the `dwg2dxf` adapter must
   override it so this distinct class is not silently reduced to ordinary
   `LINE`. Store a copied `DRW_3DLine`, dispatch `DRW::THREEDLINE` to
   `dxfRW::write3DLine()`, then use a locally constructed in-memory entity to
   verify subtype, start/end XYZ, thickness, and extrusion through generated
   DXF, two exact `dwg2dxf input -o output` passes, and public reader
   readback. Keep all generated files under the build tree and remove them on
   success. S8.13.4.2 repeats this generated adapter vector with binary DXF
   input and output, checking subtype and every payload field after two CLI
   passes and public readback. This item is adapter-only: it must not change
   DWG byte parsing, assert undocumented per-version class availability, map
   standard `LINE` to the extension, or claim that AutoCAD/LibreCAD/FreeCAD accepts the DXF
   `3DLINE` spelling. The independently gated legacy pre-R13 path and modern
   DWG class still require authentic sample/authority evidence. S8.13.4.1
   exercises the existing minimum-version guard with an AC1014 negative
   vector and requires transactional failure to leave no final DXF.

   **S8.13.4.1 — Fail closed for pre-AC1015 3DLINE output.** Drive the
   adapter's typed 3DLINE path while requesting AC1014 DXF output; require the
   existing `write3DLine()` version gate to reject the transaction and assert
   that the requested final path was not published. Generate the entity and
   path at runtime. This verifies the output-version guard only; it does not
   establish historical `3DLINE` DXF portability, target-DWG class
   availability, or any legacy pre-R13 reader behavior.

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
6. Preserve the established R12 DXF downgrade for `ELLIPSE`: sample the WCS
   center/major-axis/normal frame into a legacy `POLYLINE`, using a 2D form
   only for the default XY plane at zero elevation and a 3D WCS polyline
   otherwise. Preserve partial-arc signed sweep and closed-curve topology;
   reject invalid frames instead of publishing an empty polyline. This is an
   explicit, bounded approximation for an output version that cannot encode
   `ELLIPSE`, not a FreeCAD-specific rewrite of newer DXF. FreeCAD's default
   `dwg2dxf` route preserves the source revision, so its ELLIPSE importer row
   is separately gated in S8.15.9.

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

24. **S8.15.2 — Isolate legacy POLYLINE_MESH and PFACE through FreeCAD's
   configured `dwg2dxf` route without mistaking record acceptance for topology
   support.** Use small, locally authored AC1015 `.dwgadd` recipes for a 2×2
   non-planar polygon mesh, a single non-planar PFACE, and a two-face PFACE
   exception reproducer. The opt-in fast CTests generate DWGs in the build
   directory, invoke the exact `dwg2dxf input -o output` argv used by
   `Draft.importDWG.open()`, assert subtype markers, declared dimensions/counts,
   ordered WCS vertices and (for PFACE) signed face indices, then repeat those
   checks after DXF readback. Extend the
   opt-in FreeCAD feature audit with bounded, environment-gated shape details
   (actual vertices, edge endpoints, validity and face counts); default audits
   remain summary-only, detail mode caps shape objects/components, and
   generated controls stay outside default CI.

   In FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++
   importer mode 2, the 2×2 MESH control survives conversion/readback as one
   POLYLINE with dimensions 2×2 and four ordered WCS vertices. FreeCAD creates
   one valid `Part::Feature` but interprets it as a three-edge chain over the
   sequential source vertex list (3 edges, 0 faces), not the expected grid
   connectivity; five zero-length-extrusion warnings also appear. The
   single-face PFACE control survives conversion/readback with four coordinate
   vertices, one face-record vertex and indices `(1,2,3,4)`, but FreeCAD makes
   a valid four-edge wire with 0 faces and one erroneous edge from parent
   origin `(0,0,0)` to the last face vertex instead of closing to `(0,0,1)`;
   six zero-length-extrusion warnings appear. Neither FreeCAD row is
   semantically supported despite object creation and an empty unsupported
   list.

   The separately isolated two-face PFACE control
   (`ac1015_pface_multiface_freecad_control.dwgadd`) has its own fast
   converter/readback CTest and preserves its POLYLINE/vertex/two-face records;
   FreeCAD's C++ importer raises
   `CDxfRead::ReadEntity`'s unknown exception and creates no geometry. Two
   faces alone reproduce the failure; one signed face `(2,3,-4,5)` and one
   triangle `(1,2,3,0)` each import separately, so the observed trigger is
   multi-face PFACE, not signed or zero indices in isolation. Keep these as
   pinned downstream importer limitations; do not alter correct POLYLINE
   subtype/face records to coerce a FreeCAD object. No target-authored
   interoperability or general FreeCAD support is promoted. The mixed
   topology exception is now attributable to its PFACE member but remains
   non-evidence for other family rows.

25. **S8.15.3 — Qualify a nonzero-Z WCS POINT through FreeCAD's configured
   converter path.** Use a locally authored AC1015 `.dwgadd` recipe containing
   one POINT at `(10,20,30)`. An optional fast CTest generates the DWG only in
   the build tree, invokes exact `dwg2dxf input -o output`, verifies one DXF
   POINT with groups 10/20/30, then repeats through DXF readback. An opt-in
   assertion macro must call `Draft.importDWG.open()` with FreeCAD's default
   C++ importer, assert exactly one valid `Part::Feature` with one vertex at
   the expected XYZ and no edges/faces, and check the POINT import count and
   unsupported-feature statistics. Keep `Import points` enabled and report the
   FreeCAD revision/mode/settings, source/output hashes, and converter path.

   In FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, mode 2, the
   configured converter is resolved from `PATH`; the actual route imports one
   valid vertex-only `Part::Feature` at `(10,20,30)`, with zero edges/faces and
   no unsupported feature. The fast CLI/readback CTest and the opt-in runtime
   assertion pass. This locally generated vector remains a basic route check;
   it does not qualify POINT across DWG versions or FreeCAD runtimes.

   **S8.15.3.1 — Add one target-authored consumer witness without vendoring it.**
   The external AutoCAD-authored AC1027 `point3d_2013.dwg` from
   [nextgis/dwg_samples](https://github.com/nextgis/dwg_samples/blob/master/README.md)
   has recorded AutoCAD 2016 provenance and the `_POINT 50,50,50` creation
   command; DWG metadata corroborates the producer. Pin its expected
   SHA-256 (`839297daa100c318d5afbfa128145d2c4782300650c86b2ab58b34cf6fe1aba4`)
   and signature. Independent LibreDWG 0.14 direct DXF export and the paired
   source DXF each contain exactly one POINT at `(50,50,50)`. The optional
   `LIBDXFRW_ENABLE_FREECAD_AUTOCAD_POINT3D_CONTROL` test accepts a caller-
   supplied sample path, verifies the hash/signature, and copies it only into
   a unique system-temp root; do not add this external DWG/DXF to the
   repository. First run the dependency-free exact-argv converter and public
   DXF readback checks for one AC1027 POINT and preserved `$ACADVER`; then use
   isolated FreeCAD 1.1.3/macOS 27 arm64/C++ mode 2 with the installed
   `dwg2dxf` first on PATH, fallbacks disabled, `Import points` enabled, and
   explicit executable/hash and macro PASS-marker checks. The observed result
   is exactly one vertex at `(50,50,50)`, no edges/faces, and no unsupported
   POINT. This raises only the evidence for this one target-authored
   AC1027/POINT/FreeCAD tuple; it does not establish general POINT, other
   entity, version, platform, or importer support. Reject hash/signature or
   installed-binary identity mismatches, missing/extra/wrong POINT geometry,
   unsupported diagnostics, or a missing explicit FreeCAD PASS marker; retain
   the exact isolated failure root for diagnosis. Keep this external runtime
   CTest default-OFF, with the tracked-fixture fast converter/readback test as
   the per-change gate.

26. **S8.15.4 — Qualify an elevated default-normal ARC/CIRCLE pair through the
   FreeCAD converter/importer path.** A local AC1015 `.dwgadd` control contains
   one 90-degree ARC (center `(20,30,40)`, radius 3, angles 0–90) and one
   CIRCLE (center `(10,20,30)`, radius 5), both in the DXF default XY plane
   (`210/220/230 = 0/0/1` by omission). The opt-in exact-argv CTest asserts one
   record of each family, exact center/radius/angle group values, ASCII
   `$ACADVER`, then checks the same after DXF readback. The opt-in runtime
   assertion macro calls `Draft.importDWG.open()` with the C++ importer and
   checks valid edge geometry, ARC center/radius/+Z axis and directed endpoint
   order, plus CIRCLE center/radius/closed XY-plane locus; importer statistics
   must report one ARC and one CIRCLE without unsupported reports.

   FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS
   27 arm64, mode 2, resolves the configured converter through PATH and creates
   two valid `Part::Feature` edges. ARC center/radius are `(20,30,40)`/3, axis
   is `+Z`, and ordered endpoints are `(23,30,40)` then `(20,33,40)`. CIRCLE
   center/radius are `(10,20,30)`/5 with a closed edge in the same XY plane;
   FreeCAD's underlying curve reports axis `-Z`, which is orientation-equivalent
   for this closed, undirected full-circle locus and is recorded rather than
   normalized away. This supports only these generated default-normal,
   elevated controls in the pinned consumer profile. It does not qualify
   oblique OCS transforms, other versions, or target-authored samples. An
   exploratory `dwgadd 0.14` recipe assignment to `arc.extrusion` was explicitly
   ignored, so that attempted control does not qualify oblique OCS. S8.15.10
   later uses a direct LibreDWG API generator and verifies the converter's
   field-preserving route; its pinned FreeCAD result is still negative for
   oblique ARC/CIRCLE shape semantics, so those rows remain downstream-
   unsupported rather than being inferred from this default-normal item.

27. **S8.15.5 — Prove which converter FreeCAD actually used before attributing
   an import result.** In the opt-in feature audit, wrap the configured
   `Draft.importDWG.convertToDxf()` call to record each `subprocess.Popen`
   argv/return code, derive the output path for LibreDWG/ODA/QCAD commands, and
   capture the exact path passed to `importDXF.open()`. Report the configured
   `DWGConversion` preference, resolved `dwg2dxf` path, attempted commands,
   candidate-output existence, final converted/imported DXF paths and the
   process whose output path matches the importer input. Do not label a
   converter “used” from discovery or DXF record counts alone. Tighten each
   positive LINE/POINT/3D-POLYLINE/ARC-CIRCLE assertion macro: temporarily
   disable ODA/QCAD fallback discovery, require exactly one converter process,
   assert its executable/input/`-o` output match the resolved `dwg2dxf`,
   require exit status 0, and require `importDXF.open()` to receive that same
   output. Restore monkeypatched functions even on assertion/error exits.

   This closes an evidence-attribution gap, not an entity-support gap. The
existing AC1024 `visualization_-_condominium_with_skylight.dwg` probe is
user-owned and remains unstaged: `dwg2dxf` fails it at format 13/error 10
with a `ReadEntity` exception; FreeCAD then succeeds through ODA fallback.
The audit previously inspected the fallback DXF but reported only converter
availability, so its SPLINE record/import observation is not evidence for
this `dwg2dxf`. The same fallback-enabled audit on the user-owned AC1024
`visualization_-_conference_room.dwg` also records `dwg2dxf` return 1/no
output followed by ODAFileConverter return 0; the imported ODA DXF has two
SPLINE records and FreeCAD reports two SPLINEs. This is still ODA evidence,
not a direct `dwg2dxf` witness. LibreDWG `dwgadd` 0.14 also emitted no SPLINE
entity from its documented local recipe in the attempted run; classify that
as a generator limitation only, not format or importer evidence. S8.15.7 below
adds a source-authored DXF passed through an independent DWG writer, then
checks `dwg2dxf` fields/readback and the actual FreeCAD importer. This does not
qualify target-authored DWGs or other SPLINE forms. Do not add generated
DWG/DXF binaries or external samples to the repository. Keep FreeCAD
integration optional and use affected fast converter/readback tests plus six
small runtime assertion macros for LINE, POINT, 3D POLYLINE, ARC/CIRCLE, INSERT
and SPLINE as the bounded validation set.

28. **S8.15.6 — Qualify one transformed block INSERT through FreeCAD's actual
   `dwg2dxf` route.** Use only the locally authored
   `tests/fixtures/dxf/ac1015_insert_freecad_control.dxf` source. Its custom
   block contains one LINE from `(1,2,3)` to `(4,6,9)`; one model-space INSERT
   places it at `(10,20,30)` with scale `(2,3,4)` and a 90-degree rotation
   about the default +Z axis. ODAFileConverter independently writes the
   AC1015 DWG into the build directory; no generated DWG/DXF output is checked
   in. The optional fast CTest must check the DWG signature, exact FreeCAD CLI
   invocation `dwg2dxf <input> -o <output>`, INSERT/block/LINE fields, and
   those fields again after libdxfrw DXF readback. The FreeCAD 1.1.3 C++
   runtime assertion must disable ODA/QCAD fallback, prove the only successful
   converter process is this `dwg2dxf` and that its exact output is the path
   passed to `importDXF.open()`, then require one valid `App::Link` with one
   edge and independently calculated endpoints `(4,22,42)` and
   `(-8,28,66)`. Record converter argv/exit, source/output hashes, FreeCAD
   revision, OS/architecture, and importer mode. This qualifies only this
   generated AC1015 default-normal, non-array, non-nested, non-mirrored block
   reference with zero block base point and no attributes. It does not qualify
   MINSERT, nested INSERTs, nonzero block bases, attribute/SEQEND chains,
   negative or mirrored transforms, scale vectors other than this tested
   `(2,3,4)`, oblique OCS, other DWG versions, the legacy Python importer, or
   target-authored interoperability. Keep the ODA dependency opt-in and
   outside default CI.

   Generator evidence is deliberately bounded: the attempted locally authored
   LibreDWG `dwgadd` INSERT recipe produced inconsistent block-entity ownership
   and was discarded; do not weaken libdxfrw's ownership checks to make it
   pass. Separately, LibreDWG issue #1351 documents a shipped `dwgadd` example
   whose INSERT advertises attributes but has a broken attribute chain, so it
   is not an acceptable clean control for this case. Those are generator
   limitations, not evidence against INSERT format semantics or grounds to
   widen the parser change. The locally authored DXF-to-ODA control supplies
   an independent, valid writer path instead.

29. **S8.15.7 — Preserve SPLINE planarity correctly through FreeCAD's
   `dwg2dxf` route.** Use the locally authored
   `tests/fixtures/dxf/ac1015_nonplanar_spline_freecad_control.dxf` as the
   source oracle and ODAFileConverter 27.1.0.0 only as an opt-in independent
   AC1015 DWG writer. Generate the DWG under the build directory; commit only
   the locally authored DXF source, never converted DWG/DXF output. ODA v5.4.1
   §20.4.40 stores scenario-1 control points/knots/weights or scenario-2 fit
   points/tangents, but stores neither DXF's planar flag nor its optional
   normal. Therefore derive a DXF plane normal only when the complete control
   or fit geometry (including fit tangents) establishes a plane within
   floating-point numerical error; do not use the spline's fit/control
   tolerance to relabel a genuinely nonplanar curve as planar. If a stable
   plane cannot be determined, leave bit 8 clear and omit groups 210/220/230.
   Preserve all WCS points and tangents unchanged.

   The source covers four unweighted cubic cases: (a) four clearly
   non-coplanar control points with eight clamped knots; (b) four control
   points on `z = x + 2y`, expected planar with canonical unit normal
   `(1,2,-1)/sqrt(6)`; (c) four non-coplanar fit points and non-coplanar
   endpoint tangents; and (d) control points deviating `1e-8` from a plane
   while the declared control tolerance is `1e-7`, which must remain
   nonplanar. The optional fast CTest
   must verify the AC1015 magic, invoke exactly FreeCAD's no-shell
   `dwg2dxf <input> -o <output>` argv, assert each record's scenario, degree,
   knot/control/fit counts, ordered XYZ geometry, tangents, flags and normal,
   then repeat those semantic assertions after DXF readback. The optional
   FreeCAD 1.1.3 C++ importer-mode-2 macro must disable ODA/QCAD fallbacks,
   prove the one successful `dwg2dxf` process produced the exact file opened
   by `importDXF.open()`, and require four valid single-edge B-rep shapes
   with the independent endpoints and, for the two control splines, cubic
   midpoint values `(5.5,4.25,12.125)` and `(1.25,0.75,2.75)`. Record source
   and output hashes, argv/status, FreeCAD revision, platform/architecture,
   and import settings. ODA and FreeCAD remain opt-in and outside default CI.

   This source-driven result verifies only this generated AC1015 conversion
   and importer profile. It is not target-authored DWG interoperability or a
   blanket SPLINE claim: rational/weighted, closed, periodic, repeated/degenerate
   controls, other knot vectors, other scenarios, AC1021+ DWG layouts, other
   import modes and other FreeCAD releases remain unqualified. The converter
   must not flatten points or alter a legal SPLINE to make an importer accept
   it; representability and downstream acceptance remain separate evidence.

30. **S8.15.8 — Preserve rational 3D HELIX through FreeCAD's converter argv.**
   Use the locally-authored
   `tests/fixtures/dxf/ac1015_helix_freecad_control.dxf`: a nonplanar,
   one-turn, +Z-axis HELIX with a rational cubic, four-quarter control curve
   (13 ordered WCS controls, 17 knots and 13 weights), radius 1, and total
   height 4, plus a separate nonplanar weighted cubic SPLINE with four
   ordered controls and unit/non-unit weights. ODAFileConverter writes only a generated AC1015 DWG under the
   build tree. The fast opt-in CTest must invoke the exact noninteractive
   `dwg2dxf <input> -o <output>` argv, then read the result back, asserting the
   HELIX/AcDbHelix record and standalone SPLINE record, their degree/counts,
   all weight cardinalities, ordered WCS controls, helix
   axis/start/vector/radius/turn/height/handedness/constraint, group-70
   rational flags only, and absence of planar normal groups. Never commit
   generated DWG/DXF outputs.

   The focused trace exposed a real conversion defect: when the DWG spline
   body said weights are present, `parseDwgSplineBody()` set DXF group-70 bit
   `0x10`, which Autodesk defines as *linear* (with the planar bit implied),
   instead of rational bit `0x04`. The control's nonplanar WCS geometry made
   this mismatch observable. Set the DXF flag to `0x04`; the new opt-in route
   test fails against the old value and passes with the fix. Source authority:
   [Autodesk SPLINE group codes](https://help.autodesk.com/cloudhelp/2025/ENU/AutoCAD-DXF/files/GUID-E1F884F8-AA90-4864-A215-3182D47A9C74.htm).

   This qualifies only the locally-authored control through this ODA-generated
   AC1015 DWG, `dwg2dxf`, and libdxfrw DXF readback. In the FreeCAD `main`
   source snapshot reviewed 2026-09-23, the C++ DXF parser
   (`src/Mod/Import/App/dxf/dxf.cpp`) dispatches `SPLINE` to `ReadSpline()`
   but has no `HELIX` case in `ReadEntity()`; HELIX therefore reaches
   `ReadUnknownEntity()`. `ImpExpDxf.cpp`'s generic spline shape path does not
   make HELIX importable. Record this as a downstream limitation of that
   importer revision, not a converter defect: keep the spec-correct HELIX
   record and its fields, and do not replace it with SPLINE or another 2D
   surrogate. Implementing geometric HELIX import requires a separate
   FreeCAD importer change outside this libdxfrw repository. The pinned
   FreeCAD 1.1.3 runtime is a separate, older target and may capture its exact
   warning/object outcome; do not generalize between it and current `main`.
   Any positive FreeCAD HELIX shape claim must pin the runtime revision and
   prove an explicit, semantics-preserving importer route. Keep
   target-authored DWG interoperability, other versions, handedness/axis
   variants, and editing/display claims unqualified.

31. **S8.15.9 — Qualify ELLIPSE semantics through FreeCAD's actual `dwg2dxf`
   workflow without consumer-specific flattening.** The exact FreeCAD
   `Draft.importDWG.open()` path invokes `dwg2dxf <input> -o <output>` and
   defaults this converter to the source DWG revision. Current FreeCAD C++
   `ReadEllipse()` does not consume the extrusion normal, `OnReadEllipse()`
   derives orientation from only the major-axis XY components, and its shape
   builder constructs a complete +Z-plane ellipse while ignoring arc
   start/end. Parser acceptance is therefore not semantic 3D-ellipse or
   elliptic-arc support. Keep converter preservation and downstream shape
   support as separate claims and pin the exact FreeCAD revision/importer.

   **S8.15.9.1 — Fast planar converter/readback control (COMMITTED).** Track
   only locally authored source controls, never generated DWG/DXF outputs.
   `tests/fixtures/dwg/ac1015_ellipse_freecad_control.dwgadd` generates an
   AC1015 planar full ellipse and partial ellipse under the build directory
   with LibreDWG `dwgadd`; this generator is intentionally used only for the
   planar subset because it cannot faithfully express the required tilted or
   elevated geometry. `dwg2dxf_freecad_3d_ellipse_cli` invokes the exact
   FreeCAD argv (`dwg2dxf input -o output`) on paths containing spaces,
   checks the AC1015 signature and DXF `$ACADVER`, both entity records and
   their WCS center/major-axis vector, ratio, start/end parameters, and
   default-normal omission, then repeats those field checks after public DXF
   readback. `tests/fixtures/dxf/ac1015_ellipse_freecad_control.dxf` is a
   separate locally authored source containing planar, elevated-partial, and
   tilted-partial cases for the independent-writer phase; it is not an input
   to this DWGADD CTest and presently promotes no result. Focused CTest passes
   1/1. This is self-generated planar converter/readback evidence only—not
   independent DWG interoperability, FreeCAD shape support, or general
   ELLIPSE support.

   **S8.15.9.2 — Independent DWG writer and pinned FreeCAD geometry check
   (COMMITTED).** The opt-in `dwg2dxf_freecad_3d_ellipse_oda_cli` test invokes
   ODA File Converter 27.1.0.0 to write an AC1015 DWG from the locally
   authored DXF control, placing generated DWG/DXF only in the build tree.
   It checks the three entities' independent WCS centers, major-axis vectors,
   ratios, parameters, and default/tilted normals in `dwg2dxf` output, then
   repeats those field checks after public DXF readback. The focused CTest
   passes 1/1 and uses FreeCAD's exact `dwg2dxf input -o output` CLI contract
   with paths containing spaces. The separate headless runtime macro,
   `tests/freecad_dwg2dxf_3d_ellipse_check.FCMacro`, verifies the isolated
   `--user-cfg`, PATH discovery, executable SHA-256
   `5b7d23baf049746597bf0ca0a78141e94e260dfb0086aa3d468ac702df063f45`, exact
   argv and zero status, and that the same output path reaches both
   `Draft.importDWG.open()`'s DXF handoff and FreeCAD's C++ `Import.readDXF`.
   On FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794` / macOS 27 arm64 / C++ importer
   mode 2, the full XY ellipse becomes one valid closed ellipse with the
   expected center, radii, +Z axis, and full parameter range. The elevated
   partial ellipse keeps its center/radii but is incorrectly made a closed
   full ellipse; the tilted partial ellipse keeps center/radii but loses its
   tilted normal, major-axis direction, and arc parameters and also becomes a
   closed full +Z ellipse.
   FreeCAD reports no unsupported features for any of the three, so the two
   partial rows are explicitly downstream-unsupported despite successful
   object creation. The earlier ODA `-4960` Pasteboard/XPC attempt was
   transient: an unmodified CLI retry produced the AC1015 control and the
   registered CTest then passed. No UI-lock workaround, generated fixture,
   or consumer-specific flattening was used. This is one locally authored
   source through one independent writer and one pinned FreeCAD profile, not
   target-authored DWG interoperability, general ELLIPSE support, or a claim
   about other FreeCAD versions/importer modes.

   **S8.15.10 — Separate oblique OCS converter preservation from FreeCAD
   importer semantics.** Autodesk's [ARC](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-0B14D8F1-0EBA-44BF-9108-57D8CE614BC8.htm),
   [CIRCLE](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-8663262B-222C-414D-B133-4A8506A27C18.htm),
   and [OCS/arbitrary-axis](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm)
   references define group 210/220/230 as the extrusion normal and the ARC /
   CIRCLE center and angles in OCS. Keep a local analytic OCS→WCS oracle.

   Use the opt-in `LIBDXFRW_ENABLE_LIBREDWG_API_FREECAD_CONTROL` target and
   `tests/fixtures/dwg/create_oblique_arc_circle_freecad_control.c` to create
   an AC1015 DWG in the build directory: ARC center `(10,20,30)`, radius 5,
   0–90 degrees; CIRCLE center `(-5,4,10)`, radius 2.5; both normals
   `(0.6,0,0.8)`. The direct LibreDWG public API is deliberate: a local
   `dwgadd 0.14` recipe trial ignored vector-field assignments. Commit the
   generator source only, never the generated DWG/DXF. The focused CTest must
   invoke exact FreeCAD argv `dwg2dxf input -o output` in a path containing
   spaces, assert AC1015 and each entity's OCS center/radius/angle/normal,
   then re-read the emitted DXF through the public API and assert those fields
   again. Keep this a fast opt-in control, independent of full-suite cadence.

   For consumer semantics, `tests/freecad_dwg2dxf_oblique_arc_circle_check.FCMacro`
   must seed only an isolated `--user-cfg`, disable ODA/QCAD fallback, prove
   PATH discovery plus exact executable hash/argv/status and same-path
   `Draft.importDWG.open()` → C++ `Import.readDXF` handoff, and record valid
   shape geometry against the analytic WCS oracle. Run it only in the pinned
   headless environment; do not make FreeCAD a default-CI dependency. The
   helper source in FreeCAD's mutable [`dxf.cpp`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp)
   is a research lead only; the pinned [shape-construction source](https://github.com/FreeCAD/FreeCAD/blob/145529fe741292ff0b3977a01195bf0247425794/src/Mod/Import/App/dxf/ImpExpDxf.cpp)
   and runtime result are the evidence for this exact profile.

   **Completed result / support ceiling.** The opt-in CTest
   `dwg2dxf_freecad_3d_oblique_arc_circle_cli` passes and preserves both
   complete DXF records through DWG→DXF and public DXF readback. FreeCAD 1.1.3
   revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64,
   C++ importer mode 2, invokes the exact installed `dwg2dxf` binary and
   imports both shapes without an unsupported-feature warning, but silently
   ignores the oblique extrusion: the ARC/CIRCLE retain their untransformed
   centers and +Z axes, and the ARC endpoints remain XY-plane endpoints. Both
   semantic comparisons therefore fail. Preserve DXF OCS fields in the
   converter; do not flatten or rewrite them to disguise the downstream
   importer limitation. This qualifies only the generated AC1015 conversion
   route and proves neither external DWG-writer interoperability nor positive
   oblique FreeCAD support. An ODA 27.1.0.0 read of this LibreDWG API-generated
   DWG failed on its dictionary object, so it is not an independent-writer
   witness. Requalify after a pinned FreeCAD importer fix or another
   independently readable oblique-DWG source is available.

   Do not silently rewrite newer-version `ELLIPSE` records as sampled
   `POLYLINE`s to make this importer appear successful. Any consumer-specific
   fallback needs an explicit user-selected mode, documented approximation
   tolerance, and separate opt-in CLI contract. S5.6's sampled R12 output
   applies only to explicitly requested legacy output, not FreeCAD's
   source-version-preserving default. Keep target-authored DWG/version claims
   separate and this optional integration out of default CI.

   Positive gate: `.9.1` preserves its explicitly listed planar fields;
   `.9.2` promotes only exact writer/runtime/geometry rows whose output fields
   and resulting FreeCAD shapes match independent expectations. `.10` may
   establish converter field preservation while separately recording a
   downstream semantic failure, but cannot promote FreeCAD oblique-geometry
   support. Negative gate: wrong plane, missing Z, wrong closure, or wrong
   endpoints stays downstream-unsupported, and no fallback converter may
   contribute to a positive result.

   **S8.15.11 — Preserve MINSERT array fields and measure FreeCAD's imported
   geometry separately.** Autodesk's [INSERT DXF reference](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-28FA4CFB-9D5E-4880-9F11-36C97578252F.htm)
   defines the insertion point in OCS, column/row counts in groups 70/71, and
   column/row spacing in groups 44/45. Use the locally authored
   `tests/fixtures/dxf/ac1015_minsert_freecad_control.dxf`: a default-XY
   `AcDbMInsertBlock`, two columns by two rows, spacing 5/4, insertion point
   `(10,20,30)`, and one WCS LINE in the block from `(1,2,3)` to `(4,6,9)`.
   The subclass marker is required: an initial control carrying the array
   fields under only `AcDbBlockReference` was normalized by ODA to a single
   INSERT with default array values and was discarded as an invalid witness.

   Keep the opt-in ODA CTest small and dependency-isolated from default CI:
   ODAFileConverter 27.1.0.0 writes the AC1015 DWG under the build directory;
   this `dwg2dxf` receives exactly `input -o output` (the FreeCAD argv), and
   the test asserts one `INSERT`/`AcDbMInsertBlock`, block name, placement,
   unit scale/rotation, 2×2 counts, spacing 5/4, and the block LINE both before
   and after public DXF readback. Generated DWG/DXF files remain in the build
   tree; commit only the locally authored DXF source and focused test code.

   The optional `tests/freecad_dwg2dxf_3d_minsert_check.FCMacro` must use an
   isolated FreeCAD profile, disable ODA/QCAD fallbacks, and record FreeCAD
   identity/import settings, installed converter path/hash/argv/status, exact
   output-to-import handoff, entity counts, and edge endpoints. The independent
   expected array has four edges: `(11,22,33)-(14,26,39)`,
   `(16,22,33)-(19,26,39)`, `(11,26,33)-(14,30,39)`, and
   `(16,26,33)-(19,30,39)`. FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer
   mode 2, invokes this installed `dwg2dxf` once and preserves those fields,
   but creates one link/edge at the first cell only and reports no unsupported
   feature. The current C++ `ReadInsert()` source parses location, scale,
   rotation and block name but not array counts/spacings; the pinned runtime
   confirms this consumer limitation. Record converter support and importer
   semantics as separate outcomes. Do not decompose the default MINSERT into
   repeated INSERT records or claim full FreeCAD array support: that would
   hide the lost MINSERT semantics. A future consumer-specific compatibility
   mode, or a positive array claim, needs an explicitly scoped contract and a
   separately qualified FreeCAD importer behavior. This slice does not qualify
   non-default OCS, block bases, nested/mirrored transforms, attributes, other
   DWG versions, legacy Python importer, or target-authored interoperability.

   **S8.15.12 — Qualify SOLID conversion and pin the downstream version gap.**
   FreeCAD `main`'s C++ dispatcher includes `SOLID` but not `3DFACE`, and its
   `OnReadSolid()` callback constructs a face after reordering its input
   corners. That mutable source is not the installed target: FreeCAD 1.1.3
   revision `145529fe741292ff0b3977a01195bf0247425794` has no equivalent
   callback in its pinned `ImpExpDxf.cpp` and its runtime reports SOLID
   unsupported. The test must keep those source/release results distinct.
   Do not combine TRACE, `3DFACE`, arbitrary SOLID normals, or malformed
   controls in this slice.

   The locally authored
   `tests/fixtures/dxf/ac1015_solid_freecad_control.dxf` is one simple 4×3
   quadrilateral at Z=5 with default +Z normal. DXF SOLID's point slots are
   1,2,4,3: group 12 is corner 4 `(0,3,5)` and group 13 is corner 3 `(4,3,5)`.
   ODA File Converter 27.1.0.0 writes the AC1015 DWG only under the build
   directory. The opt-in fast CTest invokes exact FreeCAD argv
   `dwg2dxf input -o output` with paths containing spaces and checks AC1015,
   all twelve corner coordinates, and public DXF readback. Preserve the
   original SOLID; do not substitute sampled lines or another entity.

   The isolated FreeCAD C++ macro disables ODA/QCAD fallbacks and proves the
   installed executable/hash/argv/status and exact output handoff. For the
   pinned 1.1.3 C++ mode-2 profile its expected assertion is deliberately
   negative: one SOLID entity counted, one SOLID unsupported diagnostic, and
   zero created shapes. This records a downstream importer limitation, not a
   `dwg2dxf` conversion failure and not SOLID import support. Generated
   DWG/DXF outputs stay in build/temp; only the locally authored DXF source
   and harness are tracked. A future positive geometry gate must run against
   an exact FreeCAD build containing the newer `ReadSolid`/`OnReadSolid`
   implementation and then verify one valid planar face, area 12, four
   edges, expected corner set, and bounds. Neither path qualifies
   AutoCAD-authored DWGs, arbitrary OCS, TRACE/3DFACE, other versions, or
   general SOLID support.

   **S8.15.13 — Qualify the pinned C++ import path for a basic closed
   LWPOLYLINE.** FreeCAD `main` dispatches `LWPOLYLINE` to `ReadLwPolyLine()`;
   `OnReadPolyline()` builds a wire. Use this common 2D entity as a separate
   consumer-baseline slice, not as 3D evidence. Author one AC1015 closed,
   default-XY four-vertex rectangle at elevation zero; exclude bulges, widths,
   thickness, non-default extrusion, and nonplanar inputs.

   ODA File Converter 27.1.0.0 must generate only a build-tree DWG; exact
   `dwg2dxf input -o output` and public DXF readback checks must preserve
   AC1015, LWPOLYLINE record/count/closed flag, and the ordered four vertices.
   The isolated pinned FreeCAD C++ macro must disable converter fallbacks,
   attribute installed executable/hash/argv/status and exact output handoff,
   then assert one valid closed wire, four edges, four expected vertices,
   zero unsupported entities, and expected planar bounds. Keep the check
   opt-in and fast; this does not qualify bulges, OCS/elevation variants,
   arbitrary files/versions, DWG interoperability, or general polyline
   support. If the pinned importer drops closure or changes topology, record
   that consumer limitation without altering the converter's DXF semantics.

   Implementation evidence: `tests/run_freecad_2d_lwpolyline_oda_cli_test.cmake`
   passes the fast opt-in exact-argv conversion/readback test with ODA File
   Converter 27.1.0.0; its generated AC1015 DWG and both DXFs remain under
   `build/`. The optional `tests/freecad_dwg2dxf_2d_lwpolyline_check.FCMacro`
   passes on FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer
   mode 2. It resolves installed `/opt/homebrew/bin/dwg2dxf` by PATH (SHA-256
   `e4cca8e3f5af9a878eb0085ed14e992f64d35f03065743847701274a46781c99`), invokes
   it exactly once with `input -o output`, and imports that same successful
   output. FreeCAD creates one valid closed four-edge wire with the four
   expected vertices and bounds `(0,0,0)`–`(4,3,0)`, counts one LWPOLYLINE,
   and reports no unsupported entities. The generated ODA DWG also caused
   class-stability and unknown-codepage warnings in `dwg2dxf`; conversion and
   semantic assertions nevertheless passed. This qualifies only this
   locally-authored AC1015/ODA/FreeCAD profile, not target-authored DWGs or
   general LWPOLYLINE/3D support.

   **S8.15.18 — Separate tilted 3DFACE conversion from pinned FreeCAD import
   support.** Autodesk's [3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm)
   defines four WCS corners and optional group-70 flags for the four edges.
   Add one from-scratch AC1015 control with corners `(0,0,0)`, `(4,0,0)`,
   `(4,3,4)`, `(0,3,4)` and group 70 equal to 4 (third edge invisible).
   ODA File Converter 27.1.0.0 writes its DWG only to the build/temp area.
   The fast opt-in CTest must invoke the exact FreeCAD argv, then verify the
   AC1015 header, exactly one 3DFACE, all twelve WCS coordinates, group 70,
   and the same fields after public DXF readback. Never stage generated DWG or
   output DXF files.

   At the integration checkpoint, install the build-identical `dwg2dxf`
   outside the build tree and use an isolated FreeCADCmd profile, PATH
   discovery, `DWGConversion=1`, and suppressed ODA/QCAD fallbacks. Require
   the exact executable/hash/argv and output-to-importer handoff. Pin the
   expected negative result to FreeCAD 1.1.3 revision
   `145529fe741292ff0b3977a01195bf0247425794`: one 3DFACE counted, one
   unsupported diagnostic, and zero created shapes. This is a successful
   converter-integration result but explicitly **not** FreeCAD geometry
   support. Preserve the legal 3DFACE record; do not substitute triangles,
   solids, or lines to conceal the importer gap. A future positive claim needs
   an exact FreeCAD revision whose importer maps 3DFACE, followed by analytic
   vertex/bounds/area validation (expected planar area 20); separately retain
   target-authored DWG/version evidence gates. Keep this runtime CTest OFF by
   default and require FreeCAD/ODA only when opted in.

   **S8.15.18 evidence (COMMITTED):** locally authored DXF source SHA-256
   `d79d3d532b5c1b2364a35773e20fd98c6f7e1af576e0527d3f3a327e4510d1dc`.
   `tests/run_freecad_3dface_oda_cli_test.cmake` passes: ODA generated an
   ephemeral AC1015 DWG; `dwg2dxf input -o output` and DXF re-conversion
   preserve all four tilted WCS corners and invisible-edge flag 4. In the
   isolated FreeCAD runtime, the installed
   `/private/tmp/libdxfrw-freecad-3dface-20260924/bin/dwg2dxf` (SHA-256
   `6a60077d4389437e1e9dd0e2be761062d2f0b792caac432a23fde4f96c1289e4`)
   receives FreeCAD 1.1.3's exact argv and exits 0; the identical AC1015 DXF
   is handed to the C++ importer. FreeCAD revision
   `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ mode 2,
   reports one unsupported 3DFACE (handle `E6`) and creates zero shapes.
   Runtime DWG SHA-256 `5f1a561849721bd5810cb09527cce538fcb7e1e476e43358e62b8f76a24af31a`;
   converted DXF SHA-256
   `79aa28f44b150011cc88c1a489455d90cc3f39ab52327a7dcdacf79c5a04e2dd`.
   Fast CLI validation, runtime handoff/negative assertion, Python AST parse,
   CMake test registration, and `git diff --check` pass. The new runtime test
   is opt-in and default CI remains FreeCAD/ODA-free. Only the from-scratch
   DXF source and harness are tracked; no generated DWG/DXF is.

   **S8.15.19 — Qualify AC1027 ACDSDATA as a FreeCAD converter handoff, not
   as solid import.** This depends on S2.3.4–S2.3.6 passing: do not start a
   FreeCAD runtime check against an unqualified or locally self-round-tripped
   ACDSDATA serializer. Use the hash-pinned ODA-produced AC1027 pair only as
   an opt-in external control; never commit downloaded/resaved DWG/DXF files.
   Invoke the installed `dwg2dxf` with FreeCAD's exact
   `[binary, input, "-o", output]` argv, `DWGConversion=1`, isolated
   preferences, and converter fallbacks disabled. Check `open()` and
   `insert()` in separate runs; attribute the executable/hash/status, assert
   the complete output is AC1027 ASCII DXF, and verify the identical path is
   passed to the corresponding DXF importer. Independently inspect output
   ACDSDATA for the matching schema/record, 3DSOLID handle, payload marker,
   length, and bytes using the external acceptance gate from S2.3.6.

   Current FreeCAD C++ source audit has no `3DSOLID` construction route, and
   the prior pinned AC1015 SAT-v1 runtime result is unsupported; the AC1027
   ACDSDATA behavior is a hypothesis that this check must measure, not an
   expected pass result.
   Any complete successful handoff without a verified solid shape is converter
   integration only, not geometry support. If the drawing contains supported
   neighboring entities, verify their geometry independently as well; do not
   require a solid object from this importer. If FreeCAD rejects the whole
   DXF or fails before the exact handoff, record a negative integration result
   and investigate the legal section/parser interaction without deleting the
   section, dropping the solid, or rewriting it as proxy graphics. Keep this
   runtime test OFF by default and run it only after the fast converter/
   readback and ODA 27.1.0.0 acceptance gates pass. This qualifies only
   AC1027, the pinned FreeCAD revision, OS/architecture, importer mode, and
   operation tested; it does not qualify AC1021/AC1024 SAB conversion, other
   ACDSDATA schemas, other platforms, or FreeCAD ACIS geometry.

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
| S2 | S0, S1 | S2.1/S2.2/S2.2.1–S2.2.4 are committed. S2.2.1 preserves unknown ACDSDATA at codec level; S2.2.3 carries it through the concrete `dwg2dxf` DXF→DXF adapter; S2.2.4 updates only the verified `ASM_Data` owner key to the final emitted modeler handle. Runtime-generated ASCII/binary library controls and an exact FreeCAD-form CLI AC1032 two-pass check cover the join; unrelated code-320 values stay opaque and malformed/ambiguous/orphan joins fail closed. This repairs DXF-input→DXF-output consistency only. S2.2.2 preserves observed AC1027 shell fields. None qualifies DWG-to-DXF SAB/ACDSDATA emission or FreeCAD solid import. ACadSharp #1139 independently corroborates the bounded group-320/`ASM_Data` reader mapping; #1150's exact source was audited but its reports remain advisory. The complete entity envelope, schema requirements, and insert/XREF key lifecycle remain unresolved. Next unblock only S2.3.2 through authoritative/target evidence, then create a complete candidate, run one ODA acceptance check, and only then one optional installed-FreeCAD handoff with exact argv. Keep converter handoff distinct from solid geometry; no DWG parser edits absent the per-version ODA/trace gate. |
| S3 | S0 applicable ODA chapter, per-version authentic trace, S1, S2 | Ready only for modeler entities whose layout is actually described and witnessed. MESH/surface/modern 3DLINE layout gaps stay separately blocked; continue S5/S6 DXF lanes and keep those DWG claims unqualified. |
| S4 | S3 verified payload reading, S2 | Blocked until S3 read evidence; do not build writer layout by mirroring an unverified reader. |
| S5 | S0, S1 | DXF topology/coordinate portion ready after S1; only the DXF portion may proceed while the ODA gate is unresolved. |
| S6 | S0, S1 | DXF surfaces/HELIX ready after S1. DWG spline edits require the local ODA chapter and authentic per-version trace; modern DWG surface edits additionally require a surface-specific primary layout and independent witness. |
| S7 | S1-S6 | Qualify completed rows independently. A blocked DWG row does not block completed DXF evidence or docs; it remains unqualified. |
| S8 | S1, S5-S7.5 | Consumer-contract matrix, 2D source-compatibility guard, generated-DXF `ext=true` regression, and DXF 3D-consumer probe are committed. S8.7 adds selected generated AC1027 ARC/CIRCLE callback fields under both `ext` modes; S8.8 adds selected PFACE values; neither qualifies those entity families. Narrow target-sample DWG read evidence covers AC1024 INSERT/SPLINE and LINE fields, AC1021 3DFACE/LINE fields, the 24-record ELLIPSE, 243-record ARC, and 168-record CIRCLE subsets from one provenance-unknown user-owned sample, plus the planar AC1015 3D-POLYLINE subset. These planar ARC/CIRCLE records have default +Z extrusion and zero thickness and do not qualify non-default OCS/3D placement. A separate LibreDWG-generated AC1015 control exercises nonzero-Z 3D POLYLINE, legacy POLYLINE_MESH, and PFACE across libdxfrw/LibreDWG readers but does not qualify AutoCAD interoperability or promote support claims. S8.9 brings the helper CLI into FreeCAD's exact `input -o output` converter contract while preserving its old syntax; S8.9.1 installs and documents the CLI with fast compatibility checks; S8.9.4 clarifies that the package supplies import-side `dwg2dxf` only, not FreeCAD's sibling `dxf2dwg` export tool. S8.9.2 now verifies installed-binary PATH discovery plus configured direct-path and sibling derivation through pinned FreeCAD 1.1.3 `freecadcmd` on macOS arm64; Windows `.exe` lookup remains separately unqualified. S8.9.3 independently verifies the `insert()`/`importDXF.insert()` handoff through the same headless C++ importer. Both are optional isolated runtime checks, not default-CI dependencies or general shape/support claims. S8.11 directly tests source-version mapping; S8.10 verifies FreeCAD 1.1.3's macOS arm64 C++ `open()` importer against tracked AC1015/AC1018/AC1021/AC1027 LINE fixtures and independent LibreDWG DXF exports. S8.12 covers transactional failure publication. S8.13 fixes typed-entity loss in the concrete CLI adapter, adds exact FreeCAD-argv record-preservation and DWG→DXF→DXF field regressions, and records actual importer outcomes for three tracked advanced fixtures. FreeCAD currently reports those advanced custom/dimension entities as unsupported; preserving correct DXF types is the converter's contract, not proof of import. S8.14 verifies one locally generated nonzero-Z LINE through the full DWG→converter→FreeCAD C++ `open()` path, including both B-rep endpoints. S8.15 expands only to entity families the pinned FreeCAD importer demonstrably maps and keeps unsupported rows explicit. FreeCAD remains an integration-only dependency. Other FreeCAD runtime/import modes and all other DWG rows retain their own gates. Modeler rows additionally wait for S3/S4. Keep adapters outside parser semantics and do not require GUI/rendering code. |

The execution sequence is therefore readiness-first, not table-order-first:
S0 → S1 → S2. S2.2.1–S2.2.4 are completed independent fast DXF regressions
for generic raw-section replay, AC1027 modeler-shell preservation, raw-section
passthrough through the concrete CLI adapter, and safe `ASM_Data` key repair
after output handles are allocated. S2.2.4 is exercised by ASCII/binary
`dx_iface` writer/readback vectors and a locally generated AC1032 CLI input
run twice with FreeCAD's exact `dwg2dxf input -o output` form. The first and
second ASCII and binary outputs join the ACDS key to the public-reader entity
handle. Extending the local check to a binary second pass exposed DIMSTYLE
boolean group codes 290/295 being replayed as 16-bit integers; the parser and
writer now retain their one-byte boolean representation, and the focused
`libdxfrw_dwg_local_roundtrip` CTest passes. This remains a locally generated
codec/adapter regression, not external ACDSDATA acceptance. The new .4 item
repairs data loss only on DXF input→DXF output; none unblocks DWG-to-DXF
modeler-carrier serialization.
S2.3.1/.3 research is complete and S2.3.2 is `READY_NARROW` only for the
hash-pinned ODA AC1027 standalone `3DSOLID` tuple. The bounded S2.3.4
projection is implemented; S2.3.5 generated ASCII/binary vectors, including
reversed proxy/material/dictionary callback collections, and the real
source-revision-default CLI conversion pass. Its output hash matches the
file already read by Open CAD Studio. ODA's official GUI accepted the file and
produced an AC1032 DWG, but only an R14 ASCII DXF return conversion is
available; that downgrade rewrites SAB as inline SAT and drops the ACDSDATA
relationships, so the one-time S2.3.6 semantic gate remains pending. Retry
with ACAD2013 DWG→ACAD2013 DXF when the Mac is unlocked;
do not broaden or promote the supported tuple. Then run S2.3.7/S8.15.19 once
as an optional FreeCAD checkpoint. Meanwhile continue any independent ready
DXF and evidence-vector work; keep S3 → S4 gated on DWG spec/trace readiness;
S7 and S8 proceed per completed rows, with S8's DXF
consumer probe independent of DWG. S8.4a.8/.9 are later read-only POINT-matrix
and flat legacy-mesh field extensions; they do not reopen the already-
completed S8.5 consumer contract or broaden FreeCAD support. S8.14's
nonzero-Z LINE integration,
S8.15.1's 3D POLYLINE integration, S8.15.2's isolated MESH/PFACE controls,
S8.15.3's nonzero-Z POINT integration, S8.15.4's elevated ARC/CIRCLE
integration, S8.15.5's converter-route audit/assertions, S8.15.6's transformed
block INSERT integration, and S8.15.7's control/fit SPLINE integration are
committed. S8.15.8's local rational-HELIX converter/readback control and
correction of the false linear flag are also committed; its FreeCAD runtime
behavior remains unqualified. S8.15.9.2 now closes the independent-writer
ELLIPSE/FreeCAD probe: only its full-XY control imported semantically in the
pinned runtime, while elevated and tilted partials remain downstream-
unsupported. S8.15.10 verifies oblique ARC/CIRCLE field preservation through
the exact converter route while documenting FreeCAD C++ mode 2's silent loss
of OCS geometry; both remain downstream-unsupported. S8.15.11 verifies exact
MINSERT array-field preservation through this converter and readback; pinned
FreeCAD 1.1.3 C++ imports only the first array cell, so array geometry remains
downstream-unsupported. S8.9.5 tracks native
platform installation/discovery separately from the qualified macOS
`freecadcmd` route; those platform gaps do not block independent slices.
S8.9.11 is committed: the observed AC1014 LTYPE extension-dictionary graph
survives conversion and a pinned installed-artifact FreeCAD `open()` check.
Native Linux/Windows qualification remains separate under S8.9.5 and does not
block independent slices.
S8.9.6 is COMMITTED for the pinned macOS GUI profile; the opt-in harness invokes the
registered `importDWG` route via FreeCAD's `module_io.OpenInsertObject()` for
both open and insert, with separate isolated profiles and the existing tracked
LINE fixture. Its desktop CTest passes 1/1 and confirms converter identity/argv,
same-DXF handoff, and expected geometry. The default modal DXF options dialog
initially blocked automation; the isolated profile now disables it and asserts
the setting. This does not test file-dialog/menu interaction or viewport
rendering. Windows/Linux remain S8.9.5 external gates; continue independent
implementation slices. S8.15.12 verifies one locally
authored elevated SOLID through ODA DWG generation and exact `dwg2dxf`
conversion/readback. The pinned FreeCAD 1.1.3 C++ importer reports that same
SOLID unsupported and creates no shape; this is a consumer limitation, not a
converter defect. `FreeCAD/main` contains a newer SOLID callback, but no
runtime claim transfers across revisions. `3DFACE` remains unmapped in the
audited current dispatcher. S8.15.18's separate pinned FreeCAD 1.1.3 runtime
also confirms the C++ importer reports one 3DFACE unsupported and creates no
shape; the legal converter record and WCS fields remain intact. Do not promote
FreeCAD geometry support or rewrite the entity. S8.15.13 now qualifies one ordinary default-XY
closed LWPOLYLINE through the same
DWG→`dwg2dxf`→DXF-readback→pinned-FreeCAD C++ route. It is deliberately not 3D
support evidence. The desktop dispatch smoke is already committed for the
pinned macOS LINE profile; keep its GUI gate separate from entity semantics
and rerun only when integration behavior or a claimed platform changes. Neither
gate should stall independent family work. The
condominium sample's
SPLINE observation remains ODA-fallback evidence only, and the `dwgadd`
attempt remains a generator limitation. S8.15.7 instead qualifies one
locally-authored source through ODA DWG generation, this `dwg2dxf`, DXF
readback and the pinned FreeCAD C++ importer; it does not remove the need for
target-authored DWG/version witnesses. S8.15.8 separately validates a locally
authored rational 3D HELIX through DXF→ODA DWG→exact converter argv→DXF
readback, and fixes weighted-spline group-70 semantics. The current FreeCAD
`dxf.cpp` entity dispatcher in the 2026-09-23 `main` snapshot explicitly maps
SPLINE to `ReadSpline()` and sends unlisted HELIX to `ReadUnknownEntity()`;
generic spline construction in `ImpExpDxf.cpp` does not cover this separate
entity. Thus the converter lane is verified, while FreeCAD `main` HELIX
geometry is downstream-unsupported in that audited source revision and
requires a separate FreeCAD importer change. The pinned 1.1.3 runtime is a
distinct target and may record its own warning/object outcome; neither result
is generalized to the other revision or used as a reason to rewrite the
entity. S8.9.1 verifies the installed CLI and documents FreeCAD setup;
S8.9.4 now states the import-only boundary and shared-preference/sibling
behavior in README/man because this repository does not build FreeCAD's
opposite-direction `dxf2dwg`. Current `Draft/importDWG.py` source has a dedicated
LibreDWG lane that uses the configured converter preference or platform-aware
PATH lookup, while automatic mode may fall back to ODA/QCAD. S8.9.2 is
now qualified on pinned macOS arm64 FreeCAD `freecadcmd` for PATH discovery,
configured direct-path discovery, and shared-preference sibling derivation.
All three use the installed `dwg2dxf`, exact argv and imported-DXF handoff,
and the independent three-LINE bounds; the Windows `.exe` lane remains
unqualified. S8.9.3 separately passes the headless `insert()`/`Import.readDXF`
handoff and target-document-content check for planar LINEs; S8.15.15 adds an
independent nonzero-Z LINE through the same `insert()` route. Both optional runs use a fresh
`--user-cfg` under a system-temp root; preference seeding verifies FreeCAD
actually loaded that exact file before any writes. Since FreeCADCmd may return
zero after reporting a macro exception, only the explicit PASS marker counts
as success. These headless importer lanes are qualified, but they do not prove
GUI-process registration/environment behavior. S8.9.6 launches the bundle's
GUI executable directly (not through LaunchServices) to preserve the isolated
environment and calls the registered importer dispatcher from inside the GUI
process. Its `dwg2dxf_freecad_desktop_open_insert` CTest passes 1/1 after
disabling the default modal import-options dialog in isolated preferences. It
verifies converter identity/argv, open and insert handoffs, and expected LINE
geometry. Windows/Linux, dialog UX, and viewport remain unqualified.

S8.15.16 extends this consumer lane for the AC1015 modeler carrier without
overstating FreeCAD's importer: the bounded SAT-v1 path is verified through
local extraction and DXF readback. An opt-in CTest now runs a caller-supplied
external `Cone.dwg` through the exact FreeCAD command form and compares the
`3DSOLID` SAT text against a caller-supplied LibreDWG DXF reference; the
30-pair comparison passes. S8.15.17 now reuses the isolated FreeCAD import
macro and profiles for both `Draft.importDWG.open()` and `insert()`, using the
installed `dwg2dxf` through PATH and requiring exact argv/status/output
handoff. On pinned FreeCAD 1.1.3 C++ importer mode 2 / macOS 27 arm64, each
route reads the one AC1015 `3DSOLID`, reports it unsupported, and creates no
shape; the converter output still matches all 30 SAT group-1/3 reference
pairs. This closes the narrow FreeCAD converter-integration check, not
FreeCAD solid geometry support. The input and independent DXF reference stay
outside the repository; their temporary copies, profiles, and outputs are
removed after passing tests. Desktop dispatch for this entity, target-authored
DWGs, and positive B-rep semantics remain unqualified. A locally generated
full-DWG witness is optional only if an existing independent writer accepts a
locally authored SAT carrier; do not build a new ACIS/DWG encoder for test
convenience or block other work on it. S8.15.12's distinct `SOLID` result is
not evidence for `3DSOLID` importer behavior.

FreeCAD integration is an additional bounded S8 consumer lane, not a new
format-support claim. The exact converter invocation, four-revision planar
`open()` LINE smoke matrix, and failure-safe output publication are implemented.
Treat this lane as two independent contracts: first, the installed
`dwg2dxf` must honor FreeCAD's exact `dwg2dxf <input> -o <output>` process
contract and preserve standard DXF fields through readback; second, a pinned
FreeCAD importer must create geometry with independently expected semantics.
Report these outcomes separately. Do not distort otherwise-correct DXF fields
to compensate for a downstream importer that ignores elevation, OCS, topology,
or another standard field. A feature is FreeCAD-supported only for the pinned
release/revision, platform, importer mode, converter-discovery route, and
tested DWG→DXF→import entry point; a pass through `open()` does not qualify
`insert()`, GUI registration, another importer, or another platform.
Current FreeCAD source routes `Draft.importDWG.insert()` through the same
converter helper but hands off to `importDXF.insert()`; that separate runtime
boundary is now S8.9.3 and is not inferred from the existing `open()` tests.

FreeCAD-specific implementation contract and acceptance order:

1. Treat FreeCAD as an external process consumer of the installed `dwg2dxf`,
   not a library/API integration. Test the executable FreeCAD actually resolves:
   its `TeighaFileConverter` preference or platform `PATH` lookup (the program
   name is `dwg2dxf.exe` on Windows and `dwg2dxf` on Linux/macOS). Record the
   resolved real path and binary hash. Exercise paths containing spaces; keep
   argument boundaries as an argv vector, never shell-concatenate a command.
2. Match the current Draft/importDWG.py invocation exactly:
   `[dwg2dxf, input.dwg, "-o", output.dxf]`. Do not require `-v`, `-y`, or
   other switches FreeCAD does not pass. Honor the existing converter version
   policy and preserve valid source/entity fields; validate the result through
   the public DXF reader. Because the wrapper's LibreDWG lane treats output-file
   existence as success after `communicate()` rather than checking the process
   return code, keep publication atomic: failed conversion must not leave a
   plausible final DXF. Exercise no-output failure with malformed input.
3. Qualify `Draft.importDWG.open()` and `insert()` as separate routes. For each,
   assert exactly one expected converter invocation, status and output path,
   and that FreeCAD receives that same DXF; for `insert()`, also assert the
   target document. Test the C++ and legacy Python DXF importers independently
   only when both are actually installed. An absent optional importer/addon is
   an explicit unqualified lane, not a reason to download or mutate a user's
   FreeCAD installation.
4. Separate converter compatibility from importer semantics. A valid DXF,
   matching record/field readback, and correct handoff can pass while FreeCAD
   reports an entity unsupported or silently constructs wrong geometry. Only
   promote geometry for a pinned FreeCAD commit/release, platform/architecture,
   importer mode/settings, discovery route, entry point, and independently
   checked geometry. Pin all preferences in an isolated profile and disable
   ODA/QCAD fallbacks when attributing a result to this converter.
5. **Prioritize end-to-end FreeCAD feature rows.** Select the next
   FreeCAD-facing entity slice from the existing S8.15 outcomes and the
   actually selected FreeCAD DXF importer. For a positive feature claim,
   exercise the installed converter through both `Draft.importDWG.open()` and
   `insert()` when both operations are claimed, and compare FreeCAD's created
   geometry with independent expected coordinates/topology. Pin C++ versus
   legacy Python importer, release/revision, platform, discovery route,
   preferences, and operation. Keep a standards-correct converter/readback
   result even when FreeCAD reports the entity unsupported or silently
   constructs different geometry; label that row converter-only or
   downstream-limited, and do not add output substitutions, entity flattening,
   or a FreeCAD patch to disguise it. Promote neither the other importer mode
   nor another operation/platform from that result.
6. Keep the implementation loop fast: run the entity-specific CLI/readback
   CTest and focused unit test in the normal slice; keep FreeCAD runtime and
   desktop-dispatch tests opt-in and run them only for changed converter/import
   behavior or the exact consumer claim being added. Leave other-platform,
   other-release, GUI/file-dialog, viewport, and unavailable-importer checks
   explicitly deferred, and continue unrelated ready slices.

S8.15.18.1 now applies item 3 to a negative tilted-3DFACE witness: the fast
converter/readback test remains separate from the two pinned FreeCAD runtime
operations, and `open()` and `insert()` each assert the installed executable,
exact argv, same-output handoff, importer limitation, and (for insert) target
document. Both operations produce the same bounded result on the pinned
FreeCAD 1.1.3/C++/macOS profile: one standard 3DFACE is passed through, then
reported unsupported with no created shapes. This proves converter integration
for those two routes only; it is expressly not FreeCAD 3DFACE geometry support.

S8.15.18.2 adds a source-from-scratch 3DFACE DWG conversion matrix for AC1018,
AC1021, AC1024, AC1027, and AC1032, complementing the existing AC1015 case.
ODA File Converter generates each version only under the ignored build tree;
the opt-in tests require the DWG magic and converted `$ACADVER` to agree, then
check one tilted 3DFACE's twelve WCS coordinates and group-70 invisible-edge
flag after `dwg2dxf` and public DXF readback. When `dwgread` is available, its
independent DWG read/export must preserve the same version and fields too. On
2026-09-24, ODA File Converter 27.1.0.0 plus LibreDWG 0.14 `dwgread` passed
all six AC1015-through-AC1032 controls (6/6; 41.06 seconds including external
tool startup). This is a bounded local-writer semantic matrix for this entity;
it does not establish target-authored DWGs, every class/page delta, broad
version support, or FreeCAD 3DFACE geometry. Keep ODA/LibreDWG controls
opt-in, and leave generated DWG/DXF files out of source control.

S8.14 adds one locally generated AC1015 nonzero-Z LINE control, which FreeCAD
1.1.3's default C++ importer converts to one valid B-rep edge with endpoints
`(1,2,3)` and `(4,6,9)`. This demonstrates this generated vector through
the configured converter/importer path, not target-authored DWG interoperability
or broader family support. S8.15.1 adds an isolated AC1015 3D POLYLINE control;
the same pinned FreeCAD importer creates one valid shape with two edges and
preserves each expected vertex. S8.15.2 isolates legacy mesh and polyface:
the converter/readback retains their proper `POLYLINE` subtype/count/vertex/
face data, but FreeCAD's 1.1.3 C++ importer turns a 2×2 mesh into a 3-edge
chain (not grid topology), turns a one-face PFACE into a wire with a wrong
origin-based closing edge, and throws on a two-face PFACE. These findings
remain downstream limitations, not reasons to flatten or omit typed DXF
records. S8.15.3 adds one nonzero-Z `POINT`; FreeCAD creates one valid
vertex-only shape at `(10,20,30)` with the point-import setting enabled.
S8.15.3.1 adds a separate AutoCAD-authored AC1027 POINT `(50,50,50)` witness:
the exact installed converter route and DXF readback preserve `$ACADVER`, and
the pinned FreeCAD importer creates one vertex at the expected coordinates.
This remains one target-authored entity/version/runtime tuple, not family-wide
support. S8.9.10 also protects declared AC1027 on DXF re-readback without
altering `dxfRW::getVersion()` semantics. The
ARC/CIRCLE slice additionally preserves a 0–90-degree radius-3 ARC centered at
`(20,30,40)` with the expected directed endpoints, and a closed radius-5
CIRCLE centered at `(10,20,30)`. FreeCAD reports the circle curve axis as -Z;
the full circle has the expected XY locus and is geometrically orientation-
equivalent. These samples do not cover oblique OCS normals. The bounded shape
output is opt-in and does not require FreeCAD at CTest time: the `.dwgadd`
controls require LibreDWG's generator, while S8.15.6's independent-writer
INSERT control requires ODAFileConverter; both use this `dwg2dxf`. The 3D
POLYLINE's four zero-length-extrusion warnings and the topology controls'
repeated warnings are recorded separately from semantic geometry outcomes.
S8.15.13 establishes one closed, flat XY LWPOLYLINE baseline. S8.15.14 adds a
closed 2D POLYLINE whose OCS has elevation 5 and extrusion `(0,0.6,0.8)`:
ODA→`dwg2dxf`→DXF readback preserves its closed flag, elevation, normal, and
ordered OCS vertices, but pinned FreeCAD 1.1.3 C++ mode 2 silently imports the
vertices at their raw XY coordinates with Z=0. This is a downstream consumer
limitation, not justification to rewrite the converter output into WCS or
drop the normal. A FreeCAD OCS fix belongs in a separately reviewed importer
change and must be retested against the same exact generated control.
S8.13 adds an
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

S8.15.5 now makes route attribution explicit in the optional macros. Positive
assertions disable ODA/QCAD fallback discovery and require one successful
`dwg2dxf` process whose exact `-o` output is the path passed to FreeCAD's C++
DXF importer. The fallback-enabled feature audit records every converter argv,
exit code, candidate output/existence, configured converter preference, and
the exact final path supplied to `importDXF.open()`. This was verified against
the user-owned AC1024 condominium sample: `dwg2dxf` returned 1 with no output;
FreeCAD then invoked ODAFileConverter (return 0) and imported that ODA-produced
DXF. Although it contains one SPLINE record and FreeCAD reports one SPLINE,
this is ODA evidence only. The sample and all converter products remain
unstaged; this fallback audit itself promotes no support claim. S8.15.7 below
is a separate generated, route-attributed SPLINE control.

S8.15.6 adds one locally authored DXF source and an opt-in independent-writer
control: ODAFileConverter 27.1.0.0 writes an AC1015 DWG only in the build tree, then
libdxfrw's `dwg2dxf` must preserve the custom block, 3D LINE, INSERT placement,
scale, and rotation through a DXF readback. The fast CTest passes. FreeCAD
1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64,
default C++ mode 2, resolves this `dwg2dxf`, imports that exact output as one
`App::Link` and one valid edge, and matches the independently transformed
endpoints `(4,22,42)` and `(-8,28,66)`; the opt-in runtime assertion passes.
The expected values are computed by component scale, +90-degree Z rotation,
then translation, not inferred from converter readback. Only the source DXF
is committed; generated DWG/DXF products remain in build/temp. This does not
qualify other INSERT variants, MINSERT, attributes, nested/nonzero-base blocks,
oblique normals, other versions/importers, or target-authored DWGs. Do not
use the failed `dwgadd` ownership control or its shipped broken-attribute
example as parser evidence.

S8.15.7 adds one locally authored DXF source with nonplanar control-point,
oblique planar control-point, nonplanar fit-point, and near-planar-within-
tolerance SPLINEs. The DWG
converter previously set the planar bit unconditionally for both DWG spline
scenarios; because the DWG fields do not store DXF planarity metadata, it now
derives a normal from all available geometry and conservatively omits planar
metadata when the points/tangents do not establish a plane. The fast optional
ODA CTest passes: generated AC1015 output preserves all four SPLINEs, the
correct planar flag/normal, clear flags/normals for clearly and nearly
nonplanar geometry, XYZ data and fit tangents through
`dwg2dxf <input> -o <output>` and DXF readback. FreeCAD 1.1.3 revision
`145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ mode
2, resolves the exact binary on PATH and imports its exact successful output
as four valid one-edge shapes with correct endpoints; the control-curve
midpoints match independently evaluated cubic Bezier values and no SPLINE is
reported unsupported. Runtime assertions pass. The source, converter output,
and importer handoff are hashed/attributed; generated DWG/DXF stays in
build/temp. This does not qualify weighted/rational, closed, periodic,
degenerate, other-knot, other-scenario, target-authored, or other-version
splines.

Current implementation-item ledger (update in every corresponding slice
commit; 102/116 committed, 13 blocked, 0 verified, 1 in progress, and 0 ready):

| Item | State | Evidence / next action |
| --- | --- | --- |
| S8.9.13 | COMMITTED | A shared macOS install reproduced FreeCAD's installed-child loader failure: `dwg2dxf` referenced `@rpath/libdxfrw.2.dylib` but had no `LC_RPATH`. `dwg2dxf/CMakeLists.txt` now sets an install-relative `@loader_path`/`$ORIGIN` path to `CMAKE_INSTALL_LIBDIR` for shared Apple/Unix builds, while static builds and Windows' colocated runtime DLL layout remain unchanged. Added `dwg2dxf_freecad_installed_cli_compat` and `tests/run_freecad_installed_dwg2dxf_test.cmake`: install the build under a unique prefix/path containing spaces, reject absolute or escaping GNUInstallDirs destinations and build-tree executable substitution, then reuse the existing exact-argv converter checks for the three repository-tracked DWG controls with `DYLD_*`/`LD_*` loader overrides unset and a minimal system PATH. The static macOS CTest passes 1/1 (0.49 s); a separate `BUILD_SHARED_LIBS=ON` macOS build passes the same test 1/1 (0.62 s). Direct `otool` inspection confirms the installed shared executable has `@loader_path/../lib` and links only the installed libdxfrw plus macOS system libraries; `env -i PATH=/usr/bin:/bin` converts tracked AC1027 input successfully. Freshly installing the current static build and using the runner's unchanged strict hash mode also passes FreeCAD 1.1.3/macOS 27 arm64 `open()` and `insert()` (C++ importer mode 2, `DWGConversion=1`), with exact argv/handoff, one LINE at `(1,2,3)-(4,6,9)`, and no unsupported features; installed/build SHA-256 is `2ce17276b418bc5d3e7417e2d7642bcc0eea7b0535c8d0a8bf027ea822035b1f`. A separately configured stale install hash (`54c9a6...`) was correctly refused, so the shared-only exception has not weakened the default identity guard. Tests use the tracked local `.dwgadd` recipe and existing DWG controls; no drawing fixture is added. This is installed CLI/dependency-resolution evidence for macOS only; S8.9.14 covers the pinned shared FreeCAD process tuple, and native Linux/Windows remain S8.9.5. |
| S8.9.14 | COMMITTED | Added default-OFF `LIBDXFRW_ENABLE_FREECAD_SHARED_INSTALLED_3D_LINE_CONTROL` and `tests/run_freecad_shared_installed_3d_line_test.cmake`, which install the current shared build into a unique prefix and reuse the existing isolated FreeCAD LINE harness. The FreeCAD runner retains its strict installed-vs-build byte-identity check by default; only this wrapper can permit the expected install-RPATH hash rewrite, and only when the executable resolves inside the prefix it just installed from the current build. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, isolated preferences, `DWGConversion=1`, C++ importer mode 2: both `dwg2dxf_freecad_shared_installed_3d_line_open` and `_insert` pass (2/2, 2.39 s combined) outside the sandbox. Both resolve installed `dwg2dxf` by PATH (SHA-256 `ffe65184f05da5be8f27fd697f27fd23cdd0a48cd87858c79e5d9042059cc443`), invoke exact `[binary,input,-o,output]`, exit zero, hand the same DXF path to `importDXF.open()` / `insert()`, and create one valid LINE with bounds `(1,2,3)-(4,6,9)` and no unsupported features; `insert()` targets `FreeCADDwgInsertCheck`. The sandboxed attempt aborts before app code because Qt cannot see host `neon`; the same tests pass outside the sandbox. The AC1015 DWG is generated from the tracked local `.dwgadd` recipe into system temp, and isolated install/output roots are removed on success; no drawing fixture is added. This qualifies only the shared installed converter's pinned macOS open/insert LINE handoff, not other features/platforms or general shared FreeCAD support. Native Linux/Windows qualification remains S8.9.5. |
| S8.9.12 | COMMITTED | Generalized the shared installed-LINE FreeCAD macro/runner for Automatic mode (`DWGConversion=0`), preserving actual ODA/QCAD resolver behavior and requiring exactly one child process. Added the default-OFF `LIBDXFRW_ENABLE_FREECAD_AUTOMATIC_3D_LINE_CONTROL` with separate open/insert CTests; no drawing fixtures were added and generated DWGs stayed in temporary roots. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, isolated profile/PATH: both Automatic operations select installed `/private/tmp/libdxfrw-freecad-auto.m85IDP/bin/dwg2dxf` (SHA-256 `54c9a6fedd8a4dd8ef59e3490b1a730f68e0defa70e665fb322dbc8c2757d00f`) exactly once, use `[binary,input,-o,output]`, exit 0, hand the same AC1015 DXF to the importer, create one LINE with bounds `(1,2,3)-(4,6,9)`, and report no unsupported entities. No ODA/QCAD process runs. Existing LibreDWG-only `open()`/`insert()` controls still pass 2/2; combined modes pass 4/4. Fast version-policy/CLI CTests pass 2/2 and the `dwg2dxf` build passes. FreeCAD aborts before app code inside the sandbox due to Qt's hidden host `neon` feature; all four runtime checks pass outside it. This closes only the Automatic-mode selection/handoff for this pinned macOS LINE profile; native Linux/Windows and general entity/3D support remain unqualified. README and man page record the bounded result and continuing fallback semantics. |
| S2.2.4 | COMMITTED | Added a DXF write-context map from unique modeler source handles to their final emitted handles. During opaque ACDSDATA replay, only a group-320 key inside an `ACDSRECORD`'s `AcDbDs::ID` subsection is rewritten, and only when that same record has one `ASM_Data` marker and the source uniquely maps to a written modeler entity. Unrelated code-320 values remain byte/spelling-preserved. Malformed, missing, duplicate, ambiguous-source, and orphan associations fail the output transaction. Runtime-generated ASCII/binary `dx_iface` exports now check both first- and second-pass reads and assert the key equals the public modeler handle; the exact FreeCAD-argv CLI control (`input -o output`) uses a local AC1032 DXF, checks both conversion passes and the unrelated opaque key, and passes. Enabling the binary second pass exposed DIMSTYLE boolean group codes 290/295 being serialized as 16-bit integers; parsing validates their 0/1 domain, writing uses the one-byte boolean representation for static and dynamic variables, and writer preflight rejects invalid direct-field/dynamic values before publication. Runtime-generated ASCII/binary checks reject -1 and 2 for both codes; focused CTest `libdxfrw_dwg_local_roundtrip` and `dwg2dxf_freecad_cli_compat` pass 1/1 each. No DWG/DXF fixture was committed. This repairs DXF→DXF association and a binary table-width defect only; DWG DataStorage serialization, ACDSDATA schema validity, ODA acceptance, and FreeCAD modeler geometry remain blocked/unqualified. |
| S5.11 | COMMITTED | `dxfRW::writePolyline()` now omits group 210/220/230 for WCS 3D POLYLINE, polygon mesh, and polyface forms, so a zero-initialized DWG `extPoint` cannot become a zero-length DXF extrusion. ASCII and binary round-trip vectors cover flags 8/16/64, no explicit extrusion after public readback, a nonzero-Z polygon-mesh vertex, and a classic planar OCS POLYLINE retaining its non-default vector `(0,0.6,0.8)`. The exact FreeCAD `dwg2dxf input -o output` CTest rejects any 210/220/230 tuple on the WCS parent. `dwg2dxf_freecad_cli_compat`, `dwg2dxf_freecad_3d_polyline_cli`, `libdxfrw_dwg_local_roundtrip`, and `libdxfrw_3d_consumer_probe` pass (4/4); `lc3_compat_check` builds. The pinned FreeCAD 1.1.3/macOS 27 arm64/C++ importer mode 2 `open()` check imports the expected two edges and XYZ endpoints with no zero-length-extrusion warnings. No generated DWG/DXF fixture was added. This is bounded writer and one FreeCAD consumer-profile evidence only; broader POLYLINE/DWG/platform claims remain unchanged. |
| S5.12 | COMMITTED | ARC/CIRCLE DXF reads reject zero/non-finite extrusion directions before callback publication; ASCII and binary entity writers reject zero directions before emitting any record bytes. Hardening vectors cover both entities in `ext=false` and `ext=true`, accept omitted default +Z, and preserve a finite oblique normal `(0.6,0,0.8)` plus independently expected `ext=true` center `(0,2,5)` without changing the existing OCS policy. `dwg2dxf` and `libdxfrw_hardening_tests` build; focused `libdxfrw_hardening`, `dwg2dxf_freecad_cli_compat`, and `libdxfrw_3d_consumer_probe` pass 3/3; `git diff --check` passes. No fixture files were added. Autodesk's ARC/CIRCLE references define OCS centers and optional extrusion direction defaulting to +Z. This is malformed-normal rejection and field-preservation evidence only; it does not repair FreeCAD's downstream oblique ARC/CIRCLE interpretation, qualify a malformed DWG sample, or promote general ARC/CIRCLE or platform support. |
| S8.12 | COMMITTED | Extended `tests/run_freecad_dwg2dxf_compat_test.cmake` with a runtime-generated malformed DWG. The exact `-o` invocation fails nonzero without publishing a final DXF; the same failure with `-y` preserves an existing sentinel, and no `.libdxfrw-*` output temp remains. Added a UTF-8 input/output path case, which passes on this macOS host; Windows is explicitly skipped because narrow `main(argc, argv)` encoding needs native qualification. Existing writer-primitives tests independently cover transactional publish/rollback and destination preservation. `cmake --build build --target dwg2dxf libdxfrw_writer_primitives_tests` passed; focused CTest `dwg2dxf_version_policy`, `dwg2dxf_freecad_cli_compat`, and `libdxfrw_writer_primitives` passed 3/3; `git diff --check` passed. No fixtures added. The converter now has tested failure-safe publication through FreeCAD's file-existence check on this host; Windows Unicode paths remain unqualified. |
| S8.13 | COMMITTED | Fixed typed DXF pass-through in `dwg2dxf/dx_iface`: preserve derived RTEXT/ARCALIGNEDTEXT/MPOLYGON objects and dispatch to their specialized writers rather than generic TEXT/HATCH or omission. `tests/run_freecad_dwg2dxf_compat_test.cmake` now invokes exact FreeCAD argv on tracked `rtext_arctext.dwg` and `mpolygon_solid.dwg` and requires RTEXT, ARCALIGNEDTEXT, and MPOLYGON records. `tests/dwg_fixture_tests.cpp` checks DWG→DXF→DXF subtype and stable payload/radius/solid/fill fields. Added opt-in `tests/freecad_dwg2dxf_feature_audit.FCMacro` to record source/output hashes, converter path, FreeCAD/importer settings, record counts, unsupported reports, and created object types. FreeCAD 1.1.3 (rev 20260725), macOS 27 arm64, default C++ importer / converter from PATH: MPOLYGON 1, RTEXT 1, ARCALIGNEDTEXT 1, and DIMENSION 1 are emitted; FreeCAD reports MPOLYGON, RTEXT, ARCALIGNEDTEXT and dimension type 4 unsupported (0 entity objects for these rows). The existing four AC1015/AC1018/AC1021/AC1027 LINE imports remain the only positive FreeCAD import subset. `cmake --build build --target dwg2dxf libdxfrw_dwg_fixture_tests` passed; focused CTest `libdxfrw_dwg_fixtures`, `dwg2dxf_freecad_cli_compat`, and `dwg2dxf_version_policy` passed 3/3; `git diff --check` passed. No fixtures were added. Optional legacy Python import, GUI/rendering, and general feature support remain unqualified; next add matrix rows only with independent expected fields and an established importer mode. |
| S8.14 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_3d_line_control.dwgadd`, optional `LIBDXFRW_ENABLE_DWGADD_FREECAD_CONTROL` CTest and `tests/freecad_dwg2dxf_3d_line_check.FCMacro`. The fast test uses LibreDWG 0.14 `dwgadd` to create an AC1015 DWG only in the build tree, invokes exact FreeCAD argv (`dwg2dxf input -o output`), verifies ASCII `$ACADVER`, exactly one LINE with endpoints `(1,2,3)`/`(4,6,9)`, and repeats through libdxfrw DXF readback. Optional real runtime passed on FreeCAD 1.1.3 revision `145529e` / macOS 27 arm64 / default C++ importer mode 2: `Draft.importDWG.open()` resolved this build's `dwg2dxf` via `PATH`, imported exactly one LINE and one valid B-rep edge, matched both endpoint XYZ tuples, and reported no unsupported features. `cmake --build build --target dwg2dxf libdxfrw_dwg_fixture_tests libdxfrw_dwg2dxf_version_tests` passed; focused CTest (`libdxfrw_dwg_fixtures`, `dwg2dxf_version_policy`, `dwg2dxf_freecad_cli_compat`, `dwg2dxf_freecad_3d_line_cli`) passed 4/4; `git diff --check` passed. Revalidated `dwg2dxf_freecad_3d_line_cli` on 2026-09-24 (1/1 pass). DWG/DXF outputs stayed under ignored `build/` or temporary paths; the only committed sample artifact is the locally authored recipe. This is a generated route control, not AutoCAD-authored DWG interoperability or general LINE/FreeCAD 3D support. |
| S8.15.1 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_3d_polyline_freecad_control.dwgadd`, optional `dwg2dxf_freecad_3d_polyline_cli` CTest, and `tests/freecad_dwg2dxf_3d_polyline_check.FCMacro`. The CTest generates an AC1015 DWG in the build tree, invokes exact FreeCAD argv, verifies one POLYLINE with 3D flag, three ordered coordinate VERTEX records and SEQEND, then repeats those checks after libdxfrw DXF readback; it passes 1/1. FreeCAD 1.1.3 revision `145529e` / macOS 27 arm64 / C++ importer mode 2 resolved this build's converter via PATH and created one valid shape with the two expected edges `(0,0,1)-(2,3,4)` and `(2,3,4)-(5,1,-2)`, with no unsupported entities. The earlier four `Entity has zero-length extrusion direction` warnings are resolved by S5.11: the WCS parent no longer emits a meaningless extrusion tuple, and the pinned runtime check now completes without that warning while retaining the expected geometry. The mixed topology experiment separately logged an unknown entity-read exception and cannot identify a subtype. No generated DWG/DXF or foreign sample was committed; only the local-from-scratch recipe is tracked. This is a generated control and one pinned FreeCAD profile, not AutoCAD-authored DWG interoperability or all classic POLYLINE subtype support. |
| S8.15.2 | COMMITTED | Added locally authored AC1015 MESH, single-face PFACE and two-face PFACE `.dwgadd` controls, optional `dwg2dxf_freecad_3d_mesh_cli` / `...pface_cli` / `...pface_multiface_cli` CTests, and environment-gated bounded shape-detail output in `tests/freecad_dwg2dxf_feature_audit.FCMacro`. Exact FreeCAD `dwg2dxf input -o output` conversion and DXF readback retain one MESH POLYLINE (flag/dimensions 2×2/four XYZ vertices), one PFACE POLYLINE (four XYZ vertices/one face-record vertex/indices 1,2,3,4), and the two-face PFACE's five XYZ vertices/two face records/indices. All three new CTests pass; combined with the existing LINE/POLYLINE CLI tests and `libdxfrw_dwg_local_roundtrip`, focused CTest passes 6/6; no FreeCAD dependency enters default CI. FreeCAD 1.1.3 full revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ mode 2: mesh creates one valid `Part::Feature`, but has 3 sequential-chain edges and no faces instead of 2×2 grid topology; single-face PFACE creates a valid 4-edge wire/no faces and erroneously connects `(0,0,0)` to `(0,3,4)`. Tracked two-face PFACE converter output triggers `CDxfRead::ReadEntity` unknown exception and creates no shape; isolated signed-quad and triangle controls import, so the observed failure is multi-face. Mesh/single-face controls print 5/6 zero-length-extrusion warnings. Converter semantics are preserved, but these FreeCAD topology results are explicitly unsupported; generated DWG/DXF outputs remain in build/temp and no external or binary fixture was committed. `git diff --check` passes. This generated evidence promotes no target-authored DWG interoperability or general FreeCAD 3D claim. |
| S8.15.3 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_3d_point_freecad_control.dwgadd`, opt-in `dwg2dxf_freecad_3d_point_cli` exact-argv converter/readback CTest, and `tests/freecad_dwg2dxf_3d_point_check.FCMacro`. The fast CTest generates DWG in the build tree and checks one AC1015 POINT at groups 10/20/30 `(10,20,30)` after conversion and DXF readback. FreeCAD 1.1.3 full revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2 with `Import points=Yes`, resolves this `dwg2dxf` through PATH and creates exactly one valid `Part::Feature` containing one vertex `(10,20,30)`, no edges/faces, and no unsupported report. Runtime assertion macro passes. This is one generated control and importer profile only; not a general POINT/DWG-version or target-authored interoperability claim. Generated DWG/DXF outputs remain in build/temp; no binaries were committed. |
| S8.15.4 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_arc_circle_freecad_control.dwgadd`, optional `dwg2dxf_freecad_3d_arc_circle_cli` exact-argv conversion/readback CTest, and `tests/freecad_dwg2dxf_3d_arc_circle_check.FCMacro`. Fast test preserves AC1015, one ARC (center `(20,30,40)`, radius 3, 0–90°) and one CIRCLE (center `(10,20,30)`, radius 5) through `dwg2dxf input -o output` and DXF readback; it passes. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ mode 2: two valid `Part::Feature` edges, ARC center/radius `(20,30,40)`/3, +Z axis and ordered endpoints `(23,30,40)`→`(20,33,40)`; CIRCLE center/radius `(10,20,30)`/5 and closed XY-plane locus. FreeCAD's full-circle curve internally reports -Z axis, recorded as orientation-equivalent for an undirected closed circle. No unsupported features. The runtime assertion passes. Only default-normal elevated controls are qualified; an exploratory non-default `arc.extrusion` assignment was ignored by LibreDWG dwgadd 0.14, so oblique OCS stays unqualified and requires a trustworthy sample/generator before testing. Combined focused CTest (`libdxfrw_dwg_local_roundtrip`, LINE, POINT, 3D POLYLINE, MESH, one-/multi-face PFACE and ARC/CIRCLE CLI controls) passes 8/8. Generated DWG/DXF remains in build/temp. This promotes no AutoCAD-authored interoperability, oblique OCS, or general FreeCAD 3D claim. |
| S8.15.5 | COMMITTED | The feature audit now records configured `DWGConversion`, every converter argv/return code/candidate output, whether each output exists, and the exact DXF path passed to `importDXF.open()`. Positive LINE/POINT/3D-POLYLINE/ARC-CIRCLE macros disable ODA/QCAD fallbacks and assert exactly one successful `dwg2dxf <input> -o <same imported DXF>` invocation. Focused fast CTest for those four generated controls passes 4/4; all four optional FreeCAD runtime assertion macros pass on FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ mode 2. The fallback-enabled audit against the user-owned AC1024 condominium sample records `dwg2dxf` return 1/no output, then ODAFileConverter return 0 and its output as the actual importer input. Its SPLINE observation is therefore not attributed to libdxfrw. No sample/output was staged or committed; no SPLINE or wider support claim is promoted. |
| S8.15.6 | COMMITTED | Added the single locally authored `tests/fixtures/dxf/ac1015_insert_freecad_control.dxf` source plus an opt-in ODAFileConverter 27.1.0.0 CTest and `tests/freecad_dwg2dxf_3d_insert_check.FCMacro`. The fast test generates an AC1015 DWG in the build tree, invokes exact FreeCAD argv, checks the custom block / 3D LINE / INSERT point, scales and 90-degree rotation, then repeats field checks after libdxfrw DXF readback; it passes. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ mode 2 imports the exact successful `dwg2dxf` output with fallbacks disabled, creates one valid `App::Link` edge, and matches independently calculated endpoints `(4,22,42)` and `(-8,28,66)`; the runtime assertion passes. Generated DWG/DXF files stay in the build tree. This is one locally authored vector through an independent DWG writer and one pinned importer profile, not target-authored interoperability or general INSERT/MINSERT support. A broken `dwgadd` ownership recipe was discarded; do not weaken parser ownership, and do not use LibreDWG issue #1351's broken attribute-chain sample as a clean witness. |
| S8.15.7 | COMMITTED | Added the locally authored `tests/fixtures/dxf/ac1015_nonplanar_spline_freecad_control.dxf`, opt-in ODAFileConverter 27.1.0.0 AC1015 writer/converter-readback CTest, and `tests/freecad_dwg2dxf_3d_spline_check.FCMacro`. Corrected DWG SPLINE conversion: ODA v5.4.1 §20.4.40 scenarios store control geometry or fit geometry/tangents, not DXF planarity metadata; derive a normal only from complete planar geometry using numeric precision checks, without treating spline fit/control tolerances as permission to relabel nonplanar curves. The CTest passes 1/1 and verifies four SPLINEs, degree/counts, all XYZ control/fit points, fit tangents, clear planar flags/normals for clearly nonplanar cases and for a control spline deviating `1e-8` from a plane within its `1e-7` control tolerance, and expected bit-8 flag plus unit normal `(1,2,-1)/sqrt(6)` for the oblique planar case, both before and after DXF readback. FreeCAD 1.1.3 full revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ importer mode 2: with fallbacks disabled, exactly one successful PATH-resolved `dwg2dxf <input> -o <same imported output>` process yields four valid one-edge shapes and matching endpoints; the two control-spline midpoints equal independent cubic-Bezier evaluations `(5.5,4.25,12.125)` and `(1.25,0.75,2.75)`; no SPLINE is unsupported. The macro records source/output hashes, argv/status, runtime identity/settings and imported geometry. Generated DWG/DXF files stay in build/temp. This qualifies only these locally authored unweighted cubic controls and this AC1015 writer/runtime path; other spline forms, target-authored DWGs, versions and importer modes remain unqualified. |
| S0.1 | COMMITTED | Rebased onto `origin/master`. The last ancestry check before the S8.11 slice (HEAD `aa5a8fb`) found `origin/master` to be an ancestor of `HEAD` (zero behind, 44 local commits ahead); subsequent plan/code slices are local branch commits. Existing user-owned untracked paths remain untouched. |
| S0.2 | COMMITTED | Resolved the authoritative local ODA v5.4.1 PDF at `/Users/dli/doc/dwg/OpenDesign_Specification_for_.dwg_files (1).pdf`; title/version/page count (279) match the official download. Read §§20.4.40 SPLINE and 20.4.41 REGION/3DSOLID/BODY, plus Chapter 8 and §9 for the R2018 organization/header boundary; continue reading each relevant section immediately before any DWG parser change. Authority lookup is unblocked; this alone does not qualify unlisted layouts. |
| S0.3 | COMMITTED | Recorded read/write versions and reader/writer lineage, source-visible routes for each 3D family, read-only legacy AC1009 behavior, the missing modeler DWG encoder, and unsupported/unqualified claim ceilings. Unknown class/layout/version identities remain explicitly unknown; this inventory is not interoperability qualification. |
| S0.4 | COMMITTED | AC1024 `visualization_-_conference_room.dwg` debug trace records class/type 524=`AcDbPlaneSurface`, DXF record `PLANESURFACE`, entity flag 1, two instances; successful conversion emits two `PLANESURFACE` records. This is class identity/dispatch evidence for this file/version only, not byte-layout or semantic qualification. No 3DLINE identity found. Candidate AC1018 `Extruder2.dwg` stops at Tables error 9 and AC1021 `dwgreader21_230.dwg` is too small (header error 5); neither supplied target-type evidence. |
| S0.5 | COMMITTED | Added path-specific ezdxf/LibreDWG compatibility evidence. Prior S5.1 LibreDWG 0.14 runs accept the exact generated 3DFACE/polyline/MESH DXF vectors. In this pass LibreDWG 0.14 omitted checked 3D families while exporting local AC1024/AC1027 target DWGs and rejected the generated surface/NURBS cases; these are not positives. S8.4a later compared a direct LibreDWG JSON read with libdxfrw on the same local AC1024 DWG and matched only six INSERT placement records and two scenario-2 SPLINE fit records; this is distinct from the earlier DXF-export coverage check and does not qualify other families. ezdxf 1.4.4 docs establish field/API scope and ACIS limitations, but no executable ezdxf run was possible because the package is not installed. LibreDWG 0.11 stability labels remain historical only. |
| S0.6 | COMMITTED | Full-text review of the local 279-page v5.4.1 on 2026-09-24 confirms §§20.4.40 SPLINE, 20.4.41 ACIS modeler, and §24 DataStorage. Chapter 8 covers R2018 and says its organization is structurally almost identical to R2013, listing three auxiliary-header shorts and a bounded set of entity/object deltas; §9 explicitly documents the R2018+ header-size field. The searchable text still has no named `AcDbSubDMesh`, modern `AcDb*Surface`, or `3DLINE` class layout; those class-specific DWG rows stay unqualified despite generic R2018 organization coverage. The spec does not fully decrypt ACIS and does not qualify pre-R13 forms. |
| S0.7 | COMMITTED | ODA v5.4.1 §§2.13, 20.4.44, and 20.4.45 plus LibreDWG 0.14 JSON for the untracked AC1024 conference-room DWG establish a valid named DICTIONARY entry with a null child reference. DWG parsing now accepts only a zero-reference/zero-counter handle with code 0 or the expected item-handle code; DICTIONARYWDFLT still requires its separate default handle. Runtime-generated regular/DWFDT dictionary vectors preserve null members; the local round-trip and hardening tests pass. DXF group-350 validation remains strict. The full 72-target build, `lc3_compat_check`, and 3D consumer probe pass. The spline integration checkpoint now reaches DXF output but is blocked on three unqualified E3DSOLID DWG-frame payloads in the sample's `Fluorescent Fixture` block; this does not qualify spline or modeler conversion. The local DWG was not staged. |
| S0.8 | COMMITTED | Re-read ODA v5.4.1 §24.2.2.7.1 against the R2013+ FreeCAD `dwg2dxf` ACDSDATA bridge. The DWG DataStorage schema-search table couples DataStorage-segment object handles to sorted indexes; the specification says AutoCAD ignores an entity if this search data is absent. §24.3's default schema dump is likewise DWG DataStorage evidence, not a DXF ACDSDATA envelope. Recorded the key distinction in S2.3: DWG DataStorage handle/index links must not be conflated with the separately evidenced DXF ACDSRECORD group-320 entity key. This sharpens implementation guards but does not resolve DXF contract or promote conversion support. Authority: [ODA Open Design Specification for .dwg files](https://static.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf), §§24.2.2.7.1 and 24.3. |
| S1.1 | COMMITTED | Field-level serializers now cover 3DFACE, 3DLINE, POLYLINE/VERTEX, MESH, HELIX/SPLINE, modeler geometry, all surface subtypes, INSERT placement, and nested ATTRIB fields. Loft reference values are typed; binary values remain digest carriers. |
| S1.2 | COMMITTED | Runtime self-tests verify mesh-coordinate mutation, polyface index serialization, INSERT/ATTRIB placement, loft-reference typed/binary separation, and modeler frame-body labeling/digest. Manual C++17 `-Wall -Wextra -Werror` adapter build and `--self-test` pass; `ctest -R '^libdxfrw_dwg_local_roundtrip$'` passes 1/1. Existing generated malformed DXF modeler checks remain in the fast round-trip test. No downloaded fixtures added. |
| S2.1 | COMMITTED | DXF modeler reads now retain ordered group-1/3 text and group-310 binary chunk metadata as bounded views into `m_rawBytes`. The semantic adapter reports chunk bounds, keeps surface bytes explicitly unclassified, and gives DWG frame/DataStorage payloads distinct digests; `ACDSDATA` stays an independent raw-section carrier with no invented entity link. Strict C++17 adapter build/self-test and focused round-trip CTest pass for text, binary, and mixed chunk sequences. |
| S2.2 | COMMITTED | Modeler and surface writers emit SAT text groups 1/3 only through AC1024, reject binary/mixed/unqualified DWG carriers, and reject AC1027+ inline payloads until ACDSDATA association is known. Runtime tests pass for AC1015/1018/1021/1024, ASCII/binary DXF file encodings, surface SAT, mixed-input rejection, and unsupported SAB/frame payloads. LibreDWG 0.14 `dxf2dwg --as r2000` independently read all four generated version vectors and wrote DWG output (non-fatal unknown `HEADER.DIMLDRBLK` warnings only). Generic ACDSDATA capture/replay remains a separate opaque-section path; no entity association/support claim is added. |
| S2.2.1 | COMMITTED | Added an in-memory, locally generated orphan `ACDSDATA`-shaped section regression: read through the generic raw-section callback, verify ordered group codes and raw spellings, replay/write and read again, and confirm no typed entity association. `cmake --build build --target libdxfrw_wave1_tests --parallel 2` and `ctest --test-dir build -R '^libdxfrw_wave1$' --output-on-failure` pass (1/1); `git diff --check` passes. No DWG/DXF fixture file was added. This is opaque-carrier preservation only—not schema, SAB, DataStorage association, DXF semantic writer, FreeCAD importer acceptance, or a promoted support claim—and it does not unblock S2.3. |
| S2.2.2 | COMMITTED | Preserved observed R2013+ `AcDbModelerGeometry` group 290 and group-2 identifier opaquely; omitted the pre-R2013 group-70 format field for AC1027+ and remapped group-350 history references through the entity output-handle map. Runtime-generated ASCII and binary DXF vectors verify false/true group 290, zero/nonzero GUID-like values, changed entity handles, correctly remapped history, and no AC1027 group 70; no fixture was added. `cmake --build build --target dwg2dxf lc3_compat_check libdxfrw_dwg_local_roundtrip --parallel 2` succeeds; focused CTest (`libdxfrw_dwg_local_roundtrip`, `dwg2dxf_version_policy`, `dwg2dxf_freecad_cli_compat`) passes 3/3; `git diff --check` passes. Shell-field preservation only—not a normative field contract, ACDSDATA schema/association, DWG-to-DXF SAB conversion, FreeCAD importer acceptance, or support claim. |
| S2.2.3 | COMMITTED | Closed an adapter gap: `dx_iface` inherited the no-op `addRawDxfSection`, so `dwg2dxf input.dxf -o output.dxf` discarded unknown sections even though the generic codec could replay them. `dx_data` now retains these opaque sections and `fileExport()` supplies them to the writer. New `dwg2dxf_raw_dxf_section_passthrough` generates a local AC1032 control at test runtime, checks the ACDSDATA-shaped section and neighboring LINE through two actual CLI passes, and removes only its unique build-tree directory. Build targets `dwg2dxf`, `lc3_compat_check`, and `libdxfrw_dwg_local_roundtrip` pass; focused CLI/version/local-roundtrip CTest passes 4/4; `git diff --check` passes. Existing sibling ezdxf v1.4.4 sample `examples_dxf/uncommon.dxf` (SHA-256 `5efdd671e3b6d8c45bd0688b1d6479d523f2290f0b390f1584348bf46e34c965`) also round-trips through the CLI twice with identical canonicalized ACDSDATA tag/value digest `a0b28e71c99bde07e9d9942f38eae538988dcb5d6acfd680cf291cdc2fe4265d`: six schemas, fifteen records, two 3DSOLID entities. The sample remains external, and its authoring provenance is unknown. This is DXF→DXF opaque section retention only; it does not qualify DWG DataStorage-to-DXF emission, ACDSDATA schema/link semantics, FreeCAD DWG handoff, or solid geometry support. No drawing fixture was added. |
| S2.3 | IN_PROGRESS_NARROW; S2.3.4 IMPLEMENTED; S2.3.5 FAST_PASS; S2.3.6 PENDING_ODA; S2.3.7 PENDING_FREECAD | ODA File Converter 27.1.0.0 generated a hash-pinned AC1027 DWG/DXF pair from tracked LibreDWG Cone; Open CAD Studio v2026.38 independently confirms its 1,980-byte `ASM_Data` payload and matching nonempty mesh metrics. The bounded S2.3.4 implementation projects only that standalone `3DSOLID` / `AcDb3DSolid_ASM_Data` tuple, preserving the observed six-schema table, SAB bytes/timestamp, opaque GUID, handle-key and history/proxy/material closure; material appearance fields are not preserved. Generated ASCII/binary vectors and the production `-o` source-revision-default CLI conversion pass; exact output SHA-256 `6289fd99756dbb68d05ce2f7a4c3d468c8f8948bb3554ad34e26fec328c9337c` matches the file read by Open CAD Studio. The ODA GUI accepts it and writes AC1032 DWG, but the only return DXF is R14 ASCII and rewrites the SAB carrier as inline SAT without ACDSDATA/history; the version-matched ACAD2013→ACAD2013 semantic round-trip remains pending until the Mac is unlocked. A correctly parameterized official CLI retry exits 134 on macOS pasteboard/XPC initialization without output. CAD Assistant v1.6.0 ARM64 returned generic transfer errors on the DXF and extracted SAB; its signed 1.6.0.0 app bundle contains no standalone reader CLI. FreeCAD handoff remains gated after S2.3.6. No public support claim is promoted; clones, INSERT/XREF, other producers/versions/schemas/entities, DWG modeler writing, material fidelity, and FreeCAD geometry remain unqualified. External ACadSharp/ezdxf controls remain advisory; no fixture was copied or committed. |
| S5.1 | COMMITTED | Generated ASCII/binary DXF vectors verify 3DFACE WCS corners/invisible-edge flags, SOLID and TRACE numbered corner fields, 3D POLYLINE WCS vertices, polyface counts/subclass typing/signed invisible-edge indices, LWPOLYLINE OCS elevation/normal/local vertices, and MESH vertices/faces/edges/creases. Fast in-memory ASCII inputs verify omitted corner 4 duplicates corner 3 for 3DFACE and SOLID, reject half-present fourth corners for 3DFACE/SOLID, and check TRACE OCS values plus the negative-normal `ext=true` projection. Fixed in-tree `dx_iface` 3DFACE/MESH/TRACE output routes and MESH import callback; corrected polyface groups 71/72, subclass selection, and group-91 omission; removed the invalid `AcDbSequenceEnd` marker rejected by LibreDWG. The focused round-trip and hardening CTests pass, the 4-test fast regression slice passes 4/4, and `lc3_compat_check` builds. LibreDWG 0.14 accepts the generated ASCII/binary DXF through its R2000 converter, but the DWG→DXF check drops nonzero SOLID/TRACE corner Z values and therefore does not independently qualify those semantics; no support claim is based on that lossy result. Vectors are generated at runtime and not committed. |
| S5.2 | COMMITTED | Generated ASCII/binary DXF vectors exercise nested INSERTs with nonzero block base points, attached ATTRIB, oblique OCS, 90-degree rotation, nonuniform/mirrored scales, and MINSERT arrays. An independent arbitrary-axis/matrix oracle checks a nested world point and an array-cell offset; malformed non-finite insertion points are rejected. Writer array counts now stop at the signed 16-bit group-code limit accepted by the reader. Focused CTest passes 1/1, and LibreDWG 0.14 independently converts the exact generated ASCII/binary files to R2000 DWG (non-fatal unknown `HEADER.DIMLDRBLK` warnings only). This validates DXF acceptance, not the transform oracle; vectors are runtime-generated and not committed. |
| S5.3 | COMMITTED | Closed the 3DFACE group-70 writer truncation gap: DXF defines only the four edge bits (`1`, `2`, `4`, `8`), but serialization used a signed 16-bit field without validating the `int` source. The writer now fails closed before record emission for values below zero or above `0x0f`. Runtime-generated ASCII and binary exports reject `-1`, reserved bit `16`, and `65536` (which would otherwise narrow to zero); tests verify no output file is published. DXF parsing remains permissive and keeps the parsed integer in the typed callback for forward compatibility. The official [Autodesk 3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm) anchors the four defined flags. Focused round-trip CTest passes; no DWG claim or fixture added. |
| S5.4 | COMMITTED | Autodesk defines DXF ELLIPSE center and major-axis vector in WCS, but `processEllipse()` previously called the OCS-to-WCS helper whenever `ext=true`, rotating already-world coordinates and mirroring parameter ranges for negative Z normals. The DXF path now preserves the WCS tuples and parameters in both modes; the DWG `emitWithExtrusion()` path is deliberately unchanged and remains version-qualified. Runtime in-memory DXF test checks WCS center/axis, normal, ratio, and partial-ellipse parameters with `ext=false` and `ext=true`; the focused hardening CTest and 4-test fast regression slice pass, and `lc3_compat_check` builds. Updated the 3D consumer contract and status table without promoting family-level support. No API, DWG parser, or fixture added. |
| S5.5 | COMMITTED | Fixed DWG→DXF PFACE face emission: DWG type-14 face records encode signed indices but no flags field, so `writePolyline()` now recognizes the typed `PolyfaceFace` subtype and synthesizes DXF group-70 bit 128, `AcDbFaceRecord`, and groups 71–74. Existing DXF-input vertices with explicit bit 128 retain their route. The ASCII/binary round-trip regression starts with subtype set and flags zero; both variants reparse with flags 128 and preserved signed indices. Build succeeded; `libdxfrw_dwg_local_roundtrip` passes 1/1. The regenerated local AC1015 control matches libdxfrw, LibreDWG 0.14, and its recipe using valid one-based indices across nonzero-Z 3D POLYLINE, 3×2 legacy MESH, and five-vertex/three-face PFACE. DWG→DXF inspection shows three `AcDbFaceRecord`s with group 70=128 and indices `(1,2,3,4)`, `(2,3,-4,5)`, and `(3,-4,5,0)`. All four pre-existing optional DWG comparator profiles also pass; `git diff --check` passes. This closes one conversion-path gap only; no general PFACE/DWG writer or target-interoperability claim is promoted. |
| S5.6 | COMMITTED | Reworked `DRW_Ellipse::toPolyline()` for legacy AC1009/R12 output to sample the WCS ellipse frame, emit 2D `POLYLINE` only for default-XY/zero-elevation input, and emit WCS 3D `POLYLINE` with bit-32 vertices otherwise. Fixed ratio-greater-than-one axis normalization to preserve signed partial sweeps across parameter wrap; runtime-generated tests check every sampled point for both directions of tilted ratio>1 arcs against an independent WCS ellipse equation. A zero-normal ellipse is rejected and no empty polyline/output is published. Runtime-generated AC1009 ASCII DXF covers tilted and planar full ellipses, two tilted partial arcs, and the malformed negative. `cmake --build build --target libdxfrw_dwg_local_roundtrip -j4` and focused CTest pass 1/1; `git diff --check` passes. No testing DWG/DXF fixture was added or committed. This qualifies only explicit legacy-version downgrade vectors; FreeCAD's default source-version workflow is separately gated by S8.15.9, and no target-authored or general ellipse support claim follows. |
| S5.7 | COMMITTED | Autodesk's HATCH DXF reference fixes the parent elevation point's X/Y to zero and its Z to elevation, while boundary/seed coordinates are OCS values and extrusion defines the plane. `DRW_Hatch::validateDxf()` now rejects nonzero parent X/Y, non-finite elevation/normal, and a zero normal; parsing therefore fails before HATCH/MPOLYGON callback publication, and `validateHatchPayload()` makes writers reject rather than silently emit zero in place of caller X/Y. Runtime-only ASCII tests preserve one oblique HATCH polyline boundary and seed point unchanged under both `applyExt` settings, calculate an independent expected WCS point from N=(0.6,0,0.8), and reject each malformed parent coordinate plus a zero MPOLYGON normal. No fixture added. `cmake --build build --target libdxfrw_hardening_tests libdxfrw_wave1_tests --parallel 2` and focused CTest (`libdxfrw_wave1`, `libdxfrw_hardening`) pass 2/2; `git diff --check` passes. This is one parent-header/OCS retention contract only, not HATCH tessellation/rendering, all path edge types, independent CAD interoperability, or DWG support. Sources: [Autodesk HATCH DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-C6C71CED-CE0F-4184-82A5-07AD6241F15B.htm), [boundary path data](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-DC5215D6-E73F-4DFF-8BE9-01CA9610FAEE.htm), and [OCS rules](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm). |
| S5.8 | COMMITTED | `DRW_Hatch::parseCode()` enforces adjacent HATCH/MPOLYGON point/vector pairs: groups 10–13 require the corresponding 20–23 before another parsed field, preventing missing coordinates from silently remaining at their default zero. The transition check is shared with `DRW_MPolygon::parseCode()` before its subclass-only trailer fields are handled; a follow-up audit found and closed the bypass where an MPOLYGON fill-color group could otherwise appear between X/Y while leaving the pending pair for a later Y. Runtime ASCII cases reject missing Y values for a polyline vertex, line endpoint, circular center, ellipse major-axis point, spline control point, both spline tangents, and seed point before callback publication; unmatched Y is also rejected for a line endpoint, seed point, and parent elevation. Direct MPOLYGON cases reject both an incomplete polyline-path pair and a seed pair split by fill-color group 63, while accepting a complete seed pair before that trailer. Complete OCS pairs for polyline, line, arc, ellipse, spline control/fit points, and tangents remain accepted. Tests generate strings in memory; no fixtures added. Latest affected build of `libdxfrw_hardening_tests` and `libdxfrw_wave1_tests` succeeds; focused CTest (`libdxfrw_hardening`, `libdxfrw_wave1`) passes 2/2; `git diff --check` passes. This validates pair completeness only—not wholly omitted optional tuples, edge continuity/closure or curve evaluation, HATCH rendering/fill, independent CAD interoperability, or DWG layouts. Sources: [Autodesk boundary path data](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-DC5215D6-E73F-4DFF-8BE9-01CA9610FAEE.htm) and [OCS rules](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm). |
| S5.9 | COMMITTED | Autodesk's [HATCH boundary-path table](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-DC5215D6-E73F-4DFF-8BE9-01CA9610FAEE.htm) assigns code 73 to elliptic-edge counterclockwise direction; [ezdxf's `EllipseEdge` model](https://github.com/mozman/ezdxf/blob/master/docs/source/dxfentities/hatch.rst) exposes the same `ccw` field. Audit found `DRW_Hatch::parseCode()` routed code 73 to ARC and polyline but not ELLIPSE, so clockwise boundaries silently defaulted to counterclockwise. It now stores the code in `DRW_Ellipse::isccw`. A hand-authored runtime ASCII edge asserts direction, center, major-axis endpoint, ratio, and angle parameters; generated ASCII and binary DXF round trips assert arc and ellipse direction plus typed boundary fields. `cmake --build build --target libdxfrw_hardening_tests libdxfrw_dwg_local_roundtrip --parallel 2` succeeds; focused CTest (`libdxfrw_hardening`, `libdxfrw_dwg_local_roundtrip`) passes 2/2; `git diff --check` passes. No fixtures added. This preserves one HATCH elliptic-edge field only; it does not establish path closure, curve evaluation/rendering, independent CAD interoperability, FreeCAD import, or DWG support. |
| S5.10 | COMMITTED | Autodesk's HATCH group-code table assigns group 73 to the MPOLYGON annotated-boundary flag, while its boundary-path table also assigns group 73 to polyline closure and ARC/ELLIPSE direction. `DRW_MPolygon::parseCode()` now distinguishes the entity-level trailer after top-level group 76 from nested path values, restricts the annotation flag to 0/1, and preserves MPOLYGON's group-11/21 offset-vector pair in the existing `xDirX`/`xDirY` fields; an incomplete pair fails parsing. The DXF writer emits non-default annotation and the vector; writer preflight rejects invalid annotation and non-finite vector values. Runtime ASCII tests verify annotation 0/1 does not change path closure and reject annotation 2/dangling vector; generated ASCII and binary round trips preserve annotation=1, the vector `(0.25,-0.5)`, and ordered closed-boundary vertices. `dwg2dxf_freecad_cli_compat` still passes its exact `input -o output` invocation and MPOLYGON record-retention case. Build targets `dwg2dxf`, `libdxfrw_hardening_tests`, `libdxfrw_dwg_local_roundtrip`, and `libdxfrw_wave1_tests` pass; focused CTest passes 4/4; no fixture files added. Autodesk sources: [HATCH codes](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-C6C71CED-CE0F-4184-82A5-07AD6241F15B.htm) and [boundary-path codes](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-DC5215D6-E73F-4DFF-8BE9-01CA9610FAEE.htm); independent mapping: [ezdxf MPOLYGON implementation](https://github.com/mozman/ezdxf/blob/master/src/ezdxf/entities/mpolygon.py). This is bounded DXF field/CLI preservation only—not DWG MPOLYGON semantics, fill rendering, or FreeCAD geometry import (the pinned importer limitation remains S8.13). |
| S6.1 | COMMITTED | ASCII and binary runtime round-trips cover PLANESURFACE, EXTRUDED, REVOLVED, SWEPT, LOFTED, NURBSURFACE, SPLINE, and HELIX fields. Added bounded subtype group-90 sizes/group-310 byte retention; corrected SWEPT ID/size ordering and legacy group-91 acceptance; corrected one-byte binary-DXF Boolean encoding; tightened field/count/transform/constraint validation; made `dx_iface` preserve HELIX callbacks. Focused CTest `libdxfrw_dwg_local_roundtrip` passes 1/1, including malformed lengths/booleans/partial vectors and invalid writer fields. Runtime vectors are not committed. LibreDWG 0.14 rejected/does not handle the generated surface/NURBS cases, so it provides no independent semantic qualification. ODA v5.4.1 §20.4.40 says `splFlag1` is BL for R2013+; current code matches; no width fix is justified. Authentic per-version spline qualification and all DWG surface layouts remain outstanding. The integration checkpoint also found and corrected the REVOLVEDSURFACE copy/assignment hardening vector to enter `AcDbRevolvedSurface` before testing group-90 subtype-ID state; the corrected focused hardening CTest passes. |
| S6.2 | COMMITTED | Added parser-only synthetic SPLINE body tests for both sides of the version boundary. The AC1027 scenario-2 body hand-encodes `splFlag1=13` and `knotParam=2` as BL; the AC1024 scenario-1 body confirms the R2013+ BL fields are absent and the three legacy flags remain individual B values. Both bodies are bounded before distinct BL sentinels and assert parsed fields plus exact cursor alignment. `cmake --build build --target libdxfrw_dwg_local_roundtrip --parallel 2` and focused CTest pass (1/1); no drawing fixture or DWG parser change. ODA v5.4.1 §20.4.40 agrees with the existing version-gated reader. This closes only local synthetic branch coverage; authentic AC1024/AC1027/AC1032 spline witnesses and independent/version-wide support remain outstanding. |
| S3.1 | COMMITTED | Validated ODA v5.4.1 §20.4.41's non-empty modeler version range (1 or 2); empty ACIS bodies retain the absent-version/default-zero case. Runtime-generated AC1018 frames cover empty, 1, 2, 0, and 3; build and focused round-trip CTest pass. A local AC1024 conference-room debug conversion reached modeler parsers and retained 3DSOLID history handles, but the overall CLI failed on an OBJECTS-pass type-42 frame, so it is not an end-to-end positive. The version-2 byte is opaque filler; no payload extraction or semantic ACIS claim follows. |
| S3.2.1 | COMMITTED | Implemented the AC1024/R2010 non-empty version-2 inline SAB carrier slice in commit `c7c8eea`. The parser requires the exact `ACIS BinaryFile` signature and a unique tagged ACIS end marker bounded by the entity data body; publishes the extracted bytes and source bit range separately from the whole DWG frame; and leaves missing/duplicate-marker cases opaque. A local-from-scratch AC1024 frame covers exact extraction, tail exclusion, marker absence/ambiguity, no DWG-frame decoder fallback, and no unqualified DXF SAB write. A fresh read-only comparison against LibreDWG 0.14 now matches every payload by handle, length, and SHA-256 across four untracked local AC1024/R2010 samples: `visualization_-_aerial.dwg` (5; 29,268 carrier bytes), `visualization_-_conference_room.dwg` (33; 700,746 bytes), `visualization_-_condominium_with_skylight.dwg` (76; 1,694,840 bytes), and `visualization_-_sun_and_sky_demo.dwg` (15; 188,240 bytes); 129 3DSOLID carriers total (2,613,094 bytes including the SAB signature). LibreDWG reports the `ACIS BinaryFile` signature separately, which is rejoined for the digest comparison. `libdxfrw_dwg_local_roundtrip`, `libdxfrw_graph_preservation`, `libdxfrw_hardening`, and `libdxfrw_3d_consumer_probe` pass; `lc3_compat_check` builds. This is exact opaque-carrier evidence only, not semantic solid support, a DWG writer, a claim for other entity types/versions, or AC1024 SAT/alternate-variant coverage. No sample fixture was staged or committed. |
| S3.2.2 | COMMITTED | AC1027/R2013 `has_ds_data` modeler entities no longer fail solely because the entity-local BS is outside 1/2; effective version 2 is set only after exactly one handle-linked record starts with the exact ODA-documented `ACIS BinaryFile` SAB prefix. Runtime-generated AC1027 frame with raw value 168 passes the typed parser regression. `libdxfrw_hardening` exercises unique selection, exact numeric/key identity, disagreement rejection, duplicate-section ambiguity, wrong-version/orphan accounting, malformed-signature non-normalization, alternate `ASM BinaryFile4` retention without ACIS normalization, entity-handle fallback, idempotent replay, and two records linked while entity traversal order is reversed; these use in-memory records only. The local untracked `Cover.dwg` emits one `MODELER_GEOMETRY` callback for handle `0x6f`, effective version 2, non-empty modeler state, a linked 22,983-byte record, and handle key `6F`; its carrier SHA-256 (`e0a5e069175edafd980c942bb5766091705534b1e6e3cfe43f5fc982e17b9eda`) matches LibreDWG 0.14's entity `acis_data` after rejoining the signature. Treat that as one-sample payload corroboration, not an independent/general association oracle: LibreDWG's [NEWS](https://github.com/LibreDWG/libredwg/blob/master/NEWS) records incomplete, brute-force AcDs extraction in v0.11, while open [issue #1411](https://github.com/LibreDWG/libredwg/issues/1411) reports missing AC1027+ AcDs extraction with LibreDWG 0.14.8593, including an AC1032 case. An open-source [AcDs round-trip note](https://github.com/hakanaktt/acadrust/blob/main/tests/roundtrip.rs#L3785-L3800) identifies positional record/entity mispairing as a failure mode; we use it only to motivate an order-reversed local vector, not as format evidence. Alternate ASM-prefixed records remain opaque until primary layout/sample evidence is available. Focused round-trip, graph, hardening, and consumer tests pass; `lc3_compat_check` builds. The DWG sample remains untracked; no fixture added. Opaque byte identity only, not geometry semantics, other AC1027 records/variants, AC1032, or writing. |
| S3.2–S3.6 (remaining versions and paths) | BLOCKED_PER_VERSION | R13/R14 and non-AC1015 R2000 SAT layouts/entities; full AC1015 SAT-v1 qualification beyond the narrow `3DSOLID` slice in S8.15.16; AC1018, other AC1021, AC1027, and AC1032 inline variants; AC1027+ external DataStorage association beyond the single S3.2.2 record (including missing/conflicting/orphan cases and other entities); cross-version handle/frame accounting; and modeler DWG writing remain unimplemented or unqualified. A fresh read-only audit with LibreDWG 0.14 minJSON found 129 modeler records across the four local AC1024 samples: 33 in `visualization_-_conference_room.dwg`, 76 in `visualization_-_condominium_with_skylight.dwg`, 5 in `visualization_-_aerial.dwg`, and 15 in `visualization_-_sun_and_sky_demo.dwg`; every record is `3DSOLID` with modeler version 2. Those four AC1024 samples supply no version-1, `REGION`, or `BODY` witness; separately, S8.15.16 now records one external AC1015 `3DSOLID` SAT-v1 sample and an exact converter/reference comparison. Thus the AC1024 corpus confirms only its qualified v2 lane and cannot unlock another entity/version variant. A focused recheck on 2026-09-24 passed `libdxfrw_dwg_reader_matrix` (1/1); the AC1024 `v2010` end-to-end conversion cases remain 6/10, with four visualization conversions failing. Each debug trace reaches `dwgReader24::readDwgClasses END` after a CRC-mismatch warning. None prints `DWG file error` or `Error reading file`; `dwg2dxf` therefore completes DWG import and fails during DXF export. The writer's `writeModelerGeometry()` deliberately rejects unqualified DWG modeler/frame/DataStorage payloads, and these four samples contain 129 non-empty AC1024 version-2 `3DSOLID` SAB carriers. The focused reader, local round-trip, hardening, and FreeCAD CLI-contract CTests pass 4/4; the 12/53/13 object-parser warning counts in aerial/condominium/sun-and-sky are not evidence of a class-footer failure, and the conference trace also reaches OBJECTS. These conversion failures are explained by the intentionally unsupported SAB-to-DXF writer boundary, not a reproducible `BAD_READ_CLASSES` regression. Keep the class-footer synthetic test green; do not relax CRC checks or change class-size arithmetic. Any future positive conversion requires an evidenced DXF representation for the SAB content, not relabeling the carrier as SAT. Keep the three `E3DSOLID` spline-checkpoint objects distinct from spline correctness; do not relabel frame bytes as SAT or weaken the writer gate. R1.4/R11 remains blocked on era-appropriate reference/sample. Continue with any available per-version ODA/trace/independent-witness lane; do not infer a neighboring version's layout. These samples are user-owned, remain unstaged, and diagnostic outputs were kept under `/private/tmp`. |
| S3.2.3 | COMMITTED | Added AC1021/R2007 version-2 inline SAB extraction with the R2007 object-size and embedded-handle framing kept distinct from AC1024/R2010. Runtime-generated AC1021 and AC1024 frame tests verify exact bounded payload extraction, source-version/range metadata, tail exclusion, missing/duplicate marker rejection, and no fallback from raw frame bytes; `libdxfrw_dwg_local_roundtrip` passes 1/1. One temporary ODA File Converter 27.1.0.0-resaved AC1021 `Cone.dwg` witness has a 1,629-byte SAB from signature plus 1,614 body bytes according to LibreDWG 0.14 minJSON and libdxfrw's trace; this is envelope/length corroboration only, not byte-for-byte identity or target-authored DWG evidence. An earlier comparison with the LibreDWG AC1015 reference DXF was cross-version and is not a semantic finding. On the same AC1021 input, LibreDWG 0.14 `dwgread -O DXF` emits no group-1/3 SAT values for the `3DSOLID`, while ODA's DXF has 31 pairs. LibreDWG's verbose trace fails on unknown SAB tag 18 in its SAB-v2-to-SAT1 path yet exits successfully and writes the DXF without SAT; neither export is an independent SAT oracle or proof of equivalence. The exact FreeCAD CLI form parses the DWG but still fails closed in `writeModelerGeometry()` with no DXF published; FreeCAD receives no importer handoff. This does not promote AC1021 conversion, FreeCAD converter integration, or geometry support. Other AC1021 inline variants remain in the aggregate blocker row. `dwg2dxf_freecad_cli_compat` passes 1/1; `git diff --check` passes. External DWG/DXF inputs and generated outputs remain outside the repository; no new drawing fixture is committed. |
| S4.1–S4.4 | BLOCKED_ON_S3 | Opaque DWG modeler payload writing follows only verified read layouts. |
| S7.1–S7.3 | BLOCKED_ON_INDEPENDENT_WITNESS | Target-sample comparisons cover AC1024 INSERT placement and scenario-2 SPLINE fit fields, AC1021 3DFACE/LINE and ELLIPSE/ARC/CIRCLE/POLYLINE_MESH field subsets from one provenance-unverified local sample (S8.4a.6/.7/.9), the planar AC1015 3D-POLYLINE subset (S8.4a-S8.4a.4), and six exact AutoCAD-authored POINT files (S8.4a.8). S8.4a.5 additionally checks nonzero-Z 3D POLYLINE, legacy MESH, and PFACE using a locally generated LibreDWG control; since LibreDWG both generates and reads it, this is not an independent target witness. The broad required family/version/direction matrix remains blocked; all corresponding support claims stay unqualified. |
| S7.4 | COMMITTED | Added docs/3D_SUPPORT_STATUS.md, linked from README, with family-specific DXF test versions/encodings, explicit DWG reader/writer version sets, direction-specific status, evidence grade, and unqualified/unsupported boundaries. Does not modify frozen metadata/qualified-format-claims-v1.json or metadata/qualified-format-status-v1.json, and promotes no semantic claim. |
| S7.5 | COMMITTED | Read-only audit of `../LibreCAD/librecad/src/lib/filters/rs_filterdxfrw.cpp` at LibreCAD HEAD `c67c02a01`: `add3dFace` and `addMesh` project XY into 2D polylines; 3DFACE preserves 3D corners/edge flags in a sidecar, while MESH renders base-cage faces without a native editable 3D mesh representation. Polygon-mesh polylines record counts/flags in advanced metadata, attach source 3D vertices to a fallback XData anchor, and render XY row/column polylines; smooth polygon meshes are not rendered. `addTrace`/`addSolid` produce 2D solids and retain native TRACE/SOLID corners/thickness in sidecars only for supported axial extrusion; non-axial extrusion is skipped. `addHelix` delegates to spline approximation; LibreCAD's callback documents axis/turn metadata as not represented in its entity model and dropped on import. `addSurface` and `addModelerGeometry` retain advanced metadata and render decoded SAB wireframe edges when available; neither creates an editable parametric/native 3D surface or solid. Generic spline handling creates LibreCAD 2D spline/conic entities and may approximate higher degrees. Base `DRW_Interface` defaults for `addMesh`, `addHelix`, `addSurface`, and `addModelerGeometry` are no-ops; only source-compatible delivery is guaranteed to other adapters. This is source-level callback evidence only: no LibreCAD build/UI interaction or 3D editing behavior was tested, and no public semantic 3D claim is promoted. |
| S8.1 | COMMITTED | Added `docs/3D_CONSUMER_CONTRACT.md` mapping each in-scope family to public callbacks/fields, WCS/OCS/subtype/opaque handling, topology/transforms, default callbacks, and conservative copy/lifetime guidance. The source audit found `ext=false` is the required path for 3D adapters; `ext=true` mutates only selected entities under the legacy 2D extrusion option. Autodesk OCS/INSERT/SPLINE/3DFACE/POINT/LINE references anchor DXF coordinate claims; DWG coordinate semantics remain per-version unqualified. No API or format-support claim changed. |
| S8.2 | COMMITTED | `cmake --build build --target lc3_compat_check --parallel 2` passed (exit 0); its static assertion verifies an older 2D-style `DRW_Interface` implementation remains concrete without overriding later optional callbacks. This is compile-time source-compatibility evidence, not a LibreCAD UI/runtime test or binary-ABI guarantee. No sibling checkout changes. |
| S8.2a | COMMITTED | Extended the runtime-generated AC1027 ASCII/binary DXF consumer probe to read the same LWPOLYLINE with both `ext=false` and `ext=true`. For normal `(0,1,0)`, elevation `5`, and local points `(2,3)` / `(4,5)`, the unprojected callback preserves OCS/elevation fields, while the established extrusion path emits callback XY `(-2,5)` / `(-4,5)`; 3DFACE WCS corners remain unchanged. This is a focused regression for one legacy mode, not a claim of full LibreCAD runtime compatibility. No DWG parser or LibreCAD source changed; no fixtures were committed. |
| S8.3 | COMMITTED | Added the standalone-only `libdxfrw_3d_consumer_probe` CTest, using `SemanticSink` as a headless consumer. It writes runtime-generated AC1027 ASCII and binary DXF, reads with `ext=false`, and checks callback delivery of 3DFACE XYZ/edge flags, 3D POLYLINE vertex Z, MESH XYZ/face-edge topology/creases, INSERT/MINSERT placement/scales/grid/OCS normal, SPLINE knots/control XYZ, LWPOLYLINE elevation/local XY/extrusion/bulge, and LOFTED surface typed fields plus a separately identified group-310 carrier. Added the missing LWPOLYLINE typed-field serialization to the semantic adapter. CTest probe passes 1/1; `libdxfrw_dwg_local_roundtrip` passes 1/1; `lc3_compat_check` builds. S8.2a additionally checks the same LWPOLYLINE legacy `ext=true` result in both DXF encodings. S8.3.1 extends the probe with typed ELLIPSE fields and checks WCS invariance in both modes. This is generated-DXF callback-delivery evidence only: it does not qualify third-party interoperability, DWG versions, surface evaluation, FreeCAD shape construction, or a renderer. No fixture files are committed. S8.4a supplies a separate narrow DWG read comparison; all other DWG consumer rows remain unqualified. |
| S8.3.1 | COMMITTED | Extended `tests/semantic_differential_adapter.cpp` so `addEllipse` serializes center, WCS major axis, ratio, start/end parameters, and extrusion instead of only common entity fields. Added runtime-generated default-plane full and oblique-plane partial ELLIPSEs to the existing ASCII/binary consumer probe; assertions verify all six fields in `ext=false` and legacy `ext=true`, including a nonzero-Z center and a unit oblique normal perpendicular to the major axis. `cmake --build build --target libdxfrw_3d_consumer_probe --parallel 2` succeeded and `ctest --test-dir build -R '^libdxfrw_3d_consumer_probe$' --output-on-failure` passes 1/1. No DXF fixture was added. This is local writer/parser callback evidence only; it does not qualify independent interoperability, DWG ELLIPSE layouts, or FreeCAD shape semantics. |
| S8.4a | COMMITTED | Added `tools/compare_dwg_3d_consumer_oracle.py`, an optional read-only comparator keyed by entity handle. Against the locally available AC1024 conference-room sample, libdxfrw's semantic adapter and LibreDWG `dwgread 0.14` match all six INSERTs for insertion XYZ, scale, rotation, and extrusion, plus both scenario-2 SPLINEs for degree, scenario, fit tolerance, start/end tangent, and all seven XYZ fit points. The libdxfrw read and `dwgread -O minJSON` both complete successfully. ODA v5.4.1 §§20.4.9, 20.4.10, and 20.4.40 anchor the relevant layouts. This narrows only experimental AC1024 read-field evidence for those fields; it adds no write, other-version, modeler, or general 3D claim. PLANESURFACE values disagree between the readers (libdxfrw emits zero typed fields while LibreDWG reports nonzero modeler fields) and have no named layout in the reviewed ODA text, so that family remains unqualified. The local DWG is not staged or committed. |
| S8.4a.1 | COMMITTED | Extended `tools/compare_dwg_3d_consumer_oracle.py` to accept AC1021 samples and match all 48 3DFACE handles from the local `tablet.dwg` against LibreDWG 0.14, including four 3D corners and invisible-edge flags. `has_no_flags=1` is normalized only to the default zero flag state. ODA v5.4.1 §20.4.32 describes the R2000+ layout. The AC1021 comparison and the existing AC1024 INSERT/SPLINE comparison both pass; Python syntax and `git diff --check` pass. Added exact read-only evidence to the consumer contract and status matrix. The untracked DWG was not staged. This narrows one-sample reads only and qualifies no write or general DWG support. |
| S8.4a.2 | COMMITTED | Extended the optional comparator and support docs to cover AC1021 LINE fields. All 3,002 handles match LibreDWG 0.14 for start/end XYZ, thickness, and extrusion; 670 records have nonzero endpoint Z. ODA v5.4.1 §20.4.21 defines the R2000+ layout. AC1021 (LINE and 3DFACE) and AC1024 (INSERT and SPLINE) comparator cases pass; Python syntax and `git diff --check` pass. The local DWG remains unstaged. This is a one-sample read subset only, not write or general version support. |
| S8.4a.3 | COMMITTED | Added an exact AC1024 condominium-sample profile to the optional comparator. Both LINE handles match LibreDWG 0.14 for start/end XYZ, thickness, and extrusion; both have nonzero endpoint Z. ODA v5.4.1 §20.4.21 defines the layout. The external minJSON has a bare `nan` in an unrelated surface record; the comparator normalizes only bare NaN tokens outside JSON strings and rejects non-finite values in all compared LINE fields. All three supported local sample profiles pass, as do the sanitizer assertion, Python syntax, and `git diff --check`. Documented the precise caveat and sample-only boundary; the DWG remains unstaged. No write or general AC1024 support claim. |
| S8.4a.4 | COMMITTED | Added an exact AC1015/R2000 profile for the pinned LibreDWG `PolyLine3D.dwg` blob; the comparator verifies SHA-256 before joining the libdxfrw adapter to LibreDWG 0.14. The parent 3D-POLYLINE and six ordered VERTEX children match for subtype/flags, curve type, handles/ownership, vertex flags, XYZ, and SEQEND. ODA v5.4.1 §§20.4.12/.17 supply the layout authority; the pinned source has an AutoCAD VLA property dump and paired DXF. All Z values are zero, so this qualifies only the planar compound-record read subset. The external sample stays in `/private/tmp`; no fixture, writer, nonzero-Z, PFACE/MESH, or general AC1015 claim is added. Existing supported AC1021/AC1024 oracle profiles and the new profile pass; Python syntax and `git diff --check` pass. |
| S8.4a.5 | COMMITTED | Added a locally authored `.dwgadd` recipe and optional AC1015 comparator profile. LibreDWG `dwgadd 0.14` generates one nonzero-Z 3D POLYLINE, a 3×2 legacy POLYLINE_MESH, and a non-planar PFACE with five XYZ vertices and three faces, including signed one-based face indices. The libdxfrw adapter and LibreDWG `dwgread 0.14` match parent classes/flags, child order/handles/owners, SEQEND, XYZ, mesh dimensions/density, and PFACE face indices against the recipe. ODA v5.4.1 §§20.4.12/.17, .13/.34, and .14/.15/.33 anchor the layouts. LibreDWG is both writer and reader, so this is a generated cross-reader parser control only; it does not remove target-sample gates or promote support claims. Temporary DWG not committed. The profile and Python CLI/syntax check pass. |
| S8.4a.6 | COMMITTED | Extended the optional comparator to the exact-hash user-owned AC1021 `tablet.dwg` sample (SHA-256 `7f203649dc8434ef7cf7a46f7f6def2a0192a1163ba34ebcf88ebfe69635ccd4`), matching all 24 ELLIPSE records by handle with LibreDWG 0.14 in `ext=false` mode for WCS center/major-axis vectors, extrusion, axis ratio, and start/end eccentric-anomaly parameters. ODA v5.4.1 §20.4.39 is the field-layout authority; two records have negative-Z normals, 14 have non-full parameter intervals, and center/major-axis Z values are zero. The sample's producer/date are unverified and are explicitly not called target-authored. All five optional comparator profiles (AC1021 tablet; AC1024 conference-room/condominium; pinned AC1015 PolyLine3D; generated AC1015 topology control) pass. The comparator source, this plan, and the contract/status evidence docs are the only changed files; no user-owned/generated DWG is added or staged. This is one-sample reader corroboration only: no ELLIPSE family support, DWG writing, FreeCAD shape, other-version, or interoperability claim is promoted. |
| S8.4a.7 | COMMITTED | Extended the optional comparator to match all 243 ARC and 168 CIRCLE handles in the exact-hash user-owned AC1021 `tablet.dwg` against LibreDWG 0.14 with `ext=false`. ARC checks center tuple, radius, thickness, extrusion, and start/end angle radians; CIRCLE checks center tuple, radius, thickness, and extrusion. ODA v5.4.1 §§20.4.18/.20 are the field authorities. In this sample centers have Z=0, normals are default +Z, and thickness is zero; provenance remains unverified. All five current comparator profiles pass, including 3DFACE/ELLIPSE/ARC/CIRCLE/LINE for AC1021. Updated the consumer contract and status matrix; no binary or user-owned sample was staged. One-sample baseline field corroboration only—not non-default OCS, extrusion, elevated placement, write, FreeCAD shape, other-version, target-authored interoperability, or general ARC/CIRCLE support. |
| S8.4a.8 | COMMITTED | Added an exact-hash AutoCAD-authored POINT profile for `nextgis/dwg_samples`' six R14/2000/2004/2007/2010/2013 DWGs. Each profile requires AC1014/1015/1018/1021/1024/1027 respectively, SHA-256 identity, exactly one handle `0x7c`, WCS `(50,50,50)`, and default extrusion `(0,0,1)` against LibreDWG 0.14 and the independent corpus recipe; five paired source DXFs also match by handle/fields. ODA v5.4.1 §20.4.31 anchors the DWG layout. All six profiles passed using the locally rebuilt adapter; paired-DXF hashes are pinned and checked where the corpus has them. No corpus files were downloaded, modified, staged, or committed. Updated the plan, consumer contract, and status matrix. This is six exact-file read-field evidence only—not `dwg2dxf` conversion or FreeCAD evidence, write support, or family/version-wide support. |
| S8.4a.9 | COMMITTED | Extended the optional comparator to both legacy AC1021 `POLYLINE_MESH` parents and their twenty ordered vertices in the exact-hash user-owned `tablet.dwg` sample against LibreDWG 0.14; ODA v5.4.1 §§20.4.13/.34 anchor the fields. Handles, type/subclass, flags, curve type, M/N dimensions, densities, child ordering/ownership/subtypes/flags, SEQEND, and all coordinates match; all Z values are zero and provenance remains unverified. Five locally available non-POINT comparator profiles pass, including the updated tablet comparison; the pinned POINT DWG corpus was unavailable in this environment and was not downloaded. Python AST validation and `git diff --check` pass. The sample remains unstaged/uncommitted; this is read-field corroboration only, not target-authored MESH interoperability, non-planar/topology evidence, FreeCAD geometry, writing, or general MESH support. |
| S8.4b | BLOCKED_PER_FAMILY_VERSION | Broaden DWG consumer qualification only when the exact reader layout, authentic target sample, and independent semantic oracle are all available. S8.4a.5 now supplies a local parser control for nonzero-Z 3D POLYLINE, legacy MESH, and PFACE, but LibreDWG both generated and read that file, so target interoperability remains unqualified. S8.4a.6 adds one provenance-unknown AC1021 ELLIPSE read-field comparison; S8.4a.8 adds six exact AutoCAD-authored POINT read-field instances; S8.4a.9 adds two flat legacy POLYLINE_MESH read-field comparisons from the same provenance-unknown AC1021 sample. None closes broader version/family evidence or writing. The AutoCAD-authored S8.4a.4 sample is planar and does not verify nonzero-Z 3D POLYLINE, PFACE, or MESH. The local AC1021 polygon-mesh sample has two meshes and 20 vertices, all with Z=0; it is not a non-planar/topology witness. ODA v5.4.1 includes Chapter 8's R2018 organization summary and §9's R2018+ header-size rule, but its searchable text has no named `AcDbSubDMesh` or modern `AcDb*Surface` class layout; see the [official ODA specification](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf). The LibreDWG-maintained 5.4.2 diff is project-specific, not normative authority. ARC/CIRCLE OCS, modeler, modern surface, and other-version rows remain gated; modern surface fields currently disagree and remain unqualified. Next action: obtain an authentic target-generated sample exercising the exact feature/version, its class identity/field-layout authority, and an independent field-level oracle. Do not fabricate DWGs, infer unsupported fields from neighboring families, or promote support from flat/absent corpus cases. |
| S8.5 | COMMITTED | Updated README, `docs/3D_CONSUMER_CONTRACT.md`, and `docs/3D_SUPPORT_STATUS.md` to separate the existing 2D source-compatibility lane, 3D typed-data callback access, format/version semantic qualification, and consumer display/edit behavior. Documented the exact AC1027 generated ASCII/binary consumer-probe families and its self-generated evidence ceiling; S8.4a-S8.4a.9 separately document narrow target-sample comparisons and the local generated topology control with distinct evidence ceilings, including the provenance boundaries for AC1021 ELLIPSE/ARC/CIRCLE/flat legacy MESH and the AutoCAD-authored POINT corpus. Other DWG rows remain unqualified. No general 3D, renderer, evaluator, editing, or binary-ABI claim is added. |
| S8.6 | COMMITTED | Made additive 3D support with preserved 2D behavior an explicit cross-slice acceptance rule. `lc3_compat_check` and `libdxfrw_3d_consumer_probe` build/pass; the generated ASCII/binary probe compares the same LWPOLYLINE under `ext == true` and `ext == false`, and S5.4 adds a DXF ELLIPSE WCS-invariance check in both modes. Future affected paths must repeat the applicable fast gate. This is source/callback evidence only; no LibreCAD code/UI, ABI, or general semantic format claim is added. |
| S8.7 | COMMITTED | The semantic sink now records ARC center/radius/thickness/extrusion/start/end radians and CIRCLE center/radius/thickness/extrusion. Runtime-generated AC1027 ASCII and binary DXF include a default-normal CIRCLE, oblique-normal CIRCLE, and negative-Z ARC. The probe verifies native OCS fields with `ext == false` and the exact established `ext == true` oblique center and negative-Z ARC angle mirror/swap values; the existing LWPOLYLINE/3DFACE invariants also remain passing. Focused consumer CTest passes 1/1 and `lc3_compat_check` builds. This is writer-self-generated callback-field evidence, not independent interoperability or family qualification; no source API/DWG parser changes or fixture files. |
| S8.8 | COMMITTED | Extended the runtime-generated AC1027 ASCII/binary consumer probe with a PFACE POLYLINE containing four nonzero-Z vertices and a typed face record whose zero source flags cause the writer to emit DXF group-70 bit 128. The sink verifies PFACE declaration/count fields, first/last vertex XYZ, face marker, and all four signed one-based face indices through both `ext == false` and `ext == true`. Focused consumer CTest passes 1/1; `git diff --check` passes. This is generated writer/readback field evidence only, not independent PFACE topology/interoperability or DWG child ownership evidence; no fixtures committed. |
| S8.9 | COMMITTED | Added FreeCAD's exact `dwg2dxf <input> -o <output>` invocation while retaining the old positional form. The converter captures reader version, defaults to the source revision for supported versions (AC1012/R13 maps to supported AC1014/R14), emits ASCII by default, refuses existing outputs without prompting, and accepts explicit `-y`; explicit output-version overrides remain available. The new fast CTest uses the repository-tracked `tests/fixtures/dwg/ordinary_enc_AC1027.dwg` copied only into the build tree so both input and output paths contain spaces; it checks AC1027 `$ACADVER` preservation, legacy `-v2010` output AC1024, no-prompt/no-overwrite sentinel preservation within a 5-second timeout, and explicit overwrite. `cmake --build build --target dwg2dxf lc3_compat_check` succeeds; `ctest --test-dir build -R '^dwg2dxf_freecad_cli_compat$' --output-on-failure` passes 1/1; `git diff --check` passes. No DWG/DXF fixture was added. S8.11 supplies unit coverage of the AC1012→AC1014 mapping; no local authentic AC1012 DWG is available, so that reader/version path remains under its existing witness gate. This verifies converter CLI/output only, not FreeCAD import or display. |
| S8.9.1 | COMMITTED | Verified CMake installation under a temporary macOS prefix (`bin/dwg2dxf`); the installed binary converted tracked AC1027 input using exact FreeCAD argv and wrote AC1027 ASCII DXF under a path containing spaces. Re-ran the full `tests/run_freecad_dwg2dxf_compat_test.cmake` against that relocated installed binary; fixture-preservation, Unicode-path, legacy positional, overwrite, and transactional-failure checks all passed. Added README and man-page guidance for FreeCAD's converter selection, `PATH`/configured full-path setup, same-name converter precedence, automatic fallback caveat, and the boundary between conversion and downstream importer support. No generated fixture/output entered the repository. FreeCAD 1.1.3 macOS resolution is subsequently qualified under S8.9.2; Windows `.exe` behavior remains unqualified there. S8.9.4 clarified that this is an import-side executable, not FreeCAD's paired `dxf2dwg` export tool. |
| S8.9.4 | COMMITTED | Updated README/man to say this package supplies FreeCAD's `dwg2dxf` import-side converter only, not its paired `dxf2dwg` export program. Verified against the current FreeCAD `main` `Draft/importDWG.py` source on 2026-09-24: one shared preference derives the opposite executable name, and an absent configured sibling does not fall back to PATH. That URL is mutable; runtime qualification is pinned to FreeCAD 1.1.3 under S8.9.2. Guidance recommends PATH discovery for import without naming either LibreDWG executable in the shared preference, and warns that direct `dwg2dxf` configuration can leave export unavailable. The fast `dwg2dxf_freecad_cli_compat` CTest passes 1/1; `mandoc -Tlint dwg2dxf/dwg2dxf.1` is clean after fixing the stale date and description line. Links reviewed; no product code, fake sibling, export claim, or fixtures added. The installed converter's macOS FreeCAD import route is qualified under S8.9.2/.3; Windows behavior remains a platform gate. |
| S8.9.5 | BLOCKED_EXTERNAL_PLATFORM | Refined 2026-09-25 against pinned FreeCAD 1.1.3 `importDWG.py` revision `145529fe741292ff0b3977a01195bf0247425794` and current upstream source. Native Linux and Windows remain the deployment gaps; macOS static/shared installed artifacts and pinned handoffs are already recorded above. On each remaining native host: (1) build/install outside the build tree, including paths with spaces; qualify shared mode only if shipped; (2) inspect runtime closure in a clean environment (Linux: no `LD_LIBRARY_PATH`; Windows: required DLLs from the normal package layout, never FreeCAD-private directories); (3) run fast installed-CLI, public-readback and failure-atomicity controls with tracked or locally authored-from-scratch input; (4) with an isolated FreeCAD profile and fallbacks disabled, assert platform executable name, hash, intended PATH/configured-path discovery, exact `[binary,input,"-o",output]`, valid output, and identical output-to-importer handoff for separate `open()` and `insert()`; use nonzero-Z LINE only as deployment smoke, leaving entity semantics under S8.15; (5) test the documented native desktop launch route or narrow the docs to a tested shell/configured-path route. Windows additionally needs native `.exe`, paths with spaces, and Unicode/non-ASCII path qualification. Because FreeCAD currently checks output existence rather than child status, never publish a failed/partial final DXF; Automatic fallback must not be attributed to libdxfrw. Run fast CLI/readback per change, FreeCAD processes only for platform qualification or changed handoff behavior, and dispatcher checks only for a new launch route. If no native host is available, retain this external blocker and continue independent ready work; cross-compilation is not evidence. |
| S8.9.6 | COMMITTED | Implemented and passed the opt-in actual-GUI-process harness using only tracked `ordinary_enc_AC1027.dwg`, copied beneath an input path containing spaces. On FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, the app-bundle GUI executable launched directly under the test harness's controlled environment registered `importDWG` and the shared `freecad.module_io.OpenInsertObject()` dispatcher completed both `open` and `insert` in separate GUI processes with fresh `--user-cfg` and isolated user paths. Both used the C++ `ImportGui` backend, `DxfImportMode=2`, `DWGConversion=1`, and the installed `/private/tmp/libdxfrw-freecad-desktop-prefix.RRbMas/install/bin/dwg2dxf` (SHA-256 `bac02b1df67c515cff5b88c8a3369ea4e05e60d4234a409be9054438c8ac40c2`) via the configured full path; each asserted one successful exact `dwg2dxf input -o output` call, fallback suppression, identical output-to-importer handoff, and three expected LINE bounds `(1,2,0)-(3,4,0)`, `(5,6,0)-(7,8,0)`, `(9,10,0)-(11,12,0)` in the open document and insertion target, with no unsupported features. Verbose CTest output now preserves the observed FreeCAD revision/platform, binary hash, argv, handoff paths, importer settings, and geometry evidence. The first launch timed out after conversion because FreeCAD's default `dxfShowDialog=true` opened the modal C++ importer-options dialog; the final harness disables it only in isolated preferences and asserts false in its result. Original smoke passed 1/1 (20.53 seconds); revalidated 2026-09-24 outside the macOS sandbox, passes 1/1 (21.67 seconds). The sandboxed retry aborts before import because Qt cannot see the host `neon` feature; the nonzero-Z headless `dwg2dxf_freecad_3d_line_cli` still passes inside the sandbox (1/1). Python macro syntax and `git diff --check` pass. Test-owned profiles/results are removed on pass and no fixture is added. This qualifies registered dispatcher/API behavior only—not OS-managed launcher PATH inheritance, file chooser/menu interaction, viewport rendering, or 3D entities in this desktop profile. Windows/Linux native execution remains S8.9.5. |
| S8.9.7 | COMMITTED | Added default-OFF `LIBDXFRW_ENABLE_FREECAD_FAILURE_CONTROL`, `tests/run_freecad_dwg2dxf_failure_test.cmake`, and a failure-only mode in `tests/freecad_dwg2dxf_import_check.FCMacro`. The CMake harness creates a malformed AC1027-signature input from scratch under a unique temp root, runs the installed converter from FreeCAD 1.1.3 `freecadcmd` with `DWGConversion=1` and fallbacks disabled, and requires a dedicated PASS marker. On macOS 27 arm64, revision `145529fe741292ff0b3977a01195bf0247425794`, FreeCAD invoked `/private/tmp/libdxfrw-freecad-install.0XOVoo/bin/dwg2dxf` (SHA-256 `5cf1832e7c5d5c2d82f1b05f42aa82dc972069edbc79046043c4a81da1aaa44d`) with exact argv `[binary,input,-o,output]`; it exited 1, published no candidate DXF, returned no document, made no `importDXF.open()` handoff, and logged no conversion-success message. `dwg2dxf_freecad_failed_conversion` passes 1/1 outside the sandbox; the sandboxed attempt aborts in Qt before application code because host `neon` is hidden. The shared macro's positive `dwg2dxf_freecad_sat_v1_open` regression passes 1/1 outside the sandbox; fast `dwg2dxf_version_policy` and `dwg2dxf_freecad_cli_compat` pass 2/2. Python AST parsing and `git diff --check` pass. Generated input/config/profile were confined to system temp and removed on success; no fixture was added. This is failure-publication evidence for one FreeCAD profile, not a DWG-format or platform-wide qualification. |
| S8.9.8 | COMMITTED | Added default-OFF `LIBDXFRW_ENABLE_FREECAD_INSTALLED_3D_LINE_CONTROL` and `tests/run_freecad_installed_3d_line_test.cmake`, registering distinct opt-in `open` and `insert` CTests. Each run uses the existing local-from-scratch `ac1015_3d_line_control.dwgadd` recipe to generate its AC1015 DWG beneath a unique system-temp root/path with spaces, seeds an isolated FreeCAD profile, and puts the configured installed converter first on PATH. The CMake harness rejects the build-tree executable and byte-compares its SHA-256 with the installed artifact before launch; the shared FreeCAD macro additionally asserts selected binary/hash, exact `[dwg2dxf,input,-o,output]` argv, status 0, identical importer handoff, `DWGConversion=1`, C++ importer mode 2, one LINE edge with bounds `(1,2,3)-(4,6,9)`, no unsupported features, and the insertion target document. On FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, both CTests pass (1.78 seconds combined) using installed `/private/tmp/libdxfrw-freecad-3d-line.0A0SD7/bin/dwg2dxf`, SHA-256 `dbb971f88365f4e3a9722e82216662d1f3cfdcd0d0696cc8c3deaa95a578b8f0`; the installed process's runtime dependencies resolve from FreeCAD. A sandboxed attempt aborted before application code because Qt could not see host `neon`; the same tests passed outside the sandbox. Success roots are removed by the harness; failure evidence remains in uniquely named temp paths. No generated DWG/DXF fixture was added. This closes only the pinned macOS installed-artifact AC1015 LINE/open+insert bridge; it does not close S8.9.5 or qualify other entity/version/platform tuples. |
| S8.9.9 | COMMITTED | Updated README and `dwg2dxf(1)` to state the actual FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794` / macOS 27 arm64 / C++ importer / PATH-discovery qualification, naming the installed-artifact `open()` and `insert()` tests for a generated AC1015 nonzero-Z LINE. Both documents explicitly leave native Linux/Windows profiles unqualified and retain entity/version-specific support and the import-only/export caveat. `mandoc -Tlint dwg2dxf/dwg2dxf.1` is clean and `git diff --check` passes. This documentation-only slice makes no new runtime or entity-support claim and adds no fixture. |
| S8.9.2 | COMMITTED | Extended `tests/freecad_dwg2dxf_import_check.FCMacro` with PATH/configured discovery and a guarded isolated-preference seeding mode: it checks the active `App.ConfigGet("UserParameter")` equals the requested `--user-cfg` under a fresh system-temp root before writing `DWGConversion=1` or `TeighaFileConverter`. Installed macOS `dwg2dxf` SHA-256 `5b7d23baf049746597bf0ca0a78141e94e260dfb0086aa3d468ac702df063f45` was exercised on FreeCAD 1.1.3, revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, `DxfImportMode=2`: PATH `open()`, configured direct-path `open()` with converter omitted from PATH, and sibling-derivation `open()` with the preference naming a nonexistent `dxf2dwg` beside the real `dwg2dxf` all pass. No fake sibling was created. Each asserts exact `[binary,input,-o,output]`, zero converter status, executable/hash attribution, identical `Import.readDXF` handoff, three `Part::Feature` LINE bounds `(1,2,0)-(3,4,0)`, `(5,6,0)-(7,8,0)`, `(9,10,0)-(11,12,0)`, and no unsupported features. The copied tracked AC1027 input SHA-256 is `a0ebf245e570bf0dc337c696330b7ea883feaebb7c4e781de8d77ac723815f83`; all config/data/output paths stayed in temp and the macro removed its produced DXFs. A mismatched expected config path was rejected before converter launch; `freecadcmd` nevertheless exits zero after a script exception, so the explicit `FREECAD_DWG_IMPORT_ASSERTIONS_PASS=` marker is mandatory. Python AST parsing, `dwg2dxf_freecad_cli_compat` (1/1), and `git diff --check` pass. Windows `.exe` lookup remains unqualified; GUI/display and other FreeCAD versions/importer modes remain outside this row. No fixture or generated DWG/DXF was added. |
| S8.9.3 | COMMITTED | Independently exercised `LIBDXFRW_FREECAD_OPERATION=insert` with FreeCAD 1.1.3, revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, `DxfImportMode=2`, and the isolated-config guard added under S8.9.2. The installed converter SHA-256 is `5b7d23baf049746597bf0ca0a78141e94e260dfb0086aa3d468ac702df063f45`; with PATH discovery and fallbacks disabled it receives exact `[binary,input,-o,output]`, exits zero, and FreeCAD hands that same output path and target `FreeCADDwgInsertCheck` to `importDXF.insert()`. The headless C++ `Import.readDXF` route adds exactly three `Part::Feature` LINE shapes with independent bounds `(1,2,0)-(3,4,0)`, `(5,6,0)-(7,8,0)`, `(9,10,0)-(11,12,0)` and no unsupported features. Input SHA-256: `a0ebf245e570bf0dc337c696330b7ea883feaebb7c4e781de8d77ac723815f83`. The macro emitted its PASS marker; a separate mismatched-config negative check confirmed fail-closed behavior (FreeCADCmd can still return zero on exceptions). The tracked AC1027 input was copied under a path with spaces, and configuration/output artifacts stayed in temp; no fixture was added. Python AST parse, `dwg2dxf_freecad_cli_compat` (1/1), and `git diff --check` pass. This qualifies this one pinned headless `insert()` route only; GUI display, Windows `.exe`, other FreeCAD revisions/importer modes, and wider feature support remain unqualified. |
| S8.10 | COMMITTED | Resolved the apparent runtime blocker: macOS sandboxing hid `hw.optional.neon`, causing Qt's false incompatibility abort; outside the restricted sandbox FreeCAD 1.1.3 revision `20260725` / arm64 / Qt 6.8.3 starts. `tests/freecad_dwg2dxf_import_check.FCMacro` runs the actual `Draft.importDWG.open()` route on each already tracked ordinary-encoding fixture (AC1015/AC1018/AC1021/AC1027) with this build's `dwg2dxf` on `PATH`. For all four, the C++ DXF importer reports 3 LINEs, creates 3 `Part::Feature`s, and reports no unsupported features. Each resulting XYZ bounding box matches LibreDWG 0.14's independent direct DXF export: `(1,2,0)-(3,4,0)`, `(5,6,0)-(7,8,0)`, `(9,10,0)-(11,12,0)`. An exploratory read of the existing untracked AC1021 `tablet.dwg` further shows both libdxfrw and ODA File Converter 27.1.0.0 exports contain 48 3DFACE/81 SOLID/24 HATCH records, while FreeCAD's C++ importer creates 4,868 objects from each and reports 38 3DFACE/69 SOLID/22 HATCH as unsupported with entity-read exceptions. This shared behavior is not attributable solely to libdxfrw conversion, but it does not establish semantic equivalence or FreeCAD usability; the sample remains user-owned and unstaged. The optional legacy Python importer was not tested because its `dxfReader` dependencies are absent. No fixture added. Keep qualification limited to this one host/default importer/four LINE fixtures; no GUI, general DWG, or 3D consumer claim. Issue #19247 remains unreproduced because it has no affected DWG/output pair. |
| S8.11 | COMMITTED | Extracted the implicit source-revision policy into CLI-private `dwg2dxf/dx_cli.h` and directly tested every supported revision, AC1012→AC1014, and UNKNOWN/unsupported rejection in `dwg2dxf_version_tests.cpp`. `cmake --build build --target dwg2dxf libdxfrw_dwg2dxf_version_tests` succeeds; `ctest --test-dir build -R '^dwg2dxf_(version_policy|freecad_cli_compat)$' --output-on-failure` passes 2/2; `git diff --check` passes. The AC1012 mapping policy is unit-tested, but no authentic AC1012 DWG fixture was available to validate that reader path. No fixtures added. |
| S8.9.10 | COMMITTED | AC1027 DXF readback exposed that `dxfRW::getVersion()` reports the normalized text-codec version AC1021 for modern inputs; `dwg2dxf` was therefore silently downgrading a FreeCAD-form DXF re-conversion. The CLI adapter now reads the declared `$ACADVER` from its copied header callback data and maps it through `DRW::dwgVersionStrings`, falling back to `getVersion()` for missing/unknown header values; the public API behavior remains unchanged. The fast `dwg2dxf_freecad_cli_compat` CTest converts tracked AC1027 DWG to DXF and re-reads/re-emits it with exact `input -o output`, requiring AC1027 after both passes. Build targets `dwg2dxf` and `libdxfrw_dwg2dxf_version_tests` pass; focused `dwg2dxf_version_policy` and `dwg2dxf_freecad_cli_compat` pass 2/2; `git diff --check` passes. |
| S8.9.11 | COMMITTED | Captures and writes the mandatory-LTYPE → DICTIONARY → XRECORD closure, including AC1014 group-330 owners, owner/reactor links, ordered XRECORD data, canonical LTYPE metadata, and fixed-handle collision remapping. Fails closed on incomplete/null children, duplicate handles, cycles, unresolved object refs in ordered or fallback data, raw payloads, or unsafe remaps. From-scratch AC1014 DXF tests verify public-reader round-trip, `hardOwner` preservation, XDATA group-1005 handle remapping, and no partial output; the external AutoCAD `point3d_R14.dwg` (SHA-256 `7e6b1378b053f20de2cf9d153da3485e6475d0cd5845d5a8144b09cfe17cdc46`) converts without being copied/tracked. Installed at `/private/tmp/libdxfrw-r14-install-final-verified/bin/dwg2dxf`, SHA-256 `6a60077d4389437e1e9dd0e2be761062d2f0b792caac432a23fde4f96c1289e4`; FreeCAD 1.1.3 rev `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, isolated FreeCADCmd, C++ importer mode 2, PATH discovery, `DWGConversion=1` and ODA/QCAD fallbacks disabled: exact `[binary,input,-o,output]` exits 0; the identical AC1014 DXF imports as one valid POINT vertex `(50,50,50)`, no edges/faces/unsupported features. Output DXF SHA-256 `2a7910281628144cf806a9733f076ac30f3a9248cad8de174acd1955e39ffd8a`. Focused eight-test suite passes: fast-focus, graph-preservation, writer-primitives, writer-version-matrix, DXF fixtures, DWG fixtures, CLI version policy, and FreeCAD CLI compatibility; `git diff --check` passes. Corrected one stale object-vector assertion that contradicted S0.7's already-committed null-DWG-reference contract. No external fixture, output, or config entered the repo. Qualifies only this macOS AC1014 POINT `open()` tuple; other versions/entities/operations/platforms remain independently gated. |
| S8.15.3.1 | COMMITTED | External AutoCAD-authored `nextgis/dwg_samples/point3d_2013.dwg` (AutoCAD 2016, `_POINT 50,50,50`, AC1027; SHA-256 `839297daa100c318d5afbfa128145d2c4782300650c86b2ab58b34cf6fe1aba4`) agrees with both its paired source DXF and LibreDWG 0.14 direct export: exactly one POINT at `(50,50,50)`. The default-OFF `LIBDXFRW_ENABLE_FREECAD_AUTOCAD_POINT3D_CONTROL` path/hash-pinned opt-in test copies the caller-supplied sample only under unique system temp, verifies installed/build converter identity, exact FreeCAD argv, AC1027 converter output and readback, isolated user config and explicit macro PASS marker. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, resolves the installed converter on PATH and creates one vertex-only `Part::Feature` at `(50,50,50)`, with zero edges/faces and no unsupported POINT. The opt-in `dwg2dxf_freecad_autocad_ac1027_point3d` passes 1/1 outside the sandbox; a sandboxed run aborts in Qt before app code because host `neon` is hidden. No external DWG/DXF fixture is tracked or staged. This is a single target-authored entity/version/pinned-runtime witness only. |

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
7. `dwg2dxf` is installed to the configured bindir, discoverable through
   FreeCAD's configured converter path or platform-specific PATH search,
   accepts FreeCAD's exact noninteractive `input -o output` call, and preserves
   the existing positional interface. Both `Draft.importDWG.open()` and
   `Draft.importDWG.insert()` must pass their exact successful conversion
   output to `importDXF.open()` / `importDXF.insert()` respectively, with the
   importer outcome (expected geometry or an explicit unsupported result)
   checked in the opened document or insertion target.
   Because FreeCAD currently checks
   output-path existence rather than the converter's exit status, failed
   conversions must not publish a final/partial DXF at that path. Verify the
   intended executable was invoked and the identical output was imported.
   Report converter output validation separately from an actual FreeCAD DXF
   import; neither alone promotes broad DWG/3D support. State explicitly that
   this repository supplies the FreeCAD import-side `dwg2dxf` executable, not
   its paired `dxf2dwg` export program; a configured shared converter
   preference may make that missing sibling relevant to export. A claim that
   users can invoke this converter from FreeCAD's desktop application also
   requires the bounded GUI-process open/insert and executable-attribution
   smoke in S8.9.6; `freecadcmd` alone does not qualify desktop launch
   registration/environment. That smoke verifies imported document geometry,
   not viewport rendering. For modeler entities, an end-to-end converter
   handoff may be supported even when FreeCAD reports the DXF entity as
   unsupported; record this as converter integration, not FreeCAD geometry
   support. SAT carrier text equality and a result for the distinct DXF
   `SOLID` entity cannot qualify FreeCAD `3DSOLID` B-rep construction.
8. The opt-in FreeCAD `open()` and `insert()` lanes each include a verified
   nonzero-Z geometry result from their complete DWG→`dwg2dxf`→DXF→pinned
   FreeCAD importer route (S8.14 and S8.15.15 respectively). Keep those
   entry-point results separate: one route never substitutes for the other.
   Each is presently only a locally generated AC1015 LINE on one pinned
   headless macOS runtime. Broader family rows record imported shape
   semantics and failures independently; a correct but unsupported DXF
   record remains converter success plus a downstream limitation, not a
   reason to substitute another entity or claim import.
9. The AC1015 SAT-v1 FreeCAD consumer row (S8.15.17) verifies the installed
   converter selection, both Draft entry-point handoffs, and the importer
   outcome on a pinned profile. A negative/unsupported importer result may
   close the converter-integration check but leaves `3DSOLID` geometry
   unsupported; only an independent B-rep oracle and matching geometry can
   promote that claim.
10. Qualify FreeCAD's Automatic converter preference (`DWGConversion=0`)
    separately from LibreDWG-only mode (`DWGConversion=1`) before describing
    automatic selection as a verified setup: prove the installed `dwg2dxf`
    runs first and succeeds without an ODA/QCAD fallback. The macOS bounded
    check is S8.9.12; native platform claims remain under S8.9.5.

## References and evidence hierarchy

Normative references take precedence over implementation analogies. Open-source
implementations are cross-checks, not authorities when they conflict with the
format specification.

- [Open Design Specification for .dwg files, v5.4.1](https://www.opendesign.com/files/guestdownloads/OpenDesign_Specification_for_.dwg_files.pdf) — relevant coverage includes §20.4.11 for 2D VERTEX (Z normally 0; elevation/thickness inherited from its POLYLINE) and §§20.4.13–20.4.17 for vertex/polyline families, §20.4.33–20.4.36 for PFACE/classic POLYLINE mesh/SOLID/TRACE, §20.4.40 for SPLINE scenario/control/fit fields, and §20.4.41 for REGION/3DSOLID/BODY ACIS layout. Searchable-text and rendered-page review confirmed these passages; no named modern `AcDbSubDMesh`, `AcDb*Surface`, or `3DLINE` layout was found, so keep those corresponding DWG rows unqualified absent another primary source. The required local v5.4.1 copy was verified under S0.2 at `/Users/dli/doc/dwg/OpenDesign_Specification_for_.dwg_files (1).pdf`.
- [AutoCAD 2010 DXF Reference](https://images.autodesk.com/adsk/files/acad_dxf1.pdf) and [current Autodesk 3DSOLID DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm) — 3DSOLID/SURFACE ACIS payloads use groups 1/3; group 310 is not specified for the entity-inline ACIS body.
- [Autodesk DXF ENTITIES reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-7D07C886-FD1D-4A0C-A7AB-B4D21F18E484.htm) — record-family index and group-code reference.
- [Autodesk ARC DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-0B14D8F1-0EBA-44BF-9108-57D8CE614BC8.htm) and [CIRCLE DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-8663262B-222C-414D-B133-4A8506A27C18.htm) — ARC/CIRCLE center coordinates are OCS, group 210/220/230 supplies the extrusion normal, and ARC angles are in that OCS frame.
- [Autodesk OCS](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm) and [arbitrary-axis algorithm](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-E19E5B42-0CC7-4EBA-B29F-5E1D595149EE.htm) — basis construction for the independent expected-WCS oracle; normative for the coordinate-frame check, unlike a reader/writer round trip.
- [Autodesk 3DFACE DXF reference](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm) — WCS corners, optional fourth vertex, invisible-edge bits.
- [Autodesk POLYLINE DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-ABF6B778-BE20-4B49-9B58-A94E64CEFFF3.htm) — flags, 2D OCS versus 3D WCS, mesh counts, extrusion.
- [Autodesk VERTEX DXF reference](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-0741E831-599E-4CBF-91E1-8ADBCFD6556D.htm) and [Polyface Meshes DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-96B6288E-F413-46C0-968A-A314171C0AAE.htm) — polyface vertex/face flags, signed invisible-edge indices, face ordering, and vertex/face counts.
- [Autodesk SEQEND DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-FD4FAA74-1F6D-45F6-B132-BF0C4BE6CC3B.htm) — sequence terminator fields; SEQEND is an R12-era entity and has no class-specific subclass data.
- [Autodesk MESH DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-4B9ADA67-87C8-4673-A579-6E4C76FF7025.htm) — group-code sequence for `AcDbSubDMesh`.
- [Autodesk 3DSOLID DXF reference](https://help.autodesk.com/cloudhelp/2020/ENU/AutoCAD-DXF/files/GUID-19AB1C40-0BE0-4F32-BCAB-04B37044A0D3.htm) — ACIS/modeler version and proprietary payload groups.
- [Autodesk object-name reference](https://help.autodesk.com/cloudhelp/2016/ENU/AutoCAD-Customization/files/GUID-ECB6F2FF-6680-4514-86A7-7AD5551E378D.htm) — confirms DXF/object names for the AutoCAD surface families, `MESH`, `3DSOLID`, and other extension-sensitive classes; use as a class-name inventory, not a substitute for group-code or DWG byte-layout documentation.
- [Autodesk OCS in DXF](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm) — arbitrary-axis basis and distinction between WCS 3D entities and planar OCS entities.
- [Autodesk SOLID DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-E0C5F04E-D0C5-48F5-AC09-32733E8848F2.htm), [TRACE DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-EA6FBCA8-1AD6-4FB2-B149-770313E93511.htm), [ELLIPSE DXF reference](https://help.autodesk.com/cloudhelp/2023/ENU/AutoCAD-DXF/files/GUID-107CB04F-AD4D-4D2F-8EC9-AC90888063AB.htm), and [INSERT DXF reference](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-28FA4CFB-9D5E-4880-9F11-36C97578252F.htm) — SOLID's optional fourth-corner fallback, TRACE's OCS corner fields, ELLIPSE WCS axes/center, and INSERT/MINSERT's OCS insertion point, scale/rotation, column/row counts, and spacings.
- [Autodesk SPLINE DXF reference](https://help.autodesk.com/cloudhelp/2018/ENU/AutoCAD-DXF/files/GUID-E1F884F8-AA90-4864-A215-3182D47A9C74.htm) — SPLINE flag bit 8, degree, knots, weights, WCS control/fit points, and optional planar normal groups; use it with ODA §20.4.40 to keep DWG-stored geometry distinct from DXF-derived planarity metadata.
- [Autodesk POLYLINE DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-ABF6B778-BE20-4B49-9B58-A94E64CEFFF3.htm) and [VERTEX DXF reference](https://help.autodesk.com/cloudhelp/2015/ENU/AutoCAD-DXF/files/GUID-0741E831-599E-4CBF-91E1-8ADBCFD6556D.htm) — the parent 2D POLYLINE carries elevation in OCS; 2D VERTEX location fields are OCS coordinates. Use them with ODA §20.4.11/.16 for the S8.15.14 group-30 serialization correction.
- [Autodesk Object Coordinate Systems in DXF](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm) — defines the OCS basis/elevation relation and the rule that planar entities' points are OCS coordinates; use the arbitrary-axis algorithm and the entity extrusion vector to compute independent WCS expectations. FreeCAD's pinned runtime, not this specification, determines its observed importer behavior.
- [Autodesk arbitrary-axis algorithm](https://help.autodesk.com/view/OARX/2026/ENU/?guid=GUID-E19E5B42-0CC7-4EBA-B29F-5E1D595149EE) and [BlockReference.BlockTransform](https://help.autodesk.com/cloudhelp/2018/ENU/OARX-ManagedRefGuide/files/OREFNET-Autodesk_AutoCAD_DatabaseServices_BlockReference_BlockTransform.html) — independent basis and block-transform semantics used by the INSERT/MINSERT expected-value oracle.
- [ezdxf INSERT implementation](https://github.com/mozman/ezdxf/blob/master/src/ezdxf/entities/insert.py) — open-source cross-check for nested INSERT and MINSERT grid offsets (array spacing is rotated by insertion angle without applying block scale); implementation analogy only, not normative evidence.
- [ezdxf ACIS FAQ](https://ezdxf.readthedocs.io/en/stable/faq.html) — cross-project statement that DXF R2000–R2010 uses SAT in entity records and R2013+ uses SAB in ACDSDATA; treat as a high-value implementation cross-check and confirm acceptance with Autodesk/real-file witnesses.
- [Autodesk DXF ENTITIES index](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-7D07C886-FD1D-4A0C-A7AB-B4D21F18E484.htm) — current listed record families; `3DLINE` is not listed there, so qualify it as a portability-sensitive extension.
- [LibreDWG entity/object definitions](https://github.com/LibreDWG/libredwg/blob/master/src/objects.in) and [LibreDWG manual](https://www.gnu.org/software/libredwg/manual/LibreDWG.html) — open-source entity names and implementation coverage; check stability per family.
- [FreeCAD `Draft/importDWG.py`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py) — current configured-path/PATH discovery, external `dwg2dxf <input> -o <output>` invocation, output-existence success check, and distinct `importDXF.open()` / `importDXF.insert()` handoffs; this establishes the converter CLI/publication contract, not entity support. Source reviewed 2026-09-24; `main` is mutable and each runtime result must pin a commit.
- [FreeCAD `Draft/importDWG.py` LibreDWG resolution](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py#L129-L165) and [converter invocation/fallback logic](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py#L244-L335) — `get_libredwg_converter()` selection from the `TeighaFileConverter` preference or platform PATH (`dwg2dxf.exe` on Windows, `dwg2dxf` on Linux/macOS), exact `input -o output` invocation, output-existence success check, and LibreDWG-only versus automatic ODA/QCAD fallback lanes. Pin an exact FreeCAD revision for each runtime test because `main` is mutable.
- [FreeCAD Draft DWG registration](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/Init.py#L33-L34), [GUI open/insert dispatch](https://github.com/FreeCAD/FreeCAD/blob/main/src/Gui/Application.cpp#L742-L749), and [`module_io.OpenInsertObject`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/freecad/module_io.py#L1-L20) — the desktop registers `importDWG` for `.dwg`, then routes open/import through the registered module/method. S8.9.6 exercises that dispatcher in a GUI process; it does not simulate a file-dialog click. `main` is mutable; pin the exact source/runtime revision for qualification.
- [FreeCAD `Draft/importDWG.py` current source anchors](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDWG.py#L52-L97) — `open()` and `insert()` call `convertToDxf()` and then dispatch to separate DXF entry points; use with the registration/GUI dispatch sources above when auditing the desktop route. The link is mutable; pin the exact source/runtime revision before claiming desktop support.
- [FreeCAD C++ DXF entity dispatch](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2743-L2782) and [SOLID face construction](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/ImpExpDxf.cpp#L1226-L1266) — the current dispatcher has a `SOLID` path but no `3DFACE` branch; `OnReadSolid()` constructs a closed wire using the 1-2-4-3 corner ordering and makes a face. Mutable `main` source selects S8.15.12's candidate and analytic checks only; it is not pinned runtime evidence or a support claim.
- [FreeCAD C++ POLYLINE/LWPOLYLINE dispatch](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2760-L2778) and [`OnReadPolyline()`](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/ImpExpDxf.cpp#L1334-L1360) — current `main` routes these entities through its C++ reader/wire callback. This guides S8.15.13/.14 fixture selection only: mutable source is not semantic evidence, and the pinned S8.15.14 runtime demonstrates that a successfully-created classic 2D POLYLINE wire can still lose its elevation and OCS placement. Keep that row downstream-unsupported until a pinned consumer produces the independently expected WCS geometry.
- [FreeCAD C++ DXF entity dispatcher](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2040-L2062) and [`ReadEntity()` dispatch](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2720-L2782) — current `main` maps SPLINE to `ReadSpline()` and sends unlisted HELIX to `ReadUnknownEntity()`. This is a downstream importer limitation, not evidence to rewrite or omit a legal DXF HELIX.
- [FreeCAD C++ ELLIPSE parser](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2135-L2150) and [shape construction callback](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/ImpExpDxf.cpp#L1122-L1160) — current `main` reads center, major-axis vector, ratio, and parameters without reading the extrusion normal; the shape callback derives XY rotation, sets +Z, and constructs a complete ellipse while ignoring arc endpoints. This defines a FreeCAD importer limitation only; preserve DXF ELLIPSE fields in `dwg2dxf` and keep the tilted/partial rows unsupported until the consumer path is semantics-correct.
- [FreeCAD pinned C++ DXF shape construction](https://github.com/FreeCAD/FreeCAD/blob/145529fe741292ff0b3977a01195bf0247425794/src/Mod/Import/App/dxf/ImpExpDxf.cpp) — exact S8.15.10 runtime revision; use alongside its macro's geometry observation. The current `main` parser source remains a mutable audit lead and cannot substitute for pinned runtime evidence.
- [FreeCAD C++ DXF importer](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/ImpExpDxf.cpp) and [FreeCAD Draft DXF importer](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Draft/importDXF.py) — primary implementation references for the C++ shape-construction path and legacy Python importer; audit the exact runtime revision and preferences because support differs by importer and release.
- [FreeCAD C++ block composition](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/ImpExpDxf.cpp) — constructs parametric `App::Link` objects for INSERTs; the pinned S8.15.6 runtime assertion checks the imported link's resulting edge geometry rather than assuming a DXF INSERT record alone constitutes successful import.
- [FreeCAD C++ INSERT parser](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2231-L2245) and [entity dispatch](https://github.com/FreeCAD/FreeCAD/blob/main/src/Mod/Import/App/dxf/dxf.cpp#L2743-L2782) — current `ReadInsert()` consumes insertion point, scale, rotation, and block name but not group-70/71 array counts or group-44/45 spacing; runtime S8.15.11 confirms this pinned C++ consumer limitation. `main` is mutable; do not generalize to legacy Python or other releases.
- [LibreDWG `dwgadd` example](https://github.com/LibreDWG/libredwg/blob/master/examples/dwgadd.example) and [issue #1351](https://github.com/LibreDWG/libredwg/issues/1351) — generator syntax and a documented broken INSERT attribute-chain output; issue evidence is used only to reject that sample as a clean control, not as a format authority or libdxfrw defect claim.
- [FreeCAD Import/Export Preferences](https://github.com/FreeCAD/FreeCAD-documentation/blob/main/wiki/Import_Export_Preferences.md) — current docs state DWG conversion is external, inherits DXF settings, describe PATH/configured executable discovery and platform filenames, and distinguish the faster C++ importer from the legacy Python importer. Reviewed 2026-09-24; the documentation is mutable and runtime results remain revision-pinned.
- [FreeCAD issue #19247](https://github.com/FreeCAD/FreeCAD/issues/19247) — adjacent LibreDWG-produced-DXF import report without a reproducible input/output pair; investigation lead only.
- [ezdxf POLYLINE reference](https://ezdxf.readthedocs.io/en/stable/dxfentities/polyline.html), [Polyface tutorial](https://ezdxf.readthedocs.io/en/stable/tutorials/polyface.html), [MESH reference](https://ezdxf.readthedocs.io/en/stable/dxfentities/mesh.html), and [ACIS documentation](https://ezdxf.readthedocs.io/en/stable/acis.html) — independent DXF parser/writer cross-checks and explicit limits on arbitrary ACIS support.
- [LibreDWG issue #1411](https://github.com/LibreDWG/libredwg/issues/1411) — recent ACIS/DataStorage report retained as an investigation lead only; not a format authority or proof of libdxfrw behavior.

Latest implementation-item ledger addition:

| Item | State | Evidence / next action |
| --- | --- | --- |
| S8.15.8 | COMMITTED | Added locally-authored `tests/fixtures/dxf/ac1015_helix_freecad_control.dxf` and opt-in ODA CTest `dwg2dxf_freecad_3d_helix_cli`; generated DWG/DXF outputs remain under `build/`. The source contains a nonplanar rational HELIX (13 ordered controls/weights, 17 knots, axis `(0,0,1)`, radius 1, one turn, height 4) and a separate nonplanar weighted cubic SPLINE. The exact FreeCAD argv test and public DXF readback preserve rational group-70 flag `0x04`, degree/counts, ordered WCS controls/weights, absent planar normals, and HELIX trailer; it caught and fixed weighted DWG SPLINE output incorrectly using linear bit `0x10`. Focused internal round-trip and ODA Spline/HELIX route tests pass; no generated files enter source. Current FreeCAD `main` C++ `dxf.cpp` maps SPLINE to `ReadSpline()` and falls through to `ReadUnknownEntity()` for HELIX, so classify this as converter-supported/preserved but downstream-unsupported in that importer. Do not rewrite the entity; a positive FreeCAD HELIX shape claim requires a separate upstream importer change. Target-authored DWGs and other versions remain unqualified. |
| S8.15.9.1 | COMMITTED | Added locally authored `tests/fixtures/dwg/ac1015_ellipse_freecad_control.dwgadd`, `tests/fixtures/dxf/ac1015_ellipse_freecad_control.dxf`, and opt-in `dwg2dxf_freecad_3d_ellipse_cli`. The DWGADD recipe emits only two planar AC1015 ELLIPSE controls (full and partial) because LibreDWG `dwgadd` cannot faithfully express the elevated/tilted vectors needed by `.9.2`. In a build-tree path containing spaces, the CTest invokes exact FreeCAD argv `dwg2dxf input -o output`, verifies the AC1015 signature/header and both entity field sequences (center, WCS major axis, ratio, parameters, omitted default extrusion), then repeats the checks after public DXF readback. Focused CTest passes 1/1. The locally authored elevated/tilted DXF source is reserved for an independent writer; it is not exercised by this DWGADD test. This is self-generated planar conversion/readback evidence only, not independent DWG interoperability, FreeCAD shape support, or general ELLIPSE support. All generated DWG/DXF outputs stay under `build/`; no generated drawing fixture is tracked. |
| S8.15.9.2 | COMMITTED | Added opt-in `dwg2dxf_freecad_3d_ellipse_oda_cli` and `tests/run_freecad_3d_ellipse_oda_cli_test.cmake`. ODA File Converter 27.1.0.0 writes the locally-authored AC1015 control only under build/temp; the fast CTest checks all three independent ELLIPSE field sets before conversion and after public DXF readback, and passes 1/1. Added `tests/freecad_dwg2dxf_3d_ellipse_check.FCMacro` to enforce an isolated FreeCAD config, capture executable/hash/argv/status and identical DWG→DXF→C++ importer handoffs, then compare resulting curve plane, major-axis direction, radii, closure, parameters, and endpoints to analytic expectations. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, with `dwg2dxf` found on PATH (SHA-256 `5b7d23baf049746597bf0ca0a78141e94e260dfb0086aa3d468ac702df063f45`): the default-XY full ellipse matches; elevated and tilted partial ellipses are silently built as closed full +Z ellipses, with tilted normal/major-axis direction and arc endpoints lost, though FreeCAD reports no unsupported features. Thus converter output is field-preserving; only this full-XY control is a positive pinned-FreeCAD result, and partial/tilted rows remain downstream-unsupported. An earlier ODA `-4960` failure did not recur on unmodified CLI retry; no UI-lock bypass was used. Locally-authored DXF source SHA-256 `078a9982f049ee4c669f878b6cc8cc6dfcd292b72d8f394fe7c2c258b9fa7cc7`; generated DWG/DXF files were not added to source. Other FreeCAD versions/importers, target-authored DWG interoperability, and general ELLIPSE support remain unqualified. |
| S8.15.10 | COMMITTED | Added `tests/fixtures/dwg/create_oblique_arc_circle_freecad_control.c`, an opt-in `LIBDXFRW_ENABLE_LIBREDWG_API_FREECAD_CONTROL` CTest target, `tests/run_freecad_3d_oblique_arc_circle_cli_test.cmake`, and `tests/freecad_dwg2dxf_oblique_arc_circle_check.FCMacro`. The API generator writes only an ephemeral AC1015 DWG under the build tree because a `dwgadd 0.14` recipe could not set extrusion vectors. The fast exact-FreeCAD-argv CTest verifies one ARC (center `(10,20,30)`, radius 5, 0–90 degrees) and one CIRCLE (center `(-5,4,10)`, radius 2.5), both normal `(0.6,0,0.8)`, then verifies identical fields after public DXF readback; build and CTest pass. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ mode 2 resolves installed `dwg2dxf` by PATH, runs exact argv, imports the same output, and emits no unsupported warnings; however both entities retain their raw OCS centers and +Z axes. ARC endpoints `(15,20,30)` / `(10,25,30)` differ from the analytic WCS expectations `(2,15,36)` / `(-2,10,39)`; expected centers are `(2,10,36)` and `(2.8,-5,10.4)`, but FreeCAD keeps `(10,20,30)` and `(-5,4,10)`. Both semantic comparisons fail, so oblique ARC/CIRCLE are downstream-unsupported despite converter field preservation. ODA 27.1.0.0 rejected this API-generated DWG's dictionary object; no independent-writer or target-authored DWG interoperability is claimed. No generated DWG/DXF was committed. `cmake --build build --target libdxfrw_freecad_oblique_arc_circle_generator dwg2dxf --parallel 2`, focused CTest (`dwg2dxf_freecad_3d_oblique_arc_circle_cli`, 1/1), and `git diff --check` pass. |

Additional implementation-item record:

| Item | State | Evidence / next action |
| --- | --- | --- |
| S8.15.11 | COMMITTED | Added locally authored `tests/fixtures/dxf/ac1015_minsert_freecad_control.dxf`, opt-in `dwg2dxf_freecad_3d_minsert_cli`, its ODA/CMake harness, and `tests/freecad_dwg2dxf_3d_minsert_check.FCMacro`. The fast test uses ODA File Converter 27.1.0.0 to generate an AC1015 DWG only under `build/`, invokes exact FreeCAD argv `dwg2dxf input -o output`, and verifies the `AcDbMInsertBlock`, referenced block/3D LINE, insertion point `(10,20,30)`, unit scales/zero rotation, two columns/two rows, and 5/4 spacing in the converter output and public DXF readback; focused CTest passes 1/1. The first control omitted the `AcDbMInsertBlock` subclass and ODA normalized its array fields to 1×1/default spacing; that invalid witness was replaced before implementation. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, invokes the installed binary (SHA-256 `5b7d23baf049746597bf0ca0a78141e94e260dfb0086aa3d468ac702df063f45`) exactly once, imports that same DXF, and reports one INSERT/one LINE without an unsupported warning, but creates only the first of four expected array edges. This confirms correct converter preservation and a silent downstream MINSERT-array limitation; do not decompose or rewrite the MINSERT or claim full FreeCAD array support. The optional runtime macro emits a separate semantic status. Build `dwg2dxf` and focused CTest pass; all generated DWG/DXF outputs remain in build/temp. No target-authored interoperability, non-default OCS, other array transforms, versions, importer modes, or general INSERT claim follows. |

Additional implementation-item record:

| Item | State | Evidence / next action |
| --- | --- | --- |
| S8.15.12 | COMMITTED | Added the locally authored `tests/fixtures/dxf/ac1015_solid_freecad_control.dxf`, opt-in `dwg2dxf_freecad_3d_solid_oda_cli`, `tests/run_freecad_3d_solid_oda_cli_test.cmake`, and `tests/freecad_dwg2dxf_3d_solid_check.FCMacro`. ODA File Converter 27.1.0.0 generated AC1015 only under `build/`; focused exact-FreeCAD-argv conversion/readback CTest passes 1/1 and verifies all twelve corner coordinates (group 12 is corner 4; group 13 is corner 3). FreeCAD 1.1.3 full revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, with isolated user config and installed `dwg2dxf` SHA-256 `4b959ec93a99da102507f3b53d7cbf2d083f12f62e86e96afa914c97894fa0e3`, runs exact `[binary,input,-o,output]` once and imports that same output; it counts one SOLID, reports one `Entity type 'SOLID'` unsupported diagnostic, and creates zero shapes. The macro asserts that negative outcome rather than promoting a face-import claim. A first invalid control had groups 12/13 swapped; the corrected control still confirms this pinned release's downstream gap. Generated DWG/DXF files remain in build/temp; only the local-from-scratch DXF source and code are tracked. Mutable `FreeCAD/main` has a newer SOLID path, but that code is not the pinned binary. A future positive geometry check requires running an exact FreeCAD build with that implementation and checking area 12, four edges, four corners, and z=5 bounds. No AutoCAD-authored, arbitrary-OCS, TRACE/3DFACE, other-version, or general SOLID claim. |
| S8.15.13 | COMMITTED | Added locally authored `tests/fixtures/dxf/ac1015_lwpolyline_freecad_control.dxf`, opt-in `dwg2dxf_freecad_2d_lwpolyline_oda_cli`, `tests/run_freecad_2d_lwpolyline_oda_cli_test.cmake`, and `tests/freecad_dwg2dxf_2d_lwpolyline_check.FCMacro`. ODA File Converter 27.1.0.0 generates an AC1015 DWG only under `build/`; exact FreeCAD argv conversion and public DXF readback preserve one closed four-vertex LWPOLYLINE in order; focused CTest passes 1/1. FreeCAD 1.1.3 full revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, invokes installed `/opt/homebrew/bin/dwg2dxf` via PATH (SHA-256 `e4cca8e3f5af9a878eb0085ed14e992f64d35f03065743847701274a46781c99`) exactly once and imports the same output. It creates one valid closed wire/four edges with the expected four vertices and `(0,0,0)`–`(4,3,0)` bounds, counts one LWPOLYLINE, and reports no unsupported entities. The generated DWG produced class-stability and unknown-codepage warnings, but the converter and semantic checks passed. Source DXF SHA-256 is `a09e2b9d83576ed71aca26f54db4d1be848a01c9ddf734306cc9c3f431034fef`; generated DWG/DXFs remain in build/temp. This is one local AC1015/ODA/FreeCAD consumer baseline only—not target-authored interoperability, 3D evidence, non-default OCS, bulges, widths, thickness, other importer modes/releases, or general polyline support. |
| S8.15.14 | COMMITTED | Added the locally authored `tests/fixtures/dxf/ac1015_2d_polyline_ocs_freecad_control.dxf`, opt-in ODA CTest `dwg2dxf_freecad_3d_2d_polyline_ocs_oda_cli`, `tests/run_freecad_2d_polyline_ocs_oda_cli_test.cmake`, and pinned-consumer probe `tests/freecad_dwg2dxf_3d_2d_polyline_ocs_check.FCMacro`. The fixture is authored from scratch. ODA File Converter 27.1.0.0 generates its AC1015 DWG only under ignored `build/`; the exact FreeCAD CLI (`dwg2dxf input -o output`) and public DXF readback preserve one closed 2D POLYLINE, ordered OCS vertex tuples `(0,0,0),(4,0,0),(4,3,0),(0,3,0)`, parent elevation 5, and extrusion `(0,0.6,0.8)`. ODA v5.4.1 §§20.4.11/.16 state that 2D VERTEX Z is zero (elevation/thickness are on parent POLYLINE); the first control exposed the writer copying the parsed parent elevation into each VERTEX group 30, so `dxfRW::writePolyline` now serializes zero for typed DWG `Vertex2D` while leaving the parent elevation intact. The strengthened CTest verifies that field as well as the subtype, order, closure, normal, and elevation before/after DXF readback; it passes 1/1. `cmake --build build --target dwg2dxf --parallel 2` and the focused local-roundtrip/version-policy/FreeCAD CLI plus 2D/3D POLYLINE/MESH/PFACE/ODA control suite pass; filtered CTest passes 9/9. FreeCAD 1.1.3 full revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ mode 2, resolves this checkout's `build/dwg2dxf/dwg2dxf` first on PATH (SHA-256 `bac02b1df67c515cff5b88c8a3369ea4e05e60d4234a409be9054438c8ac40c2`), invokes the exact binary once on a path with spaces, and imports that same successful output. It creates one valid closed four-edge wire but silently ignores parent elevation and oblique extrusion: vertices/bounds are flat at Z=0 rather than WCS `(-4,0.6,5.8),(-4,3,4),(0,0.6,5.8),(0,3,4)`; no unsupported feature is reported. The probe emits `FREECAD_DWG2DXF_2D_POLYLINE_OCS_LIMITATION_CONFIRMED=` and records this as downstream-unsupported, not a converter reason to emit nonstandard coordinates. Generated DWG/DXF and isolated FreeCAD profiles stay under `build/` or system temp; no generated fixture is committed. This is one AC1015 generated-control and pinned `open()`/C++/macOS/PATH profile only—not target-authored DWG interoperability, all OCS/elevation combinations, `insert()`, desktop GUI, other platforms/importers/releases, or general POLYLINE support. |
| S8.15.15 | COMMITTED | Generalized `tests/freecad_dwg2dxf_import_check.FCMacro` with a validated `LIBDXFRW_FREECAD_EXPECT_LINE_BOUNDS` override so the existing isolated harness can assert nonzero-Z geometry on `insert()` without another framework. Reused the locally-authored `tests/fixtures/dwg/ac1015_3d_line_control.dwgadd`; LibreDWG `dwgadd`'s AC1015 output was generated under `build/` then copied under an input path containing spaces in system temp. FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, default C++ importer mode 2 and isolated `--user-cfg`: `Draft.importDWG.insert()` found this checkout's `build/dwg2dxf/dwg2dxf` through PATH (SHA-256 `bac02b1df67c515cff5b88c8a3369ea4e05e60d4234a409be9054438c8ac40c2`), invoked it once with exact argv `[binary,input,-o,output]`, got status 0, and passed the identical output path plus target document `FreeCADDwgInsertCheck` to `importDXF.insert()`. The C++ importer created exactly one LINE edge with independently specified XYZ bounds `(1,2,3)-(4,6,9)` and no unsupported features; the macro emitted its explicit PASS marker. `cmake --build build --target dwg2dxf --parallel 2` passed; focused `dwg2dxf_version_policy`, `dwg2dxf_freecad_cli_compat`, and `dwg2dxf_freecad_3d_line_cli` CTests passed 3/3; Python macro syntax and `git diff --check` pass. All generated DWG/DXF and profiles remain in build/temp; no drawing binary added. This adds one generated-control/headless `insert()` result only—not desktop GUI dispatch, target-authored DWG interoperability, other platform/revision/importer, or general 3D support. |
| S8.15.16 | COMMITTED | The bounded AC1015 SAT-v1 path (ODA v5.4.1 §20.4.41) retains declared ranges/source version, applies the documented character transform, and emits a DXF group-1/3 text carrier only for qualified source/version/ranges. Generic modeler text carriers are unchanged; incomplete/wrong-version frames stay opaque, and the DWG `unknown` bit is neither assigned semantics nor claimed to round-trip through DXF. Synthetic frame extraction and DXF export/import/re-export assertions pass. The opt-in `LIBDXFRW_ENABLE_EXTERNAL_SAT_V1_CLI_CONTROL`, `tests/run_freecad_sat_v1_external_cli_test.cmake.in`, and `tests/modeler_sat_dxf_compare.cpp` compare the external LibreDWG `test-data/2000/Cone.dwg` (SHA-256 `a444b0148dd58bb4269bf80dbda3f81dfc0a09980fb9713e89de7ff6b1c396f4`) with its `dwgread -O DXF` reference; exact `input -o output` conversion matches one `3DSOLID` across 30 SAT group-1/3 code/value pairs. Caller-supplied source/reference files are never staged, and the test is OFF by default. Focused local-roundtrip, version-policy, FreeCAD CLI-compatibility, and external SAT-v1 CLI tests pass 4/4. This verifies one external-writer AC1015 `3DSOLID` converter path only—not AutoCAD-authored input, other entities/versions, DWG modeler writing, SAB/DataStorage association, or FreeCAD geometry support. |
| S8.15.17 | COMMITTED | Added opt-in `LIBDXFRW_ENABLE_FREECAD_SAT_V1_RUNTIME_CONTROL`, `tests/run_freecad_sat_v1_runtime_test.cmake.in`, and a `3DSOLID` mode in `tests/freecad_dwg2dxf_import_check.FCMacro`; reuses the existing isolated profile/attribution code and stays OFF by default. With external LibreDWG `Cone.dwg` (SHA-256 `a444b0148dd58bb4269bf80dbda3f81dfc0a09980fb9713e89de7ff6b1c396f4`) and reference DXF (SHA-256 `6e6ec37a78261d0786537f87b65cb6b5f546d499c5b53a20a7a86fa9e0a7cda9`), both `dwg2dxf_freecad_sat_v1_open` and `_insert` pass using FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, `DWGConversion=1`, and PATH discovery. They invoke the installed `/private/tmp/libdxfrw-freecad-install.0XOVoo/bin/dwg2dxf` (SHA-256 `5cf1832e7c5d5c2d82f1b05f42aa82dc972069edbc79046043c4a81da1aaa44d`) exactly once with `[binary,input,-o,output]`, all paths include spaces, and each hands that same DXF to `importDXF.open()`/`insert()`. The emitted AC1015 DXF matches the reference's one `3DSOLID` and all 30 SAT code/value pairs. FreeCAD reports `Entity type '3DSOLID'` unsupported (line 1950, handle 40), `totalEntitiesCreated=0`, and no shapes/solids in both operations. This is converter integration only, not FreeCAD B-rep support; no `SOLID` analogy or SAT-text equality promotes geometry. CMake tests additionally require the explicit macro PASS marker, operation, carrier comparison, installed binary identity, and importer outcome. Focused roundtrip/version/CLI/external-SAT/FreeCAD open+insert suite passes 6/6; macro AST validation and `git diff --check` pass. All runtime inputs, profiles, and DXFs remain external or under a unique system-temp root and were removed after success; no fixture was added. This verifies one external AC1015 source and headless FreeCAD profile only—not desktop-dispatch 3DSOLID, target-authored DWGs, other platforms/importers/versions, or FreeCAD solid geometry support. |

Additional implementation-item record:

| Item | State | Evidence / next action |
| --- | --- | --- |
| S8.15.18 | COMMITTED | Added locally authored tests/fixtures/dxf/ac1015_3dface_freecad_control.dxf, optional fast dwg2dxf_freecad_3dface_oda_cli, optional dwg2dxf_freecad_3dface_handoff FreeCAD runtime CTest, and a pinned negative mode in tests/freecad_dwg2dxf_import_check.FCMacro. The tilted AC1015 3DFACE has WCS corners (0,0,0),(4,0,0),(4,3,4),(0,3,4), and invisible-edge flag 4. Autodesk's DXF reference defines the corners as WCS and group 70 as independent invisible-edge flags. ODA File Converter 27.1.0.0 writes only an ephemeral DWG; the fast script passes exact input -o output conversion and public DXF readback with all coordinates/flag retained. FreeCAD 1.1.3 revision 145529fe741292ff0b3977a01195bf0247425794, macOS 27 arm64, C++ mode 2, DWGConversion=1, isolated profile/PATH discovery: installed /private/tmp/libdxfrw-freecad-3dface-20260924/bin/dwg2dxf (SHA-256 6a60077d4389437e1e9dd0e2be761062d2f0b792caac432a23fde4f96c1289e4) receives exact argv, exits 0, and hands the same DXF to the importer. The pinned importer counts one 3DFACE (handle E6), reports it unsupported, and creates zero shapes; this is converter integration only, not FreeCAD geometry support. Source DXF SHA-256 d79d3d532b5c1b2364a35773e20fd98c6f7e1af576e0527d3f3a327e4510d1dc; runtime DWG SHA-256 5f1a561849721bd5810cb09527cce538fcb7e1e476e43358e62b8f76a24af31a; converted DXF SHA-256 79aa28f44b150011cc88c1a489455d90cc3f39ab52327a7dcdacf79c5a04e2dd. CMake registration, fast script, runtime handoff/negative assertion, Python AST parse, and git diff --check pass. Both CTests are opt-in; no generated DWG/DXF is tracked. Future geometry qualification requires a pinned FreeCAD build that imports 3DFACE plus independent vertex/area checks; target-authored DWG/version evidence remains open. |
| S8.15.18.1 | COMMITTED | Split the prior optional FreeCAD runtime check into independent `dwg2dxf_freecad_3dface_open_handoff` and `..._insert_handoff` CTests, with separate isolated profiles and explicit operation assertion. Both run the exact installed binary with FreeCAD's argv `[dwg2dxf, input, -o, output]` and require that exact output be passed to the corresponding `importDXF.open()` or `insert()` call; insert also verifies `FreeCADDwgInsertCheck` as its destination document. On FreeCAD 1.1.3 revision `145529fe741292ff0b3977a01195bf0247425794`, macOS 27 arm64, C++ importer mode 2, both consume one 3DFACE and independently confirm the same pinned importer gap: unsupported entity and zero shapes. The result qualifies both converter handoff routes only; it does not promote FreeCAD geometry support. Both runtime CTests are opt-in. |
| S8.15.18.2 | COMMITTED | Generalized the locally-authored 3DFACE control to ODA target versions `ACAD2004/2007/2010/2013/2018` (DWG signatures AC1018/1021/1024/1027/1032), while retaining ACAD2000/AC1015. All generated DWGs remain under ignored `build/`. Each opt-in CTest checks magic/header version, one record, four tilted WCS corners and invisible-edge flag 4 in `dwg2dxf` output and public DXF readback; when installed, LibreDWG `dwgread -O DXF` is an independent reader gate on the same generated DWG. ODA File Converter 27.1.0.0 and LibreDWG 0.14 pass 6/6 versions in 41.06 seconds; the implementation is isolated to the exact 3DFACE/WCS/flag vector. This does not claim target-authored samples, complete parser coverage for those releases, or FreeCAD geometry support. The external controls stay opt-in; no generated DWG/DXF fixture is committed. |
| S8.13.1 | COMMITTED | Closed a FreeCAD-converter adapter gap: `dxfRW::writeViewport()` existed, but `dwg2dxf`'s `dx_iface::writeEntity()` omitted `DRW::VIEWPORT`, so DXF→DXF CLI conversion dropped viewport records. Added typed dispatch and corrected group 292 (`useDefaultLighting`) from 16-bit integer output to the binary-DXF boolean representation. Runtime-generated ASCII/binary library round trips preserve the lighting flag, following group 282 value, and viewport fields; the locally-authored build-tree CLI control verifies viewport retention and binary/text field alignment across two exact `[dwg2dxf, input, -o, output]` passes. `libdxfrw_dwg_local_roundtrip`, `dwg2dxf_freecad_cli_compat`, and `dwg2dxf_raw_dxf_section_passthrough` pass 3/3. `git diff --check` passes; no drawing fixture was added. This qualifies converter/readback behavior only, not FreeCAD viewport geometry/rendering, DWG viewport parsing, or native Linux/Windows execution. |
| S8.13.2 | COMMITTED | Closed two more typed-output gaps in the FreeCAD converter adapter: import already stores `RAY`/`XLINE`, and `dxfRW` already has typed writers, but `dx_iface::writeEntity()` omitted both. Added dispatch and runtime-generated ASCII/binary round trips for nonzero-Z base points and independent direction vectors (including normalized oblique XLINE direction); the build-tree exact-argv CLI control checks each entity's WCS base/direction over two DXF→DXF passes. `cmake --build build --target libdxfrw_dwg_local_roundtrip dwg2dxf --parallel 2` and the focused `libdxfrw_dwg_local_roundtrip`, `dwg2dxf_freecad_cli_compat`, and `dwg2dxf_raw_dxf_section_passthrough` tests pass (3/3); `git diff --check` passes. Controls are created locally in the build tree; no test DWG/DXF fixture is committed. This is bounded converter/readback evidence only, not proof of FreeCAD importer geometry support or native Linux/Windows behavior. |
| S8.13.3 | COMMITTED | Fixed a resource-link loss in the concrete converter adapter: `dx_iface::addImage()` previously made one `dx_ifaceImg` copy for the block entity and a second for the deferred `IMAGEDEF` link list, so `linkImage()` updated an object the writer never serialized. The entity and link list now share the same owned instance. Added a runtime-generated adapter control that writes an image with a relative path, then invokes the real exact-argv `dwg2dxf input -o output` path twice and checks the image's linked path through public `dx_iface::fileImport()` readback after each pass. Built `libdxfrw_dwg2dxf_adapter_tests` and `dwg2dxf`; focused CTest (`dwg2dxf_image_path_cli`, `libdxfrw_dwg_local_roundtrip`, `dwg2dxf_freecad_cli_compat`, `dwg2dxf_raw_dxf_section_passthrough`) passes 4/4; `git diff --check` passes. Generated DXF files stay in a unique build-tree directory and are removed on success; no image or drawing fixture was committed. This is path/reference preservation only—not raster asset availability, FreeCAD image rendering, or any 3D geometry support claim. |
| S8.13.4 | COMMITTED | `dx_iface::add3DLine()` now stores `DRW_3DLine` rather than invoking the interface's compatibility fallback to `addLine()`, and `writeEntity()` dispatches `DRW::THREEDLINE` to the existing typed `dxfRW::write3DLine()`. Extended the runtime-generated adapter test with an in-memory 3DLINE carrying distinct nonzero XYZ endpoints, thickness 2.5 and oblique extrusion `(0,1,0)`; its subtype and every field survive source DXF generation, two exact FreeCAD-argv `dwg2dxf` passes and public reader readback. `cmake --build build --target libdxfrw_dwg2dxf_adapter_tests dwg2dxf --parallel 2` succeeds; focused `dwg2dxf_image_path_cli` and `dwg2dxf_3dline_adapter_cli` pass 2/2. All drawings are generated under the build tree and deleted on success; no DWG/DXF fixture was committed. This repairs only the concrete converter adapter for the existing DXF extension. The DXF ENTITIES index does not list `3DLINE`; target-DWG class/version availability, independent CAD acceptance, legacy pre-R13 behavior, and FreeCAD geometry support remain unqualified. |
| S8.13.4.1 | COMMITTED | Extended `dwg2dxf_3dline_adapter_cli` with a generated AC1014 negative vector that enters the typed `DRW::THREEDLINE` route, requires the existing AC1015 minimum-version writer guard to fail, and verifies no final DXF is published. `cmake --build build --target libdxfrw_dwg2dxf_adapter_tests --parallel 2` succeeds; `dwg2dxf_image_path_cli` and `dwg2dxf_3dline_adapter_cli` pass 2/2; `git diff --check` passes. The test leaves no drawing fixture. A 2026-09-26 authoritative-source check found Autodesk's current ENTITIES index enumerates `LINE` but not `3DLINE`; LibreDWG 0.14 manual field listings are retained as implementation cross-check only. This closes the local older-target fail-closed test, not historical DXF interoperability or modern DWG class/version evidence. |
| S8.13.4.2 | COMMITTED | Added a runtime-generated AC1027 binary DXF control for the typed 3DLINE and enabled the existing CLI harness's optional `-b` output flag for this case. The control parses binary input, emits binary output on both exact `dwg2dxf input -o output -b` passes, and checks subtype plus endpoints, thickness, and extrusion through public reader readback after each pass. `cmake --build build --target libdxfrw_dwg2dxf_adapter_tests dwg2dxf --parallel 2` succeeds; ASCII and binary `dwg2dxf_3dline_*_adapter_cli` tests pass 2/2, and `git diff --check` passes. Runtime files are removed after success; no test fixture is committed. This adds local binary codec/adapter coverage only and does not qualify `3DLINE` standards portability, target-authored DWG support, or CAD/FreeCAD acceptance. |
