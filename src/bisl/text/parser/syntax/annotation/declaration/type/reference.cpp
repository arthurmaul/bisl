#include "../../../../../../../../include/bisl/text/parser/syntax/annotation/declaration/type/reference.hpp"
#include "../../../../../../../../include/bisl/compiler/utils/typechecking.hpp"
#include "../../../../../../../../include/bisl/text/parser/syntax/annotation/declaration/type/name.hpp"

bisl::io::format::types::String* bisl::text::parser::syntax::annotation::
    declaration::type::Reference::getTypeName() const {
    auto type = dynamic_cast<declaration::type::Type*>(new Reference(*this));

    while (compiler::utils::isDynamicallyCastableTo<
           typename declaration::type::Type*, Reference*>(type)) {
        type = dynamic_cast<Reference*>(type)->getTypeType();
    }

    return dynamic_cast<declaration::type::Name*>(type)->getValue();
}

unsigned long long int
bisl::text::parser::syntax::annotation::declaration::type::Reference::getDepth()
    const {
    auto depth = 0;

    auto type = dynamic_cast<declaration::type::Type*>(new Reference(*this));

    while (compiler::utils::isDynamicallyCastableTo<
           typename declaration::type::Type*, Reference*>(type)) {
        type = dynamic_cast<Reference*>(type)->getTypeType();
        depth++;
    }

    return depth;
}

std::string
bisl::text::parser::syntax::annotation::declaration::type::Reference::format() {
    auto result = std::string(getTypeName()->format());

    for (unsigned long long int index = 0; index < getDepth(); index++) {
        result.push_back('*');
    }

    return result;
}

std::string
bisl::text::parser::syntax::annotation::declaration::type::Reference::debug() {
    return "Reference(typetype: " + getTypeName()->debug() +
           ", depth: " + std::to_string(getDepth()) + ')';
}
