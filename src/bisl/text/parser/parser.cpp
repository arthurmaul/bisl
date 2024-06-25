#include <algorithm>
#include <array>
#include <cstring>
#include <memory>

#include "../../../../include/bisl/compiler/utils/typechecking.hpp"
#include "../../../../include/bisl/diagnostic/note.hpp"
#include "../../../../include/bisl/text/parser/parser.hpp"
#include "../../../../include/bisl/text/parser/syntax/primary/block.hpp"
#include "../../../../include/bisl/text/parser/syntax/primary/operation/operation.hpp"
#include "../../../../include/bisl/text/parser/syntax/statement/conditional/if.hpp"
#include "../../../../include/bisl/text/parser/syntax/statement/conditional/while.hpp"
#include "../../../../include/bisl/text/parser/syntax/statement/declaration/procedure.hpp"

#define EatAndReturnIfError(types)                                             \
    if (eat(types).isFailure())                                                \
    return new Vec(bisl::diagnostic::Diagnostic*, {currentError})

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parse() {
    return parseBlock(lexer::Token::Type::EndOfFile);
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseNext() {
    if (current.isFailure()) {
        return new Vec(diagnostic::Diagnostic*, {current.getError()});
    }

    switch (current.getValue()->getType()) {
        case lexer::Token::Type::Identifier:
            return parseName();

        default:
            auto value = current.getValue();
            EatAndReturnIfError({current.getValue()->getType()});
            return value;
    }
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseBlock(const lexer::Token::Type& until) {
    auto tree = Vec(syntax::AbstractSyntaxTree*, {});
    auto errors = new Vec(diagnostic::Diagnostic*, {});
    auto first_token = current;

    while (true) {
        if (current.isSuccess() && current.getValue()->getType() == until) {
            break;
        }

        if (current.isFailure() ||
            (current.isSuccess() && (until != lexer::Token::Type::EndOfFile &&
                                     current.getValue()->getType() ==
                                         lexer::Token::Type::EndOfFile))) {
            auto note = new diagnostic::Note(
                "unterminated statement block.",
                (first_token.isSuccess() ? first_token.getValue()->getSpan()
                 : current.isSuccess()   ? current.getValue()->getSpan()
                                         : io::string::Span()));
            auto diagnostics = Vec(
                diagnostic::Diagnostic*,
                {expect({lexer::Token::getTypeAsString(until)}).getError()});

            diagnostics.push_back(note);
            errors->insert(errors->end(), diagnostics.begin(),
                           diagnostics.end());

            break;
        }

        auto currentIsFailure = current.isFailure();
        auto next = parseNext();

        if (next.isSuccess()) {
            tree.push_back(next.getValue());
        } else {
            errors->insert(errors->end(), next.getError()->begin(),
                           next.getError()->end());

            if (currentIsFailure) {
                skip();
            }
        }
    }

    if (current.isSuccess()) {
        EatAndReturnIfError({until});
    }

    if (errors->empty()) {
        return new syntax::primary::Block(tree);
    } else {
        return errors;
    }
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseName() {
    if (currentIsReservedWord()) {
        return parseReservedWord();
    }

    auto value = current.getValue();
    EatAndReturnIfError({current.getValue()->getType()});
    return value;
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseReservedWord() {
    if (current.isFailure()) {
        return new Vec(diagnostic::Diagnostic*, {current.getError()});
    }

    if (current.getValue()->getValue() == "if") {
        return parseIfStatement();
    } else if (current.getValue()->getValue() == "while") {
        return parseWhileStatement();
    } else if (current.getValue()->getValue() == "proc") {
        return parseProcedureDeclaration();
    }

    auto value = current.getValue();
    EatAndReturnIfError({current.getValue()->getType()});
    return value;
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseIfStatement() {
    auto value = current.getValue()->getValue();
    auto confirmedElse = value == "else";
    EatAndReturnIfError({lexer::Token::Type::Identifier});
    DefineAndReturnIfError(body, parseBlock(lexer::Token::Type::SemiColon));

    syntax::statement::conditional::If* elseClause = nullptr;
    if (!confirmedElse &&
        (currentIsReservedWord() && current.getValue()->getValue() == "else")) {
        DefineAndReturnIfError(result, parseIfStatement());

        elseClause = dynamic_cast<syntax::statement::conditional::If*>(
            result.getValue());
    }

    return new syntax::statement::conditional::If(
        dynamic_cast<syntax::primary::Block*>(body.getValue()), elseClause);
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseWhileStatement() {
    EatAndReturnIfError({lexer::Token::Type::Identifier});
    DefineAndReturnIfError(body, parseBlock(lexer::Token::Type::SemiColon));

    return new syntax::statement::conditional::While(
        dynamic_cast<syntax::primary::Block*>(body.getValue()));
}

bisl::diagnostic::DiResult<
    bisl::text::parser::syntax::AbstractSyntaxTree*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::parseProcedureDeclaration() {
    EatAndReturnIfError({lexer::Token::Type::Identifier});
    DefineAndReturnIfError(name, getName());
    EatAndReturnIfError({lexer::Token::Type::Identifier});
    DefineAndReturnIfError(body, parseBlock(lexer::Token::Type::SemiColon));

    return new syntax::statement::declaration::Procedure(
        *name.getValue(),
        dynamic_cast<syntax::primary::Block*>(body.getValue()));
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::parser::Parser::eat(const std::vector<lexer::Token::Type>& types) {
    if (std::find(types.begin(), types.end(), current.getValue()->getType()) ==
        types.end()) {
        auto typesaAsformatVector = Vec(io::format::types::String*, {});

        for (const auto& type : types) {
            typesaAsformatVector.push_back(new io::format::types::String(
                lexer::Token::getTypeAsString(type)));
        }

        return currentError = new diagnostic::Error(
                   diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                   diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                   current.getValue()->getSpan(),
                   "unexpected token " +
                       (current.getValue()->getValue().empty()
                            ? current.getValue()->getTypeAsString()
                            : '`' + current.getValue()->getValue() + '`') +
                       ". " +
                       (typesaAsformatVector.size() > 1
                            ? "expected one of these types"
                            : "expected") +
                       ": " + typesaAsformatVector.formatNoBraces());
    }

    return current = lexer->lexNext();
}

bisl::diagnostic::DiResult<bisl::io::format::types::String*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::parser::Parser::expect(
    const std::vector<std::string>& names) const {
    if (current.isFailure()) {
        return current.getError();
    }

    if (std::find(names.begin(), names.end(), current.getValue()->getValue()) !=
        names.end()) {
        return new io::format::types::String(current.getValue()->getValue());
    }

    auto namesAsformatVector = Vec(io::format::types::String*, {});

    for (const auto& name : names) {
        namesAsformatVector.push_back(new io::format::types::String(name));
    }

    return new diagnostic::Error(
        diagnostic::Error::Type::ErrorTypeUnexpectedToken,
        diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
        current.getValue()->getSpan(),
        "unexpected token `" + current.getValue()->getValue() + "`. " +
            (namesAsformatVector.size() > 1 ? "expected one of these tokens"
                                            : "expected") +
            ": " + namesAsformatVector.formatNoBraces());
}

bisl::diagnostic::DiResult<
    bisl::io::format::types::String*,
    bisl::io::format::types::Vector<bisl::diagnostic::Diagnostic*>*>
bisl::text::parser::Parser::getName() const {
    if (current.isSuccess()) {
        if (currentIsReservedWord()) {
            DefineAndReturnVecIfError(expectation, diagnostic::Diagnostic*,
                                      expect({"name"}));
        } else {
            return new io::format::types::String(current.getValue()->getValue(),
                                                 current.getValue()->getSpan());
        }
    }

    return new Vec(diagnostic::Diagnostic*, {current.getError()});
}

bisl::diagnostic::DiResult<bisl::text::lexer::Token*,
                           bisl::diagnostic::Diagnostic*>
bisl::text::parser::Parser::skip() {
    return current = lexer->lexNext();
}

bool bisl::text::parser::Parser::currentIsReservedWord() const {
    auto reservedWords = std::vector({"if", "else", "while", "proc"});

    return current.isSuccess() &&
           current.getValue()->getType() == lexer::Token::Type::Identifier &&
           std::find(reservedWords.begin(), reservedWords.end(),
                     current.getValue()->getValue()) != reservedWords.end();
}
