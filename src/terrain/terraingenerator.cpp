#include "terraingenerator.h"
#include "noise/perlin.h"

#include <cmath>
#include <iostream>
#include <ostream>
#include "glm/glm.hpp"



// Helper for generateTerrain()
inline void addPointToVector(glm::vec3 point, std::vector<float>& vector) {
    vector.push_back(point.x);
    vector.push_back(point.y);
    vector.push_back(point.z);
}


// Constructor
TerrainGenerator::TerrainGenerator(int noiseMapResolution, int noiseMapNumGrids, int noiseMapNumOctaves,
                                   glm::vec3 terrainTranslation, float terrainExtent, int terrainTesselation, float maxHeight) :
    noiseMapResolution(noiseMapResolution), noiseMapNumGrids(noiseMapNumGrids), noiseMapNumOctaves(noiseMapNumOctaves),
    terrainTranslation(terrainTranslation), terrainExtent(terrainExtent), terrainTesselation(terrainTesselation), maxHeight(maxHeight),
    perlinNoiseGen(noiseMapResolution, noiseMapNumGrids, noiseMapNumOctaves)
{

}

// Destructor
TerrainGenerator::~TerrainGenerator(){}



std::vector<glm::vec2> TerrainGenerator::generatePlane() {

    // generate plane spanning [0, terrainExtent]^2 in the world space
    // translation to the vertex shader
    std::vector<glm::vec2> plane;
    const float d = 1.f / terrainTesselation;
    const int numSquaresPerSide = terrainExtent * terrainTesselation;
    plane.reserve(numSquaresPerSide * numSquaresPerSide * 6);

    float x = 0.f, z;
    for (int xi = 0; xi < numSquaresPerSide; xi++) {
        z = 0.f;
        for (int zi = 0; zi < numSquaresPerSide; zi++) {
            plane.push_back(glm::vec2(x, z));
            plane.push_back(glm::vec2(x+squareSize, z));
            plane.push_back(glm::vec2(x+squareSize, z+squareSize));

            plane.push_back(glm::vec2(x, z));
            plane.push_back(glm::vec2(x+squareSize, z+squareSize));
            plane.push_back(glm::vec2(x, z+squareSize));

            z += squareSize;
        }
        x += squareSize;
    }

    return plane;
}




// scaling version zhou
void TerrainGenerator::generateTerrain() {

    // base plane
    plane = generatePlane();

    // height map
    heightMap = perlinNoiseGen.generatePerlinNoise2D();

    // normal map
    normalMap = perlinNoiseGen.computePerlinNormal2D(heightMap, maxHeight);

}


//glm::vec3 TerrainGenerator::getPosition(int row, int col) {
//    float x = 1.0 * row / m_resolution ;
//    float y = 1.0 * col / m_resolution ;
//    float z = getHeight(row, col);
//    return glm::vec3(x,y,z);
//}
