#pragma once

#include "../../io/format/display.hpp"
#include "../../io/format/types/string.hpp"
#include "../../io/string/span.hpp"
#include "../parser/syntax/primary/operation/operation.hpp"

namespace bisl::text::lexer {
class Token: public parser::syntax::primary::opertion::Operation {
  public:
    enum class Type {
        Identifier,
        Integer,
        Float,
        Character,
        String,

        Plus,
        Minus,
        Asterisk,
        Slash,
        Modulo,
        Ampersand,
        Pipe,
        Caret,
        Tilde,
        DoubleLessThan,
        DoubleGreaterThan,
        Exclamation,
        Assign,
        ExclamationAssign,
        DoubleAssign,
        DoubleAmpersand,
        DoublePipe,
        LessThan,
        GreaterThan,
        LessThanAssign,
        GreaterThanAssign,
        PlusAssign,
        MinusAssign,
        AsteriskAssign,
        SlashAssign,
        ModuloAssign,
        AmpersandAssign,
        PipeAssign,
        CaretAssign,
        DoubleLessThanAssign,
        DoubleGreaterThanAssign,

        SemiColon,

        Unhandled,
        EndOfFile
    };

    Token(const Type& type, const io::string::Span& span,
          const std::string value)
        : type(type), span(span), value(value) {}

    Token(const Type& type, const io::string::Span& span)
        : type(type), span(span) {}

    Token(): type(Type::Identifier) {}

    Type getType() const { return type; }

    io::string::Span getSpan() const { return span; }

    std::string getValue() const { return value; }

    std::string getTypeAsString() const;
    static std::string getTypeAsString(const Token::Type& type);

    std::string format() override;

    std::string debug() override;

  private:
    Type type;
    io::string::Span span;
    const std::string value;
};
} // namespace bisl::text::lexer
