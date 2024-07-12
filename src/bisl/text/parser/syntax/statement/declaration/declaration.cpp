#include "../../../../../../../include/bisl/text/parser/syntax/statement/declaration/declaration.hpp"

std::string
bisl::text::parser::syntax::statement::declaration::Declaration::format() {
    return "proc " + getName() + ' ' + getValue()->format();
}

std::string
bisl::text::parser::syntax::statement::declaration::Declaration::debug() {
    return "Declaration(name: " + getName() + ", body: " + getValue()->debug() +
           ')';
}
