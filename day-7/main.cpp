#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <iterator>
#include <ranges>
#include <unordered_map>

//#include <absl/strings/str_split.h>

#include <bits/ranges_algo.h>

import Util;

[[nodiscard]] bool test_operators(
    const long answer,
    const long current_value,
    const std::vector<long> &elements,
    const std::vector<long>::iterator left
) {
    //std::cout << current_value << std::endl;
    if (left == elements.end()) {
        if (current_value == answer)
            return true;
        return false;
    }


    const bool output_plus = test_operators(answer, current_value + *left, elements, left + 1);
    const bool output_times = test_operators(answer, current_value * *left, elements, left + 1);

    return output_plus || output_times;
}

int main() {
    // Load data
    using std::operator""sv;

    // load data
    auto initial_read = util::load_to_vector("./puzzle_input_day_7.txt");

    std::vector<std::string> answers_strings{}, elements_strings{};
    for (const auto &vec: util::split_vector_of_strings(initial_read, ": "sv)) {
        answers_strings.emplace_back(vec[0]);
        elements_strings.emplace_back(vec[1]);
    }

    auto answers = util::string_to_long(answers_strings);

    auto elements = util::split_vector_of_strings(elements_strings, " ")
                    | std::views::transform([](const std::vector<std::string> &vec) {
                        return util::string_to_long(vec);
                    })
                    | std::ranges::to<std::vector>();

    long output_problem_1 = 0;
    for (int i = 0; i < answers.size(); ++i) {
        if (test_operators(answers[i], elements[i][0], elements[i], elements[i].begin() + 1)) {
            output_problem_1 += answers[i];
        }
    }

    // problem 1
    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;

    // problem 2
    // the two branches split into 6, as there are now three operators,
    // but for each, the future operation could also be concat and needs to be accounted for

    return 0;
}
