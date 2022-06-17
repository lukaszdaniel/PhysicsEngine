#pragma once
#include <cmath>

namespace Math
{
    constexpr float PI = 3.141592653f;

    template <typename T>
    T sign(T v)
    {
        return v < 0.0f ? -1.0f : 1.0f;
    }

    template <typename T>
    T sigm(T x)
    {
        return 1.0f / (1.0f + exp(-x));
    }

    template <typename T>
    T sigm_0(T x)
    {
        return sigm(x) - 0.5f;
    }

    template <typename T>
    T radToDeg(T r)
    {
        constexpr T radian_to_deg = 180.0f / PI;
        return r * radian_to_deg;
    }
};

namespace MathVec2
{
    template <template <typename> class Vec2Type, typename T>
    T dot(const Vec2Type<T> &v1, const Vec2Type<T> &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template <template <typename> class Vec2Type, typename T>
    T cross(const Vec2Type<T> &v1, const Vec2Type<T> &v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    template <typename Vec2Type>
    Vec2Type normal(const Vec2Type &v)
    {
        return {-v.y, v.x};
    }

    template <typename Vec2Type>
    Vec2Type rotate(const Vec2Type &v, float angle)
    {
        const float ca = std::cos(angle);
        const float sa = std::sin(angle);
        return {ca * v.x - sa * v.y, sa * v.x + ca * v.y};
    }

    template <template <typename> class Vec2Type, typename T>
    T length(const Vec2Type<T> &v)
    {
        return std::sqrt(dot(v, v));
    }

    template <template <typename> class Vec2Type, typename T>
    T angle(const Vec2Type<T> &v_1, const Vec2Type<T> &v_2)
    {
        const T prod = dot(v_1, v_2);
        const T det = cross(v_1, v_2);
        return std::atan2(det, prod);
    }

    template <typename Vec2Type>
    Vec2Type normalize(const Vec2Type &v)
    {
        return v / length(v);
    }
};
