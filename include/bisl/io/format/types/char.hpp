#pragma once

#include "../display.hpp"

namespace bisl::io::format::types {
class Char: Display {
  public:
    explicit Char(const char& value): value(value) {}

    char getValue() const { return value; }

    std::string format() override { return std::string({value}); }

    std::string debug() override {
        return "Char(value: \'" + std::string({value}) + "\')";
    }

  private:
    char value;
};
} // namespace bisl::io::format::types
