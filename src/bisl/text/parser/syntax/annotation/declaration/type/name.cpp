#include "../../../../../../../../include/bisl/text/parser/syntax/annotation/declaration/type/name.hpp"

std::string
bisl::text::parser::syntax::annotation::declaration::type::Name::format() {
    return getValue()->format();
}

std::string
bisl::text::parser::syntax::annotation::declaration::type::Name::debug() {
    return "Name(" + format() + ')';
}
