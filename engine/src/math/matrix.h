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

#include "engine/src/math/vector3.h"

#include <cmath>

namespace Engine::Math
{
    /// A 4×4 transformation matrix, indexed row first (m_row_col), so m41/m42/m43 hold the
    /// translation.
    ///
    /// The member order is m11, m12, m13, m14, m21, … — that puts the translation at indices
    /// 12, 13 and 14, which is exactly where a column-major OpenGL `mat4` keeps it. The float
    /// copy Matrixf can therefore go to the driver as-is, with no transposing step in between:
    /// `glUniformMatrix4fv(location, 1, GL_FALSE, (float *)&matrix)`.
    ///
    /// `a * b` reads right to left, like the mathematical notation: b happens first, then a.
    /// A world transform is therefore `translation * rotation * scale`.
    ///
    /// World space is right-handed with +Z up.
    struct Matrix
    {
        double m11 = 1.0, m12 = 0.0, m13 = 0.0, m14 = 0.0; ///< Row 1.
        double m21 = 0.0, m22 = 1.0, m23 = 0.0, m24 = 0.0; ///< Row 2.
        double m31 = 0.0, m32 = 0.0, m33 = 1.0, m34 = 0.0; ///< Row 3.
        double m41 = 0.0, m42 = 0.0, m43 = 0.0, m44 = 1.0; ///< Row 4; m41/m42/m43 is the translation.

        /// Returns the identity matrix. The default-constructed Matrix already is one — this
        /// only spells it out where the intent matters.
        static Matrix identity()
        {
            return {};
        }

        /// Creates a matrix that moves by `position`.
        static Matrix translation(const Vector3 &position)
        {
            Matrix m = {
                .m11 =        1.0, .m12 =        0.0, .m13 =        0.0, .m14 = 0.0,
                .m21 =        0.0, .m22 =        1.0, .m23 =        0.0, .m24 = 0.0,
                .m31 =        0.0, .m32 =        0.0, .m33 =        1.0, .m34 = 0.0,
                .m41 = position.x, .m42 = position.y, .m43 = position.z, .m44 = 1.0
            };
            return m;
        }

        /// Creates a matrix that scales each axis by the matching component of `scale`.
        static Matrix scale(const Vector3 &scale)
        {
            Matrix m = {
                .m11 = scale.x, .m12 =     0.0, .m13 =     0.0, .m14 = 0.0,
                .m21 =     0.0, .m22 = scale.y, .m23 =     0.0, .m24 = 0.0,
                .m31 =     0.0, .m32 =     0.0, .m33 = scale.z, .m34 = 0.0,
                .m41 =     0.0, .m42 =     0.0, .m43 =     0.0, .m44 = 1.0
            };
            return m;
        }

        /// Creates a perspective projection matrix.
        ///
        /// Depth comes out in [-1, 1], the near plane at -1 and the far plane at 1, which is the
        /// range OpenGL clips to, so the whole depth buffer is spent between the two planes.
        /// Apart from the portrait adjustment below, this is the matrix of the classic
        /// `gluPerspective`.
        ///
        /// @param fov     Field of view across the shorter side of the viewport, in radians:
        ///                vertical in a landscape window, horizontal in a portrait one.
        /// @param aspect  Viewport width divided by height.
        /// @param zNear   Distance to the near clip plane.
        /// @param zFar    Distance to the far clip plane.
        static Matrix perspective(double fov, double aspect, double zNear, double zFar)
        {
            double yScale = 1.0 / std::tan(fov * 0.5);
            double xScale = yScale / aspect;

            if (aspect < 1.0)
            {
                // A window taller than it is wide spans fov across its width instead, so the
                // view grows taller rather than narrower.
                xScale = yScale;
                yScale *= aspect;
            }

            Matrix m = {
                .m11 = xScale, .m12 =    0.0, .m13 =                                 0.0, .m14 =  0.0,
                .m21 =    0.0, .m22 = yScale, .m23 =                                 0.0, .m24 =  0.0,
                .m31 =    0.0, .m32 =    0.0, .m33 =     (zFar + zNear) / (zNear - zFar), .m34 = -1.0,
                .m41 =    0.0, .m42 =    0.0, .m43 = 2.0 * zFar * zNear / (zNear - zFar), .m44 =  0.0
            };
            return m;
        }

        /// Creates a view matrix for a camera at `eye` looking towards `target`.
        ///
        /// There is no `up` parameter: pinning it to +Z here is what makes the whole engine
        /// z-up.
        ///
        /// View space is unaffected by the world convention — the camera still looks down its
        /// own negative z, which is exactly what `perspective` expects.
        ///
        /// @param eye     Camera position in world space.
        /// @param target  The point the camera looks at.
        static Matrix lookAt(const Vector3 &eye, const Vector3 &target)
        {
            // The camera looks down its own negative z, so the z axis points back towards the eye.
            Vector3 zAxis = Vector3::normalize(eye - target);

            // A camera sitting exactly on its target has no view direction to build a basis
            // from. Vector3::normalize returns a zero vector rather than a NaN, so without this
            // the matrix below would come out rank-deficient and the screen would just go black.
            if (zAxis.length() == 0.0) return Matrix::identity();

            // Looking straight up or straight down makes the cross product below degenerate:
            // the reference direction and the z axis are then parallel, so their cross product
            // has zero length and the basis collapses the same silent way. Any reference that
            // is not parallel to the z axis works; +X is chosen so a top-down view comes out
            // map-like, with forward up the screen and right to the right of it.
            bool alongUp = std::abs(Vector3::dot(zAxis, Vector3::up)) > 0.9999;
            Vector3 reference = alongUp ? Vector3::forward : Vector3::up;

            Vector3 xAxis = Vector3::normalize(Vector3::cross(reference, zAxis));
            Vector3 yAxis = Vector3::cross(zAxis, xAxis);

            // A matrix that placed the camera would carry its axes as rows; written as columns
            // they give the transpose, which for a rotation is the inverse. The bottom row moves
            // the eye to the origin, measured along those same axes.
            Matrix m = {
                .m11 = xAxis.x,                    .m12 = yAxis.x,                    .m13 = zAxis.x,                    .m14 = 0.0,
                .m21 = xAxis.y,                    .m22 = yAxis.y,                    .m23 = zAxis.y,                    .m24 = 0.0,
                .m31 = xAxis.z,                    .m32 = yAxis.z,                    .m33 = zAxis.z,                    .m34 = 0.0,
                .m41 = -Vector3::dot(xAxis, eye),  .m42 = -Vector3::dot(yAxis, eye),  .m43 = -Vector3::dot(zAxis, eye),  .m44 = 1.0
            };
            return m;
        }

        /// Combines two transformations. `b` is applied first, then `a`.
        friend Matrix operator*(const Matrix &a, const Matrix &b)
        {
            Matrix m = {
                .m11 = a.m11 * b.m11 + a.m21 * b.m12 + a.m31 * b.m13 + a.m41 * b.m14,
                .m12 = a.m12 * b.m11 + a.m22 * b.m12 + a.m32 * b.m13 + a.m42 * b.m14,
                .m13 = a.m13 * b.m11 + a.m23 * b.m12 + a.m33 * b.m13 + a.m43 * b.m14,
                .m14 = a.m14 * b.m11 + a.m24 * b.m12 + a.m34 * b.m13 + a.m44 * b.m14,

                .m21 = a.m11 * b.m21 + a.m21 * b.m22 + a.m31 * b.m23 + a.m41 * b.m24,
                .m22 = a.m12 * b.m21 + a.m22 * b.m22 + a.m32 * b.m23 + a.m42 * b.m24,
                .m23 = a.m13 * b.m21 + a.m23 * b.m22 + a.m33 * b.m23 + a.m43 * b.m24,
                .m24 = a.m14 * b.m21 + a.m24 * b.m22 + a.m34 * b.m23 + a.m44 * b.m24,

                .m31 = a.m11 * b.m31 + a.m21 * b.m32 + a.m31 * b.m33 + a.m41 * b.m34,
                .m32 = a.m12 * b.m31 + a.m22 * b.m32 + a.m32 * b.m33 + a.m42 * b.m34,
                .m33 = a.m13 * b.m31 + a.m23 * b.m32 + a.m33 * b.m33 + a.m43 * b.m34,
                .m34 = a.m14 * b.m31 + a.m24 * b.m32 + a.m34 * b.m33 + a.m44 * b.m34,

                .m41 = a.m11 * b.m41 + a.m21 * b.m42 + a.m31 * b.m43 + a.m41 * b.m44,
                .m42 = a.m12 * b.m41 + a.m22 * b.m42 + a.m32 * b.m43 + a.m42 * b.m44,
                .m43 = a.m13 * b.m41 + a.m23 * b.m42 + a.m33 * b.m43 + a.m43 * b.m44,
                .m44 = a.m14 * b.m41 + a.m24 * b.m42 + a.m34 * b.m43 + a.m44 * b.m44
            };
            return m;
        }
    };

    /// Single-precision copy of a Matrix, used only for the upload to the GPU. The engine itself
    /// never computes with it — OpenGL takes floats, everything before that stays double.
    struct Matrixf
    {
        /// Copies `m`, narrowing every element to float.
        Matrixf(const Matrix &m)
        {
            m11 = (float)m.m11;  m12 = (float)m.m12;  m13 = (float)m.m13;  m14 = (float)m.m14;
            m21 = (float)m.m21;  m22 = (float)m.m22;  m23 = (float)m.m23;  m24 = (float)m.m24;
            m31 = (float)m.m31;  m32 = (float)m.m32;  m33 = (float)m.m33;  m34 = (float)m.m34;
            m41 = (float)m.m41;  m42 = (float)m.m42;  m43 = (float)m.m43;  m44 = (float)m.m44;
        }

        float m11, m12, m13, m14;
        float m21, m22, m23, m24;
        float m31, m32, m33, m34;
        float m41, m42, m43, m44;
    };
}