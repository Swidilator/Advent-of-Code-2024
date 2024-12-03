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
    // Load data
    using std::operator""sv;

    auto input_file_ifstream = std::ifstream("./puzzle_input_day_2.txt");

    if (!input_file_ifstream.is_open()) {
        std::cout << "Failed to open file" << std::endl;
    }

    std::vector<std::string> input_lines;
    std::vector<std::vector<int>> reports;


    std::string single_line;
    while (std::getline(input_file_ifstream, single_line)) {
        //input_lines.emplace_back(single_line);

        constexpr auto delim{" "sv};

        std::vector<int> split_line;

        for (const auto &&word: single_line | std::views::split(delim)) {
            int result;
            std::from_chars(word.data(), word.data() + word.size(), result);
            split_line.emplace_back(result);
        }

        reports.emplace_back(std::move(split_line));

        // abseil library from google, better, but I wanted to use std library
        //std::vector<std::string> split_string = absl::StrSplit(single_line, ' ', absl::SkipEmpty());
    }

    // problem 1
    struct GradientChecker {
        int num_safe{};

        void operator()(const std::vector<int> &single_report) {
            int grad = 0;

            for (int i = 0; i < single_report.size() - 1; ++i) {
                int diff{single_report[i + 1] - single_report[i]};
                if (std::abs(diff) > 3 or diff == 0) {
                    return;
                }

                int this_grad = diff / std::abs(diff);

                if (grad == 0) {
                    grad = this_grad;
                } else if (grad != this_grad) {
                    return;
                }
            }
            num_safe += 1;
        }
    };

    auto [i, output] = std::ranges::for_each(reports, GradientChecker());


    int output_problem_1{output.num_safe};

    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;

    // problem 2d

    return 0;
}
