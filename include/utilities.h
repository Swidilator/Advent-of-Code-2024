//
// Created by kyle on 2024/12/07.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <ranges>
#include <filesystem>
#include <algorithm>

namespace util {
    using std::operator""sv;

    // template<typename R, typename T>
    // concept input_range_of =
    //     std::ranges::input_range<R> &&
    //         std::convertible_to<std::ranges::range_value_t<R>, T>;
    //
    //
    // class VectorStringToInt {
    // public:
    //     std::vector<int> data{};
    //     explicit VectorStringToInt(input_range_of<int> auto&& range)
    //     {
    //         data = range | std::views::transform([](const auto&& inp) {
    //             return std::stoi(inp);
    //         }) | std::ranges::to<std::vector>();
    //     }
    // };

    // inline int string_to_int(const std::string_view &input) {
    //     return string_to_int(std::string(input));
    // }

    inline long string_to_long(const std::string &input) {
        return std::stol(input);
    }

    // inline std::vector<int> string_to_int(const std::vector<const char*> &input) {
    //     return input | std::views::transform([](const char* ch){return std::string{ch};})
    //     | std::ranges::to<std::vector>();
    // }

    inline std::vector<long> string_to_long(const std::vector<std::string> &input) {
        return input | std::views::transform([](const std::string &str) {
                   return std::stol(str);
               })
               | std::ranges::to<std::vector>();
    }

    inline std::vector<std::string> load_to_vector(const std::filesystem::path &file_path) {
        auto input_file_ifstream = std::ifstream(file_path);

        if (!input_file_ifstream.is_open()) {
            std::cout << "Failed to open file" << std::endl;
            throw std::runtime_error("Could not open file at path: '{}'" + file_path.string());
        }

        std::string single_line;
        std::vector<std::string> input_lines;

        while (std::getline(input_file_ifstream, single_line)) {
            input_lines.emplace_back(single_line);
        }
        return std::move(input_lines);
    }

    inline std::vector<std::vector<std::string> > split_vector_of_strings(
        const std::vector<std::string> &input, const std::string_view delim) {
        return input | std::views::transform([delim](const std::string &str) {
            return std::views::split(str, delim)
                   | std::views::transform([](const auto &a) { return std::string(std::string_view{a}); })
                   | std::ranges::to<std::vector>();
        }) | std::ranges::to<std::vector>();
    }


    struct Coordinates {
        long x, y;

        bool operator==(const Coordinates & b) const {
            return (x == b.x) && (y == b.y);
        }
    };


    template<typename T>
    class MapElementBase {
    protected:
        T _data{};
        Coordinates _coordinates;

    public:
        explicit MapElementBase(const T data, const Coordinates& c)
        : _data{data}, _coordinates{c} {
        }

        const Coordinates& get_coordinates() const {
            return _coordinates;
        }
    };



    template<typename S, std::derived_from<MapElementBase<S>> E>
    class Grid {
        std::size_t _width, _height = 0;
        std::vector<std::vector<E> > _map{};

    public:
        [[nodiscard]] std::size_t width() const {
            return _width;
        }

        [[nodiscard]] std::size_t height() const {
            return _height;
        }

        // referenced x,y = 0 at top left, row major
        [[nodiscard]] E &operator[](const std::size_t x, const std::size_t y) {
            if (x < _height && y < _width) {
                return _map[x][y];
            }
            throw std::out_of_range("Invalid map coordinates");
        }

        [[nodiscard]] const E &operator[](const std::size_t x, const std::size_t y) const {
            if (x < _height && y < _width) {
                return _map[x][y];
            }
            throw std::out_of_range("Invalid map coordinates");
        }

        explicit Grid(const std::vector<std::vector<S> > &input_map) {
            _height = input_map.size();
            _width = input_map[0].size();

            int x = 0;
            std::ranges::for_each(input_map, [this, &x](const std::vector<S> &input_row) -> void {
                std::vector<E> row{};
                for (std::size_t y = 0; y < input_row.size(); ++y) {
                    row.emplace_back(input_row[y], Coordinates(x, y));
                }
                _map.emplace_back(std::move(row));
                ++x;
            });
        }
    };
}

#endif //UTILITIES_H
