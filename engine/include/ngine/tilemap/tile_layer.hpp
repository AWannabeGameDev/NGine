#ifndef NG_TILE_LAYER_HPP
#define NG_TILE_LAYER_HPP

#include <tmxlite/TileLayer.hpp>

#include <vector>

#include "ngine/renderer/image.hpp"
#include "ngine/renderer/renderer_data.hpp"
#include "ngine/tilemap/tileset.hpp"
#include "ngine/renderer/renderer.hpp"

namespace ng
{

class TileLayer
{
	friend class Tilemap;

private :
	struct _TileRepo
	{
		const Tileset* tileset;
		std::vector<ModelData> tileVector;
	};

	struct _TileReference
	{
		size_t tileRepoIdx;
		size_t tileIdx;
	};

	const unsigned int _tileCountX;
	const unsigned int _tileCountY;

	std::vector<_TileRepo> _tileRepos {};

	// top to bottom, left to right
	std::vector<_TileReference> _tileMatrix {};

	Prefab _tileModel;

	size_t _getTilesetIndexFor(uint32_t gid, const std::vector<Tileset>& tilesets, size_t first, size_t last);

	// Assumes that `tilesets` is sorted in ascending order on firstGid.
	// `tilesets` must live at least as long as this TileLayer.
	TileLayer(const tmx::TileLayer& layer, const std::vector<Tileset>& tilesets, Renderer& render,
			  float tileLength, unsigned int tileCountX, unsigned int tileCountY);

public : 
	TileLayer(const TileLayer& other) = delete;
	const TileLayer& operator=(const TileLayer& other) = delete;

	void draw(Renderer& render);
};

}

#endif