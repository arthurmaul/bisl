#include "../../../../include/bisl/text/lexer/token.hpp"

std::string bisl::text::lexer::Token::getTypeAsString() const {
    switch (type) {
        case Type::Identifier:
            return "Identifier";

        case Type::Integer:
            return "Integer";

        case Type::Float:
            return "Float";

        case Type::Character:
            return "Character";

        case Type::String:
            return "String";

        case Type::Ampersand:
            return "`&`";

        case Type::Pipe:
            return "`|`";

        case Type::Assign:
            return "`=`";

        case Type::Colon:
            return "`:`";

        case Type::SemiColon:
            return "`;`";

        case Type::QuestionMark:
            return "`?`";

        case Type::Arrow:
            return "`->`";

        case Type::Unhandled:
            return "Unhandled";

        case Type::EndOfFile:
            return "EndOfFile";

        default:
            return "Unreachable";
    }
}

std::string bisl::text::lexer::Token::getTypeAsString(const Token::Type& type) {
    auto copy = Token();
    copy.type = type;
    return copy.getTypeAsString();
}

std::string bisl::text::lexer::Token::format() {
    return getTypeAsString() + ": " + getValue();
}

std::string bisl::text::lexer::Token::debug() {
    return "Token(type: " + getTypeAsString() + ", span: " + span.debug() +
           ", value: \"" + getValue() + "\")";
}
