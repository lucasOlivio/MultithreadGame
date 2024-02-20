#include "pch.h"

#include "GameplayUtils.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Graphics/VAO/VAOManagerLocator.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/Math.h"

namespace MyEngine
{
    const std::string BULLET_MODEL = "sphere.ply";

    Entity GameplayUtils::GetPlayerId(Scene* pScene)
    {
        SceneView<PlayerComponent>::Iterator it = SceneView<PlayerComponent>(*pScene).begin();

        return *it;
    }

    Entity GameplayUtils::CreateBullet(const float radius,
								       const glm::vec3& position, 
								       const glm::vec3& velocity, 
								       const glm::vec3& acceleration,
								       Scene* pScene)
    {
        Entity bulletId = pScene->CreateEntity();

        TransformComponent* pTransform = pScene->AddComponent<TransformComponent>(bulletId);
        pTransform->position = position;
        pTransform->orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        pTransform->scale = 1.0f;

        MovementComponent* pMovement = pScene->AddComponent<MovementComponent>(bulletId);
        pMovement->velocity = velocity;
        pMovement->acceleration = acceleration;
        pMovement->drag = 0.0f;
        pMovement->maxSpeed = glm::length(velocity);

        RigidBodyComponent* pRigidBody = pScene->AddComponent<RigidBodyComponent>(bulletId);
        pRigidBody->bodyType = eBody::ACTIVE;
        pRigidBody->shapeType = eShape::SPHERE;

        SphereColliderComponent* pSphereCollider = pScene->AddComponent<SphereColliderComponent>(bulletId);
        pSphereCollider->radius = radius;

        ModelComponent* pModel = pScene->AddComponent<ModelComponent>(bulletId);
        pModel->isActive = true;
        pModel->doNotLight = true;
        pModel->isWireframe = false;
        pModel->isDynamic = false;
        pModel->material = "";
        pModel->useColorTexture = false;
        pModel->useDefaultColor = true;
        pModel->defaultColor = RED;
        pModel->models = { BULLET_MODEL };
        pModel->FBOIDs = { 0 };

        // Loading mesh
        iVAOManager* pVAOManager = VAOManagerLocator::Get();
        sMesh* pMesh = pVAOManager->LoadModelIntoVAO(BULLET_MODEL, false);
        pModel->pMeshes = { pMesh };

        return bulletId;
    }

    void GameplayUtils::CalculateSteeringDirections(const glm::vec3& myPosition, const glm::vec3& targetPosition,
                                                   glm::quat& myOrientation, glm::vec3& myVelocity, 
                                                   const glm::vec3& targetVelocity, bool isFleeing, 
                                                   float speed, float maxDistance, 
                                                   float futureTime, StopConditionFunction stopCondition)
    {
        // Calculate direction to/from target
        glm::vec3 futurePosition = targetPosition + (targetVelocity * futureTime);
        glm::vec3 direction = futurePosition - myPosition;
        if (isFleeing)
        {
            direction = -direction;
        }

        // Normalize direction
        glm::vec3 lockedAxis = glm::vec3(1.0f, 0.0f, 1.0f);
        glm::vec3 directionNormal = glm::normalize(direction * lockedAxis);

        // Update orientation
        glm::vec3 currForward = TransformUtils::GetForwardVector(myOrientation);
        float angle = Angle(currForward, direction, UP_VECTOR);
        glm::vec3 yaw = angle * UP_VECTOR;
        myOrientation = TransformUtils::AdjustOrientation(myOrientation, yaw);

        // Update movement direction
        float distToTarget = glm::distance(futurePosition, myPosition);
        if (stopCondition(distToTarget, maxDistance))
        {
            myVelocity = glm::vec3(0.0f);

            return;
        }

        // HACK: Additional condition from m_ApproachTarget
        if (!isFleeing && distToTarget < maxDistance)
        {
            directionNormal = -directionNormal;
        }

        myVelocity = directionNormal * speed;
    }
}
