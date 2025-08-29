#ifndef NG_TILEMAP_UTIL_HPP
#define NG_TILEMAP_UTIL_HPP

#include <tmxlite/Property.hpp>

#include "ngine/tilemap/tilemap_data.hpp"
#include "ngine/renderer/renderer_data.hpp"
#include "ngine/tilemap/tileset.hpp"

namespace ng::tiled
{

Property translateTmxProperty(const tmx::Property& tmxProperty);
ImageSampleData getTileSample(const Tileset& tileset, uint32_t localTileId);

}

#endif