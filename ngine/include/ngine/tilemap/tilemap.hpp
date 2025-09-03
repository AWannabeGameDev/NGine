#ifndef NG_TILEMAP_HPP
#define NG_TILEMAP_HPP

#include <vector>
#include <string_view>
#include <span>
#include <unordered_map>

#include "ngine/tilemap/tile_layer.hpp"
#include "ngine/tilemap/object_layer.hpp"

namespace ng::tiled
{

class Tilemap
{
private:
	std::vector<Tileset> _tilesets {};
	std::vector<TileLayer> _tileLayers {};
	std::vector<ObjectLayer> _objectLayers {};

	std::unordered_map<std::string_view, size_t> _nameToTileLayerIdx {};
	std::unordered_map<std::string_view, size_t> _nameToObjLayerIdx {};

public :
	// Only supports finite maps for now.
	// Render results are best with nearest-pixel filtering
	// Object coordinates are normalized so that the values are as if tile-size was 1 unit.
	Tilemap(std::string_view path, Renderer& render, ImageShrinkFilter tilesetShrink, ImageEnlargeFilter tilesetEnlarge, unsigned int tilesetMipmaps, float tileSize);

	std::span<TileLayer> getTileLayers();
	std::span<const TileLayer> getTileLayers() const;
	std::span<ObjectLayer> getObjectLayers();
	std::span<const ObjectLayer> getObjectLayers() const;

	TileLayer& getTileLayer(std::string_view name);
	const TileLayer& getTileLayer(std::string_view name) const;
	ObjectLayer& getObjectLayer(std::string_view name);
	const ObjectLayer& getObjectLayer(std::string_view name) const;

	void draw(Renderer& render, const Transform& transform = Transform {}) const;
};

}

#endif