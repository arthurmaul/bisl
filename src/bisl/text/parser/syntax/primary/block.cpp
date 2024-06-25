#include "../../../../../../include/bisl/text/parser/syntax/primary/block.hpp"

std::string bisl::text::parser::syntax::primary::Block::format() {
    return getValue().format();
}

std::string bisl::text::parser::syntax::primary::Block::debug() {
    return "Block(value: " + getValue().debug() + ')';
}
