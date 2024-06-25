#include "../../../../../../../include/bisl/text/parser/syntax/statement/conditional/while.hpp"

std::string
bisl::text::parser::syntax::statement::conditional::While::format() {
    return "while " + getBody()->format();
}

std::string bisl::text::parser::syntax::statement::conditional::While::debug() {
    return "While(body: " + getBody()->debug() + ')';
}
