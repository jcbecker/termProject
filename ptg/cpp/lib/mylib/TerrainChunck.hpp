#ifndef TERRAICHUNCK_HPP
#define TERRAICHUNCK_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <VertexDescription.hpp>
#include <vector>
#include <PerlinNoise.hpp>

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
        this->auxFreq = 16;
        this->vertices.reserve(this->gridSize * this->gridSize);
        this->genVectors();
        this->setUpGLBuffers();
    }
    
private:
    void genVectors(){
        for(unsigned int i=0; i<this->gridSize; i++){
            for(unsigned int j=0; j<this->gridSize; j++){
                Vertex avaux;
                float heightaux = getHeightValue(this->xi + this->vertexInterval * i, this->zi + this->vertexInterval * j);
                avaux.Position = glm::vec3(this->xi + this->vertexInterval * i, heightaux * 10.0f, this->zi + this->vertexInterval * j);
                avaux.Color = glm::vec3(heightaux/2.0f+0.5f, heightaux/2.0f+0.5f, heightaux/2.0f+0.5f);//DEBUG: preciso colocar uma cor descente
                this->vertices.push_back(avaux);
            }
        }
        
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
    
    float getHeightValue(float x, float z){
        double auxfxz = (double) this->gridSize/this->auxFreq;
        return (float) pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, this->auxOctaves);
        
    }
    
public:
    void DrawChunck(){
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    
    
    
};

#endif