#include "../../../../../include/bisl/io/format/color/colorscheme.hpp"

bisl::diagnostic::DiResult<bisl::io::format::types::String*,
                           bisl::diagnostic::Diagnostic*>
bisl::io::format::color::Colorscheme::color(const std::string& target) const {
    auto span = string::Span("color parser", "", 0, 1, 1, target.length());

    auto current = target[*span.getIndex()];
    auto result = new types::String("");

    while (*span.getIndex() < target.length()) {
        if (current == '#') {
            if (target[*span.getIndex() + 1] == '{') {
                current = target[*span.getIndex() += 2];
                auto currentColorSequenceKey = std::string();
                auto currentColorSequenceValue = std::string();

                while (*span.getIndex() < target.length() && current != '}') {
                    currentColorSequenceKey.push_back(current);
                    current = target[*span.getIndex() += 1];
                }

                if (current != '}') {
                    return new diagnostic::Error(
                        diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        diagnostic::Error::Icon::ErrorIconProgramCross,
                        span.withColumnOf(*span.getIndex() + 1),
                        "unterminated color sequence near key");
                }

                if ((current = target[*span.getIndex() += 1]) != '(') {
                    return new diagnostic::Error(
                        diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                        diagnostic::Error::Icon::ErrorIconProgramCross,
                        span.withColumnOf(*span.getIndex() + 1),
                        "expected '(' after color sequence start");
                }

                current = target[*span.getIndex() += 1];

                while (*span.getIndex() < target.length() && current != '#') {
                    currentColorSequenceValue.push_back(current);
                    current = target[*span.getIndex() += 1];
                }

                if (current != '#' &&
                    (current = target[*span.getIndex() += 1]) != ')') {
                    return new diagnostic::Error(
                        diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        diagnostic::Error::Icon::ErrorIconProgramCross,
                        span.withColumnOf(*span.getIndex() + 1),
                        "unterminated color sequence near value");
                }

                current = target[*span.getIndex() += 1];

                if (colors.has(new types::String(currentColorSequenceKey))) {
                    result->append(
                        "\x1b" +
                        colors.get(new types::String(currentColorSequenceKey))
                            ->get() +
                        currentColorSequenceValue + "\x1b" +
                        Color(Color::Type::ColorTypeReset).get());
                } else {
                    return new diagnostic::Error(
                        diagnostic::Error::Type::ErrorTypeColorNotFound,
                        diagnostic::Error::Icon::ErrorIconColorLine,
                        span.withColumnOf(*span.getIndex() + 1),
                        "could not find color `" + currentColorSequenceValue +
                            '`');
                }
            }
        } else {
            result->push_back(current);
        }

        current = target[*span.getIndex() += 1];
    }

    return result;
}

bisl::diagnostic::DiResult<bisl::io::format::types::String*,
                           bisl::diagnostic::Diagnostic*>
bisl::io::format::color::Colorscheme::color(const std::string& target,
                                            const std::string& which,
                                            string::Span span) const {
    auto copy = target;

    copy.insert(*span.getColumn() - 1, "#{" + which + "}(");
    copy.insert(*span.getColumn() - 1 + *span.getLength() +
                    ("#{" + which + "})").length(),
                "#)");

    return color(copy);
}
