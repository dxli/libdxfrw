/* Internal opt-in observation of an already committed XRECORD frame.
 * This is source evidence only; it grants no output or semantic authority.
 */
#ifndef DWG_XRECORD_VERIFIED_SOURCE_H
#define DWG_XRECORD_VERIFIED_SOURCE_H

#include "drw_objects.h"
#include "dwgobjectframe.h"

class dwgReader;

class DwgVerifiedXRecordTicket final {
    friend class dwgReader;
    DwgVerifiedXRecordTicket(DRW::Version version, std::uint32_t handle,
            std::uint32_t offset, std::uint64_t ordinal,
            DRW_DwgFrameOffsetSpace space, const DwgObjectFrame& frame) noexcept
        : version_(version), handle_(handle), offset_(offset), ordinal_(ordinal),
          space_(space), frame_(frame) {}
public:
    DwgVerifiedXRecordTicket(const DwgVerifiedXRecordTicket&) = delete;
    DwgVerifiedXRecordTicket& operator=(const DwgVerifiedXRecordTicket&) = delete;
    DRW::Version version() const noexcept { return version_; }
    std::uint32_t handle() const noexcept { return handle_; }
    std::uint32_t offset() const noexcept { return offset_; }
    std::uint64_t ordinal() const noexcept { return ordinal_; }
    DRW_DwgFrameOffsetSpace space() const noexcept { return space_; }
    const DwgObjectFrame& frame() const noexcept { return frame_; }
private:
    DRW::Version version_;
    std::uint32_t handle_, offset_;
    std::uint64_t ordinal_;
    DRW_DwgFrameOffsetSpace space_;
    const DwgObjectFrame& frame_;
};

class DwgVerifiedXRecordSink {
public:
    virtual ~DwgVerifiedXRecordSink() = default;
    virtual bool wantsVerifiedXRecord(std::uint32_t handle) const noexcept = 0;
    virtual void verifiedXRecord(const DwgVerifiedXRecordTicket&) noexcept = 0;
};
#endif
