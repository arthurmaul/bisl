#pragma once

#include "../../../../../io/format/types/vector.hpp"
#include "../../primary/block.hpp"
#include "declaration.hpp"

namespace bisl::text::parser::syntax::statement::declaration {
class Procedure: public Declaration {
  public:
    Procedure(const io::format::types::String& name, primary::Block* body)
        : Declaration(Declaration::Type::Procedure, name), body(body) {}

    primary::Block* getBody() const { return body; }

    std::string format() override, debug() override;

  private:
    primary::Block* body;
};
} // namespace bisl::text::parser::syntax::statement::declaration
