#pragma once

#include <memory>
#include "Entity/Entity.h"

class CWorld;
class CCamera;

class CCameraManager
{
public:
	CCameraManager(CWorld* World);
	~CCameraManager();

	void FollowEntity(const SEntityId Entity);
	void StopFollowingEnttiy();
	void Update(const float dt);
	const std::unique_ptr<CCamera>& GetCamera() const { return m_CurrentCamera; }
private:
	std::unique_ptr<CCamera> m_CurrentCamera;
	SEntityId m_AttachedEntity;
	CWorld* World;
	bool m_FollowAttachedEntity = false;
};

