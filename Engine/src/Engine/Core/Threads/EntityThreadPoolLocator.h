#pragma once

#include "NullEntityThreadPool.hpp"
#include "iEntityThreadPool.h"

namespace MyEngine
{
	class EntityThreadPoolLocator
	{
	public:
		static iEntityThreadPool* Get();

		static void Set(iEntityThreadPool* pEntityThreadPool);

	private:
		static iEntityThreadPool* m_pEntityThreadPool;
		static NullEntityThreadPool m_nullService;
	};
}
