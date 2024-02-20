#include "pch.h"

#include "Scene.h"

#include "Engine/ECS/Components.h"

namespace MyEngine
{
    Scene::Scene()
    {
        m_componentCounter = 0;
        m_pEntityManager = new EntityManager();

        InitializeCriticalSection(&m_CSEntities);
        InitializeCriticalSection(&m_CSComponents);
    }

    Scene::~Scene()
    {
        delete m_pEntityManager;

        using ComponentMapPair = std::pair<ComponentType, BiMap<Entity, ComponentId>*>;
        for (ComponentMapPair componentMap : m_componentMaps)
        {
            delete componentMap.second;
        }

        using ComponentPoolPair = std::pair<ComponentType, ComponentPool*>;
        for (ComponentPoolPair componentPool : m_componentPools)
        {
            delete componentPool.second;
        }

        DeleteCriticalSection(&m_CSEntities);
        DeleteCriticalSection(&m_CSComponents);
    }

    Entity Scene::CreateEntity(bool addDefault)
    {
        EnterCriticalSection(&m_CSEntities);

        Entity entityId = m_pEntityManager->AddEntity(EntityMask());

        if (addDefault)
        {
            AddComponent<TransformComponent>(entityId);
        }

        LeaveCriticalSection(&m_CSEntities);

        return entityId;
    }

    Entity Scene::CreateEntity(Entity entityId)
    {
        Entity newEntityId = CreateEntity();

        EntityMask entityMask = m_pEntityManager->GetMask(entityId);
        for (int componentType = 0; componentType < entityMask.size(); componentType++)
        {
            if (!entityMask[componentType])
            {
                continue;
            }

            // Check which component it is and add to the new entity
            if (componentType == GetComponentType<TagComponent>())
            {
                TagComponent* pTag = AddComponent<TagComponent>(newEntityId);
                *pTag = *(Get<TagComponent>(entityId));
            }
            else if (componentType == GetComponentType<TransformComponent>())
            {
                TransformComponent* pTransform = AddComponent<TransformComponent>(newEntityId);
                *pTransform = *(Get<TransformComponent>(entityId));
            }
            else if (componentType == GetComponentType<MovementComponent>())
            {
                MovementComponent* pMovement = AddComponent<MovementComponent>(newEntityId);
                *pMovement = *(Get<MovementComponent>(entityId));
            }
            else if (componentType == GetComponentType<RotationComponent>())
            {
                RotationComponent* pRotation = AddComponent<RotationComponent>(newEntityId);
                *pRotation = *(Get<RotationComponent>(entityId));
            }
            else if (componentType == GetComponentType<ParentComponent>())
            {
                ParentComponent* pParent = AddComponent<ParentComponent>(newEntityId);
                *pParent = *(Get<ParentComponent>(entityId));
            }
            else if (componentType == GetComponentType<TextureComponent>())
            {
                TextureComponent* pTexture = AddComponent<TextureComponent>(newEntityId);
                *pTexture = *(Get<TextureComponent>(entityId));
            }
            else if (componentType == GetComponentType<MaterialComponent>())
            {
                MaterialComponent* pMaterial = AddComponent<MaterialComponent>(newEntityId);
                *pMaterial = *(Get<MaterialComponent>(entityId));
            }
            else if (componentType == GetComponentType<LightComponent>())
            {
                LightComponent* pLight = AddComponent<LightComponent>(newEntityId);
                *pLight = *(Get<LightComponent>(entityId));
            }
            else if (componentType == GetComponentType<ModelComponent>())
            {
                ModelComponent* pModel = AddComponent<ModelComponent>(newEntityId);
                *pModel = *(Get<ModelComponent>(entityId));
            }
            else if (componentType == GetComponentType<TilingComponent>())
            {
                TilingComponent* pTiling = AddComponent<TilingComponent>(newEntityId);
                *pTiling = *(Get<TilingComponent>(entityId));
            }
            else if (componentType == GetComponentType<GravityComponent>())
            {
                GravityComponent* pGravity = AddComponent<GravityComponent>(newEntityId);
                *pGravity = *(Get<GravityComponent>(entityId));
            }
            else if (componentType == GetComponentType<RigidBodyComponent>())
            {
                RigidBodyComponent* pRigidBody = AddComponent<RigidBodyComponent>(newEntityId);
                *pRigidBody = *(Get<RigidBodyComponent>(entityId));
            }
            else if (componentType == GetComponentType<MeshColliderComponent>())
            {
                MeshColliderComponent* pMeshCollider = AddComponent<MeshColliderComponent>(newEntityId);
                *pMeshCollider = *(Get<MeshColliderComponent>(entityId));
            }
            else if (componentType == GetComponentType<SphereColliderComponent>())
            {
                SphereColliderComponent* pSphereCollider = AddComponent<SphereColliderComponent>(newEntityId);
                *pSphereCollider = *(Get<SphereColliderComponent>(entityId));
            }
            else if (componentType == GetComponentType<AABBColliderComponent>())
            {
                AABBColliderComponent* pAABBCollider = AddComponent<AABBColliderComponent>(newEntityId);
                *pAABBCollider = *(Get<AABBColliderComponent>(entityId));
            }
        }

        return newEntityId;
    }

    void Scene::EnterEntityCS(Entity entityId)
    {
        m_pEntityManager->EnterEntityCS(entityId);
    }

    void Scene::LeaveEntityCS(Entity entityId)
    {
        m_pEntityManager->LeaveEntityCS(entityId);
    }

    size_t Scene::GetNumberEntities()
    {
        return m_pEntityManager->Size();
    }

    void Scene::RemoveEntity(Entity entityId)
    {
        // TODO: Delete entity thread
        EnterCriticalSection(&m_CSEntities);
        m_entitiesToDestroy.insert(entityId);
        LeaveCriticalSection(&m_CSEntities);
    }

    bool Scene::HasComponents(Entity entityId, const EntityMask& componentMask)
    {
        return m_pEntityManager->HasComponents(entityId, componentMask);
    }

    void Scene::RemoveComponent(Entity entityId, ComponentType componentType)
    {
        EntityMask entityMask = m_pEntityManager->GetMask(entityId);

        if (!entityMask[componentType])
        {
            return;
        }

        EnterCriticalSection(&m_CSComponents);

        CompToDestroy comp = CompToDestroy();
        comp.componentType = componentType;
        comp.entityId = entityId;
        m_componentsToDestroy.insert(comp);

        LeaveCriticalSection(&m_CSComponents);
    }

    EntityManager* Scene::GetEntitymanager()
    {
        return m_pEntityManager;
    }

    void Scene::m_DestroyEntities()
    {
        for (Entity entityId : m_entitiesToDestroy)
        {
            // Go through every component the entity have to remove them
            EntityMask entityMask = m_pEntityManager->GetMask(entityId);
            for (int componentType = 0; componentType < entityMask.size(); componentType++)
            {
                RemoveComponent(entityId, componentType);
            }

            // Remove entity from entity manager list
            m_pEntityManager->RemoveEntity(entityId);
        }

        m_entitiesToDestroy.clear();
    }

    void Scene::m_DestroyComponents()
    {
        for (CompToDestroy comp : m_componentsToDestroy)
        {
            BiMap<Entity, ComponentId>* pComponentMap = m_componentMaps[comp.componentType];
            ComponentPool* pComponentPool = m_componentPools[comp.componentType];

            // Component to remove and new location for moved component
            ComponentId componentId;
            bool isValid = pComponentMap->GetByKey(comp.entityId, componentId);
            if (!isValid)
            {
                continue;
            }

            // Remove the component from the pool
            ComponentId componentIdOld;
            pComponentPool->Remove(componentId, componentIdOld);

            // Remove the relation mapping
            pComponentMap->RemoveByKey(comp.entityId);

            // Remove component flag mask
            m_pEntityManager->UnsetComponent(comp.entityId, comp.componentType);

            // Get entity id that had component moved
            Entity entityIdMoved;
            isValid = pComponentMap->GetByValue(componentIdOld, entityIdMoved);
            if (!isValid)
            {
                continue;
            }

            // Update the relation for the moved component
            pComponentMap->Update(entityIdMoved, componentId);
        }

        m_componentsToDestroy.clear();
    }
}
