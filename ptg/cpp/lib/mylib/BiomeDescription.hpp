#ifndef BIOMEDESCRIPTION_HPP
#define BIOMEDESCRIPTION_HPP

#define BIOMESN 5


enum BiomeType : int{
PLAINS = 0, MOUNTAINS = 1, VALLEY = 2, DESERT = 3, CANYONS = 4
    
};

struct BiomeValues{
    double bFrequence;
    int bOctaves;
    glm::vec3 minColor;
    glm::vec3 midColor;
    glm::vec3 maxColor;
    
};

glm::vec3 rgbTransform(glm::vec3 inrgb){
    return glm::vec3(inrgb.r/255.0, inrgb.g/255.0, inrgb.b/255.0);
}

BiomeValues getTheBiomeDescription(BiomeType pbt){
    
    
    BiomeValues descrip;
    //--------------------------------------------------------------------------PLAINS
    if(pbt == PLAINS){
        descrip.bFrequence = 10.0;
        descrip.bOctaves   = 4;
        descrip.minColor   = rgbTransform(glm::vec3(34.0,139.0,34.0));
        descrip.midColor   = rgbTransform(glm::vec3(154.0,205.0,50.0));
        descrip.maxColor   = rgbTransform(glm::vec3(143.0,188.0,143.0));//(143,188,143)
        
    }else if(pbt == MOUNTAINS){//------------------------------------------------MOUNTAINS
        descrip.bFrequence = 18.0;
        descrip.bOctaves   = 8;
        descrip.minColor   = rgbTransform(glm::vec3(128.0, 128.0, 0.0));
        descrip.midColor   = rgbTransform(glm::vec3(112.0, 128.0, 144.0));
        descrip.maxColor   = rgbTransform(glm::vec3(255.0, 255.0, 255.0));
        
        
    }else if(pbt == VALLEY){//--------------------------------------------------VALLEY
        descrip.bFrequence = 8.0;
        descrip.bOctaves   = 8;
        descrip.minColor   = rgbTransform(glm::vec3(30.0, 144.0, 255.0));
        descrip.midColor   = rgbTransform(glm::vec3(46.0, 139.0, 87.0));
        descrip.maxColor   = rgbTransform(glm::vec3(34.0, 139.0, 34.0));
        
        
    }else if(pbt == DESERT){//--------------------------------------------------DESERT
        descrip.bFrequence = 6.0;
        descrip.bOctaves   = 2;
        descrip.minColor   = rgbTransform(glm::vec3(0, 0, 0));
        descrip.midColor   = rgbTransform(glm::vec3(244.0, 164.0, 96.0));
        descrip.maxColor   = rgbTransform(glm::vec3(255, 255, 255));
        
    }else if(pbt == CANYONS){//-------------------------------------------------CANYONS
        descrip.bFrequence = 6.0;
        descrip.bOctaves   = 4;
        descrip.minColor   = rgbTransform(glm::vec3(0, 0, 0));
        descrip.midColor   = rgbTransform(glm::vec3(244.0, 164.0, 96.0));
        descrip.maxColor   = rgbTransform(glm::vec3(255, 255, 255));
        
        
    }
    
    return descrip;
}



#endif