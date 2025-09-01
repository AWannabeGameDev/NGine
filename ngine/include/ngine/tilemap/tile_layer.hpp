#ifndef NG_TILE_LAYER_HPP
#define NG_TILE_LAYER_HPP

#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include <vector>
#include <span>

#include "ngine/renderer/image.hpp"
#include "ngine/renderer/renderer_data.hpp"
#include "ngine/tilemap/tileset.hpp"
#include "ngine/renderer/renderer.hpp"
#include "ngine/renderer/animation.hpp"

namespace ng::tiled
{

class TileLayer
{
public :
	class TileRepo
	{
		friend class TileLayer;

	private :
		std::vector<ModelData> _tiles {};
		std::vector<int> _localIdToTileId {}; // int, not size_t so that empty localId's can be -1.

	public :
		Tileset* const tileset;

		TileRepo(Tileset* tileset);

		ModelData& getTileModelData(size_t localId);
		const ModelData& getTileModelData(size_t localId) const;
	};

private :
	const unsigned int _tileCountX;
	const unsigned int _tileCountY;

	std::vector<TileRepo> _tileRepos {};

	size_t _getTilesetIndexFor(uint32_t gid, const std::vector<Tileset>& tilesets, size_t first, size_t last);

public : 
	// Assumes that `tilesets` is sorted in ascending order on firstGid.
	// `tilesets` must live at least as long as this TileLayer.
	// Only supports finite maps right now.
	// For internal use only, it is not intended for the user to call this constructor.
	TileLayer(const tmx::TileLayer& layer, std::vector<Tileset>& tilesets, Renderer& render, float tileLength, unsigned int tileCountX, unsigned int tileCountY, float layerDepth);

	std::span<TileRepo> getTileRepos();
	std::span<const TileRepo> getTileRepos() const;

	// Bottom-left of the layer is rendered at the world-origin (unless the global transform has been set)
	void draw(Renderer& render, const Prefab& quad) const;
};

}

#endif