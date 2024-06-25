#include "../../../../../../../include/bisl/text/parser/syntax/statement/conditional/if.hpp"

std::string bisl::text::parser::syntax::statement::conditional::If::format() {
    return "if/else " + getBody()->format() +
           (getElseClause() ? getElseClause()->format() : "");
}

std::string bisl::text::parser::syntax::statement::conditional::If::debug() {
    return "If(body: " + getBody()->debug() + ", elseClause: " +
           (getElseClause() ? getElseClause()->debug() : "none");
}
