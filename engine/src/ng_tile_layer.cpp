#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include "ngine/tilemap/tile_layer.hpp"
#include "ngine/renderer/default_models.hpp"

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
						 float tileLength, unsigned int tileCountX, unsigned int tileCountY) :
	_tileCountX {tileCountX}, _tileCountY {tileCountY},
	_tileModel {render.newPrefab(quadVertices.data(), quadVertices.size(), quadIndices.data(), quadIndices.size())}
{
	_tileMatrix.reserve(layer.getTiles().size());

	std::unordered_map<uint32_t, size_t> tileRepoFirstGidToIdx {};

	for(size_t tileIdx {0}; tileIdx < layer.getTiles().size(); tileIdx++)
	{
		const tmx::TileLayer::Tile& tile {layer.getTiles()[tileIdx]};
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

		glm::vec3 tilePosition
		{
			((float)(tileIdx % tileCountX) + 0.5f) * tileLength,
			((float)tileCountY - (float)(tileIdx / tileCountX) - 1.0f + 0.5f) * tileLength,
			0.0f
		};

		glm::mat4 tileTransform {1.0f};
		tileTransform = glm::scale(tileTransform, glm::vec3 {tileLength, tileLength, 1.0f});
		tileTransform = glm::translate(tileTransform, tilePosition);

		unsigned int tileIdxInTileset {tile.ID - tileset.firstGid};

		glm::ivec2 tilesetSamplePosition
		{
			(tileIdxInTileset % tileset.tileCountU) * tileset.tileLength,
			(tileset.tileCountV - tileIdxInTileset / tileset.tileCountU - 1) * tileset.tileLength
		};

		ImageSampleData tileSampleData {tilesetSamplePosition, tileset.tileLength, tileset.tileLength};

		_tileRepos[tileRepoIdx].tileVector.emplace_back(tileTransform, tileSampleData);
		_tileMatrix.emplace_back(tileRepoIdx, _tileRepos[tileRepoIdx].tileVector.size() - 1);
	}
}

void ng::TileLayer::draw(Renderer& render, const glm::mat4& transform) const
{
	render.setGlobalTransform(transform);

	for(const auto& tileRepo : _tileRepos)
	{
		render.draw(_tileModel, tileRepo.tileVector.data(), tileRepo.tileVector.size(), tileRepo.tileset->image);
	}

	render.resetGlobalTransform();
}