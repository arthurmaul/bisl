#include <cctype>
#include <functional>
#include <unordered_map>

#include "../../../../include/bisl/diagnostic/error.hpp"
#include "../../../../include/bisl/text/lexer/lexer.hpp"

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexNext() {
    switch (skipWhitespace()) {
        case '_':
            return lexIdentifier();

        case '.':
            return lexNumber();

        case '\'':
            return lexCharacter();

        case '\"':
            return lexString();

        case '+':
            return lexPlus();

        case '-':
            return lexMinus();

        case '*':
            return lexAsterisk();

        case '/':
            return lexSlash();

        case '%':
            return lexModulo();

        case '&':
            return lexAmpersand();

        case '|':
            return lexPipe();

        case '^':
            return lexCaret();

        case '~':
            return lexTilde();

        case '<':
            return lexLessThan();

        case '>':
            return lexGreaterThan();

        case '!':
            return lexExclamation();

        case '=':
            return lexAssign();

        case ';':
            return lexSemiColon();

        case '\0':
            return lexEndOfFile();

        default:
            if (std::isalpha(current)) {
                return lexIdentifier();
            } else if (std::isdigit(current)) {
                return lexNumber();
            } else {
                return lexUnhandled();
            }
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexIdentifier() {
    auto value = std::string();
    auto start = span.clone();

    while (std::isalnum(current) || current == '_') {
        value.push_back(current);
        advance();
    }

    return new Token(Token::Type::Identifier,
                     start.withLengthOf(value.length()), value);
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexNumber() {
    auto value = std::string();
    auto start = span.clone();
    auto type = current == '.' ? Token::Type::Float : Token::Type::Integer;

    while (std::isdigit(current) || current == '.') {
        if (current == '.') {
            if (type == Token::Type::Float) {
                return advanceWithResult(new diagnostic::Error(
                    diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                    diagnostic::Error::Icon::ErrorIconProgramCross,
                    start.clone()
                        .withLengthOf(value.length() - 1)
                        .withLengthOf(value.length()),
                    "odd '.'"));
            }

            type = Token::Type::Float;
        }

        value.push_back(current);
        advance();
    }

    if (value == "." ||
        (value.length() && value.at(value.length() - 1) == '.')) {
        return advanceWithResult(new diagnostic::Error(
            diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            diagnostic::Error::Icon::ErrorIconProgramCross,
            start.clone()
                .withLengthOf(value.length() - 1)
                .withLengthOf(value.length()),
            "odd '.'"));
    }

    return new Token(type, start.withLengthOf(value.length()), value);
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexCharacter() {
    char value;
    auto start = span.clone();
    auto length = 3;

    if (advance() == '\\') {
        auto escapeSequence = lexEscapeSequence();

        if (escapeSequence.isFailure()) {
            return advanceWithResult(escapeSequence.getError());
        }

        value = escapeSequence.getValue()->getValue();
        length++;
    } else {
        value = current;
    }

    if (advance() != '\'') {
        return advanceWithResult(new diagnostic::Error(
            diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            diagnostic::Error::Icon::ErrorIconProgramCross,
            start.withLengthOf(length), "unterminated character sequence"));
    }

    return advanceWithResult(
        new Token(Token::Type::Character, start.withLengthOf(length), {value}));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexString() {
    auto value = std::string();
    auto start = span.clone();

    while (advance() != '\"') {
        if (current == '\0') {
            return new diagnostic::Error(
                diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                diagnostic::Error::Icon::ErrorIconProgramCross,
                start.withLengthOf(value.length()),
                "unterminated string sequence");
        } else if (current == '\\') {
            auto escapeSequence = lexEscapeSequence();

            if (escapeSequence.isFailure()) {
                return escapeSequence.getError();
            }

            value.push_back(escapeSequence.getValue()->debug().at(0));
        } else {
            value.push_back(current);
        }
    }

    return advanceWithResult(new Token(
        Token::Type::String, start.withLengthOf(value.length() + 2), value));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexPlus() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::PlusAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Plus, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexMinus() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::MinusAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Minus, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexAsterisk() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::AsteriskAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Asterisk, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexSlash() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::SlashAssign, start.withLengthOf(2)));

        case '/':
            while (advance() != '\n')
                ;
            return lexNext();

        case '*': {
            auto start = span.clone();

            advance();

            while (true) {
                if (current == '*') {
                    advance();

                    if (current == '/') {
                        break;
                    }
                } else if (current == '\0') {
                    return new diagnostic::Error(
                        diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        diagnostic::Error::Icon::ErrorIconProgramCross,
                        start.withLengthOf(*start.getColumn() -
                                           *span.getColumn()),
                        "unterminated multi-line comment sequence");
                } else if (current == '\n') {
                    *span.getRow() += 1;
                    *span.getColumn() = 0;
                }

                advance();
            }

            advance();
            return lexNext();
        }

        default:
            return new Token(Token::Type::Slash, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexModulo() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::ModuloAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Modulo, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexAmpersand() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::AmpersandAssign, start.withLengthOf(2)));

        case '&':
            return advanceWithResult(
                new Token(Token::Type::DoubleAmpersand, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Ampersand, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexPipe() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::PipeAssign, start.withLengthOf(2)));

        case '|':
            return advanceWithResult(
                new Token(Token::Type::DoublePipe, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Pipe, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexCaret() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::CaretAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Caret, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexTilde() {
    return advanceWithResult(
        new Token(Token::Type::Tilde, span.clone().withLengthOf(1)));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexLessThan() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::LessThanAssign, start.withLengthOf(2)));

        case '<':
            switch (advance()) {
                case '=':
                    return advanceWithResult(
                        new Token(Token::Type::DoubleLessThanAssign,
                                  start.withLengthOf(3)));

                default:
                    return new Token(Token::Type::DoubleLessThan,
                                     span.withLengthOf(2));
            }

        default:
            return new Token(Token::Type::LessThan, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexGreaterThan() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(new Token(Token::Type::GreaterThanAssign,
                                               start.withLengthOf(2)));

        case '>':
            switch (advance()) {
                case '=':
                    return advanceWithResult(
                        new Token(Token::Type::DoubleGreaterThanAssign,
                                  start.withLengthOf(3)));

                default:
                    return new Token(Token::Type::DoubleGreaterThan,
                                     span.withLengthOf(2));
            }

        default:
            return new Token(Token::Type::GreaterThan, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexExclamation() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(new Token(Token::Type::ExclamationAssign,
                                               start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Exclamation, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexSemiColon() {
    return advanceWithResult(
        new Token(Token::Type::SemiColon, span.withLengthOf(1)));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexAssign() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::DoubleAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Assign, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexUnhandled() {
    return advanceWithResult(new Token(
        Token::Type::Unhandled, span.clone().withLengthOf(1), {current}));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexEndOfFile() {
    return new Token(Token::Type::EndOfFile, span.clone().withLengthOf(1));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::advanceWithResult(
    const bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                                     bisl::diagnostic::Diagnostic*>& result) {
    advance();
    return result;
}

bisl::diagnostic::DiResult<bisl::io::format::types::Char*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexEscapeSequence() {
    return std::unordered_map<char, io::format::types::Char*> {
        {'a', new io::format::types::Char('\a')},
        {'b', new io::format::types::Char('\b')},
        {'f', new io::format::types::Char('\f')},
        {'n', new io::format::types::Char('\n')},
        {'r', new io::format::types::Char('\r')},
        {'v', new io::format::types::Char('\v')},
        {'\\', new io::format::types::Char('\\')},
        {'\'', new io::format::types::Char('\'')},
        {'\"', new io::format::types::Char('\"')},
        {'?', new io::format::types::Char('\?')}}
        .at(advance());
}

char bisl::text::lexer::Lexer::skipWhitespace() {
    while (std::isspace(current)) {
        if (current == '\n') {
            *span.getRow() += 1;
            *span.getColumn() = 0;
        }

        advance();
    }

    return current;
}

char bisl::text::lexer::Lexer::advance() {
    *span.getColumn() += 1;

    return current =
               source.getReader().getValue()->readAll()[*span.getIndex() += 1];
}
