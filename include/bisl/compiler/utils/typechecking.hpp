#pragma once

namespace bisl::compiler::utils {
template<typename T, typename U> bool isDynamicallyCastableTo(const T& which) {
    return dynamic_cast<U>(which);
}
} // namespace bisl::compiler::utils
