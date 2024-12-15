#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <ranges>

//#include <absl/strings/str_split.h>

#include <bits/ranges_algo.h>

import Util;

struct MemoryBlock {
    long id{};
    long value{};
    bool is_free_space{};
};

int main() {
    // Load data
    using std::operator""sv;

    std::string input_data = util::load_to_vector("./puzzle_input_day_9.txt")[0];

    // Expand into blocks
    std::vector<MemoryBlock> expanded_memory{};
    expanded_memory.reserve(input_data.size());

    bool free_space_switch = false;
    long block_id{};
    long block_value{};
    for (const char c: input_data) {
        long num_blocks{c - 48}; // convert char to long
        for (long b = 0; b < num_blocks; ++b) {
            expanded_memory.emplace_back(block_id, free_space_switch ? 0 : block_value, free_space_switch);
            block_id++;
        }
        if (!free_space_switch) {
            block_value++;
        }
        free_space_switch = !free_space_switch;
    }


    // Find empty blocks
    const auto empty_blocks = expanded_memory
                              | std::views::filter([](const MemoryBlock& b) {
                                  return b.is_free_space;
                              })
                              | std::views::transform([](MemoryBlock& b) {
                                  return std::ref(b);
                              })
                              | std::ranges::to<std::vector>();


    // Move blocks
    auto back_iterator = expanded_memory.end() - 1;
    for (auto r: empty_blocks) {
        while (back_iterator->is_free_space) {
            back_iterator -= 1;
        }
        if (r.get().id >= back_iterator->id) {
            break;
        }
        r.get().is_free_space = false;
        r.get().value = back_iterator->value;
        back_iterator->is_free_space = true;\
        back_iterator->value = 0;
        back_iterator -= 1;
    }


    // Calculate output
    long output_problem_1{};
    for (auto& b: expanded_memory) {
        if (!b.is_free_space) {
            output_problem_1 += b.id * b.value;
        }
    }

    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;
    return 0;
}
