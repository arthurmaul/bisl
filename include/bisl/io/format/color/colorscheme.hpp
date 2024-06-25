#pragma once

#include "../../../diagnostic/error.hpp"
#include "../../../diagnostic/result.hpp"
#include "../display.hpp"
#include "../types/map.hpp"
#include "../types/string.hpp"
#include "color.hpp"

namespace bisl::io::format::color {
class Colorscheme: Display {
  public:
    Colorscheme(const types::Map<types::String*, Color*>& colors)
        : colors(colors) {}

    diagnostic::DiResult<types::String*, diagnostic::Diagnostic*>
    color(const std::string& target) const,
        color(const std::string& target, const std::string& which,
              string::Span span) const;

    std::string format() override { return colors.format(); }

    std::string debug() override {
        return "Colorscheme(colors: " + colors.debug() + ')';
    }

  private:
    types::Map<types::String*, Color*> colors;
};
} // namespace bisl::io::format::color
