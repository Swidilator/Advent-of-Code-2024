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
    obstacle, open_space
};

struct MapElement {
    MapElementType type;
    bool stepped_on = false;
};

enum class GuardDirection {
    up, down, left, right
};

enum class WalkResult {
    moved, unmoved, exited
};

struct coord {
    int x, y;
};

class PuzzleMap {
    std::size_t _width, _height = 0;
    coord _guard_starting_position{};
    std::vector<std::vector<MapElement> > _map{};
public:


    [[nodiscard]] coord guard_starting_position() const {
        return _guard_starting_position;
    }

    [[nodiscard]] std::size_t width() const {
        return _width;
    }

    [[nodiscard]] std::size_t height() const {
        return _height;
    }

    // referenced x,y = 0 at top left, row major
    [[nodiscard]] MapElement &operator[](const std::size_t x, const std::size_t y) {
        if (x < _height && y < _width) {
            return _map[x][y];
        }
        throw std::out_of_range("Invalid map coordinates");
    }

    [[nodiscard]] const MapElement &operator[](const std::size_t x, const std::size_t y) const {
        if (x < _height && y < _width) {
            return _map[x][y];
        }
        throw std::out_of_range("Invalid map coordinates");
    }

    [[nodiscard]] int count_stepped_on() const {
        int num_stepped_on{};
        std::ranges::for_each(_map, [&num_stepped_on](const std::vector<MapElement> &vec) {
            std::ranges::for_each(vec, [&num_stepped_on](const MapElement &e) {
                if (e.stepped_on) {
                    num_stepped_on++;
                }
            });
        });
        return num_stepped_on;
    }

    explicit PuzzleMap(const std::vector<std::string> &input_map) {
        _height = input_map.size();
        _width = input_map[0].size();

        int x = 0;
        std::ranges::for_each(input_map, [this, &x](const std::string &str) -> void {
            std::vector<MapElement> row{};
            for (std::size_t y = 0; y < str.size(); ++y) {
                if (str[y] == '#')
                    row.emplace_back(MapElement{MapElementType::obstacle, false});
                else if (str[y] == '.')
                    row.emplace_back(MapElement{MapElementType::open_space, false});
                else if (str[y] == '^') {
                    _guard_starting_position = coord(x, y);
                    row.emplace_back(MapElement{MapElementType::open_space, true});
                }
            }
            _map.emplace_back(std::move(row));
            ++x;
        });
    }
};

class Guard {
    coord _guard_pos{};
    PuzzleMap _map;
    GuardDirection dir{};

    [[nodiscard]] WalkResult take_step() {
        int x_movement = -(dir == GuardDirection::up) + (dir == GuardDirection::down);
        int y_movement = -(dir == GuardDirection::left) + (dir == GuardDirection::right);

        // std::cout << std::format("x: {}, y: {}, x_movement: {}, y_movement: {}",
        //                          _guard_pos.x,
        //                          _guard_pos.y,
        //                          x_movement,
        //                          y_movement
        // ) << std::endl;

        if (_guard_pos.x + x_movement < 0
            || _guard_pos.x + x_movement >= _map.height()
            || _guard_pos.y + y_movement < 0
            || _guard_pos.y + y_movement >= _map.width()
        ) {
            return WalkResult::exited;
        }

        if (MapElement &e{_map[_guard_pos.x + x_movement, _guard_pos.y + y_movement]};
            e.type != MapElementType::obstacle) {
            e.stepped_on = true;
            _guard_pos = coord(_guard_pos.x + x_movement, _guard_pos.y + y_movement);
            return WalkResult::moved;
        }
        return WalkResult::unmoved;
    }

    void turn_90_degrees() {
        if (dir == GuardDirection::up)
            dir = GuardDirection::right;
        else if (dir == GuardDirection::right)
            dir = GuardDirection::down;
        else if (dir == GuardDirection::down)
            dir = GuardDirection::left;
        else if (dir == GuardDirection::left)
            dir = GuardDirection::up;

        //std::cout << static_cast<std::underlying_type_t<GuardDirection>>(dir) << std::endl;
    }

public:
    explicit Guard(PuzzleMap &&map)
        : _map{std::move(map)}, dir{GuardDirection::up} {
        _guard_pos = _map.guard_starting_position();
    }

    void walk_route() {
        WalkResult r{};
        int steps_taken{};
        while ((r = take_step()) != WalkResult::exited) {
            if (r == WalkResult::moved) {
                //std::cout << std::format("steps_taken: {}", steps_taken) << std::endl;
                steps_taken++;
                //std::cout << std::format("steps_taken: {}", steps_taken) << std::endl;
            } else {
                turn_90_degrees();
            }
        }
    }

    [[nodiscard]] int count_stepped_on() const {
        return _map.count_stepped_on();
    }
};


int main() {
    // Load data
    using std::operator""sv;

    Guard g{PuzzleMap{util::load_to_vector("./puzzle_input_day_6.txt")}};

    g.walk_route();
    int output_problem_1 = g.count_stepped_on();


    std::cout << "Answer for problem 1 is: " << output_problem_1 << std::endl;
    return 0;
}
