# libdxfrw 3D consumer contract

This note defines the public data-delivery contract needed by 3D-aware clients
while preserving the existing source-level contract used by 2D clients such as
LibreCAD. It is not a format/version qualification, renderer specification, or
promise that all listed fields are correctly decoded for every DWG release.

## Read mode and ownership

For a DXF or DWG client that must handle coordinates itself, call
`dxfRW::read` / `dxfRW::readAscii` or `dwgRW::read` with `ext == false`. The
public parameter is documented as applying extrusion for 2D conversion when
true. In DXF paths, that option invokes `applyExtrusion()` on a selected set of
entity types and mutates their fields; it is not a uniform 3D normalization
contract. DWG has the same opt-in behavior for a selected set of entities.
Keep the parsed format-coordinate tuples, elevation, and extrusion direction, then
apply the entity-specific DXF/DWG coordinate rules in the consuming adapter.
`ext == true` remains available for existing adapters with their current 2D
policy; do not change its behavior as a side effect of 3D-consumer work.

Callbacks expose borrowed objects as references or pointers. The interface does
not specify that callback object addresses remain valid after the callback
returns. A consumer that retains a scene/document model must copy the needed
fields (including nested vectors/strings/opaque bytes) during the callback.
Preserve entity and owner handles where relationships matter; do not assume
callback order alone is a stable ownership contract.

## Field and coordinate contract

The Autodesk DXF OCS reference distinguishes WCS coordinates for 3D entities
such as POINT, LINE, 3DFACE, 3D POLYLINE, and 3D mesh from OCS coordinates for
planar entities such as CIRCLE, ARC, SOLID, TRACE, INSERT, 2D POLYLINE,
LWPOLYLINE, and HATCH. OCS conversion depends on the entity normal/extrusion
vector, elevation, and the arbitrary-axis algorithm. The table below records
what a consumer should preserve; DWG coordinate interpretation remains
version-qualified separately.

| Family | Public callback and fields | Coordinate handling for a 3D consumer | Default / boundary |
| --- | --- | --- | --- |
| WCS point and line primitives | `addPoint(DRW_Point)`, `addLine(DRW_Line)`, `addRay(DRW_Ray)`, `addXline(DRW_Xline)`; `basePoint`, `secPoint`, `extPoint`, `thickness`, `xAxisAngle` | Preserve XYZ endpoints and direction vectors; DXF POINT/LINE locations are WCS. Keep extrusion/thickness as separate fields rather than folding them into endpoints. | Existing required callbacks. DWG interpretation still needs per-version evidence. |
| 3DLINE extension | `add3DLine(DRW_3DLine)`; inherits start/end/extrusion/thickness fields from LINE | Preserve full XYZ. Keep legacy pre-R13 type 21, modern DWG custom class, and DXF `3DLINE` extension identities distinct from ordinary LINE. | Default delegates to `addLine`; override `add3DLine` when extension identity matters. Portability/version claims remain unqualified. |
| 3DFACE | `add3dFace(DRW_3Dface)`; four WCS corners and `invisibleflag` | Preserve all four XYZ corners and each invisible-edge bit. A missing fourth DXF corner is represented by the third corner. | Required callback. No projection is performed by libdxfrw when `ext == false`. |
| Planar primitives and filled faces | `addCircle`, `addArc`, `addEllipse`, `addTrace`, `addSolid`; centers/axes/corners, radii/parameters, `extPoint`, elevation or thickness as present | Retain OCS inputs, normal/extrusion, elevation, and subtype parameters. Build the entity plane using the format rule; do not infer that OCS XY is WCS XY. TRACE/SOLID are planar faces with thickness, not ACIS 3DSOLID. | Required callbacks. The optional `ext == true` path mutates fields for selected types and is not the 3D consumer path. |
| 2D and 3D POLYLINE/VERTEX forms | `addLWPolyline(DRW_LWPolyline)`, `addPolyline(DRW_Polyline)`; flags, elevation, extrusion, vertices, bulges, M/N counts, subtype and face indices | Branch on polyline flags and each vertex subtype. 2D forms use OCS/elevation; 3D polyline vertices are WCS. Preserve child/owner handles, ordering, signed one-based polyface indices, and invisible-edge meaning. | Required callbacks. `DRW_Polyline` is a tagged family, not one coordinate model. |
| MESH / AcDbSubDMesh | `addMesh(DRW_Mesh)`; WCS `vertices`, face index lists, `edges`, `creases`, subdivision fields and `propertyOverrides` | Preserve base-cage XYZ and the complete topology/index structure. The face stream is not a triangle list by definition; do not triangulate or flatten it in the interchange adapter. | Default no-op; a 3D consumer must override it. No subdivision evaluator is supplied. DWG class/layout qualification remains separate. |
| Block placement / MINSERT | `addInsert(DRW_Insert)`; insertion `basePoint`, `extPoint`, scale factors, angle, row/column counts and spacing, block name, and any published ATTRIB data/handles | DXF INSERT insertion point is OCS. Resolve the named block and base point, compose its placement and nested parents, and retain array spacing and attached attributes. Use the spec/tested transform order; do not treat the insertion point as an already-expanded WCS transform. | Required callback. Current round-trip/matrix vectors cover a tested DXF subset only. |
| SPLINE / HELIX | `addSpline(DRW_Spline*)`, `addHelix(DRW_Helix*)`; control/fit points, knots, weights, degree, flags, normal/tangents; HELIX axis/turn/radius/handedness trailer | Preserve all XYZ tuples, scalar knot/weight arrays, flags, and HELIX-specific metadata. Do not replace the native parameter set with a sampled polyline. DXF SPLINE control/fit points are WCS; retain the normal when present. | `addSpline` is required; `addHelix` defaults to no-op. LibreCAD currently maps HELIX to its 2D spline representation; this is consumer behavior, not the library contract. No curve evaluator is supplied. |
| Analytic/NURBS surfaces | `addSurface(DRW_Surface*)` with dynamic subtype `DRW_PlaneSurface`, `DRW_ExtrudedSurface`, `DRW_RevolvedSurface`, `DRW_SweptSurface`, `DRW_LoftedSurface`, or `DRW_NurbsSurface`; subtype transforms, axes/vectors, flags, handles/references and surface metadata | Preserve the concrete subtype and every typed field/matrix/reference separately. Do not assume a universal frame or silently evaluate, tessellate, or discard subtype fields. Keep typed parameters separate from the ACIS/SAB payload and derived wireframe. | Default no-op; 3D consumers must override. Parameter presence is not proof of a valid/evaluable surface; DWG subtype layouts remain unqualified per version. |
| ACIS 3DSOLID / REGION / BODY | `addModelerGeometry(DRW_ModelerGeometry)`; type, modeler/status/version fields, history handle, raw bytes, DXF chunk/range metadata, optional DataStorage data | Treat bytes as an opaque carrier unless a supported decoder succeeds. Keep DXF SAT/SAB chunks, whole DWG object-frame bytes, linked DataStorage bytes, and proxy graphics in separate namespaces with independent identity/length/digests. A decoded wireframe is derived display data, never the source body. | Default no-op; a consumer must override to retain it. No ACIS kernel, typed DWG writer, or general semantic solid decode is supplied. |
| Proxy graphics / unsupported records | `DRW_Entity::proxyGraphics`, `addUnsupportedObject`, and related raw-section/carrier callbacks | Keep source entity identity and proxy bytes separate from its typed geometry and any derived preview. Record a stop reason; a proxy preview is not a replacement for the source entity. | Generic preservation routes only; no 3D semantic claim. Unknown `ACDSDATA` sections are not implicitly linked to an entity. |

All callback data is exact only to the extent the selected reader/version row is
qualified. This matrix is a consumer-handling contract, not a claim that every
listed parser path is complete or interoperable. Coordinate rules for DWG must
be confirmed per family/version with the applicable ODA section and authentic
target evidence before they are promoted.

## Adapter requirements

- Keep the existing 2D adapter lane intact. Projection, curve approximation,
  and sidecar recovery stay in that adapter; do not add implicit XY projection
  to `DRW_*` parsers or library callbacks.
- A 3D-aware adapter overrides default-no-op callbacks for HELIX, MESH,
  surfaces, and modeler geometry when it needs those families. `add3DLine`
  currently delegates to `addLine`, so override it if the distinction matters.
- Capture typed values before building a renderer-specific mesh/scene. Store
  WCS, OCS, and subtype-local data with an explicit frame tag; conversion to the
  scene's world frame is an adapter operation governed by the file format.
- Carry opaque data and semantic fields independently. Do not label hashes,
  byte retention, callback receipt, or a 2D wireframe preview as successful 3D
  semantic decoding.
- Adding any new callback must use a default implementation unless all existing
  public consumers are intentionally being migrated. This preserves source
  compatibility only; this project does not claim binary ABI compatibility.

## Evidence and references

Implementation contract: [`DRW_Interface`](../src/drw_interface.h),
[`DRW_*` entity fields](../src/drw_entities.h),
[`dxfRW::read`](../src/libdxfrw.h), [`dwgRW::read`](../src/libdwgr.h), and the
existing [`LibreCAD_3 source-compatibility check`](../tests/lc3_compat_check.cpp).

Primary coordinate references: Autodesk's [Object Coordinate Systems in
DXF](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-D99F1509-E4E4-47A3-8691-92EA07DC88F5.htm),
[INSERT](https://help.autodesk.com/cloudhelp/2021/ENU/AutoCAD-DXF/files/GUID-28FA4CFB-9D5E-4880-9F11-36C97578252F.htm),
[SPLINE](https://help.autodesk.com/view/OARX/2018/ENU/?guid=GUID-E1F884F8-AA90-4864-A215-3182D47A9C74),
[3DFACE](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-747865D5-51F0-45F2-BEFE-9572DBC5B151.htm),
[POINT](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-9C6AD32D-769D-4213-85A4-CA9CCB5C5317.htm),
and [LINE](https://help.autodesk.com/cloudhelp/2024/ENU/AutoCAD-DXF/files/GUID-FCEF5726-53AE-4C43-B4EA-C84EB8686A66.htm).
