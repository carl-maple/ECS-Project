#include "PlayerInputMovementSystem.h"
#include "Entity/EntityManager.h"
#include "System/SystemManager.h"
#include "Transform/VelocityComponentManager.h"
#include "PlayerInput.h"

CPlayerInputMovementSystem::CPlayerInputMovementSystem()
{
}


CPlayerInputMovementSystem::~CPlayerInputMovementSystem()
{
}

void CPlayerInputMovementSystem::Update(const float dt)
{
	const std::bitset<NBitSize::BIT_SIZE>& SystemBit = SystemManager->GetBit<CPlayerInputMovementSystem>();

	const std::vector<SEntityId>& EntitiesToBeUpdated = SystemManager->GetSystem<CEntityManager>()->GetEntitiesToBeUpdated(SystemBit);

	CComponentPool<SVelocityComponent>* VelocityComponentManager = SystemManager->GetComponent<SVelocityComponent>();

	constexpr float MovementSpeed = 50.f; //This should be a component

	for (const SEntityId Entity : EntitiesToBeUpdated)
	{
		SVelocityComponent& VelocityComponent = VelocityComponentManager->GetComponent(Entity);
		const std::unique_ptr<CPlayerInput>& PlayerInput = SystemManager->GetWorld()->GetPlayerInput();

		VelocityComponent.Velocity.x = 0.f;
		VelocityComponent.Velocity.y = 0.f;

		if (PlayerInput->IsHeld(EAction::A_MOVE_LEFT))
		{
			VelocityComponent.Velocity.x -= 1;
		}
		if (PlayerInput->IsHeld(EAction::A_MOVE_RIGHT))
		{
			VelocityComponent.Velocity.x += 1;
		}
		if (PlayerInput->IsHeld(EAction::A_MOVE_BACKWARD))
		{
			VelocityComponent.Velocity.y -= 1;
		}
		if (PlayerInput->IsHeld(EAction::A_MOVE_FORWARD))
		{
			VelocityComponent.Velocity.y += 1;
		}

		VelocityComponent.Velocity.Normalize();
		VelocityComponent.Velocity *= MovementSpeed * dt;
	}
}
