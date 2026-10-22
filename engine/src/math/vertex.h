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

#include <array>

namespace Engine::Math
{
    /// Interleaved vertex attributes, laid out exactly as uploaded to the GPU.
    /// Float, not double: this struct is the vertex buffer's memory image.
    ///
    /// Tangent and bitangent are not part of the mesh file: the loader derives them from each
    /// triangle's positions and texture coordinates, and leaves them zero where the texture
    /// coordinates enclose no area. Together with the normal they span the tangent space a
    /// normal map is stored in.
    struct Vertex
    {
        std::array<float, 3> position = {};  ///< Position in the model's local space.
        std::array<float, 2> texCoord = {};  ///< Texture coordinate (u, v).
        std::array<float, 3> normal = {};    ///< Surface normal in the model's local space.
        std::array<float, 3> tangent = {};   ///< Direction in which u grows across the surface, in the model's local space.
        std::array<float, 3> bitangent = {}; ///< Direction in which v grows across the surface, in the model's local space.
    };
}
