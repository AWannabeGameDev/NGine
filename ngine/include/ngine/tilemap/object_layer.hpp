#ifndef NG_OBJECT_LAYER_HPP
#define NG_OBJECT_LAYER_HPP

#include <tmxlite/ObjectGroup.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <string>

#include "ngine/tilemap/tilemap_data.hpp"

namespace ng::tiled
{

class ObjectLayer
{
public :
    const std::string name;
    std::vector<Object> objects {};

    ObjectLayer(const tmx::ObjectGroup& objLayer, std::string_view name, unsigned int mapWidth, unsigned int mapHeight, unsigned int mapTileSize);
};

}

#endif