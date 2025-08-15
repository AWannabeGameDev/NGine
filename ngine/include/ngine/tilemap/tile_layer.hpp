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
		// these are ints and not size_t's so that an empty tile can be represented by -1.
		int tileRepoIdx;
		int tileIdx;
	};

	const unsigned int _tileCountX;
	const unsigned int _tileCountY;

	std::vector<_TileRepo> _tileRepos {};

	// top to bottom, left to right
	std::vector<_TileReference> _tileMatrix {};

	size_t _getTilesetIndexFor(uint32_t gid, const std::vector<Tileset>& tilesets, size_t first, size_t last);

public : 
	// Assumes that `tilesets` is sorted in ascending order on firstGid.
	// `tilesets` must live at least as long as this TileLayer.
	// Only supports finite maps right now.
	// For internal use only, it is not intended for the user to call this constructor.
	TileLayer(const tmx::TileLayer& layer, const std::vector<Tileset>& tilesets, Renderer& render,
			  float tileLength, unsigned int tileCountX, unsigned int tileCountY, float layerDepth);

	// Bottom-left of the layer is rendered at the world-origin (unless the global transform has been set)
	void draw(Renderer& render, const Prefab& quad) const;
};

}

#endif