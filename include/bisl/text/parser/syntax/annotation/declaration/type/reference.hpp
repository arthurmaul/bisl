#pragma once

#include "type.hpp"

namespace bisl::text::parser::syntax::annotation::declaration::type {
class Reference: public Type {
  public:
    explicit Reference(Type* type)
        : Type(Type::Kind::Reference), typeType(type) {}

    Type* getTypeType() const { return typeType; }

    io::format::types::String* getTypeName() const;

    unsigned long long int getDepth() const;

    std::string format() override, debug() override;

  private:
    Type* typeType;
};
} // namespace bisl::text::parser::syntax::annotation::declaration::type
