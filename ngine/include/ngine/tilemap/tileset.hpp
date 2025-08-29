#ifndef NG_TILESET_HPP
#define NG_TILESET_HPP

#include <vector>

#include "ngine/renderer/image.hpp"
#include "ngine/renderer/animation.hpp"
#include "ngine/tilemap/tilemap_data.hpp"

namespace ng::tiled
{

struct Tileset
{
	Image image;
	uint32_t firstGid;
	uint32_t lastGid;
	unsigned int tileLength;
	unsigned int tileCountU;
	unsigned int tileCountV;

	std::vector<SpecialTile> specialTiles;
};

}

#endif