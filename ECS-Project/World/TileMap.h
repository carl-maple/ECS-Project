#pragma once

#include "Types/Vector2i.h"
#include "Types/Vector2f.h"

#include <array>
#include <vector>

class CWorld;

namespace NTileMapConstans
{
	constexpr int32 MAP_SIZE_X = 25;
	constexpr int32 MAP_SIZE_Y = 25;
	constexpr CVector2i MAP_SIZE = { MAP_SIZE_X, MAP_SIZE_Y };

	constexpr CVector2f TILE_SIZE = { 20.f, 20.f };
	constexpr char* TILE_TEXURE_FILE_NAME = { "square.png" };

	constexpr uint32 MAX_NUMBER_OF_OCCUPANTS = 10;
}

struct STileIndex
{
	STileIndex(const uint32& InIndex) :Index(InIndex) {}
	operator uint32() const { return Index; }
private:
	uint32 Index;
};

struct SEntityId;

class CTileMap
{
public:
	CTileMap(CWorld* InWorld);
	~CTileMap();

	void CreateTileEntity(const CVector2f& InPosition);
	void AddOccupant(const STileIndex InTileIndex, const SEntityId Entity);
	void RemoveOccupant(const STileIndex InTileIndex, const SEntityId Entity);
	const STileIndex GetTileIndexFromPosition(const CVector2f& InPosition) const;
	const STileIndex GetTileIndex(const CVector2i& InTilePosition) const;

private:

	size_t TileTextureHash;
	CWorld* World;
	std::array <std::vector< SEntityId >, NTileMapConstans::MAP_SIZE_X * NTileMapConstans::MAP_SIZE_Y> TileOccupants;
};

