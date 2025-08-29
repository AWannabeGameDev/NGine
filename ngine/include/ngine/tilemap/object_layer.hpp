#ifndef NG_OBJECT_LAYER_HPP
#define NG_OBJECT_LAYER_HPP

#include <tmxlite/ObjectGroup.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <variant>
#include <vector>

#include "ngine/tilemap/tilemap_data.hpp"

namespace ng::tiled
{

class ObjectLayer
{
public :
    std::vector<Object> objects {};

    ObjectLayer(const tmx::ObjectGroup& objLayer, unsigned int mapWidth, unsigned int mapHeight, unsigned int mapTileSize);
};

}

#endif