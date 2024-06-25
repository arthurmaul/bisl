#pragma once

#include <string>

namespace bisl::io::format {
class Display {
  public:
    virtual std::string format() = 0, debug() = 0;
};
} // namespace bisl::io::format
