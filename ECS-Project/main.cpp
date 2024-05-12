#define SDL_MAIN_HANDLED

#include <SDL.h>
#include "Utils/Base.h"

#include "System/SystemManager.h"

#include "World/World.h"
#include "AABox/AABox.h"
#include "World/TileMap.h"

#include "Graphics/GraphicsSystem.h"
#include "Graphics/TextureComponentManager.h"

#include "GJKSolver/GJKSolver.h"
#include "GJKSolver/EPACalculator.h"

#include "PlayerInput/PlayerInput.h"
#include "PlayerInput/PlayerInputMovementSystem.h"

#include "Entity/EntityManager.h"
#include "Component/GetterUtils.h"

#include "Camera/CameraManager.h"

#include "Transform/VelocityComponentManager.h"
#include "Transform/TransformComponentManager.h"
#include "Transform/UpdatePosition.h"


static bool g_Running = false;

static int WindowHeight = 0;
static int WindowWidth = 0;

void HandleEvents(std::vector<SDL_Event>& InputEvents, std::vector<SDL_WindowEvent>& WindowEvents)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
			{
				InputEvents.push_back(event);
				break;
			}
			case SDL_QUIT:
			{
				g_Running = false;
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	std::unique_ptr<CWorld> World = std::make_unique<CWorld>();

	std::vector<SDL_Event> InputEvents;
	InputEvents.reserve(24);

	std::vector<SDL_WindowEvent> WindowEvents;
	WindowEvents.reserve(8);

	uint32 CurrentTimeStamp = 0;
	uint32 LastFrameTimeStamp = 0;
	float DeltaTime = 0;

	CGraphicsSystem* const GraphicsSystem = World->GetSystemManager()->GetSystem<CGraphicsSystem>();

	std::unique_ptr<CTileMap> TileMap = std::make_unique<CTileMap>(World.get());

	{
		CVector2i WindowSize;
		GraphicsSystem->GetRendererOutputSize(WindowSize);

		CEntityManager* EntityManager = World->GetSystemManager()->GetSystem<CEntityManager>();
		const SEntityId Entity = EntityManager->CreateEntity();

		CComponentPool<STransformComponent>* TransformComponent = NGetterUtils::GetComponent<STransformComponent>(World.get());
		CComponentPool<STextureComponent>* TextureComponent = NGetterUtils::GetComponent<STextureComponent>(World.get());
		CComponentPool<SVelocityComponent>* VelocityComponent = NGetterUtils::GetComponent<SVelocityComponent>(World.get());

		const CVector2f CenterPos = { WindowSize.x * 0.5f, WindowSize.y * 0.5f };

		EntityManager->AddComponent(Entity, TransformComponent);

		STransformComponent NewTransformComponent;
		NewTransformComponent.Position = CenterPos;
		TransformComponent->CreateInstance(Entity, std::move(NewTransformComponent));

		EntityManager->AddComponent(Entity, TextureComponent);

		constexpr char* TEXTURE_HASH = { "square.png" };
		size_t TextureHash = GraphicsSystem->CreateTexture(TEXTURE_HASH);

		STextureComponent NewTextureComponent;
		NewTextureComponent.TextureHash = TextureHash;
		NewTextureComponent.Size = NTileMapConstans::TILE_SIZE;
		TextureComponent->CreateInstance(Entity, std::move(NewTextureComponent));

		EntityManager->AddSystemBit(Entity, World->GetSystemTypeManager()->GetType(typeid(CGraphicsSystem)).GetBit());
		EntityManager->AddSystemBit(Entity, World->GetSystemTypeManager()->GetType(typeid(CUpdatePosition)).GetBit());
		EntityManager->AddSystemBit(Entity, World->GetSystemTypeManager()->GetType(typeid(CPlayerInputMovementSystem)).GetBit());

		World->GetCameraManager()->FollowEntity(Entity);
	}

	g_Running = true;
	while (g_Running)
	{
		LastFrameTimeStamp = CurrentTimeStamp;
		CurrentTimeStamp = SDL_GetTicks();
		DeltaTime = (CurrentTimeStamp - LastFrameTimeStamp) / 1000.0f;

		HandleEvents(InputEvents, WindowEvents);
		World->GetPlayerInput()->Update(InputEvents);
		GraphicsSystem->HandleWindowEvents(WindowEvents);

		{
			CVector2i WindowSize;
			GraphicsSystem->GetRendererOutputSize(WindowSize);

			const CVector2f CenterPosition = { WindowSize.x * 0.5f, WindowSize.y * 0.5f };

			int32 MouseX;
			int32 MouseY;
			SDL_GetMouseState(&MouseX, &MouseY);

			static float Size1 = 50.f;

			CVector2f Position(CenterPosition);
			static bool UseMousePosition = true;
			if (UseMousePosition)
			{
				Position = CVector2f(static_cast<float>(MouseX), static_cast<float>(WindowSize.y - MouseY));
			}
			
			const CAABox Box1 = { CAABox::CreateAABoxFromPositionSize(Position, CVector2f(Size1, Size1)) };
			GraphicsSystem->DrawAABox(Box1, NColor::Red);
			
			static float OffsetX2 = 25.f;
			static float OffestY2 = 25.f;

			static float Size2 = 50.f;

			const CAABox Box2 = { CAABox::CreateAABoxFromPositionSize(CVector2f(CenterPosition.x + OffsetX2
				, CenterPosition.y + OffestY2), CVector2f(Size2, Size2)) };
			GraphicsSystem->DrawAABox(Box2, NColor::Green);

			CGJKSolver Solver;
			std::vector<SSupportPoint> SupportPoints;
			const bool Intersection = Solver.Intersect(Box1.GetVertices(), Box2.GetVertices(), SupportPoints);

			GraphicsSystem->DrawDebugText(CVector2f::Zero()
				, NColor::White, "Is overlapping: %s", Intersection ? "true" : "false" );

			for (size_t i = 0; i < SupportPoints.size(); i++)
			{
				const int32 NextIndex = (i + 1) % SupportPoints.size();
				GraphicsSystem->DrawDebugLine(SupportPoints[i].Position
					, SupportPoints[NextIndex].Position, NColor::Magenta);
			}

			if (Intersection)
			{
				CEPACalculator Calculator;

				const SEPAContactInfo ContactInfo = Calculator.GetContactInfo(Box1.GetVertices()
					, Box2.GetVertices(), SupportPoints);

				GraphicsSystem->DrawAABox(CAABox::CreateAABoxFromPositionSize(ContactInfo.m_PositionA
					, CVector2f(10.f, 10.f)), NColor::Red);
				GraphicsSystem->DrawAABox(CAABox::CreateAABoxFromPositionSize(ContactInfo.m_PositionB
					, CVector2f(10.f, 10.f)), NColor::Green);
			}
		}

		InputEvents.clear();
		WindowEvents.clear();

		World->Update(DeltaTime);

		GraphicsSystem->Render();
	}

	SDL_Quit();

	return 0;
}