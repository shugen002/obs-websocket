/*
obs-websocket
Copyright (C) 2016-2021 Stephane Lepin <stephane.lepin@gmail.com>
Copyright (C) 2020-2021 Kyle Manning <tt2468@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#pragma once

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>

namespace Utils {
namespace Compat {
class SimpleThreadPool {
public:
SimpleThreadPool()
{
int numThreads = std::max(2, (int)std::thread::hardware_concurrency());
for (int i = 0; i < numThreads; i++) {
_workers.emplace_back([this] {
while (true) {
std::function<void()> task;
{
std::unique_lock<std::mutex> lock(_mutex);
_condition.wait(lock, [this] {
return _stopping || !_tasks.empty();
});
if (_stopping && _tasks.empty())
return;
task = std::move(_tasks.front());
_tasks.pop();
_activeCount++;
}
task();
{
std::unique_lock<std::mutex> lock(_mutex);
_activeCount--;
if (_tasks.empty() && _activeCount == 0)
_waitCondition.notify_all();
}
}
});
}
}

~SimpleThreadPool()
{
{
std::unique_lock<std::mutex> lock(_mutex);
_stopping = true;
}
_condition.notify_all();
for (auto &w : _workers)
w.join();
}

void start(std::function<void()> task)
{
{
std::unique_lock<std::mutex> lock(_mutex);
_tasks.push(std::move(task));
}
_condition.notify_one();
}

void waitForDone()
{
std::unique_lock<std::mutex> lock(_mutex);
_waitCondition.wait(lock, [this] { return _tasks.empty() && _activeCount == 0; });
}

int maxThreadCount() const { return (int)_workers.size(); }

private:
std::vector<std::thread> _workers;
std::queue<std::function<void()>> _tasks;
std::mutex _mutex;
std::condition_variable _condition;
std::condition_variable _waitCondition;
bool _stopping = false;
int _activeCount = 0;
};
}
}
