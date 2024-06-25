#include <array>
#include <iostream>

#include "../../../include/bisl/diagnostic/error.hpp"
#include "../../../include/bisl/io/file/reader.hpp"
#include "../../../include/bisl/io/format/color/colorscheme.hpp"

std::string bisl::diagnostic::Error::format() {
    auto colorscheme = io::format::color::Colorscheme(
        io::format::types::Map<io::format::types::String*,
                               io::format::color::Color*>({
            {new io::format::types::String("accent"),
             new io::format::color::Color(
                 io::format::color::Color::Type::ColorTypeRed)},
            {new io::format::types::String("type"),
             new io::format::color::Color(
                 io::format::color::Color::Type::ColorTypeMagenta)},
        }));

    auto row = getMessage().find("EndOffile.") != std::string::npos
                   ? *getSpan().getRow() - 1
                   : *getSpan().getRow();

    if (showSource) {
        auto reader =
            io::file::ReaderBase<io::file::Reader*>::New(
                "inaccessible by bisl::diagnostic::Error",
                getSpan().getPath().substr(0, getSpan().getPath().length() - 1))
                .getValue();

        auto previousLine = reader->readLine(row - 1);
        auto line = reader->readLine(row);
        auto nextLine = reader->readLine(row + 1);

        char emptiness = (previousLine.empty() << 2) | (line.empty() << 1) |
                         (nextLine.empty());

        if (row != *getSpan().getRow()) {
            *getSpan().getRow() = row;
            *getSpan().getColumn() = line.length() + 1;
            line.append("; // <- add ';'");
            *getSpan().getLength() = 1;
        }

        return colorscheme.color("#{accent}(" + getIconAsString() + "#)")
                   .format() +
               '\t' +
               colorscheme.color("#{type}(" + getTypeAsString() + "#)")
                   .format() +
               ' ' + getSpan().format() + ": " +
               colorscheme.color("#{accent}(" + getMessage() + "#)\n\n")
                   .format() +
               '\t' + std::to_string(row - 1) + " │ " +
               (emptiness & 0x4 ? "..." : previousLine) + "\n" +
               colorscheme.color("#{accent}(- at#)\t").format() +
               std::to_string(row) + " │ " +
               (emptiness & 0x2
                    ? "[Failed to read] --- please report an issue."
                    : colorscheme.color(line, "accent", getSpan()).format()) +
               "\n\t" + std::to_string(row + 1) + " │ " +
               (emptiness & 0x1 ? "..." : nextLine) + '\n';
    }

    return colorscheme.color("#{accent}(" + getIconAsString() + "#)").format() +
           '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + getSpan().format() + ": " +
           colorscheme.color("#{accent}(" + getMessage() + "#)\n").format();
}

std::string bisl::diagnostic::Error::debug() {
    return "Error(type: " + getTypeAsString() + ", icon: \"" +
           getIconAsString() + "\", span: " + getSpan().debug() +
           ", message: \"" + getMessage() + "\")";
}

std::string bisl::diagnostic::Error::getTypeAsString() const {
    return std::array<std::string, Type::ErrorTypeCount + 1> {
        "fileNotFound",    "fileNotRegular", "fileNotReadable", "InvalidSyntax",
        "UnexpectedToken", "ColorNotFound",  "Mismatchedtypes", "Redefinition"}
        .at(errorType);
}

std::string bisl::diagnostic::Error::getIconAsString() const {
    return std::array<std::string, Icon::ErrorIconCount + 1> {
        "", "󰮘", "󰈡", "󰺬", "", "󰹊", "󱄑"}
        .at(icon);
}
