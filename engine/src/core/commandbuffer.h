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

#include <cstddef>
#include <vector>

using Engine::Math::Matrix;

namespace Engine::Core
{
    struct ClearCommand
    {
        float red = 0.0f;
        float green = 0.0f;
        float blue = 0.0f;
    };

    struct RenderItem
    {
        size_t modelId = 0;
        Matrix transform;
    };

    class CommandBuffer
    {
      public:
        void reset()
        {
            clear = {};
            renderItems.clear();
        }

        ClearCommand clear;

        Matrix viewMatrix;

        std::vector<RenderItem> renderItems;
    };
}