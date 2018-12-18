#pragma once
#include "Hollow/Platform.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#define MAX_CONSOLE_LINES 500;

namespace Hollow { namespace Console {
	using namespace std;

	// maximum mumber of lines the output console should have

	HOLLOW_API inline void RedirectIOToConsole();
}}
