#include <iostream>

#include "../../../include/bisl/compiler/compiler.hpp"
#include "../../../include/bisl/text/lexer/lexer.hpp"
#include "../../../include/bisl/text/parser/parser.hpp"

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::compiler::compile(const bisl::io::file::Source& source) {
    if (source.getReader().isFailure()) {
        return new Vec(diagnostic::Diagnostic*,
                       {source.getReader().getError()});
    }

    auto lexer = bisl::text::lexer::Lexer(source);
    auto parser = bisl::text::parser::Parser(&lexer);
    auto tree = parser.parse();
    auto diagnostics = new Vec(diagnostic::Diagnostic*, {});

    InsertIfFailure(diagnostics, tree);

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    return tree.getValue();
}
