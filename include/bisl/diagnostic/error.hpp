#pragma once

#include <filesystem>

#include "../io/format/display.hpp"
#include "../io/string/span.hpp"
#include "diagnostic.hpp"

#define DefineAndReturnIfError(name, value)                                    \
    auto name = value;                                                         \
    if (name.isFailure())                                                      \
    return name.getError()

#define DefineAndReturnVecIfError(name, errorType, value)                      \
    auto name = value;                                                         \
    if (name.isFailure())                                                      \
    return new Vec(errorType, {name.getError()})

#define DefineAndReturnVecIfErrorAndSkip(name, errorType, value)               \
    auto name = value;                                                         \
    if (name.isFailure()) {                                                    \
        skip();                                                                \
        return new Vec(errorType, {name.getError()});                          \
    }

namespace bisl::diagnostic {
class Error: public Diagnostic {
  public:
    enum Type : unsigned char {
        ErrorTypeFileNotFound,
        ErrorTypeFileNotRegular,
        ErrorTypeFileNotReadable,
        ErrorTypeInvalidSyntax,
        ErrorTypeUnexpectedToken,
        ErrorTypeColorNotFound,
        ErrorTypeMismatchedTypes,
        ErrorTypeRedefinition,
        ErrorTypeCount
    };

    enum Icon : unsigned char {
        ErrorIconFileQuestionMark,
        ErrorIconFileCross,
        ErrorIconFileLock,
        ErrorIconProgramCross,
        ErrorIconCurlyBracesCross,
        ErrorIconColorLine,
        ErrorIconName,
        ErrorIconCount
    };

    Error(const Type& type, const Icon& icon, const io::string::Span& span,
          const std::string& message, const bool& showSource = true)
        : Diagnostic(Diagnostic::Type::Error, span, message), errorType(type),
          icon(icon), showSource(showSource) {}

    explicit Error()
        : Diagnostic(Diagnostic::Type::Error),
          errorType(Type::ErrorTypeFileNotFound),
          icon(Icon::ErrorIconFileQuestionMark), showSource(false) {}

    unsigned char getNumber() const { return errorType; }

    std::string getTypeAsString() const, getIconAsString() const,
        format() override, debug() override;

  private:
    const Type errorType;
    const Icon icon;
    const bool showSource;
};
} // namespace bisl::diagnostic
