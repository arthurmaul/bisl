#pragma once

#include "../diagnostic/error.hpp"
#include "../diagnostic/result.hpp"
#include "../io/file/source.hpp"
#include "../io/format/types/vector.hpp"
#include "../text/parser/syntax/ast.hpp"

namespace bisl::compiler {
diagnostic::DiResult<text::parser::syntax::AbstractSyntaxTree*,
                     io::format::types::Vector<diagnostic::Diagnostic*>*>
compile(const io::file::Source& source);
}
