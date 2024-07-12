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

        case '-':
            return lexArrow();

        case '/':
            return lexSlash();

        case '&':
            return lexAmpersand();

        case '|':
            return lexPipe();

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
bisl::text::lexer::Lexer::lexArrow() {
    auto start = span.clone();
    advance();

    if (current == '>') {
        return advanceWithResult(
            new Token(Token::Type::Arrow, start.withLengthOf(2)));
    }

    return new Token(Token::Type::Unhandled, start.withLengthOf(1));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexSlash() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
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
            return new Token(Token::Type::Unhandled, start);
    }
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexAmpersand() {
    return advanceWithResult(
        new Token(Token::Type::Ampersand, span.clone().withLengthOf(1)));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexPipe() {
    return advanceWithResult(
        new Token(Token::Type::Ampersand, span.clone().withLengthOf(1)));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexSemiColon() {
    return advanceWithResult(
        new Token(Token::Type::SemiColon, span.clone().withLengthOf(1)));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::lexer::Lexer::lexAssign() {
    return advanceWithResult(
        new Token(Token::Type::Assign, span.clone().withLengthOf(1)));
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
