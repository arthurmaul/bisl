#pragma once

#include "../primary.hpp"

namespace bisl::text::parser::syntax::primary::opertion {
class Operation: public Primary {
  public:
    Operation(): Primary(Primary::Type::Operation) {}
};
} // namespace bisl::text::parser::syntax::primary::opertion
