
#pragma once

#include "Base.h"
#include <cmath>

namespace VREN
{
    class VREN_API Vector2
    {
    public:
        float x, y;

        Vector2();
        Vector2(const Vector2 &other);
        Vector2(float x, float y);
        ~Vector2() = default;

        Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }
        Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }

        Vector2 &operator+=(const Vector2 &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }
        Vector2 &operator-=(const Vector2 &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }
        Vector2 &operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        Vector2 &operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Vector2 &other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vector2 &other) const { return !(*this == other); }

        void Set(float X, float Y);
        float Length() const { return std::sqrt(x * x + y * y); }
        float LengthSquared() const { return x * x + y * y; }
        Vector2 Normalize() const
        {
            float len = Length();
            return len != 0 ? *this / len : Vector2(0, 0);
        }
    };

    class VREN_API Vector3
    {
    public:
        float x, y, z;

        Vector3();
        Vector3(const Vector3 &other);
        Vector3(float x, float y, float z);
        ~Vector3() = default;

        Vector3 operator+(const Vector3 &other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        Vector3 operator-(const Vector3 &other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
        Vector3 operator/(float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        Vector3 operator*(const Vector3 &other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        Vector3 &operator+=(const Vector3 &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
        Vector3 &operator-=(const Vector3 &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }
        Vector3 &operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }
        Vector3 &operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        bool operator==(const Vector3 &other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }
        bool operator!=(const Vector3 &other) const { return !(*this == other); }

        void operator*=(const Vector3 &other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
        }

        void Set(float X, float Y, float Z);
        float Length() const { return std::sqrt(x * x + y * y + z * z); }
        float LengthSquared() const { return x * x + y * y + z * z; }
        Vector3 Normalize() const
        {
            float len = Length();
            return len != 0 ? *this / len : Vector3(0, 0, 0);
        }
    };
} // namespace VREN
// namespace VREN
