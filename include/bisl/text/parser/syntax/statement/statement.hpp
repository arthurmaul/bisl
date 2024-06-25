#pragma once

#include "../ast.hpp"

namespace bisl::text::parser::syntax::statement {
class Statement: public AbstractSyntaxTree {
  public:
    enum class Type { Conditional, Return, Declaration };

    Statement(const Type& type)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Statement),
          statementType(type) {}

    Type getStatementType() const { return statementType; }

  private:
    const Type statementType;
};
} // namespace bisl::text::parser::syntax::statement
