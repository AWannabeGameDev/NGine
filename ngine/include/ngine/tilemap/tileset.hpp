#ifndef NG_TILESET_HPP
#define NG_TILESET_HPP

#include "ngine/renderer/image.hpp"

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
};

}

#endif