#pragma once

#include "Engine/ECS/Base.h"

#include <functional>

namespace MyEngine
{
	// Encapsulate a task function
	struct Task {
		std::function<void()> func;
	};
}
