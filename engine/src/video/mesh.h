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

#include "engine/src/math/vertex.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

using Engine::Math::Vertex;

namespace Engine::Video
{
    struct MeshData
    {
        std::string filename;

        std::vector<std::array<float, 3>> positions;
        std::vector<std::array<float, 2>> texCoords;
        std::vector<std::array<float, 3>> normals;

        std::vector<Vertex> vertices;
    };

    class Mesh
    {
      public:
        static std::shared_ptr<Mesh> create(const std::string &filename);
        ~Mesh();

        Mesh(const Mesh &) = delete;
        Mesh &operator=(const Mesh &) = delete;

        static bool readFile(MeshData &data);
        static void createMesh(Mesh &mesh, MeshData &data);

        void draw() const;
        uint32_t getId() const { return vertexArrayId; };
        size_t getVertexCount() const { return vertexCount; };

      private:
        Mesh() = default;
        uint32_t vertexArrayId = 0;
        uint32_t vertexBufferId = 0;
        size_t vertexCount = 0;
    };
}
