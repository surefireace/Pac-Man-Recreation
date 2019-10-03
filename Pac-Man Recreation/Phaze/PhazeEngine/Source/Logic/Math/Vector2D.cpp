#include "Vector2D.h"

Vector2D Vector2D::operator+(const Vector2D& v1)
{
    Vector2D v2;
    v2.m_x = this->m_x + v1.m_x;
    v2.m_y = this->m_y + v1.m_y;
    return v2;
}

Vector2D Vector2D::operator-(const Vector2D& v1)
{
    Vector2D v2;
    v2.m_x = this->m_x - v1.m_x;
    v2.m_y = this->m_y - v1.m_y;
    return v2;
}

Vector2D Vector2D::operator*(const Vector2D& v1)
{
    Vector2D v2;
    v2.m_x = this->m_x * v1.m_x;
    v2.m_y = this->m_y * v1.m_y;
    return v2;
}

Vector2D Vector2D::operator*(float n)
{
    Vector2D v2;
    v2.m_x = this->m_x * n;
    v2.m_y = this->m_y * n;
    return v2;
}

Vector2D Vector2D::operator/(float n)
{
    Vector2D v2;
    v2.m_x = this->m_x / n;
    v2.m_y = this->m_y / n;
    return v2;
}

Vector2D Vector2D::operator/(const Vector2D& v1)
{
    Vector2D v2;
    v2.m_x = this->m_x / v1.m_x;
    v2.m_y = this->m_y / v1.m_y;
    return v2;
}

void Vector2D::operator=(const Vector2D& v1)
{
    this->m_x = v1.m_x;
    this->m_y = v1.m_y;
}

void Vector2D::operator+=(const Vector2D& v1)
{
    this->m_x += v1.m_x;
    this->m_y += v1.m_y;
}

void Vector2D::operator-=(const Vector2D& v1)
{
    this->m_x -= v1.m_x;
    this->m_y -= v1.m_y;
}

void Vector2D::operator*=(const Vector2D& v1)
{
    this->m_x *= v1.m_x;
    this->m_y *= v1.m_y;
}

void Vector2D::operator/=(const Vector2D& v1)
{
    this->m_x /= v1.m_x;
    this->m_y /= v1.m_y;
}
