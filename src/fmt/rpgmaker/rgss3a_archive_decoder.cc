#include "fmt/rpgmaker/rgss3a_archive_decoder.h"
#include "fmt/rpgmaker/rgs/common.h"
#include "util/range.h"

using namespace au;
using namespace au::fmt::rpgmaker;

static const bstr magic = "RGSSAD\x00\x03"_b;

bool Rgss3aArchiveDecoder::is_recognized_internal(File &arc_file) const
{
    return arc_file.io.read(magic.size()) == magic;
}

std::unique_ptr<fmt::ArchiveMeta>
    Rgss3aArchiveDecoder::read_meta(File &arc_file) const
{
    arc_file.io.seek(magic.size());
    u32 key = arc_file.io.read_u32_le() * 9 + 3;
    auto meta = std::make_unique<ArchiveMeta>();
    while (!arc_file.io.eof())
    {
        auto entry = std::make_unique<rgs::ArchiveEntryImpl>();
        entry->offset = arc_file.io.read_u32_le() ^ key;
        if (!entry->offset)
            break;

        entry->size = arc_file.io.read_u32_le() ^ key;
        entry->key = arc_file.io.read_u32_le() ^ key;

        size_t name_size = arc_file.io.read_u32_le() ^ key;
        entry->name = arc_file.io.read(name_size).str();
        for (auto i : util::range(name_size))
            entry->name[i] ^= key >> (i << 3);

        meta->entries.push_back(std::move(entry));
    }
    return meta;
}

std::unique_ptr<File> Rgss3aArchiveDecoder::read_file(
    File &arc_file, const ArchiveMeta &m, const ArchiveEntry &e) const
{
    return rgs::read_file(
        arc_file, *static_cast<const rgs::ArchiveEntryImpl*>(&e));
}

static auto dummy = fmt::Registry::add<Rgss3aArchiveDecoder>("rm/rgss3a");