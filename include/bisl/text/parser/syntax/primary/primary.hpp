#pragma once

#include "../ast.hpp"

namespace bisl::text::parser::syntax::primary {
class Primary: public AbstractSyntaxTree {
  public:
    enum class Type { Block, Expression, Operation };

    Primary(const Type& type)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Primary),
          primaryType(type) {}

    Type getPrimaryType() const { return primaryType; }

  private:
    const Type primaryType;
};
} // namespace bisl::text::parser::syntax::primary
