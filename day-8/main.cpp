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

enum class MapElementType {
    open_space, antenna
};

class MapElement : public util::MapElementBase<char> {
    MapElementType _map_element_type{};

public:
    bool antinode = false;


    MapElement(const char c, const util::Coordinates &coords)
        : MapElementBase{c, coords} {
        if (c == '.') {
            _map_element_type = MapElementType::open_space;
        } else {
            _map_element_type = MapElementType::antenna;
        }
    }

    const char &get_raw_data() const {
        return _data;
    }

    MapElementType get_map_element_type() const {
        return _map_element_type;
    }

    void print_data() {
        std::cout << _data << std::endl;
    }
};

util::Coordinates calculate_antinode(const util::Coordinates &a, const util::Coordinates &b) {
    return {b.x + (b.x - a.x), b.y + (b.y - a.y)};
}

int main() {
    // Load data
    using std::operator""sv;

    std::vector<std::vector<char> > input_data = util::load_to_vector("./puzzle_input_day_8_example.txt")
                                                 | std::views::transform([](const std::string &str) {
                                                     return str | std::ranges::to<std::vector>();
                                                 })
                                                 | std::ranges::to<std::vector>();

    util::Grid<char, MapElement> grid{input_data};

    std::unordered_map<char, std::vector<std::reference_wrapper<MapElement> > > antennas{};
    for (std::size_t x = 0; x < grid.height(); ++x) {
        for (std::size_t y = 0; y < grid.width(); ++y) {
            if (grid[x,y].get_map_element_type() == MapElementType::antenna) {
                antennas[grid[x,y].get_raw_data()].emplace_back(std::reference_wrapper(grid[x,y]));
            }
        }
    }

    bool solution_1_only = true;

    for (const auto &kv: antennas) {
        for (const auto &a: kv.second) {
            for (const auto &b: kv.second) {
                auto a_coords = a.get().get_coordinates();
                auto b_coords = b.get().get_coordinates();
                if (a_coords == b_coords) {
                    continue;
                }


                if (!solution_1_only) {
                    a.get().antinode = true;
                    b.get().antinode = true;
                }


                auto antinode_pos = calculate_antinode(a_coords, b_coords);
                std::cout << std::format("x: {}  y: {}\n", antinode_pos.x, antinode_pos.y);

                while (grid.contains(antinode_pos)) {
                    grid[antinode_pos.x, antinode_pos.y].antinode = true;
                    if (solution_1_only) {
                        break;
                    }
                    a_coords = b_coords;
                    b_coords = antinode_pos;
                    antinode_pos = calculate_antinode(a_coords, b_coords);
                }
            }
        }
    }


    int output_problem{};
    for (std::size_t x = 0; x < grid.height(); ++x) {
        for (std::size_t y = 0; y < grid.width(); ++y) {
            if (grid[x,y].antinode) {
                output_problem += 1;
            }
        }
    }

    std::cout << std::format(
        "Answer for problem {} is: {}\n",
        solution_1_only ? "1" : "2",
        output_problem
    );

    return 0;
}
