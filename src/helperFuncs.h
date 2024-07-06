/**
* @file helperFuncs.h 
* @author Tatiana Lipert 
* @date 2024-04-07 
* @brief helper functions for an Aftrburner module: 
*	convert from an Nvidia Physx 4x4 matrix to an aftrburner 4x4 matrix 
*	update function to cync the pose of a physx actor and WO 
* 
*/

#pragma once 

#include "foundation/PxMat44.h"
#include "foundation/PxVec4.h"
#include "Mat4.h"
#include "Vector.h" 
#include "WO.h"
#include "PxRigidDynamic.h"
using namespace Aftr; 
using namespace physx; 
using namespace std; 

PxMat44 aftrToPxMat4(Mat4 m) {
	PxMat44 t; 
	t.column0.x = m.at(0);
	t.column0.y = m.at(2);
	t.column0.z = m.at(1);
	t.column0.w = m.at(3);

	t.column1.x = m.at(4);
	t.column1.y = m.at(6);
	t.column1.z = m.at(5);
	t.column1.w = m.at(7);

	t.column2.x = m.at(8);
	t.column2.y = m.at(10);
	t.column2.z = m.at(9);
	t.column2.w = m.at(11);

	t.column3.x = m.at(12);
	t.column3.y = m.at(14);
	t.column3.z = m.at(13);
	t.column3.w = m.at(15);

	return t; 
}

Mat4 pxToAftrMat4(physx::PxMat44 m) {
	PxVec4T x = m.column0;
	PxVec4T y = m.column1;
	PxVec4T z = m.column2;
	PxVec4T w = m.column3;

	string s = to_string(x.x) + " " + to_string(x.z) + " " + to_string(x.y) + " " + to_string(x.w) + " " +
			   to_string(z.x) + " " + to_string(z.z) + " " + to_string(z.y) + " " + to_string(z.w) + " " +
			   to_string(y.x) + " " + to_string(y.z) + " " + to_string(y.y) + " " + to_string(y.w) + " " +
			   to_string(w.x) + " " + to_string(w.z) + " " + to_string(w.y) + " " + to_string(w.w);

	cout << s << endl << endl; 

	Mat4 t(s);
	return t;
}

void update(PxRigidDynamic* actor, WO* wo) {

	PxTransform t = actor->getGlobalPose();
	PxMat44 pxm = PxMat44(t);//convert transform to a matrix 
	PxVec4T pos = pxm.column3; //grab the last column for the position 

	Mat4 am = pxToAftrMat4(pxm);
	wo->setDisplayMatrix(am);
	wo->setPosition(Vector(pos.x, pos.z, pos.y));
}