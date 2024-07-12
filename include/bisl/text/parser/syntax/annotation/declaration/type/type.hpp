#pragma once

#include "../../../../../../io/format/types/string.hpp"
#include "../../annotation.hpp"

namespace bisl::text::parser::syntax::annotation::declaration::type {
class Type: public Annotation {
  public:
    enum class Kind { Name, Reference };

    Type(const Kind& kind): Annotation(Annotation::Type::Type), kind(kind) {}

    Kind getKind() const { return kind; }

  private:
    const Kind kind;
};
} // namespace bisl::text::parser::syntax::annotation::declaration::type
