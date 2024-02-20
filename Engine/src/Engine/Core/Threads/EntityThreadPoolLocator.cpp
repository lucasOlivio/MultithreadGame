#include "pch.h"

#include "EntityThreadPoolLocator.h"

namespace MyEngine
{
	NullEntityThreadPool EntityThreadPoolLocator::m_nullService = NullEntityThreadPool();
	iEntityThreadPool* EntityThreadPoolLocator::m_pEntityThreadPool = &(EntityThreadPoolLocator::m_nullService);

	iEntityThreadPool* EntityThreadPoolLocator::Get()
	{
		return m_pEntityThreadPool;
	}

	void EntityThreadPoolLocator::Set(iEntityThreadPool* pEntityThreadPool)
	{
		if (pEntityThreadPool == nullptr)
		{
			m_pEntityThreadPool = &m_nullService;
		}
		else
		{
			m_pEntityThreadPool = pEntityThreadPool;
		}
	}
}
