#include "pch.h"

#include "PhysicsLocator.h"

namespace MyEngine
{
    NarrowPhaseTestsComponent* PhysicsLocator::m_pNarrowPhaseTests = new NarrowPhaseTestsComponent();
    FrameCollisionComponent* PhysicsLocator::m_pFrameCollision = new FrameCollisionComponent();

    NarrowPhaseTestsComponent* PhysicsLocator::GetNarrowPhaseTests()
    {
        return m_pNarrowPhaseTests;
    }

    FrameCollisionComponent* PhysicsLocator::GetFrameCollision()
    {
        return m_pFrameCollision;
    }

    void PhysicsLocator::SetNarrowPhaseTests(NarrowPhaseTestsComponent* pNarrowPhaseTests)
    {
        delete m_pNarrowPhaseTests;
        m_pNarrowPhaseTests = pNarrowPhaseTests;
    }

    void PhysicsLocator::SetFrameCollision(FrameCollisionComponent* pFrameCollision)
    {
        delete m_pFrameCollision;
        m_pFrameCollision = pFrameCollision;
    }

    void PhysicsLocator::Clear()
    {
        delete m_pNarrowPhaseTests;
        delete m_pFrameCollision;
    }
}
