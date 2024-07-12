#pragma once

#include <vector>

#include "../../io/format/types/vector.hpp"
#include "../lexer/lexer.hpp"
#include "syntax/annotation/declaration/type/type.hpp"
#include "syntax/ast.hpp"

namespace bisl::text::parser {
class Parser {
  public:
    explicit Parser(lexer::Lexer* lexer)
        : lexer(lexer), current(lexer->lexNext()) {}

    diagnostic::DiResult<syntax::AbstractSyntaxTree*,
                         io::format::types::Vector<diagnostic::Diagnostic*>*>
    parse(), parseBlock(const lexer::Token::Type& until), parseNext(),
        parseOperation(), parseName(), parseReservedWord(), parseIfStatement(),
        parseDeclaration();

    diagnostic::DiResult<io::format::types::Vector<
                             syntax::annotation::declaration::type::Type*>*,
                         io::format::types::Vector<diagnostic::Diagnostic*>*>
    parseAnnotations();

    diagnostic::DiResult<io::format::types::String*,
                         io::format::types::Vector<diagnostic::Diagnostic*>*>
    getName() const;

    diagnostic::DiResult<lexer::Token*, diagnostic::Diagnostic*>
    eat(const std::vector<lexer::Token::Type>& types), skip();

    diagnostic::DiResult<io::format::types::String*, diagnostic::Diagnostic*>
    expect(const std::vector<std::string>& names) const;

    bool currentIsReservedWord() const;

  private:
    lexer::Lexer* lexer;
    diagnostic::DiResult<lexer::Token*, diagnostic::Diagnostic*> current;
    diagnostic::Diagnostic* currentError;
};
} // namespace bisl::text::parser
