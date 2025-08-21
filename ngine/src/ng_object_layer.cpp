#include "ngine/tilemap/object_layer.hpp"

ng::tiled::ObjectLayer::ObjectLayer(const tmx::ObjectGroup& objLayer, unsigned int mapWidth, unsigned int mapHeight, unsigned int mapTileSize)
{
    for(const tmx::Object& object : objLayer.getObjects())
    {
        std::vector<Property>* properties;

        switch(object.getShape())
        {
            case tmx::Object::Shape::Rectangle :
            {
                const tmx::FloatRect& tmxShape {object.getAABB()};

                properties = &objects.emplace_back(
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
            switch(property.getType())
            {
                case tmx::Property::Type::Boolean :
                    properties->emplace_back(std::move(property.getName()), property.getBoolValue());
                    break;

                case tmx::Property::Type::Float :
                    properties->emplace_back(std::move(property.getName()), property.getFloatValue());
                    break;
                    
                case tmx::Property::Type::Int :
                    properties->emplace_back(std::move(property.getName()), property.getIntValue());
                    break;

                case tmx::Property::Type::String :
                    properties->emplace_back(std::move(property.getName()), std::move(property.getStringValue()));
                    break;

                case tmx::Property::Type::Colour :
                {
                    const tmx::Colour& tmxColor {property.getColourValue()};
                    properties->emplace_back
                    (
                        std::move(property.getName()),
                        decltype(Property::value) {std::in_place_type<glm::vec4>, tmxColor.r, tmxColor.g, tmxColor.b, tmxColor.a}
                    );
                    break;
                }

                case tmx::Property::Type::File :
                    properties->emplace_back(std::move(property.getName()), std::move(property.getFileValue()));
                    break;
            }
        }
    }
}