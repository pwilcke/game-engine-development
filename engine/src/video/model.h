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
#include "engine/src/video/mesh.h"
#include "engine/src/video/shader.h"
#include "engine/src/video/texture.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

using Engine::Math::Matrix;

namespace Engine::Video
{
    class Model
    {
      public:
        static std::shared_ptr<Model> create(const std::string &filename);

        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        void draw(const Matrix &worldMatrix, const Matrix &viewMatrix, const Matrix &projectionMatrix) const;

      private:
        Model() = default;
        static void load(Model &model, const std::string &filename);

        std::vector<std::shared_ptr<Mesh>> meshes = {};
        std::shared_ptr<Shader> shader = {};
        std::map<std::string, std::shared_ptr<Texture>> textures = {};
    };
}
