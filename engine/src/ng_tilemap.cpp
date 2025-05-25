#include <tmxlite/Map.hpp>

#include "ngine/tilemap/tilemap.hpp"

ng::Tilemap::Tilemap(std::string_view path, Renderer& render, 
					 ImageShrinkFilter tilesetShrink, ImageEnlargeFilter tilesetEnlarge, unsigned int tilesetMipmaps,
					 float tileSize)
{
	tmx::Map map {};
	map.load(std::string {path});

	for(const auto& tileset : map.getTilesets())
	{
		Image tilesetImg {tileset.getImagePath(), tilesetShrink, tilesetEnlarge, 
						  ImageWrap::CLAMP_TO_EDGE, tilesetMipmaps};

		_tilesets.emplace_back(tilesetImg, tileset.getFirstGID(), tileset.getLastGID(),
							   tileset.getTileSize().x, tileset.getColumnCount(), 
							   tileset.getTileCount() / tileset.getColumnCount());
	}

	for(const auto& layer : map.getLayers())
	{
		const auto& tileLayer {layer->getLayerAs<tmx::TileLayer>()};
		_tileLayers.emplace_back(tileLayer, _tilesets, render, tileSize, map.getTileCount().x, map.getTileCount().y);
	}
}

void ng::Tilemap::draw(Renderer& render, const glm::mat4& transform) const
{
	for(const auto& tileLayer : _tileLayers)
	{
		tileLayer.draw(render, transform);
	}
}