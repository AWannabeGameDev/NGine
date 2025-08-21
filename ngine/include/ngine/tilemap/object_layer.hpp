#ifndef NG_OBJECT_LAYER_HPP
#define NG_OBJECT_LAYER_HPP

#include <tmxlite/ObjectGroup.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <variant>
#include <vector>

namespace ng::tiled
{

struct Rectangle
{
    glm::vec2 topLeft;
    float width;
    float height;
};

using Point = glm::vec2;

using Shape = std::variant<Rectangle, Point>;

enum class ShapeIndex
{
    RECTANGLE, POINT
};

struct Property
{
    std::string name;
    std::variant<bool, float, int, std::string, glm::vec4> value;
};

enum class PropertyIndex
{
    BOOL, FLOAT, INT, STRING, COLOR
};

struct Object
{
    Shape shape;
    std::vector<Property> properties;
};

class ObjectLayer
{
public :
    std::vector<Object> objects {};

    ObjectLayer(const tmx::ObjectGroup& objLayer, unsigned int mapWidth, unsigned int mapHeight, unsigned int mapTileSize);
};

}

#endif