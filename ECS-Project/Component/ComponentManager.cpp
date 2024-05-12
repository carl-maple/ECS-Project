#include "Component/ComponentManager.h"
#include "Graphics/TextureComponentManager.h"
#include "Transform/TransformComponentManager.h"
#include "Transform/VelocityComponentManager.h"

CComponentManager::CComponentManager(CWorld* InWorld)
:World(InWorld)
{
	CreateComponent<STransformComponent>(10000);
	CreateComponent<STextureComponent>(10000);
	CreateComponent<SVelocityComponent>(10000);
}


CComponentManager::~CComponentManager()
{
}
