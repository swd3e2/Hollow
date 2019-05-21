#pragma once
#include "Hollow/Platform.h"
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

// maximum mumber of lines the output console should have
#define MAX_CONSOLE_LINES 1000;

namespace Hollow { namespace Console {
	using namespace std;
	 void RedirectIOToConsole();
}}
