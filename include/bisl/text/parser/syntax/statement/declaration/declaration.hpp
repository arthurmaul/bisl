#pragma once

#include "../../../../../io/format/types/string.hpp"
#include "../statement.hpp"

namespace bisl::text::parser::syntax::statement::declaration {
class Declaration: public Statement {
  public:
    enum class Type { Procedure };

    explicit Declaration(const Type& declarationType,
                         const io::format::types::String& name)
        : Statement(Statement::Type::Declaration),
          declarationType(declarationType), name(name) {}

    Type getDeclarationType() const { return declarationType; }

    io::format::types::String getName() const { return name; }

  private:
    const Type declarationType;
    const io::format::types::String name;
};
} // namespace bisl::text::parser::syntax::statement::declaration
