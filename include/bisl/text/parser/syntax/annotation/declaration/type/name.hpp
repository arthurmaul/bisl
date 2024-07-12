#pragma once

#include "../../../../../../io/string/span.hpp"
#include "type.hpp"

namespace bisl::text::parser::syntax::annotation::declaration::type {
class Name: public Type {
  public:
    explicit Name(io::format::types::String* value,
                  const io::string::Span& span)
        : Type(Type::Kind::Name), value(value), span(span) {}

    io::format::types::String* getValue() const { return value; }

    io::string::Span getSpan() const { return span; }

    std::string format() override, debug() override;

  private:
    io::format::types::String* value;
    const io::string::Span span;
};
} // namespace bisl::text::parser::syntax::annotation::declaration::type
