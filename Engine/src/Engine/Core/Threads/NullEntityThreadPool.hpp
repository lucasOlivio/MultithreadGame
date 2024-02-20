#pragma once

#include "Engine/Core/Threads/iEntityThreadPool.h"

#include "Engine/Utils/Log.h"

namespace MyEngine
{
	class NullEntityThreadPool : public iEntityThreadPool
	{
	public:
		NullEntityThreadPool() {};
		virtual ~NullEntityThreadPool() {};

		virtual void CreateWorkers()
		{
			LOG_ERROR("Thread pool not created!");
		}

		virtual void EntityUpdate(Engine* pEngine, const Entity& entityId, const float& deltaTime)
		{
			LOG_ERROR("Thread pool not created!");
		}

		virtual void EntityRender(Engine* pEngine, const Entity& entityId)
		{
			LOG_ERROR("Thread pool not created!");
		}

		virtual void EnqueueTask(const Task& task)
		{
			LOG_ERROR("Thread pool not created!");
		}
	};
}
