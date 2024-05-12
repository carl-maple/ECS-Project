#pragma once

#include "Utils/Base.h"
#include <unordered_map>
#include <SDL_render.h>

class CTextureManager
{
public:
	CTextureManager(SDL_Renderer* renderer);
	~CTextureManager();
	SDL_Texture* CreateTexture(const char* Texture);
private:
	std::unordered_map<std::string, SDL_Texture*> m_TextureMap;
	SDL_Renderer* m_Renderer;
};

