#ifndef NG_OBJECT_LAYER_HPP
#define NG_OBJECT_LAYER_HPP

#include <tmxlite/ObjectGroup.hpp>
#include <glm/vec2.hpp>

#include <variant>
#include <vector>

namespace ng
{

struct Rectangle
{
    glm::vec2 topLeft;
    float width;
    float height;
};

using Point = glm::vec2;

using Shape = std::variant<Rectangle, Point>;

class ObjectLayer
{
private :
    std::vector<Shape> _objects {}; 

public :
    ObjectLayer(const tmx::ObjectGroup& objLayer);
};

}

#endif