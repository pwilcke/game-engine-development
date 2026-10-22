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

#include "app/src/shipscript.h"

#include "engine/src/math/quaternion.h"
#include "engine/src/sim/entity.h"

using Engine::Math::Quaternion;

namespace App
{
    void ShipScript::update(Entity &entity, double dt, double t)
    {
        Quaternion rotation = entity.getRotation();
        rotation = rotation * Quaternion(0.25 * dt, 0.0, 0.0);
        entity.setRotation(rotation);
    }
}
