#pragma once

#include "Engine/Graphics/Renderer/iRendererManager.h"

#include <map>
#include <vector>

namespace MyEngine
{
	class RendererManager : public iRendererManager
	{
	public:
		RendererManager();
		virtual ~RendererManager() {};

		// Maps the fbo id and reserve its vector
		virtual void AddFBO(uint FBOID);

		// Add model to rendering pipeline into the respective FBO
		virtual void AddToRender(uint FBOID, const sRenderModelInfo& renderInfo);

		// Render all models mapped into their respective FBOs
		virtual void RenderAllModels(Scene* pScene);

		// Clear all model mapping from all FBOs
		virtual void ClearRender();

	private:
		std::map<uint /* FBOID */, std::vector<sRenderModelInfo>> m_mapRenderInfos;
	};
}
