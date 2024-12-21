#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <regex>
#include <ranges>

//#include <absl/strings/str_split.h>

#include <set>
#include <unordered_map>
#include <bits/ranges_algo.h>

import Util;

class MapElement
        : public util::GridElementBase<char> {
public:
    bool _used = false;
    const int _height = -1;


    MapElement(char data, const util::Coordinates& c)
        : GridElementBase{data, c}, _height{data - 48} {
    }
};

using map_grid = util::Grid<char, MapElement>;

auto is_valid_step(
    const map_grid& g,
    const util::Coordinates& c,
    const util::Coordinates& c_next
) -> bool {
    return g.contains(c_next)
           && !g[c_next]._used
           && g[c_next]._height == g[c]._height + 1;
}

auto find_paths(const util::Coordinates& c, map_grid g) -> std::vector<std::string> {
    g[c]._used = true;

    if (g[c]._height == 9) {
        std::cout << std::format("Found coords: x: {}   y: {}\n", c.x, c.y);

        return std::vector{std::format("{}|{}", c.x, c.y)};
    }

    // look up
    const std::array move_coords{
        util::Coordinates{c.x - 1, c.y},
        util::Coordinates{c.x + 1, c.y},
        util::Coordinates{c.x, c.y - 1},
        util::Coordinates{c.x, c.y + 1}
    };

    std::vector<std::vector<std::string> > out{};

    std::ranges::for_each(move_coords, [&out, &g, &c](const util::Coordinates& c_next) {
        if (is_valid_step(g, c, c_next)) {
            std::cout << std::format("Next: val: {}   x: {}   y: {}\n", g[c_next]._height, c_next.x, c_next.y);
            auto a = find_paths(c_next, g);
            out.push_back(a);
        } else {
            //out.emplace_back();
        }
    });

    std::vector<std::string> out_all{};
    std::ranges::for_each(out, [&out_all](const std::vector<std::string> s) {
        out_all.insert(out_all.end(), s.begin(), s.end());
    });

    return out_all;
}


int main() {
    // Load data
    using std::operator""sv;


    const map_grid map{
        util::load_to_vector("./puzzle_input_day_10.txt")
        | std::views::transform([](const std::string& s) {
            return s | std::ranges::to<std::vector>();
        }) | std::ranges::to<std::vector>()
    };

    long output_problem_1{};

    for (long x = 0; x < map.height(); ++x) {
        for (long y = 0; y < map.width(); ++y) {
            if (map[x,y]._height != 0) {
                continue;
            }

            auto out = find_paths({x, y}, map);

            std::unordered_map<std::string, int> out_map{};
            std::ranges::for_each(out, [&out_map](const std::string& s) {
                out_map[s] += 1;
            });

            output_problem_1 += out_map.size();
        }
    }


    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;
    return 0;
}
