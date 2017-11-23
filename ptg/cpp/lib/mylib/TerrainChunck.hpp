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
                ijvertex.Color = heightaux.ColorH;//DEBUG: preciso colocar uma cor descente
                this->vertices.push_back(ijvertex);
                
                
                if(i == 0 && j == 0){
                    printf("xs=%lf  zs=%lf\n",this->xi + (float)i, this->zi + (float)j);
                }
                if(i+1 == this->gridSize && j+1 == this->gridSize){
                    printf("xs=%lf  zs=%lf\n",this->xi + (float)i, this->zi + (float)j);
                }
            }
        }
        
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
        glm::vec3 actualC;
        
        double oauxHX, oauxHZ;
        glm::vec3 oauxCX, oauxCZ;
        
        
        
        r.BioTp = getBiomeXZ(x, z);
        double auxfxz = (double) this->gridSize/bvbuild[r.BioTp].bFrequence;//bvbuild[r.BioTp]
        
        r.BaseH = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[r.BioTp].bOctaves);
        
        actualC = biomeColorValuation(r.BioTp, r.BaseH);
        
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
        
        BiomeType otherSide_x, otherSide_z, otherSide_xz;
        double beyondTheWallBase_x, beyondTheWallFinal_x;
        double beyondTheWallBase_z, beyondTheWallFinal_z;
        double beyondTheWallBase_xz, beyondTheWallFinal_xz;
        
        
        r.FinalH = actualH;
        r.ColorH = actualC;
        
        if(someAuxforX.isBorder){
            if(someAuxforX.borderAbove){//black
                otherSide_x = getBiomeXZ(x + this->biomeBorderLen + 0.5, z);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_x].bFrequence;
                beyondTheWallBase_x = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_x].bOctaves);
                beyondTheWallFinal_x = biomeHeightValuation(otherSide_x, beyondTheWallBase_x);
                oauxHX = glm::mix(beyondTheWallFinal_x, actualH,   someAuxforX.pDistToBorder);
                r.FinalH = oauxHX;
                oauxCX = glm::mix(biomeColorValuation(otherSide_x, beyondTheWallBase_x), actualC,  someAuxforX.pDistToBorder);
                r.ColorH = oauxCX;
                
                //r.ColorH = glm::mix(glm::vec3(1.0, 0.0, 0.0), r.ColorH, someAuxforX.pDistToBorder);

                
                
            }else{
                //r.FinalH = actualH;
            }
            
        }
        if(someAuxforZ.isBorder){
            if(someAuxforZ.borderAbove){//black
                otherSide_z = getBiomeXZ(x, z + this->biomeBorderLen + 0.5);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_z].bFrequence;
                beyondTheWallBase_z = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_z].bOctaves);
                beyondTheWallFinal_z = biomeHeightValuation(otherSide_z, beyondTheWallBase_z);
                oauxHZ = glm::mix(beyondTheWallFinal_z, actualH,   someAuxforZ.pDistToBorder);
                r.FinalH = oauxHZ;
                oauxCZ = glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), actualC,  someAuxforZ.pDistToBorder);
                r.ColorH = oauxCZ;
                
                
                //r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 1.0), r.ColorH, someAuxforZ.pDistToBorder);

                
                
            }else{
                //r.FinalH = actualH;
            }
            
        }
        
        if(someAuxforX.isBorder && someAuxforZ.isBorder){
            if( someAuxforX.borderAbove && someAuxforZ.borderAbove){
                //r.FinalH = (glm::mix(beyondTheWallFinal_z, actualH,   someAuxforZ.pDistToBorder) + glm::mix(beyondTheWallFinal_x, actualH,   someAuxforX.pDistToBorder)) * 0.5;
                //r.ColorH = glm::mix(glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), r.ColorH,  someAuxforZ.pDistToBorder), glm::mix(biomeColorValuation(otherSide_x, beyondTheWallBase_x), r.ColorH,  someAuxforX.pDistToBorder), 0.5);
                //r.FinalH = glm::mix(r.FinalH, actualH, 1 - ((someAuxforZ.pDistToBorder + someAuxforX.pDistToBorder) * 0.5));
                //r.FinalH = actualH;
                //r.ColorH = glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), r.ColorH,  someAuxforZ.pDistToBorder);
                //r.ColorH = glm::mix(r.ColorH, glm::vec3(0.0, 0.0, 0.0), (someAuxforZ.pDistToBorder + someAuxforX.pDistToBorder) * 0.5);
                //r.ColorH = biomeColorValuation(r.BioTp, r.BaseH);
                
                otherSide_xz = getBiomeXZ(x + this->biomeBorderLen + 0.5, z + this->biomeBorderLen + 0.5);
                auxfxz = (double) this->gridSize/bvbuild[otherSide_xz].bFrequence;
                beyondTheWallBase_xz = pNoise.octaveNoise((double)x/auxfxz, (double)z/auxfxz, bvbuild[otherSide_xz].bOctaves);
                beyondTheWallFinal_xz = biomeHeightValuation(otherSide_xz, beyondTheWallBase_xz);
                
                
                
                if(someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder  < 1){
                    r.FinalH = beyondTheWallFinal_xz;
                    
                    r.ColorH = biomeColorValuation(otherSide_xz, beyondTheWallBase_xz);
                    //r.ColorH = glm::mix(actualC , biomeColorValuation(otherSide_xz, beyondTheWallBase_xz), glm::clamp(((1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) +  (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)), 0.0, 1.0));
                    
                }else{
                    r.FinalH = actualH;
                    
                    r.ColorH = actualC;
                    
                    
                    
                    r.ColorH = glm::mix(biomeColorValuation(otherSide_xz, beyondTheWallBase_xz) , actualC, glm::clamp((someAuxforX.pDistToBorder * someAuxforX.pDistToBorder * someAuxforX.pDistToBorder * someAuxforX.pDistToBorder +  someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder), 0.0, 1.0));
                    r.FinalH = glm::mix(beyondTheWallFinal_xz , actualH, glm::clamp((someAuxforX.pDistToBorder * someAuxforX.pDistToBorder * someAuxforX.pDistToBorder * someAuxforX.pDistToBorder +  someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder), 0.0, 1.0));
                    
                }
                
                
                
                
                //(+x, +z)
                //r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), actualC, glm::clamp(glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder), 0.0, 1.0));
                
                
                
                if(glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)) < 1){
                    //r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), actualC, glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)));
                    r.ColorH = glm::mix(biomeColorValuation(otherSide_x, beyondTheWallBase_x), r.ColorH, glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)));
                    r.FinalH = glm::mix(beyondTheWallFinal_x, r.FinalH, glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)));
                    
                }
                
                if(glm::sqrt((1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder) < 1){
                    r.ColorH = glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), r.ColorH, glm::sqrt((1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder));
                    r.FinalH = glm::mix(beyondTheWallFinal_z, r.FinalH, glm::sqrt((1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder));
                    
                }
                
                /*
                if(glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder) < 1){
                    r.ColorH = glm::mix(biomeColorValuation(otherSide_xz, beyondTheWallBase_xz), r.ColorH, glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder));
                    r.FinalH = glm::mix(beyondTheWallFinal_xz, r.FinalH, glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder));
                    
                }
                */
                
                
                //Color debug
                /*
                if(glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder) < 1
                &&
                glm::sqrt((1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) + (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)) < 1){
                    r.ColorH = glm::mix(glm::vec3(0.0, 0.0, 0.0), r.ColorH, 0.5);
                                        
                }
                */
                //r.ColorH = glm::mix(r.ColorH , glm::vec3(0.0, 0.0, 0.0), glm::clamp((someAuxforX.pDistToBorder * someAuxforX.pDistToBorder * someAuxforX.pDistToBorder * someAuxforX.pDistToBorder +  someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder), 0.0, 1.0));
                //r.ColorH = glm::mix(r.ColorH , glm::vec3(1.0, 1.0, 1.0), glm::clamp(((1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) * (1-someAuxforX.pDistToBorder) +  (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder) * (1-someAuxforZ.pDistToBorder)), 0.0, 1.0));
                //r.ColorH = glm::mix(r.ColorH , glm::vec3(0.0, 0.0, 0.0), glm::clamp((someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder), 0.0, 1.0));
                
                
                
                
                //r.FinalH = (glm::mix(beyondTheWallFinal_xz, actualH, glm::clamp(glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder), 0.0, 1.0))
                //+ glm::mix(beyondTheWallFinal_z, actualH,   someAuxforZ.pDistToBorder)
                //+ glm::mix(beyondTheWallFinal_x, actualH,   someAuxforX.pDistToBorder))/3.0;
                
                //r.ColorH = glm::mix(biomeColorValuation(otherSide_xz, beyondTheWallBase_xz), r.ColorH,  glm::clamp(glm::sqrt(someAuxforX.pDistToBorder * someAuxforX.pDistToBorder + someAuxforZ.pDistToBorder * someAuxforZ.pDistToBorder), 0.0, 1.0));
                //+ glm::mix(biomeColorValuation(otherSide_z, beyondTheWallBase_z), r.ColorH,  someAuxforZ.pDistToBorder)
                //+ glm::mix(biomeColorValuation(otherSide_x, beyondTheWallBase_x), r.ColorH,  someAuxforX.pDistToBorder);
                
                //r.ColorH.r /= 2.0;
                //r.ColorH.g /= 2.0;
                //r.ColorH.b /= 2.0;
                
                //r.ColorH = glm::mix(r.ColorH, glm::vec3(0.0, 0.0, 0.0), 0.5);
                
                
                
                
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
        
        
        
        
        
        
        
        
        double areaValue = pNoise.noise0_1((dxs + 7.3) * 2.0, (dzs+ 7.3) * 2.0);
        areaValue = glm::clamp(areaValue, 0.0, 1.0);
        
        /*
        if(this->auxDebugmax < areaValue){
            this->auxDebugmax = areaValue;
        }
        if(this->auxDebugmin > areaValue){
            this->auxDebugmin = areaValue;
        }
        */
        
        BiomeType toTest =(BiomeType) (areaValue * BIOMESN);
        if(this->auxDebugmax < toTest){
            this->auxDebugmax = toTest;
        }
        if(this->auxDebugmin > toTest){
            this->auxDebugmin = toTest;
        }
        
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
        }else if(pbtval == MONTAINS){
            
            
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
        
        
        
        return bNoise;//DEBUG: aqui chamar função extra
        
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