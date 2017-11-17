#ifndef BIOMEDESCRIPTION_HPP
#define BIOMEDESCRIPTION_HPP

#define BIOMESN 3

enum BiomeType{
PLAINS, MONTAINS, VALLEY    
    
};

struct BiomeValues{
    float minHeight;
    float rangeHeight;//maxHeight = minHeight + rangeHeight
    glm::vec3 minColor;
    glm::vec3 maxColor;
    
};


#endif