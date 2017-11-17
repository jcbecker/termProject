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

class TerrainChunck{
private:
    unsigned int gridSize, seed;
    float vertexInterval;
    float xi, zi;//xf = xi + gridSize * vertexInterval, zf = zi + gridSize * vertexInterval;
    unsigned int VBO, EBO;
    siv::PerlinNoise pNoise;
    unsigned int biomeSize, biomeBorderLen;
    BiomeValues bvbuild[BIOMESN];
    
    double auxDebugmin, auxDebugmax;
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
        this->auxDebugmax = -15.0;
        this->auxDebugmin  =15.0;
        this->describeTheBiomes();
        this->vertices.reserve(this->gridSize * this->gridSize);
        this->indices.reserve(this->gridSize-1 * this->gridSize-1);
        this->genVectors();
        this->setUpGLBuffers();
    }
    
private:
    
    void describeTheBiomes(){
        bvbuild[0] = getTheBiomeDescription(PLAINS);
        bvbuild[1] = getTheBiomeDescription(MONTAINS);
        bvbuild[2] = getTheBiomeDescription(VALLEY);
        
        
        
        
        
    }
    
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
        printf("Maior Debug     : %lf    Menor: %lf\n", this->auxDebugmax, this->auxDebugmin);
        
        
        
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
        HVertexStatus r;
        double actualH;
        
        r.BioTp = getBiomeXZ(x, z);
        double auxfxz = (double) this->gridSize/bvbuild[r.BioTp].bFrequence;//bvbuild[r.BioTp]
        
        r.BaseH = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[r.BioTp].bOctaves);
        
        r.ColorH = biomeColorValuation(r.BioTp, r.BaseH);
        
        actualH = biomeHeightValuation(r.BioTp, r.BaseH);
        
        
        
        
        BorderHelper someAuxforX = borderTest(x), someAuxforZ = borderTest(z);
        
        if(someAuxforX.isBorder){
            if(someAuxforX.borderAbove){
                //r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), r.ColorH,  someAuxforX.pDistToBorder);
            }else{
                //r.ColorH = glm::mix(glm::vec3(1.0, 1.0, 1.0), r.ColorH,  someAuxforX.pDistToBorder);
            }
        }
        
        if(someAuxforZ.isBorder){
            if(someAuxforZ.borderAbove){
                //r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), r.ColorH, someAuxforZ.pDistToBorder);
            }else{
                //r.ColorH = glm::mix(glm::vec3(1.0, 1.0, 1.0), r.ColorH, someAuxforZ.pDistToBorder);
            }
        }
        
        BiomeType otherSide_x, otherSide_z;
        double beyondTheWallBase_x, beyondTheWallFinal_x;
        double beyondTheWallBase_z, beyondTheWallFinal_z;
        
        r.FinalH = actualH;
        
        if(someAuxforX.isBorder){
            if(someAuxforX.borderAbove){//black
                otherSide_x = getBiomeXZ(x + this->biomeBorderLen + 1, z);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_x].bFrequence;
                beyondTheWallBase_x = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_x].bOctaves);
                beyondTheWallFinal_x = biomeHeightValuation(otherSide_x, beyondTheWallBase_x);
                r.FinalH = glm::mix(beyondTheWallFinal_x, actualH,   someAuxforX.pDistToBorder);
                
                r.ColorH = glm::mix(biomeColorValuation(otherSide_x, beyondTheWallBase_x), r.ColorH,  someAuxforX.pDistToBorder);
                
                if(auxDebugmax < someAuxforX.pDistToBorder){
                    auxDebugmax = someAuxforX.pDistToBorder;
                }
                if(auxDebugmin > someAuxforX.pDistToBorder){
                    auxDebugmin = someAuxforX.pDistToBorder;
                    
                }
                
                
            }else{
                r.FinalH = actualH;
            }
            
        }
        if(someAuxforZ.isBorder){
            if(someAuxforZ.borderAbove){//black
                otherSide_z = getBiomeXZ(x, z + this->biomeBorderLen + 1);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_z].bFrequence;
                beyondTheWallBase_z = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_z].bOctaves);
                beyondTheWallFinal_z = biomeHeightValuation(otherSide_z, beyondTheWallBase_z);
                r.FinalH = glm::mix(beyondTheWallFinal_z, actualH,   someAuxforZ.pDistToBorder);
                
                r.ColorH = glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), r.ColorH,  someAuxforZ.pDistToBorder);
                
                if(auxDebugmax < someAuxforZ.pDistToBorder){
                    auxDebugmax = someAuxforZ.pDistToBorder;
                }
                if(auxDebugmin > someAuxforZ.pDistToBorder){
                    auxDebugmin = someAuxforZ.pDistToBorder;
                    
                }
                
                
            }else{
                r.FinalH = actualH;
            }
            
        }
        
        if(someAuxforX.isBorder && someAuxforZ.isBorder){
            if( someAuxforX.borderAbove && someAuxforZ.borderAbove){
                
                
                
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
    
    glm::vec3 biomeColorValuation(BiomeType pbtval, double bNoise){
        double yaauxColor = glm::clamp(bNoise, -1.0, 1.0);
        if(bNoise < 0){
            return glm::mix(bvbuild[pbtval].midColor, bvbuild[pbtval].minColor, yaauxColor*-1);
            
        }else{
            return glm::mix(bvbuild[pbtval].midColor, bvbuild[pbtval].maxColor, yaauxColor);
        }    
    }
    
    double biomeHeightValuation(BiomeType pbtval, double bNoise){
        return bNoise * bvbuild[pbtval].amplitudeH;//DEBUG: aqui chamar função extra
        
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