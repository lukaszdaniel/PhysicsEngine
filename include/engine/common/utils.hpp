#pragma once
#include <engine/common/index_vector.hpp>
#include <sstream>

template <typename U, typename T>
U to(const T &v)
{
    return static_cast<U>(v);
}

template <typename T>
using CIVector = civ::Vector<T>;

template <typename T>
std::string toString(T value)
{
    std::stringstream sx;
    sx << value;
    return sx.str();
}

template <typename T>
sf::Vector2f toVector2f(sf::Vector2<T> v)
{
    return {to<float>(v.x), to<float>(v.y)};
}
