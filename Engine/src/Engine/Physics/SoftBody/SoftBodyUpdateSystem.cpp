#include "pch.h"

#include "SoftBodyUpdateSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Graphics/VAO/VAOManagerLocator.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/Math.h"

namespace MyEngine
{
    void SoftBodyUpdateSystem::Init()
    {
    }

    void SoftBodyUpdateSystem::Start(Scene* pScene)
    {
        return;
    }

    void SoftBodyUpdateSystem::Update(Scene* pScene, float deltaTime)
    {
        iVAOManager* pVAOManager = VAOManagerLocator::Get();
        // Update velocity and position
        for (Entity entityId : SceneView<TransformComponent, MovementComponent, SoftBodyComponent>(*pScene))
        {
            TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
            MovementComponent* pMovement = pScene->Get<MovementComponent>(entityId);
            SoftBodyComponent* pSoftBody = pScene->Get<SoftBodyComponent>(entityId);
            ModelComponent* pModel = pScene->Get<ModelComponent>(entityId);

            glm::vec3 weightedSum = glm::vec3(0.0f);
            glm::vec3 weightedDirection = glm::vec3(0.0f);
            float totalWeight = 0.0f;
            size_t vecSize = pSoftBody->vecParticles.size();
            sMesh* pMesh = nullptr;
            if (pModel)
            {
                pMesh = pModel->pMeshes[pModel->currMesh];
            }
            for (size_t i = 0; i < vecSize; i++)
            {
                SoftBodyParticle* pParticle = pSoftBody->vecParticles[i];
                // Verlet integration to apply acceleration to particles
                glm::vec3 currentPos = pParticle->position;
                glm::vec3 oldPos = pParticle->oldPosition;

                pParticle->position += (currentPos - oldPos) + (pMovement->acceleration * (float)(deltaTime * deltaTime));

                pParticle->oldPosition = currentPos;

                CleanZeros(pParticle->position);
                CleanZeros(pParticle->oldPosition);

                // Update vertex positions, normals and transform based on particles
                if (pMesh)
                {
                    sVertex& vertex = pMesh->pVertices[i];
                    glm::vec3 localPosition = TransformUtils::WorldToLocalPoint(pParticle->position,
                                                                                pTransform->worldPosition, 
                                                                                pTransform->worldOrientation, 
                                                                                pTransform->worldScale);
                    vertex.x = localPosition.x;
                    vertex.y = localPosition.y;
                    vertex.z = localPosition.z;
                }

                // HACK: Just to test it bouncing, later it should go to a collision system
                if (pParticle->position.y < 0.0f)
                {
                    pParticle->position.y = 0.0f;
                }

                // HACK: Updating transform based on particles average position, this should not be here

                // Calculate weighted sum of particle positions
                float distanceSquared = glm::length(pParticle->position - pTransform->position);
                float weight = 1.0f / (distanceSquared + 1.0f); // Adding 1 to avoid division by zero
                weightedSum += pParticle->position * weight;
                weightedDirection += glm::normalize(pParticle->position - pTransform->position) * weight;
                totalWeight += weight;
            }

            if (pMesh)
            {
                // Update transform position to weighted average position of particles
                if (totalWeight > 0.0f) {
                    glm::vec3 averagePosition = weightedSum / totalWeight;
                    pTransform->position = averagePosition;

                    // Calculate rotation to align with the weighted direction
                    glm::quat rotation = glm::quatLookAt(glm::normalize(weightedDirection), glm::vec3(UP_VECTOR));
                    pTransform->orientation = rotation;
                }

                m_UpdateModelNormals(pMesh);
                pVAOManager->UpdateVAOBuffers(pMesh->name, pMesh);
            }
        }
    }

    void SoftBodyUpdateSystem::Update(Scene* pScene, Entity entityId, float deltaTime)
    {
    }

    void SoftBodyUpdateSystem::Render(Scene* pScene)
    {
    }

    void SoftBodyUpdateSystem::End(Scene* pScene)
    {
    }

    void SoftBodyUpdateSystem::Shutdown()
    {
    }

    void SoftBodyUpdateSystem::m_UpdateModelNormals(sMesh* pMesh)
    {
        for (unsigned int vertIndex = 0; vertIndex != pMesh->numberOfVertices; vertIndex++)
        {
            pMesh->pVertices[vertIndex].nx = 0.0f;
            pMesh->pVertices[vertIndex].ny = 0.0f;
            pMesh->pVertices[vertIndex].nz = 0.0f;
            pMesh->pVertices[vertIndex].nw = 0.0f;
        }

        for (unsigned int i = 0; i < pMesh->numberOfIndices; i += 3)
        {
            unsigned int vertAIndex = pMesh->pIndices[i + 0];
            unsigned int vertBIndex = pMesh->pIndices[i + 1];
            unsigned int vertCIndex = pMesh->pIndices[i + 2];

            sVertex& vertexA = pMesh->pVertices[vertAIndex];
            sVertex& vertexB = pMesh->pVertices[vertBIndex];
            sVertex& vertexC = pMesh->pVertices[vertCIndex];

            glm::vec3 vertA = glm::vec3(vertexA.x, vertexA.y, vertexA.z);
            glm::vec3 vertB = glm::vec3(vertexB.x, vertexB.y, vertexB.z);
            glm::vec3 vertC = glm::vec3(vertexC.x, vertexC.y, vertexC.z);

            glm::vec3 triangleEdgeAtoB = vertB - vertA;
            glm::vec3 triangleEdgeAtoC = vertC - vertA;

            glm::vec3 normal = glm::cross(triangleEdgeAtoB, triangleEdgeAtoC);
            normal = glm::normalize(normal);

            // Add (accumulate) this normal to the three vertices
            vertexA.nx += normal.x;
            vertexA.ny += normal.y;
            vertexA.nz += normal.z;

            vertexB.nx += normal.x;
            vertexB.ny += normal.y;
            vertexB.nz += normal.z;

            vertexC.nx += normal.x;
            vertexC.ny += normal.y;
            vertexC.nz += normal.z;
        }

        // Now normalize the accumulated normals
        for (unsigned int vertIndex = 0; vertIndex != pMesh->numberOfVertices; vertIndex++)
        {
            glm::vec3 normal = glm::vec3(pMesh->pVertices[vertIndex].nx,
                                         pMesh->pVertices[vertIndex].ny,
                                         pMesh->pVertices[vertIndex].nz);
            normal = glm::normalize(normal);

            pMesh->pVertices[vertIndex].nx = normal.x;
            pMesh->pVertices[vertIndex].ny = normal.y;
            pMesh->pVertices[vertIndex].nz = normal.z;
        }
    }
}
