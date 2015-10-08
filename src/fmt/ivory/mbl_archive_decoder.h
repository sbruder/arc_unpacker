#pragma once

#include "fmt/archive_decoder.h"

namespace au {
namespace fmt {
namespace ivory {

    class MblArchiveDecoder final : public ArchiveDecoder
    {
    public:
        MblArchiveDecoder();
        ~MblArchiveDecoder();
        void register_cli_options(ArgParser &) const override;
        void parse_cli_options(const ArgParser &) override;
        void set_plugin(const std::string &name);
        std::unique_ptr<ArchiveMeta> read_meta(File &) const override;
        std::unique_ptr<File> read_file(
            File &, const ArchiveMeta &, const ArchiveEntry &) const override;
    protected:
        bool is_recognized_internal(File &) const override;
    private:
        struct Priv;
        std::unique_ptr<Priv> p;
    };

} } }