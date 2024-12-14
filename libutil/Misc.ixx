//
// Created by kyle on 2024/12/07.
//

module;
#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <filesystem>
#include <algorithm>
#include <vector>


export module Util:Misc;

namespace util {
    using std::operator""sv;

    export typedef std::vector<std::string> string_vec;
    export typedef std::vector<std::vector<std::string> > string_vec_vec;

    export auto string_to_long(const string_vec &input) -> std::vector<long> {
        return input | std::views::transform([](const std::string &str) {
                   return std::stol(str);
               })
               | std::ranges::to<std::vector>();
    }


    export auto load_to_vector(const std::filesystem::path &file_path) -> string_vec {
        auto input_file_ifstream = std::ifstream(file_path);

        if (!input_file_ifstream.is_open()) {
            std::cout << "Failed to open file" << std::endl;
            throw std::runtime_error("Could not open file at path: '{}'" + file_path.string());
        }

        std::string single_line;
        string_vec input_lines;

        while (std::getline(input_file_ifstream, single_line)) {
            input_lines.emplace_back(single_line);
        }
        return std::move(input_lines);
    }


    export auto split_vector_of_strings(
        const string_vec &input,
        const std::string_view delim
    ) -> string_vec_vec {
        return input | std::views::transform([delim](const std::string &str) {
            return std::views::split(str, delim)
                   | std::views::transform([](const auto &a) { return std::string(std::string_view{a}); })
                   | std::ranges::to<std::vector>();
        }) | std::ranges::to<std::vector>();
    }
}
