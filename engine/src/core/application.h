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
#include "engine/src/core/settings.h"
#include "engine/src/core/window.h"
#include "engine/src/sim/simulation.h"
#include "engine/src/video/renderer.h"

#include <ios>
#include <istream>
#include <memory>
#include <optional>
#include <string>

using Engine::Sim::Simulation;
using Engine::Video::Renderer;

namespace Engine::Core
{
    class Application
    {
      public:
        Application(int argc, char *argv[]);
        bool init();
        bool run();

        static std::unique_ptr<std::istream> load(const std::string &filename, std::ios::openmode mode = std::ios::in);

      protected:
        Settings settings;

        static inline std::string assetRoot = {};

        std::unique_ptr<CommandBuffer> frontCommandBuffer;
        std::unique_ptr<CommandBuffer> backCommandBuffer;

        std::optional<Window> window;

        std::optional<Renderer> renderer;
        std::optional<Simulation> simulation;
    };
}
