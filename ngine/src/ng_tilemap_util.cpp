#include "ngine/tilemap/tilemap_util.hpp"

ng::tiled::Property ng::tiled::translateTmxProperty(const tmx::Property& tmxProperty)
{
    switch(tmxProperty.getType())
    {
    case tmx::Property::Type::Boolean :
        return ng::tiled::Property {std::move(tmxProperty.getName()), tmxProperty.getBoolValue()};

    case tmx::Property::Type::Float :
        return ng::tiled::Property {std::move(tmxProperty.getName()), tmxProperty.getFloatValue()};
        
    case tmx::Property::Type::Int :
        return ng::tiled::Property {std::move(tmxProperty.getName()), tmxProperty.getIntValue()};

    case tmx::Property::Type::String :
        return ng::tiled::Property {std::move(tmxProperty.getName()), std::move(tmxProperty.getStringValue())};

    case tmx::Property::Type::Colour :
    {
        const tmx::Colour& tmxColor {tmxProperty.getColourValue()};
        return ng::tiled::Property 
        {
            std::move(tmxProperty.getName()),
            decltype(ng::tiled::Property::value) {std::in_place_type<glm::vec4>, tmxColor.r, tmxColor.g, tmxColor.b, tmxColor.a}
        };
    }

    case tmx::Property::Type::File :
        return ng::tiled::Property {std::move(tmxProperty.getName()), std::move(tmxProperty.getFileValue())};
    }
}

ng::ImageSampleData ng::tiled::getTileSample(const ng::tiled::Tileset& tileset, uint32_t localTileId)
{
    glm::uvec2 tilesetSamplePosition
    {
        (localTileId % tileset._tileCountU) * tileset._tileLength,
        (tileset._tileCountV - localTileId / tileset._tileCountU - 1) * tileset._tileLength
    };

    return ng::ImageSampleData {tilesetSamplePosition, tileset._tileLength, tileset._tileLength};
}