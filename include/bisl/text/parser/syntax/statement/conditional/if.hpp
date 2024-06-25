#pragma once

#include "../../primary/block.hpp"
#include "conditional.hpp"

namespace bisl::text::parser::syntax::statement::conditional {
class If: public Conditional {
  public:
    If(primary::Block* body, If* elseClause)
        : Conditional(Conditional::Type::If, body), elseClause(elseClause) {}

    If* getElseClause() const { return elseClause; }

    std::string format() override, debug() override;

  private:
    If* elseClause;
};
} // namespace bisl::text::parser::syntax::statement::conditional
