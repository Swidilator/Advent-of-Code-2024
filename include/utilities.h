//
// Created by kyle on 2024/12/07.
//

#ifndef UTILITIES_H
#define UTILITIES_H
#include <charconv>
#include <string>
#include <ranges>
#include <filesystem>

namespace util {
    using std::operator""sv;

    inline int string_to_int(const std::string &input) {
        int result;
        std::from_chars(input.data(), input.data() + input.size(), result);
        return result;
    }

    inline std::vector<std::string> load_to_vector(const std::filesystem::path &file_path) {
        auto input_file_ifstream = std::ifstream(file_path);

        if (!input_file_ifstream.is_open()) {
            std::cout << "Failed to open file" << std::endl;
            throw std::runtime_error("Could not open file at path: '{}'" + file_path.string());
        }

        std::string single_line;
        std::vector<std::string> input_lines;

        while (std::getline(input_file_ifstream, single_line)) {
            input_lines.emplace_back(single_line);
        }
        return std::move(input_lines);
    }

    inline std::vector<std::vector<std::string_view> > split_vector_of_strings(
        const std::vector<std::string> &input, const std::string_view delim) {
        return input | std::views::transform([delim](const std::string &str) {
            return std::views::split(str, delim)
                   | std::views::transform([](const auto &a) { return std::string_view{a}; })
                   | std::ranges::to<std::vector>();
        }) | std::ranges::to<std::vector>();
    }
}

#endif //UTILITIES_H
