#pragma once

#include <iostream>
#include <string>

#include "../../string/span.hpp"
#include "../display.hpp"

namespace bisl::io::format::types {
class String: Display, public std::string {
  public:
    String(const std::string& value) { this->assign(value); }

    String(const std::string& value, const io::string::Span& span): span(span) {
        this->assign(value);
    }

    std::string get() const { return *this; }

    io::string::Span getSpan() const { return span; }

    std::string format() override { return *this; }

    std::string debug() override {
        return "String(value: std::string(\"" + *this + "\")";
    }

  private:
    const io::string::Span span;
};
} // namespace bisl::io::format::types
