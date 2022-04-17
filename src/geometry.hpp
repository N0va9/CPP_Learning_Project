#pragma once

#include <algorithm>
#include <numeric>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;


template<int dimension, typename T> struct Point {
    std::array<T, dimension> values {};

    Point() {}
    Point(T x, T y) : values {x, y} {}
    Point(T x, T y, T z) : values {x, y, z} {}

    T x() const {
        static_assert(dimension>=1);
        return values[0];
    }

    T& x() {
        static_assert(dimension>=1);
        return values[0];
    }


    T y() const {
        static_assert(dimension>=2);
        return values[1];
    }

    T& y() {
        static_assert(dimension>=2);
        return values[1];
    }

    T z() const {
        static_assert(dimension>=3);
        return values[2];
    }

    T& z() {
        static_assert(dimension>=3);
        return values[2];
    }

    Point& operator+=(const Point& other)
    {
        transform(values.begin(), values.end(), other.values.begin(), values.begin(), plus<float>());
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        transform(values.begin(), values.end(), other.values.begin(), values.begin(), minus<float>());
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point& operator*=(const float scalar)
    {
        transform(values.begin(), values.end(), values.begin(), [scalar](float value) {return value * scalar; });
        return *this;
    }

    Point& operator*=(const Point& other) {
        transform(values.begin(), values.end(), other.values.begin(), values.begin(), multiplies<float>());
        return *this;
    }


    Point operator*(const float scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    Point operator-() const { return Point { -x(), -y(), -z() }; }

    T length() const { return sqrt(reduce(values.begin(), values.end(), 0.0, [](float acc, float actual) {return (actual * actual) + acc; } ) );}

    T distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const T target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const T max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    /*friend ostream& operator<< (std::ostream& stream,Point& p) {
        if (dimension == 2){
            stream << "( " << p.x() << ", " << p.y() << " ) ";
            return stream;
        }

        if (dimension == 3){
            stream << "( " << p.x() << ", " << p.y() << ", " << p.z() << " ) ";
            return stream;
        }
        else
            stream << "Invalid dimension";
        return stream;
    }*/
};

using Point2D = Point<2, float>;
using Point3D = Point<3, float>;

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}

