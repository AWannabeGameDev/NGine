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
						  ImageWrap::MIRRORED_REPEAT, tilesetMipmaps};

		_tilesets.emplace_back(tilesetImg, tileset.getFirstGID(), tileset.getLastGID(),
							   tileset.getTileSize().x, tileset.getColumnCount(), 
							   tileset.getTileCount() / tileset.getColumnCount());
	}

	for(size_t layerIdx {0}; layerIdx < map.getLayers().size(); layerIdx++)
	{
		const auto& layer {map.getLayers()[layerIdx]};

		switch(layer->getType())
		{
			case tmx::Layer::Type::Tile :
			{
				const tmx::TileLayer& tileLayer {layer->getLayerAs<tmx::TileLayer>()};
				_tileLayers.emplace_back(tileLayer, _tilesets, render, tileSize, 
										map.getTileCount().x, map.getTileCount().y, 
										(float)(map.getLayers().size() - layerIdx));
				
				break;
			}

			case tmx::Layer::Type::Object :
			{
				const tmx::ObjectGroup& objLayer {layer->getLayerAs<tmx::ObjectGroup>()};
				_objLayers.emplace_back(objLayer);
				
				break;
			}
		}
	}
}

void ng::Tilemap::draw(Renderer& render, const Prefab& quad, const glm::mat4& transform) const
{
	render.setGlobalTransform(transform);

	for(const auto& tileLayer : _tileLayers)
	{
		tileLayer.draw(render, quad);
	}

	render.resetGlobalTransform();
}