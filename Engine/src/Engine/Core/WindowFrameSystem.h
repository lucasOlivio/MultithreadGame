#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Keeps track of all related to FPS
	class WindowFrameSystem : public iSystem
	{
	public:
		WindowFrameSystem() = default;
		virtual ~WindowFrameSystem() { };

		virtual std::string SystemName() { return "WindowFrameSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Update(Scene* pScene, Entity entityId, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void Render(Scene* pScene, Entity entityId);

		virtual void End(Scene* pScene);

		virtual void Shutdown();
	};
}
