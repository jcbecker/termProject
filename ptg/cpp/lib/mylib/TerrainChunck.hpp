#ifndef TERRAICHUNCK_HPP
#define TERRAICHUNCK_HPP

#include <cstdio>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <VertexDescription.hpp>
#include <vector>
#include <PerlinNoise.hpp>
#include <BiomeDescription.hpp>

struct HVertexStatus{
    float BaseH;
    float FinalH;
    glm::vec3 ColorH;
    BiomeType BioTp;
};

struct BorderHelper{
    bool isBorder;
    bool borderAbove;
    double pDistToBorder;
};

class TerrainChunck{
private:
    unsigned int gridSize, seed;
    float vertexInterval;
    float xi, zi;//xf = xi + gridSize * vertexInterval, zf = zi + gridSize * vertexInterval;
    unsigned int VBO, EBO;
    siv::PerlinNoise pNoise;
    int auxOctaves;
    double auxFreq;
    unsigned int biomeSize, biomeBorderLen;
public:
    unsigned int VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    TerrainChunck(unsigned int pseed, float pvertexInterval, unsigned int pgridSize, float pxi, float pzi, unsigned int pBiosz, unsigned int pBioborderlen){
        this->seed = pseed;
        this->vertexInterval = pvertexInterval;
        this->gridSize = pgridSize;
        this->xi = pxi;
        this->zi = pzi;
        this->pNoise.reseed(seed);
        this->biomeSize = pBiosz;
        this->biomeBorderLen = pBioborderlen;
        this->auxOctaves = 8;
        this->auxFreq = 16;
        this->vertices.reserve(this->gridSize * this->gridSize);
        this->indices.reserve(this->gridSize-1 * this->gridSize-1);
        this->genVectors();
        this->setUpGLBuffers();
    }
    
private:
    void genVectors(){
        float maxHTest = -10.0f, minHTest= 100.0f;
        float maxXTest = -1000000.0f, minXTest= 100000.0f;
        
        Vertex ijvertex;
        HVertexStatus heightaux;
        for(unsigned int i=0; i<this->gridSize; i++){
            for(unsigned int j=0; j<this->gridSize; j++){
                heightaux = vertexValuation(this->xi + (float)i, this->zi + (float)j);
                ijvertex.Position = glm::vec3(this->vertexInterval * i, heightaux.FinalH, this->vertexInterval * j);
                ijvertex.Color = heightaux.ColorH;//DEBUG: preciso colocar uma cor descente
                this->vertices.push_back(ijvertex);
                
                if(heightaux.BaseH > maxHTest)
                    maxHTest = heightaux.BaseH;
                if(heightaux.BaseH < minHTest)
                    minHTest = heightaux.BaseH;
                
                
                if(this->xi + (float)i > maxXTest)
                    maxXTest = this->xi + (float)i;
                if(this->xi + (float)i < minXTest)
                    minXTest = this->xi + (float)i;
                
            }
        }
        
        printf("Maior valor Base: %f    Menor: %f\n", maxHTest, minHTest);
        printf("Maior valor Base: %f    Menor: %f\n", maxXTest, minXTest);
        
        
        for (unsigned int i=0; i < this->gridSize-1; i++){//vertex i
            for(unsigned int j=0; j < this->gridSize-1; j++){
                this->indices.push_back(i*this->gridSize + j);//first triangle
                this->indices.push_back(i*this->gridSize + j+1);
                this->indices.push_back((i+1)*this->gridSize + j);
                
                this->indices.push_back((i+1) * this->gridSize + j+1);//second triangle
                this->indices.push_back(i * this->gridSize + j+1);
                this->indices.push_back((i+1) * this->gridSize + j);
                
            }
        }
    }
    
    void setUpGLBuffers(){
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
        
        //Vertex Position
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        //Vertex Color
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
        
        glBindVertexArray(0);
    }
    
    HVertexStatus vertexValuation(float x, float z){
        double auxfxz = (double) this->gridSize/this->auxFreq;
        HVertexStatus r;
        float auxHColor;
        
        
        r.BioTp = getBiomeXZ(x, z);
        
        r.BaseH = (float) pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, this->auxOctaves);
        
        
        auxHColor = glm::clamp((r.BaseH+1.0f)/2.0f, 0.0f, 1.0f);
        r.ColorH = glm::vec3(auxHColor, auxHColor, auxHColor);
        
        
        
        if(r.BioTp == PLAINS){
            r.FinalH = (r.BaseH)*10;
            r.ColorH.r = 1;
        }else if(r.BioTp == MONTAINS){
            r.FinalH = (r.BaseH)*35;
            r.ColorH.b = 1;
            
        }else if(r.BioTp == VALLEY){
            r.FinalH = glm::pow(1.5, (r.BaseH +0.4)*8);
            r.ColorH.g = 1;
        }
        
        BorderHelper someAuxforX = borderTest(x), someAuxforZ = borderTest(z);
        
        if(someAuxforX.isBorder){
            if(someAuxforX.borderAbove){
                r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), r.ColorH,  someAuxforX.pDistToBorder);
            }else{
                r.ColorH = glm::mix(glm::vec3(1.0, 1.0, 1.0), r.ColorH,  someAuxforX.pDistToBorder);
            }
        }
        
        if(someAuxforZ.isBorder){
            if(someAuxforZ.borderAbove){
                r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), r.ColorH, someAuxforZ.pDistToBorder);
            }else{
                r.ColorH = glm::mix(glm::vec3(1.0, 1.0, 1.0), r.ColorH, someAuxforZ.pDistToBorder);
            }
        }
        
        return r;
    }
    
    
    BorderHelper borderTest(float k){
        bool negativeFlag;
        long int kaux;
        negativeFlag = k < 0.0 ? true : false;
        kaux = negativeFlag? k*-1: k;
        long int nearborder = ((kaux / this->biomeSize) + 1) * this->biomeSize;
        long int nearborder2 = ((kaux / this->biomeSize)) * this->biomeSize;
        
        if(negativeFlag){
            nearborder *= -1;
            nearborder2 *= -1;
        }
        
        BorderHelper borderRes;
        if(glm::abs(nearborder - k) < this->biomeBorderLen){
            borderRes.isBorder = true;
            borderRes.borderAbove = true;
            borderRes.pDistToBorder = glm::abs(nearborder - k) / this->biomeBorderLen;
        }else if(glm::abs(nearborder2 - k) < this->biomeBorderLen){
            borderRes.isBorder = true;
            borderRes.borderAbove = false;
            borderRes.pDistToBorder = glm::abs(nearborder2 - k) / this->biomeBorderLen;
        }else{
            borderRes.isBorder = false;
        }
        
        if(negativeFlag){
            borderRes.borderAbove = !borderRes.borderAbove;
        }
        return borderRes;
    }
    
    BiomeType getBiomeXZ(double px, double pz){
        double dxs = (px/(double)this->biomeSize);
        double dzs = (pz/(double)this->biomeSize);
        if(dxs < 0) dxs -=1.0;
        if(dzs < 0) dzs -=1.0;
        
        dxs = glm::trunc(dxs);
        dzs = glm::trunc(dzs);
        
        
        double areaValue = pNoise.octaveNoise0_1(dxs + 0.3, dzs+ 0.3, 1);
        areaValue = glm::clamp(areaValue, 0.0, 1.0);
        return (BiomeType) (areaValue * BIOMESN);
    }
    
    
public:
    void DrawChunck(){
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
    void shutDown(){
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }
    
    glm::vec3 getInitialXZ(){
        return glm::vec3(this->xi * this->vertexInterval, 0.0f, this->zi * this->vertexInterval);
    }
    
};

#endif