/*
...
*/

#ifndef __MAP_TILE_H__
#define __MAP_TILE_H__

#include "glm/glm.hpp"
#include <vector>

class MapTile {


    //vertex data for vbo
    std::vector<float> m_Vertices; //in glBufferData, use &m_vertices.front()
    //std::vector<int> m_Indices; //in glBufferData, use &m_indices.front()

  public:
    glm::vec3 m_MercXYZ;
    MapTile(glm::vec3 m_MercXYZ);
    //Helper Functions
    bool setVBO(std::vector<float> vboData);
    glm::vec2 GetBoundingBox();
    //Maybe these should be moved 2 functions to sceneDefinition
    glm::vec3 MercToPix();
    glm::vec2 MercToLatLong();
    ~MapTile() {
        m_Vertices.clear();
    }
};

#endif