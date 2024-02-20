#include "pch.h"

#include "SystemBuilder.h"

#include "Engine/Core/CoreSystem.h"
#include "Engine/Core/WindowFrameSystem.h"
#include "Engine/Core/TransformParentSystem.h"
#include "Engine/Core/InputSystem.h"
#include "Engine/Core/StateSystem.h"

#include "Engine/Graphics/WindowSystem.h"
#include "Engine/Graphics/ModelRenderSystem.h"
#include "Engine/Graphics/Renderer/RenderSystem.h"
#include "Engine/Graphics/Animations/AnimationSystem.h"
#include "Engine/Graphics/Animations/AnimationPlayerSystem.h"
#include "Engine/Graphics/Lights/LightSystem.h"
#include "Engine/Graphics/Shaders/ShaderSystem.h"
#include "Engine/Graphics/Particles/ParticleEmissionSystem.h"
#include "Engine/Graphics/Particles/ParticleUpdaterSystem.h"
#include "Engine/Graphics/Particles/ParticleRenderSystem.h"
#include "Engine/Graphics/UI/BaseUISystem.h"
#include "Engine/Graphics/FrameBuffers/FrameBufferSystem.h"

#include "Engine/Physics/MovementSystem.h"
#include "Engine/Physics/RotationSystem.h"
#include "Engine/Physics/GravitySystem.h"
#include "Engine/Physics/BroadPhase/GridBroadPhaseSystem.h"
#include "Engine/Physics/NarrowPhase/CollisionSystem.h"
#include "Engine/Physics/SoftBody/SotBodyConstraintsSystem.h"
#include "Engine/Physics/SoftBody/SoftBodyUpdateSystem.h"
#include "Engine/Physics/SoftBody/SoftBodyResponseSystem.h"

#include "Engine/Gameplay/SteeringBehaviorSystem.h"

#include "Engine/Debug/DebugSystem.h"
#include "Engine/Debug/DrawGridSystem.h"
#include "Engine/Debug/DrawCollisionSystem.h"
#include "Engine/Debug/DrawSoftBodyPartclesSystem.h"

namespace MyEngine
{
    std::map<std::string, SystemBuilder::SystemFactory> SystemBuilder::m_systemFactories
    {
        // Any other system from engine should come into this map
        // 
        // Core Systems
        {"CoreSystem", []() { return new CoreSystem; }},
        {"WindowFrameSystem", []() { return new WindowFrameSystem; }},
        {"TransformParentSystem", []() { return new TransformParentSystem; }},
        {"InputSystem", []() { return new InputSystem; }},
        {"StateSystem", []() { return new StateSystem; }},
        // Graphics systems
        {"WindowSystem", []() { return new WindowSystem; }},
        {"BaseUISystem", []() { return new BaseUISystem; }},
        {"ShaderSystem", []() { return new ShaderSystem; }},
        {"ModelRenderSystem", []() { return new ModelRenderSystem; }},
        {"RenderSystem", []() { return new RenderSystem; }},
        {"FrameBufferSystem", []() { return new FrameBufferSystem; }},
        // Physics systems
        {"MovementSystem", []() { return new MovementSystem; }}
    };

    void SystemBuilder::RegisterSystem(const std::string& name, SystemFactory factory)
    {
        m_systemFactories[name] = factory;
    }

    iSystem* SystemBuilder::CreateSystem(const std::string& name)
    {
        auto it = m_systemFactories.find(name);
        if (it != m_systemFactories.end())
        {
            // Call the factory function to create an instance
            return it->second();
        }
        else
        {
            LOG_ERROR("Unknown system '" + name);
            return nullptr;
        }
    }


}
