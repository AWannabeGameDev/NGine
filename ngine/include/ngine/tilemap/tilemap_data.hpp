#ifndef NG_TILEMAP_DATA_HPP
#define NG_TILEMAP_DATA_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <variant>
#include <string>
#include <optional>

#include "ngine/renderer/animation.hpp"

namespace ng::tiled
{

struct Property
{
    std::string name;
    std::variant<bool, float, int, std::string, glm::vec4> value;
};

enum class PropertyIndex
{
    BOOL, FLOAT, INT, STRING, COLOR
};
   
struct SpecialTile
{
    uint32_t localId;
    std::vector<Property> properties;
    std::optional<Animation> animation;
};

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

struct Object
{
    Shape shape;
    std::vector<Property> properties;
};

};

#endif