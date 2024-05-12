#pragma once

class CSystemManager;

class CSystem
{
public:
	virtual ~CSystem() {};
	virtual void Init(CSystemManager* system_manager) { SystemManager = system_manager; };
	virtual void Update(const float dt) {};
	virtual void SendMessage() {};
	CSystemManager* SystemManager;
};

