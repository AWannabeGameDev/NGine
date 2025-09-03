#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include "ngine/tilemap/tile_layer.hpp"
#include "ngine/renderer/default_models.hpp"
#include "ngine/util/transform.hpp"
#include "ngine/tilemap/tilemap_util.hpp"

ng::tiled::TileLayer::TileRepo::TileRepo(Tileset* tileset) :
	tileset {tileset}
{}

ng::ModelData& ng::tiled::TileLayer::TileRepo::getTileModelData(size_t localId)
{
	return _tiles[_localIdToTileId[localId]];
}

const ng::ModelData& ng::tiled::TileLayer::TileRepo::getTileModelData(size_t localId) const
{
	return _tiles[_localIdToTileId[localId]];
}

size_t ng::tiled::TileLayer::_getTilesetIndexFor(uint32_t gid, const std::vector<Tileset>& tilesets, size_t first, size_t last)
{
	size_t middleIdx {(first + last) / 2};

	if(gid < tilesets[middleIdx]._firstGid)
	{
		return _getTilesetIndexFor(gid, tilesets, first, last - middleIdx - 1);
	}
	else if(gid > tilesets[middleIdx]._lastGid)
	{
		return _getTilesetIndexFor(gid, tilesets, first + middleIdx + 1, last);
	}
	else
	{
		return middleIdx;
	}
}

ng::tiled::TileLayer::TileLayer(const tmx::TileLayer& layer, std::vector<Tileset>& tilesets, std::string_view name, Renderer& render, float tileLength, unsigned int tileCountX, unsigned int tileCountY, float layerDepth) :
	_tileCountX {tileCountX}, _tileCountY {tileCountY},
	name {name}
{
	std::unordered_map<uint32_t, size_t> tileRepoFirstGidToIdx {};

	for(size_t tileIdx {0}; tileIdx < layer.getTiles().size(); tileIdx++)
	{
		const tmx::TileLayer::Tile& tile {layer.getTiles()[tileIdx]};

		// empty tile
		if(tile.ID == 0)
		{
			continue;
		}

		size_t tilesetIdx {_getTilesetIndexFor(tile.ID, tilesets, 0, tilesets.size() - 1)};
		Tileset& tileset {tilesets[tilesetIdx]};
		size_t tileRepoIdx;

		auto found {tileRepoFirstGidToIdx.find(tileset._firstGid)};
		
		if(found != tileRepoFirstGidToIdx.end())
		{
			tileRepoIdx = found->second;
		}
		else
		{
			_tileRepos.emplace_back(&tileset);
			tileRepoFirstGidToIdx.try_emplace(tileset._firstGid, _tileRepos.size() - 1);
			tileRepoIdx = _tileRepos.size() - 1;
		}

		TileRepo& tileRepo {_tileRepos[tileRepoIdx]};

		float tileFlipX {1.0f};
		float tileFlipY {1.0f};

		if(tile.flipFlags & (uint8_t)2)
		{
			tileFlipX *= -1.0f;
			tileFlipY *= -1.0f;
		}

		if(tile.flipFlags & (uint8_t)4)
		{
			tileFlipY *= -1.0f;
		}

		if(tile.flipFlags & (uint8_t)8)
		{
			tileFlipX *= -1.0f;
		}

		Transform tileTransform
		{
			.position {((float)(tileIdx % tileCountX) + 0.5f) * tileLength, ((float)tileCountY - (float)(tileIdx / tileCountX) - 1.0f + 0.5f) * tileLength, layerDepth},
			.scale {tileLength * tileFlipX, tileLength * tileFlipY, 1.0f}
		};

		uint32_t localId {tile.ID - tileset._firstGid};

		tileRepo._tiles.emplace_back
		(
			tileTransform, 
			getTileSample(tileset, localId)
		);

		if(localId >= tileRepo._localIdToTileId.size())
		{
			tileRepo._localIdToTileId.resize(localId + 1, -1);
		}

		tileRepo._localIdToTileId[localId] = tileRepo._tiles.size() - 1;
	}
}

std::span<ng::tiled::TileLayer::TileRepo> ng::tiled::TileLayer::getTileRepos()
{
	return std::span<TileRepo> {_tileRepos.begin(), _tileRepos.size()};
}

std::span<const ng::tiled::TileLayer::TileRepo> ng::tiled::TileLayer::getTileRepos() const
{
	return std::span<const TileRepo> {_tileRepos.begin(), _tileRepos.size()};
}

void ng::tiled::TileLayer::draw(Renderer& render) const
{
	for(const TileRepo& tileRepo : _tileRepos)
	{
		render.addModelsToBatch(tileRepo._tiles.data(), tileRepo._tiles.size());
		render.drawAndResetBatch(render.quad, tileRepo.tileset->image);
	}
}