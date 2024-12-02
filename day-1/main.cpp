#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <ranges>
#include <string_view>
#include <algorithm>
#include <unordered_map>

//#include <absl/strings/str_split.h>

int main() {
    using std::operator""sv;

    auto input_file_ifstream = std::ifstream("./puzzle_input.txt");

    if (!input_file_ifstream.is_open()) {
        std::cout << "Failed to open file" << std::endl;
    }

    std::vector<std::string> input_lines;
    std::vector<int> left_list;
    std::vector<int> right_list;


    std::string single_line;

    while (std::getline(input_file_ifstream, single_line)) {
        input_lines.emplace_back(single_line);

        constexpr auto delim{"   "sv};

        std::vector<int> split_line;

        for (const auto &&word: single_line | std::views::split(delim)) {
            int result;
            std::from_chars(word.data(), word.data() + word.size(), result);
            split_line.emplace_back(result);
        }

        left_list.emplace_back(split_line[0]);
        right_list.emplace_back(split_line[1]);

        // abseil library from google, better, but I wanted to use std library
        //std::vector<std::string> split_string = absl::StrSplit(single_line, ' ', absl::SkipEmpty());
    }

    int output_problem_1{};

    std::ranges::sort(left_list);
    std::ranges::sort(right_list);

    for (int i = 0; i < left_list.size(); ++i) {
        output_problem_1 += std::abs(left_list[i] - right_list[i]);
    }

    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;

    // problem 2
    auto right_list_hist = std::unordered_map<int, int>{};
    for (auto& val: right_list) {
        right_list_hist[val] += 1;
    }

    int output_problem_2{};

    for (auto& val: left_list) {
        output_problem_2 += val * right_list_hist[val];
    }

    std::cout << "Answer for problem 2 is: " << output_problem_2 << std::endl;

    return 0;
}
