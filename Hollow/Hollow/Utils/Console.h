#pragma once
#include "Hollow/Platform.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

// maximum mumber of lines the output console should have
#define MAX_CONSOLE_LINES 1000;

namespace Hollow { namespace Console {
	using namespace std;
	HOLLOW_API inline void RedirectIOToConsole();
}}
