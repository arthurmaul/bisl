#pragma once

#include "conditional.hpp"

namespace bisl::text::parser::syntax::statement::conditional {
class While: public Conditional {
  public:
    While(primary::Block* body): Conditional(Conditional::Type::While, body) {}

    std::string format() override, debug() override;
};
} // namespace bisl::text::parser::syntax::statement::conditional
