
#include <glm/ext/vector_int2.hpp>
#include <glm/vec3.hpp>
#include <utility>
#include <vector>

enum TileTypeEnum
{
    undetermined = 0,
    deepWater = 1,
    shallowWater = 2,
    beach = 3,
    grass = 4
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
        case TileTypeEnum::undetermined:
            return glm::vec3(1.0f, 0.f, 0.f);
        default:
            assert(false);
    }
}

inline static std::vector<TileTypeEnum> GetAllTiles()
{
    return { deepWater, shallowWater, beach, grass };
}

struct BasicTileDataStruct
{
        const std::vector<TileTypeEnum> allowedNeighbors;
        const glm::vec3 tileColor;

        BasicTileDataStruct(std::vector<TileTypeEnum> allowedTiles, glm::vec3 tileColor)
            : allowedNeighbors(std::move(allowedTiles))
            , tileColor(tileColor) {};
};

struct BeachTileDataStruct : BasicTileDataStruct
{
        BeachTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::beach, TileTypeEnum::grass, TileTypeEnum::shallowWater },
                      EnumToColorValue(TileTypeEnum::beach)
              ) {};
};

struct ShallowWaterTileDataStruct : BasicTileDataStruct
{
        ShallowWaterTileDataStruct()
            : BasicTileDataStruct(
                      { TileTypeEnum::beach, TileTypeEnum::deepWater, TileTypeEnum::shallowWater },
                      EnumToColorValue(TileTypeEnum::shallowWater)
              ) {};
};

struct DeepWaterTileDataStruct : BasicTileDataStruct
{
        DeepWaterTileDataStruct()
            : BasicTileDataStruct({ TileTypeEnum::shallowWater, TileTypeEnum::deepWater }, EnumToColorValue(TileTypeEnum::deepWater)) {};
};

struct GrasTileDataStruct : BasicTileDataStruct
{
        GrasTileDataStruct() : BasicTileDataStruct({ TileTypeEnum::beach, TileTypeEnum::grass }, EnumToColorValue(TileTypeEnum::grass)) {};
};

struct UndeterminedTileDataStruct : BasicTileDataStruct
{
        UndeterminedTileDataStruct() : BasicTileDataStruct({ TileTypeEnum::beach, TileTypeEnum::grass, TileTypeEnum::deepWater, TileTypeEnum::shallowWater }, EnumToColorValue(TileTypeEnum::undetermined)) {};
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
        case TileTypeEnum::undetermined:
            return UndeterminedTileDataStruct();
    }
}


// Careful!! These could brake the algorithm if wrong
inline static std::vector<std::pair<glm::ivec2, TileTypeEnum>> GetPredeterminedTiles()
{
    return {
        { glm::ivec2(6, 6), TileTypeEnum::grass },
        { glm::ivec2(4, 4), TileTypeEnum::grass },
        { glm::ivec2(0, 0), TileTypeEnum::deepWater },
        { glm::ivec2(1, 0), TileTypeEnum::deepWater },
        { glm::ivec2(2, 0), TileTypeEnum::deepWater },
        { glm::ivec2(3, 0), TileTypeEnum::deepWater },
        { glm::ivec2(4, 0), TileTypeEnum::deepWater },
        { glm::ivec2(5, 0), TileTypeEnum::deepWater },
        { glm::ivec2(6, 0), TileTypeEnum::deepWater },
        { glm::ivec2(7, 0), TileTypeEnum::deepWater },
        { glm::ivec2(8, 0), TileTypeEnum::deepWater },
        { glm::ivec2(9, 0), TileTypeEnum::deepWater },
        { glm::ivec2(9, 1), TileTypeEnum::deepWater },
        { glm::ivec2(9, 2), TileTypeEnum::deepWater },
        { glm::ivec2(9, 3), TileTypeEnum::deepWater },
        { glm::ivec2(9, 4), TileTypeEnum::deepWater },
        { glm::ivec2(9, 5), TileTypeEnum::deepWater },
        { glm::ivec2(9, 6), TileTypeEnum::deepWater },
        { glm::ivec2(9, 7), TileTypeEnum::deepWater },
        { glm::ivec2(9, 8), TileTypeEnum::deepWater },
        { glm::ivec2(9, 9), TileTypeEnum::deepWater },
        { glm::ivec2(8, 9), TileTypeEnum::deepWater },
        { glm::ivec2(7, 9), TileTypeEnum::deepWater },
        { glm::ivec2(6, 9), TileTypeEnum::deepWater },
        { glm::ivec2(5, 9), TileTypeEnum::deepWater },
        { glm::ivec2(4, 9), TileTypeEnum::deepWater },
        { glm::ivec2(3, 9), TileTypeEnum::deepWater },
        { glm::ivec2(2, 9), TileTypeEnum::deepWater },
        { glm::ivec2(1, 9), TileTypeEnum::deepWater },
        { glm::ivec2(0, 9), TileTypeEnum::deepWater },
        { glm::ivec2(0, 8), TileTypeEnum::deepWater },
        { glm::ivec2(0, 7), TileTypeEnum::deepWater },
        { glm::ivec2(0, 6), TileTypeEnum::deepWater },
        { glm::ivec2(0, 5), TileTypeEnum::deepWater },
        { glm::ivec2(0, 4), TileTypeEnum::deepWater },
        { glm::ivec2(0, 3), TileTypeEnum::deepWater },
        { glm::ivec2(0, 2), TileTypeEnum::deepWater },
        { glm::ivec2(0, 1), TileTypeEnum::deepWater },
    };
}