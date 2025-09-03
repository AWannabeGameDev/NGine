#ifndef NG_TILESET_HPP
#define NG_TILESET_HPP

#include <vector>
#include <span>

#include "ngine/renderer/image.hpp"
#include "ngine/renderer/animation.hpp"
#include "ngine/tilemap/tilemap_data.hpp"

namespace ng::tiled
{

class Tileset
{
	friend class Tilemap;
	friend class TileLayer;
	friend ng::ImageSampleData getTileSample(const Tileset& tileset, uint32_t localTileId);

private :
	uint32_t _firstGid;
	uint32_t _lastGid;
	unsigned int _tileLength;
	unsigned int _tileCountU;
	unsigned int _tileCountV;

	std::vector<SpecialTile> _specialTiles {};

public :
	Image image;
	
	Tileset(uint32_t firstGid, uint32_t lastGid, unsigned int tileLength, unsigned int tileCountU, unsigned int tileCountV, const Image& image);

	std::span<SpecialTile> getSpecialTiles();
	std::span<const SpecialTile> getSpecialTiles() const;

	unsigned int tileLength();
	unsigned int widthInTiles();
	unsigned int heightInTiles();
};

}

#endif