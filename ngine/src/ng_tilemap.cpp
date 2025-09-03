#include <tmxlite/Map.hpp>

#include "ngine/tilemap/tilemap.hpp"
#include "ngine/tilemap/tilemap_util.hpp"

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

		Tileset& newTileset {_tilesets.emplace_back
		(
			tileset.getFirstGID(), tileset.getLastGID(),
			tileset.getTileSize().x, tileset.getColumnCount(), 
			tileset.getTileCount() / tileset.getColumnCount(),
			tilesetImg
		)};

		for(const tmx::Tileset::Tile& specialTile : tileset.getTiles())
		{
			SpecialTile& newSpecialTile {newTileset._specialTiles.emplace_back
			(
				specialTile.ID, 
				std::vector<Property> {}, 
				std::optional<Animation> {}
			)};

			if(specialTile.animation.frames.size() != 0)
			{
				Animation& tileAnimation {newSpecialTile.animation.emplace(specialTile.animation.frames.size())};

				for(const auto& frame : specialTile.animation.frames)
				{
					tileAnimation.addFrame(getTileSample(newTileset, frame.tileID), frame.duration);
				}
			}

			if(specialTile.properties.size() != 0)
			{
				for(const tmx::Property& property : specialTile.properties)
				{
					newSpecialTile.properties.emplace_back(translateTmxProperty(property));
				}
			}
		}
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
				tileLayer, _tilesets, tileLayer.getName(), render, tileSize, 
				tileCount.x, tileCount.y, 
				(float)(map.getLayers().size() - layerIdx)
			);

			_nameToTileLayerIdx[_tileLayers.back().name] = _tileLayers.size() - 1;
			
			break;
		}

		case tmx::Layer::Type::Object :
		{
			const tmx::ObjectGroup& objLayer {layer->getLayerAs<tmx::ObjectGroup>()};

			const tmx::Vector2u& tileCount {map.getTileCount()};
			const tmx::Vector2u& tileSize {map.getTileSize()};
			_objectLayers.emplace_back(objLayer, objLayer.getName(), tileCount.x * tileSize.x, tileCount.y * tileSize.x, tileSize.x);
			
			_nameToObjLayerIdx[_objectLayers.back().name] = _objectLayers.size() - 1;

			break;
		}
		}
	}
}

std::span<ng::tiled::TileLayer> ng::tiled::Tilemap::getTileLayers()
{
	return std::span<TileLayer> {_tileLayers.begin(), _tileLayers.size()};
}

std::span<const ng::tiled::TileLayer> ng::tiled::Tilemap::getTileLayers() const
{
	return std::span<const TileLayer> {_tileLayers.begin(), _tileLayers.size()};
}

std::span<ng::tiled::ObjectLayer> ng::tiled::Tilemap::getObjectLayers()
{
	return std::span<ObjectLayer> {_objectLayers.begin(), _objectLayers.size()};
}

std::span<const ng::tiled::ObjectLayer> ng::tiled::Tilemap::getObjectLayers() const
{
	return std::span<const ObjectLayer> {_objectLayers.begin(), _objectLayers.size()};
}

ng::tiled::TileLayer& ng::tiled::Tilemap::getTileLayer(std::string_view name)
{
	return _tileLayers[_nameToTileLayerIdx.at(name)];
}

const ng::tiled::TileLayer& ng::tiled::Tilemap::getTileLayer(std::string_view name) const
{
	return _tileLayers[_nameToTileLayerIdx.at(name)];
}

ng::tiled::ObjectLayer& ng::tiled::Tilemap::getObjectLayer(std::string_view name)
{
	return _objectLayers[_nameToObjLayerIdx.at(name)];
}

const ng::tiled::ObjectLayer& ng::tiled::Tilemap::getObjectLayer(std::string_view name) const
{
	return _objectLayers[_nameToObjLayerIdx.at(name)];
}

void ng::tiled::Tilemap::draw(Renderer& render, const glm::mat4& transform) const
{
	render.setGlobalTransform(transform);

	for(const TileLayer& tileLayer : _tileLayers)
	{
		tileLayer.draw(render);
	}

	render.resetGlobalTransform();
}