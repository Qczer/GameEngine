#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

#include <string>
#include <format>
#include <print>

#include <array>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "GameEngine/Core/Log.h"

#include "GameEngine/Debug/Instrumentor.h"

#ifdef GE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif