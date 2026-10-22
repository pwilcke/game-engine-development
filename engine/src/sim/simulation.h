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
#include "engine/src/sim/camera.h"
#include "engine/src/sim/entity.h"
#include "engine/src/video/renderer.h"

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

using Engine::Core::CommandBuffer;
using Engine::Video::Renderer;

namespace Engine::Sim
{
    class Simulation
    {
      public:
        Simulation(std::unique_ptr<CommandBuffer> &commandBuffer, Renderer &renderer);

        std::shared_ptr<Entity> loadEntity(const std::string &filename);
        bool unloadEntity(size_t entityId);

        void update(double dt, double t);

      private:
        std::unique_ptr<CommandBuffer> &commandBuffer;

        Renderer &renderer;

        double elapsedTime = 0.0;

        Camera camera;

        size_t nextEntityId = 1;
        std::unordered_map<size_t, std::shared_ptr<Entity>> entities = {};
    };
}
