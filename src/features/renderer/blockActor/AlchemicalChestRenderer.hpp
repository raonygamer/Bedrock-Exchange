#pragma once

#include "mc/src-client/common/client/renderer/blockActor/ChestRenderer.hpp"

class AlchemicalChestRenderer : 
	public ChestRenderer 
{
public:
	AlchemicalChestRenderer(const std::shared_ptr<mce::TextureGroup>& textureGroup)
		: ChestRenderer(textureGroup) 
	{
		normalTex = TextureJSONParser::getTextureInfoFromTextureSet(textureGroup, Json::Value("textures/entity/chest/alchemical_chest"));
	}
};