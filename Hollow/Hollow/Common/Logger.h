#pragma once

#ifndef HW_LOGGER_H
#define HW_LOGGER_H

#include "Hollow/Core/CModule.h"
#include <string>
#include <vector>
#include "vendor/spdlog/include/spdlog/fmt/fmt.h"
#include <iostream>

#define LOG_BUFFER_SIZE 500
namespace Hollow {
	class Logger : public CModule<Logger>
	{
	public:
		std::string buffer[500];
		unsigned int counter = 0;
		size_t size = 0;
	public:
		template<typename ...Args>
		void log(const char* string, Args&& ...args)
		{
			fmt::memory_buffer buf;
			fmt::format_to(buf, string, args...); // replaces itoa(42, buffer, 10)
			std::string res = fmt::to_string(buf);
			buffer[counter++ % 500] = res;

			fwrite(res.c_str(), sizeof(char), res.size(), stdout);
			fflush(stdout);
			size++;
			if (size > 500) {
				size = 500;
			}
		}

		void clear()
		{
			counter = 0;
			size = 0;
			for (int i = 0; i < 500; i++) {
				buffer[i].clear();
			}
		}
	};
}

#endif