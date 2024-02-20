#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Should be the only one to change the transform component data
	class MovementSystem : public iSystem
	{
	public:
		MovementSystem() = default;
		virtual ~MovementSystem() { };

		virtual std::string SystemName() { return "MovementSystem"; };

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
