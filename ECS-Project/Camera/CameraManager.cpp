#include "Camera/CameraManager.h"
#include "World/World.h"

#include "Component/GetterUtils.h"
#include "Component/ComponentManager.h"
#include "Transform/TransformComponentManager.h"

#include "Camera/Camera.h"

CCameraManager::CCameraManager(CWorld* World)
:World(World)
{
	m_CurrentCamera = std::make_unique<CCamera>();
}


CCameraManager::~CCameraManager()
{
}

void CCameraManager::FollowEntity(const SEntityId Entity)
{
	m_FollowAttachedEntity = true;
	m_AttachedEntity = Entity;
}

void CCameraManager::StopFollowingEnttiy()
{
	m_FollowAttachedEntity = false;
}

void CCameraManager::Update(const float /*dt*/)
{
	if (m_FollowAttachedEntity)
	{
		const CComponentPool<STransformComponent>* const transform_component_manager = NGetterUtils::GetComponent<STransformComponent>(World);
		const CVector2f& position = transform_component_manager->GetComponent(m_AttachedEntity).Position;
		m_CurrentCamera->SetPosition(position);
	}
}
