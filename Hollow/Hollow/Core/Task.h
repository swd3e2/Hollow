#pragma once

#include <functional>

class Task
{
public:
	std::function<void()> function;
	int id;
public:
	Task(std::function<void()>& func) :
		function(func), id(0)
	{}
};