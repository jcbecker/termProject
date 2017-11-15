#ifndef TERRAICHUNCK_HPP
#define TERRAICHUNCK_HPP

#include <cstdio>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <VertexDescription.hpp>
#include <vector>
#include <PerlinNoise.hpp>

struct HPair{
    float BaseH;
    float FinalH;
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
public:
    unsigned int VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    TerrainChunck(unsigned int pseed, float pvertexInterval, unsigned int pgridSize, float pxi, float pzi){
        this->seed = pseed;
        this->vertexInterval = pvertexInterval;
        this->gridSize = pgridSize;
        this->xi = pxi;
        this->zi = pzi;
        this->pNoise.reseed(seed);
        this->auxOctaves = 8;
        this->auxFreq = 32;
        this->vertices.reserve(this->gridSize * this->gridSize);
        this->indices.reserve(this->gridSize-1 * this->gridSize-1);
        this->genVectors();
        this->setUpGLBuffers();
    }
    
private:
    void genVectors(){
        float maxHTest = -10.0f, minHTest= 100.0f, auxHColor;
        Vertex ijvertex;
        for(unsigned int i=0; i<this->gridSize; i++){
            for(unsigned int j=0; j<this->gridSize; j++){
                HPair heightaux = getHeightValue(this->xi + this->vertexInterval * i, this->zi + this->vertexInterval * j);
                ijvertex.Position = glm::vec3(this->vertexInterval * i, heightaux.FinalH, this->vertexInterval * j);
                auxHColor = glm::clamp(heightaux.BaseH/2.0f, 0.0f, 1.0f);
                ijvertex.Color = glm::vec3(auxHColor, auxHColor, auxHColor);//DEBUG: preciso colocar uma cor descente
                this->vertices.push_back(ijvertex);
                
                if(heightaux.BaseH > maxHTest)
                    maxHTest = heightaux.BaseH;
                if(heightaux.BaseH < minHTest)
                    minHTest = heightaux.BaseH;
                
            }
        }
        
        printf("Maior valor Base: %f    Menor: %f\n", maxHTest, minHTest);
        
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
    
    HPair getHeightValue(float x, float z){
        double auxfxz = (double) this->gridSize/this->auxFreq;
        HPair r;
        r.BaseH = ((float) pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, this->auxOctaves)) + 1.0f;
        r.FinalH = (r.BaseH-1.0f)*10;
        return r;
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
        return glm::vec3(this->xi, 0.0f, this->zi);
    }
    
};

#endif