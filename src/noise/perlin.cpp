#include "perlin.h"
#include <random>
#include <chrono>


inline auto pos2DToIndex(auto x, auto y, auto xDim) {
    return x + xDim * y;
}

inline glm::vec2 indexToPos2D(auto index, auto xDim) {
    auto [y, x] = std::div(index, xDim);
    return {x, y};
}

Perlin::Perlin(size_t noiseResolution, size_t gridResolution, size_t numOctaves) :
    noiseResolution(noiseResolution), gridResolution(gridResolution), numOctaves(numOctaves)
{}

/* Generate (sideLength x sideLength) random unit vectors in R^2 */
std::vector<glm::vec2> Perlin::createRandomVectors2D(size_t sideLength) {
    // initialize U(0, 2*pi)
    std::mt19937_64 rng;
//    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    uint64_t timeSeed = 42;
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_real_distribution<float> U(0, glm::two_pi<float>());

    std::vector<glm::vec2> arr(sideLength * sideLength);

    for (auto &v : arr) {
        float theta = U(rng);
        v = { std::cos(theta), std::sin(theta) };
    }

    return arr;
}

float easing(float x0, float x1, float lambda) {
    float alpha = 3 * std::pow(lambda, 2) - 2 * std::pow(lambda, 3);
    return x0 + alpha * (x1 - x0);
}

float bilerpEase(float u, float v, float topleft, float topright, float bottomleft, float bottomright) {
    auto top = easing(topleft, topright, u);
    auto bottom = easing(bottomleft, bottomright, u);
    return easing(top, bottom, v);
}

float Perlin::getPerlinNoiseValue(float u, float v,
                                  const glm::vec2 &topleft,
                                  const glm::vec2 &topright,
                                  const glm::vec2 &bottomleft,
                                  const glm::vec2 &bottomright)
{
    float dotTL, dotTR, dotBL, dotBR;
    dotTL = glm::dot(glm::vec2(  u,   v), topleft);
    dotTR = glm::dot(glm::vec2(u-1,   v), topright);
    dotBL = glm::dot(glm::vec2(  u, v-1), bottomleft);
    dotBR = glm::dot(glm::vec2(u-1, v-1), bottomright);
    return bilerpEase(u, v, dotTL, dotTR, dotBL, dotBR);
}

float Perlin::getPerlinNoiseValue_f(float x_g, float y_g, int frequency,
                                    const std::vector<glm::vec2> &randomVectors) {
    x_g *= frequency;
    y_g *= frequency;
    int row_g = static_cast<int>(y_g) % gridResolution;  // row left
    int rowp1_g = static_cast<int>(y_g + 1) % gridResolution;  // row right
    int col_g = static_cast<int>(x_g) % gridResolution;  // col up
    int colp1_g = static_cast<int>(x_g + 1) % gridResolution;  // col down
    float u = x_g - std::floor(x_g);
    float v = y_g - std::floor(y_g);
    const glm::vec2 &topleft     = randomVectors[pos2DToIndex(col_g,   row_g,   gridResolution)];
    const glm::vec2 &topright    = randomVectors[pos2DToIndex(colp1_g, row_g,   gridResolution)];
    const glm::vec2 &bottomleft  = randomVectors[pos2DToIndex(col_g,   rowp1_g, gridResolution)];
    const glm::vec2 &bottomright = randomVectors[pos2DToIndex(colp1_g, rowp1_g, gridResolution)];

    float noise_f = getPerlinNoiseValue(u, v, topleft, topright, bottomleft, bottomright);
    return noise_f;
}

std::vector<float> Perlin::generatePerlinNoise2D() {
    const std::vector<glm::vec2> randomVectors = createRandomVectors2D(gridResolution);
    const float scalePixelToGrid = static_cast<float>(gridResolution) / static_cast<float>(noiseResolution);

    float noiseMin = INFINITY;
    float noiseMax = -INFINITY;
    std::vector<float> noiseMap(noiseResolution * noiseResolution);
    for (int row_p = 0; row_p < noiseResolution; row_p++) {
        float y_g = scalePixelToGrid * row_p;
        for (int col_p = 0; col_p < noiseResolution; col_p++) {
            float x_g = scalePixelToGrid * col_p;

            int freq = 1;
            float noiseVal = 0.f;
            for (int octave = 0; octave < numOctaves; octave++) {
                noiseVal += getPerlinNoiseValue_f(x_g, y_g, freq, randomVectors) / freq;
                freq *= 2;
            }
            int index_p = pos2DToIndex(col_p, row_p, noiseResolution);
            noiseMap[index_p] = noiseVal;

            noiseMin = std::min(noiseVal, noiseMin);
            noiseMax = std::max(noiseVal, noiseMax);
        }
    }

    for (auto &v : noiseMap)
        v = (v - noiseMin) / (noiseMax - noiseMin);

    return noiseMap;
}

glm::vec3 getPosition(const std::vector<float> &perlinHeight, int row, int col, int noiseResolution, float maxHeight) {
    float x, y, z;
    x = static_cast<float>(row) / noiseResolution;
    z = 1.f - static_cast<float>(col) / noiseResolution;  // flip z to match uv coords order

    col = (col + noiseResolution) % noiseResolution;
    row = (row + noiseResolution) % noiseResolution;
    y = perlinHeight[ pos2DToIndex(col, row, noiseResolution) ];  // zero to one
    y = y * maxHeight;  // zero to maxHeight
    return glm::vec3(x, y, z);
}

std::vector<glm::vec3> Perlin::computePerlinNormal2D(const std::vector<float> &perlinHeight, float maxHeight = 1.f) {

    std::vector<glm::vec3> normals;
    normals.reserve(noiseResolution * noiseResolution);
    for (int row = 0; row < noiseResolution; row++) {
        for (int col = 0; col < noiseResolution; col++) {

            // flip column number
            glm::vec3 v = getPosition(perlinHeight, row, col, noiseResolution, maxHeight);
            glm::vec3 p[8] = {getPosition(perlinHeight, row-1, col-1, noiseResolution, maxHeight) - v, getPosition(perlinHeight, row  , col-1, noiseResolution, maxHeight) - v,
                              getPosition(perlinHeight, row+1, col-1, noiseResolution, maxHeight) - v, getPosition(perlinHeight, row+1, col  , noiseResolution, maxHeight) - v,
                              getPosition(perlinHeight, row+1, col+1, noiseResolution, maxHeight) - v, getPosition(perlinHeight, row  , col+1, noiseResolution, maxHeight) - v,
                              getPosition(perlinHeight, row-1, col+1, noiseResolution, maxHeight) - v, getPosition(perlinHeight, row-1, col  , noiseResolution, maxHeight) - v};
            glm::vec3 normal(0.f);
            for (int i = 0; i < 8; i++)
                normal += glm::normalize( glm::cross(p[i], p[(i+1)%8]) );

            normal = glm::normalize(normal);
            normals.push_back(normal);
        }
    }

    return normals;
}
