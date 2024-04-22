/*
* @file vectorToVec3df.h
* @author Tatiana Lipert 
* @date 2024-02-20
* @brief simple helper function to convert a vector to a vec3df
*/

#include <irrKlang.h>
#include "Vector.h"

using namespace irrklang;
using namespace Aftr; 

vec3df vectorToVec3df(Vector v) {
	return vec3df(v.y, v.x, v.z);
}