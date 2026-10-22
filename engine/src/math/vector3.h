/**
 * Game Engine Development
 * Technische Hochschule Mittelhessen
 * Copyright © 2021-2026 Tobias Reimann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <algorithm>
#include <cmath>

namespace Engine::Math
{
    /// A point or a direction in world space, or any other three numbers that belong together,
    /// such as a scale or a color. Double, not float: the engine computes in double precision
    /// everywhere and narrows only at the moment a value is handed to the GPU.
    ///
    /// The world is right-handed and z-up: +X forward, +Y left, +Z up. The three axes are
    /// spelled out as `forward`, `left` and `up` below.
    struct Vector3
    {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        /// The zero vector.
        constexpr Vector3() = default;

        /// Creates a vector from its three components.
        constexpr Vector3(double x, double y, double z)
            : x(x), y(y), z(z)
        {
        }

        /// The world axes. A model is authored facing +X, so `forward` is the axis its front
        /// points along and `left` is +Y. A reader coming from a y-up engine would reach for Z
        /// and X instead; naming the axes keeps that out of the call sites.
        static const Vector3 left;    ///< +Y
        static const Vector3 forward; ///< +X
        static const Vector3 up;      ///< +Z

        /// Adds `b` component by component.
        Vector3 operator+(const Vector3 &b) const
        {
            Vector3 result = {
                x + b.x,
                y + b.y,
                z + b.z
            };
            return result;
        }

        /// Subtracts `b` component by component. `to - from` points from `from` to `to`.
        Vector3 operator-(const Vector3 &b) const
        {
            Vector3 result = {
                x - b.x,
                y - b.y,
                z - b.z
            };
            return result;
        }

        /// Scales every component by `factor`.
        Vector3 operator*(double factor) const
        {
            Vector3 result = {
                x * factor,
                y * factor,
                z * factor
            };
            return result;
        }

        /// Divides every component by `factor`.
        Vector3 operator/(double factor) const
        {
            Vector3 result = {
                x / factor,
                y / factor,
                z / factor
            };
            return result;
        }

        /// The same vector pointing the opposite way.
        Vector3 operator-() const
        {
            Vector3 result = {-x, -y, -z};
            return result;
        }

        /// The distance from the origin: Pythagoras in three dimensions.
        double length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        /// Returns `v` with every component limited to the range [`min`, `max`].
        static Vector3 clamp(const Vector3 &v, double min, double max)
        {
            Vector3 result = {
                std::clamp(v.x, min, max),
                std::clamp(v.y, min, max),
                std::clamp(v.z, min, max)
            };
            return result;
        }

        /// The dot product, |a| · |b| · cos(angle between them). It is 0 for perpendicular
        /// vectors, and for two unit vectors it is the cosine of their angle.
        static double dot(const Vector3 &a, const Vector3 &b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        /// The cross product: a vector perpendicular to both `a` and `b`, with the direction
        /// given by the right-hand rule and the length |a| · |b| · sin(angle between them).
        /// `cross(forward, left)` is `up`.
        static Vector3 cross(const Vector3 &a, const Vector3 &b)
        {
            Vector3 result = {
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x
            };
            return result;
        }

        /// Returns `v` scaled to unit length, or the zero vector when `v` has no length —
        /// a division by zero here would poison every matrix built from the result.
        static Vector3 normalize(const Vector3 &v)
        {
            double length = v.length();
            if (length == 0.0) return {};
            return v * (1.0 / length);
        }
    };

    inline constexpr Vector3 Vector3::left    = {0.0, 1.0, 0.0};
    inline constexpr Vector3 Vector3::forward = {1.0, 0.0, 0.0};
    inline constexpr Vector3 Vector3::up      = {0.0, 0.0, 1.0};

    /// Single-precision copy, used only where a value is uploaded to the GPU.
    struct Vector3f
    {
        /// Copies `v`, narrowing every component to float.
        Vector3f(const Vector3 &v)
            : x((float)v.x), y((float)v.y), z((float)v.z)
        {
        }

        float x, y, z;
    };
}