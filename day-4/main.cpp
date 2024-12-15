#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <iterator>
#include <ranges>

//#include <absl/strings/str_split.h>

import Util;

int main() {
    // Load data
    using std::operator""sv;

    auto input_strings = util::load_to_vector("./puzzle_input_day_4.txt");
    int input_width = input_strings[0].size();
    int input_height = input_strings.size();

    auto check_word = [](const std::string& word) {
        return word == "XMAS" || word == "SAMX";
    };

    int num_xmas{};
    // Brute force solution...
    for (int i = 0; i < input_height; ++i) {
        for (int j = 0; j < input_width; ++j) {
            //working clockwise
            //check up
            if (i >= 3) {
                std::string word{""};
                word += input_strings[i][j];
                word += input_strings[i - 1][j];
                word += input_strings[i - 2][j];
                word += input_strings[i - 3][j];
                if (check_word(word)) {
                    num_xmas += 1;
                }
            }

            //check up-right
            if (i >= 3 && j < input_width - 3) {
                std::string word{""};
                word += input_strings[i][j];
                word += input_strings[i - 1][j + 1];
                word += input_strings[i - 2][j + 2];
                word += input_strings[i - 3][j + 3];
                if (check_word(word)) {
                    num_xmas += 1;
                }
            }

            //check right
            if (j < input_width - 3
            ) {
                std::string word{""};
                word += input_strings[i][j];
                word += input_strings[i][j + 1];
                word += input_strings[i][j + 2];
                word += input_strings[i][j + 3];
                if (check_word(word)) {
                    num_xmas += 1;
                }
            }

            //check down-right
            if (i < input_height - 3 & j < input_width - 3
            ) {
                std::string word{""};
                word += input_strings[i][j];
                word += input_strings[i + 1][j + 1];
                word += input_strings[i + 2][j + 2];
                word += input_strings[i + 3][j + 3];
                if (check_word(word)) {
                    num_xmas += 1;
                }
            }
        }
    }

    std::cout << "Answer for problem 1 is: " << num_xmas << std::endl;
    return 0;
}
