#include "pch.h"

#include "MovementSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void MovementSystem::Init()
    {
    }

    void MovementSystem::Start(Scene* pScene)
    {
        return;
    }

    void MovementSystem::Update(Scene* pScene, float deltaTime)
    {
    }

    void MovementSystem::Update(Scene* pScene, Entity entityId, float deltaTime)
    {
        EntityMask mask = SceneView<TransformComponent, MovementComponent>::GetMask(*pScene);

        if (!pScene->HasComponents(entityId, mask))
        {
            return;
        }

        TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
        MovementComponent* pMovement = pScene->Get<MovementComponent>(entityId);

        glm::vec3 newVelocity = pMovement->velocity + (pMovement->acceleration * deltaTime);
        glm::vec3 dragForce = newVelocity * -(pMovement->drag * deltaTime);
        pMovement->velocity = newVelocity + dragForce;

        // Clip velocity between min and max
        if (pMovement->velocity.length() <= 0.5f || pMovement->maxSpeed == 0.0f)
        {
            pMovement->velocity = glm::vec3(0.0f);
        }
        else if (pMovement->velocity.length() > pMovement->maxSpeed)
        {
            pMovement->velocity = glm::normalize(pMovement->velocity) * pMovement->maxSpeed;
        }

        pTransform->position = pTransform->position + (pMovement->velocity * deltaTime);

        // HACK: Avoid from falling
        if (pTransform->position.y < 0)
        {
            pTransform->position.y = 0;
        }
    }

    void MovementSystem::Render(Scene* pScene)
    {
    }

    void MovementSystem::Render(Scene* pScene, Entity entityId)
    {
    }

    void MovementSystem::End(Scene* pScene)
    {
    }

    void MovementSystem::Shutdown()
    {
    }
}
