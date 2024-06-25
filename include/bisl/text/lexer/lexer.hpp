#pragma once

#include "../../diagnostic/result.hpp"
#include "../../io/file/source.hpp"
#include "../../io/format/types/char.hpp"
#include "token.hpp"

namespace bisl::text::lexer {
class Lexer {
  public:
    Lexer(const io::file::Source& source)
        : source(source),
          span("lexer", source.getPath().string() + ':', 0, 1, 1,
               source.getReader().getValue()->readAll().length()),
          current(source.getReader().getValue()->readAll()[*span.getIndex()]) {}

    diagnostic::DiResult<Token*, diagnostic::Diagnostic*> lexNext(),
        lexIdentifier(), lexNumber(), lexCharacter(), lexString(), lexPlus(),
        lexMinus(), lexAsterisk(), lexSlash(), lexModulo(), lexAmpersand(),
        lexPipe(), lexCaret(), lexTilde(), lexLessThan(), lexGreaterThan(),
        lexExclamation(), lexAssign(), lexSemiColon(), lexUnhandled(),
        lexEndOfFile(),
        advanceWithResult(
            const diagnostic::DiResult<Token*, diagnostic::Diagnostic*>
                & result);

    diagnostic::DiResult<io::format::types::Char*, diagnostic::Diagnostic*>
    lexEscapeSequence();

    char skipWhitespace();
    char advance();

  private:
    const io::file::Source source;

  public:
    io::string::Span span;
    char current;
};
} // namespace bisl::text::lexer
