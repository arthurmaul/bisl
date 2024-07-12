#pragma once

#include "../../../../io/format/display.hpp"
#include "../ast.hpp"

namespace bisl::text::parser::syntax::annotation {
class Annotation: public AbstractSyntaxTree {
  public:
    enum class Type { Type, Flags };

    explicit Annotation(const Type& type)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Annotation),
          annotationType(type) {}

    Type getAnnotationType() const { return annotationType; }

  private:
    const Type annotationType;
};
} // namespace bisl::text::parser::syntax::annotation
