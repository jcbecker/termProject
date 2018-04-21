#ifndef TERRAICHUNK_HPP
#define TERRAICHUNK_HPP

#include <cstdio>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <VertexDescription.hpp>
#include <vector>
#include <PerlinNoise.hpp>
#include <BiomeDescription.hpp>

struct HVertexStatus{
    double BaseH;
    double FinalH;
    glm::vec3 ColorH;
    BiomeType BioTp;
};

struct BorderHelper{
    bool isBorder;
    bool borderAbove;
    double pDistToBorder;
};

class TerrainChunk{
private:
    unsigned int gridSize, seed;
    float vertexInterval;
    float xi, zi;//xf = xi + gridSize * vertexInterval, zf = zi + gridSize * vertexInterval;
    unsigned int VBO, EBO;
    siv::PerlinNoise pNoise;
    unsigned int biomeSize, biomeBorderLen;
    BiomeValues bvbuild[BIOMESN];
    
public:
    unsigned int VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    TerrainChunk(unsigned int pseed, float pvertexInterval, unsigned int pgridSize, float pxi, float pzi, unsigned int pBiosz, unsigned int pBioborderlen){
        this->seed = pseed;
        this->vertexInterval = pvertexInterval;
        this->gridSize = pgridSize;
        this->xi = pxi;
        this->zi = pzi;
        this->pNoise.reseed(seed);
        this->biomeSize = pBiosz;
        this->biomeBorderLen = pBioborderlen;
        this->describeTheBiomes();
        this->vertices.reserve(this->gridSize * this->gridSize);
        this->indices.reserve((this->gridSize-1) * (this->gridSize-1) * 6);
        this->genVectors();
        this->setUpGLBuffers();
    }
    
private:
    
    void describeTheBiomes(){
        bvbuild[0] = getTheBiomeDescription(PLAINS);
        bvbuild[1] = getTheBiomeDescription(MOUNTAINS);
        bvbuild[2] = getTheBiomeDescription(VALLEY);
        bvbuild[3] = getTheBiomeDescription(DESERT);
        bvbuild[4] = getTheBiomeDescription(CANYONS);
    }
    
    void genVectors(){
        Vertex ijvertex;
        HVertexStatus heightaux;
        for(unsigned int i=0; i<this->gridSize; i++){
            for(unsigned int j=0; j<this->gridSize; j++){
                heightaux = vertexValuation(this->xi + (float)i, this->zi + (float)j);
                ijvertex.Position = glm::vec3(this->vertexInterval * i, heightaux.FinalH, this->vertexInterval * j);
                ijvertex.Color = heightaux.ColorH;
                this->vertices.push_back(ijvertex);
                
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
        
        printf("vertices: %u\nindices: %u\n",this->vertices.size(), this->indices.size());
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
        HVertexStatus r;
        double actualH;
        glm::vec3 actualC;
        
        double oauxHX, oauxHZ;
        glm::vec3 oauxCX, oauxCZ;
        
        r.BioTp = getBiomeXZ(x, z);
        double auxfxz = (double) this->gridSize/bvbuild[r.BioTp].bFrequence;//bvbuild[r.BioTp]
        
        r.BaseH = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[r.BioTp].bOctaves);
        
        actualC = biomeColorValuation(r.BioTp, r.BaseH);
        
        actualH = biomeHeightValuation(r.BioTp, r.BaseH);
        
        BorderHelper someAuxforX = borderTest(x), someAuxforZ = borderTest(z);
        
        
        
        BiomeType otherSide_x, otherSide_z, otherSide_xz;
        double beyondTheWallBase_x, beyondTheWallFinal_x;
        double beyondTheWallBase_z, beyondTheWallFinal_z;
        double beyondTheWallBase_xz, beyondTheWallFinal_xz;
        
        
        r.FinalH = actualH;
        r.ColorH = actualC;
        
        if(someAuxforX.isBorder){
            if(someAuxforX.borderAbove){
                // The actual point(x, z), is border in the x axis
                otherSide_x = getBiomeXZ(x + this->biomeBorderLen, z);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_x].bFrequence;
                beyondTheWallBase_x = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_x].bOctaves);
                beyondTheWallFinal_x = biomeHeightValuation(otherSide_x, beyondTheWallBase_x);
                oauxHX = glm::mix(beyondTheWallFinal_x, actualH,   someAuxforX.pDistToBorder);
                r.FinalH = oauxHX;
                oauxCX = glm::mix(biomeColorValuation(otherSide_x, beyondTheWallBase_x), actualC,  someAuxforX.pDistToBorder);
                r.ColorH = oauxCX;
            }
            
        }
        
        if(someAuxforZ.isBorder){
            if(someAuxforZ.borderAbove){
                // The actual point(x, z), is border in the z axis
                otherSide_z = getBiomeXZ(x, z + this->biomeBorderLen);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_z].bFrequence;
                beyondTheWallBase_z = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_z].bOctaves);
                beyondTheWallFinal_z = biomeHeightValuation(otherSide_z, beyondTheWallBase_z);
                oauxHZ = glm::mix(beyondTheWallFinal_z, actualH,   someAuxforZ.pDistToBorder);
                r.FinalH = oauxHZ;
                oauxCZ = glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), actualC,  someAuxforZ.pDistToBorder);
                r.ColorH = oauxCZ;
            }
            
        }
        
        if(someAuxforX.isBorder && someAuxforZ.isBorder){
            if( someAuxforX.borderAbove && someAuxforZ.borderAbove){
                // The actual point(x, z), is border in the x and z axes
                otherSide_xz = getBiomeXZ(x + this->biomeBorderLen, z + this->biomeBorderLen);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_xz].bFrequence;
                beyondTheWallBase_xz = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_xz].bOctaves);
                beyondTheWallFinal_xz = biomeHeightValuation(otherSide_xz, beyondTheWallBase_xz);
                
                //Bilinear interpolation
                r.ColorH = glm::mix(biomeColorValuation(otherSide_xz, beyondTheWallBase_xz), biomeColorValuation(otherSide_x, beyondTheWallBase_x),  someAuxforZ.pDistToBorder);
                r.FinalH = glm::mix(beyondTheWallFinal_xz, beyondTheWallFinal_x,   someAuxforZ.pDistToBorder);
                
                
                r.ColorH = glm::mix(r.ColorH, oauxCZ, someAuxforX.pDistToBorder);
                r.FinalH = glm::mix(r.FinalH, oauxHZ, someAuxforX.pDistToBorder);
                
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
        
        /* to force four different adjacent biomes (worst case)
        int di, dj;
        di = dxs;
        dj = dzs;
        
        if(di == 0 && dj == 0){
            return DESERT;
        }else if(di == 0 && dj == 1){
            return MOUNTAINS;
        }else if(di == 1 && dj == 0){
            return VALLEY;
        }else if(di == 1 && dj == 1){
            return PLAINS;
        }
        */
        
        double areaValue = pNoise.noise0_1((dxs + 7.3)/0.5, (dzs+ 7.3)/0.5);
        areaValue = glm::clamp(areaValue, 0.0, 1.0);
        
        
        BiomeType toTest =(BiomeType) (areaValue * BIOMESN);
        
        
        return toTest;
    }
    
    glm::vec3 biomeColorValuation(BiomeType pbtval, double bNoise){
        double yaauxColor = glm::clamp(bNoise, -1.0, 1.0);
        if(bNoise < 0){
            return glm::mix(bvbuild[pbtval].midColor, bvbuild[pbtval].minColor, yaauxColor*-1);
            
        }else{
            return glm::mix(bvbuild[pbtval].midColor, bvbuild[pbtval].maxColor, yaauxColor);
        }    
    }
    
    double biomeHeightValuation(BiomeType pbtval, double bNoise){
        if(pbtval == PLAINS){
            bNoise *=10;
        }else if(pbtval == MOUNTAINS){
            
            
            bNoise = glm::pow(1.5, bNoise*7.0)*5;
        }else if (pbtval == VALLEY){
            bNoise = glm::clamp(bNoise, -0.5, 1.0);
            
            
            bNoise =glm::pow(bNoise * 5, 3);
            
        }else if(pbtval == DESERT){
            bNoise *=16;
            
        }else if(pbtval == CANYONS){
            double auxnn = glm::clamp(bNoise, -0.3, 0.3);
            
            bNoise = (glm::pow(1.5, auxnn*14.0)*5) + (bNoise * 8.0);
            
        }
        
        return bNoise;
        
    }
    
public:
    void DrawChunk(){
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