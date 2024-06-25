#include "../../../../include/bisl/io/file/reader.hpp"
#include "../../../../include/bisl/io/string/span.hpp"

#include <fstream>
#include <iostream>

std::string bisl::io::file::Reader::readAll() const { return content; }

std::string
bisl::io::file::Reader::readLine(const unsigned long long int& index) {
    std::stringstream stream(content);
    std::string line;
    unsigned long long int currentLine = 0;

    while (std::getline(stream, line, '\n')) {
        if (currentLine == index - 1) {
            return line;
        }

        currentLine++;
    }

    return "";
}

std::string bisl::io::file::Reader::format() {
    return "-- " + path + " --\n" + readAll() + '\n';
}

std::string bisl::io::file::Reader::debug() {
    return "Reader(path: \"" + path + "\", content: [content; " +
           std::to_string(readAll().length()) + " bytes]}";
}
