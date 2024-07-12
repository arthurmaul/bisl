#include "../../../../../include/bisl/text/parser/syntax/ast.hpp"

std::string
bisl::text::parser::syntax::AbstractSyntaxTree::getTypeAsString() const {
    switch (type) {
        case Type::Primary:
            return "Primary";

        case Type::Statement:
            return "Statement";

        case Type::Annotation:
            return "Annotation";

        default:
            return "Unreachable";
    }
}
