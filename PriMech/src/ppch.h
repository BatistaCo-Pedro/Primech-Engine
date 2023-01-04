#pragma once

// precompiled Header file

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Primech/Core/Logging/Log.h"
#include "Primech/Debug/Instrumentor.h"

//only include std windows.h if the platform is windows
#ifdef PM_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
