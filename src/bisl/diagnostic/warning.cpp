#include <array>

#include "../../../include/bisl/diagnostic/warning.hpp"
#include "../../../include/bisl/io/file/reader.hpp"
#include "../../../include/bisl/io/format/color/colorscheme.hpp"

std::string bisl::diagnostic::Warning::format() {
    auto colorscheme = io::format::color::Colorscheme(
        io::format::types::Map<io::format::types::String*,
                               io::format::color::Color*>({
            {new io::format::types::String("accent"),
             new io::format::color::Color(
                 io::format::color::Color::Type::ColorTypeYellow)},
            {new io::format::types::String("type"),
             new io::format::color::Color(
                 io::format::color::Color::Type::ColorTypeMagenta)},
        }));

    auto reader =
        io::file::ReaderBase<io::file::Reader*>::New(
            "inaccessible by bisl::diagnostic::Warning",
            getSpan().getPath().substr(0, getSpan().getPath().length() - 1))
            .getValue();

    auto previousLine = reader->readLine(*getSpan().getRow() - 2);
    auto line = reader->readLine(*getSpan().getRow());
    auto nextLine = reader->readLine(*getSpan().getRow() + 1);

    char emptiness =
        (previousLine.empty() << 2) | (line.empty() << 1) | (nextLine.empty());

    return colorscheme.color("#{accent}(" + getIconAsString() + "#)").format() +
           '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + getSpan().format() + ": " +
           colorscheme.color("#{accent}(" + getMessage() + "#)\n\n").format() +
           '\t' + std::to_string(*getSpan().getRow() - 1) + " │ " +
           (emptiness & 0x4 ? "..." : previousLine) + "\n" +
           colorscheme.color("#{accent}(- at#)\t").format() +
           std::to_string(*getSpan().getRow()) + " │ " +
           (emptiness & 0x2
                ? "[Failed to read] --- please report an issue."
                : colorscheme.color(line, "accent", getSpan()).format()) +
           "\n\t" + std::to_string(*getSpan().getRow() + 1) + " │ " +
           (emptiness & 0x1 ? "..." : nextLine) + '\n';
}

std::string bisl::diagnostic::Warning::debug() {
    return "Warning(type: " + getTypeAsString() + ", icon: \"" +
           getIconAsString() + "\", span: " + getSpan().debug() +
           ", message: \"" + getMessage() + "\")";
}

std::string bisl::diagnostic::Warning::getTypeAsString() const {
    return std::array<std::string, Type::WarningTypeCount + 1>(
               {"UnknownType", "UndeclaredName"})
        .at((unsigned char) warningType);
}

std::string bisl::diagnostic::Warning::getIconAsString() const {
    return std::array<std::string, Icon::WarningIconCount + 1>({"", ""})
        .at((unsigned char) icon);
}
