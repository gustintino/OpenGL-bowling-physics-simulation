#pragma once

#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <sstream> 

#include "glm/glm.hpp"
#include "Collider.h"

//GLuint modelLocation = 0;

class Collider;

struct vertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normals;
};

