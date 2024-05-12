#include "GraphicsSystem.h"
#include "System/SystemManager.h"
#include "Camera/CameraManager.h"

#include "Transform/TransformComponentManager.h"
#include "Graphics/TextureComponentManager.h"
#include "Component/ComponentManager.h"

#include "Entity/EntityManager.h"
#include "AABox/AABox.h"

#include "C/SDL_FontCache.h"
#include <SDL_image.h>

#include <vadefs.h>
#include <set>

static CGraphicsSystem::STexture DUMMY_TEXTURE = CGraphicsSystem::STexture(nullptr, CVector2i());

CGraphicsSystem::CGraphicsSystem()
{
	WindowSize = CVector2i(WINDOW_SIZE_X, WINDOW_SIZE_Y);

	Window = SDL_CreateWindow("SpaceInvaders",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WindowSize.x,
		WindowSize.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (Window == nullptr)
	{
		SDL_Log("Unable to create SDL window: %s", SDL_GetError());
	}

	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (Renderer == nullptr)
	{
		SDL_Log("Unable to create SDL renderer: %s", SDL_GetError());
	}

	Font = FC_CreateFont();
	constexpr char* FONT_PATH = "fonts/Simple tfb.ttf";
	FC_LoadFont(Font, Renderer, FONT_PATH, 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
}


CGraphicsSystem::~CGraphicsSystem()
{
	for (const std::pair<size_t, STexture> texture_pair : TextureMap)
	{
		SDL_DestroyTexture(texture_pair.second.Texture);
	}

	TextureMap.clear();

	FC_FreeFont(Font);

	SDL_DestroyRenderer(Renderer);

	SDL_DestroyWindow(Window);
}

void CGraphicsSystem::Update(float dt)
{
}

void CGraphicsSystem::Render()
{
	const CComponentPool<STextureComponent>* TextureComponentPool = SystemManager->GetWorld()->GetComponentManager()->GetComponent<STextureComponent>();
	const CComponentPool<STransformComponent>* TransformComponentPool = SystemManager->GetWorld()->GetComponentManager()->GetComponent<STransformComponent>();

	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Renderer);

	SDL_Rect SourceRect;
	SDL_Rect DestRect;

	const std::bitset<NBitSize::BIT_SIZE>& SystemBit = SystemManager->GetWorld()->GetSystemTypeManager()->GetBit<CGraphicsSystem>();

	const std::vector<SEntityId>& EntitiesToBeUpdated = SystemManager->GetSystem<CEntityManager>()->GetEntitiesToBeUpdated(SystemBit);

	const CVector2f CameraAdjustmentValues = GetCameraAdjustmentValues();

	for (const SEntityId& Entity : EntitiesToBeUpdated)
	{
		CVector2f EntityPosition = TransformComponentPool->GetComponent(Entity).Position;

		const STextureComponent& TextureComponent = TextureComponentPool->GetComponent(Entity);
		const STexture& Texture = GetTexture(TextureComponent.TextureHash);

		SourceRect.x = 0;
		SourceRect.y = 0;
		SourceRect.w = Texture.Size.x;
		SourceRect.h = Texture.Size.y;

		EntityPosition.x += CameraAdjustmentValues.x;
		EntityPosition.y += CameraAdjustmentValues.y;
		EntityPosition.y = std::abs(WindowSize.y - EntityPosition.y);

		DestRect.x = static_cast<int32>(EntityPosition.x);
		DestRect.y = static_cast<int32>(EntityPosition.y);
		DestRect.w = static_cast<int32>(TextureComponent.Size.x);
		DestRect.h = static_cast<int32>(TextureComponent.Size.y);

		SDL_RenderCopy(Renderer, Texture.Texture, &SourceRect, &DestRect);
	}


	for (const NDebugRender::SLineStruct& DebugLine : DebugLineDraw)
	{
		SetRenderDrawColor(DebugLine.Color);

		const int32 StartX = static_cast<int32>(DebugLine.Start.x + CameraAdjustmentValues.x);
		int32 StartY = static_cast<int32>(DebugLine.Start.y + CameraAdjustmentValues.y);
		StartY = std::abs(static_cast<int32>(WindowSize.y - StartY));

		const int32 EndX = static_cast<int32>(DebugLine.End.x + CameraAdjustmentValues.x);
		int32 EndY = static_cast<int32>(DebugLine.End.y + CameraAdjustmentValues.y);
		EndY = std::abs(static_cast<int32>(WindowSize.y - EndY));

		SDL_RenderDrawLine(Renderer, StartX, StartY, EndX, EndY);
	}

	for (const NDebugRender::SDebugText& DebugString : DebugText)
	{
		FC_Draw(Font, Renderer, DebugString.Position.x, DebugString.Position.y, DebugString.DebugText.c_str());
	}

	DebugText.clear();
	DebugLineDraw.clear();

	SDL_RenderPresent(Renderer);
}

void CGraphicsSystem::SetRenderDrawColor(const NColor::SColor& color)
{
	SDL_SetRenderDrawColor(Renderer, color.Red, color.Green, color.Blue, color.Alpha);
}

size_t CGraphicsSystem::CreateTexture(const char* texture_name)
{
	std::hash<std::string> hash_string;
	const size_t hash = hash_string(texture_name);

	std::unordered_map<size_t, STexture>::const_iterator it = TextureMap.find(hash);
	if (it != TextureMap.end())
	{
		return it->first;
	}

	SDL_Texture* Texture = nullptr;
	SDL_Surface* surface = IMG_Load(texture_name);

	if (surface == nullptr)
	{
		SDL_Log("Failed in creating the loading bmp image: %s", SDL_GetError());
		return -1;
	}

	Texture = SDL_CreateTextureFromSurface(Renderer, surface);

	if (Texture == nullptr)
	{
		SDL_Log("Failed in creating the Texture: %s", SDL_GetError());
		return -1;
	}

	CVector2i size;
	SDL_QueryTexture(Texture, nullptr, nullptr, &size.x, &size.y);

	STexture texture_struct = { Texture, size };

	TextureMap.insert(std::make_pair(hash, texture_struct));
	
	SDL_FreeSurface(surface);

	return hash;
}

int CGraphicsSystem::GetRendererOutputSize(CVector2i& WindowSize) const
{
	return SDL_GetRendererOutputSize(Renderer, &WindowSize.x, &WindowSize.y);
}

void CGraphicsSystem::HandleWindowEvents(const std::vector<SDL_WindowEvent>& WindowEvents)
{
	for (const SDL_WindowEvent& window_event : WindowEvents)
	{
		switch (static_cast<SDL_WindowEventID>(window_event.event))
		{
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			GetRendererOutputSize(WindowSize);
		}
		case SDL_WINDOWEVENT_NONE:
			//SDL_Log("SDL_WINDOWEVENT_NONE");
			break;
		case SDL_WINDOWEVENT_SHOWN:
			//SDL_Log("SDL_WINDOWEVENT_SHOWN");
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			//SDL_Log("SDL_WINDOWEVENT_HIDDEN");
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			//SDL_Log("SDL_WINDOWEVENT_EXPOSED");
			break;
		case SDL_WINDOWEVENT_MOVED:
			//SDL_Log("SDL_WINDOWEVENT_MOVED");
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			//SDL_Log("SDL_WINDOWEVENT_MINIMIZED");
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			//SDL_Log("SDL_WINDOWEVENT_MAXIMIZED");
			break;
		case SDL_WINDOWEVENT_RESTORED:
			//SDL_Log("SDL_WINDOWEVENT_RESTORED");
			break;
		case SDL_WINDOWEVENT_ENTER:
			//SDL_Log("SDL_WINDOWEVENT_ENTER");
			break;
		case SDL_WINDOWEVENT_LEAVE:
			//SDL_Log("SDL_WINDOWEVENT_LEAVE");
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			//SDL_Log("SDL_WINDOWEVENT_FOCUS_GAINED");
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			//SDL_Log("SDL_WINDOWEVENT_FOCUS_LOST");
			break;
		case SDL_WINDOWEVENT_CLOSE:
			//SDL_Log("SDL_WINDOWEVENT_CLOSE");
			break;
		case SDL_WINDOWEVENT_TAKE_FOCUS:
			//SDL_Log("SDL_WINDOWEVENT_TAKE_FOCUS");
			break;
		case SDL_WINDOWEVENT_HIT_TEST:
			//SDL_Log("SDL_WINDOWEVENT_HIT_TEST");
			break;
		default:
			break;
		}
	}
}

void CGraphicsSystem::DrawDebugLine(const CVector2f& InStart, const CVector2f& InEnd, const NColor::SColor& InColor)
{
	DebugLineDraw.emplace_back(InStart, InEnd, InColor);
}

const CGraphicsSystem::STexture& CGraphicsSystem::GetTexture(const size_t Hash) const
{
	const std::unordered_map<size_t, STexture>::const_iterator It = TextureMap.find(Hash);
	if (It != TextureMap.end())
	{
		return It->second;
	}

	return DUMMY_TEXTURE;
}

const CVector2f CGraphicsSystem::GetCameraAdjustmentValues() const
{
	const std::unique_ptr<CCamera>& Camera = SystemManager->GetWorld()->GetCameraManager()->GetCamera();

	const CVector2f& CameraPosition = Camera->GetPosition();

	CVector2f CameraAdjustmentValues;
	CameraAdjustmentValues.x = (WindowSize.x * 0.5f) - CameraPosition.x;
	CameraAdjustmentValues.y = (WindowSize.y * 0.5f) - CameraPosition.y;

	return CameraAdjustmentValues;
}

void CGraphicsSystem::DrawDebugText(const CVector2f Position, const NColor::SColor& Color
	, const char* FormattedText, ...)
{
	constexpr int32 TempBufferSize = 256;
	char TempBuffer[TempBufferSize];

	va_list VaList;
	va_start(VaList, FormattedText);
	vsnprintf(TempBuffer, TempBufferSize, FormattedText, VaList);
	va_end(VaList);

	DebugText.emplace_back(TempBuffer, Position, Color);
}


void CGraphicsSystem::DrawAABox(const CAABox& InBox, const NColor::SColor& InColor)
{
	const CVector2f BottomLeft = { InBox.Min };
	const CVector2f BottomRight = { InBox.Max.x, InBox.Min.y };
	const CVector2f TopLeft = { InBox.Min.x, InBox.Max.y };
	const CVector2f TopRight = { InBox.Max };

	DrawDebugLine(BottomLeft, BottomRight, InColor);
	DrawDebugLine(BottomLeft, TopLeft, InColor);
	DrawDebugLine(TopLeft, TopRight, InColor);
	DrawDebugLine(TopRight, BottomRight, InColor);
}