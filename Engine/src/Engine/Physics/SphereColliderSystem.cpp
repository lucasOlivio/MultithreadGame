#include "pch.h"

#include "SphereColliderSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/CollisionsUtils.h"

namespace MyEngine
{
    void SphereColliderSystem::Init()
    {
    }

    void SphereColliderSystem::Start(Scene* pScene)
    {
        return;
    }

    void SphereColliderSystem::Update(Scene* pScene, float deltaTime)
    {
    }

    void SphereColliderSystem::Update(Scene* pScene, Entity entityId, float deltaTime)
    {
        EntityMask mask = SceneView<TransformComponent, SphereColliderComponent>::GetMask(*pScene);

        if (!pScene->HasComponents(entityId, mask))
        {
            return;
        }

        TransformComponent* pTransformA = pScene->Get<TransformComponent>(entityId);
        SphereColliderComponent* pSphereA = pScene->Get<SphereColliderComponent>(entityId);

        const glm::vec3& positionA = pTransformA->worldPosition;
        const float& radiusA = pSphereA->radius;

        for (Entity entityIdB : SceneView<TransformComponent, SphereColliderComponent>(*pScene))
        {
            if (entityId == entityIdB)
            {
                continue;
            }

            TransformComponent* pTransformB = pScene->Get<TransformComponent>(entityIdB);
            SphereColliderComponent* pSphereB = pScene->Get<SphereColliderComponent>(entityIdB);

            const glm::vec3& positionB = pTransformB->worldPosition;
            const float& radiusB = pSphereB->radius;

            bool isCollision = CollisionsUtils::SphereSphere_Overlap(radiusA, positionA,
									                                radiusB, positionB);

            if (!isCollision)
            {
                continue;
            }

            m_ResolveCollision(pTransformA, pSphereA,
                                pTransformB, pSphereB);
        }
    }

    void SphereColliderSystem::Render(Scene* pScene)
    {
    }

    void SphereColliderSystem::Render(Scene* pScene, Entity entityId)
    {
    }

    void SphereColliderSystem::End(Scene* pScene)
    {
    }

    void SphereColliderSystem::Shutdown()
    {
    }

    void SphereColliderSystem::m_ResolveCollision(TransformComponent* pTransformA, SphereColliderComponent* pSphereA, 
                                                  TransformComponent* pTransformB, SphereColliderComponent* pSphereB)
    {
        glm::vec3 normal = CollisionsUtils::SphereSphere_Normal(pTransformA->worldPosition, pTransformB->worldPosition);

        float displacement = glm::length(pTransformA->worldPosition - pTransformB->worldPosition);
        float penetrationDepth = displacement - pSphereA->radius;

        glm::vec3 correction = normal * penetrationDepth;

        pTransformA->position += correction;
        pTransformB->position -= correction;
    }
}
