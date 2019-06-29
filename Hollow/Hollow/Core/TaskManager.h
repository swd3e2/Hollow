#pragma once

#ifndef HW_TASK_MANAGER_H
#define HW_TASK_MANAGER_H

#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <atomic>
#include "TaskList.h"
#include "CModule.h"

namespace Hollow {
	class TaskManager : public CModule<TaskManager>
	{
	public:
		std::thread** workers;
		const int numWorkers = std::thread::hardware_concurrency() - 1;
		TaskList<std::function<void()>, 256> queue;

		std::mutex wakeMutex;
		std::condition_variable wakeCondition;
		int numTasks;
		int currentLabel;
		std::atomic_int finishedLabel;
	public:
		TaskManager() : numTasks(0), currentLabel(0) {}

		virtual void onStartUp() override
		{
			finishedLabel.store(0);
			workers = new std::thread * [numWorkers];

			for (int i = 0; i < numWorkers; i++)
			{
				std::thread worker([&]()
					{
						std::function<void()> task;
						while (true)
						{
							if (queue.pop(task))
							{
								task();
								finishedLabel.fetch_add(1);
							}
							else
							{
								std::unique_lock<std::mutex> lock(wakeMutex);
								wakeCondition.wait(lock);
							}
						}
					});
				worker.detach();
				workers[i] = &worker;
			}
		}

		void add(std::function<void()> job)
		{
			currentLabel += 1;
			queue.push(job);
			wakeCondition.notify_one();
		}

		inline void poll()
		{
			wakeCondition.notify_one();
			std::this_thread::yield();
		}

		void wait()
		{
			while (currentLabel != finishedLabel.load())
			{
				poll();
			}
		}
	};
}

#endif