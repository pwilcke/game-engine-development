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

#include "engine/src/core/application.h"
#include "engine/src/sim/entity.h"

#include <memory>

using Engine::Core::Application;
using Engine::Sim::Entity;

namespace App
{
    class MyGame : public Application
    {
      public:
        MyGame(int argc, char *argv[]);
        bool init();

      private:
        std::shared_ptr<Entity> ship = {};
    };
}