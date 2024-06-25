#pragma once

#include "../../../io/format/display.hpp"

namespace bisl::text::parser::syntax {
class AbstractSyntaxTree: io::format::Display {
  public:
    enum class Type { Primary, Statement, Anotation };

    AbstractSyntaxTree(const Type& type): type(type) {}

    Type getType() const { return type; }

    std::string getTypeAsString() const;

    std::string format() { return "Defaultformatter"; }

    std::string debug() { return "DefaultDebugger"; }

  private:
    const Type type;
};
} // namespace bisl::text::parser::syntax
