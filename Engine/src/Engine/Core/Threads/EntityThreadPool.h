#pragma once

#include "Engine/Core/Threads/iEntityThreadPool.h"

#include <queue>

namespace MyEngine
{
	class EntityThreadPool : public iEntityThreadPool
	{
	public:
		EntityThreadPool();
		virtual ~EntityThreadPool();

		virtual void CreateWorkers();

		virtual void EntityUpdate(Engine* pEngine, const Entity& entityId, const float& deltaTime);

		virtual void EntityRender(Engine* pEngine, const Entity& entityId);

		virtual void EnqueueTask(const Task& task);

	private:
		const int MAX_THREADS = 128;

		// Task queue and synchronization primitives
		std::queue<Task> m_taskQueue;
		static CRITICAL_SECTION m_CSTasks;

		static DWORD WINAPI m_WorkerThread(LPVOID lpParam);
	};
}
