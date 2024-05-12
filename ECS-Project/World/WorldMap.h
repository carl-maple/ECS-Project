#pragma once
#include "System/System.h"
#include "Utils/Base.h"

class CWorldMap : public CSystem
{
public:
	constexpr static uint32 WORLD_MAP_SIZE_X = 512;
	constexpr static uint32 WORLD_MAP_SIZE_Y = 512;
	constexpr static uint32 TILE_SIZE = 32;

	CWorldMap();
	~CWorldMap();

	void Update(const float dt) override;
};

