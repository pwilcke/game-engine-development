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

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

namespace Engine::Core
{
    class ThreadPool
    {
      public:
        ThreadPool() = delete;

        static void create()
        {
            unsigned int threadCount = std::thread::hardware_concurrency();

            for (unsigned int i = 0; i < threadCount; i++)
            {
                threads.push_back(std::thread(process));
            }
        }

        static void terminate()
        {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                active = false;
            }
            queueCondition.notify_all();

            for (std::thread &thread : threads)
            {
                thread.join();
            }

            threads.clear();
        }

        static void run(const std::function<void()> &job)
        {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                jobs.push(job);
            }
            queueCondition.notify_one();
        }

        static void runOnRenderThread(const std::function<void()> &job)
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            jobsForRenderThread.push_back(job);
        }

        static void runRenderThreadTasks()
        {
            std::vector<std::function<void()>> tasks = {};

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                std::swap(tasks, jobsForRenderThread);
            }

            for (const std::function<void()> &task : tasks)
            {
                task();
            }
        }

      private:
        static void process()
        {
            while (true)
            {
                std::function<void()> job = {};

                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    queueCondition.wait(lock, []()
                    {
                        return !jobs.empty() || !active;
                    });
                    if (!active) return;

                    job = jobs.front();
                    jobs.pop();
                }

                job();
            }
        }

        static inline bool active = true;
        static inline std::mutex queueMutex = {};
        static inline std::condition_variable queueCondition = {};
        static inline std::vector<std::thread> threads = {};
        static inline std::queue<std::function<void()>> jobs = {};
        static inline std::vector<std::function<void()>> jobsForRenderThread = {};
    };
}
