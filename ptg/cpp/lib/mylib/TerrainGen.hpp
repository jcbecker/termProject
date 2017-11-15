#ifndef TERRAINGEN_HPP
#define TERRAINGEN_HPP

#include <glm/glm.hpp>
#include <TerrainChunck.hpp>
#include <shader.hpp>


class TerrainGen{
private:
    unsigned int seed, chunckSize;
    float vertexInterval;
    unsigned int biomeSize, borderLen;
    Shader sProgram;
public:
    //glm::vec3 globalPos;
    TerrainChunck *tca1;
    TerrainChunck *tca2;
    
    
    TerrainGen(unsigned int pSeed, unsigned int psp):sProgram(psp){
        this->seed = pSeed;
        this->vertexInterval = 0.1;
        this->chunckSize = 1024*2;
        this->biomeSize = 512;
        this->borderLen = 64;
    //    this->globalPos = glm::vec3(0.0f, 17.0f, -25.0f)
        tca1 = new TerrainChunck(this->seed, this->vertexInterval, this->chunckSize, this->chunckSize/2.0f*-1.0f,  this->chunckSize/2.0f*-1.0f, this->biomeSize, this->borderLen);
        tca2 = new TerrainChunck(this->seed, this->vertexInterval, this->chunckSize, this->chunckSize*-1.0f,  this->chunckSize*-1.0f, this->biomeSize, this->borderLen);
        
    }
    
    void drawTerrain(){
        this->sProgram.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, tca1->getInitialXZ());
        this->sProgram.setMat4("model", model);
        tca1->DrawChunck();
        
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, tca2->getInitialXZ());
        this->sProgram.setMat4("model", model);
        tca2->DrawChunck();
        
    }
    
    void shutDown(){
        tca1->shutDown();
        tca2->shutDown();
    }
    
    
};

#endif