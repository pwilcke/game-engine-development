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

#include "engine/src/sim/simulation.h"

#include <cmath>
#include <numbers>

namespace Engine::Sim
{
    Simulation::Simulation(std::unique_ptr<CommandBuffer> &commandBuffer, Renderer &renderer)
        : commandBuffer(commandBuffer), renderer(renderer)
    {
    }

    std::shared_ptr<Entity> Simulation::loadEntity(const std::string &filename)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>();

        bool success = entity->load(filename, renderer);
        if (!success) return nullptr;

        size_t entityId = nextEntityId++;
        entity->setEntityId(entityId);
        entities[entityId] = entity;

        return entity;
    }

    bool Simulation::unloadEntity(size_t entityId)
    {
        auto entry = entities.find(entityId);
        if (entry == entities.end()) return false;

        entry->second->unload(renderer);
        entities.erase(entry);
        return true;
    }

    void Simulation::update(double dt, double t)
    {
        elapsedTime += dt;

        float phase = static_cast<float>(elapsedTime) * 2.0f * std::numbers::pi_v<float> / 6.0f;

        commandBuffer->reset();
        commandBuffer->clear.red = 0.29f + 0.20f * std::sin(phase);
        commandBuffer->clear.green = 0.36f + 0.20f * std::sin(phase + 2.0f);
        commandBuffer->clear.blue = 0.40f + 0.20f * std::sin(phase + 4.0f);

        commandBuffer->viewMatrix = camera.getViewMatrix();

        for (auto &[entityId, entity] : entities)
        {
            entity->update(dt, t);
        }

        for (const auto &[entityId, entity] : entities)
        {
            entity->commit(*commandBuffer);
        }
    }
}