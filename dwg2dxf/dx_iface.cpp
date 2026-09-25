/******************************************************************************
**  dwg2dxf - Program to convert dwg/dxf to dxf(ascii & binary)              **
**                                                                           **
**  Copyright (C) 2015 José F. Soriano, rallazz@gmail.com                    **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include <iostream>
#include <algorithm>
#include <charconv>
#include <functional>
#include <limits>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "dx_iface.h"
#include "libdwgr.h"
#include "libdxfrw.h"

namespace {

DRW::Version sourceVersionFromDxfHeader(
    const DRW_Header& header, DRW::Version fallback) {
    const auto acadVersion = header.vars.find("$ACADVER");
    if (acadVersion == header.vars.end() || acadVersion->second == nullptr
            || acadVersion->second->type() != DRW_Variant::STRING) {
        return fallback;
    }

    const std::string value = acadVersion->second->c_str();
    for (const auto& versionEntry : DRW::dwgVersionStrings) {
        if (versionEntry.first != nullptr && value == versionEntry.first)
            return versionEntry.second;
    }
    return fallback;
}

} // namespace

bool dx_iface::prepareExtensionObjectGraph(dx_data* data) {
    m_extensionDictionaryHandles.clear();
    m_extensionXRecordHandles.clear();
    if (data == nullptr)
        return false;

    try {
        std::unordered_map<std::uint32_t, const DRW_Dictionary*> dictionaries;
        std::unordered_map<std::uint32_t, const DRW_XRecord*> xrecords;
        std::unordered_set<std::uint32_t> sourceHandles;
        std::unordered_set<std::uint32_t> linetypeHandles;
        for (const DRW_LType& lineType : data->lineTypes) {
            if (lineType.handle == 0)
                continue;
            if (!sourceHandles.insert(lineType.handle).second)
                return false;
            linetypeHandles.insert(lineType.handle);
        }
        for (const DRW_Dictionary& dictionary : data->dictionaries) {
            if (dictionary.handle == 0)
                continue;
            if (!sourceHandles.insert(dictionary.handle).second
                || !dictionaries.emplace(dictionary.handle, &dictionary).second)
                return false;
        }
        for (const DRW_XRecord& record : data->xRecords) {
            if (record.handle == 0)
                continue;
            if (!sourceHandles.insert(record.handle).second
                || !xrecords.emplace(record.handle, &record).second)
                return false;
        }

        std::unordered_set<std::uint32_t> visiting;
        std::unordered_set<std::uint32_t> visitedDictionaries;
        std::unordered_set<std::uint32_t> visitedXRecords;
        std::function<bool(std::uint32_t)> visitDictionary;
        std::function<bool(std::uint32_t)> visitXRecord;
        visitXRecord = [&](std::uint32_t handle) {
            const auto found = xrecords.find(handle);
            if (found == xrecords.end())
                return false;
            if (visiting.count(handle) != 0)
                return false;
            if (visitedXRecords.count(handle) != 0)
                return true;
            if (found->second->m_rawDataValid
                || (!found->second->m_handleValues.empty()
                    && std::any_of(found->second->m_handleValues.cbegin(),
                                   found->second->m_handleValues.cend(),
                                   [](const auto& value) {
                                       return value.first == 0;
                                   })))
                return false;
            visiting.insert(handle);
            m_extensionXRecordHandles.push_back(handle);
            if (found->second->xDictHandle != 0
                && !visitDictionary(found->second->xDictHandle)) {
                visiting.erase(handle);
                return false;
            }
            for (const std::uint32_t reactor : found->second->reactorHandles) {
                if (reactor == 0) {
                    visiting.erase(handle);
                    return false;
                }
                if (linetypeHandles.count(reactor) != 0)
                    continue;
                // Reactors are non-owning back-references; an object already
                // on the containment walk is present in the output closure.
                if (visiting.count(reactor) != 0)
                    continue;
                if (dictionaries.count(reactor) != 0) {
                    if (!visitDictionary(reactor)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else if (xrecords.count(reactor) == 0
                           || !visitXRecord(reactor)) {
                    visiting.erase(handle);
                    return false;
                }
            }
            visiting.erase(handle);
            visitedXRecords.insert(handle);
            return true;
        };
        visitDictionary = [&](std::uint32_t handle) {
            const auto found = dictionaries.find(handle);
            if (found == dictionaries.end() || handle == 0xCu || handle == 0xDu)
                return false;
            if (visiting.count(handle) != 0)
                return false;
            if (visitedDictionaries.count(handle) != 0)
                return true;
            if (!visiting.insert(handle).second)
                return false;
            if (!found->second->hasCompleteDwgEntries()
                && !found->second->hasCompleteDxfEntries()) {
                visiting.erase(handle);
                return false;
            }
            m_extensionDictionaryHandles.push_back(handle);
            if (found->second->xDictHandle != 0
                && !visitDictionary(found->second->xDictHandle)) {
                visiting.erase(handle);
                return false;
            }
            for (const std::uint32_t reactor : found->second->reactorHandles) {
                if (reactor == 0) {
                    visiting.erase(handle);
                    return false;
                }
                if (linetypeHandles.count(reactor) != 0
                    || visiting.count(reactor) != 0)
                    continue;
                if (dictionaries.count(reactor) != 0) {
                    if (!visitDictionary(reactor)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else if (xrecords.count(reactor) == 0
                           || !visitXRecord(reactor)) {
                    visiting.erase(handle);
                    return false;
                }
            }
            for (const DRW_Dictionary::Entry& entry : found->second->m_entries) {
                if (entry.m_handle == 0)
                    return false;
                if (dictionaries.count(entry.m_handle) != 0) {
                    if (!visitDictionary(entry.m_handle)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else if (xrecords.count(entry.m_handle) != 0) {
                    if (!visitXRecord(entry.m_handle)) {
                        visiting.erase(handle);
                        return false;
                    }
                } else {
                    visiting.erase(handle);
                    return false;
                }
            }
            visiting.erase(handle);
            visitedDictionaries.insert(handle);
            return true;
        };

        for (const DRW_LType& lineType : data->lineTypes) {
            if (lineType.xDictHandle != 0
                && !visitDictionary(lineType.xDictHandle))
                return false;
            for (const std::uint32_t reactor : lineType.reactorHandles) {
                if (reactor == 0)
                    return false;
                if (linetypeHandles.count(reactor) != 0
                    || visiting.count(reactor) != 0)
                    continue;
                if (dictionaries.count(reactor) != 0) {
                    if (!visitDictionary(reactor))
                        return false;
                } else if (xrecords.count(reactor) == 0
                           || !visitXRecord(reactor)) {
                    return false;
                }
            }
        }

        const auto hasParent = [&dictionaries, &visitedDictionaries,
                                &visitedXRecords, &linetypeHandles](
                                   std::uint32_t parent) {
            return parent == 0 || parent == 0xCu || parent == 0xDu
                || linetypeHandles.count(parent) != 0
                || (dictionaries.count(parent) != 0
                    && visitedDictionaries.count(parent) != 0)
                || visitedXRecords.count(parent) != 0;
        };
        for (std::uint32_t handle : m_extensionDictionaryHandles) {
            const DRW_Dictionary* dictionary = dictionaries.at(handle);
            if (!hasParent(dictionary->parentHandle))
                return false;
        }
        for (std::uint32_t handle : m_extensionXRecordHandles) {
            const DRW_XRecord* record = xrecords.at(handle);
            if (!hasParent(record->parentHandle))
                return false;
            const auto isHandleCode = [](int code) {
                return (code >= 320 && code <= 369)
                    || (code >= 390 && code <= 399)
                    || (code >= 480 && code <= 481)
                    || code == 5 || code == 105
                    || (code >= 1005 && code <= 1009);
            };
            const auto validateHandleTarget =
                [&isHandleCode, &linetypeHandles, &visitedDictionaries,
                 &visitedXRecords](int code, std::uint64_t target) {
                    if (!isHandleCode(code))
                        return true;
                    if (target > std::numeric_limits<std::uint32_t>::max())
                        return false;
                    const std::uint32_t targetHandle =
                        static_cast<std::uint32_t>(target);
                    return targetHandle == 0
                        || linetypeHandles.count(targetHandle) != 0
                        || visitedDictionaries.count(targetHandle) != 0
                        || visitedXRecords.count(targetHandle) != 0;
                };
            const auto validateHandleValue =
                [&isHandleCode, &validateHandleTarget](
                    const DRW_Variant& value) {
                    const int code = value.code();
                    if (!isHandleCode(code))
                        return true;
                    std::uint64_t target = 0;
                    if (value.type() == DRW_Variant::INTEGER) {
                        if (value.i_val() < 0)
                            return false;
                        target = static_cast<std::uint32_t>(value.i_val());
                    } else if (value.type() == DRW_Variant::INTEGER64) {
                        if (value.i64_val() < 0)
                            return false;
                        target = static_cast<std::uint64_t>(value.i64_val());
                    } else if (value.type() == DRW_Variant::STRING) {
                        const char* text = value.c_str();
                        if (text == nullptr || *text == '\0')
                            return false;
                        const char* end = text
                            + std::char_traits<char>::length(text);
                        const auto parsed = std::from_chars(text, end, target, 16);
                        if (parsed.ec != std::errc{} || parsed.ptr != end)
                            return false;
                    } else {
                        return false;
                    }
                    return validateHandleTarget(code, target);
                };
            if (!record->m_dataEntries.empty()) {
                for (const DRW_Variant& value : record->m_dataEntries) {
                    if (!validateHandleValue(value))
                        return false;
                }
            } else {
                for (const DRW_Variant& value : record->m_values) {
                    if (!validateHandleValue(value))
                        return false;
                }
                for (const auto& value : record->m_handleValues) {
                    if (!validateHandleTarget(value.first, value.second))
                        return false;
                }
            }
        }
        return true;
    } catch (...) {
        m_extensionDictionaryHandles.clear();
        m_extensionXRecordHandles.clear();
        return false;
    }
}


bool dx_iface::fileImport(const std::string& fileI, dx_data *fData, bool debug){
    unsigned int found = fileI.find_last_of(".");
    std::string fileExt = fileI.substr(found+1);
    std::transform(fileExt.begin(), fileExt.end(),fileExt.begin(), ::toupper);
    cData = fData;
    currentBlock = cData->mBlock;

    if (fileExt == "DXF"){
        //loads dxf
        dxfRW* dxf = new dxfRW(fileI.c_str());
        if (debug) {
            dxf->setDebug(DRW::DebugLevel::Debug);
        }
        bool success = dxf->read(this, false);
        if (success)
            fData->sourceVersion = sourceVersionFromDxfHeader(
                fData->headerC, dxf->getVersion());
        if (!success) {
            std::cout << "DXF file error: format " << dxf->getVersion() << " error " << dxf->getError() << std::endl;
        }
        delete dxf;
        return success;
    } else if (fileExt == "DWG"){
        //loads dwg
        dwgR* dwg = new dwgR(fileI.c_str());
        if (debug) {
            dwg->setDebug(DRW::DebugLevel::Debug);
        }
        bool success = dwg->read(this, false);
        if (success)
            fData->sourceVersion = dwg->getVersion();
        if (!success) {
            std::cout << "DWG file error: format " << dwg->getVersion() << " error " << dwg->getError() << std::endl;
        }
        delete dwg;
        return success;
    }
    std::cout << "file extension can be dxf or dwg" << std::endl;
    return false;
}

bool dx_iface::fileExport(const std::string& file, DRW::Version v, bool binary, dx_data *fData, bool debug){
    cData = fData;
    if (!prepareExtensionObjectGraph(cData))
        return false;
    dxfW = new dxfRW(file.c_str());
    std::vector<DRW_LType> lineTypes(cData->lineTypes.begin(),
                                     cData->lineTypes.end());
    dxfW->setCanonicalLineTypeMetadata(lineTypes);
    dxfW->setRawDxfSections(cData->rawDxfSections);
    if (debug) {
        dxfW->setDebug(DRW::DebugLevel::Debug);
    }
    const std::set<std::uint32_t> fixedHandles = {
        0x1u, 0x2u, 0x3u, 0x5u, 0x6u, 0x7u, 0x8u, 0x9u, 0xAu,
        0xCu, 0xDu, 0x10u, 0x12u, 0x14u, 0x15u, 0x16u,
        0x1Cu, 0x1Du, 0x1Eu, 0x1Fu, 0x20u, 0x21u};
    for (const DRW_Dictionary& dictionary : cData->dictionaries) {
        if (dictionary.handle != 0 && !dxfW->reserveHandle(dictionary.handle)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
    }
    for (const DRW_XRecord& record : cData->xRecords) {
        if (record.handle != 0 && !dxfW->reserveHandle(record.handle)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
    }
    std::map<std::uint32_t, std::uint32_t> handleRemap;
    for (std::uint32_t handle : m_extensionDictionaryHandles) {
        if (fixedHandles.count(handle) == 0)
            continue;
        const std::uint32_t replacement = dxfW->allocHandle();
        if (replacement == 0 || !dxfW->reserveHandle(replacement)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
        handleRemap.emplace(handle, replacement);
    }
    for (std::uint32_t handle : m_extensionXRecordHandles) {
        if (fixedHandles.count(handle) == 0)
            continue;
        const std::uint32_t replacement = dxfW->allocHandle();
        if (replacement == 0 || !dxfW->reserveHandle(replacement)) {
            delete dxfW;
            dxfW = nullptr;
            return false;
        }
        handleRemap.emplace(handle, replacement);
    }
    dxfW->setHandleRemap(handleRemap);
    bool success = dxfW->write(this, v, binary);
    if (!success && debug) {
        const DRW_OperationDiagnostic diagnostic = dxfW->getLastDiagnostic();
        std::cerr << "DXF export failed: " << diagnostic.code << ": "
                  << diagnostic.message << '\n';
    }
    delete dxfW;
    dxfW = nullptr;
    return success;
}

void dx_iface::writeEntity(DRW_Entity* e){
    switch (e->eType) {
    case DRW::POINT:
        dxfW->writePoint(static_cast<DRW_Point*>(e));
        break;
    case DRW::LINE:
        dxfW->writeLine(static_cast<DRW_Line*>(e));
        break;
    case DRW::CIRCLE:
        dxfW->writeCircle(static_cast<DRW_Circle*>(e));
        break;
    case DRW::ARC:
        dxfW->writeArc(static_cast<DRW_Arc*>(e));
        break;
    case DRW::SOLID:
        dxfW->writeSolid(static_cast<DRW_Solid*>(e));
        break;
    case DRW::DXF_TRACE:
        dxfW->writeTrace(static_cast<DRW_Trace*>(e));
        break;
    case DRW::E3DFACE:
        dxfW->write3dface(static_cast<DRW_3Dface*>(e));
        break;
    case DRW::ELLIPSE:
        dxfW->writeEllipse(static_cast<DRW_Ellipse*>(e));
        break;
    case DRW::LWPOLYLINE:
        dxfW->writeLWPolyline(static_cast<DRW_LWPolyline*>(e));
        break;
    case DRW::POLYLINE:
        dxfW->writePolyline(static_cast<DRW_Polyline*>(e));
        break;
    case DRW::MESH:
        dxfW->writeMesh(static_cast<DRW_Mesh*>(e));
        break;
    case DRW::HELIX:
        dxfW->writeHelix(static_cast<DRW_Helix*>(e));
        break;
    case DRW::SPLINE:
        dxfW->writeSpline(static_cast<DRW_Spline*>(e));
        break;
//    case RS2::EntitySplinePoints:
//        writeSplinePoints(static_cast<DRW_Point*>(e));
//        break;
//    case RS2::EntityVertex:
//        break;
    case DRW::INSERT:
        dxfW->writeInsert(static_cast<DRW_Insert*>(e));
        break;
    case DRW::MTEXT:
        dxfW->writeMText(static_cast<DRW_MText*>(e));
        break;
    case DRW::TEXT:
        if (auto* rtext = dynamic_cast<DRW_RText*>(e))
            dxfW->writeRText(rtext);
        else if (auto* arcText = dynamic_cast<DRW_ArcAlignedText*>(e))
            dxfW->writeArcAlignedText(arcText);
        else
            dxfW->writeText(static_cast<DRW_Text*>(e));
        break;
    case DRW::DIMLINEAR:
    case DRW::DIMALIGNED:
    case DRW::DIMANGULAR:
    case DRW::DIMANGULAR3P:
    case DRW::DIMRADIAL:
    case DRW::DIMDIAMETRIC:
    case DRW::DIMORDINATE:
        dxfW->writeDimension(static_cast<DRW_Dimension*>(e));
        break;
    case DRW::LEADER:
        dxfW->writeLeader(static_cast<DRW_Leader*>(e));
        break;
    case DRW::HATCH:
        dxfW->writeHatch(static_cast<DRW_Hatch*>(e));
        break;
    case DRW::MPOLYGON:
        dxfW->writeMPolygon(static_cast<DRW_MPolygon*>(e));
        break;
    case DRW::VIEWPORT:
        dxfW->writeViewport(static_cast<DRW_Viewport*>(e));
        break;
    case DRW::IMAGE:
        dxfW->writeImage(static_cast<DRW_Image*>(e), static_cast<dx_ifaceImg*>(e)->path);
        break;
    case DRW::PLANESURFACE:
    case DRW::EXTRUDEDSURFACE:
    case DRW::REVOLVEDSURFACE:
    case DRW::SWEPTSURFACE:
    case DRW::LOFTEDSURFACE:
    case DRW::NURBSURFACE:
        dxfW->writeSurface(static_cast<DRW_Surface*>(e));
        break;
    case DRW::E3DSOLID:
    case DRW::REGION:
    case DRW::BODY:
        dxfW->writeModelerGeometry(static_cast<DRW_ModelerGeometry*>(e));
        break;
    default:
        break;
    }
}
