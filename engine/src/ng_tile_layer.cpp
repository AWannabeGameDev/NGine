#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include "ngine/tilemap/tile_layer.hpp"
#include "ngine/renderer/default_models.hpp"
#include "ngine/util/transform.hpp"

size_t ng::TileLayer::_getTilesetIndexFor(uint32_t gid, const std::vector<Tileset>& tilesets, size_t first, size_t last)
{
	size_t middleIdx {(first + last) / 2};

	if(gid < tilesets[middleIdx].firstGid)
	{
		return _getTilesetIndexFor(gid, tilesets, first, last - middleIdx - 1);
	}
	else if(gid > tilesets[middleIdx].lastGid)
	{
		return _getTilesetIndexFor(gid, tilesets, first + middleIdx + 1, last);
	}
	else
	{
		return middleIdx;
	}
}

ng::TileLayer::TileLayer(const tmx::TileLayer& layer, const std::vector<Tileset>& tilesets, Renderer& render,
						 float tileLength, unsigned int tileCountX, unsigned int tileCountY, float layerDepth) :
	_tileCountX {tileCountX}, _tileCountY {tileCountY}
{
	_tileMatrix.reserve(layer.getTiles().size());

	std::unordered_map<uint32_t, size_t> tileRepoFirstGidToIdx {};

	for(size_t tileIdx {0}; tileIdx < layer.getTiles().size(); tileIdx++)
	{
		const tmx::TileLayer::Tile& tile {layer.getTiles()[tileIdx]};

		// empty tile
		if(tile.ID == 0)
		{
			_tileMatrix.emplace_back(-1, -1);
			continue;
		}

		const Tileset& tileset {tilesets[_getTilesetIndexFor(tile.ID, tilesets, 0, tilesets.size() - 1)]};
		size_t tileRepoIdx;

		auto found {tileRepoFirstGidToIdx.find(tileset.firstGid)};
		
		if(found != tileRepoFirstGidToIdx.end())
		{
			tileRepoIdx = found->second;
		}
		else
		{
			_tileRepos.emplace_back(&tileset, std::vector<ModelData> {});
			tileRepoFirstGidToIdx.try_emplace(tileset.firstGid, _tileRepos.size() - 1);
			tileRepoIdx = _tileRepos.size() - 1;
		}

		_TileRepo& tileRepo {_tileRepos[tileRepoIdx]};

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
			.position
			{
				((float)(tileIdx % tileCountX) + 0.5f) * tileLength,
				((float)tileCountY - (float)(tileIdx / tileCountX) - 1.0f + 0.5f) * tileLength,
				layerDepth
			},

			.scale {tileLength * tileFlipX, tileLength * tileFlipY, 1.0f}
		};

		unsigned int tileIdxInTileset {tile.ID - tileset.firstGid};

		glm::uvec2 tilesetSamplePosition
		{
			(tileIdxInTileset % tileset.tileCountU) * tileset.tileLength,
			(tileset.tileCountV - tileIdxInTileset / tileset.tileCountU - 1) * tileset.tileLength
		};

		ImageSampleData tileSampleData {tilesetSamplePosition, tileset.tileLength, tileset.tileLength};

		tileRepo.tileVector.emplace_back(tileTransform.generateMatrix(), tileSampleData);
		_tileMatrix.emplace_back((int)tileRepoIdx, (int)(tileRepo.tileVector.size() - 1));
	}
}

void ng::TileLayer::draw(Renderer& render, const Prefab& quad) const
{
	for(const auto& tileRepo : _tileRepos)
	{
		render.draw(quad, tileRepo.tileVector.data(), tileRepo.tileVector.size(), tileRepo.tileset->image);
	}
}