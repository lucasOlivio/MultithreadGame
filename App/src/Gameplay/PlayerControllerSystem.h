#pragma once

#include "Engine/ECS/System/iSystem.h"

#include "Engine/Events/InputEvent.h"

namespace MyEngine
{
	// All player input handling
	class PlayerControllerSystem : public iSystem
	{
	public:
		PlayerControllerSystem() = default;
		virtual ~PlayerControllerSystem() { };

		virtual std::string SystemName() { return "PlayerControllerSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Update(Scene* pScene, Entity entityId, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void Render(Scene* pScene, Entity entityId);

		virtual void End(Scene* pScene);

		virtual void Shutdown();

		static void InputTriggered(const KeyboardEvent& event);
	};
}
