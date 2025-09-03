#include "ngine/tilemap/object_layer.hpp"
#include "ngine/tilemap/tilemap_util.hpp"

ng::tiled::ObjectLayer::ObjectLayer(const tmx::ObjectGroup& objLayer, std::string_view name, unsigned int mapWidth, unsigned int mapHeight, unsigned int mapTileSize) :
    name {name}
{
    for(const tmx::Object& object : objLayer.getObjects())
    {
        std::vector<Property>* properties;

        switch(object.getShape())
        {
        case tmx::Object::Shape::Rectangle :
        {
            const tmx::FloatRect& tmxShape {object.getAABB()};

            properties = &objects.emplace_back
            (
                Shape {std::in_place_type<Rectangle>, glm::vec2{tmxShape.left, mapHeight - tmxShape.top} / (float)mapTileSize, tmxShape.width / mapTileSize, tmxShape.height / mapTileSize},
                std::vector<Property> {}
            ).properties;
            
            break;
        }

        case tmx::Object::Shape::Point :
        {
            const tmx::Vector2f& tmxShape {object.getPosition()};

            properties = &objects.emplace_back
            (
                Shape {std::in_place_type<Point>, tmxShape.x / mapTileSize, (mapHeight - tmxShape.y) / mapTileSize},
                std::vector<Property> {}
            ).properties;
            
            break;
        }
        }

        for(const tmx::Property& property : object.getProperties())
        {
            properties->emplace_back(translateTmxProperty(property));
        }
    }
}