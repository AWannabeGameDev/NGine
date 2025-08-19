#include "ngine/tilemap/object_layer.hpp"

ng::ObjectLayer::ObjectLayer(const tmx::ObjectGroup& objLayer, unsigned int mapWidth, unsigned int mapHeight, unsigned int mapTileSize)
{
    for(const auto& object : objLayer.getObjects())
    {
        switch(object.getShape())
        {
            case tmx::Object::Shape::Rectangle :
            {
                const tmx::FloatRect& tmxShape {object.getAABB()};
                objects.emplace_back(std::in_place_type<Rectangle>, 
                                     glm::vec2{tmxShape.left, mapHeight - tmxShape.top} / (float)mapTileSize,
                                     tmxShape.width / mapTileSize, tmxShape.height / mapTileSize);       
                
                break;
            }

            case tmx::Object::Shape::Point :
            {
                const tmx::Vector2f& tmxShape {object.getPosition()};
                objects.emplace_back(std::in_place_type<Point>, tmxShape.x / mapTileSize, (mapHeight - tmxShape.y) / mapTileSize);
                
                break;
            }
        }
    }
}