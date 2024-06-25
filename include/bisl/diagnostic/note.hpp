#pragma once

#include "diagnostic.hpp"

namespace bisl::diagnostic {
class Note: public Diagnostic {
  public:
    Note(const std::string& message, const io::string::Span& span,
         const bool& showLine = false)
        : Diagnostic(Diagnostic::Type::Note, span, message),
          showLine(showLine) {}

    std::string format() override, debug() override;

  private:
    const bool showLine;
};
} // namespace bisl::diagnostic
