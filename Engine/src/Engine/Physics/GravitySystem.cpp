#include "pch.h"

#include "GravitySystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

namespace MyEngine
{
    void GravitySystem::Init()
    {
    }

    void GravitySystem::Start(Scene* pScene)
    {
    }

    void GravitySystem::Update(Scene* pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : SceneView<MovementComponent, GravityComponent>(*pScene))
        {
            MovementComponent* pMovement = pScene->Get<MovementComponent>(entityId);
            GravityComponent* pGravity = pScene->Get<GravityComponent>(entityId);

            pMovement->velocity = pMovement->velocity + (pGravity->acceleration * deltaTime);
        }
    }

    void GravitySystem::Update(Scene* pScene, Entity entityId, float deltaTime)
    {
        EntityMask mask = SceneView<MovementComponent, GravityComponent>::GetMask(*pScene);

        if (!pScene->HasComponents(entityId, mask))
        {
            return;
        }

        MovementComponent* pMovement = pScene->Get<MovementComponent>(entityId);
        GravityComponent* pGravity = pScene->Get<GravityComponent>(entityId);

        pMovement->velocity = pMovement->velocity + (pGravity->acceleration * deltaTime);
    }

    void GravitySystem::Render(Scene* pScene)
    {
    }

    void GravitySystem::End(Scene* pScene)
    {
    }

    void GravitySystem::Shutdown()
    {
    }
}
