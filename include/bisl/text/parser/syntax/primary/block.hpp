#pragma once

#include <memory>

#include "../../../../io/format/types/vector.hpp"
#include "primary.hpp"

namespace bisl::text::parser::syntax::primary {
class Block: public Primary {
  public:
    Block(const io::format::types::Vector<AbstractSyntaxTree*>& value)
        : Primary(Primary::Type::Block), value(value) {}

    io::format::types::Vector<AbstractSyntaxTree*> getValue() const {
        return value;
    }

    std::string format() override, debug() override;

  private:
    io::format::types::Vector<AbstractSyntaxTree*> value;
};
} // namespace bisl::text::parser::syntax::primary
