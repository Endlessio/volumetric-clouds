#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "noise/perlin.h"


class TerrainGenerator
{
public:

// constructor and deconstructor
    TerrainGenerator() = default;
    TerrainGenerator(int noiseMapResolution, int noiseMapNumGrids, int noiseMapNumOctaves,
                     glm::vec3 terrainTranslation, float terrainExtent, int terrainTesselation, float maxHeight);
    ~TerrainGenerator();

// generator functions
    void generateTerrain();
    std::vector<glm::vec2> generatePlane();  // xz coords of the bottom plane

    // perlin noise related
    Perlin perlinNoiseGen;
    int noiseMapResolution;  // in px
    int noiseMapNumGrids;
    int noiseMapNumOctaves;

    // only dictates the plane
    glm::vec3 terrainTranslation;
    float terrainExtent;     // how big the plane is in world space unit
    int terrainTesselation;  // number of subdivided rectangle per unit length in world space
    float maxHeight;  // max height of the terrain in world space

    std::vector<glm::vec2> plane;

    // textures
    std::vector<float> heightMap;
    std::vector<glm::vec3> normalMap;




    int m_resolution, m_gridRes, m_numOctaves; // perlin noise related
    float m_xScale;
    float m_yScale;
    glm::vec3 translation;
    std::vector<float> height_data;
    std::vector<float> normal_data;
    std::vector<float> color_data;
    std::vector<float> xz_data;
};
