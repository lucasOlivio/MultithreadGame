#pragma once

#include "Engine/ECS/SingletonComponents/Components.h"

namespace MyEngine
{
	class PhysicsLocator
	{
	public:
		static NarrowPhaseTestsComponent* GetNarrowPhaseTests();
		static FrameCollisionComponent* GetFrameCollision();

		static void SetNarrowPhaseTests(NarrowPhaseTestsComponent* pNarrowPhaseTests);
		static void SetFrameCollision(FrameCollisionComponent* pFrameCollision);

		static void Clear();

	private:
		static NarrowPhaseTestsComponent* m_pNarrowPhaseTests;
		static FrameCollisionComponent* m_pFrameCollision;
	};
}
