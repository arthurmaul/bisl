#include "../../../include/bisl/diagnostic/note.hpp"
#include "../../../include/bisl/io/file/reader.hpp"
#include "../../../include/bisl/io/format/color/colorscheme.hpp"

std::string bisl::diagnostic::Note::format() {
    return io::format::color::Colorscheme(
               io::format::types::Map<io::format::types::String*,
                                      io::format::color::Color*>(
                   {{new io::format::types::String("asterisk"),
                     new io::format::color::Color(
                         io::format::color::Color::Type::ColorTypeBlue)},
                    {new io::format::types::String("note"),
                     new io::format::color::Color(
                         io::format::color::Color::Type::ColorTypeCyan)}}))
               .color("#{asterisk}(*#) #{note}(" + getMessage() + '\n' +
                      (showLine
                           ? "- at\t" + std::to_string(*getSpan().getRow()) +
                                 " │ " +
                                 io::file::ReaderBase<io::file::Reader*>::New(
                                     "inaccessible by bisl::diagnostic::Note",
                                     getSpan().getPath().substr(
                                         0, getSpan().getPath().length() - 1))
                                     .getValue()
                                     ->readLine(*getSpan().getRow())
                           : "") +
                      "#)")
               .format() +
           '\n';
}

std::string bisl::diagnostic::Note::debug() { return "Note(...)"; }
