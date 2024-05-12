#include "System/SystemManager.h"
#include "Graphics/GraphicsSystem.h"
#include "Entity/EntityManager.h"
#include "World/World.h"
#include "SystemTypeManager.h"
#include "PlayerInput/PlayerInputMovementSystem.h"
#include "Transform/UpdatePosition.h"

CSystemManager::CSystemManager(CWorld* InWorld)
: World(InWorld)
{
	CreateSystem<CGraphicsSystem>();
	CreateSystem<CEntityManager>();
	CreateSystem<CPlayerInputMovementSystem>();
	CreateSystem<CUpdatePosition>();
}

CSystemManager::~CSystemManager()
{
}

void CSystemManager::Update(float DeltaTime)
{
	for (auto& System : Systems)
	{
		System.second->Update(DeltaTime);
	}
}
