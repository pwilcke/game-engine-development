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

#include "engine/src/core/commandbuffer.h"
#include "engine/src/math/matrix.h"
#include "engine/src/math/quaternion.h"
#include "engine/src/math/vector3.h"
#include "engine/src/sim/script.h"
#include "engine/src/video/renderer.h"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using Engine::Core::CommandBuffer;
using Engine::Math::Matrix;
using Engine::Math::Quaternion;
using Engine::Math::Vector3;
using Engine::Video::Renderer;

namespace Engine::Sim
{
    class Entity
    {
      public:
        Entity() = default;
        Entity(const Vector3 &position, const Quaternion &rotation = {})
            : position(position), rotation(rotation)
        {
        }

        bool load(const std::string &filename, Renderer &renderer);
        void unload(Renderer &renderer);

        void update(double dt, double t);

        void commit(CommandBuffer &commandBuffer) const;

        Vector3 getPosition() { return position; }
        void setPosition(const Vector3 &p) { position = p; }

        Quaternion getRotation() { return rotation; }
        void setRotation(const Quaternion &q) { rotation = q; }

        Vector3 getScale() { return scale; }
        void setScale(const Vector3 &s) { scale = s; }

        size_t getEntityId() const { return entityId; }
        void setEntityId(size_t id) { entityId = id; }

        void addScript(std::unique_ptr<Script> script)
        {
            script->load(*this);
            scripts.push_back(std::move(script));
        }

      private:
        struct ModelInstance
        {
            size_t modelId = 0;
            Matrix transform;
        };

        size_t entityId = 0;
        Vector3 position = {};
        Quaternion rotation = {};
        Vector3 scale = {1.0, 1.0, 1.0};
        std::vector<ModelInstance> models = {};
        std::vector<std::unique_ptr<Script>> scripts = {};
    };
}
