#ifndef TERRAINGEN_HPP
#define TERRAINGEN_HPP

#include <PerlinNoise.hpp>
#include <glm/glm.hpp>
#include <TerrainChunck.hpp>

class TerrainGen{
private:
    unsigned int seed, chunckSize;
    float vertexInterval;

public:
    //glm::vec3 globalPos;
    TerrainChunck *tca1, *tca2, *tca3;
    
    TerrainGen(unsigned pSeed){
        this->seed = pSeed;
        this->vertexInterval = 0.1;
        this->chunckSize = 1024;
    //    this->globalPos = glm::vec3(0.0f, 17.0f, -25.0f)
        tca1 = new TerrainChunck(this->vertexInterval, this->chunckSize, 0.0f, 0.0f);
        tca2 = new TerrainChunck(this->vertexInterval, this->chunckSize, 127.0f, 0.0f);
        tca3 = new TerrainChunck(this->vertexInterval, this->chunckSize, 130.0f, 129.0f);
        
    }
    
    void drawTerrain(){
        tca1->DrawChunck();
        tca2->DrawChunck();
        tca3->DrawChunck();
        
        
    }
    
    
};

#endif