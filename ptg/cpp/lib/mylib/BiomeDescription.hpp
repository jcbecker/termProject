#ifndef BIOMEDESCRIPTION_HPP
#define BIOMEDESCRIPTION_HPP

#define BIOMESN 4


enum BiomeType : int{
PLAINS = 0, MONTAINS = 1, VALLEY = 2, DESERT = 3
    
};

struct BiomeValues{
    double bFrequence;
    int bOctaves;
    float amplitudeH;//maxHeight = minHeight + amplitudeH
    glm::vec3 minColor;
    glm::vec3 midColor;
    glm::vec3 maxColor;
    
};

glm::vec3 rgbTransform(glm::vec3 inrgb){
    return glm::vec3(inrgb.r/255.0, inrgb.g/255.0, inrgb.b/255.0);
}

BiomeValues getTheBiomeDescription(BiomeType pbt){
    /*
    descrip.bFrequence =
    descrip.bOctaves   =
    descrip.amplitudeH =
    descrip.minColor   = rgbTransform(glm::vec3());
    descrip.midColor   = rgbTransform(glm::vec3());
    descrip.maxColor   = rgbTransform(glm::vec3());
    
    */
    
    BiomeValues descrip;
    //--------------------------------------------------------------------------PLAINS
    if(pbt == PLAINS){
        descrip.bFrequence = 10.0;
        descrip.bOctaves   = 4;
        descrip.amplitudeH = 12;
        descrip.minColor   = rgbTransform(glm::vec3(34.0,139.0,34.0));
        descrip.midColor   = rgbTransform(glm::vec3(154.0,205.0,50.0));
        descrip.maxColor   = rgbTransform(glm::vec3(143.0,188.0,143.0));//(143,188,143)
        
    }else if(pbt == MONTAINS){//------------------------------------------------MONTAINS
        descrip.bFrequence = 18.0;
        descrip.bOctaves   = 8;
        descrip.amplitudeH = 10.0;
        descrip.minColor   = rgbTransform(glm::vec3(255.0,128.0,0.0));
        descrip.midColor   = rgbTransform(glm::vec3(255.0,128.0,144.0));
        descrip.maxColor   = rgbTransform(glm::vec3(255.0, 255.0, 255.0));
        
        
    }else if(pbt == VALLEY){//--------------------------------------------------VALLEY
        descrip.bFrequence = 8.0;
        descrip.bOctaves   = 8;
        descrip.amplitudeH = 36.0;
        descrip.minColor   = rgbTransform(glm::vec3(128.0, 128.0, 128.0));
        descrip.midColor   = rgbTransform(glm::vec3(112.0,128.0,144.0));
        descrip.maxColor   = rgbTransform(glm::vec3(255.0, 255.0, 255.0));
        
        
    }else if(pbt == DESERT){//--------------------------------------------------DESERT
        descrip.bFrequence = 6.0;
        descrip.bOctaves   = 2;
        descrip.amplitudeH = 25.0;
        descrip.minColor   = rgbTransform(glm::vec3(205.0, 133.0, 63.0));
        descrip.midColor   = rgbTransform(glm::vec3(244.0, 164.0, 96.0));
        descrip.maxColor   = rgbTransform(glm::vec3(205.0, 133.0, 63.0));
        
    }
    
    return descrip;
}



#endif