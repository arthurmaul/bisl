#pragma once

#include <filesystem>
#include <fstream>

#include "../../diagnostic/error.hpp"
#include "../../diagnostic/result.hpp"
#include "../../io/format/display.hpp"

namespace bisl::io::file {
class Reader: format::Display {
  public:
    Reader(const std::filesystem::path& path, const std::string& content)
        : path(path), content(content) {}

    explicit Reader() {}

    std::string readAll() const, readLine(const unsigned long long int& index);

    std::string format() override, debug() override;

  private:
    const std::string path;
    const std::string content;
};

template<typename Derived> class ReaderBase: format::Display {
  public:
    static diagnostic::DiResult<Derived, diagnostic::Diagnostic*>
    New(const char* executableName, const std::filesystem::path& path) {

        auto fileNameStart = std::string(executableName).length() + 1;
        const auto pathAsString = std::string(path);

        if (!std::filesystem::exists(path)) {
            return new diagnostic::Error(
                diagnostic::Error::Type::ErrorTypeFileNotFound,
                diagnostic::Error::Icon::ErrorIconFileQuestionMark,
                io::string::Span("command line input", "", fileNameStart, 1,
                                 fileNameStart + 1, pathAsString.length()),
                "could not find file `" + pathAsString + '`', false);
        } else if (!std::filesystem::is_regular_file(path)) {
            return new diagnostic::Error(
                diagnostic::Error::Type::ErrorTypeFileNotRegular,
                diagnostic::Error::Icon::ErrorIconFileCross,
                io::string::Span("command line input", "", fileNameStart, 1,
                                 fileNameStart + 1, pathAsString.length()),
                "specified file is not a regular `" + pathAsString + '`',
                false);
        } else if ((std::filesystem::status(path).permissions() &
                    std::filesystem::perms::others_read) ==
                   std::filesystem::perms::none) {
            return new diagnostic::Error(
                diagnostic::Error::Type::ErrorTypeFileNotReadable,
                diagnostic::Error::Icon::ErrorIconFileLock,
                io::string::Span("command line input", "", fileNameStart, 1,
                                 fileNameStart + 1, pathAsString.length()),
                "could not read file `" + pathAsString + '`', false);
        }

        std::ifstream file(path);
        std::stringstream stream;
        stream << file.rdbuf();
        return new Reader(path, stream.str());
    }
};
} // namespace bisl::io::file
