#include "pch.h"

#include "WindowSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"

#include "Engine/Events/EventBusLocator.hpp"
#include "Engine/Events/WindowEvents.h"

#include "Engine/Graphics/Shaders/ShaderManager.h"

namespace MyEngine
{
    void WindowSystem::Init()
    {
        glfwSetErrorCallback(GlfwErrorCallback);
        if (!glfwInit())
        {
            LOG_ERROR("Error initializing GLFW!\n");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        // Set width and height from maximized window
        WindowComponent* pWindow = GraphicsLocator::GetWindow();

        pWindow->pGLFWWindow = glfwCreateWindow(pWindow->width,
                                                pWindow->height,
                                                pWindow->name.c_str(), NULL, NULL);
        if (!pWindow->pGLFWWindow)
        {
            LOG_ERROR("Error creating window '" + pWindow->name + "'!\n");
            glfwTerminate();
            return;
        }

        // Update width and height for maximized window
        glfwGetFramebufferSize(pWindow->pGLFWWindow, &(pWindow->width), &(pWindow->height));

        glfwMakeContextCurrent(pWindow->pGLFWWindow);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);

        // While drawing a pixel, see if the pixel that's already there is closer or not?
        glEnable(GL_DEPTH_TEST);
        // Activate transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // set to not draw "back facing" triangles
        glCullFace(GL_BACK);
    }

    void WindowSystem::Start(Scene* pScene)
    {
    }

    void WindowSystem::Update(Scene* pScene, float deltaTime)
    {
    }

    void WindowSystem::Update(Scene* pScene, Entity entityId, float deltaTime)
    {
    }

    void WindowSystem::Render(Scene* pScene)
    {
        WindowComponent* pWindow = GraphicsLocator::GetWindow();

        if (glfwWindowShouldClose(pWindow->pGLFWWindow))
        {
            m_TriggerWindowClose();
        }

        // Update window title
        if (pWindow->prevName == pWindow->name)
        {
            return;
        }
        glfwSetWindowTitle(pWindow->pGLFWWindow, pWindow->name.c_str());
        pWindow->prevName = pWindow->name;
    }

    void WindowSystem::Render(Scene* pScene, Entity entityId)
    {
    }

    void WindowSystem::End(Scene* pScene)
    {
    }

    void WindowSystem::Shutdown()
    {
        WindowComponent* pWindow = GraphicsLocator::GetWindow();
        if (pWindow->pGLFWWindow) {
            glfwDestroyWindow(pWindow->pGLFWWindow);
        }
        glfwTerminate();
    }

    void WindowSystem::m_TriggerWindowClose()
    {
        iEventBus<eWindowEvents, WindowCloseEvent>* pEventBus = EventBusLocator<eWindowEvents, WindowCloseEvent>::Get();

        WindowCloseEvent collEvent = WindowCloseEvent();
        pEventBus->Publish(collEvent);
    }
}
