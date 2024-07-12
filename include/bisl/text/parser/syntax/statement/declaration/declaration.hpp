#pragma once

#include "../../../../../io/format/types/string.hpp"
#include "../../annotation/declaration/type/type.hpp"
#include "../../primary/block.hpp"
#include "../statement.hpp"

namespace bisl::text::parser::syntax::statement::declaration {
class Declaration: public Statement {
  public:
    explicit Declaration(
        const io::format::types::String& name,
        io::format::types::Vector<annotation::declaration::type::Type*>*
            annotations,
        primary::Block* value)
        : Statement(Statement::Type::Declaration), name(name),
          annotations(annotations), value(value) {}

    io::format::types::String getName() const { return name; }

    io::format::types::Vector<annotation::declaration::type::Type*>*
    getAnnotations() const {
        return annotations;
    }

    primary::Block* getValue() const { return value; }

    std::string format() override, debug() override;

  private:
    const io::format::types::String name;
    io::format::types::Vector<annotation::declaration::type::Type*>*
        annotations;
    primary::Block* value;
};
} // namespace bisl::text::parser::syntax::statement::declaration
