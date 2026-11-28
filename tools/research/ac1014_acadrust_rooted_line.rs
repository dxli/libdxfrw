// SPDX-License-Identifier: GPL-2.0-or-later
// Scratch-only generated R14 LINE and rooted plot-style control. The producer
// reconstructs LAYOUT on reopen; this is not native AutoCAD evidence.
use acadrust::{CadDocument, DwgReader, DwgWriter};
use acadrust::entities::{EntityType, Line, Viewport};
use acadrust::objects::ObjectType;
use acadrust::tables::Ucs;
use acadrust::types::{DxfVersion, Handle};
use std::path::PathBuf;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let output = PathBuf::from(std::env::var_os("AC1012_WITNESS_OUTPUT")
        .ok_or("AC1012_WITNESS_OUTPUT is required")?);
    let mut doc = CadDocument::with_version(DxfVersion::AC1014);
    doc.add_entity(EntityType::Line(Line::from_coords(1., 2., 0., 4., 5., 0.)))?;
    let plotstyle = doc.header.acad_plotstylename_dict_handle;
    let original = output.join("r14-line.dwg");
    DwgWriter::write_to_file(&original, &doc)?;
    let mut reopened = DwgReader::from_file(&original)?.read()?;
    reopened.named_objects_dictionary_mut().ok_or("missing root")?
        .insert("ACAD_PLOTSTYLENAME", plotstyle);
    let paper_viewport = if std::env::var_os("AC1012_WITNESS_PAPER_VIEWPORT").is_some() {
        let mut viewport = Viewport::new();
        viewport.id = 1;
        Some(reopened.add_entity_to_layout(EntityType::Viewport(Box::new(viewport)), "Layout1")?)
    } else { None };
    if std::env::var_os("AC1012_WITNESS_NAMED_UCS").is_some() {
        let named = Handle::new(0x9000);
        let base = Handle::new(0x9001);
        for (name, handle) in [("Research_UCS", named), ("Research_BASE_UCS", base)] {
            let mut ucs = Ucs::new(name);
            ucs.handle = handle;
            reopened.ucss.add(ucs)?;
        }
        let mut count = 0;
        for object in reopened.objects.values_mut() {
            if let ObjectType::Layout(layout) = object {
                layout.named_ucs = named;
                layout.base_ucs = base;
                layout.ucs_ortho_type = 1;
                count += 1;
            }
        }
        assert!(count > 0, "reopened drawing must contain layouts");
        reopened.header.handle_seed = 0x9002;
    }
    let result = output.join("r14-line-rooted-resaved.dwg");
    DwgWriter::write_to_file(&result, &reopened)?;
    if std::env::var_os("AC1012_WITNESS_NAMED_UCS").is_some() {
        let readback = DwgReader::from_file(&result)?.read()?;
        assert_eq!(readback.ucss.get("Research_UCS").ok_or("missing named UCS")?.handle, Handle::new(0x9000));
        assert_eq!(readback.ucss.get("Research_BASE_UCS").ok_or("missing base UCS")?.handle, Handle::new(0x9001));
        let layouts: Vec<_> = readback.objects.values().filter_map(|object| {
            if let ObjectType::Layout(layout) = object { Some(layout) } else { None }
        }).collect();
        assert!(!layouts.is_empty());
        for layout in layouts {
            assert_eq!(layout.named_ucs, Handle::new(0x9000));
            assert_eq!(layout.base_ucs, Handle::new(0x9001));
            assert_eq!(layout.ucs_ortho_type, 1);
        }
    }
    if let Some(handle) = paper_viewport {
        let readback = DwgReader::from_file(&result)?.read()?;
        let layout = readback.objects.values().find_map(|object| {
            if let ObjectType::Layout(layout) = object {
                if layout.name == "Layout1" { Some(layout) } else { None }
            } else { None }
        }).ok_or("missing paper layout")?;
        assert_eq!(layout.viewport, handle);
    }
    Ok(())
}
