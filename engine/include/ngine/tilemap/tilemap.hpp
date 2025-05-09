#ifndef NG_TILEMAP_HPP
#define NG_TILEMAP_HPP

#include <vector>
#include <string_view>

#include "ngine/tilemap/tile_layer.hpp"

namespace ng
{

class Tilemap
{
private:
	std::vector<TileLayer> _tileLayers;

public :
	Tilemap(std::string_view path, Renderer& render);
};

}

#endif