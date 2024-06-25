#include "../../include/bisl/compiler/compiler.hpp"
#include "../../include/bisl/io/file/source.hpp"

#include "../../include/bisl/io/format/color/colorscheme.hpp"
#include "../../include/bisl/text/parser/syntax/primary/block.hpp"

#include <iostream>

int main(int argc, char** argv) {
    (void) argc;

    auto source = bisl::io::file::Source(argv[0], argv[1]);
    auto compilation = bisl::compiler::compile(source);

    if (compilation.isSuccess()) {
        for (auto value :
             dynamic_cast<bisl::text::parser::syntax::primary::Block*>(
                 compilation.getValue())
                 ->getValue()) {
            std::cerr << (argv[2] ? value->debug() : value->format())
                      << std::endl;
        }
    } else {
        for (auto error : *compilation.getError()) {
            std::cerr << (argv[2] ? error->debug() : error->format())
                      << std::endl
                      << std::endl;
        }
    }

    if (compilation.isFailure()) {
        auto colorscheme = bisl::io::format::color::Colorscheme(
            bisl::io::format::types::Map<bisl::io::format::types::String*,
                                         bisl::io::format::color::Color*>(
                {{new bisl::io::format::types::String("error"),
                  new bisl::io::format::color::Color(
                      bisl::io::format::color::Color::Type::ColorTypeRed)},
                 {new bisl::io::format::types::String("warning"),
                  new bisl::io::format::color::Color(
                      bisl::io::format::color::Color::Type::
                          ColorTypeYellow)}}));

        auto errors = 0, warnings = 0;

        for (const auto& diagnostic : *compilation.getError()) {
            if (diagnostic->getType() ==
                bisl::diagnostic::Diagnostic::Type::Error) {
                errors++;
            } else if (diagnostic->getType() ==
                       bisl::diagnostic::Diagnostic::Type::Warning) {
                warnings++;
            }
        }

        if (!(errors | warnings)) {
            return 0;
        }

        auto message = std::string();

        if (errors) {
            message.append("#{error}(" + std::to_string(errors) + " errors#)");
        }

        if (warnings) {
            if (errors) {
                message.append(", ");
            }

            message.append("#{warning}(" + std::to_string(warnings) +
                           " warnings#)");
        }

        message.append(" generated.");

        std::cerr << colorscheme.color(message).format() << std::endl;
    }
}
