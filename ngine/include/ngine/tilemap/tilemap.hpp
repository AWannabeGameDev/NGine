#ifndef NG_TILEMAP_HPP
#define NG_TILEMAP_HPP

#include <vector>
#include <string_view>

#include "ngine/tilemap/tile_layer.hpp"
#include "ngine/tilemap/object_layer.hpp"

namespace ng
{

class Tilemap
{
private:
	std::vector<Tileset> _tilesets {};
	std::vector<TileLayer> _tileLayers {};

public :
	std::vector<ObjectLayer> objectLayers {};

	// Only supports finite maps for now.
	// Render results are best with nearest-pixel filtering
	// Object coordinates are normalized so that tile-size is 1 unit.
	Tilemap(std::string_view path, Renderer& render, 
			ImageShrinkFilter tilesetShrink, ImageEnlargeFilter tilesetEnlarge, unsigned int tilesetMipmaps,
			float tileSize);

	void draw(Renderer& render, const Prefab& quad, const glm::mat4& transform = glm::mat4 {1.0f}) const;
};

}

#endif