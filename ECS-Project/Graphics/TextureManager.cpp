#include "TextureManager.h"
#include <SDL_error.h>
#include <SDL_image.h>
#include <string>


CTextureManager::CTextureManager(SDL_Renderer* renderer)
:m_Renderer(renderer)
{
}

CTextureManager::~CTextureManager()
{
	std::unordered_map<std::string, SDL_Texture*>::iterator it = m_TextureMap.begin();
	std::unordered_map<std::string, SDL_Texture*>::iterator it_end = m_TextureMap.end();
	for (; it != it_end; ++it)
	{
		SDL_DestroyTexture(it->second);
	}

	m_TextureMap.clear();
}

SDL_Texture* CTextureManager::CreateTexture(const char* texture_name)
{
	std::unordered_map<std::string, SDL_Texture*>::const_iterator it = m_TextureMap.find(texture_name);
	if (it != m_TextureMap.end())
	{
		return it->second;
	}

	SDL_Texture* Texture = nullptr;
	SDL_Surface* surface = IMG_Load(texture_name);

	if (surface)
	{
		Texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
		if (Texture != nullptr)
		{
			m_TextureMap.insert(std::make_pair(std::string(texture_name), Texture));
		}
		else
		{
			SDL_Log("Failed in creating the Texture: %s", SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}
	else
	{
		SDL_Log("Failed in creating the loading bmp image: %s", SDL_GetError());
	}

	return Texture;
}
