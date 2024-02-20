#include "pch.h"

#include "PlayerControllerSystem.h"


#include "Engine/ECS/SingletonComponents/CoreLocator.h"
#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"
#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

#include "Engine/Events/InputEvent.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/EventBusLocator.hpp"

#include "Engine/Core/InputProperties.h"
#include "Engine/ECS/Scene/SceneManagerLocator.h"

#include "Engine/Utils/InputUtils.h"
#include "Engine/Utils/GameplayUtils.h"

namespace MyEngine
{
    const float BULLET_RADIUS = 1.0f;
    const glm::vec3 BULLET_VELOCITY = glm::vec3(0.0f, 0.0f, 50.0f);
    const glm::vec3 BULLET_ACCELERATION = glm::vec3(0.0f, -9.8f, 0.0f);

	void PlayerControllerSystem::Init()
	{
        // Subscribe to keyboard event
        iEventBus<eInputEvents, KeyboardEvent>* pEventBus = EventBusLocator<eInputEvents, KeyboardEvent>::Get();

        pEventBus->Subscribe(eInputEvents::KEYBOARD, InputTriggered);
	}

	void PlayerControllerSystem::Start(Scene* pScene)
	{
	}

	void PlayerControllerSystem::Update(Scene* pScene, float deltaTime)
	{
		KeyInputComponent* pKey = CoreLocator::GetKeyInput();
        for (Entity playerId : SceneView<PlayerComponent, TransformComponent, MovementComponent>(*pScene))
        {
            PlayerComponent* pPlayer = pScene->Get<PlayerComponent>(playerId);
            TransformComponent* pTransform = pScene->Get<TransformComponent>(playerId);
            MovementComponent* pMovement = pScene->Get<MovementComponent>(playerId);

            glm::vec3 playerFront = glm::normalize(TransformUtils::GetForwardVector(pTransform->orientation));
            glm::vec3 playerSides = glm::normalize(glm::cross(playerFront, glm::vec3(UP_VECTOR)));

            glm::vec3 newVelocity = glm::vec3(0.0f);

            // Handle key presses for movement
            if (pKey->key[eKeyCodes::W])
            {
                newVelocity = pPlayer->speed * UP_VECTOR;
            }
            if (pKey->key[eKeyCodes::S])
            {
                newVelocity = -pPlayer->speed * UP_VECTOR;
            }
            if (pKey->key[eKeyCodes::A])
            {
                newVelocity = -pPlayer->speed * playerSides;
            }
            if (pKey->key[eKeyCodes::D])
            {
                newVelocity = pPlayer->speed * playerSides;
            }

            pMovement->velocity = newVelocity;
        }
	}

    void PlayerControllerSystem::Update(Scene* pScene, Entity entityId, float deltaTime)
    {
        EntityMask mask = SceneView<PlayerComponent, TransformComponent, MovementComponent>::GetMask(*pScene);

        if (!pScene->HasComponents(entityId, mask))
        {
            return;
        }

        KeyInputComponent* pKey = CoreLocator::GetKeyInput();

        PlayerComponent* pPlayer = pScene->Get<PlayerComponent>(entityId);
        TransformComponent* pTransform = pScene->Get<TransformComponent>(entityId);
        MovementComponent* pMovement = pScene->Get<MovementComponent>(entityId);

        glm::vec3 playerFront = glm::normalize(TransformUtils::GetForwardVector(pTransform->orientation));
        glm::vec3 playerSides = glm::normalize(glm::cross(playerFront, glm::vec3(UP_VECTOR)));

        glm::vec3 newVelocity = glm::vec3(0.0f);

        // Handle key presses for movement
        if (pKey->key[eKeyCodes::W])
        {
            newVelocity = pPlayer->speed * UP_VECTOR;
        }
        if (pKey->key[eKeyCodes::S])
        {
            newVelocity = -pPlayer->speed * UP_VECTOR;
        }
        if (pKey->key[eKeyCodes::A])
        {
            newVelocity = -pPlayer->speed * playerSides;
        }
        if (pKey->key[eKeyCodes::D])
        {
            newVelocity = pPlayer->speed * playerSides;
        }

        pMovement->velocity = newVelocity;
    }

	void PlayerControllerSystem::Render(Scene* pScene)
	{
	}

    void PlayerControllerSystem::Render(Scene* pScene, Entity entityId)
    {
    }

	void PlayerControllerSystem::End(Scene* pScene)
	{
	}

	void PlayerControllerSystem::Shutdown()
	{
        // Subscribe to keyboard event
        iEventBus<eInputEvents, KeyboardEvent>* pEventBus = EventBusLocator<eInputEvents, KeyboardEvent>::Get();

        pEventBus->Unsubscribe(eInputEvents::KEYBOARD, InputTriggered);
	}

    void PlayerControllerSystem::InputTriggered(const KeyboardEvent& event)
    {
        if (event.keyData.key == eKeyCodes::ESCAPE)
        {
            // Trigger window close event
            iEventBus<eWindowEvents, WindowCloseEvent>* pEventBus = EventBusLocator<eWindowEvents, WindowCloseEvent>::Get();

            WindowCloseEvent collEvent = WindowCloseEvent();
            pEventBus->Publish(collEvent);
        }
        if (event.keyData.key == eKeyCodes::SPACE && 
            (event.keyData.action == eInputActions::KEY_PRESS || event.keyData.action == eInputActions::KEY_REPEAT))
        {
            Scene* pScene = SceneManagerLocator::Get()->GetCurrentScene();
            Entity playerId = GameplayUtils::GetPlayerId(pScene);
            TransformComponent* pTransform = pScene->Get<TransformComponent>(playerId);

            glm::vec3 playerFront = glm::normalize(TransformUtils::GetForwardVector(pTransform->orientation));

            std::vector<glm::vec3> directions = {
                glm::vec3(0.0f, 5.0f, 5.0f),    // top
                glm::vec3(5.0f, 5.0f, 5.0f),    // top-right
                glm::vec3(-5.0f, 5.0f, 5.0f),   // top-left
                glm::vec3(0.0f, -5.0f, 5.0f),   // down
                glm::vec3(5.0f, -5.0f, 5.0f),   // down-right
                glm::vec3(-5.0f, -5.0f, 5.0f),  // down-left
                glm::vec3(5.0f, 0.0f, 5.0f),    // Right
                glm::vec3(-5.0f, 0.0f, 5.0f),   // Left
            };

            // Fire bullets from each direction
            for (const glm::vec3& direction : directions) 
            {
                for (int i = 1; i < 15; i++)
                {
                    glm::vec3 bulletPosition = pTransform->position + (playerFront + (direction * static_cast<float>(i)));
                    GameplayUtils::CreateBullet(BULLET_RADIUS, bulletPosition,
                        BULLET_VELOCITY, BULLET_ACCELERATION,
                        pScene);
                }
            }
        }
    }
}
