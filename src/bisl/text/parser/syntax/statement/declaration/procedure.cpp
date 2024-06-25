#include "../../../../../../../include/bisl/text/parser/syntax/statement/declaration/procedure.hpp"

std::string
bisl::text::parser::syntax::statement::declaration::Procedure::format() {
    return "proc " + getName() + ' ' + getBody()->format();
}

std::string
bisl::text::parser::syntax::statement::declaration::Procedure::debug() {
    return "Procedure(name: " + getName() + ", body: " + getBody()->debug() +
           ')';
}
