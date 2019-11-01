#pragma once

#include <Hollow/Events/Event.h>
#include <string>

class FileChangeEvent : public Hollow::Event<FileChangeEvent>
{
public:
	FileChangeEvent(const std::string& filename)
		: filename(filename)
	{}

	std::string filename;
};