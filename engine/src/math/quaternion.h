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

#include "engine/src/math/matrix.h"
#include "engine/src/math/vector3.h"

#include <cmath>

namespace Engine::Math
{
    /// A rotation. Four numbers instead of the nine of a rotation matrix, and unlike Euler angles
    /// it has no gimbal lock — which is why the engine stores orientations this way and converts
    /// to a Matrix only when one is needed.
    struct Quaternion
    {
        /// The identity rotation. The members carry their defaults, so even a plain
        /// `Quaternion q;` leaves an entity unrotated instead of pointing it somewhere random.
        Quaternion() = default;

        /// Creates a rotation from Euler angles in radians.
        ///
        /// Each angle turns about the world axis it is named after: roll about +X (forward),
        /// pitch about +Y (left), yaw about +Z (up). They are combined in XYZ order — roll
        /// first, then pitch, then yaw — which is Blender's default, so the three numbers of a
        /// Blender XYZ Euler go into the matching parameters here.
        ///
        /// Mind the argument order, which is not the order the axes are listed in above: all
        /// three are plain doubles, so getting them out of order still compiles.
        ///
        /// @param yaw    Rotation about +Z, in radians.
        /// @param pitch  Rotation about +Y, in radians.
        /// @param roll   Rotation about +X, in radians.
        Quaternion(double yaw, double pitch, double roll);

        /// Creates a rotation of `angle` radians around `axis`. The axis is normalized here, so
        /// it does not have to arrive as a unit vector.
        Quaternion(Vector3 axis, double angle)
        {
            Vector3 normalized = Vector3::normalize(axis);
            double sinHalf = std::sin(angle * 0.5);

            x = normalized.x * sinHalf;
            y = normalized.y * sinHalf;
            z = normalized.z * sinHalf;
            w = std::cos(angle * 0.5);
        }

        /// How long the four numbers are. A rotation is a *unit* quaternion, so the answer should
        /// be 1 — and the only reason it is worth asking is that it does not stay 1 on its own.
        double length() const
        {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        /// Returns `q` scaled back to unit length.
        ///
        /// Needed the moment rotations start being accumulated by multiplying, which is how a
        /// script turns something a little further every frame. Every product rounds, sixty times
        /// a second, forever — and a quaternion that is no longer of length 1 is no longer a pure
        /// rotation: toMatrix() then scales the model as well as turning it. One divide per step
        /// is cheap next to finding out an hour later why the model is slowly shrinking.
        ///
        /// A quaternion of no length at all comes back as the identity and not as four zeros.
        /// Vector3::normalize() answers the same question with the zero vector, which is right
        /// there and wrong here: a zero quaternion is not a rotation, and anything multiplied
        /// onto it is zero again, so the entity could never be turned again.
        static Quaternion normalize(const Quaternion &q)
        {
            double length = q.length();
            if (length == 0.0) return {};

            double scale = 1.0 / length;

            Quaternion result;
            result.x = q.x * scale;
            result.y = q.y * scale;
            result.z = q.z * scale;
            result.w = q.w * scale;
            return result;
        }

        /// Converts the rotation into the equivalent 4×4 matrix. The formula assumes unit
        /// length — see normalize().
        Matrix toMatrix() const
        {
            Matrix m = {
                .m11 = 1.0 - 2.0 * (y * y + z * z), .m12 =       2.0 * (x * y + z * w), .m13 =       2.0 * (x * z - y * w), .m14 = 0.0,
                .m21 =       2.0 * (x * y - z * w), .m22 = 1.0 - 2.0 * (x * x + z * z), .m23 =       2.0 * (y * z + x * w), .m24 = 0.0,
                .m31 =       2.0 * (x * z + y * w), .m32 =       2.0 * (y * z - x * w), .m33 = 1.0 - 2.0 * (x * x + y * y), .m34 = 0.0,
                .m41 =                         0.0, .m42 =                         0.0, .m43 =                         0.0, .m44 = 1.0
            };
            return m;
        }

        /// The rotation axis scaled by sin(angle / 2) in x, y and z, and cos(angle / 2) in w —
        /// exactly what the axis-angle constructor above computes.
        double x = 0.0, y = 0.0, z = 0.0, w = 1.0;
    };

    /// Combines two rotations. `right` is applied first, then `left` — the same reading order
    /// as the matrix product.
    inline Quaternion operator*(Quaternion left, Quaternion right)
    {
        Quaternion result;
        result.w = left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z;
        result.x = left.w * right.x + left.x * right.w + left.y * right.z - left.z * right.y;
        result.y = left.w * right.y - left.x * right.z + left.y * right.w + left.z * right.x;
        result.z = left.w * right.z + left.x * right.y - left.y * right.x + left.z * right.w;
        return result;
    }

    /// Rotates `vector`. Short form of q · v · q⁻¹, without building the matrix. Like
    /// toMatrix(), it assumes unit length.
    inline Vector3 operator*(Quaternion rotation, Vector3 vector)
    {
        Vector3 axis = {rotation.x, rotation.y, rotation.z};
        Vector3 t = Vector3::cross(axis, vector) * 2.0;
        return vector + t * rotation.w + Vector3::cross(axis, t);
    }

    /// Lets a rotation stand in a matrix product directly: `rotation * matrix` is
    /// `rotation.toMatrix() * matrix`.
    inline Matrix operator*(Quaternion rotation, Matrix matrix)
    {
        return rotation.toMatrix() * matrix;
    }

    /// The same with the rotation on the right: `matrix * rotation` is
    /// `matrix * rotation.toMatrix()`.
    inline Matrix operator*(Matrix matrix, Quaternion rotation)
    {
        return matrix * rotation.toMatrix();
    }

    /// The opposite rotation. For a unit quaternion the conjugate is the inverse.
    inline Quaternion operator-(Quaternion q)
    {
        Quaternion result;
        result.x = -q.x;
        result.y = -q.y;
        result.z = -q.z;
        result.w = q.w;
        return result;
    }

    /// Defined down here, after the struct, because it is built from operator*, which takes whole
    /// Quaternions and so can only be declared once the struct is complete.
    inline Quaternion::Quaternion(double yaw, double pitch, double roll)
    {
        // One rotation per axis, combined right to left like the matrix product: roll about +X
        // (forward) happens first, then pitch about +Y (left), then yaw about +Z (up). Written
        // out this way the axes are readable; the closed form is six interleaved cos/sin terms
        // that give no hint which angle drives which axis.
        *this = Quaternion(Vector3::up, yaw) * Quaternion(Vector3::left, pitch) * Quaternion(Vector3::forward, roll);
    }
}