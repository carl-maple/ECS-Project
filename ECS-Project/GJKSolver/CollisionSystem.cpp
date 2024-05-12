#include "CollisionSystem.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "System/SystemManager.h"
#include "AABox/AABoxComponentManager.h"
#include "AABox/AABox.h"

#include "GJKSolver/EPACalculator.h"
#include "SupportPoint.h"

CCollisionSystem::CCollisionSystem()
{
}

CCollisionSystem::~CCollisionSystem()
{
}

void CCollisionSystem::Update(const float dt)
{
	const std::bitset<NBitSize::BIT_SIZE>& SystemBit = SystemManager->GetBit<CCollisionSystem>();

	
	const std::vector<SEntityId>& EntitiesToBeUpdated = SystemManager->GetSystem<CEntityManager>()->GetEntitiesToBeUpdated(SystemBit);

	const CComponentPool<CAABox>* aabox_man = SystemManager->GetComponent<CAABox>();

	for (size_t i = 0; i < EntitiesToBeUpdated.size(); ++i)
	{
		const SEntityId first = EntitiesToBeUpdated[i];
		for (size_t n = i + 1; n < EntitiesToBeUpdated.size(); ++n)
		{
			const SEntityId second = EntitiesToBeUpdated[n];

			const std::vector<CVector2f> first_box = aabox_man->GetComponent(first).GetVertices();
			const std::vector<CVector2f> second_box = aabox_man->GetComponent(second).GetVertices();

			const bool intersection = m_Solver.Intersect(first_box, second_box, m_SupportPoints);

			if (intersection)
			{
				CEPACalculator calculator;
				const SEPAContactInfo contact_info = calculator.GetContactInfo(first_box, second_box, m_SupportPoints);
			}
		}
		
	}

	
}
