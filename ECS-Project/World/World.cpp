#include "World/World.h"
#include "Component/ComponentTypeManager.h"
#include "System/SystemTypeManager.h"
#include "System/SystemManager.h"
#include "Component/ComponentManager.h"
#include "Camera/CameraManager.h"
#include "PlayerInput/PlayerInput.h"

CWorld::CWorld()
{
	ComponentTypeManager = std::make_unique<CComponentTypeManager>();
	SystemTypeManager = std::make_unique<CSystemTypeManager>();
	SystemManager = std::make_unique<CSystemManager>(this);
	ComponentManager = std::make_unique<CComponentManager>(this);
	CameraManager = std::make_unique<CCameraManager>(this);
	PlayerInput = std::make_unique<CPlayerInput>(this);
}


CWorld::~CWorld()
{
}

void CWorld::Update(const float dt)
{
	SystemManager->Update(dt);
	CameraManager->Update(dt);
}
