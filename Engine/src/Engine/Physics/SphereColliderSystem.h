#pragma once

#include "Engine/ECS/System/iSystem.h"
#include "Engine/ECS/Components.h"

namespace MyEngine
{
	// Simple collision system just to show performance improve on threads
	class SphereColliderSystem : public iSystem
	{
	public:
		SphereColliderSystem() = default;
		virtual ~SphereColliderSystem() { };

		virtual std::string SystemName() { return "SphereColliderSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Update(Scene* pScene, Entity entityId, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void Render(Scene* pScene, Entity entityId);

		virtual void End(Scene* pScene);

		virtual void Shutdown();

	private:

		void m_ResolveCollision(TransformComponent* pTransformA, SphereColliderComponent* pSphereA,
								TransformComponent* pTransformB, SphereColliderComponent* pSphereB);
	};
}
