#pragma once

#include "diagnostic.hpp"

namespace bisl::diagnostic {
class Warning: public Diagnostic {
  public:
    enum Type : unsigned char {
        WarningTypeUnknownType,
        WarningTypeUndeclaredName,
        WarningTypeCount
    };

    enum Icon : unsigned char {
        WarningIconRectangle,
        WarningIconName,
        WarningIconCount
    };

    Warning(const Type& type, const Icon& icon, const io::string::Span& span,
            const std::string& message)
        : Diagnostic(Diagnostic::Type::Warning, span, message),
          warningType(type), icon(icon) {}

    explicit Warning()
        : Diagnostic(Diagnostic::Type::Warning),
          warningType(Type::WarningTypeCount), icon(Icon::WarningIconCount) {}

    unsigned char getNumber() const { return warningType; }

    std::string getTypeAsString() const, getIconAsString() const,
        format() override, debug() override;

  private:
    const Type warningType;
    const Icon icon;
};
} // namespace bisl::diagnostic
