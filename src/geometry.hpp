#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>

struct Point2D
{
    float values[2] {};

    Point2D() {}
    Point2D(float x, float y) : values { x, y } {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    Point2D& operator+=(const Point2D& other)
    {
        x() += other.x();
        y() += other.y();
        return *this;
    }

    Point2D& operator*=(const Point2D& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    Point2D& operator*=(const float scalar)
    {
        x() *= scalar;
        y() *= scalar;
        return *this;
    }

    Point2D operator+(const Point2D& other) const
    {
        Point2D result = *this;
        result += other;
        return result;
    }

    Point2D operator*(const Point2D& other) const
    {
        Point2D result = *this;
        result *= other;
        return result;
    }

    Point2D operator*(const float scalar) const
    {
        Point2D result = *this;
        result *= scalar;
        return result;
    }
};

struct Point3D
{
    std::array<float, 3> values;

    Point3D() {}
    Point3D(float x, float y, float z) : values { x, y, z } {}

    float& x() { return values.at(0); }
    float x() const { return values.at(0); }

    float& y() { return values.at(1); }
    float y() const { return values.at(1); }

    float& z() { return values.at(2); }
    float z() const { return values.at(2); }

    Point3D& operator+=(const Point3D& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](float coord, float other_coord) { return coord + other_coord; });
        return *this;
    }

    Point3D& operator-=(const Point3D& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](float coord, float other_coord) { return coord - other_coord; });
        return *this;
    }

    Point3D& operator*=(const float scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](float coord) { return coord * scalar; });
        return *this;
    }

    Point3D operator+(const Point3D& other) const
    {
        Point3D result = *this;
        result += other;
        return result;
    }

    Point3D operator-(const Point3D& other) const
    {
        Point3D result = *this;
        result -= other;
        return result;
    }

    Point3D operator*(const float scalar) const
    {
        Point3D result = *this;
        result *= scalar;
        return result;
    }

    Point3D operator-() const { return Point3D { -x(), -y(), -z() }; }

    float length() const
    {
        return std::sqrt(std::accumulate(values.begin(), values.end(), 0.f,
                                         [](float acc, float coord) { return acc + coord * coord; }));
    }

    float distance_to(const Point3D& other) const { return (*this - other).length(); }

    Point3D& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point3D& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
};

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}

template <int dimension, typename type> struct Point
{

    Point() = default;

    std::array<type, dimension> values = {};

    Point<dimension, type>& operator+=(const Point<dimension, type>& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](type coord, type other_coord) { return coord + other_coord; });
        return *this;
    }

    Point<dimension, type>& operator-=(const Point<dimension, type>& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(),
                       [](type coord, type other_coord) { return coord - other_coord; });
        return *this;
    }

    Point<dimension, type>& operator*=(const type scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](type coord) { return coord * scalar; });
        return *this;
    }

    Point<dimension, type> operator+(const Point<dimension, type>& other) const
    {
        Point<dimension, type> result = *this;
        result += other;
        return result;
    }

    Point<dimension, type> operator-(const Point<dimension, type>& other) const
    {
        Point<dimension, type> result = *this;
        result -= other;
        return result;
    }

    Point<dimension, type> operator*(const type scalar) const
    {
        Point<dimension, type> result = *this;
        result *= scalar;
        return result;
    }

    // Point<dimension, type> operator-() const { return Point<dimension, type> { -x(), -y(), -z() }; }

    float length() const
    {
        return std::sqrt(std::accumulate(values.begin(), values.end(), 0.f,
                                         [](float acc, type coord) { return acc + coord * coord; }));
    }

    float distance_to(const Point<dimension, type>& other) const { return (*this - other).length(); }

    Point<dimension, type>& normalize(const type target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point<dimension, type>& cap_length(const type max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
};