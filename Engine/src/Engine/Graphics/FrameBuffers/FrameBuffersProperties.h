#pragma once

#include "Engine/Core/types.h"

namespace MyEngine
{
	#define INITIAL_FBO_TEXTURE_ID 50;

	struct FrameBufferObject
	{
		uint ID = 0; 
		uint colorTextureId = 0;
		uint depthTextureId = 0;

		Entity cameraId = 1;

		int width = 1;
		int height = 1;
	};
}
