#pragma once

#include "../../Utils/Macros.h"

/// \file Vector2D.h

/// \struct Vector2D
/// \brief Represents 2D vectors in game space
struct Vector2D
{
    /// \brief X value of vector
    float m_x;

    /// \brief Y value of vector
    float m_y;

    /// \brief Size of vector for normalization purposes
    float m_size;

    PHAZE_API Vector2D operator+(const Vector2D& v1);
    PHAZE_API Vector2D operator-(const Vector2D& v1);
    PHAZE_API Vector2D operator*(const Vector2D& v1);
    PHAZE_API Vector2D operator*(float n);
    PHAZE_API Vector2D operator/(float n);
    PHAZE_API Vector2D operator/(const Vector2D& v1);
    PHAZE_API void operator=(const Vector2D& v1);
    PHAZE_API void operator+=(const Vector2D& v1);
    PHAZE_API void operator-=(const Vector2D& v1);
    PHAZE_API void operator*=(const Vector2D& v1);
    PHAZE_API void operator/=(const Vector2D& v1);
    PHAZE_API bool operator!=(const Vector2D& v1) { return (this->m_x != v1.m_x || this->m_y != v1.m_y); }
    PHAZE_API bool operator==(const Vector2D& v1) { return (this->m_x == v1.m_x && this->m_y == v1.m_y); }
};