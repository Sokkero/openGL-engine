
#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <utility>
#include <vector>

inline static std::vector<glm::ivec2> GetNeighborOffsets() {
    return {
                glm::ivec2(1.f, 1.f),   glm::ivec2(0.f, 1.f),  glm::ivec2(-1.f, 1.f), glm::ivec2(-1.f, 0.f),
                glm::ivec2(-1.f, -1.f), glm::ivec2(0.f, -1.f), glm::ivec2(1.f, -1.f), glm::ivec2(1.f, 0.f),
           };
}

enum TileTypeEnum
{
    undetermined = 0,
    deepWater = 1,
    shallowWater = 2,
    beach = 3,
    grass = 4,
    hill = 5,
    mountain = 6
};

inline static glm::vec3 EnumToColorValue(int tile)
{
    switch(tile)
    {
        case TileTypeEnum::deepWater:
            return glm::vec3(0.0f, 0.0f, 0.545f);
        case TileTypeEnum::shallowWater:
            return glm::vec3(0.678f, 0.847f, 0.902f);
        case TileTypeEnum::beach:
            return glm::vec3(1.0f, 1.0f, 0.0f);
        case TileTypeEnum::grass:
            return glm::vec3(0.0f, 0.502f, 0.0f);
        case TileTypeEnum::hill:
            return glm::vec3(0.5f, 0.2f, 0.f);
        case TileTypeEnum::mountain:
            return glm::vec3(0.6f, 0.f, 0.1f);
        default:
            assert(false);
    }
}

inline static std::vector<TileTypeEnum> GetAllTiles() { return { deepWater, shallowWater, beach, grass, hill, mountain }; }

struct BasicTileDataStruct
{
        const std::vector<TileTypeEnum> allowedNeighbors;
        const glm::vec3 tileColor;
        const int weight;

        BasicTileDataStruct(std::vector<TileTypeEnum> allowedTiles, glm::vec3 tileColor, int weight)
            : allowedNeighbors(std::move(allowedTiles))
            , tileColor(tileColor)
            , weight(weight){};
};

struct HillTileDataStruct : BasicTileDataStruct
{
        HillTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::grass, TileTypeEnum::hill, TileTypeEnum::mountain },
                      EnumToColorValue(TileTypeEnum::beach),
                      2
              ) {};
};

struct MountainTileDataStruct : BasicTileDataStruct
{
        MountainTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::mountain, TileTypeEnum::hill },
                      EnumToColorValue(TileTypeEnum::mountain),
                      1
              ) {};
};

struct BeachTileDataStruct : BasicTileDataStruct
{
        BeachTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::beach, TileTypeEnum::grass, TileTypeEnum::shallowWater },
                      EnumToColorValue(TileTypeEnum::beach),
                      2
              ) {};
};

struct ShallowWaterTileDataStruct : BasicTileDataStruct
{
        ShallowWaterTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::beach, TileTypeEnum::deepWater, TileTypeEnum::shallowWater },
                      EnumToColorValue(TileTypeEnum::shallowWater),
                      2
              ) {};
};

struct DeepWaterTileDataStruct : BasicTileDataStruct
{
        DeepWaterTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::shallowWater, TileTypeEnum::deepWater },
                      EnumToColorValue(TileTypeEnum::deepWater),
                      1
              ) {};
};

struct GrasTileDataStruct : BasicTileDataStruct
{
        GrasTileDataStruct()
            : BasicTileDataStruct({ TileTypeEnum::beach, TileTypeEnum::grass, TileTypeEnum::hill }, EnumToColorValue(TileTypeEnum::grass), 20) {
            };
};

inline static BasicTileDataStruct EnumToTileData(int tile)
{
    switch(tile)
    {
        case TileTypeEnum::deepWater:
            return DeepWaterTileDataStruct();
        case TileTypeEnum::shallowWater:
            return ShallowWaterTileDataStruct();
        case TileTypeEnum::beach:
            return BeachTileDataStruct();
        case TileTypeEnum::grass:
            return GrasTileDataStruct();
        case TileTypeEnum::hill:
            return HillTileDataStruct();
        case TileTypeEnum::mountain:
            return MountainTileDataStruct();
        default:
            assert(false);
    }
}

inline static void AddTileWeighting(std::vector<TileTypeEnum>& tiles)
{
    std::vector<TileTypeEnum> tilesToAdd;
    for(const auto& tile : tiles) {
        const BasicTileDataStruct data = EnumToTileData(tile);
        for(int i = 1; i < data.weight; ++i) {
            tilesToAdd.push_back(tile);
        }
    }

    for(const auto& tile : tilesToAdd) {
        tiles.push_back(tile);
    }
}
