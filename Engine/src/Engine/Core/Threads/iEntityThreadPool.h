#pragma once

#include "Engine/Core/Threads/ThreadsProperties.h"

#include <Windows.h>

namespace MyEngine
{
	class Engine;

	class iEntityThreadPool
	{
	public:
		iEntityThreadPool() {};
		virtual ~iEntityThreadPool() {};

		virtual void CreateWorkers() = 0;

		virtual void EntityUpdate(Engine* pEngine, const Entity& entityId, const float& deltaTime) = 0;

		virtual void EntityRender(Engine* pEngine, const Entity& entityId) = 0;

		virtual void EnqueueTask(const Task& task) = 0;
	};
}
