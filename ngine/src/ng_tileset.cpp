#include "ngine/tilemap/tileset.hpp"

ng::tiled::Tileset::Tileset(uint32_t firstGid, uint32_t lastGid, unsigned int tileLength, unsigned int tileCountU, unsigned int tileCountV, const Image& image) :
    _firstGid {firstGid}, _lastGid {lastGid},
    _tileLength {tileLength}, _tileCountU {tileCountU}, _tileCountV {tileCountV},
    image {image}
{}

std::span<ng::tiled::SpecialTile> ng::tiled::Tileset::getSpecialTiles()
{
    return std::span<SpecialTile> {_specialTiles.begin(), _specialTiles.size()};
}

std::span<const ng::tiled::SpecialTile> ng::tiled::Tileset::getSpecialTiles() const
{
    return std::span<const SpecialTile> {_specialTiles.begin(), _specialTiles.size()};
}

unsigned int ng::tiled::Tileset::tileLength()
{
    return _tileLength;
}

unsigned int ng::tiled::Tileset::widthInTiles()
{
    return _tileCountU;
}

unsigned int ng::tiled::Tileset::heightInTiles()
{
    return _tileCountV;
}