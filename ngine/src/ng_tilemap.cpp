#include <tmxlite/Map.hpp>

#include "ngine/tilemap/tilemap.hpp"

ng::tiled::Tilemap::Tilemap(std::string_view path, Renderer& render, ImageShrinkFilter tilesetShrink, ImageEnlargeFilter tilesetEnlarge, unsigned int tilesetMipmaps, float tileSize)
{
	tmx::Map map {};
	map.load(std::string {path});

	for(const tmx::Tileset& tileset : map.getTilesets())
	{
		Image tilesetImg 
		{
			tileset.getImagePath(), tilesetShrink, tilesetEnlarge, 
			ImageWrap::MIRRORED_REPEAT, tilesetMipmaps
		};

		_tilesets.emplace_back
		(
			tilesetImg, tileset.getFirstGID(), tileset.getLastGID(),
			tileset.getTileSize().x, tileset.getColumnCount(), 
			tileset.getTileCount() / tileset.getColumnCount()
		);
	}

	for(size_t layerIdx {0}; layerIdx < map.getLayers().size(); layerIdx++)
	{
		const auto& layer {map.getLayers()[layerIdx]};

		switch(layer->getType())
		{
			case tmx::Layer::Type::Tile :
			{
				const tmx::TileLayer& tileLayer {layer->getLayerAs<tmx::TileLayer>()};

				const tmx::Vector2u& tileCount {map.getTileCount()};
				_tileLayers.emplace_back
				(
					tileLayer, _tilesets, render, tileSize, 
					tileCount.x, tileCount.y, 
					(float)(map.getLayers().size() - layerIdx)
				);
				
				break;
			}

			case tmx::Layer::Type::Object :
			{
				const tmx::ObjectGroup& objLayer {layer->getLayerAs<tmx::ObjectGroup>()};

				const tmx::Vector2u& tileCount {map.getTileCount()};
				const tmx::Vector2u& tileSize {map.getTileSize()};
				objectLayers.emplace_back(objLayer, tileCount.x * tileSize.x, tileCount.y * tileSize.x, tileSize.x);
				
				break;
			}
		}
	}
}

void ng::tiled::Tilemap::draw(Renderer& render, const Prefab& quad, const glm::mat4& transform) const
{
	render.setGlobalTransform(transform);

	for(const TileLayer& tileLayer : _tileLayers)
	{
		tileLayer.draw(render, quad);
	}

	render.resetGlobalTransform();
}