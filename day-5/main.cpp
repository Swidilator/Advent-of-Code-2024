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

#include "utilities.h"

int main() {
    // Load data
    using std::operator""sv;

    // Get rules
    auto input_strings_rules = util::split_vector_of_strings(
                                   util::load_to_vector("./puzzle_input_day_5_rules.txt"),
                                   "|"sv
                               )
                               | std::views::transform([](const auto &vec) {
                                   return std::pair<int, int>{
                                       util::string_to_int(std::string(vec[0])),
                                       util::string_to_int(std::string(vec[1]))
                                   };
                               })
                               | std::ranges::to<std::vector>();

    // Get updates
    auto input_strings_updates = util::split_vector_of_strings(
                                     util::load_to_vector("./puzzle_input_day_5_updates.txt"),
                                     ","sv
                                 )
                                 | std::views::transform([](const std::vector<std::string> &vec) {
                                     return util::string_to_int(vec);
                                 })
                                 | std::ranges::to<std::vector>();

    // create rules blacklist
    std::unordered_map<int, std::vector<int> > rules_map;
    for (const auto &p: input_strings_rules) {
        rules_map[p.second].emplace_back(p.first);
    }

    int sum_middle_values{};

    // find violations using blacklist
    std::ranges::for_each(input_strings_updates, [&](const std::vector<int> &update_vec) {
        bool error_found = false;
        for (int i = 0; i < update_vec.size(); ++i) {
            if (error_found) {
                break;
            }
            for (int j = i + 1; j < update_vec.size(); ++j) {
                if (rules_map.contains(update_vec[i])) {
                    for (const int &r: rules_map[update_vec[i]]) {
                        if (r == update_vec[j]) {
                            error_found = true;
                            break;
                        }
                    }
                }
            }
        }
        if (!error_found) {
            sum_middle_values += update_vec[update_vec.size() / 2];
        }
    });


    std::cout << "Answer for problem 1 is: " << sum_middle_values << std::endl;
    return 0;
}
