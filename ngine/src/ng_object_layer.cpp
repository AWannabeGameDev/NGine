#include "ngine/tilemap/object_layer.hpp"

ng::ObjectLayer::ObjectLayer(const tmx::ObjectGroup& objLayer)
{
    for(const auto& object : objLayer.getObjects())
    {
        switch(object.getShape())
        {
            case tmx::Object::Shape::Rectangle :
            {
                const tmx::FloatRect& tmxShape {object.getAABB()};
                objects.emplace_back(std::in_place_type<Rectangle>, 
                                     glm::vec2{tmxShape.left, tmxShape.top},
                                     tmxShape.width, tmxShape.height);       
                
                break;
            }

            case tmx::Object::Shape::Point :
            {
                const tmx::Vector2f& tmxShape {object.getPosition()};
                objects.emplace_back(std::in_place_type<Point>, tmxShape.x, tmxShape.y);
                
                break;
            }
        }
    }
}