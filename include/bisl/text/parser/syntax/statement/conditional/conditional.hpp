#pragma once

#include "../../primary/block.hpp"
#include "../statement.hpp"

namespace bisl::text::parser::syntax::statement::conditional {
class Conditional: public Statement {
  public:
    enum class Type { If, While, Do };

    Conditional(const Type& type, primary::Block* body)
        : Statement(Statement::Type::Conditional), conditionalType(type),
          body(body) {}

    Type getConditionalType() const { return conditionalType; }

    primary::Block* getBody() const { return body; }

  private:
    const Type conditionalType;
    primary::Block* body;
};
} // namespace bisl::text::parser::syntax::statement::conditional
