#include "pch.h"

#include "RendererManager.h"

#include "Engine/Utils/GraphicsUtils.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/Utils/TransformUtils.h"

#include "Engine/Graphics/Materials/MaterialManagerLocator.h"
#include "Engine/Graphics/FrameBuffers/FrameBufferManagerLocator.h"
#include "Engine/Graphics/Shaders/ShaderManager.h"

namespace MyEngine
{
	using pairFBOInfos = std::pair<uint, std::vector<sRenderModelInfo>>;
	using itFBOInfos = std::map<uint, std::vector<sRenderModelInfo>>::iterator;

	RendererManager::RendererManager()
	{
	}

	void RendererManager::AddFBO(uint FBOID)
	{
		m_mapRenderInfos[FBOID] = {};
		m_mapRenderInfos[FBOID].reserve(MAX_ENTITIES);
	}

	void RendererManager::AddToRender(uint FBOID, const sRenderModelInfo& renderInfo)
	{
		assert(m_mapRenderInfos.find(FBOID) != m_mapRenderInfos.end() && "FBOID not found in map!");

		m_mapRenderInfos[FBOID].push_back(renderInfo);
	}

	void RendererManager::RenderAllModels(Scene* pScene)
	{
		iMaterialManager* pMaterialManager = MaterialManagerLocator::Get();
		iFrameBufferManager* pFrameBufferManager = FrameBufferManagerLocator::Get();
		iShaderProgram* pShader = ShaderManager::GetActiveShader();

		// Bind the fbo then render all their respective models
		// First element will always be the main screen buffer, so we start from the second
		itFBOInfos it = std::next(m_mapRenderInfos.begin());

		// Iterate over the map starting from the second element
		for (; it != m_mapRenderInfos.end(); ++it)
		{
			uint FBOID = it->first;

			pFrameBufferManager->BindFBO(pScene, FBOID);
			pFrameBufferManager->ClearFBO(FBOID);

			const std::vector<sRenderModelInfo>& renderInfos = it->second;

			// Render all models associated with the current FBOID
			for (const sRenderModelInfo& renderInfo : renderInfos)
			{
				pMaterialManager->BindMaterial(renderInfo.materialName);

				if (renderInfo.isFBOView)
				{
					pFrameBufferManager->BindFBOText(renderInfo.FBOViewID);
				}

				GraphicsUtils::DrawModel(renderInfo);

				if (renderInfo.isFBOView)
				{
					pShader->SetUniformInt("isFBOView", false);
				}
			}
		}

		// TODO: Separate this into function, now its too coupled with a lot of other stuff

		// Making sure we return to main buffer
		pFrameBufferManager->BindFBO(pScene, 0);

		const std::vector<sRenderModelInfo>& renderInfos = m_mapRenderInfos[0];

		// Render all models associated with the FBOID 0
		for (const sRenderModelInfo& renderInfo : renderInfos)
		{
			pMaterialManager->BindMaterial(renderInfo.materialName);

			if (renderInfo.isFBOView)
			{
				pFrameBufferManager->BindFBOText(renderInfo.FBOViewID);
			}

			GraphicsUtils::DrawModel(renderInfo);

			if (renderInfo.isFBOView)
			{
				pShader->SetUniformInt("isFBOView", false);
			}
		}
	}

	void RendererManager::ClearRender()
	{
		for (itFBOInfos it = m_mapRenderInfos.begin(); it != m_mapRenderInfos.end(); ++it) 
		{
			it->second.clear();
		}
	}
}