#include "Transform/UpdatePosition.h"
#include "Entity/EntityManager.h"
#include "System/SystemManager.h"
#include "Transform/VelocityComponentManager.h"
#include "Transform/TransformComponentManager.h"

CUpdatePosition::CUpdatePosition()
{
}


CUpdatePosition::~CUpdatePosition()
{
}

void CUpdatePosition::Update(const float dt)
{
	const std::bitset<NBitSize::BIT_SIZE>& SystemBit = SystemManager->GetBit<CUpdatePosition>();

	
	const std::vector<SEntityId>& EntitiesToBeUpdated = SystemManager->GetSystem<CEntityManager>()->GetEntitiesToBeUpdated(SystemBit);

	const CComponentPool<SVelocityComponent>* VelocityComponentPool = SystemManager->GetComponent<SVelocityComponent>();
	CComponentPool<STransformComponent>* TransformComponentPool = SystemManager->GetComponent<STransformComponent>();

	for (const SEntityId Entity : EntitiesToBeUpdated)
	{
		const CVector2f& Velocity = VelocityComponentPool->GetComponent(Entity).Velocity;
		CVector2f& Position = TransformComponentPool->GetComponent(Entity).Position;
		Position += Velocity;
	}
}
