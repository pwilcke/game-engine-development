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

#include <cstdint>

namespace Engine::Math
{
    /// A small pseudo-random number generator. The same seed always produces the same sequence,
    /// so a world built from it comes out identical on every run, and a different seed builds a
    /// different one. Predictable by design, so never use it for anything security-related.
    class Random
    {
      public:
        /// Starts the sequence at `seed`.
        Random(uint32_t seed)
            : seed(seed)
        {
        }

        /// Returns a number between `min` and `max`, both included. draw() divided by
        /// 4294967295, the largest value it can return, is a fraction from 0 to 1, which is then
        /// stretched onto the range.
        double nextDouble(double min, double max)
        {
            return min + (max - min) * (draw() / 4294967295.0);
        }

      private:
        /// Returns the next 32-bit number of the sequence.
        ///
        /// `seed` is stepped by a fixed odd constant, which runs it through all 2³² values before
        /// one repeats. The returned number is that value scrambled by two rounds of multiplying
        /// and folding the upper half of the 64-bit product onto the lower half, so that
        /// consecutive draws look unrelated.
        ///
        /// Source: https://www.youtube.com/watch?v=ZZY9YE7rZJw&t=1697s
        uint32_t draw()
        {
            seed += 0xe120fc15;
            uint64_t tmp = (uint64_t)seed * 0x4a39b70d;
            uint32_t m1 = (uint32_t)((tmp >> 32) ^ tmp);
            tmp = (uint64_t)m1 * 0x12fad5c9;
            return (uint32_t)((tmp >> 32) ^ tmp);
        }

        uint32_t seed = 0; ///< Starts as the seed and advances with every draw().
    };
}