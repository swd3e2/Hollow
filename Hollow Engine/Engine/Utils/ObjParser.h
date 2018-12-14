#ifndef __OBJ_PARSER__
#define __OBJ_PARSER__
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Engine/Common/Log.h"

namespace Hollow {

	class ObjParser
	{
	private:
		std::vector<float> vertices;
		std::vector<float> texCoords;
		std::vector<float> normals;

		std::string currentObjectName;
	public:
		static void LoadObj(const char * filename)
		{
			std::ifstream filestream(filename);

			if (!filestream)
			{
				Hollow::Log::GetCoreLogger()->error("ObjParser: can't open filestream, filename {}", filename);
			}

			std::string linebuff;
			int lines = 0;
			while (filestream.peek() != -1)
			{
				safeGetline(filestream, linebuff);	
				lines++;

				// Trim newline '\r\n' or '\n'
				if (linebuff.size() > 0) {
					if (linebuff[linebuff.size() - 1] == '\n')
						linebuff.erase(linebuff.size() - 1);
				}
				if (linebuff.size() > 0) {
					if (linebuff[linebuff.size() - 1] == '\r')
						linebuff.erase(linebuff.size() - 1);
				}
				// Skip if empty line.
				if (linebuff.empty()) {
					continue;
				}

				// Skip leading space.
				const char *token = linebuff.c_str();
				token += strspn(token, " \t");

				if (token[0] == '#' || token[0] == '\0') continue;

				if (token[0] == 'v' && token[1] == ' ') {
					token += 2;
					Hollow::Log::GetCoreLogger()->info("{}", token);
					const char *end = token + strcspn(token, " \t\r");

					Hollow::Log::GetCoreLogger()->info("{}", strspn(token, " \t"));
				}

				//Hollow::Log::GetCoreLogger()->info("{}", token);

				//Hollow::Log::GetCoreLogger()->warn("{}", linebuff);
			}
			Hollow::Log::GetCoreLogger()->warn("Total lines {}", lines);
		}

		static std::istream& safeGetline(std::istream& is, std::string& t)
		{
			t.clear();

			std::istream::sentry se(is, true);
			std::streambuf* sb = is.rdbuf();

			if (se) {
				for (;;) {
					int c = sb->sbumpc();
					switch(c) {
						case '\n':
							return is;
						case '\r':
							if (sb->sgetc() == '\n') sb->sbumpc();
          					return is;
        				case EOF:
          					// Also handle the case when the last line has no line ending
          					if (t.empty()) is.setstate(std::ios::eofbit);
          					return is;
        				default:
         			 		t += static_cast<char>(c);
					}
				}
			}

			return is;
		}
	};

}

#endif