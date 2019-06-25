#pragma once

#ifndef HW_DELAYED_TASK_MANAGER_H
#define HW_DELAYED_TASK_MANAGER_H

#include "CModule.h"
#include <functional>
#include <queue>

namespace Hollow {
	class DelayedTaskManager : public CModule<DelayedTaskManager>
	{
	private:
		std::queue<std::function<void()>> taskList;
	public:
		DelayedTaskManager() { setStartedUp(); }
		~DelayedTaskManager() { setShutdown(); }

		void Add(std::function<void()> task) 
		{
			taskList.push(task);
		}

		void Update()
		{
			while (taskList.size() > 0) {
				taskList.front()();
				taskList.pop();
			}
		}
	};
}
#endif