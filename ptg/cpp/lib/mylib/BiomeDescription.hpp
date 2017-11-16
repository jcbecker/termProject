#ifndef BIOMEDESCRIPTION_HPP
#define BIOMEDESCRIPTION_HPP

enum BiomeType{
PLAINS, MONTAINS    
    
};

struct BiomeValues{
    float minHeight;
    float rangeHeight;//maxHeight = minHeight + rangeHeight
    glm::vec3 minColor;
    glm::vec3 maxColor;
    
};


#endif