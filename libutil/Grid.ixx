//
// Created by kyle on 2024/12/14.
//

module;

#include <string>
#include <ranges>
#include <vector>
#include <regex>
#include <algorithm>

export module Util:Grid;

namespace util {
    export struct Coordinates {
        long x, y;

        Coordinates(long x, long y)
            : x{x}, y{y} {
        }

        auto operator==(const Coordinates& b) const -> bool {
            return (x == b.x) && (y == b.y);
        }

        auto operator<(const Coordinates& b) const -> bool {
            return x < b.x || y < b.y;
        }

        auto operator<=(const Coordinates& b) const -> bool {
            return x <= b.x || y <= b.y;
        }
    };


    export template<typename T>
    class GridElementBase {
    protected:
        T _data{};
        Coordinates _coordinates;

    public:
        GridElementBase(const T data, const Coordinates& c)
            : _data{data}, _coordinates{c} {
        }


        auto get_coordinates() const -> const Coordinates& {
            return _coordinates;
        }
    };


    export template<typename S, std::derived_from<GridElementBase<S> > E>
    class Grid {
        std::size_t _width, _height = 0;
        std::vector<std::vector<E> > _grid_data{};

    public:
        explicit Grid(const std::vector<std::vector<S> >& input_map) {
            _height = input_map.size();
            _width = input_map[0].size();

            int x = 0;
            std::ranges::for_each(input_map, [this, &x](const std::vector<S>& input_row) -> void {
                std::vector<E> row{};
                for (std::size_t y = 0; y < input_row.size(); ++y) {
                    row.emplace_back(input_row[y], Coordinates(x, y));
                }
                _grid_data.emplace_back(std::move(row));
                ++x;
            });
        }


        [[nodiscard]] auto width() const -> std::size_t {
            return _width;
        }


        [[nodiscard]] auto height() const -> std::size_t {
            return _height;
        }


        // referenced x,y = 0 at top left, row major
        [[nodiscard]] auto operator[](const std::size_t x, const std::size_t y) -> E& {
            if (x < _height && y < _width) {
                return _grid_data[x][y];
            }
            throw std::out_of_range("Invalid map coordinates");
        }


        [[nodiscard]] auto operator[](const std::size_t x, const std::size_t y) const -> const E& {
            if (x < _height && y < _width) {
                return _grid_data[x][y];
            }
            throw std::out_of_range("Invalid map coordinates");
        }


        [[nodiscard]] auto operator[](const Coordinates& c) -> E& {
            return operator[](c.x, c.y);
        }


        [[nodiscard]] auto operator[](const Coordinates& c) const -> const E& {
            return operator[](c.x, c.y);
        }


        [[nodiscard]] auto contains(const Coordinates& coords) const -> bool {
            return coords.x < _height
                   && coords.x >= 0
                   && coords.y < _width
                   && coords.y >= 0;
        }


        [[nodiscard]] auto find_all_if(std::function<bool(E&)> criteria) -> std::vector<std::reference_wrapper<E> > {
            return _grid_data
                   | std::views::transform([&criteria](std::vector<E>& vec) {
                       return vec
                              | std::views::filter(criteria)
                              | std::views::transform([](E& e) {
                                  return std::ref<E>(e);
                              });
                   })
                   | std::views::join | std::ranges::to<std::vector>();
        }
    };
}
