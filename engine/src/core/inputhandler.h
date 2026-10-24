/**
 * Game Engine Development
 * Technische Hochschule Mittelhessen
 * Copyright © 2021-2026 Tobias Reimann
 * Copyright © 2026 Paul Wilcke
 * Copyright © 2026 Berkay Özgür
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
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Engine::Core
{
    class InputHandler
    {
      public:
        InputHandler() = delete;

        static void registerKeyAction(const std::string &action, uint32_t key);
        static void onAction(const std::string &action, std::function<void()> callback);
        static void fireAction(const std::string &action);
        static void keyCallback(uint32_t key, uint32_t scancode, uint32_t mods, bool pressed);
        static void onKeyPress(uint32_t code, std::function<void()> callback);

        // Axis
        static void registerKeyAxis(const std::string &axis, uint32_t negativeKey, uint32_t positiveKey);
        static double getAxis(const std::string &axis);
        static void update();

        // Controller Support
        static void registerButtonAction(const std::string &action, uint32_t button);
        static void registerStickAxis(const std::string &axis, uint32_t input, bool invert = false);
        static void loadGamepadMappings(std::istream &mappings);

      private:
        static inline std::unordered_map<std::string, std::vector<std::pair<uint32_t, uint32_t>>> keyboardAxes = {};

        static inline std::unordered_map<uint32_t, std::vector<std::function<void()>>> keyCallbacks = {};
        static inline std::unordered_map<std::string, std::vector<std::function<void()>>> actionCallbacks = {};
        static inline std::unordered_map<uint32_t, std::vector<std::string>> keyboardActions = {};

        static inline std::array<bool, 349> keyboardState = {};
        static inline std::unordered_map<std::string, double> axes = {};

        // Controller Support
        static inline std::unordered_map<uint32_t, std::vector<std::string>> gamepadActions = {};
        static inline std::unordered_map<std::string, std::vector<std::pair<uint32_t, bool>>> gamepadAxes = {};
        static inline std::array<bool, 15> gamepadState = {};
    };
}
