#pragma once

#include <filesystem>

#include "reader.hpp"

namespace bisl::io::file {
class Source {
  public:
    Source(const char* executableName, const std::filesystem::path& path)
        : path(path), reader(io::file::ReaderBase<io::file::Reader*>::New(
                          executableName, path)) {}

    explicit Source() {}

    std::filesystem::path getPath() const { return path; }

    diagnostic::DiResult<io::file::Reader*, diagnostic::Diagnostic*>
    getReader() const {
        return reader;
    }

  private:
    const std::filesystem::path path;
    diagnostic::DiResult<io::file::Reader*, diagnostic::Diagnostic*> reader;
};
} // namespace bisl::io::file
