#include "pch.h"

#include "EntityThreadPool.h"

#include "Engine/Core/EngineLocator.h"

#include "Engine/ECS/Scene/Scene.h"

namespace MyEngine
{
	CRITICAL_SECTION EntityThreadPool::m_CSTasks = CRITICAL_SECTION();

	EntityThreadPool::EntityThreadPool()
	{
		InitializeCriticalSection(&m_CSTasks);
	}

	EntityThreadPool::~EntityThreadPool()
	{
		DeleteCriticalSection(&m_CSTasks);
	}

	void EntityThreadPool::CreateWorkers()
	{
		for (int i = 0; i < MAX_THREADS; ++i) {
			HANDLE threadHandle = CreateThread(NULL, 0, EntityThreadPool::m_WorkerThread, this, 0, NULL);

			if (threadHandle)
			{
				CloseHandle(threadHandle);
			}
			else
			{
				LOG_ERROR("Failed to create thread handle!");
			}
		}
	}

	void EntityThreadPool::EntityUpdate(Engine* pEngine, const Entity& entityId, const float& deltaTime)
	{
		Task task;
		task.func = [pEngine, entityId, deltaTime]() 
		{
			pEngine->UpdateEntity(entityId, deltaTime);
		};
		EnqueueTask(task);
	}

	void EntityThreadPool::EntityRender(Engine* pEngine, const Entity& entityId)
	{
		Task task;
		task.func = [pEngine, entityId]() 
		{
			pEngine->RenderEntity(entityId);
		};
		EnqueueTask(task);
	}

	void EntityThreadPool::EnqueueTask(const Task& task)
	{
		EnterCriticalSection(&m_CSTasks);
		m_taskQueue.push(task);
		LeaveCriticalSection(&m_CSTasks);
	}

	DWORD WINAPI EntityThreadPool::m_WorkerThread(LPVOID lpParam)
	{
		Engine* pEngine = EngineLocator::Get();
		EntityThreadPool* threadPool = static_cast<EntityThreadPool*>(lpParam);

		while (pEngine->IsRunning())
		{
			EnterCriticalSection(&threadPool->m_CSTasks);
			if (threadPool->m_taskQueue.empty())
			{
				LeaveCriticalSection(&threadPool->m_CSTasks);
				Sleep(0);
				continue;
			}

			Task task = threadPool->m_taskQueue.front();
			threadPool->m_taskQueue.pop();
			LeaveCriticalSection(&threadPool->m_CSTasks);

			task.func();

			Sleep(0);
		}

		return 0;
	}
}
