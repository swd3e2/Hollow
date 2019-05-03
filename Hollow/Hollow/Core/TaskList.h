#pragma once

#include <mutex>
#include "Task.h"

template<class T, int capacity = 1024>
class TaskList
{
public:
	bool push(T& task)
	{
		bool result = false;
		taskMutex.lock();
		size_t next = (head + 1) % capacity;
		if (next != tail)
		{
			data[head] = task;
			head = next;
			result = true;
		}
		taskMutex.unlock();
		return result;
	}

	bool pop(T& task)
	{
		bool result = false;
		taskMutex.lock();
		if (tail != head)
		{
			task = data[tail];
			tail = (tail + 1) % capacity;;
			result = true;
		}
		taskMutex.unlock();
		return result;
	}

	bool empty()
	{
		return tail == head;
	}
public:
	T data[capacity];
	std::mutex taskMutex;
	size_t tail = 0;
	size_t head = 0;
};