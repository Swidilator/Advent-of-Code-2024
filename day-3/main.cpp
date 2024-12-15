#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <iterator>

//#include <absl/strings/str_split.h>

long long stoi(std::string& input) {
    long long result;
    std::from_chars(input.data(), input.data() + input.size(), result);
    return result;
}

int main() {
    // Load data
    using std::operator""sv;

    auto input_file_ifstream = std::ifstream("./puzzle_input_day_3.txt");

    if (!input_file_ifstream.is_open()) {
        std::cout << "Failed to open file" << std::endl;
    }

    std::vector<std::string> input_lines;
    std::vector<std::vector<int> > reports;


    std::string single_line;
    while (std::getline(input_file_ifstream, single_line)) {
        input_lines.emplace_back(single_line);
    }

    long long output_problem_1{};

    std::vector<std::string> all_matches;

    std::regex full_mul_regex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");

    for (auto& single_input: input_lines) {
        auto words_begin = std::sregex_iterator(single_input.begin(), single_input.end(), full_mul_regex);
        auto words_end = std::sregex_iterator();

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            std::smatch match = *i;
            std::string match_str = match.str();
            long long single_output = stoi(match[1].str()) * stoi(match[2]);
            std::cout << std::format("{}: {} x {} = {}\n", match[0].str(), match[1].str(), match[2].str(),
                                     single_output);

            output_problem_1 += single_output;
        }
    }

    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;
    return 0;
}
