#pragma once

#include "Engine/Core/Threads/EntityThreadPoolLocator.h"

#include "Engine/ECS/Scene/Scene.h"
#include "Engine/ECS/System/iSystem.h"
#include "Engine/ECS/Scene/iSceneManager.h"

#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/CollisionEvent.h"
#include "Engine/Events/InputEvent.h"
#include "Engine/Events/SceneEvent.h"
#include "Engine/Events/GameStateEvent.h"
#include "Engine/Events/AnimationEvent.h"
#include "Engine/Events/iEventBus.h"

#include "Engine/Graphics/Renderer/iRendererManager.h"
#include "Engine/Graphics/VAO/iVAOManager.h"
#include "Engine/Graphics/Shaders/iShaderManager.h"
#include "Engine/Graphics/Materials/iMaterialManager.h"
#include "Engine/Graphics/Textures/iTextureManager.h"
#include "Engine/Graphics/FrameBuffers/iFrameBufferManager.h"

#include <vector>

namespace MyEngine
{
	// App should inherit from this class to setup and run everything needed
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		float GetDeltaTime();

		// Systems that will manipulate components and handle the scene in some way,
		// the system is added and initialized, if the scene is passed the system is also started
		virtual void AddSystem(std::string systemName, Scene* pScene = nullptr);

		// Ends scene for system, shutdown and deletes pointer
		virtual void RemoveSystem(std::string systemName, Scene* pScene = nullptr);

		virtual iSystem* GetSystem(std::string systemName);

		virtual void Init();

		// Run engine starting simulation state as running or stopped
		virtual void Run(std::string initialSceneName, bool startSimulation = true);

		bool IsRunning();

		virtual void Update();

		virtual void Render();

		// Goes through all systems updating this entity
		void UpdateEntity(const Entity& entityId, const float& deltaTime);

		// Render entity using all rendering systems
		void RenderEntity(const Entity& entityId);

		virtual void Shutdown();

		virtual void LoadConfigurations();

		// Initializes systems with scene
		virtual void StartSystems(Scene* pScene);

		// Finishes systems for scene
		virtual void EndSystems(Scene* pScene);

		virtual void ShutdownSystems();

		virtual void OnSceneChange(const SceneChangeEvent& event);

		virtual void OnWindowClose(const WindowCloseEvent& event);

	protected:
		std::vector<iSystem*> m_vecSystems;

		Scene* m_pCurrentScene = nullptr;

		// Events
		iEventBus<eWindowEvents, WindowCloseEvent>* m_pEventBusWindow;
		iEventBus<eCollisionEvents, RigidBodyCollisionEvent>* m_pEventBusRigidCollision;
		iEventBus<eCollisionEvents, SoftBodyCollisionEvent>* m_pEventBusSoftCollision;
		iEventBus<eInputEvents, KeyboardEvent>* m_pEventBusKeyboard;
		iEventBus<eInputEvents, MouseEvent>* m_pEventBusMouse;
		iEventBus<eSceneEvents, SceneChangeEvent>* m_pEventBusSceneChange;
		iEventBus<eGameStateEvents, GameStartedEvent>* m_pEventBusStartedState;
		iEventBus<eGameStateEvents, GameRunningEvent>* m_pEventBusRunningState;
		iEventBus<eGameStateEvents, GameStoppedEvent>* m_pEventBusStoppedState;
		iEventBus<eGameStateEvents, GameOverEvent>* m_pEventBusGameOverState;
		iEventBus<eAnimationEvents, PositionKeyFrameEvent>* m_pEventBusPosKeyFrame;
		iEventBus<eAnimationEvents, RotationKeyFrameEvent>* m_pEventBusRotKeyFrame;
		iEventBus<eAnimationEvents, ScaleKeyFrameEvent>* m_pEventBusScaleKeyFrame;

		// Resources managers
		iVAOManager* m_pVAOManager;
		iShaderManager* m_pShaderManager;
		iMaterialManager* m_pMaterialManager;
		iTextureManager* m_pTextureManager;
		iRendererManager* m_pRendererManager;
		iSceneManager* m_pSceneManager;
		iFrameBufferManager* m_pFrameBufferManager;

		iEntityThreadPool* m_pEntityThreadPool;

		float m_lastTime = 0.0f;
		std::vector<float> m_frameTimes;

		bool m_isRunning;

		// Multithread support
		LONG m_numThreadsUpdate;
		LONG m_numThreadsRender;

		// Any major clears needed to be done at end of frame (Ex: scene deleting, entity delete)
		virtual void m_ClearFrame();

		// Operations to execute before rendering
		virtual void m_BeginFrame();

		// Operations to execute after rendering
		virtual void m_EndFrame();
	};
}
