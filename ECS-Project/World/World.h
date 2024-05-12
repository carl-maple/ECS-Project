#pragma once

#include <memory>

class CComponentTypeManager;
class CSystemTypeManager;
class CSystemManager;
class CComponentManager;
class CCameraManager;
class CPlayerInput;

class CWorld
{
public:
	CWorld();
	~CWorld();

	const std::unique_ptr<CComponentTypeManager>& GetComponentTypeManager() const { return ComponentTypeManager; }
	const std::unique_ptr<CSystemTypeManager>& GetSystemTypeManager() const { return SystemTypeManager; }
	const std::unique_ptr<CComponentManager>& GetComponentManager() const { return ComponentManager; }
	const std::unique_ptr<CCameraManager>& GetCameraManager() const { return CameraManager; }
	const std::unique_ptr<CSystemManager>& GetSystemManager() const { return SystemManager; }
	const std::unique_ptr<CPlayerInput>& GetPlayerInput() const { return PlayerInput; }

	void Update(const float dt);
private:

	std::unique_ptr<CComponentTypeManager> ComponentTypeManager;
	std::unique_ptr<CSystemTypeManager> SystemTypeManager;
	std::unique_ptr<CSystemManager> SystemManager;
	std::unique_ptr<CComponentManager> ComponentManager;
	std::unique_ptr<CCameraManager> CameraManager;
	std::unique_ptr<CPlayerInput> PlayerInput;
	
};

