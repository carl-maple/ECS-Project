#pragma once

#include "System/System.h"
#include "Utils/Base.h"
#include "Camera/Camera.h"
#include "Entity/Entity.h"
#include "Utils/DebugRender.h"

#include <vector>
#include <unordered_map>
#include <map>

#include <SDL_events.h>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Window;
struct FC_Font;
class CAABox;

class CTransformComponentManager;
class CTextureComponentManager;

class CGraphicsSystem : public CSystem
{
public:
	CGraphicsSystem();
	~CGraphicsSystem();

	struct STexture
	{
		STexture(SDL_Texture* InTexture, const CVector2i InSize) : Texture(InTexture), Size(InSize) {}
		SDL_Texture* Texture;
		CVector2i Size;
	};

	void Update(float DeltaTime) override;

	void Render();

	size_t CreateTexture(const char* TextureName);

	int GetRendererOutputSize(CVector2i& WindowSize) const;

	void HandleWindowEvents(const std::vector<SDL_WindowEvent>& WindowEvents);

	void DrawDebugLine(const CVector2f& InStart, const CVector2f& InEnd, const NColor::SColor& InColor);
	void DrawDebugText(const CVector2f InPosition, const NColor::SColor& InColor, const char* FormattedText, ...);
	void DrawAABox(const CAABox& InBox, const NColor::SColor& InColor);

	const STexture& CGraphicsSystem::GetTexture(const size_t InHash) const;
	const CVector2f GetCameraAdjustmentValues() const;

private:
	void SetRenderDrawColor(const NColor::SColor& color);

	std::unordered_map<size_t, STexture> TextureMap;
	SDL_Renderer* Renderer;
	SDL_Window* Window;

	const int WINDOW_SIZE_X = 1600;
	const int WINDOW_SIZE_Y = 900;

	CVector2i WindowSize;

	FC_Font* Font;

	std::vector<NDebugRender::SLineStruct> DebugLineDraw;
	std::vector<NDebugRender::SDebugText> DebugText;

};
