#include "TileMap.h"
#include "World/World.h"
#include "System/SystemManager.h"
#include "Graphics/GraphicsSystem.h"
#include "Entity/EntityManager.h"
#include "Component/GetterUtils.h"
#include "Transform/TransformComponentManager.h"
#include "Graphics/TextureComponentManager.h"
#include "Entity/Entity.h"

CTileMap::CTileMap(CWorld* InWorld)
:World(InWorld)
{
	for (std::vector<SEntityId>& EntityOccupants : TileOccupants)
	{
		EntityOccupants.reserve(NTileMapConstans::MAX_NUMBER_OF_OCCUPANTS);
	}

	CGraphicsSystem* const GraphicsSystem = World->GetSystemManager()->GetSystem<CGraphicsSystem>();
	TileTextureHash = GraphicsSystem->CreateTexture(NTileMapConstans::TILE_TEXURE_FILE_NAME);

	for (size_t x = 0; x < NTileMapConstans::MAP_SIZE.x; x++)
	{
		for (size_t y = 0; y < NTileMapConstans::MAP_SIZE.y; y++)
		{
			CreateTileEntity(CVector2f(x * NTileMapConstans::TILE_SIZE.x, y * NTileMapConstans::TILE_SIZE.y));
		}
	}
}


CTileMap::~CTileMap()
{
}

void CTileMap::CreateTileEntity(const CVector2f& InPosition)
{
	CEntityManager* EntityManager = World->GetSystemManager()->GetSystem<CEntityManager>();
	const SEntityId Entity = EntityManager->CreateEntity();

	CComponentPool<STransformComponent>* TransformComponentPool = NGetterUtils::GetComponent<STransformComponent>(World);
	CComponentPool<STextureComponent>* TextureComponentPool = NGetterUtils::GetComponent<STextureComponent>(World);

	STransformComponent NewTransformComponent;
	NewTransformComponent.Position = InPosition;
	TransformComponentPool->CreateInstance(Entity, std::move(NewTransformComponent));
	EntityManager->AddComponent(Entity, TransformComponentPool);

	STextureComponent NewTextureComponent;
	NewTextureComponent.Size = NTileMapConstans::TILE_SIZE;
	NewTextureComponent.TextureHash = TileTextureHash;
	TextureComponentPool->CreateInstance(Entity, std::move(NewTextureComponent));
	EntityManager->AddComponent(Entity, TextureComponentPool);

 	EntityManager->AddSystemBit(Entity, World->GetSystemTypeManager()->GetType(typeid(CGraphicsSystem)).GetBit());

	AddOccupant(GetTileIndexFromPosition(InPosition), Entity);
}

void CTileMap::AddOccupant(const STileIndex TileIndex, const SEntityId Entity)
{
	TileOccupants[TileIndex].push_back(Entity);
}

void CTileMap::RemoveOccupant(const STileIndex InTileIndex, const SEntityId Entity)
{
	const std::vector<SEntityId>::iterator it = std::find(TileOccupants[InTileIndex].begin(), TileOccupants[InTileIndex].end(), Entity);
	if (it != TileOccupants[InTileIndex].end())
	{
		TileOccupants[InTileIndex].erase(it);
	}
}

const STileIndex CTileMap::GetTileIndexFromPosition(const CVector2f& InPosition) const 
{
	CVector2i TilePosition;
	TilePosition.x = static_cast<int32>(InPosition.x / NTileMapConstans::TILE_SIZE.x);
	TilePosition.y = static_cast<int32>(InPosition.y / NTileMapConstans::TILE_SIZE.y);

	return GetTileIndex(TilePosition);
}

const STileIndex CTileMap::GetTileIndex(const CVector2i& TilePosition) const
{
	return TilePosition.x + TilePosition.y * NTileMapConstans::MAP_SIZE_Y;
}